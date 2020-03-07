#include "Arduino.h"
#include "DecodeOOK.cpp"

class AuriolDecoder : public DecodeOOK
{
protected:
    boolean isOdd = false;

public:
    AuriolDecoder() {}
    virtual ~AuriolDecoder() {}

    virtual char decode(word width)
    {
        switch (state)
        {
        case UNKNOWN:
            if (700 <= width && width < 1000)
            {
                isOdd = true;
                state = T0;
            }
            else
                return -1;
            break;
        case T0:
            if (140 <= width && width < 750)
            {
                isOdd = !isOdd;
                if (width < 400)
                {
                    if (!isOdd)
                    {
                        gotBit(0);
                    }
                }
                else
                {
                    if (!isOdd)
                    {
                        gotBit(1);
                    }
                }
                state = T0;
            }
            else if (200 <= width && width < 400)
            {
                return 0;
            }
            else if (700 <= width && width < 1000)
            {
                return 0;
            }
            else if (58000 <= width && width < 63000)
            {
                return 1;
            }
            else
            {
                return -1;
            }
            break;
        default:
            return -1;
        }
        return 0;
    }

    virtual bool checkSum()
    {   
        bits.shrink_to_fit();

        if (bits.size() != 40)
        {
            return false;
        }

        uint8_t b0 = 0;
        uint8_t b1 = 0;
        uint8_t b2 = 0;
        uint8_t b3 = 0;
        uint8_t b4 = 0;

        for (int i = 0; i < 8; i++) b0 |= bits[i] << (i);
        for (int i = 8; i < 16; i++) b1 |= bits[i] << (i - 8);
        for (int i = 16; i < 24; i++) b2 |= bits[i] << (i - 16);
        for (int i = 24; i < 32; i++) b3 |= bits[i] << (i - 24);
        for (int i = 32; i < 40; i++) b4 |= bits[i] << (i - 32);

        b0 = rev_bits(b0);
        b1 = rev_bits(b1);
        b2 = rev_bits(b2);
        b3 = rev_bits(b3);
        b4 = rev_bits(b4);

        uint8_t d0 = b0 ^ b1 ^ b2 ^ b3;
        uint8_t chk = crc8(&d0, 1, 0x31, 0x53) ^ b4;

        if (chk) {
            return false;
        }
         
        return true;
    }

    uint8_t rev_bits(uint8_t input)
    {
        uint8_t output = 0;
        unsigned int n = sizeof(input) << 3;
        unsigned int i = 0;

        for (i = 0; i < n; i++)
            if ((input >> i) & 0x1)
                output |=  (0x1 << (n - 1 - i));

        return output;
    }

    int getHumidity() {
        uint8_t b0 = 0;
        for (int i = 8; i < 16; i++) b0 |= bits[i] << (i - 8);

        return (int) rev_bits(b0);
    }

    float getTemperature() {
        int decimalValue = 0; 
        int base = 1; 

        for (int i = 31 ; i > 20; i--) { 
            if (bits[i] == 1) {
                decimalValue += base; 
            } 
         
            base = base * 2; 
        } 

        int rawResult = decimalValue;
        if (bits[20] == 1) rawResult *= -1;

        return rawResult * 0.1f;
    }


    uint8_t crc8(uint8_t const message[], unsigned nBytes, uint8_t polynomial, uint8_t init)
        {
            uint8_t remainder = init;
            unsigned byte, bit;

            for (byte = 0; byte < nBytes; ++byte) {
                remainder ^= message[byte];
                for (bit = 0; bit < 8; ++bit) {
                    if (remainder & 0x80) {
                        remainder = (remainder << 1) ^ polynomial;
                    } else {
                        remainder = (remainder << 1);
                    }
                }
            }
            return remainder;
        }
};
