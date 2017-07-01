/*
  Bike speedometer
  Interfaces a magnetic reed switch, monochromatic OLED, and ESP8266 WiFi Adafruit Feather
  Emily Lam, June 2017
*/

#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

// Defines
#define BUTTON_A 0
#define BUTTON_B 16
#define BUTTON_C 2
#define LED      0
#define WHEEL 2105 // Circumference (mm) for 700 x 25c
#define REED 15    // Magnetic Reed Switch

Adafruit_SSD1306 display = Adafruit_SSD1306();

#if (SSD1306_LCDHEIGHT != 32)
  #error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

int speedmph = 0;
double currenttime = 0;
double priortime = 0;
double distance;

void setup() {
  // Disconnect WiFi by default to reduce power consumption, -> ~20mA
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  delay(1);

  // Serial port
  Serial.begin(9600);

  // Button 
  pinMode(REED, INPUT);
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);

  //Initiate OLED FeatherWing
  Serial.println("OLED FeatherWing test");
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // I2C addr 0x3C (for the 128x32 OLED)
  Serial.println("OLED begun");
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();
  delay(1000);
 
  // Clear the buffer.
  display.clearDisplay();
  display.display();
  
  Serial.println("IO test");
  
  // Text display tests
  display.setTextSize(5);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("22");
  display.setTextSize(3);
  display.print("MPH");
  display.display(); // actually display all of the above

  // Calculate wheel stuff (Physics 101)
  distance = WHEEL / (25.4*12*5280);
  priortime = millis();
  
}



void loop() {
  if (digitalRead(REED)) {
    currenttime = millis();
    speedmph =  (int) (distance / ((currenttime-priortime)/3600000));
    Serial.println(distance,10);
    Serial.println(priortime);
    Serial.println(currenttime);
    Serial.println(speedmph);
    priortime = currenttime;
  }
  display.clearDisplay();
  display.setTextSize(5);
  display.setCursor(0,0);
  display.print(String(speedmph));
  display.setTextSize(3);
  display.setCursor(70,0);
  display.print("MPH");
  delay(10);
  yield();
  display.display();
  delay(30);
  
}
