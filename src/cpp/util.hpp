#pragma once

#include "include.hpp"

namespace util {
	
    void   RunCMD(string command);
	vector<string> RunCMD(string cmd, bool is_output_enabled);
	vector<string> GetFolderList(string path);
	void   RenameFile(string old_name, string new_name);
	string GetWowsVersion();
	string GetRawFileName(string full_path);
	void   PercentageBar(int current, int total);
	vector<string> ReadFile(string filename);
	bool   IsFileExist(string filename);
	bool   WriteToFile(string filename, vector<string> & content);
	void Report(string type, string message);
	void Report(string type, string message, bool is_same_line);
	void SetVisualizing(bool value);
	string SubString(string & str, int left, int stop);
	void CopyFiles(string source, string target);
	YAML GetYaml(string line);

	class QuickSort {
	public:

		static void Sort(vector<SortItem> &arr, size_t low, size_t high);
		static vector<size_t> Sort(vector<long long> & arr, size_t low, size_t high);
		static vector<size_t> Sort(vector<long long> & arr);

	private:
		static size_t Partition(vector<SortItem> &arr, size_t low, size_t high);
	};

};