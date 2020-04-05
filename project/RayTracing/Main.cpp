// RayTracing.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include <chrono>
#include <ctime>
#include <time.h>
#include <iomanip>
#include <iostream>

using namespace std;
#include "Script.h"

int main(int argc, char* argv[])
{
	auto now = std::chrono::system_clock::now();
	std::time_t now_c = std::chrono::system_clock::to_time_t(now);
	struct tm t;
	localtime_s(&t, &now_c);

	int year = t.tm_year+1900;

	cout << "Laboratorium Grafiki Komputerowej " << year <<"." << std::setfill('0') << std::setw(2) << t.tm_mon+1 << "." << std::setfill('0') << std::setw(2) << t.tm_mday << endl;
	cout << "Executing script... please wait" << endl;

	if (argc < 2)
	{
		cout << "No script name given..." << endl;
		cout << "Usage: RayTracing.exe <filename>" << endl;
		system("pause");
		return 0;
	}

	auto start = std::chrono::system_clock::now();
	

	Script scr(argv[1]);

	if (scr.DoScript() != 0)
		cout << endl << scr.Error() << endl;
	else
		cout << endl << "OK" << endl;

	auto end = std::chrono::system_clock::now();

	double renderingTime =
		(double)(chrono::duration_cast<chrono::nanoseconds>(end - start).count());

	renderingTime *= 1e-9; // Time in seconds

	cout << "Rendering time is: " << fixed
		<< renderingTime << setprecision(9);
	cout << " sec" << endl << endl;

	system("pause");

	return 0;
}