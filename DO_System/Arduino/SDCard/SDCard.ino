/*
   Connect the SD card to the following pins:
*/

#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include <time.h>
#include <WiFi.h>

const char *ssid = "TP-LINK_03";
const char *password = "12341234";

long timezone = 7;
byte daysavetime = 1;

char data_[30];
struct tm tmstruct;
struct
{
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
} now_data;

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
      time_t t = file.getLastWrite();
      struct tm *tmstruct = localtime(&t);
      Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
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
      time_t t = file.getLastWrite();
      struct tm *tmstruct = localtime(&t);
      Serial.printf("  LAST WRITE: %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct->tm_year) + 1900, (tmstruct->tm_mon) + 1, tmstruct->tm_mday, tmstruct->tm_hour, tmstruct->tm_min, tmstruct->tm_sec);
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

// Variable
char fileName[30];
// millis
unsigned long timer1;

void setup()
{
  Serial.begin(115200);

  //
  timer1 = millis();

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Contacting Time Server");
  configTime(3600 * timezone, daysavetime * 3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");

  delay(2000);
  tmstruct.tm_year = 0;

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
  // removeDir(SD, "/mydirSensor");
  // createDir(SD, "/mydirSensor");
  // deleteFile(SD, "/hello.txt");
  // writeFile(SD, "/DayTime.txt", "Let's go -->");
  // appendFile(SD, "/hello.txt", "World!\n");
  // listDir(SD, "/", 0);
}
//
void SD_make_file()
{
  // get_file_name();
  if (!SD.exists(fileName))
  {
    Serial.println("No file for today yet !");
    File dataFile = SD.open(fileName, FILE_WRITE);
    dataFile.print("Date,Time\n");
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

/*
 * Long Automation
 */

void loop()
{
  getLocalTime(&tmstruct, 5000);

  if ((unsigned long)(millis() - timer1 >= 1000))
  {

    Serial.printf("\nNow is : %d-%02d-%02d %02d:%02d:%02d", (tmstruct.tm_year) + 1900, (tmstruct.tm_mon) + 1, tmstruct.tm_mday, tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec);
    // gan du lieu thoi gian
    now_data.year = (tmstruct.tm_year) + 1900;
    now_data.month = (tmstruct.tm_mon) + 1;
    now_data.day = (tmstruct.tm_mday);
    now_data.hour = (tmstruct.tm_hour);
    now_data.minute = (tmstruct.tm_min);
    now_data.second = (tmstruct.tm_sec);

    // create file name
    sprintf(fileName, "/%04d-%02d-%02d-%02d-%02d.txt", now_data.year, now_data.month, now_data.day, now_data.hour, now_data.minute);
    Serial.print("File name is: ");
    Serial.println(fileName);

    sprintf(data_, "%04d-%02d-%02d,%02d-%02d-%02d\n", now_data.year, now_data.month, now_data.day, now_data.hour, now_data.minute, now_data.second);
    Serial.print("Data content: ");
    Serial.println(data_);
    
    // Kiem tra xem co file name theo ngay ?
    SD_make_file();

    appendFile(SD, fileName, data_);
    // delay(1000);

    timer1 = millis();
  }
}