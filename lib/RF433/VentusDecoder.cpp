#include "Arduino.h"
#include "DecodeOOK.cpp"

const byte reverse_bits_lookup[] = {
    0x0, 0x8, 0x4, 0xC, 0x2, 0xA, 0x6, 0xE,
    0x1, 0x9, 0x5, 0xD, 0x3, 0xB, 0x7, 0xF
};

class VentusDecoder : public DecodeOOK 
{
    public:
        VentusDecoder() {}
        virtual ~VentusDecoder() {}

        // see also http://www.tfd.hu/tfdhu/files/wsprotocol/auriol_protocol_v20.pdf
        virtual char decode(word width) 
        {  
            switch (state) {
            case UNKNOWN: // Signal on
                if (425 <= width && width < 575)
                    state = T0;
                else
                    return -1;
                break;
            case T0: // Signal off = bit
                if (1700 < width && width < 4600)
                {
                    if (width < 2300)
                    {
                        gotBit(0);
                    }
                    else if (width > 3400)
                        gotBit(1);
                    else
                        return -1;
                    state = UNKNOWN;
                }
                else if (total_bits > 35 && 7650 < width && width < 10350)
                {
                    data[pos] = data[pos] << 4;
                    pos++;
                    return 1;
                }
                else
                    return -1;
                break;
            default:
                return -1;
            }
            return 0;
        }

        virtual bool checkSum()
        {
            if (bits.size() != 36) { 
                return false; 
            }

            byte s, t;
            bool rain = ((data[1] & 0x7F) == 0x6C);
            s = (rain ? 0x7 : 0xF);
            for (byte i = 0; i < 8; i++)
            {
                if (i % 2)
                    t = reverse_bits_lookup[(data[i / 2] & 0xF)];
                else
                    t = reverse_bits_lookup[(data[i / 2] >> 4)];
                s += (rain ? t : -t);
            }
            return (s & 0x0F) == reverse_bits_lookup[(data[4] >> 4)];
        }
};


  