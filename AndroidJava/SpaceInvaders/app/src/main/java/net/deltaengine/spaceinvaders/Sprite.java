package net.deltaengine.spaceinvaders;

import android.opengl.GLES11;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.FloatBuffer;
import java.nio.ShortBuffer;

public class Sprite {
    public Sprite(Texture texture, float initialX, float initialY, float width, float height){
        this.texture = texture;
        this.initialX = initialX;
        this.initialY = initialY;
        this.width = width;
        this.height = height;
        CreateBuffers();
    }

    private final Texture texture;
    private final float initialX;
    private float initialY;
    private final float width;
    private final float height;

    public void CreateBuffers()
    {
        ByteBuffer vbb = ByteBuffer.allocateDirect( 4 * 3 * 4 );
        vbb.order( ByteOrder.nativeOrder() );
        vertexBuffer = vbb.asFloatBuffer();
        vertexBuffer.put(vertices);
        vertexBuffer.position(0);
        ByteBuffer ibb = ByteBuffer.allocateDirect( 2 * 3 * 2 );
        ibb.order( ByteOrder.nativeOrder() );
        indexBuffer = ibb.asShortBuffer();
        indexBuffer.put(new short[] { 0, 1, 2,   0, 2, 3 });
        indexBuffer.position(0);
        ByteBuffer tbb = ByteBuffer.allocateDirect( 4 * 2 * 4 );
        tbb.order( ByteOrder.nativeOrder() );
        uvBuffer = tbb.asFloatBuffer();
        uvBuffer.put(new float[] { 0.0f, 0.0f,   0.0f, 1.0f,   1.0f, 1.0f,   1.0f, 0.0f });
        uvBuffer.position(0);
    }

    private FloatBuffer vertexBuffer;
    private float[] vertices = new float[] { -1, 1, 0,   -1, -1, 0,   1, -1, 0,   1, 1, 0 };
    private FloatBuffer uvBuffer;
    private ShortBuffer indexBuffer;

    public void Draw() { Draw(0, 0); }
    public void Draw(float x) { Draw(x, 0); }
    public void Draw(float x, float y) {
        GLES11.glBindTexture(GLES11.GL_TEXTURE_2D, texture.GetHandle());
        GLES11.glTexCoordPointer(2, GLES11.GL_FLOAT, 0, uvBuffer);
        vertices[0] = initialX + x - width; vertices[1] = initialY + y + height; // top left
        vertices[3] = initialX + x - width; vertices[4] = initialY + y - height; // bottom left
        vertices[6] = initialX + x + width; vertices[7] = initialY + y - height; // bottom right
        vertices[9] = initialX + x + width; vertices[10] = initialY + y + height; // top right
        vertexBuffer.put(vertices);
        vertexBuffer.position(0);
        GLES11.glVertexPointer(3, GLES11.GL_FLOAT, 0, vertexBuffer);
        GLES11.glDrawElements(GLES11.GL_TRIANGLES, 2 * 3, GLES11.GL_UNSIGNED_SHORT, indexBuffer);
    }

    public float GetWidth() { return width; }
    public boolean IncreaseY(float amount) { initialY += amount; return initialY > 1.0f; }
    float DistanceTo(Sprite other, float xOffset)
    {
        float distanceX = Math.abs(initialX + xOffset - other.initialX);
        float distanceY = Math.abs(initialY - other.initialY);
        return (float)Math.sqrt(distanceX * distanceX + distanceY * distanceY);
    }
}