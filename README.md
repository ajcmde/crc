# CRC
# tags 
#ESP32, #CRC, #CRC-8, #CRC-16, #CRC-32, #CRC-64, #SML, #CRC-12, #CRC-7, #C, #PYTHON

# description
provides functions to calculate a crc (1-64bit)

# note
Description for python implementation needs to be added

# language
C, Python

# usage C
  To calculate a CRC you need to have an initialized CRC handle (`CRChandle_t`). The handle can be initialized statically (see example 2) or dynamically. How to create the code 
  please refer to example 3. Static initialization will save the time of calculating the CRC table which may take at least 3900 calculations which may important to 
  your application. To create a CRC handle from a polynom, pass it along with the CRC options to the `CRCCreate`. If the CRC handle is not longer used, 
  the handle can be freed by `DestroyCRC`, if the handle was not allocated statically.   
  
  1. a. initialize CRC by providig necessary details:  
  `CRChandle_t *CRCCreate(uint8_t CRCbits, CRC_t Polynom, CRC_t Init, bool RefIn, bool RefOut, CRC_t XOrOut)`   
    `CRCbits`: Number of bits for CRC value. Supported values are 1 to 64.  
    `Polynom`: CRC polynom  
    `Init`: CRC initial value  
    `RefIn`: reflect bit order of input stream (true/ false)  
    `RefOut`: reflect CRC value (true/ false)  
    `XOrOut`: Xor CRC value after processing the input 
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
    `char *CRCCreateCCode(CRChandle_t *CRChandle)`  
    `CRChandle`: handle of CRC  
  > Function will allocate memory for the generated source code.

  4. Destroy a non static CRC handle  
    `void CRCDestroy(CRChandle_t *CRChandle)`  
    `CRChandle`: handle of CRC

![image](https://github.com/ajcmde/crc/assets/134808561/02d56d6d-31ff-41e2-a588-7d227bde80eb)

# usage Python
  CRC are initialized by calling the class methods `CRCCreate` or `CRCCreateFromName`. When using the first option, provide the neccessary  
  details when calling the function. With `CRCCreateFromName` you need to pass along the name of a CRC. Unfortunatelly no naming standard  
  exists. 
  
  1. a. initialize CRC_c by providig necessary details:  
  `CRC_c CRCCreate(CRCbits, Polynom, Init, bool RefIn, bool RefOut, XOrOut)`   
    `CRCbits`: Number of bits for CRC value. Supported values are 1 to 64.  
    `Polynom`: CRC polynom  
    `Init`: CRC initial value  
    `RefIn`: reflect bit order of input stream (true/ false)  
    `RefOut`: reflect CRC value (true/ false)  
    `XOrOut`: Xor CRC value after processing the input
  > Function will fail, if number of crcbits are bigger than 64.
  
  1. b. initialize CRC_c by specifing a name (see source code for valid names ;-) ):  
  `CRCCreateFromName(CRCname)`   
    `CRCname`: name of CRC    
  > Function will fail if name is unknown

  2. Calculate CRC:  
  `CRC(Buffer)`  
    `Buffer`: data (bytes)

  3. Dump CRC_c as Python code to get a "static" CRC_c:  
    `char *CRCCreateCCode(CRChandle_t *CRChandle)`  
    `CRChandle`: handle of CRC  

![image](https://github.com/ajcmde/crc/assets/134808561/aef47709-70ec-4929-a2cb-36e250530788)
  
# example 1 (C)
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

# example 2 (C)
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

# example 3 (C)
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

# example 4 (Python)
    import CRC

    ...


    CRC_42f0e1eba9ea3693 = CRC_c(64, 0x42f0e1eba9ea3693, 0xffffffffffffffff, 0xffffffffffffffff, True, True, 0xffffffffffffffff, 0, 
        0x0000000000000000, 0xb32e4cbe03a75f6f, 0xf4843657a840a05b, 0x47aa7ae9abe7ff34, 0x7bd0c384ff8f5e33, 0xc8fe8f3afc28015c,
        0x8f54f5d357cffe68, 0x3c7ab96d5468a107, 0xf7a18709ff1ebc66, 0x448fcbb7fcb9e309, 0x0325b15e575e1c3d, 0xb00bfde054f94352,
        0x8c71448d0091e255, 0x3f5f08330336bd3a, 0x78f572daa8d1420e, 0xcbdb3e64ab761d61, 0x7d9ba13851336649, 0xceb5ed8652943926,
        0x891f976ff973c612, 0x3a31dbd1fad4997d, 0x064b62bcaebc387a, 0xb5652e02ad1b6715, 0xf2cf54eb06fc9821, 0x41e11855055bc74e,
        0x8a3a2631ae2dda2f, 0x39146a8fad8a8540, 0x7ebe1066066d7a74, 0xcd905cd805ca251b, 0xf1eae5b551a2841c, 0x42c4a90b5205db73,
        0x056ed3e2f9e22447, 0xb6409f5cfa457b28, 0xfb374270a266cc92, 0x48190ecea1c193fd, 0x0fb374270a266cc9, 0xbc9d3899098133a6,
        0x80e781f45de992a1, 0x33c9cd4a5e4ecdce, 0x7463b7a3f5a932fa, 0xc74dfb1df60e6d95, 0x0c96c5795d7870f4, 0xbfb889c75edf2f9b,
        0xf812f32ef538d0af, 0x4b3cbf90f69f8fc0, 0x774606fda2f72ec7, 0xc4684a43a15071a8, 0x83c230aa0ab78e9c, 0x30ec7c140910d1f3,
        0x86ace348f355aadb, 0x3582aff6f0f2f5b4, 0x7228d51f5b150a80, 0xc10699a158b255ef, 0xfd7c20cc0cdaf4e8, 0x4e526c720f7dab87,
        0x09f8169ba49a54b3, 0xbad65a25a73d0bdc, 0x710d64410c4b16bd, 0xc22328ff0fec49d2, 0x85895216a40bb6e6, 0x36a71ea8a7ace989,
        0x0adda7c5f3c4488e, 0xb9f3eb7bf06317e1, 0xfe5991925b84e8d5, 0x4d77dd2c5823b7ba, 0x64b62bcaebc387a1, 0xd7986774e864d8ce,
        0x90321d9d438327fa, 0x231c512340247895, 0x1f66e84e144cd992, 0xac48a4f017eb86fd, 0xebe2de19bc0c79c9, 0x58cc92a7bfab26a6,
        0x9317acc314dd3bc7, 0x2039e07d177a64a8, 0x67939a94bc9d9b9c, 0xd4bdd62abf3ac4f3, 0xe8c76f47eb5265f4, 0x5be923f9e8f53a9b,
        0x1c4359104312c5af, 0xaf6d15ae40b59ac0, 0x192d8af2baf0e1e8, 0xaa03c64cb957be87, 0xeda9bca512b041b3, 0x5e87f01b11171edc,
        0x62fd4976457fbfdb, 0xd1d305c846d8e0b4, 0x96797f21ed3f1f80, 0x2557339fee9840ef, 0xee8c0dfb45ee5d8e, 0x5da24145464902e1,
        0x1a083bacedaefdd5, 0xa9267712ee09a2ba, 0x955cce7fba6103bd, 0x267282c1b9c65cd2, 0x61d8f8281221a3e6, 0xd2f6b4961186fc89,
        0x9f8169ba49a54b33, 0x2caf25044a02145c, 0x6b055fede1e5eb68, 0xd82b1353e242b407, 0xe451aa3eb62a1500, 0x577fe680b58d4a6f,
        0x10d59c691e6ab55b, 0xa3fbd0d71dcdea34, 0x6820eeb3b6bbf755, 0xdb0ea20db51ca83a, 0x9ca4d8e41efb570e, 0x2f8a945a1d5c0861,
        0x13f02d374934a966, 0xa0de61894a93f609, 0xe7741b60e174093d, 0x545a57dee2d35652, 0xe21ac88218962d7a, 0x5134843c1b317215,
        0x169efed5b0d68d21, 0xa5b0b26bb371d24e, 0x99ca0b06e7197349, 0x2ae447b8e4be2c26, 0x6d4e3d514f59d312, 0xde6071ef4cfe8c7d,
        0x15bb4f8be788911c, 0xa6950335e42fce73, 0xe13f79dc4fc83147, 0x521135624c6f6e28, 0x6e6b8c0f1807cf2f, 0xdd45c0b11ba09040,
        0x9aefba58b0476f74, 0x29c1f6e6b3e0301b, 0xc96c5795d7870f42, 0x7a421b2bd420502d, 0x3de861c27fc7af19, 0x8ec62d7c7c60f076,
        0xb2bc941128085171, 0x0192d8af2baf0e1e, 0x4638a2468048f12a, 0xf516eef883efae45, 0x3ecdd09c2899b324, 0x8de39c222b3eec4b,
        0xca49e6cb80d9137f, 0x7967aa75837e4c10, 0x451d1318d716ed17, 0xf6335fa6d4b1b278, 0xb199254f7f564d4c, 0x02b769f17cf11223,
        0xb4f7f6ad86b4690b, 0x07d9ba1385133664, 0x4073c0fa2ef4c950, 0xf35d8c442d53963f, 0xcf273529793b3738, 0x7c0979977a9c6857,
        0x3ba3037ed17b9763, 0x888d4fc0d2dcc80c, 0x435671a479aad56d, 0xf0783d1a7a0d8a02, 0xb7d247f3d1ea7536, 0x04fc0b4dd24d2a59,
        0x3886b22086258b5e, 0x8ba8fe9e8582d431, 0xcc0284772e652b05, 0x7f2cc8c92dc2746a, 0x325b15e575e1c3d0, 0x8175595b76469cbf,
        0xc6df23b2dda1638b, 0x75f16f0cde063ce4, 0x498bd6618a6e9de3, 0xfaa59adf89c9c28c, 0xbd0fe036222e3db8, 0x0e21ac88218962d7,
        0xc5fa92ec8aff7fb6, 0x76d4de52895820d9, 0x317ea4bb22bfdfed, 0x8250e80521188082, 0xbe2a516875702185, 0x0d041dd676d77eea,
        0x4aae673fdd3081de, 0xf9802b81de97deb1, 0x4fc0b4dd24d2a599, 0xfceef8632775faf6, 0xbb44828a8c9205c2, 0x086ace348f355aad,
        0x34107759db5dfbaa, 0x873e3be7d8faa4c5, 0xc094410e731d5bf1, 0x73ba0db070ba049e, 0xb86133d4dbcc19ff, 0x0b4f7f6ad86b4690,
        0x4ce50583738cb9a4, 0xffcb493d702be6cb, 0xc3b1f050244347cc, 0x709fbcee27e418a3, 0x3735c6078c03e797, 0x841b8ab98fa4b8f8,
        0xadda7c5f3c4488e3, 0x1ef430e13fe3d78c, 0x595e4a08940428b8, 0xea7006b697a377d7, 0xd60abfdbc3cbd6d0, 0x6524f365c06c89bf,
        0x228e898c6b8b768b, 0x91a0c532682c29e4, 0x5a7bfb56c35a3485, 0xe955b7e8c0fd6bea, 0xaeffcd016b1a94de, 0x1dd181bf68bdcbb1,
        0x21ab38d23cd56ab6, 0x9285746c3f7235d9, 0xd52f0e859495caed, 0x6601423b97329582, 0xd041dd676d77eeaa, 0x636f91d96ed0b1c5,
        0x24c5eb30c5374ef1, 0x97eba78ec690119e, 0xab911ee392f8b099, 0x18bf525d915feff6, 0x5f1528b43ab810c2, 0xec3b640a391f4fad,
        0x27e05a6e926952cc, 0x94ce16d091ce0da3, 0xd3646c393a29f297, 0x604a2087398eadf8, 0x5c3099ea6de60cff, 0xef1ed5546e415390,
        0xa8b4afbdc5a6aca4, 0x1b9ae303c601f3cb, 0x56ed3e2f9e224471, 0xe5c372919d851b1e, 0xa26908783662e42a, 0x114744c635c5bb45,
        0x2d3dfdab61ad1a42, 0x9e13b115620a452d, 0xd9b9cbfcc9edba19, 0x6a978742ca4ae576, 0xa14cb926613cf817, 0x1262f598629ba778,
        0x55c88f71c97c584c, 0xe6e6c3cfcadb0723, 0xda9c7aa29eb3a624, 0x69b2361c9d14f94b, 0x2e184cf536f3067f, 0x9d36004b35545910,
        0x2b769f17cf112238, 0x9858d3a9ccb67d57, 0xdff2a94067518263, 0x6cdce5fe64f6dd0c, 0x50a65c93309e7c0b, 0xe388102d33392364,
        0xa4226ac498dedc50, 0x170c267a9b79833f, 0xdcd7181e300f9e5e, 0x6ff954a033a8c131, 0x28532e49984f3e05, 0x9b7d62f79be8616a,
        0xa707db9acf80c06d, 0x14299724cc279f02, 0x5383edcd67c06036, 0xe0ada17364673f59
        ])
    print('CRC_42f0e1eba9ea3693.CRC(Hello World!): 0x{:x}'.format(CRC_42f0e1eba9ea3693.CRC(b"Hello World!")))

# dependency C
  https://docs.espressif.com/projects/esp-idf/en/latest/esp32/get-started/index.html 

# dependency Python
  Python version >= 3

# reference
  http://graphics.stanford.edu/~seander/bithacks.html#BitReverseTable
  
  https://en.wikipedia.org/wiki/Cyclic_redundancy_check

  https://ceng2.ktu.edu.tr/~cevhers/ders_materyal/bil311_bilgisayar_mimarisi/supplementary_docs/crc_algorithms.pdf

  https://reveng.sourceforge.io/crc-catalogue/all.htm
