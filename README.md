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
    crcbits: Number of bits for CRC value. Supported values are 8, 16, 32, and 64.  
    polynom: CRC polynom  
    init: CRC initial value (usually 0 or -1)  
    RefIn: reverse bit order of input stream (true/ false)  
    RefOut: reverse CRC value (true/ false)  
    XOrOut: Xor CRC value with XOrOut value  
  > Function will fail, if number of crcbits are not supported or system does not provided enough memory for the CRC table (8 * crcbits bytes)
  2. Calculate CRC:  
  uint64_t CRC(CRChandle_t *CRChandle, uint8_t *buffer, size_t length)
  
# example
    #include "CRC.h"
    
    static CRChandle_t *CRC_CCITT16 = NULL;
   
    ...
     
    // create CRC table for smart message protocol (SML) once: 
    //    16bits 
    //    polynom is x^{16} + x^{12} + x^5 + 1
    //    initial value: 0xffff
    //    reverse input and output
    //    toggle all output bits
    if(!CRC_CCITT16)
        CRC_CCITT16 = CRCCreate(16, 0x1021, 0xffff, true, true, 0xffff);
    
    // calculate CRC
    crc16 = CRC(CRC_CCITT16, buf, len);    

# dependency
  https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html

# reference
  http://graphics.stanford.edu/~seander/bithacks.html#BitReverseTable
  
  https://en.wikipedia.org/wiki/Cyclic_redundancy_check
