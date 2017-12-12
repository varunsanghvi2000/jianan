package com.example.vikrant.battle_bot;

import android.app.Activity;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ToggleButton;

import java.net.SocketException;

public class MainActivity extends Activity {
    private SeekBar left_bar_var = null;
    private SeekBar right_bar_var = null;
    int team_color = 1;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        final TextView Red_nexus_health = (TextView) findViewById(R.id.Red_nexus_health);
        final TextView Red1 = (TextView) findViewById(R.id.Red1);
        final TextView Red2 = (TextView) findViewById(R.id.Red2);
        final TextView Red3 = (TextView) findViewById(R.id.Red3);
        final TextView Red4 = (TextView) findViewById(R.id.Red4);
        final TextView Blue_nexus_health = (TextView) findViewById(R.id.Blue_nexus_health);
        final TextView Blue1 = (TextView) findViewById(R.id.Blue1);
        final TextView Blue2 = (TextView) findViewById(R.id.Blue2);
        final TextView Blue3 = (TextView) findViewById(R.id.Blue3);
        final TextView Blue4 = (TextView) findViewById(R.id.Blue4);
        final TextView own_health = (TextView) findViewById(R.id.own_health);

        Button weapon_up_left, weapon_down_left, weapon_up_right, weapon_down_right;
        Button light1, light2, light3;
        Button sound1, sound2, sound3;


        weapon_up_left = (Button) findViewById(R.id.button_weapon_up);
        weapon_down_left = (Button) findViewById(R.id.button_weapon_down);
        weapon_up_right = (Button) findViewById(R.id.button_weapon_up_1);
        weapon_down_right = (Button) findViewById(R.id.button_weapon_down_1);
        light1 = (Button) findViewById(R.id.light1);
        light2 = (Button) findViewById(R.id.light2);
        light3 = (Button) findViewById(R.id.light3);
        sound1 = (Button) findViewById(R.id.sound1);
        sound2 = (Button) findViewById(R.id.sound2);
        sound3 = (Button) findViewById(R.id.sound3);

        new Thread(new Runnable() {
            @Override
            public void run() {
                while (true) {
                    try {
                        Thread.sleep(100);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    String textstring = null;
                    try {
                        textstring = udp_client_class.recieve_health();
                    } catch (SocketException e) {
                        e.printStackTrace();
                    }
                    final String finalTextstring = textstring;

                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            if (!finalTextstring.equals("")) {


                                //stuff that updates ui
                                Red_nexus_health.setText(finalTextstring.substring(0, 3));
                                Red1.setText(finalTextstring.substring(3, 5));
                                Red2.setText(finalTextstring.substring(5, 7));
                                Red3.setText(finalTextstring.substring(7, 9));
                                Red4.setText(finalTextstring.substring(9, 11));
                                Blue_nexus_health.setText(finalTextstring.substring(11, 14));
                                Blue1.setText(finalTextstring.substring(14, 16));
                                Blue2.setText(finalTextstring.substring(16, 18));
                                Blue3.setText(finalTextstring.substring(18, 20));
                                Blue4.setText(finalTextstring.substring(20, 22));

                                //update health
                                EditText edit = (EditText) findViewById(R.id.team_number);

                                String result = edit.getText().toString();
                                if (result.equals("")) {
                                    result = "1";
                                }
                                int finalValue = Integer.parseInt(result);
                                if (team_color == 1) {
                                    switch (finalValue) {
                                        case 1:
                                            Red1.getText();
                                            own_health.setText(Red1.getText());
                                            break;
                                        case 2:
                                            Red2.getText();
                                            own_health.setText(Red2.getText());
                                            break;
                                        case 3:
                                            Red3.getText();
                                            own_health.setText(Red3.getText());
                                            break;
                                        case 4:
                                            Red4.getText();
                                            own_health.setText(Red4.getText());
                                            break;
                                        default:
                                            Red1.getText();
                                            own_health.setText(Red1.getText());
                                    }
                                } else if (team_color == 2) {

                                    switch (finalValue) {
                                        case 1:
                                            Blue1.getText();
                                            own_health.setText(Blue1.getText());
                                            break;
                                        case 2:
                                            Blue2.getText();
                                            own_health.setText(Blue2.getText());
                                            break;
                                        case 3:
                                            Blue3.getText();
                                            own_health.setText(Blue3.getText());
                                            break;
                                        case 4:
                                            Blue4.getText();
                                            own_health.setText(Blue4.getText());
                                            break;
                                        default:
                                            Blue1.getText();
                                            own_health.setText(Blue1.getText());

                                    }

                                }


                            }
                        }});

                }
            }
        }).start();

        left_bar_var = (SeekBar) findViewById(R.id.left_bar);

        left_bar_var.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            int progressChanged = 0;

            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                progressChanged = progress;
                udp_client_class.send_msg(1, progressChanged);
                //Red_nexus_health.setText(String.valueOf(progressChanged));
                //Toast.makeText(MainActivity.this,"seek bar progress:"+progressChanged,
                //        Toast.LENGTH_SHORT).show();
            }

            public void onStartTrackingTouch(SeekBar seekBar) {
                // TODO Auto-generated method stub
            }

            public void onStopTrackingTouch(SeekBar seekBar) {
//                Toast.makeText(MainActivity.this,"seek bar progress:"+progressChanged,
//                        Toast.LENGTH_SHORT).show();
                left_bar_var.setProgress(5);
            }
        });

        right_bar_var = (SeekBar) findViewById(R.id.right_bar);

        right_bar_var.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            int progressChanged1 = 0;

            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                progressChanged1 = progress;
                udp_client_class.send_msg(2, progressChanged1);
                //udp_client_class.send_msg(8, progressChanged1*10);
                //Blue_nexus_health.setText(String.valueOf(progressChanged1));
                //Toast.makeText(MainActivity.this,"seek bar progress:"+progressChanged,
                //        Toast.LENGTH_SHORT).show();
            }

            public void onStartTrackingTouch(SeekBar seekBar) {
                // TODO Auto-generated method stub
            }

            public void onStopTrackingTouch(SeekBar seekBar) {
                /*Toast.makeText(MainActivity.this,"seek bar progress:"+progressChanged1,
                        Toast.LENGTH_SHORT).show();*/
                right_bar_var.setProgress(5);
            }
        });


        //buttons
        //weapon_up_left=(Button)findViewById(R.id.button);

        weapon_up_left.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                udp_client_class.send_msg(3, 1);


                //Toast.makeText(MainActivity.this,textstring,Toast.LENGTH_LONG).show();
            }
        });

        weapon_down_left.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                udp_client_class.send_msg(3, 2);
                Toast.makeText(MainActivity.this, "YOUR MESSAGE", Toast.LENGTH_LONG).show();
            }
        });

        weapon_up_right.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                udp_client_class.send_msg(4, 1);
                Toast.makeText(MainActivity.this, "YOUR MESSAGE", Toast.LENGTH_LONG).show();
            }
        });

        weapon_down_right.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                udp_client_class.send_msg(4, 2);
                Toast.makeText(MainActivity.this, "YOUR MESSAGE", Toast.LENGTH_LONG).show();
            }
        });

        light1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                udp_client_class.send_msgtoslave (5, 1);
                Toast.makeText(MainActivity.this, "YOUR MESSAGE", Toast.LENGTH_LONG).show();
            }
        });

        light2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                udp_client_class.send_msgtoslave(5, 2);
                Toast.makeText(MainActivity.this, "YOUR MESSAGE", Toast.LENGTH_LONG).show();
            }
        });

        light3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                udp_client_class.send_msgtoslave(5, 3);
                Toast.makeText(MainActivity.this, "YOUR MESSAGE", Toast.LENGTH_LONG).show();
            }
        });

        sound1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                udp_client_class.send_msgtoslave(8, 1);
                Toast.makeText(MainActivity.this, "YOUR MESSAGE", Toast.LENGTH_LONG).show();
            }
        });
        sound2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                udp_client_class.send_msgtoslave(8,2 );
                Toast.makeText(MainActivity.this, "YOUR MESSAGE", Toast.LENGTH_LONG).show();
            }
        });
        sound3.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                udp_client_class.send_msgtoslave(8, 3);
                Toast.makeText(MainActivity.this, "YOUR MESSAGE", Toast.LENGTH_LONG).show();
            }
        });

        //toggle button
        ToggleButton toggle = (ToggleButton) findViewById(R.id.toggleButton);
        toggle.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    // The toggle is enabled
                    team_color = 1;
                    udp_client_class.send_msg(7, 1);
                } else {
                    udp_client_class.send_msg(7, 2);
                    team_color = 2;
                    // The toggle is disabled
                }
            }
        });

        own_health.addTextChangedListener(new TextWatcher() {

            @Override
            public void afterTextChanged(Editable s) {}

            @Override
            public void beforeTextChanged(CharSequence s, int start,
                                          int count, int after) {
            }

            @Override
            public void onTextChanged(CharSequence s, int start,
                                      int before, int count) {
                /*if(s.length() != 0) {
                    if(!((s.toString()).isEmpty()))
                    {
                        s="99";
                    }
*/
                        udp_client_class.send_msg(8, Float.parseFloat(s.toString()));

              //  }
            }
        });





    }


}