package com.example.rushi.smartwatch;

import android.app.Activity;
import android.graphics.Color;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import java.util.ArrayList;

import static com.example.rushi.smartwatch.AlarmActivity.deletedAlarmPosition;

public class AlarmCustomAdapter extends ArrayAdapter<Alarm> {

    private Activity context;
    private ArrayList<Alarm> alarms;

    public AlarmCustomAdapter(Activity context, ArrayList<Alarm> alarms)
    {
        super(context, R.layout.list_alarm, alarms);
        this.context = context;
        this.alarms = alarms;
    }

    @Override
    public View getView(int position, View view, ViewGroup parent)
    {
        LayoutInflater inflater = context.getLayoutInflater();
        View rowView = inflater.inflate(R.layout.list_alarm, null, false);

        if(deletedAlarmPosition == position)
        {
            rowView.setBackgroundColor(Color.GRAY);
        }

        TextView alarmTimeTextView = (TextView) rowView.findViewById(R.id.alarmTimeTextView);
        TextView alarmMessageTextView = (TextView) rowView.findViewById(R.id.alarmMessageTextView);

        alarmTimeTextView.setText(alarms.get(position).getAlarmTime());
        alarmMessageTextView.setText(alarms.get(position).getMessage());

        return rowView;
    }
}
