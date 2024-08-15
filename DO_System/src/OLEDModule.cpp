#include "OLEDModule.h"



Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void initializeOLED() {
  Wire.begin();
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  // Initialize with the I2C addr 0x3C
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);  // Don't proceed, loop forever
  }
  display.display();
  delay(2000);  // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0,0);     // Start at top-left corner
}

void displaySensorData(float temperature, float dissolvedOxygen) {
  display.clearDisplay();
  display.setTextSize(1);    display.setCursor(0, 0);    display.print("DO Value: ");
  display.setTextSize(2);    display.setCursor(0, 10);    display.print(dissolvedOxygen);    display.print(" mg/l");

  // display Temperature
  display.setTextSize(1);    display.setCursor(0, 25);    display.print("Temparature: ");
  display.setTextSize(2);    display.setCursor(0, 35);    display.print(temperature);    display.print(" ");
  display.setTextSize(1);    display.cp437(true);    display.write(167);
  display.setTextSize(2);    display.print("C");
  display.setTextSize(1);    display.setCursor(0, 55);    display.print("Lecturer: Dr. Hien Vo");
  display.display();
}
