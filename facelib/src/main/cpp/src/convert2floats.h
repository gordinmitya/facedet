#ifndef FACEDET_CONVERT2FLOATS_H
#define FACEDET_CONVERT2FLOATS_H

#include <vector>
#include <jni.h>
#include "types.h"

inline int flattenPoint(const Point &pt, jfloat *arr, int offset) {
    arr[offset++] = pt.x;
    arr[offset++] = pt.y;
    return offset;
}

inline int flattenBBox(const Box &bbox, jfloat *arr, int offset) {
    arr[offset++] = bbox.x1;
    arr[offset++] = bbox.y1;
    arr[offset++] = bbox.x2;
    arr[offset++] = bbox.y2;
    return offset;
}

inline int flattenLandmarks(const Landmarks &lm, jfloat *arr, int offset) {
    offset = flattenPoint(lm.left_eye, arr, offset);
    offset = flattenPoint(lm.right_eye, arr, offset);
    offset = flattenPoint(lm.nose, arr, offset);
    offset = flattenPoint(lm.left_mouth, arr, offset);
    offset = flattenPoint(lm.right_mouth, arr, offset);
    return offset;
}

inline int flattenFace(const Face &face, jfloat *arr, int offset) {
    offset = flattenBBox(face.bbox, arr, offset);
    offset = flattenLandmarks(face.landmarks, arr, offset);
    arr[offset++] = face.score;
    return offset;
}

inline jint convertFacesToFloats(
        JNIEnv *env,
        const std::vector<Face> &faces,
        jfloatArray javaArray
) {
    const int face_size = 2 + 2 + 5 * 2 + 1; // x1, y1, x2, y2, 5 points (x, y) and score

    if (!javaArray) return -1;
    const jsize len = env->GetArrayLength(javaArray);
    if (len < face_size || len % face_size != 0) {
        return -1;
    }
    const int topK = std::min(len / face_size, (jint) faces.size());
    jfloat *arr = env->GetFloatArrayElements(javaArray, nullptr);
    if (!arr) return -1;

    int offset = 0;
    for (int i = 0; i < topK; ++i) {
        offset = flattenFace(faces[i], arr, offset);
    }

    env->ReleaseFloatArrayElements(javaArray, arr, JNI_COMMIT);

    return topK;
}

#endif //FACEDET_CONVERT2FLOATS_H
