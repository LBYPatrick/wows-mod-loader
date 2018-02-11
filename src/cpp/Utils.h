#pragma once

#include "includes.h"

namespace Utils {
    void   RunCommand(string command);
	void   GetFolderList(string path,vector<string> & folder_list);
	void   RenameFile(string old_name, string new_name);
	string GetWowsVersion();
	string GetRawFileName(string &full_path);
	void   PercentageBar(int current, int total);
};