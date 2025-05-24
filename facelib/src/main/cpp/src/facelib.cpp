#include <jni.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <string>
#include "ncnn/net.h"

#include "types.h"
#include "convert2java.h"
#include "convert2floats.h"

typedef struct DetectorHandle {
    ncnn::Net net;
} DetectorHandle;

extern "C" JNIEXPORT jlong
JNICALL Java_me_gordinmitya_ds_facelib_Detector_createNet(
        JNIEnv *env,
        jclass,
        jobject assetManager,
        jstring model_path_prefix
) {
    const auto g_assetManager = AAssetManager_fromJava(env, assetManager);
    if (!g_assetManager) return {};

    const auto handle = new DetectorHandle{
            ncnn::Net(),
    };

    const char *c_path = env->GetStringUTFChars(model_path_prefix, nullptr);
    std::string path(c_path);
    env->ReleaseStringUTFChars(model_path_prefix, c_path);

    auto asset = AAssetManager_open(g_assetManager, (path + ".param").c_str(), AASSET_MODE_BUFFER);
    if (!asset) {
        delete handle;
        return 0;
    }
    handle->net.load_param(asset);
    AAsset_close(asset);

    asset = AAssetManager_open(g_assetManager, (path + ".bin").c_str(), AASSET_MODE_BUFFER);
    if (!asset) {
        delete handle;
        return 0;
    }
    handle->net.load_model(asset);
    AAsset_close(asset);

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
        jobject image
) {
    auto handle = reinterpret_cast<DetectorHandle *>(ptr);
    if (!handle) return nullptr;

    std::vector<Face> detection;
    return convertFacesToJavaArray(env, detection);
}

extern "C" JNIEXPORT jint
JNICALL Java_me_gordinmitya_ds_facelib_Detector_detect2array(
        JNIEnv *env,
        jclass,
        jlong ptr,
        jobject image,
        jfloatArray detections_array
) {
    auto handle = reinterpret_cast<DetectorHandle *>(ptr);
    if (!handle) return 0;
    std::vector<Face> detection;

    jint topK = convertFacesToFloats(env, detection, detections_array);
    return topK;
}
