#pragma once

#include "includes.h"

namespace Utils {
    void   runCMD(string command);
	void   getFolderList(string path,vector<string> & folderList);
	void   renameFile(string pre, string post);
	string getWOWSversion();
	string getRawFileName(string &fullPath);
};