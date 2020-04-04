package com.example.rushi.smartwatch;

import android.Manifest;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.location.Location;
import android.location.LocationManager;
import android.os.Bundle;
import android.os.Handler;
import android.telephony.SmsManager;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import java.util.Calendar;

public class MainActivity extends AppCompatActivity {

    private Button ConnectButton, SVButton, AlarmButton, SOSButton, FMWButton;
    private ImageView ConnectImage;

    private String myDate;
    private String deviceName = null;

    LocationManager locationManager;
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

        locationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);

        int a = BluetoothCommService.getSTATUS();
        int b = FirebaseFetchService.getSTATUS();
        if(a==-1 && b==-1)
        {
            ConnectImage.setImageAlpha(32);
            SVButton.setEnabled(false);
            //AlarmButton.setEnabled(false);
            //SOSButton.setEnabled(false);
            FMWButton.setEnabled(false);
        }

        ConnectButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                //emailAndSMSStuff();
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

    private Location getDeviceLoc()
    {
        if (ActivityCompat.checkSelfPermission(MainActivity.this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED && ActivityCompat.checkSelfPermission(MainActivity.this, Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED)
        {
            ActivityCompat.requestPermissions(MainActivity.this, new String[]{Manifest.permission.ACCESS_FINE_LOCATION}, 1);
        }
        else
        {
            Location location = locationManager.getLastKnownLocation(LocationManager.NETWORK_PROVIDER);
            Location location1 = locationManager.getLastKnownLocation(LocationManager.GPS_PROVIDER);
            Location location2 = locationManager.getLastKnownLocation(LocationManager.PASSIVE_PROVIDER);
            if (location != null)
            {
                return location;
            }
            else if (location1 != null)
            {
                return location1;
            }
            else if (location2 != null)
            {
                return location2;
            }
            else
            {
                msg("Unable to trace your location");
            }
        }
        return null;
    }

    private void emailAndSMSStuff()
    {
        Location location = getDeviceLoc();
        double latitude = location.getLatitude();
        double longitude = location.getLongitude();

        final String phNo = "9426659100";
        final String bodySMS =  "https://maps.google.com/?q="+latitude+","+longitude;

        SmsManager smsManager = SmsManager.getDefault();
        smsManager.sendTextMessage(phNo, null, bodySMS, null, null);

        final String username = FirebaseFetchService.getGMailUsername();
        final String password = FirebaseFetchService.getGMailUsername();
        final String sub = "TEST Email";
        final String to = "rybhatt27@gmail.com";
        final String body = "https://maps.google.com/?q="+latitude+","+longitude;

        msg(username);

        final Thread GMailThread = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    GMailSender sender = new GMailSender(username, password);
                    sender.sendMail(sub, body, username, to);
                    Log.e("GMailThread", "Email Sent");
                } catch (Exception e)
                {
                    Log.e("GMailThread", e.getMessage(), e);
                }
            }
        });
        GMailThread.start();


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