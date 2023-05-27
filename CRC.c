
#include "CRC.h"
#include "esp_log.h"



__attribute__((unused)) static const char *LOG_TAG = "CRC";  

static uint64_t CRCreverse(uint64_t number, size_t bits)
{
        //Bit Twiddling Hacks (stanford.edu): http://graphics.stanford.edu/~seander/bithacks.html#BitReverseTable
        size_t s;
        uint64_t result;

        result = number;
        s = bits * 8 - 1; // extra shift needed at end
        for (number >>= 1; number; number >>= 1)
        {
            result <<= 1;
            result |= number & 1;
            s--;
        }
        result <<= s; // shift when v's highest bits are zero
        result >>= 64 - bits; // align to CRC bits
        return(result);
}

CRChandle_t *CRCCreate(uint8_t crcbits, uint64_t polynom, uint64_t init, bool RefIn, bool RefOut, uint64_t XOrOut)
{
    CRChandle_t *CRCh;
    uint64_t CRC;
    int i, j;

    if(crcbits != 64 && crcbits != 32 && crcbits != 16 && crcbits != 8)
        return(NULL);
    CRCh = malloc(sizeof(CRChandle_t) - sizeof(CRCh->polytable) + crcbits * 256 / 8);
    if(!CRCh)
        return(NULL);
    CRCh->crcbits = crcbits;
    CRCh->polynom = polynom;
    CRCh->polymask = (1 << crcbits) - 1;
    CRCh->init = init;
    CRCh->polyMSBit = (1 << (crcbits - 1));
    CRCh->RefIn = RefIn;
    CRCh->RefOut = RefOut;
    CRCh->XOrOut = XOrOut;

    CRCh->polytable.ui64[0] = 0;

    // initialize table

    CRC = CRCh->polyMSBit;
    for(i = 1; i < 256; i <<= 1)
    {
        if(CRC & CRCh->polyMSBit)
            CRC = (CRC << 1) ^ CRCh->polynom;
        else
            CRC <<= 1;
        CRC &= CRCh->polymask;
        switch(crcbits)
        {
            case 8:
                for(j = 0; j < i; j++)
                    CRCh->polytable.ui8[(CRCh->RefIn) ? CRCreverse(i + j, 8) : (i + j)] = (CRC) ^ CRCh->polytable.ui8[(CRCh->RefIn) ? CRCreverse(j, 8) : j];
                break;
            case 16:
                for(j = 0; j < i; j++)
                    CRCh->polytable.ui16[(CRCh->RefIn) ? CRCreverse(i + j, 8) : (i + j)] = (CRC) ^ CRCh->polytable.ui16[(CRCh->RefIn) ? CRCreverse(j, 8) : j];
                break;
            case 32:
                for(j = 0; j < i; j++)
                    CRCh->polytable.ui32[(CRCh->RefIn) ? CRCreverse(i + j, 8) : (i + j)] = (CRC) ^ CRCh->polytable.ui32[(CRCh->RefIn) ? CRCreverse(j, 8) : j];
                break;
            case 64:
                for(j = 0; j < i; j++)
                    CRCh->polytable.ui64[(CRCh->RefIn) ? CRCreverse(i + j, 8) : (i + j)] = (CRC) ^ CRCh->polytable.ui64[(CRCh->RefIn) ? CRCreverse(j, 8) : j];
                break;
        }
    }
    
    if(CRCh->RefIn)
    {
        switch(crcbits)
        {
            case 8:
                for(i = 1; i < 256; i++)
                    CRCh->polytable.ui8[i] = CRCreverse(CRCh->polytable.ui8[i], CRCh->crcbits);
                break;
            case 16:
                for(i = 1; i < 256; i++)
                    CRCh->polytable.ui16[i] = CRCreverse(CRCh->polytable.ui16[i], CRCh->crcbits);
                break;
            case 32:
                for(i = 1; i < 256; i++)
                    CRCh->polytable.ui32[i] = CRCreverse(CRCh->polytable.ui32[i], CRCh->crcbits);
                break;
            case 64:
                for(i = 1; i < 256; i++)
                    CRCh->polytable.ui64[i] = CRCreverse(CRCh->polytable.ui64[i], CRCh->crcbits);
                break;
        }
    }

    // ESP_LOG_BUFFER_HEXDUMP(LOG_TAG, CRCh->polytable.ui8, sizeof(CRCh->polytable) + crcbits * 256 / 8, ESP_LOG_INFO); 
    return(CRCh);
 }

uint64_t CRC(CRChandle_t *CRChandle, uint8_t *buffer, size_t length)
{
    uint64_t CRC;
    int i; 
    
    if(!CRChandle)
        return(0);
    CRC = CRChandle->init; 
    switch(CRChandle->crcbits)
    {
        case 8:
            for(i = 0; i < length; i++) 
                CRC = CRChandle->polytable.ui8[(buffer[i] ^ CRC) & 0xff] ^ (CRC >> 8 & 0xff);
            break;
        case 16:
            for(i = 0; i < length; i++) 
                CRC = CRChandle->polytable.ui16[(buffer[i] ^ CRC) & 0xff] ^ (CRC >> 8 & 0xff);
            break;
        case 32:
            for(i = 0; i < length; i++) 
                CRC = CRChandle->polytable.ui32[(buffer[i] ^ CRC) & 0xff] ^ (CRC >> 8 & 0xff);
            break;
        case 64:
            for(i = 0; i < length; i++) 
                CRC = CRChandle->polytable.ui64[(buffer[i] ^ CRC) & 0xff] ^ (CRC >> 8 & 0xff);
            break;
    }
    if(!CRChandle->RefOut)
        CRC = CRCreverse(CRC, CRChandle->crcbits);
    CRC = CRC ^ CRChandle->XOrOut;

    return(CRC);
}

