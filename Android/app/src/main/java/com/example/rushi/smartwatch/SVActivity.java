package com.example.rushi.smartwatch;

import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.SeekBar;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import java.util.ArrayList;
import java.util.List;

public class SVActivity extends AppCompatActivity {
    public SeekBar volumeSeekBar, vibrationSeekBar;
    public TextView volumeTextView, vibrationTextView, modeTextView;
    public EditText hourLongBuzzEditText, hourShortBuzzEditText, minuteLongBuzzEditText, minuteShortBuzzEditText;
    public Spinner modeSelectSpinner;
    public Button modeSaveButton;

    public int volume, vibration;
    public List<String> modeSpinnerList;
    public String selectedMode;
    public String hourLong, hourShort, minuteLong, minuteShort;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Intent intent = getIntent();
        setContentView(R.layout.activity_sv);

        volumeSeekBar = (SeekBar) findViewById(R.id.volumeSeekBar);
        vibrationSeekBar = (SeekBar) findViewById(R.id.vibrationSeekBar);
        volumeTextView = (TextView) findViewById(R.id.volumeTextView);
        vibrationTextView = (TextView) findViewById(R.id.vibrationTextView);
        modeTextView = (TextView) findViewById(R.id.modeTextView);
        hourLongBuzzEditText = (EditText) findViewById(R.id.hourLongBuzzEditText);
        hourShortBuzzEditText = (EditText) findViewById(R.id.hourShortBuzzEditText);
        minuteLongBuzzEditText = (EditText) findViewById(R.id.minuteLongBuzzEditText);
        minuteShortBuzzEditText = (EditText) findViewById(R.id.minuteShortBuzzEditText);
        modeSelectSpinner = (Spinner) findViewById(R.id.modeSelectSpinner);
        modeSaveButton = (Button) findViewById(R.id.modeSaveButton);

        volumeSeekBar.setProgress(Integer.parseInt(FirebaseFetchService.getVolume()));
        vibrationSeekBar.setProgress(Integer.parseInt(FirebaseFetchService.getVibration()));

        modeSaveButton.setEnabled(false);
        modeSaveButton.setVisibility(View.INVISIBLE);
        setEnableAllEditTexts(false);

        selectedMode = FirebaseFetchService.getSelectedMode();
        if (selectedMode.equals("2"))
        {
            modeSaveButton.setEnabled(true);
            modeSaveButton.setVisibility(View.VISIBLE);
            setEnableAllEditTexts(true);
        }
        Toast.makeText(getApplicationContext(), "Val is: "+selectedMode, Toast.LENGTH_SHORT).show();

        hourLongBuzzEditText.setText(FirebaseFetchService.getHourLong());
        hourShortBuzzEditText.setText(FirebaseFetchService.getHourShort());
        minuteLongBuzzEditText.setText(FirebaseFetchService.getMinuteLong());
        minuteShortBuzzEditText.setText(FirebaseFetchService.getMinuteShort());

        modeSpinnerList = new ArrayList<String>();
        modeSpinnerList.add("Terse");
        modeSpinnerList.add("Digit");
        modeSpinnerList.add("Custom");
        ArrayAdapter<String> modeSelectArrayAdapter = new ArrayAdapter<>(this, android.R.layout.simple_spinner_dropdown_item, modeSpinnerList);
        modeSelectArrayAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        modeSelectSpinner.setAdapter(modeSelectArrayAdapter);

        modeSelectSpinner.setSelection(Integer.parseInt(selectedMode));

        modeSelectSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                selectedMode = position + "";
                if (position == 0)
                {
                    setEnableAllEditTexts(false);
                    modeSaveButton.setEnabled(false);
                    modeSaveButton.setVisibility(View.INVISIBLE);
                    hourLong = "10";
                    hourShort = "1";
                    minuteLong = "10";
                    minuteShort = "1";
                    setModeVariablesFirebase();
                    setModeVariablesBluetooth();
                }
                else if (position == 1)
                {
                    setEnableAllEditTexts(false);
                    modeSaveButton.setEnabled(false);
                    modeSaveButton.setVisibility(View.INVISIBLE);
                    hourLong = "5";
                    hourShort = "1";
                    minuteLong = "15";
                    minuteShort = "1";
                    setModeVariablesFirebase();
                    setModeVariablesBluetooth();
                }
                else if (position == 2)
                {
                    setEnableAllEditTexts(true);
                    modeSaveButton.setEnabled(true);
                    modeSaveButton.setVisibility(View.VISIBLE);
                    hourLong = FirebaseFetchService.getHourLong();
                    hourShort = FirebaseFetchService.getHourShort();
                    minuteLong = FirebaseFetchService.getMinuteLong();
                    minuteShort = FirebaseFetchService.getMinuteShort();
                }
                hourLongBuzzEditText.setText(hourLong);
                hourShortBuzzEditText.setText(hourShort);
                minuteLongBuzzEditText.setText(minuteLong);
                minuteShortBuzzEditText.setText(minuteShort);
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });

        modeSaveButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                hourLong = hourLongBuzzEditText.getText().toString();
                hourShort = hourShortBuzzEditText.getText().toString();
                minuteLong = minuteLongBuzzEditText.getText().toString();
                minuteShort = minuteShortBuzzEditText.getText().toString();
                if (hourLong.equals("") || hourShort.equals("") || minuteLong.equals("") || minuteShort.equals(""))
                {
                    Toast.makeText(getApplicationContext(), "Please fill all the values", Toast.LENGTH_SHORT).show();
                }
                else {
                    setModeVariablesFirebase();
                    setModeVariablesBluetooth();
                    Toast.makeText(getApplicationContext(), "Custom Values Successfully Saved", Toast.LENGTH_SHORT).show();
                }
            }
        });


        volumeSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                volume = seekBar.getProgress();
                FirebaseFetchService.setVolume(volume+"");

                Handler handler = new Handler();
                handler.postDelayed(new Runnable() {
                    public void run()
                    {
                        BluetoothCommService.updateSV();
                    }
                }, 1000);

            }
        });

        vibrationSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                vibration = seekBar.getProgress();
                FirebaseFetchService.setVibration(vibration+"");

                Handler handler = new Handler();
                handler.postDelayed(new Runnable() {
                    public void run()
                    {
                        BluetoothCommService.updateSV();
                    }
                }, 1000);
            }
        });

    }

    private void setModeVariablesFirebase()
    {
        FirebaseFetchService.setSelectedMode(selectedMode);
        FirebaseFetchService.setHourLong(hourLong);
        FirebaseFetchService.setHourShort(hourShort);
        FirebaseFetchService.setMinuteLong(minuteLong);
        FirebaseFetchService.setMinuteShort(minuteShort);
    }

    private void setModeVariablesBluetooth()
    {
        Handler handler = new Handler();
        handler.postDelayed(new Runnable() {
            @Override
            public void run() {
                BluetoothCommService.updateModes();
            }
        }, 1500);
    }


    private void setEnableAllEditTexts(boolean bool)
    {
        hourLongBuzzEditText.setEnabled(bool);
        hourShortBuzzEditText.setEnabled(bool);
        minuteLongBuzzEditText.setEnabled(bool);
        minuteShortBuzzEditText.setEnabled(bool);
    }
}
