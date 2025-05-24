package me.gordinmitya.ds.facelib

import android.content.Context
import android.content.res.AssetManager
import android.graphics.Bitmap
import java.io.Closeable

class Detector : Closeable {
    private var ptr: Long = 0L
    private val flatFaces = FloatArray(MAX_FACES * FACE_SIZE_FLAT)

    fun load(context: Context) {
        if (ptr != 0L) return
        ptr = createNet(context.assets, MODEL_PATH_PREFIX)
        if (ptr == 0L) {
            throw RuntimeException("Failed to create face detector network")
        }
    }

    fun detect(image: Bitmap): List<Face> {
        ensureLoaded()
        return detect2objects(ptr, image)?.toList() ?: emptyList()
    }

    fun detectFlat(image: Bitmap) {
        ensureLoaded()
        val count = detect2array(ptr, image, flatFaces)
        if (count > 0) {
            floatsToFace(flatFaces, 0)
        }
        // TODO use it as flat points array
    }

    private fun ensureLoaded() {
        if (ptr == 0L) {
            throw IllegalStateException("network is not loaded")
        }
    }

    override fun close() {
        if (ptr != 0L) {
            destroyNet(ptr)
            ptr = 0L
        }
    }

    companion object {
        private const val MODEL_PATH_PREFIX = "models/face"

        //        private const val MODEL_PATH_PREFIX = "models/faceDetector.ncnn"
        private const val MAX_FACES = 10 // maximum number of faces to detect

        init {
            System.loadLibrary("facelib")
        }

        @JvmStatic
        private external fun createNet(assetManager: AssetManager, modelPathPrefix: String): Long

        @JvmStatic
        private external fun destroyNet(ptr: Long)

        @JvmStatic
        private external fun detect2objects(
            ptr: Long,
            image: Bitmap,
        ): Array<Face>?

        @JvmStatic
        private external fun detect2array(
            ptr: Long,
            image: Bitmap,
            detections: FloatArray,
        ): Int
    }
}
