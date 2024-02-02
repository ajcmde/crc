#if defined(CONFIG_IDF_TARGET_ESP32)

#include "esp_log.h"

typedef struct 
{
    char *Name;
    uint8_t *Pattern;
    uint16_t Length; // length of pattern
} _CRCtestpattern_t;

typedef struct
{
    char *Name; // CRC name
    _CRCtestpattern_t *TestPattern;
    CRC_TYPE crc; // expected result
} _CRCtest_t;


void _CRCTest(void)
{
    const char *_CRCtest_TAG = "_CRCtest";

    uint8_t _CRCpattern0[] = {0xf};
    uint8_t _CRCpattern1[] = {0x43, 0x52, 0x43};
    uint8_t _CRCpattern2[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
    uint8_t _CRCpattern3[] = {0x00, 0x01, 0x00, 0x02, 0x00, 0x04, 0x00, 0x08, 0x00, 0x10, 0x00, 0x20, 0x00, 0x40, 0x00, 0x80, 0x01, 0x00, 0x02, 0x00, 0x04, 0x00, 0x08, 0x00, 0x10, 0x00, 0x20, 0x00, 0x40, 0x00, 0x80, 0x00};
    uint8_t _CRCpattern4[] = {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01};
    uint8_t _CRCpattern5[] = {0xff};
    uint8_t _CRCpattern6[] = {0xff, 0xff};
    uint8_t _CRCpattern7[] = {0x43, 0x52, 0x43, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
    uint8_t _CRCpattern8[] = {0x33, 0x22, 0x55, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };
    _CRCtestpattern_t CRCtestpattern0 = {"0xf", _CRCpattern0, sizeof(_CRCpattern0)};
    _CRCtestpattern_t CRCtestpattern1 = {"\"CRC\"", _CRCpattern1, sizeof(_CRCpattern1)};
    _CRCtestpattern_t CRCtestpattern2 = {"singlebits(8)", _CRCpattern2, sizeof(_CRCpattern2)};
    _CRCtestpattern_t CRCtestpattern3 = {"singlebits(16)", _CRCpattern3, sizeof(_CRCpattern3)};
    _CRCtestpattern_t CRCtestpattern4 = {"bit1(8)", _CRCpattern4, sizeof(_CRCpattern4)};
    _CRCtestpattern_t CRCtestpattern5 = {"0xff", _CRCpattern5, sizeof(_CRCpattern5)};
    _CRCtestpattern_t CRCtestpattern6 = {"0xffff", _CRCpattern6, sizeof(_CRCpattern6)};
    _CRCtestpattern_t CRCtestpattern7 = {"CRC0123456789", _CRCpattern7, sizeof(_CRCpattern7)};
    _CRCtestpattern_t CRCtestpattern8 = {"AUTOSAR-05", _CRCpattern8, sizeof(_CRCpattern8)};
    const _CRCtest_t CRCtests[] =
    {
        {"CRC-8/I-432-1", &CRCtestpattern1, 0x86},
        {"CRC-8/CDMA2000", &CRCtestpattern1, 0x92},
        {"CRC-8/CDMA2000", &CRCtestpattern3, 0x56},
        {"CRC-8/DARC", &CRCtestpattern1, 0xf5},
        {"CRC-8/DARC", &CRCtestpattern2, 0x8a},
        {"CRC-8/DARC", &CRCtestpattern3, 0x0},
        {"CRC-8/DARC", &CRCtestpattern4, 0x0},
        {"CRC-12/UMTS", &CRCtestpattern1, 0x55d},
        {"CRC-7/UMTS", &CRCtestpattern1, 0x75},
        {"CRC-7/ROHC", &CRCtestpattern1, 0x17},
        {"CRC-7/ROHC", &CRCtestpattern2, 0x32},
        {"CRC-7/UMTS", &CRCtestpattern0, 0x22},
        {"CRC-7/UMTS", &CRCtestpattern5, 0x59},
        {"CRC-7/UMTS", &CRCtestpattern6, 0x24},
        {"CRC-8/SAE-J1850", &CRCtestpattern8, 0xcb}, 
        {"CRC-8/AUTOSAR", &CRCtestpattern8, 0x11},
#if defined(CONFIG_CRC_CRC16) || defined(CONFIG_CRC_CRC32) || defined(CONFIG_CRC_CRC64)
        {"CRC-10/ATM", &CRCtestpattern2, 0x15d},
        {"CRC-16/CDMA2000", &CRCtestpattern2, 0xa3ee},
        {"CRC-16/IBM-3740", &CRCtestpattern8, 0xf53f},        
        {"CRC-16/ARC", &CRCtestpattern8, 0xae98},
#endif
#if defined(CONFIG_CRC_CRC32) || defined(CONFIG_CRC_CRC64)
        {"CRC-24/OPENPGP", &CRCtestpattern7, 0x5639ee},
        {"CRC-32/ISCSI", &CRCtestpattern7, 0xfa1d936a},
        {"CRC-32/ISO-HDLC", &CRCtestpattern8, 0xb0ae863d}, 
        {"CRC-32/AUTOSAR", &CRCtestpattern8, 0xa65a343d},
#endif
#if defined(CONFIG_CRC_CRC64)
        {"CRC-40/GSM", &CRCtestpattern7, 0x19eb2415ca},
        {"CRC-64/WE", &CRCtestpattern7, 0xb072fe3926575027},
        {"CRC-64/XZ", &CRCtestpattern7, 0x319fa647bf14a7c2},
        {"CRC-64/XZ", &CRCtestpattern8, 0x701ECEB219A8E5D5},
#endif
        { /* end of table */ }
    };


    CRChandle_t *CRChandle = NULL;
    const _CRCtest_t *CRCtest;
    CRC_TYPE crc; 

    for(CRCtest = CRCtests; CRCtest->Name; CRCtest++) {
        CRChandle = CRCCreateFromName(CRCtest->Name);
        if(!CRChandle) {
            ESP_LOGE(_CRCtest_TAG, "unknown CRC %s", CRCtest->Name);
            continue;
        }
        crc =  CRC(CRChandle, CRCtest->TestPattern->Pattern, CRCtest->TestPattern->Length);
        if(crc == CRCtest->crc)
            ESP_LOGI(_CRCtest_TAG, "%s[%s] passed", CRCtest->Name, CRCtest->TestPattern->Name);
        else
            ESP_LOGE(_CRCtest_TAG, "%s[%s] failed: expected 0x%llx, calculated 0x%llx", CRCtest->Name, CRCtest->TestPattern->Name, CRCtest->crc, crc);

        CRCDestroy(CRChandle);        
    }

}


#endif
