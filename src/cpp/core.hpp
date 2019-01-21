#pragma once

#include "include.hpp"



namespace core {

	vector<FileInfo> ReadDuowanFileList(string filemap_path);
	string RemoveVersionNumber(string origin_path);
	void SetVisualizing(bool value);
	void MountCustomMods();
	void MountDuoWanMods();
	bool MountMods(ModMountType type);
};