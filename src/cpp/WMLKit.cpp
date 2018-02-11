#include "WMLKit.h"

ifstream reader;
ofstream writer;

void WMLKit::AddFile(FileInfo i) {
	file_list.push_back(i);
}

void WMLKit::ReadFileList(string filemapPath) {
	string read_buffer;
	FileInfo file_info_buffer;
	reader.open(filemapPath.c_str());
	
	while (getline(reader, read_buffer)) {
		
		if (read_buffer.find("</Mapping>") != string::npos) {
			AddFile(file_info_buffer);
			file_info_buffer = {"",""};
			continue;
		}
		else if (read_buffer.find("SourceFile") != string::npos) {
			int startPosition = read_buffer.find_first_of('"') + 1;
			int endPosition = read_buffer.find_last_of('"') - 1;

			for (int i = startPosition; i <= endPosition; ++i) {
				if (read_buffer[i] == '/') {
					file_info_buffer.prePath += R"(\)";
					continue;
				}
				file_info_buffer.prePath += read_buffer[i];
			}
		}
		else if (read_buffer.find("DestFile") != string::npos) {
			int startPosition = read_buffer.find_first_of("<DestFile>") + 10;
			int endPosition = read_buffer.find_last_of("</DestFile>") - 11;
			
			for (int i = startPosition; i <= endPosition; ++i) {
				if (read_buffer[i] == '/') {
					file_info_buffer.postPath += R"(\)";
					continue;
				}
				file_info_buffer.postPath += read_buffer[i];
			}
		}		
	}
	reader.close();
}

void WMLKit::MountCustomMods() {
	vector<string> folder_list;
	Utils::GetFolderList(string(R"(.\res_custom)"), folder_list);
	string version = Utils::GetWowsVersion();

	const int totalModCount = folder_list.size();

	if(this->visualizing) printf("Mounting Custom Mods...\n");

	for (int i = 0; i < totalModCount; ++i) {

		if(this->visualizing){
			Utils::PercentageBar(i+1, totalModCount);
		}

		Utils::RunCommand(R"(echo d|xcopy ".\res_custom\)" + folder_list[i] + R"(" /E /Y ".\res_mods\)" + version + R"(")");
	}
	if(this->visualizing) printf("\n");
}

void WMLKit::MountDuoWanMods() {
	
	ReadFileList(R"(.\res_wsp\filemap.xml)");

	const int totalModCount = file_list.size();

	const string versionNumber = Utils::GetWowsVersion();

	if(this->visualizing) printf("Mounting Duowan Box Mods...\n");

	for (int i = 0; i < totalModCount; ++i) {
		
		if (this->visualizing) {
			Utils::PercentageBar(i+1, totalModCount);
		}

		Utils::RunCommand(R"(echo f|xcopy "res_wsp\)"
		
		+file_list[i].postPath + R"(" /E /Y "res_mods\)"
		+ versionNumber 
		+ R"(\)" 
		+ RemoveVersionNumber(file_list[i].prePath)
		+ R"(")");
	}
	if(this->visualizing) printf("\n");
}

string WMLKit::RemoveVersionNumber(const string originPath) {

	string returnBuffer;
	int startPosition = originPath.find_first_of(R"(\)") + 1;
	int endPosition   = originPath.size() - 1;

	for(int i = startPosition; i <= endPosition; ++i) {
		returnBuffer += originPath[i];
	}

	return returnBuffer;
}