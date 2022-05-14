#include "commandManager.h"
#include "wmic.h"
#include "util.h"
#include <algorithm>

// 테스트용 함수
void WMIC::test() {
	string results = this->getInstalledProgram(true);
	cout << results << endl;
}

// 설치된 백신 정보와 경로를 가져옴
string WMIC::getAntivirusInfo() {
	string wmicCommand = "wmic /namespace:\\\\root\\securitycenter2 path antivirusproduct GET displayName, pathToSignedProductExe";

	string result = this->_executeWMIC(wmicCommand);

	return result;
}

// 시작 프로그램의 이름과 커맨드를 가져옴
string WMIC::getStartProgram() {
	string wmicCommand = "wmic startup get name, command";

	return this->_executeWMIC(wmicCommand);
}

// 설치된 프로그램의 이름과 버전을 가져옴
string WMIC::getInstalledProgram(bool version) {
	string wmicCommand = "wmic product get name";

	if (version)
		wmicCommand.append(", version");

	return this->_executeWMIC(wmicCommand);
}

// program path를 인자로 주면 버전을 return
string WMIC::getProgramVersionByPath(string path) {
	string versionQueryStart = "wmic datafile where name=\"";
	string versionQueryEnd = "\" get Version";

	if (path == "")
		return "";

	path = StringUtil::ReplaceAll(path, "\\", "\\\\");
	
	return this->_executeWMIC(versionQueryStart + path + versionQueryEnd);
}

// bios 정보를 가져옴
string WMIC::getBIOSInfo()
{
	string wmicCommand = "wmic bios get caption, serialnumber, version";
	string result = this->cm.reverseShell(wmicCommand);

	return StringUtil::split(StringUtil::trim(result),'\n').back();
}

// commandmanager를 통해 wmic command 실행
string WMIC::_executeWMIC(string command) {
	string result = this->cm.reverseShell(command);

	return result.erase(0, result.find('\n') + 1);
}