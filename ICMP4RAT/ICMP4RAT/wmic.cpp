#include "wmic.h"
#include "util.h"
#include <algorithm>

// �׽�Ʈ�� �Լ�
void WMIC::test() {
	string results = this->getInstalledProgram(true);
	cout << results << endl;
}

// ��ġ�� ��� ������ ��θ� ������
string WMIC::getAntivirusInfo() {
	string wmicCommand = "wmic /namespace:\\\\root\\securitycenter2 path antivirusproduct GET displayName, pathToSignedProductExe";

	string result = this->_executeWMIC(wmicCommand);

	return result;
}

// ���� ���α׷��� �̸��� Ŀ�ǵ带 ������
string WMIC::getStartProgram() {
	string wmicCommand = "wmic startup get name, command";

	return this->_executeWMIC(wmicCommand);
}

// ��ġ�� ���α׷��� �̸��� ������ ������
string WMIC::getInstalledProgram(bool version) {
	string wmicCommand = "wmic product get name";

	if (version)
		wmicCommand.append(", version");

	return this->_executeWMIC(wmicCommand);
}

// program path�� ���ڷ� �ָ� ������ return
string WMIC::getProgramVersionByPath(string path) {
	string versionQueryStart = "wmic datafile where name=\"";
	string versionQueryEnd = "\" get Version";

	if (path == "")
		return "";

	path = StringUtil::ReplaceAll(path, "\\", "\\\\");
	
	return this->_executeWMIC(versionQueryStart + path + versionQueryEnd);
}

// bios ������ ������
string WMIC::getBIOSInfo()
{
	string wmicCommand = "wmic bios get caption, serialnumber, version";
	string result = this->reverseShell(wmicCommand);

	return StringUtil::split(StringUtil::trim(result),'\n').back();
}

// commandmanager�� ���� wmic command ����
string WMIC::_executeWMIC(string command) {
	string result = this->reverseShell(command);

	return result.erase(0, result.find('\n') + 1);
}

std::string WMIC::reverseShell(std::string cmd) {
	char buffer[128];
	std::string result = "";
	FILE* pipe = _popen(cmd.c_str(), "r");
	if (!pipe) throw std::runtime_error("popen() failed!");
	try {
		while (fgets(buffer, sizeof(buffer), pipe) != NULL)
			result += buffer;
	}
	catch (...) {
		_pclose(pipe);
		throw;
	}
	_pclose(pipe);
	return result;
}