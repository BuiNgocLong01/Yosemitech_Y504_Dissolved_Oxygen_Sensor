#include <main.h>
#include <configs.h>

//init SD Card
void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root)
  {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory())
  {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print("  DIR : ");
      Serial.print(file.name());

      Serial.printf("  LAST WRITE: %04d-%02d-%02d %02d:%02d:%02d\n", rtc.year() + 2000, rtc.month(), rtc.day(), rtc.hour(), rtc.minute(), rtc.second());
      if (levels)
      {
        listDir(fs, file.path(), levels - 1);
      }
    }
    else
    {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("  SIZE: ");
      Serial.print(file.size());

      Serial.printf("  LAST WRITE: %04d-%02d-%02d %02d:%02d:%02d\n", rtc.year() + 2000, rtc.month(), rtc.day(), rtc.hour(), rtc.minute(), rtc.second());
    }
    file = root.openNextFile();
  }
}

void createDir(fs::FS &fs, const char *path)
{
  Serial.printf("Creating Dir: %s\n", path);
  if (fs.mkdir(path))
  {
    Serial.println("Dir created");
  }
  else
  {
    Serial.println("mkdir failed");
  }
}

void removeDir(fs::FS &fs, const char *path)
{
  Serial.printf("Removing Dir: %s\n", path);
  if (fs.rmdir(path))
  {
    Serial.println("Dir removed");
  }
  else
  {
    Serial.println("rmdir failed");
  }
}

void readFile(fs::FS &fs, const char *path)
{
  Serial.printf("Reading file: %s\n", path);

  File file = fs.open(path);
  if (!file)
  {
    Serial.println("Failed to open file for reading");
    return;
  }

  Serial.print("Read from file: ");
  while (file.available())
  {
    Serial.write(file.read());
  }
  file.close();
}

void writeFile(fs::FS &fs, const char *path, const char *message)
{
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file)
  {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message))
  {
    Serial.println("File written");
  }
  else
  {
    Serial.println("Write failed");
  }
  file.close();
}

void appendFile(fs::FS &fs, const char *path, const char *message)
{
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file)
  {
    Serial.println("Failed to open file for appending !");
    return;
  }
  if (file.print(message))
  {
    Serial.println("Message appended !");
  }
  else
  {
    Serial.println("Append failed !");
  }
  file.close();
}

void renameFile(fs::FS &fs, const char *path1, const char *path2)
{
  Serial.printf("Renaming file %s to %s\n", path1, path2);
  if (fs.rename(path1, path2))
  {
    Serial.println("File renamed");
  }
  else
  {
    Serial.println("Rename failed");
  }
}

void deleteFile(fs::FS &fs, const char *path)
{
  Serial.printf("Deleting file: %s\n", path);
  if (fs.remove(path))
  {
    Serial.println("File deleted");
  }
  else
  {
    Serial.println("Delete failed");
  }
}

void SD_make_file()
{
  // get_file_name();
  if (!SD.exists(fileName))
  {
    Serial.println("No file for today yet !");
    File dataFile = SD.open(fileName, FILE_WRITE);
    dataFile.print("Date,Time,DOValue,TempValue\n");
    dataFile.close();
    Serial.println("New file created !");
  }  else  {
    Serial.println("File for today existed !");
    File dataFile = SD.open(fileName, FILE_APPEND);
    dataFile.close();
    Serial.println("File check completed !");
    Serial.println("=============================================================");
  }
}


// ---------------------------------------------------------------------------
// Main setup function
// ---------------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);     // Main serial port for debugging via USB Serial Monitor
  // Bluetooth device name
  // SerialBT.begin("ESP32");
  // Serial.println("The device started, now you can pair it with bluetooth!");

  //Timer
  timer1 = millis();
  timer2 = millis();
  timer3 = millis();

  Wire.begin();
  rtc.refresh();

  // // Setup Wifi and Thingspeak
  WiFi.mode(WIFI_OFF);

  // Oled
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  delay(2000);
  display.clearDisplay();
  display.setTextColor(WHITE);


  // Config SD Card
  if (!SD.begin())  {
    Serial.println("Card Mount Failed");
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

  listDir(SD, "/", 0);
}

// ---------------------------------------------------------------------------
// Main loop function
// ---------------------------------------------------------------------------
void loop(){
  if ((unsigned long)(millis() - timer1) > 1700)  {
    timer1 = millis();
    // send the command to get the values
    float thirdValue = random(0.0, 20.0);
    float tempValue = random(0.0, 20.0);

    DOvalue = thirdValue;
    temp = tempValue;

    Serial.print("Do Value: ");    Serial.println(thirdValue);
    Serial.println("-----------------");
    Serial.print("Temperature Value: ");    Serial.print(tempValue);

    // clear display
    display.clearDisplay();
    // display DO Value
    display.setTextSize(1);    display.setCursor(0, 0);    display.print("DO Value: ");
    display.setTextSize(2);    display.setCursor(0, 10);    display.print(thirdValue);    display.print(" mg/l");

    // display Temperature
    display.setTextSize(1);    display.setCursor(0, 35);    display.print("Temparature: ");
    display.setTextSize(2);    display.setCursor(0, 45);    display.print(tempValue);    display.print(" ");
    display.setTextSize(1);    display.cp437(true);    display.write(167);
    display.setTextSize(2);    display.print("C");
    display.display();
  }

  // Print Data to Oled
  if ((unsigned long)(millis() - timer2) > 2000)  {
    timer2 = millis();

    // SD Card
    rtc.refresh();

    Serial.printf("\nNow is : %d-%02d-%02d %02d:%02d:%02d", rtc.year() + 2000, rtc.month(), rtc.day(), rtc.hour(), rtc.minute(), rtc.second());

    // gan du lieu thoi gian
    now_data.year = rtc.year() + 2000;    now_data.month = rtc.month();    now_data.day = rtc.day();
    now_data.hour = rtc.hour();    now_data.minute = rtc.minute();    now_data.second = rtc.second();

    // create file name
    sprintf(fileName, "/%04d-%02d-%02d-%02d.txt", now_data.year, now_data.month, now_data.day, now_data.hour);
    Serial.print("\nFile name is: ");    Serial.println(fileName);

    sprintf(data_, "%04d-%02d-%02d,%02d-%02d-%02d,%0.02f,%0.02f\n", now_data.year, now_data.month, now_data.day, now_data.hour, now_data.minute, now_data.second, DOvalue, temp);
    Serial.print("Data content: ");    Serial.println(data_); 

    // Bluetooth
    // SerialBT.println(data_);

    // Kiem tra xem co file name theo ngay
    SD_make_file();
    appendFile(SD, fileName, data_);    
  }

  if ((unsigned long)(millis() - timer3) > 60000)  {
    timer3 = millis();
    // Kết nối Wi-Fi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to Wi-Fi");
    unsigned long startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 10000) {
    delay(500);
    Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nWiFi connected.");
      ThingSpeak.begin(client);

      // // Gửi giá trị đến ThingSpeak
      ThingSpeak.setField(1, DOvalue); // Gán giá trị nhiệt độ cho trường 1
      ThingSpeak.setField(2, temp); // Gán giá trị độ ẩm cho trường 2
      ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey); // Gửi dữ liệu

      WiFi.disconnect();
      WiFi.mode(WIFI_OFF);
      Serial.println("WiFi disconnected.");
    } else {
      Serial.println("\nFailed to connect to WiFi. Continuing with other tasks.");
    }
  }
}