#pragma once

#include "includes.h"
#include "Utils.h"

struct FileInfo {
    string prePath;
    string postPath;
};

class WMLKit {
private:
	void AddFile(FileInfo i);
	string RemoveVersionNumber(string originPath);
public:
	bool visualizing = true;
	vector<FileInfo> file_list;
	void ReadFileList(string filemapPath);
	void MountCustomMods();
	void MountDuoWanMods();
	
};