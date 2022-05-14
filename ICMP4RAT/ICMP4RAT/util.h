#pragma once
#include <vector>
#include <string>
#include <sstream>

using namespace std;

namespace StringUtil {
	std::vector<std::string> split(std::string target, char chracter) {
        vector<string> result;
        stringstream stream(target);
        string tmp;

        while (getline(stream, tmp, chracter)) {
            result.push_back(tmp);
        }
        return result;
	}

    std::string ReplaceAll(std::string& str, const std::string& from, const std::string& to) {
        size_t start_pos = 0; 
        while ((start_pos = str.find(from, start_pos)) != std::string::npos)  
        {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
        return str;
    }

    inline std::string& ltrim(std::string& str, const char* eraseChar = " \t\n\r\f\v")
    {
        str.erase(0, str.find_first_not_of(eraseChar));
        return str;
    }

    inline std::string& rtrim(std::string& str, const char* eraseChar = " \t\n\r\f\v")
    {
        str.erase(str.find_last_not_of(eraseChar) + 1);
        return str;
    }

    inline std::string& trim(std::string& str, const char* eraseChar = " \t\n\r\f\v")
    {
        return ltrim(rtrim(str, eraseChar), eraseChar);
    }
};