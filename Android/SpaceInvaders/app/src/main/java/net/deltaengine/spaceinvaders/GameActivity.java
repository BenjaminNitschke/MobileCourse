package net.deltaengine.spaceinvaders;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;

public class GameActivity extends Activity {
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        view = new SpaceInvadersSurfaceView(this);
        setContentView(view);
    }

    private GLSurfaceView view;

    @Override
    protected void onPause() {
        super.onPause();
        view.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        view.onResume();
    }
}