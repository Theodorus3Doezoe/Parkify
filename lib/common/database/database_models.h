#pragma once

#include <array>
#include <cstdint>

enum class SessionStatus : uint8_t { NOT_PAID = 0, PAID = 1, NOT_FOUND = 0xFF };

enum SpecialID : uint16_t { LOWEST_ID = 0xFFFE, NOT_FOUND = 0xFFFD };

struct Data {
  static constexpr size_t ID_SIZE = 2;
  static constexpr size_t LICENCE_PLATE_SIZE = 12;

  std::array<char, LICENCE_PLATE_SIZE> licenceplate;
  uint32_t created_at; // Unix timestamp
  bool payed;
};
