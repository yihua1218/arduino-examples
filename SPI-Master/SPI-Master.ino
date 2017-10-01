#include <LiquidCrystal.h>
#include "SPI.h"

#define DATAOUT 11//MOSI
#define DATAIN  12//MISO 
#define SPICLOCK  13//sck
#define SLAVESELECT 10//ss

byte clr;
//data buffer
char buffer[128];
int send_count = 0;
unsigned long last_sent_time = 0; 

LiquidCrystal lcd(6, 7, 2, 3, 4, 5);

void spi_setup() {
  pinMode(DATAOUT, OUTPUT);
  pinMode(DATAIN, INPUT);
  pinMode(SPICLOCK,OUTPUT);
  pinMode(SLAVESELECT,OUTPUT);
  digitalWrite(SLAVESELECT,HIGH); //disable device

  // SPCR = 01010000
  //interrupt disabled,spi enabled,msb 1st,master,clk low when idle,
  //sample on leading edge of clk,system clock/4 rate (fastest)
  
  SPCR = (1<<SPE)|(1<<MSTR);
  clr=SPSR;
  clr=SPDR;
  delay(10);
}

char spi_transfer(volatile char data) {
  SPDR = data;                    // Start the transmission
  while (!(SPSR & (1<<SPIF)))     // Wait for the end of the transmission
  {
  };
  return SPDR;                    // return the received byte
}

void fill_buffer() {
  int i = 0;
  for (char ch = 'a'; ch <= 'z'; i++, ch++) {
    buffer[i] = ch;
  }
}

void setup() {
  Serial.begin(9600);
  
  // put your setup code here, to run once:
  /* pinMode(ss, OUTPUT);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);*/
  spi_setup();

  // LCD
  lcd.begin(16, 2);
  lcd.clear();
  //         1234567890123456
  lcd.print("SPI Master:");
  lcd.setCursor(0,1);
  //         1234567890123456
  lcd.print("Setup");
}

void displayValue(int value)
{
  lcd.clear();
  //         1234567890123456
  lcd.print("SPI Master: ");
  lcd.print(value);
}

void sendData()
{
  digitalWrite(SLAVESELECT,LOW);
  for (int i= 0; i < 26; i++) {
    spi_transfer(buffer[i]);  //write data byte
  }
  displayValue(send_count);
  send_count++;
  digitalWrite(SLAVESELECT,HIGH); //release chip
}

void loop() {
  unsigned long now = millis() / 1000;
  if (now - last_sent_time > 3) {
    sendData();
    last_sent_time = now;
  }
}
