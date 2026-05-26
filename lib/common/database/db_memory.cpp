#include "db_memory.h"
#include "HardwareSerial.h"
#include "esp_err.h"
#include <cstdint>
#include <esp_littlefs.h>

Database::Database()
    : filepath("/memory_database.bin"), isChanged(false), lastSaveTime(0),
      saveInterval(100000) {}

bool Database::begin() {
  Serial.println("Initializing littlefs Database");

  esp_vfs_littlefs_conf_t conf = {.base_path = "/littlefs",
                                  .partition_label = "littlefs",
                                  .format_if_mount_failed = true,
                                  .dont_mount = false};

  esp_err_t ret = esp_vfs_littlefs_register(&conf);

  if (ret != ESP_OK) {
    if (ret == ESP_FAIL) {
      Serial.println("Mouting littleFS failed");
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

  return loadFromFlash();
}

void Database::add(uint32_t id, const Data &Data) {
  if (!_db.count(id) || strcmp(_db[id].licenceplate, Data.licenceplate) != 0 ||
      strcmp(_db[id].created_at, Data.created_at) != 0) {

    strncpy(_db[id].licenceplate, Data.licenceplate,
            sizeof(_db[id].licenceplate) - 1);
    _db[id].licenceplate[sizeof(_db[id].licenceplate) - 1] = '\0';

    strncpy(_db[id].created_at, Data.created_at,
            sizeof(_db[id].created_at) - 1);
    _db[id].created_at[sizeof(_db[id].created_at) - 1] = '\0';

    isChanged = true;
    Serial.printf("in-memory db updated: ID %u\n", id);
  }
}

std::optional<Data> Database::get(uint32_t id) {
  if (exists(id)) {
    return _db[id];
  }
  return std::nullopt;
}

bool Database::exists(uint32_t id) { return _db.find(id) != _db.end(); }

bool Database::remove(uint32_t id) {
  if (exists(id)) {
    _db.erase(id);
    isChanged = true;
    return true;
  }
  return false;
}

// C
bool Database::saveToFlash() {
  FILE *file = fopen(filepath, "wb");
  if (file == NULL) {
    Serial.println("Can't open file");
    return false;
  }

  size_t totalItems = _db.size();
  fwrite(&totalItems, sizeof(size_t), 1, file);

  for (const auto &pair : _db) {
    uint32_t id = pair.first;
    fwrite(&id, sizeof(uint32_t), 1, file);
    fwrite(&pair.second, sizeof(Data), 1, file);
  }

  fclose(file);
  isChanged = false;
  lastSaveTime = millis();
  Serial.printf("%d Records via littlefs saved\n", totalItems);
  return true;
}

bool Database::loadFromFlash() {
  FILE *file = fopen(filepath, "rb");
  if (file == NULL) {
    Serial.println("No existing binary db found, starting with empty db");
    return false;
  }

  _db.clear();

  size_t totalItems = 0;
  if (fread(&totalItems, sizeof(size_t), 1, file) != 1) {
    fclose(file);
    return false;
  }

  for (size_t i = 0; i < totalItems; i++) {
    uint32_t id;
    Data d;

    if (fread(&id, sizeof(uint32_t), 1, file) != 1)
      break;
    if (fread(&d, sizeof(Data), 1, file) != 1)
      break;

    _db[id] = d;
  }

  fclose(file);
  Serial.printf("%d records loaded from flash\n", _db.size());
  return true;
}

void Database::autoSave() {
  if (isChanged && (millis() - lastSaveTime >= saveInterval)) {
    saveToFlash();
  }
}

Database db;
