#include "peripherals/oled_display.h"
#include <Wire.h>
#include "font5x8.h"

OledDisplay::OledDisplay(uint8_t address) : i2cAddress(address) {}

void OledDisplay::sendCommand(uint8_t command) const {
    Wire.beginTransmission(i2cAddress);
    Wire.write(0x00);
    Wire.write(command);
    Wire.endTransmission();
}

void OledDisplay::init(int sda, int scl) {
    Wire.begin(sda, scl);
    sendCommand(0xAE); // Off
    sendCommand(0x8D); // Charge Pump
    sendCommand(0x14); // Enable
    sendCommand(0xAF); // On
    clear();
}

void OledDisplay::setCursor(uint8_t col, uint8_t page) {
    sendCommand(0xB0 + page);
    sendCommand(0x00 | (col & 0x0F));
    sendCommand(0x10 | ((col >> 4) & 0x0F));
}

void OledDisplay::printString(const char* str) const {
    while (*str) {
        char c = *str++;
        if (c < 32 || c > 127) c = ' ';
        Wire.beginTransmission(i2cAddress);
        Wire.write(0x40);
        for (int i = 0; i < 5; i++) {
            Wire.write(font5x8[c - 32][i]);
        }
        Wire.write(0x00);
        Wire.endTransmission();
    }
}

void OledDisplay::clear() {
    for (uint8_t i = 0; i < 8; i++) {
        setCursor(0, i);
        Wire.beginTransmission(i2cAddress);
        Wire.write(0x40);
        for (uint8_t j = 0; j < 128; j++) {
            Wire.write(0x00);
            if (j % 16 == 0 && j > 0) {
                Wire.endTransmission();
                Wire.beginTransmission(i2cAddress);
                Wire.write(0x40);
            }
        }
        Wire.endTransmission();
    }
}

void OledDisplay::showAvailability(int available) {
    clear();
    setCursor(37, 2);
    printString("Available");

    char buf[16];
    sprintf(buf, "%d", available);
    int len = strlen(buf);
    int col = 32 + (64 - (len * 6)) / 2;

    setCursor(col, 4);
    printString(buf);
}