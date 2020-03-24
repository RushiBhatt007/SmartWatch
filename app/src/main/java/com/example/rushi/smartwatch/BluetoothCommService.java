package com.example.rushi.smartwatch;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.os.Build;
import android.os.IBinder;
import android.util.Log;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Set;
import java.util.UUID;

public class BluetoothCommService extends Service {

    BluetoothAdapter bluetoothAdapter;
    BluetoothSocket bluetoothSocket;
    BluetoothDevice bluetoothDevice;

    OutputStream outputStream;
    InputStream inputStream;

    Thread workerThread;
    Boolean stopWorker;

    int readBufferPosition;
    byte[] readBuffer;

    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    String MACAddress = "00:19:08:35:F6:00";

    private static String CONNECTED_DEVICE = null;
    private static int STATUS = 1;

    @Override
    public void onCreate()
    {
        super.onCreate();
        STATUS = 0;
        Log.e("BluetoothCommService", "OnCreate");
    }

    @Override
    public IBinder onBind(Intent intent)
    {
        return null;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId)
    {
        Log.e("BluetoothCommService", "OnStartCommand");
        if ("connect".equals(intent.getAction()))
        {
            Intent startConnect = new Intent(this, MainActivity.class);
            PendingIntent pendingIntent = PendingIntent.getActivity(this, 0, startConnect, 0);

            Notification.Builder notificationBuilder;

            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O)
            {
                String NOTIFICATION_CHANNEL_ID = "com.example.rushi.smartwatch";
                String channelName = "My Background Service";
                NotificationChannel chan = new NotificationChannel(NOTIFICATION_CHANNEL_ID, channelName, NotificationManager.IMPORTANCE_NONE);
                chan.setLightColor(Color.BLUE);
                chan.setLockscreenVisibility(Notification.VISIBILITY_PRIVATE);
                NotificationManager manager = (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);
                assert manager != null;
                manager.createNotificationChannel(chan);
                notificationBuilder = new Notification.Builder(this, NOTIFICATION_CHANNEL_ID);
            }
            else
                notificationBuilder = new Notification.Builder(this);

            //Notification notification = notificationBuilder.setContentTitle("Test").setContentText("test text").setContentIntent(pendingIntent).setSmallIcon(R.mipmap.ic_launcher_round).setTicker("Description").setOngoing(true).build();

            //startForeground(1, notification);
            try
            {
                findBT();
                if (bluetoothSocket == null)
                    openBT();
            }
            catch (IOException e)
            {
                stopForeground(true);
                onDestroy();
            }

            STATUS = 1;
            return START_STICKY;
        }
        else
        {
            stopForeground(true);
            onDestroy();
            return START_NOT_STICKY;
        }
    }

    @Override
    public void onDestroy()
    {
        closeBT();
        super.onDestroy();
    }

    void findBT()
    {
        bluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if(bluetoothAdapter == null)
        {
            msg("Bluetooth Device Not Available");
        }
        else if( !bluetoothAdapter.isEnabled() )
        {
            msg("Turn on Bluetooth");
            //Intent turnBTon = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            //startActivity(turnBTon);
        }

        Set<BluetoothDevice> pairedDevices = bluetoothAdapter.getBondedDevices();
        if(pairedDevices.size() > 0 && bluetoothDevice == null)
        {
            for(BluetoothDevice device: pairedDevices)
            {
                if(device.getAddress().equals(MACAddress))
                {
                    CONNECTED_DEVICE = device.getName();
                    bluetoothDevice = device;
                    break;
                }
            }
        }
    }

    void openBT() throws IOException
    {
        bluetoothSocket = bluetoothDevice.createRfcommSocketToServiceRecord(myUUID);
        bluetoothSocket.connect();
        outputStream = bluetoothSocket.getOutputStream();
        inputStream = bluetoothSocket.getInputStream();

        //beginListenForData();
    }

    void beginListenForData()
    {

    }

    void sendData() throws IOException
    {

    }

    void closeBT()
    {
        try
        {
            STATUS = 1;
            stopWorker = true;
            if(outputStream != null) outputStream.close();
            if(inputStream != null) inputStream.close();
            bluetoothSocket.close();
        } catch (IOException e)
        {
            outputStream = null;
            inputStream = null;
        } finally
        {
            bluetoothDevice = null;
            bluetoothSocket = null;
            CONNECTED_DEVICE = null;
        }
    }

    void msg(String s)
    {
        Toast.makeText(getApplicationContext(), s, Toast.LENGTH_SHORT).show();
    }

    public static String getConnectedDevice()
    {
        return CONNECTED_DEVICE;
    }

    public static int getSTATUS()
    {
        return STATUS;
    }
}

