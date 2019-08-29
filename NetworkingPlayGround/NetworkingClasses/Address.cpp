#include "Address.h"


Address::Address(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned short po)
{

	address = (a << 24) | (b << 16) | (c << 8) | d;
	port = po;
}

Address::Address(unsigned int addr, unsigned short po)
{
	address = addr;
	port = po;
}


Address::~Address()
{
}

unsigned int Address::GetAddress()
{
	return address;
}

unsigned int Address::GetA()
{
	return (address & 0xFF000000) >> 24;
}

unsigned int Address::GetB()
{
	return (address & 0x00FF0000) >> 16;
}

unsigned int Address::GetC()
{
	return (address & 0x0000FF00) >> 8;
}

unsigned int Address::GetD()
{
	return (address & 0x000000FF);
}

unsigned short Address::GetPort()
{
	return port;
}
