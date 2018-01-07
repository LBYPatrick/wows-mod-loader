#include "WMLKit.h"

ifstream reader;
ofstream writer;

void WMLKit::addFile(FileInfo i) {
	fileList.push_back(i);
}

void WMLKit::readFileList(string filemapPath) {
	string readBuffer;
	FileInfo fileInfoBuffer;
	reader.open(filemapPath.c_str());
	
	while (getline(reader, readBuffer)) {
		
		if (readBuffer.find("</Mapping>") != string::npos) {
			addFile(fileInfoBuffer);
			fileInfoBuffer = {"",""};
			continue;
		}
		else if (readBuffer.find("SourceFile") != string::npos) {
			int startPosition = readBuffer.find_first_of('"') + 1;
			int endPosition = readBuffer.find_last_of('"') - 1;

			for (int i = startPosition; i <= endPosition; ++i) {
				if (readBuffer[i] == '/') {
					fileInfoBuffer.prePath += R"(\)";
					continue;
				}
				fileInfoBuffer.prePath += readBuffer[i];
			}
		}
		else if (readBuffer.find("DestFile") != string::npos) {
			int startPosition = readBuffer.find_first_of("<DestFile>") + 10;
			int endPosition = readBuffer.find_last_of("</DestFile>") - 11;
			
			for (int i = startPosition; i <= endPosition; ++i) {
				if (readBuffer[i] == '/') {
					fileInfoBuffer.postPath += R"(\)";
					continue;
				}
				fileInfoBuffer.postPath += readBuffer[i];
			}
		}		
	}
	reader.close();
}

void WMLKit::mountCustomMods() {
	vector<string> folderList;
	Utils::getFolderList(string(R"(.\res_custom)"), folderList);
	string version = Utils::getWOWSversion();

	int currentModCount = 0;
	int totalModCount = folderList.size();

	for (string currentFolder : folderList) {
		
		if(this->visualizing)
		Utils::showPercentage(currentModCount, totalModCount, "Mounting Mod: " + currentFolder);

		Utils::runCMD(R"(echo d|xcopy ".\res_custom\)" + currentFolder + R"(" /E /Y ".\res_mods\)" + version + R"(")");

		currentModCount += 1;
	}
}

void WMLKit::mountDuoWanMods() {
	
	readFileList(R"(.\res_wsp\filemap.xml)");

	int currentModCount = 0;
	int totalModCount = fileList.size();

	for (FileInfo currentFile : fileList) {
		
		if (this->visualizing) {
			string message = "Mounting Duowan Box Mods...";
			Utils::showPercentage(currentModCount, totalModCount, message);
		}

		Utils::runCMD(R"(echo f|xcopy "res_wsp\)" + currentFile.postPath + R"(" /E /Y "res_mods\)" + currentFile.prePath + R"(")");
	
		currentModCount++;
	}
}