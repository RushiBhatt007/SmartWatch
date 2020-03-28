package com.example.rushi.smartwatch;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
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

    private String myDate;
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

        if(BluetoothCommService.getSTATUS()==1 && FirebaseFetchService.getSTATUS()==1)
        {
            ConnectImage.setImageAlpha(32);
            SVButton.setEnabled(false);
            //AlarmButton.setEnabled(false);
            SOSButton.setEnabled(false);
            FMWButton.setEnabled(false);
        }

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
        Intent BTForeground = new Intent(MainActivity.this, BluetoothCommService.class);
        if(deviceName == null)
        {
            BTForeground.setAction("connect");
        }
        else
        {
            BTForeground.setAction("disconnect");
        }
        startService(BTForeground);

        final Thread BTThread = new Thread(new Runnable()
        {
            @Override
            public void run()
            {
                try
                {
                    while (BluetoothCommService.getSTATUS() == 0)
                    {
                        this.wait(100);
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
        BTThread.start();

        Intent FBForeground = new Intent(MainActivity.this, FirebaseFetchService.class);
        FBForeground.setAction("fetch");
        startService(FBForeground);

        final Thread FBThread = new Thread(new Runnable() {
            @Override
            public void run() {
                try
                {
                    while(FirebaseFetchService.getSTATUS() == 0)
                    {
                        this.wait(10);
                        Log.e("MainActivity", "Fetching");
                    }
                }
                catch (InterruptedException e)
                {
                    msg("Firebase data fetch failed");
                    finish();
                }
            }
        });
        FBThread.start();
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
            msg("Couldn't connect to HC-05. Please Try Again");
        }
        else
        {
            msg("Successfully connected with "+deviceName);
            onConnect();
        }
    }

    private void onConnect()
    {
        Handler handler = new Handler();
        handler.postDelayed(new Runnable() {
            public void run()
            {
                ConnectImage.setImageAlpha(255);
                SVButton.setEnabled(true);
                AlarmButton.setEnabled(true);
                SOSButton.setEnabled(true);
                FMWButton.setEnabled(true);

                myDate = java.text.DateFormat.getDateTimeInstance().format(Calendar.getInstance().getTime());
                msg(myDate);
            }
        }, 2000);

    }

    private void msg(String s)
    {
        Toast.makeText(getApplicationContext(), s, Toast.LENGTH_SHORT).show();
    }

}
