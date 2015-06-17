package net.deltaengine.spaceinvaders;

import android.content.res.AssetManager;
import android.opengl.GLES11;
import android.opengl.GLSurfaceView;

import java.util.ArrayList;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class SpaceInvadersRenderer implements GLSurfaceView.Renderer {
    public SpaceInvadersRenderer(AssetManager assets) {
        this.assets = assets;
    }

    private AssetManager assets;

    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        gl.glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        gl.glEnable(gl.GL_BLEND);
        gl.glBlendFunc(gl.GL_SRC_ALPHA, gl.GL_ONE_MINUS_SRC_ALPHA);
        gl.glEnable(gl.GL_TEXTURE_2D);
        gl.glEnableClientState(gl.GL_VERTEX_ARRAY);
        gl.glEnableClientState(gl.GL_TEXTURE_COORD_ARRAY);
        CreateBackground();
        CreateShip();
        CreateEnemies();
    }

    float timeDelta;
    float time;
    float lastTimeShot;
    Sprite background;
    Sprite ship;
    float shipPosition = 0;
    Texture missileTexture;
    ArrayList<Sprite> activeMissiles = new ArrayList<>();
    ArrayList<Sprite> enemies = new ArrayList<>();
    float enemiesPosition = 0;
    boolean enemiesMovingRight = true;

    void CreateBackground()
    {
        background = new Sprite(new Texture(assets, "Background.png"), 0.0f, 0.0f, 2.0f, 1.0f);
    }

    void CreateShip()
    {
        ship = new Sprite(new Texture(assets, "Ship.png"), 0.0f, -0.8f, 0.07f, 0.1f);
        missileTexture = new Texture(assets, "Missile.png");
    }

    void CreateEnemies()
    {
        Texture enemyTexture = new Texture(assets, "Enemy.png");
        for (float y = 0.7f; y > 0.3f; y -= 0.3f)
            for (float x = -0.8f; x <= 0.8f; x += 0.2f)
                enemies.add(new Sprite(enemyTexture, x, y, 0.05f, 0.05f));
    }

    public void onDrawFrame(GL10 gl) {
        timeDelta = 1 / 60.0f;
        time += timeDelta;
        ControlShip();
        MoveEnemies();
        gl.glClear(gl.GL_COLOR_BUFFER_BIT);
        gl.glDisable(gl.GL_BLEND);
        background.Draw();
        gl.glEnable(gl.GL_BLEND);
        for (Sprite missile : activeMissiles)
            missile.Draw();
        ship.Draw(shipPosition);
        for (Sprite enemy : enemies)
            enemy.Draw(enemiesPosition);
    }

    void ControlShip()
    {
        if (lastTimeShot < time - 0.33f)
            FireMissile();
        HandleMissiles();
    }

    void FireMissile()
    {
        lastTimeShot = time;
        activeMissiles.add(new Sprite(missileTexture, shipPosition, -0.67f, 0.02f, 0.02f));
    }

    void HandleMissiles()
    {
        //Java 8 would make this very easy via removeIf lambda, but this is not supported on Android
        ArrayList<Sprite> missilesToRemove = new ArrayList<>();
        for (Sprite missile : activeMissiles)
            if (missile.IncreaseY(timeDelta * 3) || CollidingWithEnemyAndKilledIt(missile))
                missilesToRemove.add(missile);
        for (Sprite missile : missilesToRemove)
            activeMissiles.remove(activeMissiles);
    }

    boolean CollidingWithEnemyAndKilledIt(Sprite missile)
    {
        for (Sprite enemy : enemies)
            if (enemy.DistanceTo(missile, enemiesPosition) < 0.05f)
            {
                enemy.IncreaseY(1.0f); // Move out of screen
                return true;
            }
        return false;
    }

    void MoveEnemies()
    {
        enemiesPosition += (enemiesMovingRight ? 0.2f : -0.2f) * timeDelta;
        if (enemiesPosition < -0.1f || enemiesPosition > 0.1f)
            enemiesMovingRight = !enemiesMovingRight;
    }

    public void onSurfaceChanged(GL10 gl, int width, int height) {
        gl.glViewport(0, 0, width, height);
        screenWidth = width;
    }

    public int screenWidth;
}