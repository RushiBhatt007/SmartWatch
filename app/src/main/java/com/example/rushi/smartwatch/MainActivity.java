package com.example.rushi.smartwatch;

import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.AsyncTask;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import java.io.IOException;
import java.util.Calendar;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    private Button ConnectButton, SVButton, AlarmButton, SOSButton, FMWButton;
    private ImageView ConnectImage;

    private BluetoothAdapter bluetoothAdapter = null;
    private BluetoothSocket btSocket = null;
    private boolean bluetoothOnFlag = true;
    private BluetoothAdapter myBluetooth = null;
    private boolean isBtConnected = false;

    private String mydate;
    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    private String MACaddress = null;
    private String deviceName = null;
    private ProgressDialog progress;

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

        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        ConnectImage.setImageAlpha(32);
        SVButton.setEnabled(false);
        AlarmButton.setEnabled(false);
        SOSButton.setEnabled(false);
        FMWButton.setEnabled(false);

        MACaddress = "00:19:08:35:F6:00";
        deviceName = "HC-05";


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
    }

    private void onConnectButtonClick()
    {
        if (bluetoothAdapter == null)
        {
            msg("Bluetooth Device Not Available");
            bluetoothOnFlag = false;
            finish();
        }
        else if( !bluetoothAdapter.isEnabled() )
        {
            Intent turnBTon = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(turnBTon, 1);
        }

        if(bluetoothOnFlag == true)
            new ConnectBT().execute();
    }

    private class ConnectBT extends AsyncTask<Void, Void, Void>
    {
        private boolean ConnectSuccess = true;

        @Override
        protected void onPreExecute()
        {
            progress = ProgressDialog.show(MainActivity.this, "Connecting with "+deviceName,"Please wait...");
        }

        @Override
        protected Void doInBackground(Void... devices)
        {
            try {
                if(btSocket == null || !isBtConnected)
                {
                    myBluetooth = BluetoothAdapter.getDefaultAdapter();//get the mobile bluetooth device
                    BluetoothDevice bluetoothDevice = myBluetooth.getRemoteDevice(MACaddress);//connects to the device's address and checks if it's available
                    btSocket = bluetoothDevice.createInsecureRfcommSocketToServiceRecord(myUUID);//create a RFCOMM (SPP) connection
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
                    btSocket.connect();//start connection
                }
            }
            catch (IOException e)
            {
                ConnectSuccess = false;
            }
        return null;
        }

        @Override
        protected void onPostExecute(Void result)//after the doInBackground, it checks if everything went fine
        {
            super.onPostExecute(result);
            if (!ConnectSuccess)
            {
                msg("Connection Failed. Is "+deviceName +" a SPP Bluetooth? Try again.");
                finish();
            }
            else
            {
                msg("Successfully Connected with "+deviceName);
                isBtConnected = true;
                onConnect();
            }
            progress.dismiss();
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
