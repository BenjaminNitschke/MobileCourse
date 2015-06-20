package net.deltaengine.spaceinvaders;

import android.content.res.AssetManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES11;
import android.opengl.GLUtils;

import java.io.IOException;
import java.io.InputStream;

public class Texture {
    public Texture(AssetManager assets, String filename){
        int[] handles = new int[1];
        GLES11.glGenTextures(1, handles, 0);
        handle = handles[0];
        GLES11.glBindTexture(GLES11.GL_TEXTURE_2D, handle);
        GLES11.glTexParameterf(GLES11.GL_TEXTURE_2D, GLES11.GL_TEXTURE_MIN_FILTER, GLES11.GL_LINEAR);
        GLES11.glTexParameterf(GLES11.GL_TEXTURE_2D, GLES11.GL_TEXTURE_MAG_FILTER, GLES11.GL_LINEAR);
        try {
            InputStream stream = assets.open(filename);
            Bitmap bitmap = BitmapFactory.decodeStream(stream);
            width = bitmap.getWidth();
            height = bitmap.getHeight();
            GLUtils.texImage2D(GLES11.GL_TEXTURE_2D, 0, bitmap, 0);
            bitmap.recycle();
            stream.close();
        } catch (final IOException e) {
            e.printStackTrace();
        }
    }

    private final int handle;
    private int width;
    private int height;

    public int GetHandle() {
        return handle;
    }
}