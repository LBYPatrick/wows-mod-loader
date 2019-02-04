#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <cstring>

#ifdef _WIN32

#define popen _popen
#define pclose _pclose

#endif

#define READ_BUFFER_SIZE 2048

using namespace std;

struct FileInfo {
	string pre_path;
	string post_path;
};

enum ModMountType {
	DUOWAN_ONLY,
	CUSTOM_ONLY,
	BOTH
};

struct SortItem {
	size_t old_index;
	long long key;
};

struct YAML {
	size_t level;
	string left;
	string right;
};
