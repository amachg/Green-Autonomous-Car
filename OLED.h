/*
  Waveshare OLED display Module,
  1.5inch RGB, 128x128, 16-bit high color, Driver: SSD1351
  Connected via 4-wire SPI interface, using the SPI library.
  ADK hardware SPI pins: 50 (MISO), 51 (MOSI), 52 (SCK), 53 (SS), or via the ICSP header.
*/

// Adafruit_SSD1351 Lib exist in Arduino Library Manager.
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>

// Using the much faster hardware SPI.
#define CS_PIN A0
#define DC_PIN A1

// declare the display
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 128 
Adafruit_SSD1351 oled = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, &SPI, CS_PIN, DC_PIN);

#define OLED_BLACK   0x0000
#define OLED_BLUE    0x001F
#define OLED_RED     0xF800
#define OLED_GREEN   0x07E0
#define OLED_CYAN    0x07FF
#define OLED_MAGENTA 0xF81F
#define OLED_YELLOW  0xFFE0  
#define OLED_WHITE   0xFFFF

uint16_t OLED_Text_Color      = OLED_WHITE;
uint16_t OLED_Backround_Color = OLED_BLACK;

byte screen_lines = 0;
byte screen_lines_limit = 8;  // 8 for TextSize 2

void println(const String str) {
  if(screen_lines >= screen_lines_limit) {
    oled.fillScreen(OLED_Backround_Color);
    oled.setCursor(0,0);
    screen_lines = 0;
  }
  oled.println(str);
  ++screen_lines;
  
  Serial.println(str);
}

void setup_OLED() {
  pinMode(CS_PIN, OUTPUT);
  pinMode(DC_PIN, OUTPUT);
  
  oled.begin();  // initialise the screen
  oled.fillScreen(OLED_Backround_Color);  
  oled.setTextColor(OLED_Text_Color, OLED_Backround_Color);  
  oled.setTextSize(2); // multiply the scale of the text by a given integer factor.  
  oled.setCursor(0,0);
  
  println("OLED OK");
}
