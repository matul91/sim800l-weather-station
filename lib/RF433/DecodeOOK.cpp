#include "Arduino.h"

#ifndef _DECODE_OOK_H_
#define _DECODE_OOK_H_

class DecodeOOK
{

protected:
	byte total_bits, state, pos, data[25];
	std::vector<char> bits;
			
	virtual char decode(word width) = 0;

public:
	enum
	{
		UNKNOWN,
		T0,
		T1,
		T2,
		T3,
		OK,
		DONE
	};

	DecodeOOK() { resetDecoder(); }

	virtual bool checkSum() { return true; }

	bool nextPulse(word width)
	{
		if (state != DONE)
			switch (decode(width))
			{
				case -1:
					resetDecoder();
					break;
				case 1:
					state = DONE;
					break;
			}
		return isDone();
	}

	bool isDone() const { 
		return state == DONE; 
	}

	void resetDecoder()
	{
		bits.clear();
		total_bits = pos = 0;
		state = UNKNOWN;
	}

	virtual void gotBit(char value)
	{
		bits.push_back(value);
		data[pos] = (data[pos] << 1) | value;
		total_bits++;
		pos = total_bits >> 3;

		if (pos >= sizeof data)
		{
			resetDecoder();
			return;
		}

		state = OK;
	}

	const std::vector<char> getData () {
		bits.shrink_to_fit();
		return bits;
    }
};
#endif