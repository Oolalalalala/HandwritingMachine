#include <Wire.h>
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header

hd44780_I2Cexp lcd; // declare lcd object: auto locate & auto config expander chip

// LCD geometry
const int LCD_COLS = 20;
const int LCD_ROWS = 4;

void setup()
{
   lcd.begin(LCD_COLS, LCD_ROWS);
   lcd.clear();
   lcd.print("Hello World");
   lcd.setCursor(0, 1);
   lcd.print("Millis ");
}

void loop()
{
   updateLCD();
}



void updateLCD()
{
   static unsigned long lcdTimer = 0;
   unsigned long lcdInterval = 500;  // update 2 times per second
   if (millis() - lcdTimer >= lcdInterval)
   {
      lcdTimer = millis();
      lcd.setCursor(8, 1);
      lcd.print("       "); // overwrite old data
      lcd.setCursor(8, 1);  // reset the cursor
      lcd.print(millis());
   }
}