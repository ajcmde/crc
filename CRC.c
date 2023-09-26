
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
    uint64_t polymask;
    uint64_t polymsb;

    if(crcbits != 64 && crcbits != 32 && crcbits != 16 && crcbits != 8)
        return(NULL);
    CRCh = malloc(sizeof(CRChandle_t));
    if(!CRCh)
        return(NULL);
    CRCh->crcbits = crcbits;
    CRCh->polynom = polynom;
    polymask = (1 << crcbits) - 1;
    CRCh->init = init;
    polymsb = (1 << (crcbits - 1));
    CRCh->RefIn = RefIn;
    CRCh->RefOut = RefOut;
    CRCh->XOrOut = XOrOut;
    CRCh->polytable[0] = 0;

    // initialize table

    CRC = polymsb;
    for(i = 1; i < 256; i <<= 1)
    {
        if(CRC & polymsb)
            CRC = (CRC << 1) ^ CRCh->polynom;
        else
            CRC <<= 1;
        CRC &= polymask;
        for(j = 0; j < i; j++)
            CRCh->polytable[(CRCh->RefIn) ? CRCreverse(i + j, 8) : (i + j)] = (CRC) ^ CRCh->polytable[(CRCh->RefIn) ? CRCreverse(j, 8) : j];
    }
    
    if(CRCh->RefIn)
    {
        for(i = 1; i < 256; i++)
            CRCh->polytable[i] = CRCreverse(CRCh->polytable[i], CRCh->crcbits);
    }

    return(CRCh);
 }

// https://ceng2.ktu.edu.tr/~cevhers/ders_materyal/bil311_bilgisayar_mimarisi/supplementary_docs/crc_algorithms.pdf

uint64_t CRC(CRChandle_t *CRChandle, uint8_t *buffer, size_t length)
{
    uint64_t CRC;
    int i; 
    
    if(!CRChandle)
        return(0);
    CRC = CRChandle->init; 
    for(i = 0; i < length; i++) 
        CRC = CRChandle->polytable[(buffer[i] ^ CRC) & 0xff] ^ (CRC >> 8 & 0xff);

    if(!CRChandle->RefOut)
        CRC = CRCreverse(CRC, CRChandle->crcbits);
    CRC = CRC ^ CRChandle->XOrOut;

    return(CRC);
}



int	vsnprintf (char *__restrict, size_t, const char *__restrict, __VALIST)
               _ATTRIBUTE ((__format__ (__printf__, 3, 0)));


int CRCvnfrintf(char **buffer, size_t *n, const char *fmt, ...)
{
    char temp[1];
    char *_buffer;
    size_t _n;

    if(!buffer)
        return(-1);
    _buffer = *buffer;
    if(!_buffer)
    {
        _n = 1;
        _buffer = temp; 
    }
    else 
        _n = *n;
    va_list list;
    va_start(list, fmt);
    int result = vsnprintf(_buffer, _n, fmt, list);
    va_end(list);

    if(result < 0)
        return(0);
    if(_buffer != temp)
    {
        *buffer += result;
        *n -= result;
    }
    else 
        *n += result;
    return(result);
}


char *CRCdump(CRChandle_t *CRChandle)
{
    size_t length;
    char *_buffer, *buffer;
    int pass;
    char *sep1, *sep2, *sep3;
    int valuesperrow;

    if(!CRChandle)
        return(NULL);
        
    buffer = NULL;
    length = 1;
    
    for(pass = 0; pass < 2; pass++)
    {
        if(pass == 0)
            length = 0; 
        CRCvnfrintf(&buffer, &length, "CRChandle_t CRChandle_%llx = {\n", CRChandle->polynom);
        CRCvnfrintf(&buffer, &length, "  .crcbits = %d,\n", CRChandle->crcbits);
        CRCvnfrintf(&buffer, &length, "  .polynom = 0x%llx,\n", CRChandle->polynom);
        CRCvnfrintf(&buffer, &length, "  .init = 0x%llx,\n", CRChandle->init);
        CRCvnfrintf(&buffer, &length, "  .XOrOut = 0x%llx,\n", CRChandle->XOrOut);
        CRCvnfrintf(&buffer, &length, "  .RefIn = %s,\n", (CRChandle->RefIn) ? "true": "false");
        CRCvnfrintf(&buffer, &length, "  .RefOut = %s,\n", (CRChandle->RefOut) ? "true": "false");
        CRCvnfrintf(&buffer, &length, "  .polytable = {\n");

#ifndef CRCdump_LINELENGTH
#define CRCdump_LINELENGTH 128
#endif
        valuesperrow = CRCdump_LINELENGTH / ((CRChandle->crcbits >> 2) + 4); 

        for(int i = 0; i < 256; i++)
        {
            sep1 = (i % valuesperrow  == 0) ? "   " : "";
            sep2 = (i != 255) ? "," : "";
            sep3 = (i % valuesperrow  == valuesperrow - 1 || i == 255) ? "\n" : "";
            CRCvnfrintf(&buffer, &length,"%s 0x%ll0*x%s%s", sep1, CRChandle->crcbits >> 2, CRChandle->polytable[i], sep2, sep3);
        }           
        CRCvnfrintf(&buffer, &length, "  }");
        CRCvnfrintf(&buffer, &length, "};");

        if(pass == 0)
            _buffer = buffer = malloc(++length);
    }
    return(_buffer);

}
