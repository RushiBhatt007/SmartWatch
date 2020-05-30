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

        modeSaveButton.setEnabled(false);
        modeSaveButton.setVisibility(View.INVISIBLE);
        volumeSeekBar.setProgress(Integer.parseInt(FirebaseFetchService.getVolume()));
        vibrationSeekBar.setProgress(Integer.parseInt(FirebaseFetchService.getVibration()));

        modeSpinnerList = new ArrayList<String>();
        modeSpinnerList.add("Terse");
        modeSpinnerList.add("Digit");
        modeSpinnerList.add("Custom");
        ArrayAdapter<String> modeSelectArrayAdapter = new ArrayAdapter<>(this, android.R.layout.simple_spinner_dropdown_item, modeSpinnerList);
        modeSelectArrayAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        modeSelectSpinner.setAdapter(modeSelectArrayAdapter);

        modeSelectSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                Toast.makeText(getApplicationContext(), "ID: "+position, Toast.LENGTH_SHORT).show();
                if (position == 0)
                {
                    setEnableAllEditTexts(false);
                    modeSaveButton.setEnabled(false);
                    modeSaveButton.setVisibility(View.INVISIBLE);
                    hourLongBuzzEditText.setText("10");
                    hourShortBuzzEditText.setText("1");
                    minuteLongBuzzEditText.setText("10");
                    minuteShortBuzzEditText.setText("1");
                }
                else if (position == 1)
                {
                    setEnableAllEditTexts(false);
                    modeSaveButton.setEnabled(false);
                    modeSaveButton.setVisibility(View.INVISIBLE);
                    hourLongBuzzEditText.setText("5");
                    hourShortBuzzEditText.setText("1");
                    minuteLongBuzzEditText.setText("15");
                    minuteShortBuzzEditText.setText("1");
                }
                else if (position == 2)
                {
                    setEnableAllEditTexts(true);
                    modeSaveButton.setEnabled(true);
                    modeSaveButton.setVisibility(View.VISIBLE);
                }
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

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

    private void setEnableAllEditTexts(boolean bool)
    {
        hourLongBuzzEditText.setEnabled(bool);
        hourShortBuzzEditText.setEnabled(bool);
        minuteLongBuzzEditText.setEnabled(bool);
        minuteShortBuzzEditText.setEnabled(bool);
    }
}
