#ifndef TF_SETUP_H
#define TF_SETUP_H

#include <Arduino.h>
#include <FS.h>

void WriteFile(fs::FS &fs, const char *path, uint8_t *buf, int len);
void ReadFile(fs::FS &fs, const char *path, uint8_t *buf, int len);
void DeleteFile(fs::FS &fs, const char *path);
void RenameFile(fs::FS &fs, const char *oldPath, const char *newPath);
void ListDir(fs::FS &fs, const char *dirname, uint8_t levels);
void initTFCard();

#endif // TF_SETUP_H