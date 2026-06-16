#include "db_memory.h"
#include "HardwareSerial.h"
#include "esp_err.h"
#include <cstdint>
#include <esp_littlefs.h>
#include <string_view>
#include <sys/types.h>

Database::Database()
    : filepath(DEFAULT_FILEPATH), isChanged(false), lastSaveTime(0),
      saveInterval(DEFAULT_SAVE_INTERVAL) {}

bool Database::begin() {
  Serial.println("Initializing LittleFS Database");

  esp_vfs_littlefs_conf_t conf = {.base_path = BASE_PATH,
                                  .partition_label = PARTITION_LABEL,
                                  .format_if_mount_failed = true,
                                  .dont_mount = false};

  esp_err_t ret = esp_vfs_littlefs_register(&conf);

  if (ret != ESP_OK) {
    if (ret == ESP_FAIL) {
      Serial.println("Mounting LittleFS failed");
    } else if (ret == ESP_ERR_NOT_FOUND) {
      Serial.println("LittleFS partition not found");
    } else {
      Serial.printf("LittleFS init error: %s\n", esp_err_to_name(ret));
    }
    return false;
  }

  size_t total = 0, used = 0;
  if (esp_littlefs_info(conf.partition_label, &total, &used) == ESP_OK) {
    Serial.printf("LittleFS: %d / %d bytes used\n", used, total);
  }

  loadFromFlash();
  return true;
}

void Database::add(uint16_t id, const Data &data) {
  bool needsUpdate = false;
  bool isNewSession = false;

  auto it = _db.find(id);
  if (it == _db.end()) {
    needsUpdate = true;
    isNewSession = true;
  } else {
    std::string_view currentPlate(it->second.licenceplate.data());
    std::string_view newPlate(data.licenceplate.data());

    if (currentPlate != newPlate || it->second.created_at != data.created_at ||
        it->second.payed != data.payed) {
      needsUpdate = true;
    }
  }

  if (needsUpdate) {
    if (!freeIds.empty() && freeIds.top() == id) {
      freeIds.pop();
    } else if (id == nextId + 1) {
      nextId++;
    }
    _db[id] = data;
    isChanged = true;
    Serial.printf("In-memory DB updated: ID %u\n", id);
  }
}

std::optional<Data> Database::get(uint16_t id) {
  auto it = _db.find(id);
  if (it != _db.end()) {
    return it->second;
  }
  return std::nullopt;
}

std::optional<uint16_t> Database::findIdByPlate(const char *plate) {
  for (const auto &[id, data] : _db) {
    if (memcmp(data.licenceplate.data(), plate, 3) == 0) {
      return id;
    }
  }
  return std::nullopt;
}

bool Database::exists(uint16_t id) { return _db.find(id) != _db.end(); }

bool Database::remove(uint16_t id) {
  if (exists(id)) {
    _db.erase(id);
    isChanged = true;
    if (id < nextId)
      freeIds.push(id);
    return true;
  }
  return false;
}

// helper functions
template <typename T> static bool writeObject(FILE *f, const T &obj) {
  return fwrite(&obj, sizeof(T), 1, f) == 1;
}

template <typename T> static bool readObject(FILE *f, T &obj) {
  return fread(&obj, sizeof(T), 1, f) == 1;
}

// C
bool Database::saveToFlash() {
  FILE *file = fopen(filepath, "wb");
  if (file == NULL) {
    Serial.println("Can't open file");
    return false;
  }

  size_t totalItems = _db.size();
  writeObject(file, totalItems);

  for (const auto &pair : _db) {
    uint16_t id = pair.first;
    writeObject(file, id);
    writeObject(file, pair.second);
  }

  fclose(file);
  isChanged = false;
  lastSaveTime = millis();
  Serial.printf("%d records saved via LittleFS\n", totalItems);
  return true;
}

bool Database::loadFromFlash() {
  FILE *file = fopen(filepath, "rb");
  if (file == NULL) {
    Serial.println("No existing binary DB found, starting with empty DB");
    return false;
  }

  _db.clear();

  size_t totalItems = 0;
  if (!readObject(file, totalItems)) {
    fclose(file);
    return false;
  }

  for (size_t i = 0; i < totalItems; i++) {
    uint16_t id;
    Data d;

    if (!readObject(file, id))
      break;
    if (!readObject(file, d))
      break;

    _db[id] = d;
  }

  fclose(file);

  while (!freeIds.empty()) {
    freeIds.pop();
  }

  if (!_db.empty()) {
    uint16_t highestId = _db.rbegin()->first;

    nextId = highestId + 1;

    for (uint16_t i = 0; i < highestId; i++) {
      if (_db.find(i) == _db.end()) {
        freeIds.push(i);
      }
    }

  } else {
    nextId = 0;
  }
  Serial.printf("%d records loaded from flash\n", _db.size());
  return true;
}

void Database::autoSave() {
  if (isChanged && (millis() - lastSaveTime >= saveInterval)) {
    if (!saveToFlash()) {
      lastSaveTime = millis();
    }
  }
}

uint16_t Database::getLowestId() {
  if (!freeIds.empty()) {
    return freeIds.top();
  } else {
    return nextId + 1;
  }
}

void Database::printDatabase() {
  Serial.println("\n--- [DATABASE DUMP] ---");
  Serial.printf("Records in memory: %d\n", _db.size());

  for (const auto &[id, data] : _db) {
    Serial.printf("ID: %u | Plate: %s | Created (Unix): %u | Paid: %s\n", id,
                  data.licenceplate.data(), data.created_at,
                  data.payed ? "YES" : "NO");
  }

  size_t total = 0, used = 0;
  if (esp_littlefs_info(PARTITION_LABEL, &total, &used) == ESP_OK) {
    Serial.printf("Flash Storage (LittleFS): %d / %d bytes used\n", used,
                  total);
  }

  FILE *file = fopen(filepath, "rb");
  if (file) {
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    Serial.printf("DB File size on flash: %ld bytes\n", size);
    fclose(file);
  } else {
    Serial.println("DB File not found on flash.");
  }
  Serial.println("-----------------------\n");
}

void Database::clear() {
  _db.clear();
  while (!freeIds.empty())
    freeIds.pop();
  nextId = 0;
  isChanged = true;
  unlink(filepath);
  Serial.println("Database cleared in memory and on flash.");
}

Database db;
