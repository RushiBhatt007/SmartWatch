package com.example.rushi.smartwatch;

import android.app.Notification;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Intent;
import android.os.Handler;
import android.os.IBinder;
import android.util.Log;

import androidx.core.app.NotificationCompat;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.charset.StandardCharsets;

public class BluetoothCommReceiverService extends Service {

    public static final String CHANNEL_ID = "ForegroundBluetoothReceiverChannel";

    public static OutputStream outputStream = BluetoothCommService.getOutputStream();
    public static InputStream inputStream = BluetoothCommService.getInputStream();

    Thread workerThread;
    Boolean stopWorker;

    int readBufferPosition;
    byte[] readBuffer;

    @Override
    public void onCreate()
    {
        super.onCreate();
    }

    @Override
    public IBinder onBind(Intent intent)
    {
        return null;
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId)
    {
        Intent notificationIntent = new Intent(this, MainActivity.class);
        PendingIntent pendingIntent = PendingIntent.getActivity(this, 0, notificationIntent, 0);

        Notification notification = new NotificationCompat.Builder(this, CHANNEL_ID).setContentTitle("Receiver Service").setContentText("BT RX").setSmallIcon(R.mipmap.ic_launcher_round).setContentIntent(pendingIntent).build();

        startForeground(1, notification);

        beginListenForData();

        return START_NOT_STICKY;
    }


    void beginListenForData()
    {
        final Handler handler = new Handler();
        final byte delimiter = 10;  //ASCII code for newline character

        stopWorker = false;
        readBufferPosition = 0;
        readBuffer = new byte[1024];

        workerThread = new Thread(new Runnable()
        {
            @Override
            public void run()
            {
                while (!Thread.currentThread().isInterrupted() && !stopWorker)
                {
                    try
                    {
                        int bytesAvailable = inputStream.available();
                        if(bytesAvailable > 0)
                        {
                            byte[] packetBytes = new byte[bytesAvailable];
                            inputStream.read(packetBytes);
                            Log.e("Service", "Here at 4");
                            for(int i=0;i<bytesAvailable;i++)
                            {
                                byte b = packetBytes[i];
                                if(b == delimiter)
                                {
                                    byte[] encodedBytes = new byte[readBufferPosition];
                                    System.arraycopy(readBuffer, 0, encodedBytes, 0, encodedBytes.length);
                                    final String data = new String(encodedBytes, StandardCharsets.US_ASCII);
                                    readBufferPosition = 0;
                                    Log.e("Service", "Here at 2");
                                    handler.post(new Runnable()
                                    {
                                        public void run()
                                        {
                                            Log.e("Rx S","Received data is: "+data);
                                        }
                                    });
                                }
                                else
                                {
                                    readBuffer[readBufferPosition++] = b;
                                }
                            }
                        }
                    }
                    catch (IOException e)
                    {
                        stopWorker = true;
                    }
                }
            }
        });

    }
}
