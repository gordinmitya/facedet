package me.gordinmitya.ds.facedet

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.graphics.Canvas
import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity
import androidx.lifecycle.lifecycleScope
import kotlinx.coroutines.Dispatchers
import kotlinx.coroutines.launch
import kotlinx.coroutines.withContext
import me.gordinmitya.ds.facedet.databinding.ActivityMainBinding
import me.gordinmitya.ds.facelib.Detector
import me.gordinmitya.ds.facelib.Face
import kotlin.system.measureTimeMillis

class MainActivity : AppCompatActivity() {
    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        lifecycleScope.launch {
            doit()
        }
    }

    private suspend fun doit(): Unit = withContext(Dispatchers.IO) {
        val image = assets.open("selfie.jpg").use { inputStream ->
            BitmapFactory.decodeStream(inputStream)
        }
        withContext(Dispatchers.Main) {
            binding.image.setImageBitmap(image)
            binding.text.text = "Detecting faces..."
        }
        var timeMs = 0L
        var faces = emptyList<Face>()
        Detector().use { detector ->
            detector.load(this@MainActivity)
            timeMs = measureTimeMillis {
                faces = detector.detect(image)
            }
        }
        val paint = android.graphics.Paint().also {
            it.style = android.graphics.Paint.Style.STROKE
            it.strokeWidth = 5f
        }
        val bitmap = image.copy(Bitmap.Config.ARGB_8888, true)
        Canvas(bitmap).also { canvas ->
            faces.forEach { face ->
                canvas.drawFace(face, paint)
            }
        }
        withContext(Dispatchers.Main) {
            binding.image.setImageBitmap(bitmap)
            binding.text.text = "Detected ${faces.size} faces in ${timeMs}ms"
        }
    }
}
