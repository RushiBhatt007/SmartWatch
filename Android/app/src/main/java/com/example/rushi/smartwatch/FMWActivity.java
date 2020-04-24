package com.example.rushi.smartwatch;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import androidx.appcompat.app.AppCompatActivity;

public class FMWActivity extends AppCompatActivity {

    private Button FMWButton;

    @Override
    protected void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);
        Intent intent = getIntent();
        setContentView(R.layout.activity_fmw);

        FMWButton = (Button) findViewById(R.id.FMWButton);

        FMWButton.setOnClickListener(new View.OnClickListener()
        {
            @Override
            public void onClick(View view)
            {
                onFWMButtonClick();
            }
        });
    }

    public void onFWMButtonClick()
    {

    }
}
