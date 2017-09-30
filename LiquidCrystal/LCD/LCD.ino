#include <LiquidCrystal.h>

LiquidCrystal lcd(6, 7, 2, 3, 4, 5);

int loop_count = 0;

void nice_to_meet_you(){
  lcd.clear();
  lcd.setCursor(0,0);
  //         1234567890123456
  lcd.print("Arduino Uno");
  lcd.setCursor(0,1);
  //         1234567890123456
  lcd.print("Nice to meet you");
}

void setup(){
  lcd.begin(16, 2);
  lcd.noAutoscroll();
  nice_to_meet_you();
}

void loop(){
  if (loop_count % 10000 == 0 && 0) {
    nice_to_meet_you(); 
  }
  loop_count++;
}


