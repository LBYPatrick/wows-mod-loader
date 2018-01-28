
#include "Utils.h"
#include "includes.h"

string Utils::getRawFileName(string &fullPath) {
    int startPosition = 0;
    int endPosition = fullPath.length()-1;
    string returnBuffer;

    size_t slashPosition = fullPath.find_last_of('\\');
    size_t backSlashPosition = fullPath.find_last_of('/');

    if(slashPosition != string::npos) {
        startPosition = int(slashPosition)+1;
    }
    else if(backSlashPosition != string::npos) {
        startPosition = int(backSlashPosition)+1;
    }
    for(int n = startPosition; n <= endPosition;++n) {
        returnBuffer += fullPath[n];
    }

    return returnBuffer;
}

//From CSDN, rewritten for a reverse usage (originally for iterate files)
void Utils::getFolderList(string path, vector<string> & folderList) {

	struct _finddata_t fileInfo;
	string workingPath = path + R"(\*)";
	int folderCount = 0;
	long hFile = _findfirst( workingPath.c_str(), &fileInfo);
	
	do {
		if (fileInfo.attrib & _A_SUBDIR) {
			folderCount ++;
			if (folderCount <= 2) continue; //Skip if it's parent-folder or current folder
			else folderList.push_back(fileInfo.name);
		}

	} while (_findnext(hFile, &fileInfo) == 0);

}

//From MSDN
void Utils::runCMD(string command) {
    command = R"(cmd /q /c ")" + command + R"(")";

	TCHAR *commandInTCHAR = new TCHAR[command.size() + 1];
	commandInTCHAR[command.size()] = 0;
	std::copy(command.begin(), command.end(), commandInTCHAR);

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	si.lpReserved = NULL;
	si.lpDesktop = NULL;
	si.lpTitle = NULL;
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	si.cbReserved2 = NULL;
	si.lpReserved2 = NULL;

	si.cb = sizeof(si);
	ZeroMemory(&si, sizeof(si));
	ZeroMemory(&pi, sizeof(pi));

	bool result = CreateProcess(NULL,   // No module name (use command line)
		commandInTCHAR,    // Command line
		NULL,              // Process handle not inheritable
		NULL,              // Thread handle not inheritable
		FALSE,			   // Set handle inheritance to FALSE
	    CREATE_NO_WINDOW,  // No creation flags (Defines whether a window would be created)
		NULL,              // Use parent's environment block
		NULL,              // Use parent's starting directory 
		&si,               // Pointer to STARTUPINFO structure
		&pi)               // Pointer to PROCESS_INFORMATION structure
		;

	WaitForSingleObject(pi.hProcess, INFINITE);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
}

string Utils::getWOWSversion() {
	
	ifstream reader;
	string readBuffer;

	reader.open("paths.xml");

	while (getline(reader, readBuffer)) {
		if (readBuffer.find("<Path>res_mods") != string::npos) {
			string returnBuffer;
			int startPosition = readBuffer.find_first_of("res_mods/") + 9;
			int endPosition = readBuffer.find_last_of("</Path>") - 7;

			for (int n = startPosition; n <= endPosition; ++n) {
				returnBuffer += readBuffer[n];
			}
			reader.close();
			return returnBuffer;
		}
	}
	reader.close();
	return "Error";
}

void Utils::renameFile(string pre, string post) {
	runCMD("ren \"" + pre + "\" " + getRawFileName(post));
}

void Utils::showPercentage(int current, int total, string message) {
	
	system("cls");
		
		const int barLength = 50;
		const int leftPercent  = double(current + 1) / double(total) * barLength;
		const int rightPercent = barLength - leftPercent; 

		printf("%s\n[", message.c_str());
		current++;

		for (int i = 0; i <  leftPercent ; i++)        printf(">");
		for (int i = 0; i <  rightPercent; i++)        printf(" ");
		
		printf("] %d / %d", current, total);
		
		if (current == total) printf("\n\n\n");
}