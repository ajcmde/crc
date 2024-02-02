# code fragement


_CRCpattern0 = b"\x0f"
_CRCpattern1 = b"\x43\x52\x43"
_CRCpattern2 = b"\x01\x02\x04\x08\x10\x20\x40\x80"
_CRCpattern3 = b"\x00\x01\x00\x02\x00\x04\x00\x08\x00\x10\x00\x20\x00\x40\x00\x80\x01\x00\x02\x00\x04\x00\x08\x00\x10\x00\x20\x00\x40\x00\x80\x00"
_CRCpattern4 = b"\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01"
_CRCpattern5 = b"\xff"
_CRCpattern6 = b"\xff\xff"
_CRCpattern7 = b"\x43\x52\x43\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39"
_CRCpattern8 = b"\x33\x22\x55\xAA\xBB\xCC\xDD\xEE\xFF" 

CRCtestpattern0 = ("0xf", _CRCpattern0)
CRCtestpattern1 = ("\"CRC\"", _CRCpattern1)
CRCtestpattern2 = ("singlebits(8)", _CRCpattern2)
CRCtestpattern3 = ("singlebits(16)", _CRCpattern3)
CRCtestpattern4 = ("bit1(8)", _CRCpattern4)
CRCtestpattern5 = ("0xff", _CRCpattern5)
CRCtestpattern6 = ("0xffff", _CRCpattern6)
CRCtestpattern7 = ("CRC0123456789", _CRCpattern7)
CRCtestpattern8 = ("AUTOSAR-05", _CRCpattern8)




CRCtests = [
        ("CRC-8/I-432-1", CRCtestpattern1, 0x86),
        ("CRC-8/CDMA2000", CRCtestpattern1, 0x92),
        ("CRC-8/CDMA2000", CRCtestpattern3, 0x56),
        ("CRC-8/DARC", CRCtestpattern1, 0xf5),
        ("CRC-8/DARC", CRCtestpattern2, 0x8a),
        ("CRC-8/DARC", CRCtestpattern3, 0x0),
        ("CRC-8/DARC", CRCtestpattern4, 0x0),
        ("CRC-12/UMTS", CRCtestpattern1, 0x55d),
        ("CRC-7/UMTS", CRCtestpattern1, 0x75),
        ("CRC-7/ROHC", CRCtestpattern1, 0x17),
        ("CRC-7/ROHC", CRCtestpattern2, 0x32),
        ("CRC-7/UMTS", CRCtestpattern0, 0x22),
        ("CRC-7/UMTS", CRCtestpattern5, 0x59),
        ("CRC-7/UMTS", CRCtestpattern6, 0x24),
        ("CRC-8/SAE-J1850", CRCtestpattern8, 0xcb), 
        ("CRC-8/AUTOSAR", CRCtestpattern8, 0x11),
        ("CRC-10/ATM", CRCtestpattern2, 0x15d),
        ("CRC-16/CDMA2000", CRCtestpattern2, 0xa3ee),
        ("CRC-16/IBM-3740", CRCtestpattern8, 0xf53f),        
        ("CRC-16/ARC", CRCtestpattern8, 0xae98),
        ("CRC-24/OPENPGP", CRCtestpattern7, 0x5639ee),
        ("CRC-32/ISCSI", CRCtestpattern7, 0xfa1d936a),
        ("CRC-32/ISO-HDLC", CRCtestpattern8, 0xb0ae863d), 
        ("CRC-32/AUTOSAR", CRCtestpattern8, 0xa65a343d),
        ("CRC-40/GSM", CRCtestpattern7, 0x19eb2415ca),
        ("CRC-64/WE", CRCtestpattern7, 0xb072fe3926575027),
        ("CRC-64/XZ", CRCtestpattern7, 0x319fa647bf14a7c2),
        ("CRC-64/XZ", CRCtestpattern8, 0x701ECEB219A8E5D5) ]

for test in CRCtests:
        CRChandle = CRC_c.CRCCreateFromName(test[0])
        crc = CRChandle.CRC(test[1][1])
        if crc != test[2]:
                print(test[0], "[", test[1][0], "]", "test failed:", "calculated", hex(crc), "expected", hex(test[2]))
        else:
                print(test[0], "[", test[1][0], "]", "test passed")



