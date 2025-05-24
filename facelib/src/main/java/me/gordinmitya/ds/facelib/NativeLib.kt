package me.gordinmitya.ds.facelib

class NativeLib {
    external fun stringFromJNI(): String

    companion object {
        init {
            System.loadLibrary("facelib")
        }
    }
}
