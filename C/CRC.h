/*
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * 
 * For more information, please refer to <https://unlicense.org>
 */

#pragma once

#include "freertos/FreeRTOS.h"

#if defined(CONFIG_CRC_CRC64)
#define CRC_TYPE uint64_t  
#elif defined(CONFIG_CRC_CRC32)
#define CRC_TYPE uint32_t  
#elif defined(CONFIG_CRC_CRC16)
#define CRC_TYPE uint16_t  
#elif defined(CONFIG_CRC_CRC8)
#define CRC_TYPE uint8_t 
#else
#define CRC_TYPE uint64_t  
#endif


typedef CRC_TYPE CRC_t; // CRC data type

typedef struct {
    uint8_t CRCbits; // CRC polynom width
    uint8_t CRCpad; //  padding bits to next byte boundary [0-7]
    CRC_t Polynom; // CRC polynom
    CRC_t Init; // CRC initial value
    
    CRC_t XOrOut; // apply xor mask to CRC
    bool RefIn; // reflected input
    bool RefOut; // reflect CRC (before Xor)
    CRC_t Polymask; // polynom mask 
    CRC_t Polytable[256];  // precalculate CRC table
} CRChandle_t;

/**
 * @brief Destroys a CRC table which has been created by CRCCreate or CRCCreateFromName
 *
 * @param CRChandle handle to CRC table
 * 
 */
extern void CRCDestroy(CRChandle_t *CRChandle);

/**
 * @brief Calculate a CRC table for fast CRC processing.
 *
 * @param CRCbits CRC polynom width
 * @param Polynom CRC polynom
 * @param Init CRC initial value
 * @param RefIn reflect input
 * @param RefOut reflect CRC
 * @param XOrOut apply xor mask to CRC
 * 
 * @result Pointer to CRChandle_t. If memory allocation fails a NULL pointer will be returned 
 */
extern CRChandle_t *CRCCreate(uint8_t CRCbits, CRC_t Polynom, CRC_t Init, bool RefIn, bool RefOut, CRC_t XOrOut);

/**
 * @brief Calculate a CRC table for fast CRC processing from a named CRC.
 *
 * @param CRCname CRC name
 * 
 * @result Pointer to CRChandle_t. If CRCname points to NULL, CRC is unknown or, memory allocation fails a NULL pointer will be returned 
 */
extern CRChandle_t *CRCCreateFromName(char *CRCName);

/**
 * @brief Calculate CRC code of a block of bytes
 *
 * @param CRChandle handle to CRC table
 * @param Buffer pointer to memory block 
 * @param Length length of memory buffer in bytes
 * 
 * @result CRC. Will return 0 if CRChandle is NULL 
 */
extern CRC_t CRC(const CRChandle_t *CRChandle, const uint8_t *Buffer, size_t Length);

#ifndef CRC_CREATECODE_LINELENGTH
// max line length of the polytable C code
#define CRC_CREATECODE_LINELENGTH 128
#endif

/**
 * @brief Create C source code of the CRC table. 
 *
 * @param CRChandle handle to CRC table
 * 
 * @result Pointer to string with generated source code. Function will fail if CRChandle is NULL or if memory allocation failed. 
 */
extern char *CRCCreateCCode(const CRChandle_t *CRChandle);
