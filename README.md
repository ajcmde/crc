# CRC
# tags 
#ESP32, #CRC, #CRC-8, #CRC-16, #CRC-32, #CRC-64, #SML, #CRC-12, #CRC-7

# description
provides functions to calculate a crc (1-64bit)

# notes
tested on ESP32 w/ multiple polynoms from 7 bit to 64 bit

# language
C

# usage
  To calculate a CRC you need to have an initialized CRC handle (`CRChandle_t`). The handle can be initialized statically (see example 2). How to create the code 
  please refer to example 3. Static initialization will save the time of calculating the CRC table which may take at least 3900 calculations which may important to 
  your application. To create a CRC handle from a polynom pass it along with the calculation options to the `CRCCreate`. If the CRC handle is not longer used, 
  the handle can be freed by `DestroyCRC`, if the handle was not allocated statically.   
  
  1. a. initialize CRC by providig necessary details:  
  `CRChandle_t *CRCCreate(uint8_t CRCbits, CRC_t Polynom, CRC_t Init, bool RefIn, bool RefOut, CRC_t XOrOut)`   
    `CRCbits`: Number of bits for CRC value. Supported values are 1 to 64.  
    `Polynom`: CRC polynom  
    `Init`: CRC initial value  
    `RefIn`: reflect bit order of input stream (true/ false)  
    `RefOut`: reflect CRC value (true/ false)  
    `XOrOut`: Xor CRC value  
  > Function will fail, if number of crcbits are not supported or system does not provided enough memory to hold the CRChandle_t
  
  1. b. initialize CRC by specifing a name (see source code for valid names ;-) ):  
  `CRChandle_t *CRCCreateFromName(char *CRCname)`   
    `CRCname`: name of CRC    
  > Function will fail, if number of crcbits are not supported, name is not known or system does not provided enough memory to hold the CRChandle_t

  2. Calculate CRC:  
  `CRC_t CRC(CRChandle_t *CRChandle, uint8_t *Buffer, size_t Length)`  
    `CRChandle`: handle of CRC  
    `Buffer`: data  
    `Length`: length of data
  
  3. Dump CRC as C code to initialize a static CRC handle while compiling:  
    `char *CRCdump(CRChandle_t *CRChandle)`  
    `CRChandle`: handle of CRC  
  > Function will return an allocated string.

  4. Destroy a non static CRC handle  
    `void CRCDestroy(CRChandle_t *CRChandle)`  
    `CRChandle`: handle of CRC

# example 1
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

    ...
    
    CRC_t crc16;
    // calculate CRC
    crc16 = CRC(CRC_CCITT16, buf, len);   

# example 2
    #include "CRC.h"

    //    16bits 
    //    polynom is x^{16} + x^{12} + x^5 + 1
    //    CRC initial value: 0xffff
    //    reflect input and output
    //    toggle all output bits
    static CRChandle_t CRC_CCITT16 = {
      .CRCbits = 16,
      .CRCpad = 0,
      .Polynom = 0x1021,
      .Init = 0xffff,
      .XOrOut = 0xffff,
      .RefIn = true,
      .RefOut = true,
      .Polymask = 0xffff,
      .Polytable = {
        0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf, 0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
        0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e, 0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
        0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd, 0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
        0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c, 0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
        0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb, 0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
        0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a, 0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
        0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9, 0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
        0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738, 0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
        0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7, 0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
        0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036, 0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
        0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5, 0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
        0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134, 0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
        0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3, 0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
        0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232, 0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
        0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1, 0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
        0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330, 0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78
      }};

    ...
    CRC_t crc16;
    
    // calculate CRC
    crc16 = CRC(&CRC_CCITT16, buf, len);    

# example 3
    #include "CRC.h"
    
    ...
            
    CRChandle_t *CRC_CCITT16;
    char *ccode;
    
    // create CRC table for smart message protocol (SML): 
    //    16bits 
    //    polynom is x^{16} + x^{12} + x^5 + 1
    //    initial value: 0xffff
    //    reverse input and output
    //    toggle all output bits
    CRC_CCITT16 = CRCCreate(16, 0x1021, 0xffff, true, true, 0xffff);
    
    // dump the CRC
    ccode = CRCdump(CRC_CCITT16);
    ESP_LOGI("CRC", "%s", ccode);
    
    // cleanup
    free(ccode);
    CRCDestroy(CRC_CCITT16);

# dependency
  https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html

# reference
  http://graphics.stanford.edu/~seander/bithacks.html#BitReverseTable
  
  https://en.wikipedia.org/wiki/Cyclic_redundancy_check

  https://ceng2.ktu.edu.tr/~cevhers/ders_materyal/bil311_bilgisayar_mimarisi/supplementary_docs/crc_algorithms.pdf
