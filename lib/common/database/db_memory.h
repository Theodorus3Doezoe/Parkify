#pragma once

#include <Arduino.h>
#include <cstdint>
#include <map>
#include <optional>

struct Data {
  char licenceplate[12];
  char created_at[20];
  bool payed;
};

class Database {
private:
  std::map<uint32_t, Data> _db;
  const char *filepath;
  bool isChanged;
  unsigned long lastSaveTime;
  unsigned long saveInterval;

  bool loadFromFlash();

public:
  Database();

  bool begin();

  void add(uint32_t id, const Data &DBData);
  std::optional<Data> get(uint32_t id);
  bool exists(uint32_t id);
  bool remove(uint32_t id);

  bool saveToFlash();
  void autoSave();
};

extern Database db;
