package com.example.rushi.smartwatch;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import java.util.Calendar;

public class MainActivity extends AppCompatActivity {

    private Button ConnectButton, SVButton, AlarmButton, SOSButton, FMWButton;
    private ImageView ConnectImage;

    private String mydate;
    private String deviceName = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ConnectButton = (Button) findViewById(R.id.ConnectButton);
        SVButton = (Button) findViewById(R.id.SVButton);
        AlarmButton = (Button) findViewById(R.id.AlarmButton);
        SOSButton = (Button) findViewById(R.id.SOSButton);
        FMWButton = (Button) findViewById(R.id.FMWButton);
        ConnectImage = (ImageView) findViewById(R.id.ConnectImage);

        ConnectImage.setImageAlpha(32);
        //SVButton.setEnabled(false);
        AlarmButton.setEnabled(false);
        SOSButton.setEnabled(false);
        FMWButton.setEnabled(false);

        ConnectButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                onConnectButtonClick();
            }
        });

        FMWButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent nextActivity = new Intent(MainActivity.this, FMWActivity.class);
                startActivity(nextActivity);
            }
        });

        SVButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent nextActivity = new Intent(MainActivity.this, SVActivity.class);
                startActivity(nextActivity);
            }
        });

        AlarmButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent nextActivity = new Intent(MainActivity.this, AlarmActivity.class);
                startActivity(nextActivity);
            }
        });

        SOSButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent nextActivity = new Intent(MainActivity.this, SOSActivity.class);
                startActivity(nextActivity);
            }
        });
    }

    private void onConnectButtonClick()
    {
        Intent foreground = new Intent(MainActivity.this, BluetoothCommService.class);
        if(deviceName == null)
        {
            foreground.setAction("connect");
        }
        else
        {
            foreground.setAction("disconnect");
        }
        startService(foreground);

        final Thread thread = new Thread(new Runnable()
        {
            @Override
            public void run()
            {
                try
                {
                    while (BluetoothCommService.getSTATUS() == 0)
                    {
                        this.wait(1000);
                        Log.e("MainActivity", "Refreshing");
                    }
                } catch (InterruptedException e)
                {
                    finish();
                }
                runOnUiThread(new Runnable()
                {
                    @Override
                    public void run()
                    {
                        uiFunction();
                    }
                });
            }
        });
        thread.start();
    }

    void uiFunction()
    {
        try
        {
            deviceName = BluetoothCommService.getConnectedDevice();
        } catch (Exception e)
        {
            deviceName = null;
        }

        if(deviceName == null)
        {
            msg("Trying to Connect....");
        }
        else
        {
            msg("Successfully Connected with "+deviceName);
            onConnect();
        }
    }

    private void onConnect()
    {
        ConnectImage.setImageAlpha(255);
        SVButton.setEnabled(true);
        AlarmButton.setEnabled(true);
        SOSButton.setEnabled(true);
        FMWButton.setEnabled(true);

        mydate = java.text.DateFormat.getDateTimeInstance().format(Calendar.getInstance().getTime());
        msg(mydate);
    }

    private void msg(String s)
    {
        Toast.makeText(getApplicationContext(), s, Toast.LENGTH_SHORT).show();
    }

}
