package me.gordinmitya.ds.facelib

class Point(
    val x: Float,
    val y: Float
)

class Rect(
    val x1: Float,
    val y1: Float,
    val x2: Float,
    val y2: Float
)

class Landmarks(
    val leftEye: Point,
    val rightEye: Point,
    val nose: Point,
    val leftMouthCorner: Point,
    val rightMouthCorner: Point
)

class Face(
    val rect: Rect,
    val landmarks: Landmarks,
    val score: Float
)

// x1, y1, x2, y2, 5 points (x, y) and score
internal const val FACE_SIZE_FLAT = 2 + 2 + 5 * 2 + 1

fun floatsToFace(
    floats: FloatArray,
    faceNumber: Int = 0
): Face {
    val offset = faceNumber * FACE_SIZE_FLAT

    val x1 = floats[offset + 0]
    val y1 = floats[offset + 1]
    val x2 = floats[offset + 2]
    val y2 = floats[offset + 3]

    val leftEye = Point(floats[offset + 4], floats[offset + 5])
    val rightEye = Point(floats[offset + 6], floats[offset + 7])
    val nose = Point(floats[offset + 8], floats[offset + 9])
    val leftMouthCorner = Point(floats[offset + 10], floats[offset + 11])
    val rightMouthCorner = Point(floats[offset + 12], floats[offset + 13])

    return Face(
        Rect(x1, y1, x2, y2),
        Landmarks(leftEye, rightEye, nose, leftMouthCorner, rightMouthCorner),
        floats[offset + 14]
    )
}
