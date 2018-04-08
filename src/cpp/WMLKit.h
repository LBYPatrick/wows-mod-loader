#pragma once

#include "includes.h"
#include "Utils.h"

struct FileInfo {
    string prePath;
    string postPath;
};

namespace WMLKit {

	vector<FileInfo> ReadDuowanFileList(string filemap_path);
	string RemoveVersionNumber(string origin_path);
	void SetVisualizing(bool value);
	void MountCustomMods();
	void MountDuoWanMods();
};