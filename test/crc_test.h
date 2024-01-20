#include "crc.h"

#if defined(CONFIG_IDF_TARGET_ESP32)
/**
 * @brief Run Test CRC tests. Fail/ pass results will be logged on console
 *
 */
extern void _CRCTest(void);
#endif
