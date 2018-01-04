#pragma once

#include "includes.h"
#include "Utils.h"

struct FileInfo {
    string prePath;
    string postPath;
};

class WMLKit {
private:
	void addFile(FileInfo i);

public:
	vector<FileInfo> fileList;
	void readFileList(string filemapPath);
	void mountCustomMods();
	void mountDuoWanMods();
};