#include "FaceDetector.h"
#include "opencv2/imgproc.hpp"

std::vector<Box> create_anchors(const int w, const int h) {
    std::vector<Box> anchors;
    const float steps[] = {8, 16, 32, 64};
    std::vector<std::vector<int>> feature_map(4);
    for (int i = 0; i < feature_map.size(); ++i) {
        feature_map[i].push_back(static_cast<int>(ceil(h / steps[i])));
        feature_map[i].push_back(static_cast<int>(ceil(w / steps[i])));
    }
    const std::vector<int> min_sizes[4] = {
            {10,  16,  24},
            {32,  48},
            {64,  96},
            {128, 192, 256}
    };

    for (int k = 0; k < feature_map.size(); ++k) {
        const auto &min_size = min_sizes[k];
        for (int i = 0; i < feature_map[k][0]; ++i) {
            for (int j = 0; j < feature_map[k][1]; ++j) {
                for (int l: min_size) {
                    float s_kx = l * 1.0f / w;
                    float s_ky = l * 1.0f / h;
                    float cx = (j + 0.5f) * steps[k] / w;
                    float cy = (i + 0.5f) * steps[k] / h;
                    Box axil = {cx, cy, s_kx, s_ky};
                    anchors.push_back(axil);
                }
            }
        }
    }

    return anchors;
}

float iou(const Box &box1, const Box &box2) {
    float w = fmax(0.0f, fmin(box1.x2, box2.x2) - fmax(box1.x1, box2.x1));
    float h = fmax(0.0f, fmin(box1.y2, box2.y2) - fmax(box1.y1, box2.y1));

    float i = w * h;
    float u = (box1.x2 - box1.x1) * (box1.y2 - box1.y1)
              + (box2.x2 - box2.x1) * (box2.y2 - box2.y1)
              - i;

    if (u <= 0.0) return 0.0f;
    else return i / u;
}

void nms(std::vector<Face> &faces, float NMS_THRESH) {
    std::vector<float> vArea(faces.size());
    for (size_t i = 0; i < faces.size(); ++i) {
        vArea[i] = (faces.at(i).bbox.x2 - faces.at(i).bbox.x1 + 1)
                   * (faces.at(i).bbox.y2 - faces.at(i).bbox.y1 + 1);
    }
    for (size_t i = 0; i < faces.size(); ++i) {
        for (size_t j = i + 1; j < faces.size();) {
            if (iou(faces[i].bbox, faces[j].bbox) >= NMS_THRESH) {
                faces.erase(faces.begin() + j);
                vArea.erase(vArea.begin() + j);
            } else {
                j++;
            }
        }
    }
}

inline bool cmp(const Face &a, const Face &b) {
    return a.score > b.score;
}

std::vector<Face> FaceDetector::run(cv::Mat &bgr) {
    float long_side = 1.f * std::max(bgr.cols, bgr.rows);
    float scale = max_side / long_side;
    cv::Mat bgr_scale;
    cv::resize(bgr, bgr_scale, cv::Size(), scale, scale);

    ncnn::Mat in = ncnn::Mat::from_pixels_resize(bgr_scale.data, ncnn::Mat::PIXEL_BGR,
                                                 bgr_scale.cols, bgr_scale.rows,
                                                 bgr_scale.cols, bgr_scale.rows);
    in.substract_mean_normalize(mean, nullptr);
    ncnn::Extractor ex = net->create_extractor();
    ex.input(0, in);
    ncnn::Mat locOut, classOut, lmsOut;
//    ex.extract("out0", locOut);
//    ex.extract("out1", classOut);
//    ex.extract("out2", lmsOut);
    ex.extract("output0", locOut);
    ex.extract("530", classOut);
    ex.extract("529", lmsOut);

    std::vector<Box> anchors = create_anchors(in.w, in.h);
    std::vector<Face> total_box;

    const float *locPtr = locOut.channel(0);
    const float *classPtr = classOut.channel(0);
    const float *lmsPtr = lmsOut.channel(0);

    for (auto anchor: anchors) {
        const float score = *(classPtr + 1);
        if (score > _threshold) {
            Box tmp;
            tmp.x1 = anchor.x1 + *locPtr * 0.1f * anchor.x2;
            tmp.y1 = anchor.y1 + *(locPtr + 1) * 0.1f * anchor.y2;
            tmp.x2 = anchor.x2 * exp(*(locPtr + 2) * 0.2f);
            tmp.y2 = anchor.y2 * exp(*(locPtr + 3) * 0.2f);

            Box f_box;
            f_box.x1 = (tmp.x1 - tmp.x2 / 2);
            if (f_box.x1 < 0)
                f_box.x1 = 0;
            f_box.y1 = (tmp.y1 - tmp.y2 / 2);
            if (f_box.y1 < 0)
                f_box.y1 = 0;
            f_box.x2 = (tmp.x1 + tmp.x2 / 2);
            if (f_box.x2 > 1)
                f_box.x2 = 1;
            f_box.y2 = (tmp.y1 + tmp.y2 / 2);
            if (f_box.y2 > 1)
                f_box.y2 = 1;

            Point points[5];
            for (int j = 0; j < 5; ++j) {
                points[j].x = anchor.x1 + *(lmsPtr + j * 2) * 0.1f * anchor.x2;
                points[j].y = anchor.y1 + *(lmsPtr + j * 2 + 1) * 0.1f * anchor.y2;
            }

            Face result = {
                    .bbox = f_box,
                    .landmarks = {
                            .left_eye = points[0],
                            .right_eye = points[1],
                            .nose = points[2],
                            .left_mouth = points[3],
                            .right_mouth = points[4]
                    },
                    .score = score,
            };

            total_box.push_back(result);
        }
        locPtr += 4;
        classPtr += 2;
        lmsPtr += 10;
    }

    std::sort(total_box.begin(), total_box.end(), cmp);
    nms(total_box, _nms);

    return total_box;
}
