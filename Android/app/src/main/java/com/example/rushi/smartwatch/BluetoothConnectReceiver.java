package com.example.rushi.smartwatch;

import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.util.Log;

public class BluetoothConnectReceiver extends BroadcastReceiver {

    @Override
    public void onReceive(Context context, Intent intent)
    {
        Log.e("Receive", "here 99");
        if(BluetoothDevice.ACTION_ACL_CONNECTED.equals(intent.getAction()))
        {
            Log.e("Receive", "here 100");
            Intent foreground = new Intent(context, BluetoothCommService.class);
            foreground.setAction("connect");
            context.startService(foreground);
        }
        if(BluetoothDevice.ACTION_ACL_DISCONNECTED.equals(intent.getAction()))
        {
            Log.e("Receive", "here 101");
            Intent foreground = new Intent(context, BluetoothCommService.class);
            foreground.setAction("diconnect");
            context.startService(foreground);
        }
    }
}