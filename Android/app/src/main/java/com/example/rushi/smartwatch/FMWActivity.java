package com.example.rushi.smartwatch;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import java.io.IOException;

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
        try {
            BluetoothCommService.sendData();
            //Toast.makeText(getApplicationContext(), "SENT", Toast.LENGTH_SHORT).show();
        }
        catch (IOException e)
        {
            Toast.makeText(getApplicationContext(), "CANT", Toast.LENGTH_SHORT).show();
        }
    }
}
