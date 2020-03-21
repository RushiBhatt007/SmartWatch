package com.example.rushi.smartwatch;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity extends AppCompatActivity {

    Button ConnectButton;
    ImageView ConnectImage;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ConnectButton = (Button) findViewById(R.id.ConnectButton);
        ConnectImage = (ImageView) findViewById(R.id.ConnectImage);
        ConnectImage.setImageAlpha(32);

        ConnectButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                ConnectImage.setImageAlpha(255);
            }
        });
    }
}
