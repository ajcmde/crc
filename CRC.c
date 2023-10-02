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

#include "CRC.h"

/**
 * @brief Helper function which reflects the bit order of a number
 *
 * @param number number 
 * @param bits number of bits to be reflected 
 * 
 * @result number with reflected bits 
 */
static CRC_t CRCReflect(CRC_t number, size_t bits)
{
        //Bit Twiddling Hacks (stanford.edu): http://graphics.stanford.edu/~seander/bithacks.html#BitReverseTable
        size_t s;
        CRC_t result;

        result = number;
        s = sizeof(number) * 8 - 1;
        for (number >>= 1; number; number >>= 1)
        {
            result <<= 1;
            result |= number & 1;
            s--;
        }
        result <<= s; // shift when v's highest bits are zero
        result >>= sizeof(number) * 8 - bits; // align to CRC bits
        return(result);
}


CRChandle_t *CRCCreate(uint8_t CRCbits, CRC_t Polynom, CRC_t Init, bool RefIn, bool RefOut, CRC_t XOrOut)
{
    CRChandle_t *CRChandle;
    CRC_t CRC;
    CRC_t Polymsb;
    int i, j;

    if(CRCbits == 0 || CRCbits > sizeof(CRC_t) * 8)
        return(NULL);
    CRChandle = calloc(sizeof(CRChandle_t), 1);
    if(!CRChandle)
        return(NULL);
    CRChandle->CRCbits = CRCbits;
    CRChandle->Polymask = (((CRC_t)1) << CRCbits) - 1;
    Polymsb = (((CRC_t)1) << (CRCbits - 1));
    CRChandle->Polynom = Polynom;
    CRChandle->Init = Init & CRChandle->Polymask;
    CRChandle->RefIn = RefIn;
    CRChandle->RefOut = RefOut;
    CRChandle->XOrOut = XOrOut & CRChandle->Polymask;

    // initialize table
    CRC = Polymsb;
    CRChandle->Polytable[0] = 0;
    for(i = 1; i < 256; i <<= 1)
    {
        if(CRC & Polymsb)
            CRC = (CRC << 1) ^ Polynom;
        else
            CRC <<= 1;
        CRC &= CRChandle->Polymask;
        for(j = 0; j < i; j++)
            CRChandle->Polytable[(CRChandle->RefIn) ? CRCReflect(i + j, 8) : (i + j)] = 
                (CRC) ^ CRChandle->Polytable[(CRChandle->RefIn) ? CRCReflect(j, 8) : j];
    }    
    if(CRChandle->RefIn)
        for(i = 1; i < 256; i++)
            CRChandle->Polytable[i] = CRCReflect(CRChandle->Polytable[i], CRCbits);

    return(CRChandle);
}


CRC_t CRC(CRChandle_t *CRChandle, uint8_t *Buffer, size_t Length)
{
    CRC_t CRC;
    size_t i;

    if(!CRChandle)
        return(0);

    CRC = CRChandle->Init; 

    if(CRChandle->RefIn)
        for(i = 0; i < Length; i++) 
            CRC = CRChandle->Polytable[(Buffer[i] ^ CRC) & 0xff] ^ (CRC >> 8);
    else
    {
        int16_t CRCbits_8;

        CRCbits_8 = (CRChandle->CRCbits - 8);
        if(CRCbits_8 < 0)
            CRCbits_8 = 0;
        for(i = 0; i < Length; i++) 
            CRC = CRChandle->Polytable[((CRC >> CRCbits_8) & 0xff) ^ Buffer[i]] ^ (CRC << 8);
        CRC &= CRChandle->Polymask;
    }    

    if(CRChandle->RefOut ^ CRChandle->RefIn)
        CRC = CRCReflect(CRC, CRChandle->CRCbits);
    CRC ^= CRChandle->XOrOut;

    return(CRC);
}


int	vsnprintf (char *__restrict, size_t, const char *__restrict, __VALIST)
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


char *CRCCreateCCode(CRChandle_t *CRChandle)
{
    size_t Length;
    char *_Buffer, *Buffer;
    char *Sep1, *Sep2, *Sep3;
    int ValuesRow;

    if(!CRChandle)
        return(NULL);
        
    Buffer = NULL;
    Length = 1;
    
    for(int Pass = 0; Pass < 2; Pass++)
    { 
        CRCvnfrintf(&Buffer, &Length, "CRChandle_t CRChandle_%llx = {\n", (uint64_t)CRChandle->Polynom);
        CRCvnfrintf(&Buffer, &Length, "  .CRCbits = %u,\n", CRChandle->CRCbits);
        CRCvnfrintf(&Buffer, &Length, "  .Polynom = 0x%llx,\n", (uint64_t)CRChandle->Polynom);
        CRCvnfrintf(&Buffer, &Length, "  .Init = 0x%llx,\n", (uint64_t)CRChandle->Init);
        CRCvnfrintf(&Buffer, &Length, "  .XOrOut = 0x%llx,\n", (uint64_t)CRChandle->XOrOut);
        CRCvnfrintf(&Buffer, &Length, "  .RefIn = %s,\n", (CRChandle->RefIn) ? "true": "false");
        CRCvnfrintf(&Buffer, &Length, "  .RefOut = %s,\n", (CRChandle->RefOut) ? "true": "false");
        CRCvnfrintf(&Buffer, &Length, "  .Polymask = 0x%llx,\n", (uint64_t)CRChandle->Polymask);
        CRCvnfrintf(&Buffer, &Length, "  .Polytable = {\n");

        ValuesRow = CRC_CREATECODE_LINELENGTH / ((CRChandle->CRCbits >> 2) + 4); 

        for(int i = 0; i < 256; i++)
        {
            Sep1 = (i % ValuesRow  == 0) ? "   " : "";
            Sep2 = (i != 255) ? "," : "";
            Sep3 = (i % ValuesRow  == ValuesRow - 1 || i == 255) ? "\n" : "";
            CRCvnfrintf(&Buffer, &Length,"%s 0x%ll0*x%s%s", Sep1, CRChandle->CRCbits >> 2, (uint64_t)CRChandle->Polytable[i], Sep2, Sep3);
        }           
        CRCvnfrintf(&Buffer, &Length, "  }");
        CRCvnfrintf(&Buffer, &Length, "};");

        if(Pass == 0)
            _Buffer = Buffer = malloc(Length);
    }
    return(_Buffer);
}
