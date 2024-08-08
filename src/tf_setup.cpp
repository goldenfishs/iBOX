#include "tf_setup.h"
#include <FS.h>
#include <SD.h>
#include <SD_MMC.h>

void WriteFile(fs::FS &fs, const char *path, uint8_t *buf, int len)
{
  Serial.printf("Writing to [%s]...\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  
  if (!file.write(buf, len)) {
    Serial.println("Write failed");
    return;
  }

  file.flush();
  file.close();

  Serial.printf("Write to [%s] Complete\n", path);
}

void ReadFile(fs::FS &fs, const char *path, uint8_t *buf, int len)
{
  Serial.printf("Reading from [%s]...\n", path);

  File file = fs.open(path);
  if (!file) {
    Serial.println("Failed to open file for reading");
    return;
  }

  if (!file.read(buf, len)) {
    Serial.println("Read failed");
    return;
  }
  
  file.close();

  Serial.printf("Read from [%s] Complete: %s\n", path, buf);
}

void DeleteFile(fs::FS &fs, const char *path)
{
  Serial.printf("Deleting [%s]...\n", path);

  if (!fs.remove(path)) {
    Serial.println("Delete failed");
    return;
  }

  Serial.printf("Delete [%s] Complete\n", path);
}

void RenameFile(fs::FS &fs, const char *oldPath, const char *newPath)
{
  Serial.printf("Renaming [%s] to [%s]...\n", oldPath, newPath);

  if (!fs.rename(oldPath, newPath)) {
    Serial.println("Rename failed");
    return;
  }

  Serial.printf("Rename [%s] to [%s] Complete\n", oldPath, newPath);
}

void ListDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
  Serial.printf("Listing directory: %s\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("Not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        ListDir(fs, file.name(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}


void initTFCard()
{
  /* SD SPI Mode at VSPI */
  SPIClass spi = SPIClass(HSPI);
  spi.begin(47 /* SCK */, 21 /* MISO */, 48 /* MOSI */, 45 /* SS */);
  if (!SD.begin(45 /* SS */, spi, 80000000)) {
    Serial.println("Card Mount Failed");
    return;
  }
  SD_MMC.end(); // 取消SD挂载
}