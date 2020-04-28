package com.example.rushi.smartwatch;

import android.app.Notification;
import android.app.NotificationChannel;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.Intent;
import android.graphics.Color;
import android.os.Build;
import android.os.Handler;
import android.os.IBinder;
import android.util.Log;
import android.widget.Toast;

import androidx.core.app.NotificationCompat;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.charset.StandardCharsets;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.Locale;
import java.util.Set;
import java.util.UUID;

public class BluetoothCommService extends Service {

    BluetoothManager bluetoothManager;
    BluetoothAdapter bluetoothAdapter;
    BluetoothSocket bluetoothSocket;
    BluetoothDevice bluetoothDevice;

    public static OutputStream outputStream;
    public static InputStream inputStream;

    Thread workerThread;
    Boolean stopWorker;

    int readBufferPosition;
    byte[] readBuffer;

    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    String MACAddress = "00:19:08:35:F6:00";

    private static String CONNECTED_DEVICE = null;
    private static int STATUS = -1;

    @Override
    public void onCreate()
    {
        super.onCreate();
        STATUS = 0;
        bluetoothManager = (BluetoothManager)getSystemService(Context.BLUETOOTH_SERVICE);
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

            Notification notification = notificationBuilder.setContentTitle("Bluetooth Foreground Service").setContentText("Connected With SmartWatch").setContentIntent(pendingIntent).setSmallIcon(R.mipmap.ic_launcher_round).setTicker("Description").setOngoing(true).build();
            startForeground(1, notification);
            try
            {
                findBT();
                if (bluetoothDevice == null)
                {
                    msg("No device found");
                }
                else if (bluetoothSocket == null && bluetoothAdapter.isEnabled())
                {
                    openBT();
                }
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
        bluetoothAdapter = bluetoothManager.getAdapter();
        if(bluetoothAdapter == null)
        {
            msg("Bluetooth Device Not Available");
        }
        else if( !bluetoothAdapter.isEnabled() )
        {
            msg("Turn on Bluetooth");
            Intent turnBTon = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivity(turnBTon);
            return;
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
        Log.e("Service", "Here at 1");
        beginListenForData();
    }

    public class ListenThread extends Thread
    {
        Boolean stopWorker = false;
        int readBufferPosition = 0;
        byte[] readBuffer = new byte[1024];


        final Handler handler = new Handler();
        final byte delimiter = 10;  //ASCII code for newline character

        @Override
        public void run()
        {
            while(!Thread.currentThread().isInterrupted() && !stopWorker)
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
                                Log.d("TAG", "run: ");
                                handler.post(new Runnable()
                                {
                                    public void run()
                                    {
                                        Log.e("Rx S","Received data is: "+data);
                                        if (data.contains("interrupt"))
                                            MainActivity.emailAndSMSStuff();
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
    }

    public void beginListenForData()
    {
        Intent intent = new Intent();
        PendingIntent pendingIntent = PendingIntent.getActivity(this, 0, intent, 0);

        NotificationCompat.Builder builder = new NotificationCompat.Builder(this);

        ListenThread listenThread = new ListenThread();
        listenThread.start();
    }

    public static void initiateSend(String s) throws IOException
    {
        String data = s + '\n';
        outputStream.write(data.getBytes());
    }

    public static void sendData(String key, String value) throws IOException
    {
        String data = '{' + key + '*' + value + '}' + '\n';
        outputStream.write(data.getBytes());
    }

    public static void terminateSend(String s) throws IOException
    {
        String data = s + '\n';
        outputStream.write(data.getBytes());
    }

    public static void updateTime()
    {
        String myTime = new SimpleDateFormat("HH:mm:ss", Locale.getDefault()).format(new Date());   //A
        String[] myTimeSplit = myTime.split(":");
        String[] myTimeSplitKey = {"hr", "min", "sec"};

        try
        {
            initiateSend("~");
            for(int i=0; i<myTimeSplit.length; i++)
            {
                sendData(myTimeSplitKey[i], myTimeSplit[i]);
            }
            terminateSend("=");
        }
        catch (IOException e)
        {

        }
    }

    public static void updateAlarmTime()
    {
        int numberOfAlarms = FirebaseFetchService.getNumberOfAlarms();  //B
        ArrayList<Alarm> alarms = FirebaseFetchService.getAlarms();     //C
        String alarmHourSend[] = new String[numberOfAlarms];  // Make Tx fast
        String alarmMinSend[] = new String[numberOfAlarms];
        int alarmAMPMSend[] = new int[numberOfAlarms];

        for(int i=0; i<numberOfAlarms; i++)
        {
            alarmHourSend[i] = alarms.get(i).getAlarmHr();
            alarmMinSend[i] = alarms.get(i).getAlarmMin();
            alarmAMPMSend[i] = alarms.get(i).getAlarmAMPM();
        }

        try
        {
            initiateSend("~");
            sendData("noa", numberOfAlarms+"");
            for(int i=0; i<numberOfAlarms; i++)
            {
                sendData("h"+i, alarmHourSend[i]);
                sendData("m"+i, alarmMinSend[i]);
                sendData("ap"+i, alarmAMPMSend[i]+"");
            }
            terminateSend("=");
        }
        catch (IOException e)
        {

        }
    }

    public static void updateAlarmMessage()
    {
        int numberOfAlarms = FirebaseFetchService.getNumberOfAlarms();  //B
        ArrayList<Alarm> alarms = FirebaseFetchService.getAlarms();     //C
        String alarmSendArray[] = new String[numberOfAlarms];  // Make Tx fast
        for(int i=0; i<numberOfAlarms; i++)
        {
            alarmSendArray[i] = alarms.get(i).getMessage();
        }

        try
        {
            initiateSend("~");
            sendData("noa", numberOfAlarms+"");
            for(int i=0; i<numberOfAlarms; i++)
            {
                sendData("ms"+i, alarmSendArray[i]);
            }
            terminateSend("=");
        }
        catch (IOException e)
        {

        }
    }

    public static void updateSV()
    {
        String volume = FirebaseFetchService.getVolume();   //D
        String vibration = FirebaseFetchService.getVibration(); //E

        try
        {
            initiateSend("~");
            sendData("vo", volume);
            sendData("vi", vibration);
            terminateSend("=");
        }
        catch (IOException e)
        {

        }
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

    public static InputStream getInputStream()
    {
        return inputStream;
    }

    public static OutputStream getOutputStream()
    {
        return outputStream;
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

