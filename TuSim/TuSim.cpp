#include "stdafx.h"
#include "screen.h"
#include "Traffic.h"
#include <iostream>
#include <iomanip>      // std::setprecision

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	string output;
	Screen obr;
	// Create and clear screen
	obr.Cls();
	// Init
	InitTraffic();
	// Simulation 
	for (int t = 0; t<TIME_SIM; t++)
	{ 	// Traffic step    
		TrafficSimStep(t, output);
		obr.SetCurXY(0, 0);
		cout << output << endl;
		// Delay
		Sleep(20);
	}
	// Exit
	ExitTraffic();
	getchar();
	return 0;
}

