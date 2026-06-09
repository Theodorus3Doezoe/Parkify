#pragma once

#include "database/database_models.h"
#include <Arduino.h>
#include <cstdint>
#include <map>
#include <optional>
#include <queue>

class Database {
private:
  static constexpr unsigned long DEFAULT_SAVE_INTERVAL = 5000;
  static constexpr const char *DEFAULT_FILEPATH = "/littlefs/database.bin";
  static constexpr const char *BASE_PATH = "/littlefs";
  static constexpr const char *PARTITION_LABEL = "littlefs";

  std::map<uint16_t, Data> _db;
  uint16_t nextId = 0;
  std::priority_queue<uint16_t, std::vector<uint16_t>, std::greater<uint16_t>>
      freeIds;
  const char *filepath;
  bool isChanged;
  unsigned long lastSaveTime;
  unsigned long saveInterval;

  bool loadFromFlash();

public:
  Database();

  bool begin();

  void add(uint16_t id, const Data &data);
  std::optional<Data> get(uint16_t id);
  bool exists(uint16_t id);
  bool remove(uint16_t id);
  uint16_t getLowestId();

  void printDatabase();
  bool saveToFlash();
  void autoSave();
  void clear();
};

extern Database db;
