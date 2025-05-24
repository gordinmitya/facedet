package me.gordinmitya.ds.facedet

import android.graphics.Canvas
import android.graphics.Color
import android.graphics.Paint
import me.gordinmitya.ds.facelib.Face

fun Canvas.drawFace(face: Face, paint: Paint) {
    val rect = face.rect
    val lm = face.landmarks

    paint.color = Color.LTGRAY
    drawRect(
        rect.topLeft.x,
        rect.topLeft.y,
        rect.bottomRight.x,
        rect.bottomRight.y,
        paint
    )

    paint.color = Color.YELLOW
    drawCircle(lm.leftEye.x, lm.leftEye.y, 10f, paint)
    paint.color = Color.BLUE
    drawCircle(lm.rightEye.x, lm.rightEye.y, 10f, paint)

    paint.color = Color.RED
    drawCircle(lm.nose.x, lm.nose.y, 10f, paint)

    paint.color = Color.GREEN
    drawCircle(
        lm.leftMouthCorner.x,
        lm.leftMouthCorner.y,
        10f,
        paint
    )
    paint.color = Color.MAGENTA
    drawCircle(
        lm.rightMouthCorner.x,
        lm.rightMouthCorner.y,
        10f,
        paint
    )
}
