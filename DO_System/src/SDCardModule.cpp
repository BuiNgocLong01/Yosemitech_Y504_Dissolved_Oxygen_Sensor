#include "SDCardModule.h"
#include "OLEDModule.h"

// #define SCREEN_WIDTH 128  // OLED display width, in pixels
// #define SCREEN_HEIGHT 64  // OLED display height, in pixels
// #define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)



// Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void initializeSDCard() {
  // Config SD Card
  Wire.begin();  // Khởi tạo bus I2C
  if (!SD.begin())  {
    Serial.println("Card Mount Failed");
    //WARNING
  display.clearDisplay();
  display.setTextSize(1);    display.setCursor(0, 0);    display.print("Mount Fail");
  display.display();
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE)  {
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC)  {
    Serial.println("MMC");
  }  else if (cardType == CARD_SD)  {
    Serial.println("SDSC");
  }  else if (cardType == CARD_SDHC)  {
    Serial.println("SDHC");
  }  else  {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
}


void checkAndCreateFile(const char* fileName) {
    if (!SD.exists(fileName)) {
        File dataFile = SD.open(fileName, FILE_WRITE);
        if (dataFile) {
            dataFile.println("Date,Time,DOValue,TempValue");
            dataFile.close();
            Serial.println("New file created !");
        } else {
            Serial.println("Failed to create a new file.");
        }
    }
}

void appendToFile(const char* fileName, const char* data) {
    File file = SD.open(fileName, FILE_APPEND);
    if (file) {
        file.println(data);
        Serial.println("Data appended to file.");
    } else {
        Serial.println("Failed to open file for appending.");
    }
    file.close();
}


