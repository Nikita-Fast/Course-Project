#pragma once


class CRC16 {

public:

	CRC16();

	void reset();
	unsigned short feed(unsigned char);

	unsigned short result() const;

private:

	unsigned short crc;

	static const unsigned short ftab16[256];

};