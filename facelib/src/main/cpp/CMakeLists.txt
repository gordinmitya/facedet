cmake_minimum_required(VERSION 3.22.1)

project("facelib")

file(GLOB_RECURSE sources "src/*.cpp")
add_library(${CMAKE_PROJECT_NAME} SHARED
        ${sources})

# set the path to the OpenCV cmake config
set(OpenCV_DIR ${CMAKE_SOURCE_DIR}/vendor/opencv/sdk/native/jni)
find_package(OpenCV REQUIRED)

# set the path to ncnn cmake config
set(ncnn_DIR ${CMAKE_SOURCE_DIR}/vendor/ncnn/${ANDROID_ABI}/lib/cmake/ncnn)
find_package(ncnn REQUIRED)

# add include directories to our target
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        ${OpenCV_DIR}/include
        ${CMAKE_SOURCE_DIR}/vendor/ncnn/${ANDROID_ABI}/include)

# link libraries to our target
target_link_libraries(${CMAKE_PROJECT_NAME}
        ${OpenCV_LIBS} # add all OpenCV libraries
        # or add only necessary eg: opencv_core
        ncnn
        android
        log)
