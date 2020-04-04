package com.example.rushi.smartwatch;

import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;

public class BluetoothConnectReceiver extends BroadcastReceiver {

    @Override
    public void onReceive(Context context, Intent intent)
    {
        if(BluetoothDevice.ACTION_ACL_CONNECTED.equals(intent.getAction()))
        {
            Intent foreground = new Intent(context, BluetoothCommService.class);
            foreground.setAction("connect");
            context.startService(foreground);
        }
        if(BluetoothDevice.ACTION_ACL_DISCONNECTED.equals(intent.getAction()))
        {
            Intent foreground = new Intent(context, BluetoothCommService.class);
            foreground.setAction("diconnect");
            context.startService(foreground);
        }
    }
}