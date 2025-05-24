#ifndef FACEDET_CONVERT2JAVA_H
#define FACEDET_CONVERT2JAVA_H

#include <jni.h>
#include "types.h"

namespace facelib_jni {
    constexpr const char *POINT_CLS = "me/gordinmitya/ds/facelib/Point";
    constexpr const char *RECT_CLS = "me/gordinmitya/ds/facelib/Rect";
    constexpr const char *LANDMARKS_CLS = "me/gordinmitya/ds/facelib/Landmarks";
    constexpr const char *FACE_CLS = "me/gordinmitya/ds/facelib/Face";
    constexpr const char *POINT_CTOR = "(FF)V";
    constexpr const char *RECT_CTOR = "(Lme/gordinmitya/ds/facelib/Point;Lme/gordinmitya/ds/facelib/Point;)V";
    constexpr const char *LANDMARKS_CTOR = "(Lme/gordinmitya/ds/facelib/Point;Lme/gordinmitya/ds/facelib/Point;Lme/gordinmitya/ds/facelib/Point;Lme/gordinmitya/ds/facelib/Point;Lme/gordinmitya/ds/facelib/Point;)V";
    constexpr const char *FACE_CTOR = "(Lme/gordinmitya/ds/facelib/Rect;Lme/gordinmitya/ds/facelib/Landmarks;F)V";
}

inline jobject createJavaPoint(JNIEnv *env, const Point &pt) {
    jclass cls = env->FindClass(facelib_jni::POINT_CLS);
    jmethodID ctor = env->GetMethodID(cls, "<init>", facelib_jni::POINT_CTOR);
    return env->NewObject(cls, ctor, pt.x, pt.y);
}

inline jobject createJavaRect(JNIEnv *env, const BBox &bbox) {
    jclass cls = env->FindClass(facelib_jni::RECT_CLS);
    jmethodID ctor = env->GetMethodID(cls, "<init>", facelib_jni::RECT_CTOR);
    jobject topLeft = createJavaPoint(env, bbox.top_left);
    jobject bottomRight = createJavaPoint(env, bbox.bottom_right);
    jobject rect = env->NewObject(cls, ctor, topLeft, bottomRight);
    env->DeleteLocalRef(topLeft);
    env->DeleteLocalRef(bottomRight);
    return rect;
}

inline jobject createJavaLandmarks(JNIEnv *env, const Landmarks &lm) {
    jclass cls = env->FindClass(facelib_jni::LANDMARKS_CLS);
    jmethodID ctor = env->GetMethodID(cls, "<init>", facelib_jni::LANDMARKS_CTOR);
    jobject leftEye = createJavaPoint(env, lm.left_eye);
    jobject rightEye = createJavaPoint(env, lm.right_eye);
    jobject nose = createJavaPoint(env, lm.nose);
    jobject leftMouth = createJavaPoint(env, lm.left_mouth);
    jobject rightMouth = createJavaPoint(env, lm.right_mouth);
    jobject landmarks = env->NewObject(cls, ctor, leftEye, rightEye, nose, leftMouth, rightMouth);
    env->DeleteLocalRef(leftEye);
    env->DeleteLocalRef(rightEye);
    env->DeleteLocalRef(nose);
    env->DeleteLocalRef(leftMouth);
    env->DeleteLocalRef(rightMouth);
    return landmarks;
}

inline jobject createJavaFace(JNIEnv *env, const Face &face) {
    jclass cls = env->FindClass(facelib_jni::FACE_CLS);
    jmethodID ctor = env->GetMethodID(cls, "<init>", facelib_jni::FACE_CTOR);
    jobject rect = createJavaRect(env, face.bbox);
    jobject landmarks = createJavaLandmarks(env, face.landmarks);
    jobject faceObj = env->NewObject(cls, ctor, rect, landmarks, face.score);
    env->DeleteLocalRef(rect);
    env->DeleteLocalRef(landmarks);
    return faceObj;
}

inline jobjectArray convertFacesToJavaArray(JNIEnv *env, const std::vector<Face> &faces) {
    jclass faceCls = env->FindClass(facelib_jni::FACE_CLS);
    jobjectArray array = env->NewObjectArray((jsize) faces.size(), faceCls, nullptr);
    for (jsize i = 0; i < faces.size(); ++i) {
        jobject faceObj = createJavaFace(env, faces[i]);
        env->SetObjectArrayElement(array, i, faceObj);
        env->DeleteLocalRef(faceObj);
    }
    return array;
}

#endif //FACEDET_CONVERT2JAVA_H
