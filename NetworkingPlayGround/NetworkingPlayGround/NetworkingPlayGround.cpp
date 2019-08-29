// NetworkingPlayGround.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Socket.h"
#include <string>
#include <iostream>

#define SQUARE_ROOT_PRECISION 5

Socket* udpSocket;

float SquareRoot(float num) {

	// Square rooting is like playing the guessing game
	// Here I will do a little structured guessing until I reach a approximate number

	float finalNum = 0.0f;      // Storing the final square root
	float multiplerUp = 10.0f;  // Used for shifting numbers up in decimals
	float multiplerDown = 0.1f; // Used for shifting numbers down in decimals
	float calcResults = 0.0f;   // Stores calculations done after the main subtraction 
	int rootNumCounter = 1;     // Since im guessing, this counter will increment up
								//until i hit something good

	int rootResult = 1;       // Stores the rootNumCounter squared

							  // 1)  Incoming number has to have only 1 or 2 digits before the  decimal (ie 
							  // ~ 1.34 or 94.78)
	while ((num / 100.0f) >= 1)
	{
		num *= 0.01f;
	}

	// 2) Next we have to find the number squared that is closest and lowest to the
	// incoming number that we modified the step above
	while (rootResult <= num)
	{
		rootNumCounter++;
		rootResult = rootNumCounter  * rootNumCounter;
	}
	// Getting the closest lowest number
	rootNumCounter--;

	// Now we have the first number of the final result
	finalNum += rootNumCounter;
	rootResult = rootNumCounter * rootNumCounter;

	// 3) Now we have to calculate the decimals that come after the final number

	// here we need to store the result of subtracting the modified incoming number by
	// the closest lower squared number. Then we multiply by 100 to move the decimal
	// over 2
	calcResults = (num - rootResult) * 100;

	// Storing the final number doubled
	rootResult = finalNum * 2;

	// Calculating decimal numbers
	for (int i = 1; i < SQUARE_ROOT_PRECISION; i++)
	{
		// We need to take the stored number we doubled earlier and shift it one
		// space over to the 
		// left so we can search for the next decimal number by using its ones
		// space 
		rootResult *= 10;
		rootNumCounter = 1;

		// Checking for closest lowest squared number to calcResult
		while (((rootResult + rootNumCounter) * rootNumCounter) <= calcResults)
		{
			rootNumCounter++;
		}
		rootNumCounter--;
		rootResult += rootNumCounter;

		// Calculating new calcResult
		calcResults = calcResults - (rootResult * rootNumCounter);
		// Adding extra 2 space to the number;
		calcResults *= 100;

		// 4) Finally we take the discovered closest number and multiply it to push
		// it
		// to the correct decimal place
		finalNum += rootNumCounter * multiplerDown;

		// we have to keep track of the whole number of the final result for the
		// next decimal search
		rootResult = (finalNum * multiplerUp) * 2;

		// Setting up for the next decimal number that needs to be pushed to
		// correct slot
		multiplerDown *= 0.1f;
		multiplerUp *= 10;
	}

	return finalNum;
}



int main()
{
	WORD test = MAKEWORD(0xF, 0xF);

	// Step 0: Init socket layer

	bool init = Socket::InitSocket();

	if (!init)
	{
		printf("Socket initializing fucked up");
	}

	// Step 1: Create a UDP Socket
	udpSocket = new Socket(SocketType::UDP);

	// Step 2: Bind the socket to a port
	udpSocket->Open(30000);

	// Step 3(Optional): Set socket to Non-Blocking
	//udpSocket->SetNonBlockingMode();

	udpSocket->JoinThreads();

	//float squareRoot = SquareRoot(54);
	//printf("%f", squareRoot);


	delete udpSocket;
    return 0;
}

