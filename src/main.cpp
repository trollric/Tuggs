#include "Arduino.h"

int serial_input = 2;
int clk_pulse = 7;
int latch_pin = 8;
int button_one = 3;
int button_two = 4;
int button_three;
int button_four;
int button_five;
int button_six;
int button_seven;
int button_eight;
int button_true;
int button_false;
int button_state = 0;

char player_one_life = 3;
char player_two_life = 3;
char player_three_life = 3;
char player_four_life = 3;
char player_five_life = 3;
char player_six_life = 3;
char player_seven_life = 3;
char player_eight_life = 3;

byte shadow_first;
byte shadow_middle;
byte shadow_last;
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
bool answer;

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
    bit_wave(100);
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

int button_pressed(){ //Reads which button was pressed. Return button nr
  if(digitalRead(button_one)){
    return 1;
  }
  else if(digitalRead(button_two)){
    return 2;
  }
  else{
    return 0;
  }
}

void bit_blink_player(int player){
  switch(player){
    case 1:
      for(int i = 0; i<4; i++){
        shadow_last &= B11111000;
        bit_write(shadow_first, shadow_middle, shadow_last);
        delay(300);
        shadow_last ^= B00000111;
        bit_write(shadow_first, shadow_middle, shadow_middle);
        delay(300);
      }
    break;
    case 2:
      for(int i = 0; i<4; i++){
        shadow_last &= B11000111;
        bit_write(shadow_first, shadow_middle, shadow_last);
        delay(300);
        shadow_last ^= B00111000;
        bit_write(shadow_first, shadow_middle, shadow_middle);
        delay(300);
      }
    break;
  }
}

bool check_answer(){ //Returns 1 if the true button is pressed otherwise returns zero  //Waits for button_trye/false and then returns 1/0
  while(!(digitalRead(button_true) || digitalRead(button_false))){}
  if (digitalRead(button_true)) {
    return 1;
  }
  else if(digitalRead(button_false)){
    return 0;
  }
}

void update_shadowbytes(){
  shadow_first = first_eight;
  shadow_middle = middle_eight;
  shadow_last = last_eight;
}   //sets the shadowbytes to the same as the mainbytes

void update_life(int player){
  switch (player) {
    case 1:
      if(!check_answer()){
        player_one_life -= 1;
        if(player_one_life <= 0){
          player_one_active = false;
        }
      }
      break;
    case 2:
      if(!check_answer()){
        player_two_life -= 1;
        if(player_two_life <= 0){
          player_two_active = false;
        }
      }
      break;
    case 3:
      if(!check_answer()){
        player_three_life -= 1;
        if(player_three_life <= 0){
          player_three_active = false;
        }
      }
      break;
    case 4:
      if(!check_answer()){
        player_four_life -= 1;
        if(player_four_life <= 0){
          player_four_active = false;
        }
      }
      break;
    case 5:
      if(!check_answer()){
        player_five_life -= 1;
        if(player_five_life <= 0){
          player_five_active = false;
        }
      }
      break;
    case 6:
      if(!check_answer()){
        player_six_life -= 1;
        if(player_six_life <= 0){
          player_six_active = false;
        }
      }
      break;
    case 7:
      if(!check_answer()){
        player_seven_life -= 1;
        if(player_seven_life <= 0){
          player_seven_active = false;
        }
      }
      break;
    case 8:
      if(!check_answer()){
        player_eight_life -= 1;
        if(player_eight_life <= 0){
          player_eight_active = false;
        }
      }
      break;
  }
}

void check_for_inputs(){ //reads the buttons and blinks the highlighted player
  while(!((digitalRead(button_one) && player_one_active) || (digitalRead(button_two) && player_two_active))){}
  button_state = button_pressed();
  switch(button_state){
    case 1:
      bit_blink_player(button_state);
      break;
    case 2:
      bit_blink_player(button_state);
      break;
  }
  update_life(button_state);
}

void update_gamelogic(){  //Uppdates the three main bits

}

void loop(){
  //bit_test();
  game_intro();
  while(!game_finished){
    update_shadowbytes();
    check_for_inputs();
    update_gamelogic();
    bit_write(first_eight, middle_eight, last_eight);
  }
}
