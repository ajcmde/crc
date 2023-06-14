#pragma once

#include "freertos/FreeRTOS.h"

typedef struct {
    uint8_t crcbits; //8, 12, 16, 32, 64
    uint64_t polynom; 
    uint64_t polymask;
    uint64_t polyMSBit;
    uint64_t init;
    uint64_t XOrOut;
    bool RefIn;
    bool RefOut;

    union {
        uint8_t ui8[1];
        uint16_t ui16[1];
        uint32_t ui32[1];
        uint64_t ui64[1];
    } polytable;
} CRChandle_t;

extern CRChandle_t *CRCCreate(uint8_t crcbits, uint64_t polynom, uint64_t init, bool RefIn, bool RefOut, uint64_t XOrOut);
extern uint64_t CRC(CRChandle_t *CRChandle, uint8_t *buffer, size_t length);
