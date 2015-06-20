package net.deltaengine.spaceinvaders;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

public class SpaceInvadersSurfaceView extends GLSurfaceView {
    public SpaceInvadersSurfaceView(Context context){
        super(context);
        setEGLContextClientVersion(1);
        renderer = new SpaceInvadersRenderer(context.getAssets());
        // Fix broken emulator on some systems
        setEGLConfigChooser(8, 8, 8, 8, 16, 0);
        setRenderer(renderer);
    }

    private final SpaceInvadersRenderer renderer;

    @Override
    public boolean onTouchEvent(MotionEvent e) {
        renderer.shipPosition = -1 + 2 * (e.getX() / renderer.screenWidth);
        return true;
    }
}