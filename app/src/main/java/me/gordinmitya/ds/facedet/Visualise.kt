package me.gordinmitya.ds.facedet

import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import me.gordinmitya.ds.facelib.Face

fun Canvas.drawFace(face: Face, paint: Paint) {
    val rect = face.rect
    val lm = face.landmarks
    val w = width
    val h = height

    paint.color = Color.LTGRAY
    paint.style = Paint.Style.STROKE
    drawRect(rect.x1 * w, rect.y1 * h, rect.x2 * w, rect.y2 * h, paint)

    paint.style = Paint.Style.FILL
    paint.color = Color.YELLOW
    drawCircle(lm.leftEye.x * w, lm.leftEye.y * h, 10f, paint)
    paint.color = Color.BLUE
    drawCircle(lm.rightEye.x * w, lm.rightEye.y * h, 10f, paint)

    paint.color = Color.RED
    drawCircle(lm.nose.x * w, lm.nose.y * h, 10f, paint)

    paint.color = Color.GREEN
    drawCircle(
        lm.leftMouthCorner.x * w,
        lm.leftMouthCorner.y * h,
        10f,
        paint
    )
    paint.color = Color.MAGENTA
    drawCircle(
        lm.rightMouthCorner.x * w,
        lm.rightMouthCorner.y * h,
        10f,
        paint
    )
}
