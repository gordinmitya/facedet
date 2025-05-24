#!/bin/bash
set -e

VENDOR_DIR="facelib/src/main/cpp/vendor"
mkdir -p ${VENDOR_DIR}

OPENCV_VERSION="4.11.0"
OPENCV_SHA256="18223ab83d8ecc2d9d123ecba52bb0a35f6f76b22c7f41ce9b9b5b45a880592b"
curl -L -o opencv-mobile.zip https://github.com/nihui/opencv-mobile/releases/latest/download/opencv-mobile-${OPENCV_VERSION}-android.zip
echo "${OPENCV_SHA256}  opencv-mobile.zip" | sha256sum -c -
if [ $? -ne 0 ]; then
    exit 1
fi
unzip -o opencv-mobile.zip -d ${VENDOR_DIR}
mv ${VENDOR_DIR}/opencv-mobile-${OPENCV_VERSION}-android ${VENDOR_DIR}/opencv
rm opencv-mobile.zip

NCNN_VERSION="20250503"
NCNN_SHA256="4b54cd11094dae1371c59fb202dfd92d22962ffaef04317bdf99b12c09f44ed3"
curl -L -o ncnn.zip https://github.com/Tencent/ncnn/releases/latest/download/ncnn-${NCNN_VERSION}-android-vulkan.zip
echo "${NCNN_SHA256}  ncnn.zip" | sha256sum -c -
if [ $? -ne 0 ]; then
    exit 1
fi
unzip -o ncnn.zip -d ${VENDOR_DIR}
rm ncnn.zip
mv ${VENDOR_DIR}/ncnn-${NCNN_VERSION}-android-vulkan ${VENDOR_DIR}/ncnn
