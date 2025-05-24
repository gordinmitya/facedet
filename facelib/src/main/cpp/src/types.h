#ifndef TYPES_H
#define TYPES_H
#include <vector>

typedef struct {
    float x;
    float y;
} Point;

typedef struct {
    Point top_left;
    Point bottom_right;
} BBox;

typedef struct {
    Point left_eye;
    Point right_eye;
    Point nose;
    Point left_mouth;
    Point right_mouth;
} Landmarks;

typedef struct {
    BBox bbox;
    Landmarks landmarks;
    float score; // Confidence score
} Face;

typedef struct {
    std::vector<Face> faces;
} DetectionResult;

#endif // TYPES_H
