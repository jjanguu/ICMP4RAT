#pragma once
#include <iostream>
#include <vector>

using namespace std;

class WMIC {
private:
	commandManager cm;

	string _executeWMIC(string command);
public:
	void test();

	string getAntivirusInfo();

	string getStartProgram();

	string getInstalledProgram(bool version = false);
	string getProgramVersionByPath(string path);
	string getBIOSInfo();
};