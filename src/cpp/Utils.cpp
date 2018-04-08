#define _SCL_SECURE_NO_WARNINGS
#include "Utils.h"
#include "includes.h"

bool visualizing = true;

vector<string> Utils::ReadFile(string filename) {

	vector<string> return_buffer;
	ifstream reader;
	string read_buffer;

	//Replace Unix's backslashes with Windows' slashes
	for (int i = 0; i < filename.size(); ++i) {
		if (filename[i] == '/') { 
			filename[i] = '\\';
		}
	}

	//Check file access, return an empty list if failed
	reader.open(filename.c_str());
	if (!reader.is_open()) return {};

	//Start reading
	while (getline(reader, read_buffer)) {
		return_buffer.push_back(read_buffer);
	}

	reader.close();

	return return_buffer;
}

string Utils::GetRawFileName(string full_path) {
    int startPosition = 0;
    int endPosition = full_path.length()-1;
    string returnBuffer;

    size_t slashPosition = full_path.find_last_of('\\');
    size_t backSlashPosition = full_path.find_last_of('/');

    if(slashPosition != string::npos) {
        startPosition = int(slashPosition)+1;
    }
    else if(backSlashPosition != string::npos) {
        startPosition = int(backSlashPosition)+1;
    }
    for(int n = startPosition; n <= endPosition;++n) {
        returnBuffer += full_path[n];
    }

    return returnBuffer;
}

//From CSDN, rewritten for a reverse usage (originally for iterate files)
vector<string> Utils::GetFolderList(string path) {

	struct _finddata_t file_info;
	const string workingPath = path + R"(\*)";
	int folderCount = 0;
	long hFile = _findfirst(workingPath.c_str(), &file_info);
	vector<string> return_buffer;


	do {
		if (file_info.attrib & _A_SUBDIR) {
			folderCount ++;
			if (folderCount <= 2) continue; //Skip if it's parent-folder or current folder
			else return_buffer.push_back(file_info.name);
		}

	} while (_findnext(hFile, &file_info) == 0);

	return return_buffer;
}

//From MSDN
void Utils::RunCommand(string command) {
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

string Utils::GetWowsVersion() {
	
	ifstream reader;
	string read_buffer;

	reader.open("paths.xml");

	while (getline(reader, read_buffer)) {
		if (read_buffer.find("<Path>res_mods") != string::npos) {
			string returnBuffer;
			const int startPosition = read_buffer.find_first_of("res_mods/") + 9;
			const int endPosition = read_buffer.find_last_of("</Path>") - 7;

			for (int n = startPosition; n <= endPosition; ++n) {
				returnBuffer += read_buffer[n];
			}
			reader.close();
			return returnBuffer;
		}
	}
	reader.close();
	return string();
}

void Utils::RenameFile(string old_name, string new_name) {
	RunCommand("ren \"" + old_name + "\" " + GetRawFileName(new_name));
}

void Utils::PercentageBar(int current, int total) {
		
	if (!visualizing) return;

		const int barLength = 50;
		const int leftPercent  = double(current) / double(total) * barLength;
		const int rightPercent = barLength - leftPercent;
		string print_buffer = "\r[";

		for (int i = 0; i <  leftPercent-1; i++)        print_buffer.append("=");
		print_buffer.append(">");
		for (int i = 0; i <  rightPercent; i++)        print_buffer.append(" ");

		print_buffer.append(string("] ") + std::to_string(current) + string("/") + std::to_string(total));

		if (current == total) print_buffer.append("\n");
		printf(print_buffer.c_str());

}

bool Utils::IsFileExist(string filename) {
	ifstream reader;
	bool result;

	reader.open(filename);
	result = reader.is_open();
	reader.close();

	return result;
	
}

void Utils::CopyFiles(string source, string target) {

    //RunCommand("robocopy " + source + " " + target + " /is /e /MT:8");
	RunCommand("robocopy \"" + source + "\" \"" + target + "\" /e /MT:8 /NFL /NDL /NJH /NJS /nc /ns /np");
}

void Utils::Report(MsgType type, string message) {
	
	if (!visualizing) {return;}

	switch (type) {
	case INFO: 
		printf("[INFO]    %s\n",message.c_str());
		break;
	case ERR:  
		printf("[ERROR]   %s\n", message.c_str());
		break;
	case WARNING :
		printf("[WARNING] %s\n", message.c_str());
		break;
	case PLAIN :
		printf(message.c_str());
	}
}

bool Utils::WriteToFile(string filename, vector<string> content) {
	ofstream writer;
	writer.open(filename);
	
	if (!writer.is_open()) return false;
	
	for (string current_line : content) {
		writer << current_line + "\n";
	}

	writer.close();
	return true;
}

void Utils::SetVisualizing(bool value) {
	visualizing = value;
}