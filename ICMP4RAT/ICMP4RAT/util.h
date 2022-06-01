#pragma once
#include <vector>
#include <string>
#include <sstream>

using namespace std;

namespace StringUtil {
    extern std::vector<std::string> split(std::string target, char chracter);

    extern std::string ReplaceAll(std::string& str, const std::string& from, const std::string& to);

    extern inline std::string& ltrim(std::string& str, const char* eraseChar = " \t\n\r\f\v");

    extern inline std::string& rtrim(std::string& str, const char* eraseChar = " \t\n\r\f\v");

    extern inline std::string& trim(std::string& str, const char* eraseChar = " \t\n\r\f\v");
};