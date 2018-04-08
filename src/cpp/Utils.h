#pragma once

#include "includes.h"

enum MsgType {
	WARNING = 0,
	ERR = 1,
	INFO = 2,
	PLAIN = 3
};

namespace Utils {
	
    void   RunCommand(string command);
	vector<string> GetFolderList(string path);
	void   RenameFile(string old_name, string new_name);
	string GetWowsVersion();
	string GetRawFileName(string full_path);
	void   PercentageBar(int current, int total);
	vector<string> ReadFile(string filename);
	bool   IsFileExist(string filename);
	bool   WriteToFile(string filename,vector<string> content);
	void   Report(MsgType type, string message);
	void   SetVisualizing(bool value);
	void   CopyFiles(string source, string target);
};