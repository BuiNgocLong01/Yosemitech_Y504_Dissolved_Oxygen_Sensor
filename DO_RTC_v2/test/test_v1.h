#include <main.h>
#include <configs.h>

// init SD Card
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

// ---------------------------------------------------------------------------
// Main setup function
// ---------------------------------------------------------------------------
void setup()
{

  pinMode(PwrPin, OUTPUT);
  digitalWrite(PwrPin, HIGH);

  if (DEREPin > 0)
    pinMode(DEREPin, OUTPUT);

  Serial.begin(115200);     // Main serial port for debugging via USB Serial Monitor
  modbusSerial.begin(9600); // The modbus serial stream - Baud rate MUST be 9600.

  // Bluetooth device name
  // SerialBT.begin("ESP32");
  Serial.println("The device started, now you can pair it with bluetooth!");

  Wire.begin();
  rtc.refresh();

  // // Setup Wifi and Thingspeak
  // WiFi.mode(WIFI_STA);
  // ThingSpeak.begin(client); // Khởi tạo kết nối với ThingSpeak

  // // Kết nối Wi-Fi
  // WiFi.begin(ssid, password);
  // Serial.print("Connecting to Wi-Fi");
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println("\nConnected to Wi-Fi");

  // millis
  timer1 = millis();
  timer2 = millis();

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

  // Start up the sensor
  sensor.begin(model, modbusAddress, &modbusSerial, DEREPin);

  //  // Turn on debugging
  sensor.setDebugStream(&Serial);
  //
  //  // Start up note
  Serial.println("Yosemitech ");
  Serial.println(sensor.getModel());
  Serial.println(sensor.getParameter());
  Serial.println("Sensor");

  float hardwareV, softwareV;
  sensor.getVersion(hardwareV, softwareV);
  Serial.println("Hardware Version:");
  Serial.println(hardwareV);
  Serial.println("Software Version:");
  Serial.println(softwareV);

  //  // Get the sensor serial number
  String SN = sensor.getSerialNumber();
  Serial.println("Serial Number:");
  Serial.print(SN);
  delay(3000);

  // Get the sensor calibration status (pH only)
  if (model == Y532)
  {
    byte status = sensor.pHCalibrationStatus();
    Serial.println("Calibration Status:");
    Serial.print("0x0");
    Serial.println(status, HEX);
    delay(3000);
  }

  // Get the sensor's current calibration values
  if (model != Y532)
  {
    float Kval = 0;
    float Bval = 0;
    sensor.getCalibration(Kval, Bval);
    Serial.println("Current Calibration Equation:");
    Serial.print(Kval);
    Serial.print("*raw + ");
    Serial.println(Bval);
    delay(3000);
  }

  if (model == Y511 || model == Y513 || model == Y514)
  {
    // Check the wiper timing
    uint16_t interval = sensor.getBrushInterval();
    Serial.println("Sensor auto-cleaning interval: ");
    Serial.print(interval);
    Serial.println(" minutes");
    delay(3000);
  }

  Serial.println("Starting sensor measurements");
  success = sensor.startMeasurement();
  if (success)
    Serial.println("    Measurements started.");
  else
    Serial.println("    Failed to start measuring!");

  Serial.println("Allowing sensor to stabilize..");
  //
  if (model == Y511 || model == Y513 || model == Y514)
  {

    Serial.println("Activating brush.");
    success = sensor.activateBrush();
    if (success)
      Serial.println("    Brush activated.");
    else
      Serial.println("    Failed to activate brush!");
  }
  if (model == Y511 || model == Y513 || model == Y514 || model == Y510)
  {
    Serial.println("Continuing to stabilize..");
  }

  // Config SD Card
  if (!SD.begin())
  {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE)
  {
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC)
  {
    Serial.println("MMC");
  }
  else if (cardType == CARD_SD)
  {
    Serial.println("SDSC");
  }
  else if (cardType == CARD_SDHC)
  {
    Serial.println("SDHC");
  }
  else
  {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);

  listDir(SD, "/", 0);
}
//
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
  }
  else
  {
    Serial.println("File for today existed !");
    File dataFile = SD.open(fileName, FILE_APPEND);
    dataFile.close();
    Serial.println("File check completed !");
    Serial.println("=============================================================");
  }
}

// ---------------------------------------------------------------------------
// Main loop function
// ---------------------------------------------------------------------------
void loop()
{
  if ((unsigned long)(millis() - timer1) > 1700)
  {
    // send the command to get the values
    float parmValue, tempValue, thirdValue = -9999;
    // log data from sensor
    sensor.getValues(parmValue, tempValue, thirdValue);

    DOvalue = thirdValue;
    temp = tempValue;

    Serial.print("Do Value: ");
    Serial.println(thirdValue);
    Serial.println("-----------------");
    Serial.print("Temperature Value: ");
    Serial.print(tempValue);

    // // Gửi giá trị đến ThingSpeak
    // ThingSpeak.setField(1, thirdValue); // Gán giá trị nhiệt độ cho trường 1
    // ThingSpeak.setField(2, tempValue); // Gán giá trị độ ẩm cho trường 2
    // ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey); // Gửi dữ liệu

    // clear display
    display.clearDisplay();
    // display DO Value
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print("DO Value: ");
    display.setTextSize(2);
    display.setCursor(0, 10);
    display.print(thirdValue);
    display.print(" mg/l");

    // display Temperature
    display.setTextSize(1);
    display.setCursor(0, 35);
    display.print("Temparature: ");
    display.setTextSize(2);
    display.setCursor(0, 45);
    display.print(tempValue);
    display.print(" ");
    display.setTextSize(1);
    display.cp437(true);
    display.write(167);
    display.setTextSize(2);
    display.print("C");

    display.display();

    timer1 = millis();
  }

  // Print Data to Oled
  if ((unsigned long)(millis() - timer2) > 2000)
  {

    // SD Card
    rtc.refresh();

    Serial.printf("\nNow is : %d-%02d-%02d %02d:%02d:%02d", rtc.year() + 2000, rtc.month(), rtc.day(), rtc.hour(), rtc.minute(), rtc.second());

    // gan du lieu thoi gian
    now_data.year = rtc.year() + 2000;
    now_data.month = rtc.month();
    now_data.day = rtc.day();
    now_data.hour = rtc.hour();
    now_data.minute = rtc.minute();
    now_data.second = rtc.second();

    // create file name
    sprintf(fileName, "/%04d-%02d-%02d-%02d.txt", now_data.year, now_data.month, now_data.day, now_data.hour);
    Serial.print("\nFile name is: ");
    Serial.println(fileName);

    sprintf(data_, "%04d-%02d-%02d,%02d-%02d-%02d,%0.02f,%0.02f\n", now_data.year, now_data.month, now_data.day, now_data.hour, now_data.minute, now_data.second, DOvalue, temp);
    Serial.print("Data content: ");
    Serial.println(data_);

    // SerialBT.println(data_);

    // Kiem tra xem co file name theo ngay
    SD_make_file();
    appendFile(SD, fileName, data_);

    timer2 = millis();
  }
}