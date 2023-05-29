# CRC
# tags 
#ESP32, #CRC, #CRC-8, #CRC-16, #CRC-32, #CRC-64

# description
provides functions to create a crc table and calculate a crc (8, 16, 32 or 64bit)

# notes
tested on ESP32 w/ 16bit polynom

# language
C

# usage
  1. initialize crc by providig necessary details: 
  CRChandle_t *CRCCreate(uint8_t crcbits, uint64_t polynom, uint64_t init, bool RefIn, bool RefOut, uint64_t XOrOut)
  2. calculate crc:  
  uint64_t CRC(CRChandle_t *CRChandle, uint8_t *buffer, size_t length)

# dependency
  https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html

# reference
  http://graphics.stanford.edu/~seander/bithacks.html#BitReverseTable
  https://en.wikipedia.org/wiki/Cyclic_redundancy_check
