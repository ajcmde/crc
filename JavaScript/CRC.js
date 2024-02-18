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
class CRC_c << C, #FF7700 >> {
  -int CRCbits
  -int CRCpad
  -int Polynom
  -int Init
  -int XOrOut 
  -bool RefIn
  -bool RefOut
  -int Polymask 
  -int Polytable[256]
  +ReflectTable[]
--
  +constructor()
..
  +str CRCCreateCCode2()
..
  -int CRCReflect()
}
hide empty members
@enduml
*/



class CRC_c {

    ReflectTable = [
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
    ];

    #CRCReflect(number, bits, bitspad) {
//alert(bitspad);
        let result = BigInt(0);
        let bytes = (bits + bitspad) >> 3;
        for (let i = 0; i < bytes; i++) {
            result = (result << BigInt(8)) | BigInt(this.ReflectTable[number & BigInt(0xff)]);
            number >>= BigInt(8);
        }

        result >>= BigInt(bitspad); // align to CRC bits
        return result;
    }

    constructor (CRCbits, Polynom, Init, RefIn, RefOut, XOrOut) 
    {
        let CRChandle = {};
        let CRC;
        let Polymsb, Polymask;
        let i, j;

        if (CRCbits === 0 || CRCbits > 64) {
            return null;
        }

        Polynom = BigInt(Polynom);
        Init =  BigInt(Init);
        XOrOut = BigInt(XOrOut);

        this.CRCbits = CRCbits;

        Polymask = this.Polymask = (BigInt(1) << BigInt(CRCbits)) - BigInt(1);
        Polymsb = BigInt(1) << BigInt(CRCbits - 1);
        this.Polynom = BigInt(Polynom);
        this.Init = BigInt(Init) & this.Polymask;
        this.RefIn = RefIn;
        this.RefOut = RefOut;
        this.XOrOut = BigInt(XOrOut) & this.Polymask;
        this.CRCpad = (8 - (this.CRCbits % 8)) % 8; // non byte-aligned CRC

        if (!this.RefIn) {
            Polynom <<= BigInt(this.CRCpad);
            Polymsb <<= BigInt(this.CRCpad);
            Polymask <<= BigInt(this.CRCpad);
        }

        // Initialize table
        CRC = Polymsb;
        this.Polytable = null;
        this.Polytable = [BigInt(0)]; // Reminder of 0 is 0

        for (i = 1; i < 256; i <<= 1) {
            if (CRC & Polymsb) {
                CRC = (CRC << BigInt(1)) ^ Polynom;
            } else {
                CRC <<= BigInt(1);
            }
            CRC &= Polymask;
            for (j = 0; j < i; j++) {
                this.Polytable[this.RefIn ? this.ReflectTable[i + j] : (i + j)] =
                    CRC ^ this.Polytable[this.RefIn ? this.ReflectTable[j] : j];
            }
        }
 
        if (this.RefIn) {
            for (i = 1; i < 256; i++) {
                this.Polytable[i] = this.#CRCReflect(this.Polytable[i], CRCbits, this.CRCpad);
            }
        }
    }

    CRCCreateCCode2() {
        let CRCbits_8 = (this.CRCbits - 1) & (~(7)); // CRC bits aligned to the next byte border (12 -> 8, 32 -> 24)
        let CRCbits2 = (this.CRCbits <= 8) ? 8 : (this.CRCbits <= 16) ? 16 : (this.CRCbits <= 32) ? 32 : (this.CRCbits <= 64) ? 64 : -1;

        let Buffer = "";
        let ValuesDigits = CRCbits2 >> 2;
        const CRC_CREATECODE_LINELENGTH = 120;
        let ValuesRow = 0; 

        // CODE: header
        Buffer += `extern uint${CRCbits2}_t CRC(const uint8_t *Buffer, size_t Length);\n\n`;
        // CODE: include
        Buffer += "#include <stdint.h>\n\n";
        if (this.RefOut !== this.RefIn) {
            // CODE: supporting data for reflecting
            ValuesRow = CRC_CREATECODE_LINELENGTH / (2 + 4); 
            Buffer += "static const uint8_t ReflectTable[] = {\n";
            for (let i = 0; i < 256; i++) {
                let Sep1 = (i % ValuesRow === 0) ? "  " : "";
                let Sep2 = (i !== 255) ? "," : "";
                let Sep3 = (i % ValuesRow === ValuesRow - 1 || i === 255) ? "\n" : "";
                Buffer += `${Sep1} 0x${this.ReflectTable[i].toString(16).padStart(2, '0')}${Sep2}${Sep3}`;
            }
            Buffer += "};\n\n";
        }
        // CODE: function header
        Buffer += "__attribute__((no_sanitize(\"shift\")))\n";
        Buffer += `uint${CRCbits2}_t CRC(const uint8_t *Buffer, size_t Length)\n`;
        Buffer += "{\n";
        // CODE: description and variables
        Buffer += `  // CRCbits: ${this.CRCbits}, Polynom: 0x${this.Polynom.toString(16).padStart(ValuesDigits, '0')}, `;
        Buffer += `Init: 0x${this.Init.toString(16).padStart(ValuesDigits, '0')}, `;
        Buffer += `RefIn: ${this.RefIn}, RefOut: ${this.RefOut}, `;
        Buffer += `XOrOut: 0x${this.XOrOut.toString(16).padStart(ValuesDigits, '0')}\n`;
        Buffer += `  uint${CRCbits2}_t CRC = 0x${this.Init.toString(16).padStart(ValuesDigits, '0')};\n`;

        if (this.RefOut !== this.RefIn && this.CRCbits > 8) {
            // CODE: reflected CRC
            Buffer += `  uint${CRCbits2}_t CRCref = 0;\n`;
        }

        Buffer += "  size_t i;\n";
        // CODE: polytable
        ValuesRow = CRC_CREATECODE_LINELENGTH / (ValuesDigits + 4); 
        Buffer += `  const uint${CRCbits2}_t Polytable[] = {\n`;
        for (let i = 0; i < 256; i++) {
            let Sep1 = (i % ValuesRow === 0) ? "   " : "";
            let Sep2 = (i !== 255) ? "," : "";
            let Sep3 = (i % ValuesRow === ValuesRow - 1 || i === 255) ? "\n" : "";
            Buffer += `${Sep1} 0x${this.Polytable[i].toString(16).padStart(ValuesDigits, '0')}${Sep2}${Sep3}`;
        }
        Buffer += "  };\n";

        // CODE: loop
        Buffer += "  for(i = 0; i < Length; i++) {\n";
        if (this.RefIn) {
            if (this.CRCbits <= 8) {
                // CODE: <=8bit, Refin
                Buffer += "    CRC = Polytable[Buffer[i] ^ CRC];\n";
            } else {
                // CODE: >8bit, Refin
                Buffer += "    CRC = Polytable[(Buffer[i] ^ CRC) & 0xff] ^ (CRC >> 8);\n";
            }
        } else {
            if (this.CRCbits <= 8) {
                // CODE: <=8bit, not Refin
                Buffer += "    CRC = Polytable[CRC ^ Buffer[i]];\n";
            } else {
                // CODE: >8bit, not Refin
                Buffer += `    CRC = Polytable[((CRC >> ${CRCbits_8}) & 0xff) ^ Buffer[i]] ^ (CRC << 8);\n`;
            }
        }
        // CODE: end of loop
        Buffer += "  }\n";

        if (!this.RefIn) {
            if (this.CRCpad) {
                // CODE: not RefIn, CRCpad >0
                Buffer += `  CRC >>= ${this.CRCpad};\n`;
            }
            if (this.CRCbits > 8 && (this.CRCbits + this.CRCpad) !== CRCbits2) {
                // CODE: not RefIn, CRCbits > 8
                Buffer += `  CRC &= 0x${this.Polymask.toString(16).padStart(ValuesDigits, '0')};\n`;
            }
        }

        if (this.RefOut !== this.RefIn) {
            // CODE: RefOut ^ Refin
            if(this.CRCbits <= 8) {
                // CODE: reflect <=8bit
                Buffer += "  CRC = ReflectTable[CRC];\n";
                if(this.CRCpad) {
                    Buffer += `  CRC >>= ${this.CRCpad.toString(10)}\n`;
                }
            }
            else {
                // CODE: reflect >8bit
                Buffer += `  for(i = 0; i < ${(CRCbits2 / 8).toString(10)}; i++) {\n`;
                Buffer += "    CRCref = (CRCref << 8) | ReflectTable[CRC & 0xff];\n";
                Buffer += "    CRC >>= 8;\n";
                Buffer += "  }\n";
                if(this.CRCpad) {
                    Buffer += `  CRC = CRCref >> ${this.CRCpad.toString(10)};\n`;
                }
                else {
                    Buffer += `  CRC = CRCref;\n`;
                }
            }
        }
        if(this.XOrOut) {
            // CODE: XOrOut !=0 
            Buffer +=  `  CRC ^= 0x${this.XOrOut.toString(16).padStart(ValuesDigits, '0')};\n`;
        }

        // CODE: end of function
        Buffer +=  "  return(CRC);\n";
        Buffer +=  "}\n";

        return(Buffer);
    }

}
