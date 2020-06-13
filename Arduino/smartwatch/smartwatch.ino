#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Talkie.h"

// Including all the libraries 

Talkie voice_output;  // Creating an instance of the class Talkie

const uint8_t spTHE[]       PROGMEM = {0x08,0xE8,0x3E,0x55,0x01,0xC3,0x86,0x27,0xAF,0x72,0x0D,0x4D,0x97,0xD5,0xBC,0x64,0x3C,0xF2,0x5C,0x51,0xF1,0x93,0x36,0x8F,0x4F,0x59,0x2A,0x42,0x7A,0x32,0xC3,0x64,0xFF,0x3F};
const uint8_t spTIME[]      PROGMEM = {0x0E,0x28,0xAC,0x2D,0x01,0x5D,0xB6,0x0D,0x33,0xF3,0x54,0xB3,0x60,0xBA,0x8C,0x54,0x5C,0xCD,0x2D,0xD4,0x32,0x73,0x0F,0x8E,0x34,0x33,0xCB,0x4A,0x25,0xD4,0x25,0x83,0x2C,0x2B,0xD5,0x50,0x97,0x08,0x32,0xEC,0xD4,0xDC,0x4C,0x33,0xC8,0x70,0x73,0x0F,0x33,0xCD,0x20,0xC3,0xCB,0x43,0xDD,0x3C,0xCD,0x8C,0x20,0x77,0x89,0xF4,0x94,0xB2,0xE2,0xE2,0x35,0x22,0x5D,0xD6,0x4A,0x8A,0x96,0xCC,0x36,0x25,0x2D,0xC9,0x9A,0x7B,0xC2,0x18,0x87,0x24,0x4B,0x1C,0xC9,0x50,0x19,0x92,0x2C,0x71,0x34,0x4B,0x45,0x8A,0x8B,0xC4,0x96,0xB6,0x5A,0x29,0x2A,0x92,0x5A,0xCA,0x53,0x96,0x20,0x05,0x09,0xF5,0x92,0x5D,0xBC,0xE8,0x58,0x4A,0xDD,0xAE,0x73,0xBD,0x65,0x4B,0x8D,0x78,0xCA,0x2B,0x4E,0xD8,0xD9,0xED,0x22,0x20,0x06,0x75,0x00,0x00,0x80,0xFF,0x07};
const uint8_t spIS[]        PROGMEM = {0x21,0x18,0x96,0x38,0xB7,0x14,0x8D,0x60,0x3A,0xA6,0xE8,0x51,0xB4,0xDC,0x2E,0x48,0x7B,0x5A,0xF1,0x70,0x1B,0xA3,0xEC,0x09,0xC6,0xCB,0xEB,0x92,0x3D,0xA7,0x69,0x1F,0xAF,0x71,0x89,0x9C,0xA2,0xB3,0xFC,0xCA,0x35,0x72,0x9A,0xD1,0xF0,0xAB,0x12,0xB3,0x2B,0xC6,0xCD,0x4F,0xCC,0x32,0x26,0x19,0x07,0xDF,0x0B,0x8F,0xB8,0xA4,0xED,0x7C,0xCF,0x23,0x62,0x8B,0x8E,0xF1,0x23,0x0A,0x8B,0x6E,0xCB,0xCE,0xEF,0x54,0x44,0x3C,0xDC,0x08,0x60,0x0B,0x37,0x01,0x1C,0x53,0x26,0x80,0x15,0x4E,0x14,0xB0,0x54,0x2B,0x02,0xA4,0x69,0xFF,0x7F};
const uint8_t spA_M_[]      PROGMEM = {0xCD,0xEF,0x86,0xAB,0x57,0x6D,0x0F,0xAF,0x71,0xAD,0x49,0x55,0x3C,0xFC,0x2E,0xC5,0xB7,0x5C,0xF1,0xF2,0x87,0x66,0xDD,0x4E,0xC5,0xC3,0xEF,0x92,0xE2,0x3A,0x65,0xB7,0xA0,0x09,0xAA,0x1B,0x97,0x54,0x82,0x2E,0x28,0x77,0x5C,0x52,0x09,0x1A,0xA3,0xB8,0x76,0x49,0x25,0x68,0x8C,0x73,0xDB,0x24,0x95,0xA0,0x32,0xA9,0x6B,0xA7,0xD9,0x82,0x26,0xA9,0x76,0x42,0xD6,0x08,0xBA,0xE1,0xE8,0x0E,0x5A,0x2B,0xEA,0x9E,0x3D,0x27,0x18,0xAD,0xA8,0x07,0xF1,0x98,0x90,0x35,0xA2,0x96,0x44,0xA3,0x5D,0x66,0x8B,0x6B,0x12,0xCD,0x32,0x85,0x25,0xC9,0x81,0x2D,0xC3,0x64,0x85,0x34,0x58,0x89,0x94,0x52,0x1C,0x52,0x2F,0x35,0xDA,0xC7,0x51,0x48,0x23,0x97,0xCC,0x2C,0x97,0x2E,0xF3,0x5C,0xF3,0xA2,0x14,0xBA,0x2C,0x48,0xCE,0xCA,0x76,0xE8,0x32,0x2F,0x34,0xB2,0xDB,0x85,0xC9,0x83,0x90,0xA8,0x2C,0x57,0x26,0x8F,0x9C,0xBD,0xA2,0x53,0xD9,0xC2,0x54,0x59,0x28,0x99,0x4B,0x2C,0x5D,0xFF,0x3F};
const uint8_t spP_M_[]      PROGMEM = {0x0E,0x98,0x41,0x54,0x00,0x43,0xA0,0x05,0xAB,0x42,0x8E,0x1D,0xA3,0x15,0xEC,0x4E,0x58,0xF7,0x92,0x66,0x70,0x1B,0x66,0xDB,0x73,0x99,0xC1,0xEB,0x98,0xED,0xD6,0x25,0x25,0x6F,0x70,0x92,0xDD,0x64,0xD8,0xFC,0x61,0xD0,0x66,0x83,0xD6,0x0A,0x86,0x23,0xAB,0x69,0xDA,0x2B,0x18,0x9E,0x3D,0x37,0x69,0x9D,0xA8,0x07,0x71,0x9F,0xA0,0xBD,0xA2,0x16,0xD5,0x7C,0x54,0xF6,0x88,0x6B,0x54,0x8B,0x34,0x49,0x2D,0x29,0x49,0x3C,0x34,0x64,0xA5,0x24,0x1B,0x36,0xD7,0x72,0x13,0x92,0xA4,0xC4,0x2D,0xC3,0xB3,0x4B,0xA3,0x62,0x0F,0x2B,0x37,0x6E,0x8B,0x5A,0xD4,0x3D,0xDD,0x9A,0x2D,0x50,0x93,0xF6,0x4C,0xAA,0xB6,0xC4,0x85,0x3B,0xB2,0xB1,0xD8,0x93,0x20,0x4D,0x8F,0x24,0xFF,0x0F};
//const uint8_t spOH[]        PROGMEM = {0xC6,0xC9,0x71,0x5A,0xA2,0x92,0x14,0x2F,0x6E,0x97,0x9C,0x46,0x9D,0xDC,0xB0,0x4D,0x62,0x1B,0x55,0x70,0xDD,0x55,0xBE,0x0E,0x36,0xC1,0x33,0x37,0xA9,0xA7,0x51,0x1B,0xCF,0x3C,0xA5,0x9E,0x44,0xAC,0x3C,0x7D,0x98,0x7B,0x52,0x96,0x72,0x65,0x4B,0xF6,0x1A,0xD9,0xCA,0xF5,0x91,0x2D,0xA2,0x2A,0x4B,0xF7,0xFF,0x01};
//const uint8_t spOCLOCK[]    PROGMEM = {0x21,0x4E,0x3D,0xB8,0x2B,0x19,0xBB,0x24,0x0E,0xE5,0xEC,0x60,0xE4,0xF2,0x90,0x13,0xD4,0x2A,0x11,0x80,0x00,0x42,0x69,0x26,0x40,0xD0,0x2B,0x04,0x68,0xE0,0x4D,0x00,0x3A,0x35,0x35,0x33,0xB6,0x51,0xD9,0x64,0x34,0x82,0xB4,0x9A,0x63,0x92,0x55,0x89,0x52,0x5B,0xCA,0x2E,0x34,0x25,0x4E,0x63,0x28,0x3A,0x50,0x95,0x26,0x8D,0xE6,0xAA,0x64,0x58,0xEA,0x92,0xCE,0xC2,0x46,0x15,0x9B,0x86,0xCD,0x2A,0x2E,0x37,0x00,0x00,0x00,0x0C,0xC8,0xDD,0x05,0x01,0xB9,0x33,0x21,0xA0,0x74,0xD7,0xFF,0x07};
const uint8_t spONE[]       PROGMEM = {0xCC,0x67,0x75,0x42,0x59,0x5D,0x3A,0x4F,0x9D,0x36,0x63,0xB7,0x59,0xDC,0x30,0x5B,0x5C,0x23,0x61,0xF3,0xE2,0x1C,0xF1,0xF0,0x98,0xC3,0x4B,0x7D,0x39,0xCA,0x1D,0x2C,0x2F,0xB7,0x15,0xEF,0x70,0x79,0xBC,0xD2,0x46,0x7C,0x52,0xE5,0xF1,0x4A,0x6A,0xB3,0x71,0x47,0xC3,0x2D,0x39,0x34,0x4B,0x23,0x35,0xB7,0x7A,0x55,0x33,0x8F,0x59,0xDC,0xA2,0x44,0xB5,0xBC,0x66,0x72,0x8B,0x64,0xF5,0xF6,0x98,0xC1,0x4D,0x42,0xD4,0x27,0x62,0x38,0x2F,0x4A,0xB6,0x9C,0x88,0x68,0xBC,0xA6,0x95,0xF8,0x5C,0xA1,0x09,0x86,0x77,0x91,0x11,0x5B,0xFF,0x0F};
const uint8_t spTWO[]       PROGMEM = {0x0E,0x38,0x6E,0x25,0x00,0xA3,0x0D,0x3A,0xA0,0x37,0xC5,0xA0,0x05,0x9E,0x56,0x35,0x86,0xAA,0x5E,0x8C,0xA4,0x82,0xB2,0xD7,0x74,0x31,0x22,0x69,0xAD,0x1C,0xD3,0xC1,0xD0,0xFA,0x28,0x2B,0x2D,0x47,0xC3,0x1B,0xC2,0xC4,0xAE,0xC6,0xCD,0x9C,0x48,0x53,0x9A,0xFF,0x0F};
const uint8_t spTHREE[]     PROGMEM = {0x02,0xD8,0x2E,0x9C,0x01,0xDB,0xA6,0x33,0x60,0xFB,0x30,0x01,0xEC,0x20,0x12,0x8C,0xE4,0xD8,0xCA,0x32,0x96,0x73,0x63,0x41,0x39,0x89,0x98,0xC1,0x4D,0x0D,0xED,0xB0,0x2A,0x05,0x37,0x0F,0xB4,0xA5,0xAE,0x5C,0xDC,0x36,0xD0,0x83,0x2F,0x4A,0x71,0x7B,0x03,0xF7,0x38,0x59,0xCD,0xED,0x1E,0xB4,0x6B,0x14,0x35,0xB7,0x6B,0x94,0x99,0x91,0xD5,0xDC,0x26,0x48,0x77,0x4B,0x66,0x71,0x1B,0x21,0xDB,0x2D,0x8A,0xC9,0x6D,0x88,0xFC,0x26,0x28,0x3A,0xB7,0x21,0xF4,0x1F,0xA3,0x65,0xBC,0x02,0x38,0xBB,0x3D,0x8E,0xF0,0x2B,0xE2,0x08,0xB7,0x34,0xFF,0x0F};
const uint8_t spFOUR[]      PROGMEM = {0x0C,0x18,0xB6,0x9A,0x01,0xC3,0x75,0x09,0x60,0xD8,0x0E,0x09,0x30,0xA0,0x9B,0xB6,0xA0,0xBB,0xB0,0xAA,0x16,0x4E,0x82,0xEB,0xEA,0xA9,0xFA,0x59,0x49,0x9E,0x59,0x23,0x9A,0x27,0x3B,0x78,0x66,0xAE,0x4A,0x9C,0x9C,0xE0,0x99,0xD3,0x2A,0xBD,0x72,0x92,0xEF,0xE6,0x88,0xE4,0x45,0x4D,0x7E,0x98,0x2D,0x62,0x67,0x37,0xF9,0xA1,0x37,0xA7,0x6C,0x94,0xE4,0xC7,0x1E,0xDC,0x3C,0xA5,0x83,0x1F,0x8B,0xEB,0x52,0x0E,0x0E,0x7E,0x2E,0x4E,0xC7,0x31,0xD2,0x79,0xA5,0x3A,0x0D,0xD9,0xC4,0xFF,0x07};
const uint8_t spFIVE[]      PROGMEM = {0x02,0xE8,0x3E,0x8C,0x01,0xDD,0x65,0x08,0x60,0x98,0x4C,0x06,0x34,0x93,0xCE,0x80,0xE6,0xDA,0x9A,0x14,0x6B,0xAA,0x47,0xD1,0x5E,0x56,0xAA,0x6D,0x56,0xCD,0x78,0xD9,0xA9,0x1C,0x67,0x05,0x83,0xE1,0xA4,0xBA,0x38,0xEE,0x16,0x86,0x9B,0xFA,0x60,0x87,0x5B,0x18,0x6E,0xEE,0x8B,0x1D,0x6E,0x61,0xB9,0x69,0x36,0x65,0xBA,0x8D,0xE5,0xE5,0x3E,0x1C,0xE9,0x0E,0x96,0x9B,0x5B,0xAB,0x95,0x2B,0x58,0x6E,0xCE,0xE5,0x3A,0x6A,0xF3,0xB8,0x35,0x84,0x7B,0x05,0xA3,0xE3,0x36,0xEF,0x92,0x19,0xB4,0x86,0xDB,0xB4,0x69,0xB4,0xD1,0x2A,0x4E,0x65,0x9A,0x99,0xCE,0x28,0xD9,0x85,0x71,0x4C,0x18,0x6D,0x67,0x47,0xC6,0x5E,0x53,0x4A,0x9C,0xB5,0xE2,0x85,0x45,0x26,0xFE,0x7F};
const uint8_t spSIX[]       PROGMEM = {0x0E,0xD8,0xAE,0xDD,0x03,0x0E,0x38,0xA6,0xD2,0x01,0xD3,0xB4,0x2C,0xAD,0x6A,0x35,0x9D,0xB1,0x7D,0xDC,0xEE,0xC4,0x65,0xD7,0xF1,0x72,0x47,0x24,0xB3,0x19,0xD9,0xD9,0x05,0x70,0x40,0x49,0xEA,0x02,0x98,0xBE,0x42,0x01,0xDF,0xA4,0x69,0x40,0x00,0xDF,0x95,0xFC,0x3F};
const uint8_t spSEVEN[]     PROGMEM = {0x02,0xB8,0x3A,0x8C,0x01,0xDF,0xA4,0x73,0x40,0x01,0x47,0xB9,0x2F,0x33,0x3B,0x73,0x5F,0x53,0x7C,0xEC,0x9A,0xC5,0x63,0xD5,0xD1,0x75,0xAE,0x5B,0xFC,0x64,0x5C,0x35,0x87,0x91,0xF1,0x83,0x36,0xB5,0x68,0x55,0xC5,0x6F,0xDA,0x45,0x2D,0x1C,0x2D,0xB7,0x38,0x37,0x9F,0x60,0x3C,0xBC,0x9A,0x85,0xA3,0x25,0x66,0xF7,0x8A,0x57,0x1C,0xA9,0x67,0x56,0xCA,0x5E,0xF0,0xB2,0x16,0xB2,0xF1,0x89,0xCE,0x8B,0x92,0x25,0xC7,0x2B,0x33,0xCF,0x48,0xB1,0x99,0xB4,0xF3,0xFF};
const uint8_t spEIGHT[]     PROGMEM = {0xC3,0x6C,0x86,0xB3,0x27,0x6D,0x0F,0xA7,0x48,0x99,0x4E,0x55,0x3C,0xBC,0x22,0x65,0x36,0x4D,0xD1,0xF0,0x32,0xD3,0xBE,0x34,0xDA,0xC3,0xEB,0x82,0xE2,0xDA,0x65,0x35,0xAF,0x31,0xF2,0x6B,0x97,0x95,0xBC,0x86,0xD8,0x6F,0x82,0xA6,0x73,0x0B,0xC6,0x9E,0x72,0x99,0xCC,0xCB,0x02,0xAD,0x3C,0x9A,0x10,0x60,0xAB,0x62,0x05,0x2C,0x37,0x84,0x00,0xA9,0x73,0x00,0x00,0xFE,0x1F};
const uint8_t spNINE[]      PROGMEM = {0xCC,0xA1,0x26,0xBB,0x83,0x93,0x18,0xCF,0x4A,0xAD,0x2E,0x31,0xED,0x3C,0xA7,0x24,0x26,0xC3,0x54,0xF1,0x92,0x64,0x8B,0x8A,0x98,0xCB,0x2B,0x2E,0x34,0x53,0x2D,0x0E,0x2F,0x57,0xB3,0x0C,0x0D,0x3C,0xBC,0x3C,0x4C,0x4B,0xCA,0xF4,0xF0,0x72,0x0F,0x6E,0x49,0x53,0xCD,0xCB,0x53,0x2D,0x35,0x4D,0x0F,0x2F,0x0F,0xD7,0x0C,0x0D,0x3D,0xBC,0xDC,0x4D,0xD3,0xDD,0xC2,0xF0,0x72,0x52,0x4F,0x57,0x9B,0xC3,0xAB,0x89,0xBD,0x42,0x2D,0x0F,0xAF,0x5A,0xD1,0x71,0x91,0x55,0xBC,0x2C,0xC5,0x3B,0xD8,0x65,0xF2,0x82,0x94,0x18,0x4E,0x3B,0xC1,0x73,0x42,0x32,0x33,0x15,0x45,0x4F,0x79,0x52,0x6A,0x55,0xA6,0xA3,0xFF,0x07};
const uint8_t spTEN[]       PROGMEM = {0x0E,0xD8,0xB1,0xDD,0x01,0x3D,0xA8,0x24,0x7B,0x04,0x27,0x76,0x77,0xDC,0xEC,0xC2,0xC5,0x23,0x84,0xCD,0x72,0x9A,0x51,0xF7,0x62,0x45,0xC7,0xEB,0x4E,0x35,0x4A,0x14,0x2D,0xBF,0x45,0xB6,0x0A,0x75,0xB8,0xFC,0x16,0xD9,0x2A,0xD9,0xD6,0x0A,0x5A,0x10,0xCD,0xA2,0x48,0x23,0xA8,0x81,0x35,0x4B,0x2C,0xA7,0x20,0x69,0x0A,0xAF,0xB6,0x15,0x82,0xA4,0x29,0x3C,0xC7,0x52,0x08,0xA2,0x22,0xCF,0x68,0x4B,0x2E,0xF0,0x8A,0xBD,0xA3,0x2C,0xAB,0x40,0x1B,0xCE,0xAA,0xB2,0x6C,0x82,0x40,0x4D,0x7D,0xC2,0x89,0x88,0x8A,0x61,0xCC,0x74,0xD5,0xFF,0x0F};
const uint8_t spELEVEN[]    PROGMEM = {0xC3,0xCD,0x76,0x5C,0xAE,0x14,0x0F,0x37,0x9B,0x71,0xDE,0x92,0x55,0xBC,0x2C,0x27,0x70,0xD3,0x76,0xF0,0x83,0x5E,0xA3,0x5E,0x5A,0xC1,0xF7,0x61,0x58,0xA7,0x19,0x35,0x3F,0x99,0x31,0xDE,0x52,0x74,0xFC,0xA2,0x26,0x64,0x4B,0xD1,0xF1,0xAB,0xAE,0xD0,0x2D,0xC5,0xC7,0x2F,0x36,0xDD,0x27,0x15,0x0F,0x3F,0xD9,0x08,0x9F,0x62,0xE4,0xC2,0x2C,0xD4,0xD8,0xD3,0x89,0x0B,0x1B,0x57,0x11,0x0B,0x3B,0xC5,0xCF,0xD6,0xCC,0xC6,0x64,0x35,0xAF,0x18,0x73,0x1F,0xA1,0x5D,0xBC,0x62,0x45,0xB3,0x45,0x51,0xF0,0xA2,0x62,0xAB,0x4A,0x5B,0xC9,0x4B,0x8A,0x2D,0xB3,0x6C,0x06,0x2F,0x29,0xB2,0xAC,0x8A,0x18,0xBC,0x28,0xD9,0xAA,0xD2,0x92,0xF1,0xBC,0xE0,0x98,0x8C,0x48,0xCC,0x17,0x52,0xA3,0x27,0x6D,0x93,0xD0,0x4B,0x8E,0x0E,0x77,0x02,0x00,0xFF,0x0F};
const uint8_t spTWELVE[]    PROGMEM = {0x06,0x28,0x46,0xD3,0x01,0x25,0x06,0x13,0x20,0xBA,0x70,0x70,0xB6,0x79,0xCA,0x36,0xAE,0x28,0x38,0xE1,0x29,0xC5,0x35,0xA3,0xE6,0xC4,0x16,0x6A,0x53,0x8C,0x97,0x9B,0x72,0x86,0x4F,0x28,0x1A,0x6E,0x0A,0x59,0x36,0xAE,0x68,0xF8,0x29,0x67,0xFA,0x06,0xA3,0x16,0xC4,0x96,0xE6,0x53,0xAC,0x5A,0x9C,0x56,0x72,0x77,0x31,0x4E,0x49,0x5C,0x8D,0x5B,0x29,0x3B,0x24,0x61,0x1E,0x6C,0x9B,0x6C,0x97,0xF8,0xA7,0x34,0x19,0x92,0x4C,0x62,0x9E,0x72,0x65,0x58,0x12,0xB1,0x7E,0x09,0xD5,0x2E,0x53,0xC5,0xBA,0x36,0x6B,0xB9,0x2D,0x17,0x05,0xEE,0x9A,0x6E,0x8E,0x05,0x50,0x6C,0x19,0x07,0x18,0x50,0xBD,0x3B,0x01,0x92,0x08,0x41,0x40,0x10,0xA6,0xFF,0x0F};
const uint8_t spTHIRTEEN[]  PROGMEM = {0x08,0xE8,0x2C,0x15,0x01,0x43,0x07,0x13,0xE0,0x98,0xB4,0xA6,0x35,0xA9,0x1E,0xDE,0x56,0x8E,0x53,0x9C,0x7A,0xE7,0xCA,0x5E,0x76,0x8D,0x94,0xE5,0x2B,0xAB,0xD9,0xB5,0x62,0xA4,0x9C,0xE4,0xE6,0xB4,0x41,0x1E,0x7C,0xB6,0x93,0xD7,0x16,0x99,0x5A,0xCD,0x61,0x76,0x55,0xC2,0x91,0x61,0x1B,0xC0,0x01,0x5D,0x85,0x05,0xE0,0x68,0x51,0x07,0x1C,0xA9,0x64,0x80,0x1D,0x4C,0x9C,0x95,0x88,0xD4,0x04,0x3B,0x4D,0x4E,0x21,0x5C,0x93,0xA8,0x26,0xB9,0x05,0x4B,0x6E,0xA0,0xE2,0xE4,0x57,0xC2,0xB9,0xC1,0xB2,0x93,0x5F,0x09,0xD7,0x24,0xCB,0x4E,0x41,0x25,0x54,0x1D,0x62,0x3B,0x05,0x8D,0x52,0x57,0xAA,0xAD,0x10,0x24,0x26,0xE3,0xE1,0x36,0x5D,0x10,0x85,0xB4,0x97,0x85,0x72,0x41,0x14,0x52,0x5E,0x1A,0xCA,0xF9,0x91,0x6B,0x7A,0x5B,0xC4,0xE0,0x17,0x2D,0x54,0x1D,0x92,0x8C,0x1F,0x25,0x4B,0x8F,0xB2,0x16,0x41,0xA1,0x4A,0x3E,0xE6,0xFA,0xFF,0x01};
const uint8_t spFOURTEEN[]  PROGMEM = {0x0C,0x58,0xAE,0x5C,0x01,0xD9,0x87,0x07,0x51,0xB7,0x25,0xB3,0x8A,0x15,0x2C,0xF7,0x1C,0x35,0x87,0x4D,0xB2,0xDD,0x53,0xCE,0x28,0x2B,0xC9,0x0E,0x97,0x2D,0xBD,0x2A,0x17,0x27,0x76,0x8E,0xD2,0x9A,0x6C,0x80,0x94,0x71,0x00,0x00,0x02,0xB0,0x58,0x58,0x00,0x9E,0x0B,0x0A,0xC0,0xB2,0xCE,0xC1,0xC8,0x98,0x7A,0x52,0x95,0x24,0x2B,0x11,0xED,0x36,0xD4,0x92,0xDC,0x4C,0xB5,0xC7,0xC8,0x53,0xF1,0x2A,0xE5,0x1A,0x17,0x55,0xC5,0xAF,0x94,0xBB,0xCD,0x1C,0x26,0xBF,0x52,0x9A,0x72,0x53,0x98,0xFC,0xC2,0x68,0xD2,0x4D,0x61,0xF0,0xA3,0x90,0xB6,0xD6,0x50,0xC1,0x8F,0x42,0xDA,0x4A,0x43,0x39,0x3F,0x48,0x2D,0x6B,0x33,0xF9,0xFF};
const uint8_t spFIFTEEN[]   PROGMEM = {0x08,0xE8,0x2A,0x0D,0x01,0xDD,0xBA,0x31,0x60,0x6A,0xF7,0xA0,0xAE,0x54,0xAA,0x5A,0x76,0x97,0xD9,0x34,0x69,0xEF,0x32,0x1E,0x66,0xE1,0xE2,0xB3,0x43,0xA9,0x18,0x55,0x92,0x4E,0x37,0x2D,0x67,0x6F,0xDF,0xA2,0x5A,0xB6,0x04,0x30,0x55,0xA8,0x00,0x86,0x09,0xE7,0x00,0x01,0x16,0x17,0x05,0x70,0x40,0x57,0xE5,0x01,0xF8,0x21,0x34,0x00,0xD3,0x19,0x33,0x80,0x89,0x9A,0x62,0x34,0x4C,0xD5,0x49,0xAE,0x8B,0x53,0x09,0xF7,0x26,0xD9,0x6A,0x7E,0x23,0x5C,0x13,0x12,0xB3,0x04,0x9D,0x50,0x4F,0xB1,0xAD,0x14,0x15,0xC2,0xD3,0xA1,0xB6,0x42,0x94,0xA8,0x8C,0x87,0xDB,0x74,0xB1,0x70,0x59,0xE1,0x2E,0xC9,0xC5,0x81,0x5B,0x55,0xA4,0x4C,0x17,0x47,0xC1,0x6D,0xE3,0x81,0x53,0x9C,0x84,0x6A,0x46,0xD9,0x4C,0x51,0x31,0x42,0xD9,0x66,0xC9,0x44,0x85,0x29,0x6A,0x9B,0xAD,0xFF,0x07};
const uint8_t spSIXTEEN[]   PROGMEM = {0x0A,0x58,0x5A,0x5D,0x00,0x93,0x97,0x0B,0x60,0xA9,0x48,0x05,0x0C,0x15,0xAE,0x80,0xAD,0x3D,0x14,0x30,0x7D,0xD9,0x50,0x92,0x92,0xAC,0x0D,0xC5,0xCD,0x2A,0x82,0xAA,0x3B,0x98,0x04,0xB3,0x4A,0xC8,0x9A,0x90,0x05,0x09,0x68,0x51,0xD4,0x01,0x23,0x9F,0x1A,0x60,0xA9,0x12,0x03,0xDC,0x50,0x81,0x80,0x22,0xDC,0x20,0x00,0xCB,0x06,0x3A,0x60,0x16,0xE3,0x64,0x64,0x42,0xDD,0xCD,0x6A,0x8A,0x5D,0x28,0x75,0x07,0xA9,0x2A,0x5E,0x65,0x34,0xED,0x64,0xBB,0xF8,0x85,0xF2,0x94,0x8B,0xAD,0xE4,0x37,0x4A,0x5B,0x21,0xB6,0x52,0x50,0x19,0xAD,0xA7,0xD8,0x4A,0x41,0x14,0xDA,0x5E,0x12,0x3A,0x04,0x91,0x4B,0x7B,0x69,0xA8,0x10,0x24,0x2E,0xE5,0xA3,0x81,0x52,0x90,0x94,0x5A,0x55,0x98,0x32,0x41,0x50,0xCC,0x93,0x2E,0x47,0x85,0x89,0x1B,0x5B,0x5A,0x62,0x04,0x44,0xE3,0x02,0x80,0x80,0x64,0xDD,0xFF,0x1F};
const uint8_t spSEVENTEEN[] PROGMEM = {0x02,0x98,0x3A,0x42,0x00,0x5B,0xA6,0x09,0x60,0xDB,0x52,0x06,0x1C,0x93,0x29,0x80,0xA9,0x52,0x87,0x9A,0xB5,0x99,0x4F,0xC8,0x3E,0x46,0xD6,0x5E,0x7E,0x66,0xFB,0x98,0xC5,0x5A,0xC6,0x9A,0x9C,0x63,0x15,0x6B,0x11,0x13,0x8A,0x9C,0x97,0xB9,0x9A,0x5A,0x39,0x71,0xEE,0xD2,0x29,0xC2,0xA6,0xB8,0x58,0x59,0x99,0x56,0x14,0xA3,0xE1,0x26,0x19,0x19,0xE3,0x8C,0x93,0x17,0xB4,0x46,0xB5,0x88,0x71,0x9E,0x97,0x9E,0xB1,0x2C,0xC5,0xF8,0x56,0xC4,0x58,0xA3,0x1C,0xE1,0x33,0x9D,0x13,0x41,0x8A,0x43,0x58,0xAD,0x95,0xA9,0xDB,0x36,0xC0,0xD1,0xC9,0x0E,0x58,0x4E,0x45,0x01,0x23,0xA9,0x04,0x37,0x13,0xAE,0x4D,0x65,0x52,0x82,0xCA,0xA9,0x37,0x99,0x4D,0x89,0xBA,0xC0,0xBC,0x14,0x36,0x25,0xEA,0x1C,0x73,0x52,0x1D,0x97,0xB8,0x33,0xAC,0x0E,0x75,0x9C,0xE2,0xCE,0xB0,0xDA,0xC3,0x51,0x4A,0x1A,0xA5,0xCA,0x70,0x5B,0x21,0xCE,0x4C,0x26,0xD2,0x6C,0xBA,0x38,0x71,0x2E,0x1F,0x2D,0xED,0xE2,0x24,0xB8,0xBC,0x3D,0x52,0x88,0xAB,0x50,0x8E,0xA8,0x48,0x22,0x4E,0x42,0xA0,0x26,0x55,0xFD,0x3F};
const uint8_t spEIGHTEEN[]  PROGMEM = {0x2E,0x9C,0xD1,0x4D,0x54,0xEC,0x2C,0xBF,0x1B,0x8A,0x99,0x70,0x7C,0xFC,0x2E,0x29,0x6F,0x52,0xF6,0xF1,0xBA,0x20,0xBF,0x36,0xD9,0xCD,0xED,0x0C,0xF3,0x27,0x64,0x17,0x73,0x2B,0xA2,0x99,0x90,0x65,0xEC,0xED,0x40,0x73,0x32,0x12,0xB1,0xAF,0x30,0x35,0x0B,0xC7,0x00,0xE0,0x80,0xAE,0xDD,0x1C,0x70,0x43,0xAA,0x03,0x86,0x51,0x36,0xC0,0x30,0x64,0xCE,0x4C,0x98,0xFB,0x5C,0x65,0x07,0xAF,0x10,0xEA,0x0B,0x66,0x1B,0xFC,0x46,0xA8,0x3E,0x09,0x4D,0x08,0x2A,0xA6,0x3E,0x67,0x36,0x21,0x2A,0x98,0x67,0x9D,0x15,0xA7,0xA8,0x60,0xEE,0xB6,0x94,0x99,0xA2,0x4A,0x78,0x22,0xC2,0xA6,0x8B,0x8C,0x8E,0xCC,0x4C,0x8A,0x2E,0x8A,0x4C,0xD3,0x57,0x03,0x87,0x28,0x71,0x09,0x1F,0x2B,0xE4,0xA2,0xC4,0xC5,0x6D,0xAD,0x54,0x88,0xB2,0x63,0xC9,0xF2,0x50,0x2E,0x8A,0x4A,0x38,0x4A,0xEC,0x88,0x28,0x08,0xE3,0x28,0x49,0xF3,0xFF};
const uint8_t spNINETEEN[]  PROGMEM = {0xC2,0xEA,0x8A,0x95,0x2B,0x6A,0x05,0x3F,0x71,0x71,0x5F,0x0D,0x12,0xFC,0x28,0x25,0x62,0x35,0xF0,0xF0,0xB3,0x48,0x1E,0x0F,0xC9,0xCB,0x2F,0x45,0x7C,0x2C,0x25,0x1F,0xBF,0x14,0xB3,0x2C,0xB5,0x75,0xFC,0x5A,0x5C,0xA3,0x5D,0xE1,0xF1,0x7A,0x76,0xB3,0x4E,0x45,0xC7,0xED,0x96,0x23,0x3B,0x18,0x37,0x7B,0x18,0xCC,0x09,0x51,0x13,0x4C,0xAB,0x6C,0x4C,0x4B,0x96,0xD2,0x49,0xAA,0x36,0x0B,0xC5,0xC2,0x20,0x26,0x27,0x35,0x63,0x09,0x3D,0x30,0x8B,0xF0,0x48,0x5C,0xCA,0x61,0xDD,0xCB,0xCD,0x91,0x03,0x8E,0x4B,0x76,0xC0,0xCC,0x4D,0x06,0x98,0x31,0x31,0x98,0x99,0x70,0x6D,0x2A,0xA3,0xE4,0x16,0xCA,0xBD,0xCE,0x5C,0x92,0x57,0x28,0xCF,0x09,0x69,0x2E,0x7E,0xA5,0x3C,0x63,0xA2,0x30,0x05,0x95,0xD2,0x74,0x98,0xCD,0x14,0x54,0xCA,0x53,0xA9,0x96,0x52,0x50,0x28,0x6F,0xBA,0xCB,0x0C,0x41,0x50,0xDE,0x65,0x2E,0xD3,0x05,0x89,0x4B,0x7B,0x6B,0x20,0x17,0x44,0xAE,0xED,0x23,0x81,0x52,0x90,0x85,0x73,0x57,0xD0,0x72,0x41,0xB1,0x02,0xDE,0x2E,0xDB,0x04,0x89,0x05,0x79,0xBB,0x62,0xE5,0x76,0x11,0xCA,0x61,0x0E,0xFF,0x1F};
const uint8_t spTWENTY[]    PROGMEM = {0x01,0x98,0xD1,0xC2,0x00,0xCD,0xA4,0x32,0x20,0x79,0x13,0x04,0x28,0xE7,0x92,0xDC,0x70,0xCC,0x5D,0xDB,0x76,0xF3,0xD2,0x32,0x0B,0x0B,0x5B,0xC3,0x2B,0xCD,0xD4,0xDD,0x23,0x35,0xAF,0x44,0xE1,0xF0,0xB0,0x6D,0x3C,0xA9,0xAD,0x3D,0x35,0x0E,0xF1,0x0C,0x8B,0x28,0xF7,0x34,0x01,0x68,0x22,0xCD,0x00,0xC7,0xA4,0x04,0xBB,0x32,0xD6,0xAC,0x56,0x9C,0xDC,0xCA,0x28,0x66,0x53,0x51,0x70,0x2B,0xA5,0xBC,0x0D,0x9A,0xC1,0xEB,0x14,0x73,0x37,0x29,0x19,0xAF,0x33,0x8C,0x3B,0xA7,0x24,0xBC,0x42,0xB0,0xB7,0x59,0x09,0x09,0x3C,0x96,0xE9,0xF4,0x58,0xFF,0x0F};
const uint8_t spTHIRTY[]    PROGMEM = {0x08,0x98,0xD6,0x15,0x01,0x43,0xBB,0x0A,0x20,0x1B,0x8B,0xE5,0x16,0xA3,0x1E,0xB6,0xB6,0x96,0x97,0x3C,0x57,0xD4,0x2A,0x5E,0x7E,0x4E,0xD8,0xE1,0x6B,0x7B,0xF8,0x39,0x63,0x0D,0x9F,0x95,0xE1,0xE7,0x4C,0x76,0xBC,0x91,0x5B,0x90,0x13,0xC6,0x68,0x57,0x4E,0x41,0x8B,0x10,0x5E,0x1D,0xA9,0x44,0xD3,0xBA,0x47,0xB8,0xDD,0xE4,0x35,0x86,0x11,0x93,0x94,0x92,0x5F,0x29,0xC7,0x4C,0x30,0x0C,0x41,0xC5,0x1C,0x3B,0x2E,0xD3,0x05,0x15,0x53,0x6C,0x07,0x4D,0x15,0x14,0x8C,0xB5,0xC9,0x6A,0x44,0x90,0x10,0x4E,0x9A,0xB6,0x21,0x81,0x23,0x3A,0x91,0x91,0xE8,0xFF,0x01};
const uint8_t spFOURTY[]    PROGMEM = {0x04,0x18,0xB6,0x4C,0x00,0xC3,0x56,0x30,0xA0,0xE8,0xF4,0xA0,0x98,0x99,0x62,0x91,0xAE,0x83,0x6B,0x77,0x89,0x78,0x3B,0x09,0xAE,0xBD,0xA6,0x1E,0x63,0x3B,0x79,0x7E,0x71,0x5A,0x8F,0x95,0xE6,0xA5,0x4A,0x69,0xB9,0x4E,0x8A,0x5F,0x12,0x56,0xE4,0x58,0x69,0xE1,0x36,0xA1,0x69,0x2E,0x2B,0xF9,0x95,0x93,0x55,0x17,0xED,0xE4,0x37,0xC6,0xBA,0x93,0xB2,0x92,0xDF,0x19,0xD9,0x6E,0xC8,0x0A,0xFE,0x60,0xE8,0x37,0x21,0xC9,0xF9,0x8D,0x61,0x5F,0x32,0x13,0xE7,0x17,0x4C,0xD3,0xC6,0xB1,0x94,0x97,0x10,0x8F,0x8B,0xAD,0x11,0x7E,0xA1,0x9A,0x26,0x92,0xF6,0xFF,0x01};
const uint8_t spFIFTY[]     PROGMEM = {0x08,0xE8,0x2E,0x84,0x00,0x23,0x84,0x13,0x60,0x38,0x95,0xA5,0x0F,0xCF,0xE2,0x79,0x8A,0x8F,0x37,0x02,0xB3,0xD5,0x2A,0x6E,0x5E,0x93,0x94,0x79,0x45,0xD9,0x05,0x5D,0x0A,0xB9,0x97,0x63,0x02,0x74,0xA7,0x82,0x80,0xEE,0xC3,0x10,0xD0,0x7D,0x28,0x03,0x6E,0x14,0x06,0x70,0xE6,0x0A,0xC9,0x9A,0x4E,0x37,0xD9,0x95,0x51,0xCE,0xBA,0xA2,0x14,0x0C,0x81,0x36,0x1B,0xB2,0x5C,0x30,0x38,0xFA,0x9C,0xC9,0x32,0x41,0xA7,0x18,0x3B,0xA2,0x48,0x04,0x05,0x51,0x4F,0x91,0x6D,0x12,0x04,0x20,0x9B,0x61,0x89,0xFF,0x1F};
const uint8_t spPAUSE1[]    PROGMEM = {0x00,0x00,0x00,0x00,0xFF,0x0F};

// Loading all the coefficients corresponding to speech signals of numbers from 1 to 50

// keyValue class is created so that the parsed variables can be transferred from one place to another
class keyValue
{
  public:
  String key, value;

  keyValue(){
  }

  keyValue(String key1, String value1)
  {
    key = key1;
    value = value1;
  }

  String getKey() { return key;}

  String getValue() {return value;}

  void setKey(String key1) {key = key1;}

  void setValue(String value1) {value = value1;}
};


// alarm class allows template of alarm to be created 
class alarm
{
  public:
  int h, m, ampm;
  String msg;
  
  alarm()
  {
    h = 0;
    m = 0;
    ampm = -1;
    msg = "";
  }

  alarm(int h1, int m1, int ampm1, String msg1)
  {
    h = h1;
    m = m1;
    ampm = ampm1;
    msg = msg1;
  }

  int getH() {return h;}
  
  int getM() {return m;}

  int getAMPM() {return ampm;}

  String getMsg() {return msg;}

  void setH(int h1) {h = h1;}

  void setM(int m1) {m = m1;}

  void setAMPM(int ampm1) {ampm = ampm1;}

  void setMsg(String msg1) {msg = msg1;}
};

#define OLED_RESET 4
#define RTC_Address 0x68

Adafruit_SSD1306 display(OLED_RESET); // Creating an instance of the class Adafruit_SSD1306 for RTC

// RTC variables
char Time[]     = "  :  :    ";
char Calendar[] = "  /  /20  ";
char temperature[] = " 00.00";
char temperature_msb;
byte i, second, minute, hour, day, date, month, year, temperature_lsb;
int ampm;

// Alarm variables
alarm alarmList[4];
String myTime1="";
String myTime2="";
String myTime3="";
int numberOfAlarms=-1;

// Mode variables
String selectedMode="0";  // Default Mode 0
String hourLong="10";
String hourShort="1";
String minuteLong="10";
String minuteShort="2";

// Sound and Vibration variables
String volume="3";  // Default Volume 3
String vibration="3"; // Default Vibration 3

// Button variables
// Talkie output at Digital 3
// Display pins at SDA = A4, SCL = A5
int PWMPin = 9; // Output of PWM (for coin vibrator)
int speakTimeButton = 7;
int vibrateTimeButton = 6;
int screenBrowseButton = 5;
int triggerSOSButton = 4; 

// Initialize Button's read variables
int currentScreen = 0;
int readSpeakTimeButton = HIGH;
int readVibrateTimeButton = HIGH;
int readScreenBrowseButton = HIGH;
int readSOSButton = HIGH;

void setup() 
{
  Serial.begin(9600);
  pinMode(speakTimeButton, INPUT_PULLUP);
  pinMode(vibrateTimeButton, INPUT_PULLUP);
  pinMode(screenBrowseButton, INPUT_PULLUP);
  pinMode(triggerSOSButton, INPUT_PULLUP);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
  display.display();
  display.clearDisplay();
  showTimeScreen();

  for (int i=0; i<4; i++)
    alarmList[i] = alarm();
}

void loop() 
{
  // Listen to Changes on App
  if(Serial.available())
  {
    char c = Serial.read();
    if (c == '~')
    {
      fetchVariables();
    }
  }
  
  // TODO: Trigger SOS Button
  readSOSButton = digitalRead(triggerSOSButton);
  if (readSOSButton == LOW)
  {
    showSOSScreen();
    Serial.println("interrupt");
  }

  readSpeakTimeButton = digitalRead(speakTimeButton);
  if (readSpeakTimeButton == LOW)
  {
    voice_output.setVol(volume.toInt()*20);
    talkie_output(hour, minute/10, minute%10, ampm);
  }

  readVibrateTimeButton = digitalRead(vibrateTimeButton);
  if (readVibrateTimeButton == LOW)
  {
    // TODO: Also, add frequency mapping here
    Serial.println("Vibrate");
    if(selectedMode == "0")
    {
      hourLong="10";
      hourShort="1";
      minuteLong="10";
      minuteShort="1";
    }
    else if(selectedMode == "1")
    {
      hourLong="5";
      hourShort="1";
      minuteLong="15";
      minuteShort="1";
    }
    int hourLongBuzz = hour/(hourLong.toInt());
    int hourShortBuzz = (hour%(hourLong.toInt()))/(hourShort.toInt());
    int minuteLongBuzz = minute/(minuteLong.toInt());
    int minuteShortBuzz = (minute%(minuteLong.toInt()))/(minuteShort.toInt());

    int vibrationLong, vibrationShort, vibrationGap;
    if (vibration == "5")
    {
      vibrationLong = 200;
      vibrationShort = 100;
      vibrationGap = 300;
    }
    else if (vibration == "4")
    {
      vibrationLong = 250;
      vibrationShort = 150;
      vibrationGap = 350;
    }
    else if (vibration == "3")
    {
      vibrationLong = 300;
      vibrationShort = 200;
      vibrationGap = 400;
    }
    else if (vibration == "2")
    {
      vibrationLong = 350;
      vibrationShort = 200;
      vibrationGap = 500;

    }
    else if (vibration == "1")
    {
      vibrationLong = 400;
      vibrationShort = 200;
      vibrationGap = 500;
    }
    buzz(hourLongBuzz, vibrationLong);
    delay(vibrationGap);
    buzz(hourShortBuzz, vibrationShort);
    delay(vibrationGap);
    buzz(minuteLongBuzz, vibrationLong);
    delay(vibrationGap);
    buzz(minuteShortBuzz, vibrationShort);
    delay(vibrationGap);
  }

  // Alarm Event Check
  for (int i=0; i<4; i++)
  {
    if (alarmList[i].getAMPM() != -1)
    {
      if ((int)hour == alarmList[i].getH())
      {
        if ((int)minute == alarmList[i].getM())
        {
          if (ampm == alarmList[i].getAMPM())
          {
              showAlarmScreen(alarmList[i]);
          }
        }
      }
    }
  }
  
  // Browse Screen
  if(digitalRead(screenBrowseButton) == LOW)
  {
    Serial.println("Screen Browse");
    readScreenBrowseButton++;
    if(readScreenBrowseButton%4 == 0)
    {
      showTimeScreen();
      currentScreen = 0;
    }
    else if(readScreenBrowseButton%4 == 1)
    {
      showSVScreen(volume.toInt(),vibration.toInt());
      currentScreen = 1;
    }
    else if(readScreenBrowseButton%4 == 2)
    {
      showAlarmListScreen();
      currentScreen = 2;
    }
    else if (readScreenBrowseButton%4 == 3)
    {
      showModeScreen();
      currentScreen = 3;
    }
  }
  else
  {
    // Allow Screen to hold when button not clicked
    if(currentScreen == 0)
      showTimeScreen();
    else if(currentScreen == 1)
      showSVScreen(volume.toInt(),vibration.toInt());
    else if(currentScreen == 2)
      showAlarmListScreen();
    else if(currentScreen == 3)
      showModeScreen();
  }
}

void fetchVariables()
{
  while(true)
  {
    char c = Serial.read();
    if (c == '=')
    {
      //printRoutine();
      return;
    }
    if (c == '{')
    {
      keyValue fetch = extractKeyValue();
      String key = fetch.getKey();
      String value = fetch.getValue();
      Serial.print(key);
      Serial.print(":");
      Serial.println(value);
      if (key == "fmw" && value == "on")
        showFMWScreen();
      else if (key == "noa")
      {
        numberOfAlarms = value.toInt();
        for (int j = numberOfAlarms; j<4; j++)
          alarmList[j].setAMPM(-1);
      }
      else if (key == "h0")
        alarmList[0].setH(value.toInt());
      else if (key == "m0")
        alarmList[0].setM(value.toInt());
      else if (key == "ap0")
        alarmList[0].setAMPM(value.toInt());
      else if (key == "ms0")
        alarmList[0].setMsg(value);
      else if (key == "h1")
        alarmList[1].setH(value.toInt());
      else if (key == "m1")
        alarmList[1].setM(value.toInt());
      else if (key == "ap1")
        alarmList[1].setAMPM(value.toInt());
      else if (key == "ms1")
        alarmList[1].setMsg(value);
      else if (key == "h2")
        alarmList[2].setH(value.toInt());
      else if (key == "m2")
        alarmList[2].setM(value.toInt());
      else if (key == "ap2")
        alarmList[2].setAMPM(value.toInt());
      else if (key == "ms2")
        alarmList[2].setMsg(value);
      else if (key == "h3")
        alarmList[3].setH(value.toInt());
      else if (key == "m3")
        alarmList[3].setM(value.toInt());
      else if (key == "ap3")
        alarmList[3].setAMPM(value.toInt());
      else if (key == "ms3")
        alarmList[3].setMsg(value);
      else if (key == "mod")
        selectedMode = value;
      else if (key == "hl")
        hourLong = value;
      else if (key == "hs")
        hourShort = value;
      else if (key == "ml")
        minuteLong = value;
      else if (key == "ms")
        minuteShort = value;
      else if (key == "vo")
        volume = value;
      else if (key == "vi")
        vibration = value;
    }
  }
}

void showTimeScreen()
{
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  getRTCTime();
  display.print(Time);
  display.display();
}

void showSOSScreen()
{
  for(int i=5;i>=0;i--)
  {
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(8,0);
    display.println("Sending SOS in ...");
    display.setTextSize(3);
    display.setCursor(56,12);
    display.println(i);
    if (digitalRead(screenBrowseButton) == LOW)
    {
      currentScreen = 0;
      return;
    }
    delay(1000);
    display.display();
  }
  delay(5000);
}

void showFMWScreen()
{
  while (digitalRead(screenBrowseButton) != LOW)
  {
    delay(200);
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(26,10);
    display.println("Finding your");
    display.setCursor(26,18);
    display.print("watch .....");
    display.display();
    // TODO: Add speaker code
    buzz(2, 200);
    delay(200);
    display.clearDisplay();
    display.display();
  }
}

void showAlarmScreen(alarm alarm1)
{
  while (digitalRead(screenBrowseButton) != LOW)
  {
    delay(200);
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(12,4);
    display.print(alarm1.getH());
    display.print(":");
    display.print(alarm1.getM());
    if (alarm1.getAMPM() == 0)
      display.println("AM");
    else
      display.println("PM");
    display.setTextSize(1);
    display.setCursor(10,22);
    display.println(alarm1.getMsg());
    display.display();
    // TODO: Add speaker code
    buzz(2, 200);
    delay(200);
    display.clearDisplay();
    display.display();
  }  
}

void showAlarmListScreen()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  for(int i=0; i<4; i++)
  {
    if (alarmList[i].getAMPM() != -1)
    {
      char c = '1'+i;
      String j;
      j.concat(c);
      display.print("Alarm "+j+" - ");
      display.print(alarmList[i].getH());
      display.print(":");
      display.print(alarmList[i].getM());
      if (alarmList[i].getAMPM() == 0)
          display.println("AM");
        else
          display.println("PM");  
    }
  }
  display.display();
}

void showSVScreen(int volume, int vibration)
{
  int i;
  int x_start=0, y_start=8;
  int rect_width=20, rect_height=5;

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  display.setCursor(0, 0);
  display.println("Volume");
  for(i=1;i<=5;i++)
  {
    int colorShow = volume>=i?WHITE:BLACK;
    display.fillRect(x_start, y_start, rect_width, rect_height, colorShow);
    x_start = x_start + rect_width + 2;
  }

  x_start = 0;
  y_start = 24;
  display.setCursor(0, 16);
  display.println("Vibration");
  for(i=1;i<=53;i++)
  {
    int colorShow = vibration>=i?WHITE:BLACK;
    display.fillRect(x_start, y_start, rect_width, rect_height, colorShow);
    x_start = x_start + rect_width + 2;
  }
  display.display();
}

void showModeScreen()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Mode: ");
  if (selectedMode == "0")
    display.println("Terse");
  else if (selectedMode == "1")
    display.println("Digit");
  else if (selectedMode == "2")
    display.println("Custom");
  display.println("");
  display.print("Hour (L, S): ");
  display.print(hourLong);
  display.print(", ");
  display.println(hourShort);
  display.print("Minute (L, S): ");
  display.print(minuteLong);
  display.print(", ");
  display.println(minuteShort);  
  display.display();
}

keyValue extractKeyValue()
{
  String key="", value="";
  while(true)
  {
    char ch = Serial.read();
    if(ch == '*')
    {
      while (true)
      {
        char c = Serial.read();
        if(c == '}')
        {
          keyValue abc = keyValue(key, value);
          return(abc);
        }
        value = appendChar(value, c);
      }
    }
    key = appendChar(key, ch);
  }
}

String appendChar(String s, char ch)
{
  // Upper Case || Lower Case || Numbers || space || colon || comma
  if((ch>=65 && ch<=90) || (ch>=97 && ch<=122) || (ch>=48 && ch<=57) || ch==32 || ch==58 || ch==44)
  {
    s = s + ch;
  }
  return s;
}

void printRoutine()
{
  Serial.println();
  Serial.print("Time: ");
  Serial.println(Time);
  Serial.print("Number of Alarms: ");
  Serial.println(numberOfAlarms);
  for (int i=0; i<4; i++)
  {
    Serial.print(alarmList[i].getH());
    Serial.print(":");
    Serial.print(alarmList[i].getM());
    if (alarmList[i].getAMPM() == 0)
        Serial.print("AM, ");
      else
        Serial.print("PM, ");  
    Serial.println(alarmList[i].getMsg());
  }
  Serial.print("Volume: ");
  Serial.println(volume);
  Serial.print("Vibration: ");
  Serial.println(vibration);
  Serial.print("selectedMode: ");
  Serial.println(selectedMode);
  Serial.print("hourLong: ");
  Serial.println(hourLong);
  Serial.print("hourShort: ");
  Serial.println(hourShort);
  Serial.print("minuteLong: ");
  Serial.println(minuteLong);
  Serial.print("minuteShort: ");
  Serial.println(minuteShort);
}

void getRTCTime()
{
  Wire.beginTransmission(RTC_Address);                 // Start I2C protocol with DS3231 address
  Wire.write(0);                                // Send register address
  Wire.endTransmission(false);                  // I2C restart
  Wire.requestFrom(0x68, 7);                    // Request 7 bytes from DS3231 and release I2C bus at end of reading
  second = Wire.read();                         // Read seconds from register 0
  minute = Wire.read();                         // Read minuts from register 1
  hour   = Wire.read();                         // Read hour from register 2
  day    = Wire.read();                         // Read day from register 3
  date   = Wire.read();                         // Read date from register 4
  month  = Wire.read();                         // Read month from register 5
  year   = Wire.read();                         // Read year from register 6
  Wire.beginTransmission(0x68);                 // Start I2C protocol with DS3231 address
  Wire.write(0x11);                             // Send register address
  Wire.endTransmission(false);                  // I2C restart
  Wire.requestFrom(0x68, 2);                    // Request 2 bytes from DS3231 and release I2C bus at end of reading
  temperature_msb = Wire.read();                // Read temperature MSB
  temperature_lsb = Wire.read();                // Read temperature LSB
   
  DS3231_display();
}

void DS3231_display()
{
  // Convert BCD to decimal
  second = (second >> 4) * 10 + (second & 0x0F);
  minute = (minute >> 4) * 10 + (minute & 0x0F);
  hour   = (hour >> 4)   * 10 + (hour & 0x0F);
  date   = (date >> 4)   * 10 + (date & 0x0F);
  month  = (month >> 4)  * 10 + (month & 0x0F);
  year   = (year >> 4)   * 10 + (year & 0x0F);
  // End conversion

  
  if ((int)hour > 12)
  {
    ampm = 1; // PM
    Time[8] = 'P';
    hour = hour - 12;
  }
  else if((int)hour == 0)
  {
    ampm = 0; // AM
    Time[8] = 'A';
    hour = hour + 12;
  }
  else if((int)hour == 12)
  {
    Time[8] = 'P';
    ampm = 1; // PM
  }
  else
  {
    Time[8] = 'A';
    ampm = 0; // AM
  }

  Time[9] = 'M';
  Time[7]     = second % 10 + 48;
  Time[6]     = second / 10 + 48;
  Time[4]     = minute % 10 + 48;
  Time[3]     = minute / 10 + 48;
  Time[1]     = hour   % 10 + 48;
  Time[0]     = hour   / 10 + 48;
  Calendar[9] = year   % 10 + 48;
  Calendar[8] = year   / 10 + 48;
  Calendar[4] = month  % 10 + 48;
  Calendar[3] = month  / 10 + 48;
  Calendar[1] = date   % 10 + 48;
  Calendar[0] = date   / 10 + 48;
  
  if(temperature_msb < 0)
  {
    temperature_msb = abs(temperature_msb);
    temperature[0] = '-';
  }
  else
    temperature[0] = ' ';
  
  temperature_lsb >>= 6;
  temperature[2] = temperature_msb % 10  + 48;
  temperature[1] = temperature_msb / 10  + 48;
  
  if(temperature_lsb == 0 || temperature_lsb == 2)
  {
    temperature[5] = '0';
    if(temperature_lsb == 0) 
      temperature[4] = '0';
    else                     
      temperature[4] = '5';
  }
  if(temperature_lsb == 1 || temperature_lsb == 3)
  {
    temperature[5] = '5';
    if(temperature_lsb == 1) 
      temperature[4] = '2';
    else                     
      temperature[4] = '7';
  }
}

void mapping(int j)
{
  switch (j) 
  {
    case 1:
    voice_output.say(spONE);
    break;
    case 2:
    voice_output.say(spTWO);
    break;
    case 3:
    voice_output.say(spTHREE);
    break;
    case 4:
    voice_output.say(spFOUR);
    break;
    case 5:
    voice_output.say(spFIVE);
    break;
    case 6:
    voice_output.say(spSIX);
    break;
    case 7:
    voice_output.say(spSEVEN);
    break;
    case 8:
    voice_output.say(spEIGHT);
    break;
    case 9:
    voice_output.say(spNINE);
    break;
    case 10:
    voice_output.say(spTEN);
    break;
    case 11:
    voice_output.say(spELEVEN);
    break;
    case 12:
    voice_output.say(spTWELVE);
    break;
    case 13:
    voice_output.say(spTHIRTEEN);
    break;
    case 14:
    voice_output.say(spFOURTEEN);
    break;
    case 15:
    voice_output.say(spFIFTEEN);
    break;
    case 16:
    voice_output.say(spSIXTEEN);
    break;
    case 17:
    voice_output.say(spSEVENTEEN);
    break;
    case 18:
    voice_output.say(spEIGHTEEN);
    break;
    case 19:
    voice_output.say(spNINETEEN);
    break;
    case 20:
    voice_output.say(spTWENTY);
    break;
    case 30:
    voice_output.say(spTHIRTY);
    break;
    case 40:
    voice_output.say(spFOURTY);
    break;
    case 50:
    voice_output.say(spFIFTY);
    break;
  }
  return;
}

void talkie_output(int h,int m1,int m2,int ampm)
{
  voice_output.say(spTHE);
  voice_output.say(spTIME);
  voice_output.say(spIS);
  mapping(h);
  if ( (m1*10 + m2) <= 19)
    mapping(m1*10 + m2);
  else
    {
      mapping(m1*10);
      mapping(m2);  
    }
  if (ampm == 0)
    voice_output.say(spA_M_);
  else
    voice_output.say(spP_M_);
  return;
}

void buzz(int numberOfBuzz, int buzzDuration)
{
  for(int i=0; i<numberOfBuzz; i++)
  {
    analogWrite(PWMPin, 255);
    delay(buzzDuration);
    analogWrite(PWMPin, 0);
    delay(buzzDuration);
  }
}

