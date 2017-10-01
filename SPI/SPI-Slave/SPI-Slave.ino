#include <SPI.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(6, 7, 2, 3, 4, 5);

#define PORT_SPI    PORTB
#define DDR_SPI     DDRB
#define DD_EN    DDB4
#define DD_MISO     DDB3
#define DD_MOSI     DDB2
#define DD_SS       DDB0
#define DD_SCK      DDB1

int display_count = 0;
int SPI_STC_vect_count = 0;
unsigned long last_display_time = 0;

char buf [1025];

void spi_init(void) {

  // SETUP ENABLE OUTPUT LOW
  DDRB |= (1 << DD_EN);
  PORTB &= ~(1 << DD_EN);
  
  DDR_SPI &= ~((1<<DD_MOSI)|(1<<DD_MISO)|(1<<DD_SS)|(1<<DD_SCK));
  // Define the following pins as output
  DDR_SPI |= (1<< DD_MISO);

  SPCR = ((1<<SPE)|               // SPI Enable
          (1<<SPIE)|              // SPI Interupt Enable
          (0<<DORD)|              // Data Order (0:MSB first / 1:LSB first)
          (0<<MSTR)|              // Master/Slave select   
          (1<<SPR1)|(1<<SPR0)|    // SPI Clock Rate
          (0<<CPOL)|              // Clock Polarity (0:SCK low / 1:SCK hi when idle)
          (0<<CPHA));             // Clock Phase (0:leading / 1:trailing edge sampling
  
}

void setup (void)
{
  Serial.begin(115200);

  // LCD Setup
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0,0);
  //         1234567890123456
  lcd.print("SPI Slave:");
  lcd.setCursor(0,1);
  //         1234567890123456
  lcd.print("Setup");
  
  spi_init();
}

void displayValue(int value)
{
  Serial.print(value);
  Serial.print(":buf: (");
  Serial.print(buf);
  Serial.println(")");

  if (buf[0]) {
    lcd.clear();
    lcd.setCursor(0,0);
    //         1234567890123456
    lcd.print("SPI Slave: ");
    lcd.print(value);
    lcd.setCursor(0,1);
    buf[strlen(buf)-1] = '\0';
    lcd.print(buf);
  }
  
  buf[0] = '\0';            // reset buffer
  SPI_STC_vect_count = 0;   // reset SPI interrupt counter
  display_count++;
}

ISR (SPI_STC_vect)
{
  int index = SPI_STC_vect_count % 1024;
  byte value = SPDR; 
  buf[index] = value;
  buf[index+1] = '\0';

  SPDR = 'a';
  SPI_STC_vect_count++;
}


void loop(void) {
  unsigned long now = millis() / 1000;
  if (now - last_display_time > 3) {
    displayValue(display_count);
    last_display_time = now;
  }
}
