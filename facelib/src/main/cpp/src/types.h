#ifndef TYPES_H
#define TYPES_H

#include <vector>

typedef struct {
    float x;
    float y;
} Point;

typedef struct {
    float x1, y1, x2, y2;
} Box;

typedef struct {
    Point left_eye;
    Point right_eye;
    Point nose;
    Point left_mouth;
    Point right_mouth;
} Landmarks;

typedef struct {
    Box bbox;
    Landmarks landmarks;
    float score; // Confidence score
} Face;

typedef struct {
    std::vector<Face> faces;
} DetectionResult;

#endif // TYPES_H
