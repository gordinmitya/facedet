#ifndef FACEDET_FACEDETECTOR_H
#define FACEDET_FACEDETECTOR_H

#include <memory>
#include <vector>

#include "opencv2/core.hpp"
#include "ncnn/net.h"

#include "types.h"


class FaceDetector {
public:
    explicit FaceDetector(std::unique_ptr<ncnn::Net> net) : net(std::move(net)) {}

    std::vector<Face> run(cv::Mat &);

private:
    std::unique_ptr<ncnn::Net> net;
    const int max_side = 320;
    const float _nms = 0.4f;
    const float _threshold = 0.6f;
    const float mean[3] = {104.f, 117.f, 123.f};
};


#endif //FACEDET_FACEDETECTOR_H
