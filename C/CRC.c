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

/* PLANTUML 
@startuml
class CRC_t << T, #FF7700 >>
CRC_t <|-- CRChandle_t
class CRChandle_t << T, #FF7700 >> {
  -CRC_t CRCbits
  -int8_t CRCpad
  -CRC_t Polynom
  -CRC_t Init
  -CRC_t XOrOut 
  -bool RefIn
  -bool RefOut
  -CRC_t Polymask 
  -CRC_t Polytable[256]
--
  +CRChandle_t CreateCRC()
  +CRChandle_t CreateCRCFromName()
..
  +CRC_t CRC()
..
  +void CRCDestroy()
..
  +char *CRCCreateCCode()
..
  -CRC_t CRCReflect()
}
hide empty members
@enduml
*/

#include "CRC.h"
#include "string.h"

/**
 * @brief Helper function which reflects the bit order of a number
 *
 * @param number number 
 * @param bits number of bits to be reflected 
 * @param bitspad number of bits to the CRC byte boundary (0..7)
 * 
 * @result number with reflected bits 
 */
static CRC_t CRCReflect(CRC_t number, uint8_t bits, uint8_t bitspad)
{
    static const uint8_t ReflectTable[] = {
        0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0, 0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,
        0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8, 0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,
        0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4, 0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,
        0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec, 0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,
        0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2, 0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,
        0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea, 0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,
        0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6, 0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,
        0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee, 0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,
        0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1, 0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,
        0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9, 0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,
        0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5, 0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,
        0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed, 0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,
        0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3, 0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,
        0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb, 0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,
        0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7, 0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,
        0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef, 0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff
    };

    CRC_t result = 0;
    int i;
    uint8_t bytes = (bits + bitspad) / 8;
    for(i = 0; i < bytes; i++) {
        result = (result << 8) | ReflectTable[number & 0xff];
        number >>= 8;
    }
    result >>= bitspad; // align to CRC bits
    return(result);

}

void CRCDestroy(CRChandle_t *CRChandle)
{
    if(CRChandle) {
        free(CRChandle);
    }
}

__attribute__((no_sanitize("shift")))
CRChandle_t *CRCCreate(uint8_t CRCbits, CRC_t Polynom, CRC_t Init, bool RefIn, bool RefOut, CRC_t XOrOut)
{
    CRChandle_t *CRChandle;
    CRC_t CRC;
    CRC_t Polymsb, Polymask;
    int i, j;

    if(CRCbits == 0 || CRCbits > sizeof(CRC_t) * 8) {
        return(NULL);
    }
    CRChandle = calloc(sizeof(CRChandle_t), 1);
    if(!CRChandle) {
        return(NULL);
    }
    CRChandle->CRCbits = CRCbits;
    
    Polymask = CRChandle->Polymask = ((sizeof(CRC_t) * 8 == CRCbits) ? 0 : (((CRC_t)1) << CRCbits)) - 1; // fix undefined shift behavior 
    Polymsb = (((CRC_t)1) << (CRCbits - 1));
    CRChandle->Polynom = Polynom;
    CRChandle->Init = Init & CRChandle->Polymask;
    CRChandle->RefIn = RefIn;
    CRChandle->RefOut = RefOut;
    CRChandle->XOrOut = XOrOut & CRChandle->Polymask;
    CRChandle->CRCpad = (8 - (CRChandle->CRCbits & 7)) & 7;  // non byte aligned CRC;

    if(!CRChandle->RefIn) {
        Polynom <<= CRChandle->CRCpad;
        Polymsb <<= CRChandle->CRCpad;
        Polymask <<= CRChandle->CRCpad;
    }

    // initialize table
    CRC = Polymsb;
    CRChandle->Polytable[0] = 0; // reminder of 0 is 0

    for(i = 1; i < 256; i <<= 1) {
        if(CRC & Polymsb) {
            CRC = (CRC << 1) ^ Polynom;
        }
        else {
            CRC <<= 1;
        }
        CRC &= Polymask;

        for(j = 0; j < i; j++) {
            CRChandle->Polytable[(CRChandle->RefIn) ? CRCReflect(i + j, 8, 0) : (i + j)] = 
                (CRC) ^ CRChandle->Polytable[(CRChandle->RefIn) ? CRCReflect(j, 8, 0) : j];
        }
    }    
    if(CRChandle->RefIn) {
        for(i = 1; i < 256; i++) {    
            CRChandle->Polytable[i] = CRCReflect(CRChandle->Polytable[i], CRCbits, CRChandle->CRCpad);
        }
    }

    return(CRChandle);
}


typedef struct
{
    char *Name; // CRC name
    uint8_t CRCbits; // CRC polynom width 
    CRC_t Polynom; // CRC polynom
    CRC_t Init; // CRC initial value
    bool RefIn; // reflected input
    bool RefOut; // reflect CRC (before Xor)
    CRC_t XOrOut; // apply xor mask to CRC
} CRCdefinition_t;

CRChandle_t *CRCCreateFromName(const char *CRCName)
{
    // reference: https://reveng.sourceforge.io/crc-catalogue/all.htm
    const CRCdefinition_t CRCs[] =
    {
        { "CRC-3/GSM", 3, 0x3, 0x0, false, false, 0x7},
        { "CRC-3/ROHC", 3, 0x3, 0x7, true, true, 0x0},
        { "CRC-4/G-704", 4, 0x3, 0x0, true, true, 0x0},
        { "CRC-4/INTERLAKEN", 4, 0x3, 0xf, false, false, 0xf},
        { "CRC-5/EPC-C1G2", 5, 0x09, 0x09, false, false, 0x0},
        { "CRC-5/G-704", 5, 0x15, 0x0, true, true, 0x0},
        { "CRC-5/USB", 5, 0x05, 0x1f, true, true, 0x1f},
        { "CRC-6/CDMA2000-A", 6, 0x27, 0x3f, false, false, 0x0},
        { "CRC-6/CDMA2000-B", 6, 0x07, 0x3f, false, false, 0x0},
        { "CRC-6/DARC", 6, 0x19, 0x0, true, true, 0x0},
        { "CRC-6/G-704", 6, 0x03, 0x0, true, true, 0x0},
        { "CRC-6/GSM", 6, 0x2f, 0x0, false, false, 0x3f},
        { "CRC-7/MMC", 7, 0x09, 0x0, false, false, 0x0},
        { "CRC-7/ROHC", 7, 0x4f, 0x7f, true, true, 0x0},
        { "CRC-7/UMTS", 7, 0x45, 0x0, false, false, 0x0},
        { "CRC-8/AUTOSAR", 8, 0x2f, 0xff, false, false, 0xff},
        { "CRC-8/BLUETOOTH", 8, 0xa7, 0x0, true, true, 0x0},
        { "CRC-8/CDMA2000", 8, 0x9b, 0xff, false, false, 0x0},
        { "CRC-8/DARC", 8, 0x39, 0x0, true, true, 0x0},
        { "CRC-8/GSM-B", 8, 0x49, 0x0, false, false, 0xff},
        { "CRC-8/HITAG", 8, 0x1d, 0xff, false, false, 0x0},
        { "CRC-8/I-432-1", 8, 0x07, 0x0, false, false, 0x55},
        { "CRC-8/I-CODE", 8, 0x1d, 0xfd, false, false, 0x0},
        { "CRC-8/LTE", 8, 0x9b, 0x0, false, false, 0x0},
        { "CRC-8/MAXIM-DOW", 8, 0x31, 0x0, true, true, 0x0},
        { "CRC-8/MIFARE-MAD", 8, 0x1d, 0xc7, false, false, 0x0},
        { "CRC-8/NRSC-5", 8, 0x31, 0xff, false, false, 0x0},
        { "CRC-8/OPENSAFETY", 8, 0x2f, 0x0, false, false, 0x0},
        { "CRC-8/ROHC", 8, 0x07, 0xff, true, true, 0x0},
        { "CRC-8/SAE-J1850", 8, 0x1d, 0xff, false, false, 0xff},
        { "CRC-8/SMBUS", 8, 0x07, 0x0, false, false, 0x0},
        { "CRC-8/TECH-3250", 8, 0x1d, 0xff, true, true, 0x0},
        { "CRC-8/WCDMA", 8, 0x9b, 0x0, true, true, 0x0},
    #if defined(CONFIG_CRC_CRC16) || defined(CONFIG_CRC_CRC32) || defined(CONFIG_CRC_CRC64)     
        { "CRC-10/ATM", 10, 0x233, 0x0, false, false, 0x0},
        { "CRC-10/CDMA2000", 10, 0x3d9, 0x3ff, false, false, 0x0},
        { "CRC-10/GSM", 10, 0x175, 0x0, false, false, 0x3ff},
        { "CRC-11/FLEXRAY", 11, 0x385, 0x01a, false, false, 0x0},
        { "CRC-11/UMTS", 11, 0x307, 0x0, false, false, 0x0},
        { "CRC-12/CDMA2000", 12, 0xf13, 0xfff, false, false, 0x0},
        { "CRC-12/DECT", 12, 0x80f, 0x0, false, false, 0x0},
        { "CRC-12/GSM", 12, 0xd31, 0x0, false, false, 0xfff},
        { "CRC-12/UMTS", 12, 0x80f, 0x0, false, true, 0x0},
        { "CRC-13/BBC", 13, 0x1cf5, 0x0, false, false, 0x0},
        { "CRC-14/DARC", 14, 0x0805, 0x0, true, true, 0x0},
        { "CRC-14/GSM", 14, 0x202d, 0x0, false, false, 0x3fff},
        { "CRC-15/CAN", 15, 0x4599, 0x0, false, false, 0x0},
        { "CRC-15/MPT1327", 15, 0x6815, 0x0, false, false, 0x01},
        { "CRC-16/ARC", 16, 0x8005, 0x0, true, true, 0x0},
        { "CRC-16/CDMA2000", 16, 0xc867, 0xffff, false, false, 0x0},
        { "CRC-16/CMS", 16, 0x8005, 0xffff, false, false, 0x0},
        { "CRC-16/DDS-110", 16, 0x8005, 0x800d, false, false, 0x0},
        { "CRC-16/DECT-R", 16, 0x0589, 0x0, false, false, 0x01},
        { "CRC-16/DECT-X", 16, 0x0589, 0x0, false, false, 0x0},
        { "CRC-16/DNP", 16, 0x3d65, 0x0, true, true, 0xffff},
        { "CRC-16/EN-13757", 16, 0x3d65, 0x0, false, false, 0xffff},
        { "CRC-16/GENIBUS", 16, 0x1021, 0xffff, false, false, 0xffff},
        { "CRC-16/GSM", 16, 0x1021, 0x0, false, false, 0xffff},
        { "CRC-16/IBM-3740", 16, 0x1021, 0xffff, false, false, 0x0},
        { "CRC-16/IBM-SDLC", 16, 0x1021, 0xffff, true, true, 0xffff},
        { "CRC-16/ISO-IEC-14443-3-A", 16, 0x1021, 0xc6c6, true, true, 0x0},
        { "CRC-16/KERMIT", 16, 0x1021, 0x0, true, true, 0x0},
        { "CRC-16/LJ1200", 16, 0x6f63, 0x0, false, false, 0x0},
        { "CRC-16/M17", 16, 0x5935, 0xffff, false, false, 0x0},
        { "CRC-16/MAXIM-DOW", 16, 0x8005, 0x0, true, true, 0xffff},
        { "CRC-16/MCRF4XX", 16, 0x1021, 0xffff, true, true, 0x0},
        { "CRC-16/MODBUS", 16, 0x8005, 0xffff, true, true, 0x0},
        { "CRC-16/NRSC-5", 16, 0x080b, 0xffff, true, true, 0x0},
        { "CRC-16/OPENSAFETY-A", 16, 0x5935, 0x0, false, false, 0x0},
        { "CRC-16/OPENSAFETY-B", 16, 0x755b, 0x0, false, false, 0x0},
        { "CRC-16/PROFIBUS", 16, 0x1dcf, 0xffff, false, false, 0xffff},
        { "CRC-16/RIELLO", 16, 0x1021, 0xb2aa, true, true, 0x0},
        { "CRC-16/SPI-FUJITSU", 16, 0x1021, 0x1d0f, false, false, 0x0},
        { "CRC-16/T10-DIF", 16, 0x8bb7, 0x0, false, false, 0x0},
        { "CRC-16/TELEDISK", 16, 0xa097, 0x0, false, false, 0x0},
        { "CRC-16/TMS37157", 16, 0x1021, 0x89ec, true, true, 0x0},
        { "CRC-16/UMTS", 16, 0x8005, 0x0, false, false, 0x0},
        { "CRC-16/USB", 16, 0x8005, 0xffff, true, true, 0xffff},
        { "CRC-16/XMODEM", 16, 0x1021, 0x0, false, false, 0x0},
    #endif
    #if defined(CONFIG_CRC_CRC32) || defined(CONFIG_CRC_CRC64)  
        { "CRC-17/CAN-FD", 17, 0x1685b, 0x0, false, false, 0x0},
        { "CRC-21/CAN-FD", 21, 0x102899, 0x0, false, false, 0x0},
        { "CRC-24/BLE", 24, 0x065b, 0x555555, true, true, 0x0},
        { "CRC-24/FLEXRAY-A", 24, 0x5d6dcb, 0xfedcba, false, false, 0x0},
        { "CRC-24/FLEXRAY-B", 24, 0x5d6dcb, 0xabcdef, false, false, 0x0},
        { "CRC-24/INTERLAKEN", 24, 0x328b63, 0xffffff, false, false, 0xffffff},
        { "CRC-24/LTE-A", 24, 0x864cfb, 0x0, false, false, 0x0},
        { "CRC-24/LTE-B", 24, 0x800063, 0x0, false, false, 0x0},
        { "CRC-24/OPENPGP", 24, 0x864cfb, 0xb704ce, false, false, 0x0},
        { "CRC-24/OS-9", 24, 0x800063, 0xffffff, false, false, 0xffffff},
        { "CRC-30/CDMA", 30, 0x2030b9c7, 0x3fffffff, false, false, 0x3fffffff},
        { "CRC-31/PHILIPS", 31, 0x04c11db7, 0x7fffffff, false, false, 0x7fffffff},
        { "CRC-32/AIXM", 32, 0x814141ab, 0x0, false, false, 0x0},
        { "CRC-32/AUTOSAR", 32, 0xf4acfb13, 0xffffffff, true, true, 0xffffffff},
        { "CRC-32/BASE91-D", 32, 0xa833982b, 0xffffffff, true, true, 0xffffffff},
        { "CRC-32/BZIP2", 32, 0x04c11db7, 0xffffffff, false, false, 0xffffffff},
        { "CRC-32/CD-ROM-EDC", 32, 0x8001801b, 0x0, true, true, 0x0},
        { "CRC-32/CKSUM", 32, 0x04c11db7, 0x0, false, false, 0xffffffff},
        { "CRC-32/ISCSI", 32, 0x1edc6f41, 0xffffffff, true, true, 0xffffffff},
        { "CRC-32/ISO-HDLC", 32, 0x04c11db7, 0xffffffff, true, true, 0xffffffff},
        { "CRC-32/JAMCRC", 32, 0x04c11db7, 0xffffffff, true, true, 0x0},
        { "CRC-32/MEF", 32, 0x741b8cd7, 0xffffffff, true, true, 0x0},
        { "CRC-32/MPEG-2", 32, 0x04c11db7, 0xffffffff, false, false, 0x0},
        { "CRC-32/XFER", 32, 0x0af, 0x0, false, false, 0x0},
    #endif
    #if defined(CONFIG_CRC_CRC64)
        { "CRC-40/GSM", 40, 0x04820009, 0x0, false, false, 0xffffffffff},
        { "CRC-64/ECMA-182", 64, 0x42f0e1eba9ea3693, 0x0, false, false, 0x0},
        { "CRC-64/GO-ISO", 64, 0x01b, 0xffffffffffffffff, true, true, 0xffffffffffffffff},
        { "CRC-64/MS", 64, 0x259c84cba6426349, 0xffffffffffffffff, true, true, 0x0},
        { "CRC-64/REDIS", 64, 0xad93d23594c935a9, 0x0, true, true, 0x0},
        { "CRC-64/WE", 64, 0x42f0e1eba9ea3693, 0xffffffffffffffff, false, false, 0xffffffffffffffff},
        { "CRC-64/XZ", 64, 0x42f0e1eba9ea3693, 0xffffffffffffffff, true, true, 0xffffffffffffffff},
    #endif
        {/*end of table*/}
    };
    const CRCdefinition_t *crc;

    if(!CRCName) {
            return(NULL);
    }
    for(crc = CRCs; crc->Name; crc++) {
        if(!strcmp(CRCName, crc->Name)) {
            return(CRCCreate(crc->CRCbits, crc->Polynom, crc->Init, crc->RefIn, crc->RefOut, crc->XOrOut));
        }
    }
    return(NULL);
}


__attribute__((no_sanitize("shift")))
CRC_t CRC(const CRChandle_t *CRChandle, const uint8_t *Buffer, size_t Length)
{
    CRC_t CRC;
    size_t i;

    if(!CRChandle) {
        return(0);
    }
    CRC = CRChandle->Init; 

    if(CRChandle->RefIn) {
        for(i = 0; i < Length; i++) { 
            CRC = CRChandle->Polytable[(Buffer[i] ^ CRC) & 0xff] ^ (CRC >> 8);
        }
    }
    else {
        int16_t CRCbits_8 = (CRChandle->CRCbits - 1) & (((CRC_t)-1) ^ 7); // CRC bits aligned to next byte border (12 -> 8, 32 -> 24)
        for(i = 0; i < Length; i++) {
            CRC = CRChandle->Polytable[((CRC >> CRCbits_8) & 0xff) ^ Buffer[i]] ^ (CRC << 8);
        }
        CRC >>= CRChandle->CRCpad;  // non byte aligned CRC
        CRC &= CRChandle->Polymask;
    }

    if(CRChandle->RefOut ^ CRChandle->RefIn) {
        CRC = CRCReflect(CRC, CRChandle->CRCbits, CRChandle->CRCpad);
    }
    CRC ^= CRChandle->XOrOut;

    return(CRC);
}




extern int	vsnprintf (char *__restrict, size_t, const char *__restrict, __VALIST)
               _ATTRIBUTE ((__format__ (__printf__, 3, 0)));

/**
 * @brief Helper function which prints string to buffer if not NULL. If the caller provides NULL as pointer to the buffer the function will add the bytes needed to print the string to paramter n.
 * Otherwise the buffer pointer is increased and the length is decreased by the number of bytes printed. 
 *
 * @param buffer pointer to character string
 * @param n pointer to length of buffer
 * @param fmt format string
 * 
 * @result length of bytes needed/ printed
 */
static int CRCvnfrintf(char **buffer, size_t *n, const char *fmt, ...)
{
    char temp[1];
    char *_buffer;
    size_t _n;

    if(!buffer) {
        return(-1);
    }
    _buffer = *buffer;
    if(!_buffer) {
        _n = 1;
        _buffer = temp;
    }
    else { 
        _n = *n;
    }
    va_list list;
    va_start(list, fmt);
    int result = vsnprintf(_buffer, _n, fmt, list);
    va_end(list);

    if(result < 0) {
        return(0);
    }   
    if(_buffer != temp) {
        *buffer += result;
        *n -= result;
    }
    else {
        *n += result;
    }
    return(result);
}

char *CRCCreateCCode(const CRChandle_t *CRChandle)
{
    size_t Length;
    char *_Buffer, *Buffer;
    char *Sep1, *Sep2, *Sep3;
    int ValuesDigits;
    int ValuesRow;

    if(!CRChandle) {
        return(NULL);
    }

    Buffer = NULL;
    Length = 1;
    
    for(int Pass = 0; Pass < 2; Pass++) {
        CRCvnfrintf(&Buffer, &Length, "CRChandle_t CRChandle_%llx = {\n", (uint64_t)CRChandle->Polynom);



        CRCvnfrintf(&Buffer, &Length, "  .CRCbits = %u,\n", CRChandle->CRCbits);
        CRCvnfrintf(&Buffer, &Length, "  .CRCpad = %u,\n", CRChandle->CRCpad);
        CRCvnfrintf(&Buffer, &Length, "  .Polynom = 0x%llx,\n", (uint64_t)CRChandle->Polynom);
        CRCvnfrintf(&Buffer, &Length, "  .Init = 0x%llx,\n", (uint64_t)CRChandle->Init);
        CRCvnfrintf(&Buffer, &Length, "  .XOrOut = 0x%llx,\n", (uint64_t)CRChandle->XOrOut);
        CRCvnfrintf(&Buffer, &Length, "  .RefIn = %s,\n", (CRChandle->RefIn) ? "true": "false");
        CRCvnfrintf(&Buffer, &Length, "  .RefOut = %s,\n", (CRChandle->RefOut) ? "true": "false");
        CRCvnfrintf(&Buffer, &Length, "  .Polymask = 0x%llx,\n", (uint64_t)CRChandle->Polymask);
        CRCvnfrintf(&Buffer, &Length, "  .Polytable = {\n");

        ValuesDigits = (CRChandle->CRCbits + ((CRChandle->RefIn) ? 3 : CRChandle->CRCpad)) >> 2;
        ValuesRow = CRC_CREATECODE_LINELENGTH / (ValuesDigits + 4);
        for(int i = 0; i < 256; i++) {
            Sep1 = (i % ValuesRow  == 0) ? "   " : "";
            Sep2 = (i != 255) ? "," : "";
            Sep3 = (i % ValuesRow  == ValuesRow - 1 || i == 255) ? "\n" : "";

            CRCvnfrintf(&Buffer, &Length,"%s 0x%ll0*x%s%s", Sep1, ValuesDigits, (uint64_t)CRChandle->Polytable[i], Sep2, Sep3);
        }           
        CRCvnfrintf(&Buffer, &Length, "  }");
        CRCvnfrintf(&Buffer, &Length, "};");

        if(Pass == 0) {
            _Buffer = Buffer = malloc(Length);
        }
    }
    return(_Buffer);
}

char *CRCCreateCCode2(const CRChandle_t *CRChandle)
{
    size_t Length;
    char *_Buffer, *Buffer;
    char *Sep1, *Sep2, *Sep3;
    int ValuesDigits;
    int ValuesRow;

    if(!CRChandle) {
        return(NULL);
    }

    int16_t CRCbits_8 = (CRChandle->CRCbits - 1) & (((CRC_t)-1) ^ 7); // CRC bits aligned to next byte border (12 -> 8, 32 -> 24)
    int16_t CRCbits2 = (CRChandle->CRCbits <= 8) ? 8 : (CRChandle->CRCbits <= 16) ? 16 : (CRChandle->CRCbits <= 32) ? 32 : (CRChandle->CRCbits <= 64) ? 64 : -1;

    Buffer = NULL;
    Length = 1;
    ValuesDigits = CRCbits2 >> 2;
    ValuesRow = CRC_CREATECODE_LINELENGTH / (ValuesDigits + 4);
    
    for(int Pass = 0; Pass < 2; Pass++) {

        // CODE: header
        CRCvnfrintf(&Buffer, &Length, "extern uint%d_t CRC(const uint8_t *Buffer, size_t Length);\n\n", CRCbits2);
        // CODE: include
        CRCvnfrintf(&Buffer, &Length, "#include <stdint.h>\n\n");
        if(CRChandle->RefOut ^ CRChandle->RefIn) {
                // CODE: supporting data for reflecting
                CRCvnfrintf(&Buffer, &Length, "static const uint8_t _CRCReflectTable[] = {\n");
                CRCvnfrintf(&Buffer, &Length, "  0x00, 0x80, 0x40, 0xc0, 0x20, 0xa0, 0x60, 0xe0, 0x10, 0x90, 0x50, 0xd0, 0x30, 0xb0, 0x70, 0xf0,\n");
                CRCvnfrintf(&Buffer, &Length, "  0x08, 0x88, 0x48, 0xc8, 0x28, 0xa8, 0x68, 0xe8, 0x18, 0x98, 0x58, 0xd8, 0x38, 0xb8, 0x78, 0xf8,\n");
                CRCvnfrintf(&Buffer, &Length, "  0x04, 0x84, 0x44, 0xc4, 0x24, 0xa4, 0x64, 0xe4, 0x14, 0x94, 0x54, 0xd4, 0x34, 0xb4, 0x74, 0xf4,\n");
                CRCvnfrintf(&Buffer, &Length, "  0x0c, 0x8c, 0x4c, 0xcc, 0x2c, 0xac, 0x6c, 0xec, 0x1c, 0x9c, 0x5c, 0xdc, 0x3c, 0xbc, 0x7c, 0xfc,\n");
                CRCvnfrintf(&Buffer, &Length, "  0x02, 0x82, 0x42, 0xc2, 0x22, 0xa2, 0x62, 0xe2, 0x12, 0x92, 0x52, 0xd2, 0x32, 0xb2, 0x72, 0xf2,\n");
                CRCvnfrintf(&Buffer, &Length, "  0x0a, 0x8a, 0x4a, 0xca, 0x2a, 0xaa, 0x6a, 0xea, 0x1a, 0x9a, 0x5a, 0xda, 0x3a, 0xba, 0x7a, 0xfa,\n");
                CRCvnfrintf(&Buffer, &Length, "  0x06, 0x86, 0x46, 0xc6, 0x26, 0xa6, 0x66, 0xe6, 0x16, 0x96, 0x56, 0xd6, 0x36, 0xb6, 0x76, 0xf6,\n");
                CRCvnfrintf(&Buffer, &Length, "  0x0e, 0x8e, 0x4e, 0xce, 0x2e, 0xae, 0x6e, 0xee, 0x1e, 0x9e, 0x5e, 0xde, 0x3e, 0xbe, 0x7e, 0xfe,\n");
                CRCvnfrintf(&Buffer, &Length, "  0x01, 0x81, 0x41, 0xc1, 0x21, 0xa1, 0x61, 0xe1, 0x11, 0x91, 0x51, 0xd1, 0x31, 0xb1, 0x71, 0xf1,\n");
                CRCvnfrintf(&Buffer, &Length, "  0x09, 0x89, 0x49, 0xc9, 0x29, 0xa9, 0x69, 0xe9, 0x19, 0x99, 0x59, 0xd9, 0x39, 0xb9, 0x79, 0xf9,\n");
                CRCvnfrintf(&Buffer, &Length, "  0x05, 0x85, 0x45, 0xc5, 0x25, 0xa5, 0x65, 0xe5, 0x15, 0x95, 0x55, 0xd5, 0x35, 0xb5, 0x75, 0xf5,\n");
                CRCvnfrintf(&Buffer, &Length, "  0x0d, 0x8d, 0x4d, 0xcd, 0x2d, 0xad, 0x6d, 0xed, 0x1d, 0x9d, 0x5d, 0xdd, 0x3d, 0xbd, 0x7d, 0xfd,\n");
                CRCvnfrintf(&Buffer, &Length, "  0x03, 0x83, 0x43, 0xc3, 0x23, 0xa3, 0x63, 0xe3, 0x13, 0x93, 0x53, 0xd3, 0x33, 0xb3, 0x73, 0xf3,\n");
                CRCvnfrintf(&Buffer, &Length, "  0x0b, 0x8b, 0x4b, 0xcb, 0x2b, 0xab, 0x6b, 0xeb, 0x1b, 0x9b, 0x5b, 0xdb, 0x3b, 0xbb, 0x7b, 0xfb,\n");
                CRCvnfrintf(&Buffer, &Length, "  0x07, 0x87, 0x47, 0xc7, 0x27, 0xa7, 0x67, 0xe7, 0x17, 0x97, 0x57, 0xd7, 0x37, 0xb7, 0x77, 0xf7,\n");
                CRCvnfrintf(&Buffer, &Length, "  0x0f, 0x8f, 0x4f, 0xcf, 0x2f, 0xaf, 0x6f, 0xef, 0x1f, 0x9f, 0x5f, 0xdf, 0x3f, 0xbf, 0x7f, 0xff};\n\n");
         }
        // CODE: function header
        CRCvnfrintf(&Buffer, &Length, "__attribute__((no_sanitize(\"shift\")))\n");
        CRCvnfrintf(&Buffer, &Length, "uint%d_t CRC(const uint8_t *Buffer, size_t Length)\n", CRCbits2);
        CRCvnfrintf(&Buffer, &Length, "{\n" );
        // CODE: description and variables
        CRCvnfrintf(&Buffer, &Length, "  // CRCbits: %d,", CRChandle->CRCbits);
        CRCvnfrintf(&Buffer, &Length, "Polynom: 0x%ll0*x, ", ValuesDigits, (uint64_t)CRChandle->Polynom);
        CRCvnfrintf(&Buffer, &Length, "Init: 0x%ll0*x, ", ValuesDigits, (uint64_t)CRChandle->Init);
        CRCvnfrintf(&Buffer, &Length, "RefIn: %s, ", (CRChandle->RefIn) ? "true" : "false");
        CRCvnfrintf(&Buffer, &Length, "RefOut: %s, ", (CRChandle->RefOut) ? "true" : "false");
        CRCvnfrintf(&Buffer, &Length, "XOrOut: 0x%ll0*x\n", ValuesDigits, (uint64_t)CRChandle->XOrOut);
        
        CRCvnfrintf(&Buffer, &Length, "  uint%d_t CRC = 0x%ll0*x;\n", CRCbits2, ValuesDigits, (uint64_t)CRChandle->Init);
        if((CRChandle->RefOut ^ CRChandle->RefIn) && (CRChandle->CRCbits > 8)) {
            // CODE: reflected CRC
            CRCvnfrintf(&Buffer, &Length, "  uint%d_t CRCref = 0;\n", CRCbits2);
        }
        CRCvnfrintf(&Buffer, &Length, "  size_t i;\n");
        // CODE: polytable
        CRCvnfrintf(&Buffer, &Length, "  const uint%d_t Polytable[] = {\n", CRCbits2);
        for(int i = 0; i < 256; i++) {
            Sep1 = (i % ValuesRow  == 0) ? "   " : "";
            Sep2 = (i != 255) ? "," : "";
            Sep3 = (i % ValuesRow  == ValuesRow - 1 || i == 255) ? "\n" : "";
            CRCvnfrintf(&Buffer, &Length,"%s 0x%ll0*x%s%s", Sep1, ValuesDigits, (uint64_t)CRChandle->Polytable[i], Sep2, Sep3);
        }           
        CRCvnfrintf(&Buffer, &Length, "  };\n" );

        // CODE: loop
        CRCvnfrintf(&Buffer, &Length, "  for(i = 0; i < Length; i++) {\n" );
        if(CRChandle->RefIn) {
            if(CRChandle->CRCbits <= 8) {
                // CODE: <=8bit, Refin 
                CRCvnfrintf(&Buffer, &Length, "    CRC = Polytable[Buffer[i] ^ CRC];\n");
            }
            else {
                // CODE: >8bit, Refin  
                CRCvnfrintf(&Buffer, &Length, "    CRC = Polytable[(Buffer[i] ^ CRC) & 0xff] ^ (CRC >> 8);\n");
            }
        }
        else {
            if(CRChandle->CRCbits <= 8) {
                // CODE: <=8bit, not Refin
                CRCvnfrintf(&Buffer, &Length, "    CRC = Polytable[CRC ^ Buffer[i]];\n");
            }
            else {
                // CODE: >8bit, not Refin
                CRCvnfrintf(&Buffer, &Length, "    CRC = Polytable[((CRC >> %d) & 0xff) ^ Buffer[i]] ^ (CRC << 8);\n", CRCbits_8);
            }
        }
        // CODE: end of loop
        CRCvnfrintf(&Buffer, &Length, "  }\n" );
        
        if(!CRChandle->RefIn) {
            if (CRChandle->CRCpad) {
                // CODE: not RefIn, CRCpad >0   
                CRCvnfrintf(&Buffer, &Length, "  CRC >>= %d;\n", CRChandle->CRCpad);  // non byte aligned CRC
            }
            if(CRChandle->CRCbits > 8 && (CRChandle->CRCbits + CRChandle->CRCpad) != CRCbits2) {
                // CODE: not RefIn, CRCbits > 8
                CRCvnfrintf(&Buffer, &Length, "  CRC &= 0x%ll0*x;\n", ValuesDigits, CRChandle->Polymask);
            }
        }

        if(CRChandle->RefOut ^ CRChandle->RefIn) {
            // CODE: RefOut ^ Refin 
            if(CRChandle->CRCbits <= 8) {
                // CODE: reflect <=8bit
                CRCvnfrintf(&Buffer, &Length, "  CRC = _CRCReflectTable[CRC];\n");
                CRCvnfrintf(&Buffer, &Length, "  CRC >>= %d\n", CRChandle->CRCpad);
            }
            else {
                // CODE: reflect >8bit
                CRCvnfrintf(&Buffer, &Length, "  for(i = 0; i < %d; i++) {\n", CRCbits2 / 8);
                CRCvnfrintf(&Buffer, &Length, "    CRCref = (CRCref << 8) | _CRCReflectTable[CRC & 0xff];\n");
                CRCvnfrintf(&Buffer, &Length, "    CRC >>= 8;\n");
                CRCvnfrintf(&Buffer, &Length, "  }\n");
                CRCvnfrintf(&Buffer, &Length, "  CRC = CRCref >> %d;\n", CRChandle->CRCpad);
            }
        }
        if(CRChandle->XOrOut) {
            // CODE: XOrOut !=0 
            CRCvnfrintf(&Buffer, &Length, "  CRC ^= 0x%ll0*x;\n", ValuesDigits, CRChandle->XOrOut);
        }

        // CODE: end of function
        CRCvnfrintf(&Buffer, &Length, "  return(CRC);\n");
        CRCvnfrintf(&Buffer, &Length, "}\n");

        if(Pass == 0) {
            _Buffer = Buffer = malloc(Length);
        }
    }
    return(_Buffer);
}
