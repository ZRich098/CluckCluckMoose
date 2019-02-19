package edu.cornell.gdiac.helloworld;

import org.libsdl.app.*;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

public class HelloWorld extends SDLActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        // Make sure this is before calling super.onCreate
        setTheme(R.style.CUGLTheme);
        super.onCreate(savedInstanceState);
    }
}
