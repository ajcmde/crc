
class CRC_c:
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
                ]
        # reference: https://reveng.sourceforge.io/crc-catalogue/all.htm
        CRCs = [        
                ( "CRC-3/GSM", 3, 0x3, 0x0, False, False, 0x7),
                ( "CRC-3/ROHC", 3, 0x3, 0x7, True, True, 0x0),
                ( "CRC-4/G-704", 4, 0x3, 0x0, True, True, 0x0),
                ( "CRC-4/INTERLAKEN", 4, 0x3, 0xf, False, False, 0xf),
                ( "CRC-5/EPC-C1G2", 5, 0x09, 0x09, False, False, 0x0),
                ( "CRC-5/G-704", 5, 0x15, 0x0, True, True, 0x0),
                ( "CRC-5/USB", 5, 0x05, 0x1f, True, True, 0x1f),
                ( "CRC-6/CDMA2000-A", 6, 0x27, 0x3f, False, False, 0x0),
                ( "CRC-6/CDMA2000-B", 6, 0x07, 0x3f, False, False, 0x0),
                ( "CRC-6/DARC", 6, 0x19, 0x0, True, True, 0x0),
                ( "CRC-6/G-704", 6, 0x03, 0x0, True, True, 0x0),
                ( "CRC-6/GSM", 6, 0x2f, 0x0, False, False, 0x3f),
                ( "CRC-7/MMC", 7, 0x09, 0x0, False, False, 0x0),
                ( "CRC-7/ROHC", 7, 0x4f, 0x7f, True, True, 0x0),
                ( "CRC-7/UMTS", 7, 0x45, 0x0, False, False, 0x0),
                ( "CRC-8/AUTOSAR", 8, 0x2f, 0xff, False, False, 0xff),
                ( "CRC-8/BLUETOOTH", 8, 0xa7, 0x0, True, True, 0x0),
                ( "CRC-8/CDMA2000", 8, 0x9b, 0xff, False, False, 0x0),
                ( "CRC-8/DARC", 8, 0x39, 0x0, True, True, 0x0),
                ( "CRC-8/GSM-B", 8, 0x49, 0x0, False, False, 0xff),
                ( "CRC-8/HITAG", 8, 0x1d, 0xff, False, False, 0x0),
                ( "CRC-8/I-432-1", 8, 0x07, 0x0, False, False, 0x55),
                ( "CRC-8/I-CODE", 8, 0x1d, 0xfd, False, False, 0x0),
                ( "CRC-8/LTE", 8, 0x9b, 0x0, False, False, 0x0),
                ( "CRC-8/MAXIM-DOW", 8, 0x31, 0x0, True, True, 0x0),
                ( "CRC-8/MIFARE-MAD", 8, 0x1d, 0xc7, False, False, 0x0),
                ( "CRC-8/NRSC-5", 8, 0x31, 0xff, False, False, 0x0),
                ( "CRC-8/OPENSAFETY", 8, 0x2f, 0x0, False, False, 0x0),
                ( "CRC-8/ROHC", 8, 0x07, 0xff, True, True, 0x0),
                ( "CRC-8/SAE-J1850", 8, 0x1d, 0xff, False, False, 0xff),
                ( "CRC-8/SMBUS", 8, 0x07, 0x0, False, False, 0x0),
                ( "CRC-8/TECH-3250", 8, 0x1d, 0xff, True, True, 0x0),
                ( "CRC-8/WCDMA", 8, 0x9b, 0x0, True, True, 0x0),
                ( "CRC-10/ATM", 10, 0x233, 0x0, False, False, 0x0),
                ( "CRC-10/CDMA2000", 10, 0x3d9, 0x3ff, False, False, 0x0),
                ( "CRC-10/GSM", 10, 0x175, 0x0, False, False, 0x3ff),
                ( "CRC-11/FLEXRAY", 11, 0x385, 0x01a, False, False, 0x0),
                ( "CRC-11/UMTS", 11, 0x307, 0x0, False, False, 0x0),
                ( "CRC-12/CDMA2000", 12, 0xf13, 0xfff, False, False, 0x0),
                ( "CRC-12/DECT", 12, 0x80f, 0x0, False, False, 0x0),
                ( "CRC-12/GSM", 12, 0xd31, 0x0, False, False, 0xfff),
                ( "CRC-12/UMTS", 12, 0x80f, 0x0, False, True, 0x0),
                ( "CRC-13/BBC", 13, 0x1cf5, 0x0, False, False, 0x0),
                ( "CRC-14/DARC", 14, 0x0805, 0x0, True, True, 0x0),
                ( "CRC-14/GSM", 14, 0x202d, 0x0, False, False, 0x3fff),
                ( "CRC-15/CAN", 15, 0x4599, 0x0, False, False, 0x0),
                ( "CRC-15/MPT1327", 15, 0x6815, 0x0, False, False, 0x01),
                ( "CRC-16/ARC", 16, 0x8005, 0x0, True, True, 0x0),
                ( "CRC-16/CDMA2000", 16, 0xc867, 0xffff, False, False, 0x0),
                ( "CRC-16/CMS", 16, 0x8005, 0xffff, False, False, 0x0),
                ( "CRC-16/DDS-110", 16, 0x8005, 0x800d, False, False, 0x0),
                ( "CRC-16/DECT-R", 16, 0x0589, 0x0, False, False, 0x01),
                ( "CRC-16/DECT-X", 16, 0x0589, 0x0, False, False, 0x0),
                ( "CRC-16/DNP", 16, 0x3d65, 0x0, True, True, 0xffff),
                ( "CRC-16/EN-13757", 16, 0x3d65, 0x0, False, False, 0xffff),
                ( "CRC-16/GENIBUS", 16, 0x1021, 0xffff, False, False, 0xffff),
                ( "CRC-16/GSM", 16, 0x1021, 0x0, False, False, 0xffff),
                ( "CRC-16/IBM-3740", 16, 0x1021, 0xffff, False, False, 0x0),
                ( "CRC-16/IBM-SDLC", 16, 0x1021, 0xffff, True, True, 0xffff),
                ( "CRC-16/ISO-IEC-14443-3-A", 16, 0x1021, 0xc6c6, True, True, 0x0),
                ( "CRC-16/KERMIT", 16, 0x1021, 0x0, True, True, 0x0),
                ( "CRC-16/LJ1200", 16, 0x6f63, 0x0, False, False, 0x0),
                ( "CRC-16/M17", 16, 0x5935, 0xffff, False, False, 0x0),
                ( "CRC-16/MAXIM-DOW", 16, 0x8005, 0x0, True, True, 0xffff),
                ( "CRC-16/MCRF4XX", 16, 0x1021, 0xffff, True, True, 0x0),
                ( "CRC-16/MODBUS", 16, 0x8005, 0xffff, True, True, 0x0),
                ( "CRC-16/NRSC-5", 16, 0x080b, 0xffff, True, True, 0x0),
                ( "CRC-16/OPENSAFETY-A", 16, 0x5935, 0x0, False, False, 0x0),
                ( "CRC-16/OPENSAFETY-B", 16, 0x755b, 0x0, False, False, 0x0),
                ( "CRC-16/PROFIBUS", 16, 0x1dcf, 0xffff, False, False, 0xffff),
                ( "CRC-16/RIELLO", 16, 0x1021, 0xb2aa, True, True, 0x0),
                ( "CRC-16/SPI-FUJITSU", 16, 0x1021, 0x1d0f, False, False, 0x0),
                ( "CRC-16/T10-DIF", 16, 0x8bb7, 0x0, False, False, 0x0),
                ( "CRC-16/TELEDISK", 16, 0xa097, 0x0, False, False, 0x0),
                ( "CRC-16/TMS37157", 16, 0x1021, 0x89ec, True, True, 0x0),
                ( "CRC-16/UMTS", 16, 0x8005, 0x0, False, False, 0x0),
                ( "CRC-16/USB", 16, 0x8005, 0xffff, True, True, 0xffff),
                ( "CRC-16/XMODEM", 16, 0x1021, 0x0, False, False, 0x0),
                ( "CRC-17/CAN-FD", 17, 0x1685b, 0x0, False, False, 0x0),
                ( "CRC-21/CAN-FD", 21, 0x102899, 0x0, False, False, 0x0),
                ( "CRC-24/BLE", 24, 0x065b, 0x555555, True, True, 0x0),
                ( "CRC-24/FLEXRAY-A", 24, 0x5d6dcb, 0xfedcba, False, False, 0x0),
                ( "CRC-24/FLEXRAY-B", 24, 0x5d6dcb, 0xabcdef, False, False, 0x0),
                ( "CRC-24/INTERLAKEN", 24, 0x328b63, 0xffffff, False, False, 0xffffff),
                ( "CRC-24/LTE-A", 24, 0x864cfb, 0x0, False, False, 0x0),
                ( "CRC-24/LTE-B", 24, 0x800063, 0x0, False, False, 0x0),
                ( "CRC-24/OPENPGP", 24, 0x864cfb, 0xb704ce, False, False, 0x0),
                ( "CRC-24/OS-9", 24, 0x800063, 0xffffff, False, False, 0xffffff),
                ( "CRC-30/CDMA", 30, 0x2030b9c7, 0x3fffffff, False, False, 0x3fffffff),
                ( "CRC-31/PHILIPS", 31, 0x04c11db7, 0x7fffffff, False, False, 0x7fffffff),
                ( "CRC-32/AIXM", 32, 0x814141ab, 0x0, False, False, 0x0),
                ( "CRC-32/AUTOSAR", 32, 0xf4acfb13, 0xffffffff, True, True, 0xffffffff),
                ( "CRC-32/BASE91-D", 32, 0xa833982b, 0xffffffff, True, True, 0xffffffff),
                ( "CRC-32/BZIP2", 32, 0x04c11db7, 0xffffffff, False, False, 0xffffffff),
                ( "CRC-32/CD-ROM-EDC", 32, 0x8001801b, 0x0, True, True, 0x0),
                ( "CRC-32/CKSUM", 32, 0x04c11db7, 0x0, False, False, 0xffffffff),
                ( "CRC-32/ISCSI", 32, 0x1edc6f41, 0xffffffff, True, True, 0xffffffff),
                ( "CRC-32/ISO-HDLC", 32, 0x04c11db7, 0xffffffff, True, True, 0xffffffff),
                ( "CRC-32/JAMCRC", 32, 0x04c11db7, 0xffffffff, True, True, 0x0),
                ( "CRC-32/MEF", 32, 0x741b8cd7, 0xffffffff, True, True, 0x0),
                ( "CRC-32/MPEG-2", 32, 0x04c11db7, 0xffffffff, False, False, 0x0),
                ( "CRC-32/XFER", 32, 0x0af, 0x0, False, False, 0x0),
                ( "CRC-40/GSM", 40, 0x04820009, 0x0, False, False, 0xffffffffff),
                ( "CRC-64/ECMA-182", 64, 0x42f0e1eba9ea3693, 0x0, False, False, 0x0),
                ( "CRC-64/GO-ISO", 64, 0x01b, 0xffffffffffffffff, True, True, 0xffffffffffffffff),
                ( "CRC-64/MS", 64, 0x259c84cba6426349, 0xffffffffffffffff, True, True, 0x0),
                ( "CRC-64/REDIS", 64, 0xad93d23594c935a9, 0x0, True, True, 0x0),
                ( "CRC-64/WE", 64, 0x42f0e1eba9ea3693, 0xffffffffffffffff, False, False, 0xffffffffffffffff),
                ( "CRC-64/XZ", 64, 0x42f0e1eba9ea3693, 0xffffffffffffffff, True, True, 0xffffffffffffffff) 
                ]
        
        def CRCReflect(self, number, bits, bitspad):
                reflection = 0
                for i in range((bits + bitspad) >> 3):
                        reflection = (reflection << 8) | self.ReflectTable[number & 0xff]
                        number >>= 8

                reflection >>= bitspad
                return reflection
        
        def __init__(self, CRCbits, Polynom, Init, RefIn, RefOut, XOrOut):
                self.CRCbits = CRCbits
                Polymask = ((1 << self.CRCbits) - 1)
                Polymsb = 1 << (self.CRCbits - 1)
                self.Polymask = Polymask
                self.Polynom = Polynom
                self.Init = Init & Polymask
                self.RefIn = RefIn
                self.RefOut = RefOut
                self.XOrOut = XOrOut & Polymask
                self.CRCpad = (8 - (self.CRCbits & 7)) & 7 # non byte aligned CRC;
                
                if not self.RefIn:
                        Polynom <<= self.CRCpad
                        Polymsb <<= self.CRCpad
                        Polymask <<= self.CRCpad
                # initialze table     
                CRC = Polymsb
                self.Polytable = [0] * 256

                for i in (1, 2, 4, 8, 16, 32, 64, 128):
                        if CRC & Polymsb:
                                CRC = (CRC << 1) ^ Polynom
                        else:
                                CRC <<= 1
                        CRC &= Polymask                     
                        for j in range(i):
                                self.Polytable[self.CRCReflect(i + j, 8, 0) if self.RefIn else i + j] = CRC ^ self.Polytable[self.CRCReflect(j, 8, 0) if self.RefIn else j]
                if self.RefIn:
                        for i in range(1, 256):
                                self.Polytable[i] = self.CRCReflect(self.Polytable[i], self.CRCbits, self.CRCpad)
        def CRC(self, Buffer):
                crc = self.Init
                length = len(Buffer)
                if self.RefIn:
                        for i in range(length):
                                crc = self.Polytable[(Buffer[i] ^ crc) & 0xff] ^ (crc >> 8)
                else:
                        CRCbits_8 = (self.CRCbits - 1) & ((-1) ^ 7); # CRC bits aligned to next byte border (12 -> 8, 32 -> 24)
                        for i in range(length):
                                crc = self.Polytable[((crc >> CRCbits_8) & 0xff) ^ Buffer[i]] ^ (crc << 8)
        
                        crc >>= self.CRCpad;  # non byte aligned CRC
                        crc &= self.Polymask

                if self.RefOut ^ self.RefIn:
                        crc = self.CRCReflect(crc, self.CRCbits, self.CRCpad)
                crc ^= self.XOrOut
                return crc

        @classmethod
        def CRCCreate(self, CRCbits, Polynom, Init, RefIn, RefOut, XOrOut):
                return self(CRCbits, Polynom, Init, RefIn, RefOut, XOrOut)

        @classmethod
        def CRCCreateFromName(self, CRCName):
                CRCdef = [item for item in self.CRCs if item[0] == CRCName]
                CRCdef = CRCdef[0]
                return self(CRCdef[1], CRCdef[2], CRCdef[3], CRCdef[4], CRCdef[5], CRCdef[6])
                
