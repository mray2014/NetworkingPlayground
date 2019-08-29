#pragma once
class Address
{
public:
	Address(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned short po);
	Address(unsigned int addr, unsigned short po);
	~Address();

	unsigned int GetAddress();
	unsigned int GetA();
	unsigned int GetB();
	unsigned int GetC();
	unsigned int GetD();

	unsigned short GetPort();

private:
	unsigned int address;
	unsigned short port;
};

