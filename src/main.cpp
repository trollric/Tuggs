#include "Arduino.h"

int serial_input = 2;
int clk_pulse = 7;
int latch_pin = 8;
int button_one = 3;
int button_two = 4;
int button_state = 0;

byte last_eight = B11111111;
byte middle_eight = B11111111;
byte first_eight = B11111111;
byte bits_clear = B00000000;

bool game_finished = false;
bool player_one_active = true;
bool player_two_active = true;
bool player_three_active = true;
bool player_four_active = true;
bool player_five_active = true;
bool player_six_active = true;
bool player_seven_active = true;
bool player_eight_active = true;

void setup(){
  pinMode(serial_input, OUTPUT);
  pinMode(clk_pulse, OUTPUT);
  pinMode(latch_pin, OUTPUT);

  pinMode(button_one, INPUT);
  pinMode(button_two, INPUT);

  //Light up all leds in the beginning
  digitalWrite(latch_pin, LOW);
  shiftOut(serial_input, clk_pulse, MSBFIRST, first_eight);
  shiftOut(serial_input, clk_pulse, MSBFIRST, middle_eight);
  shiftOut(serial_input, clk_pulse, MSBFIRST, last_eight);
  digitalWrite(latch_pin, HIGH);
}

void bit_clear(){
  digitalWrite(latch_pin, LOW);
  shiftOut(serial_input, clk_pulse, MSBFIRST, bits_clear);
  shiftOut(serial_input, clk_pulse, MSBFIRST, bits_clear);
  shiftOut(serial_input, clk_pulse, MSBFIRST, bits_clear);
  digitalWrite(latch_pin, HIGH);
}

void bit_write(byte first,byte middle,byte last){
  digitalWrite(latch_pin, LOW);
  shiftOut(serial_input, clk_pulse, MSBFIRST, first);
  shiftOut(serial_input, clk_pulse, MSBFIRST, middle);
  shiftOut(serial_input, clk_pulse, MSBFIRST, last);
  digitalWrite(latch_pin, HIGH);
}

void bit_shift_single(bool one_bit){
  digitalWrite(latch_pin, LOW);
  digitalWrite(clk_pulse, LOW);
  digitalWrite(serial_input, one_bit);
  digitalWrite(clk_pulse, HIGH);
  digitalWrite(serial_input, LOW);
  digitalWrite(latch_pin, HIGH);
}

void bit_wave(int hold_time){
  delay(hold_time);
  bit_shift_single(1);
  delay(hold_time);
  bit_shift_single(0);
  delay(hold_time);
  bit_shift_single(0);
}

void game_intro(){
  for(int i =0;i<6;i++){
    bit_clear();
    delay(250);
    bit_write(first_eight, middle_eight, last_eight);
    delay(250);
  }
  bit_clear();
  for(int i = 0;i<24;i++){
    bit_wave(250);
  }
  bit_clear();
  delay(100);
  bit_write(first_eight, middle_eight, last_eight);
}

void bit_test(){
  delay(1000);
  bit_write(first_eight, middle_eight, last_eight);
  delay(1000);
  bit_write(bits_clear, middle_eight, last_eight);
  delay(1000);
  bit_write(first_eight, bits_clear, last_eight);
  delay(1000);
  bit_write(first_eight, middle_eight, bits_clear);

  for(int i =0; i<5;i++){
    delay(500);
    bit_clear();
    delay(500);
    bit_write(first_eight, middle_eight, last_eight);
  }

  for (int i = 0; i<36;i++){
    bit_wave(100);
  }
}

int button_pressed(){
  return 1;
}

void loop(){
  //bit_test();
  game_intro();
  while(!game_finished){
    while(!((digitalRead(button_one) && player_one_active) || (digitalRead(button_two) && player_two_active))){}
    button_pressed =
    switch()
  }
}
