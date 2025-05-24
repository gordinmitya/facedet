#include <jni.h>
#include <android/asset_manager.h>
#include <android/bitmap.h>
#include <android/asset_manager_jni.h>
#include <string>

#include "opencv2/imgproc.hpp"

#include "types.h"
#include "FaceDetector.h"
#include "convert2java.h"
#include "convert2floats.h"

typedef struct DetectorHandle {
    FaceDetector detector;
} DetectorHandle;

bool convertBitmapToMat(JNIEnv *pEnv, jobject bitmap, cv::Mat &dst) {
    AndroidBitmapInfo info;
    void *pixels = nullptr;
    if (AndroidBitmap_getInfo(pEnv, bitmap, &info) < 0) {
        return false;
    }
    if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        return false;
    }
    if (AndroidBitmap_lockPixels(pEnv, bitmap, &pixels) < 0) {
        return false;
    }

    dst.create(info.height, info.width, CV_8UC4);
    memcpy(dst.data, pixels, info.height * info.width * 4);

    AndroidBitmap_unlockPixels(pEnv, bitmap);

    cv::cvtColor(dst, dst, cv::COLOR_RGBA2BGR);

    return true;
}

extern "C" JNIEXPORT jlong
JNICALL Java_me_gordinmitya_ds_facelib_Detector_createNet(
        JNIEnv *env,
        jclass,
        jobject assets,
        jstring model_path_prefix
) {
    const auto assetManager = AAssetManager_fromJava(env, assets);
    if (!assetManager) return {};

    const char *c_path = env->GetStringUTFChars(model_path_prefix, nullptr);
    std::string path(c_path);
    env->ReleaseStringUTFChars(model_path_prefix, c_path);

    auto net = std::make_unique<ncnn::Net>();
    net->opt.num_threads = 4;
    net->opt.lightmode = true;

    if (net->load_param(assetManager, (path + ".param").c_str()) != 0) {
        return 0;
    }
    if (net->load_model(assetManager, (path + ".bin").c_str()) != 0) {
        return 0;
    }

    const auto handle = new DetectorHandle{
            FaceDetector(std::move(net))
    };
    return reinterpret_cast<jlong>(handle);
}

extern "C" JNIEXPORT void
JNICALL Java_me_gordinmitya_ds_facelib_Detector_destroyNet(
        JNIEnv *env,
        jclass,
        jlong ptr
) {
    delete reinterpret_cast<DetectorHandle *>(ptr);
}

extern "C" JNIEXPORT jobjectArray
JNICALL Java_me_gordinmitya_ds_facelib_Detector_detect2objects(
        JNIEnv *env,
        jclass,
        jlong ptr,
        jobject bitmap
) {
    auto handle = reinterpret_cast<DetectorHandle *>(ptr);
    if (!handle) return nullptr;

    cv::Mat bgr;
    if (!convertBitmapToMat(env, bitmap, bgr)) {
        return nullptr;
    }

    std::vector<Face> detection = handle->detector.run(bgr);
    return convertFacesToJavaArray(env, detection);
}

extern "C" JNIEXPORT jint
JNICALL Java_me_gordinmitya_ds_facelib_Detector_detect2array(
        JNIEnv *env,
        jclass,
        jlong ptr,
        jobject bitmap,
        jfloatArray detections_array
) {
    auto handle = reinterpret_cast<DetectorHandle *>(ptr);
    if (!handle) return 0;
    std::vector<Face> detection;

    jint topK = convertFacesToFloats(env, detection, detections_array);
    return topK;
}
