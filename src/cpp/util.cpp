#include "util.hpp"
#include "include.hpp"

bool visualizing = true;

vector<string> util::ReadFile(string path) {

	FILE * handler;
	vector<string> r;
	string long_buffer;
	char buffer[READ_BUFFER_SIZE];

#ifdef _WIN32 

	//Convert Unix backslashes to Windows slashes
	for (auto & i : path) {
		if (i == '/') {
			i = '\\';
		}
	}

	auto result = fopen_s(&handler, path.c_str(), "r");

	if (result == EINVAL) {
		return r;
	}
#else
	handler = fopen(path.c_str(), "r");

	if (!handler) {
		return r;
	}
#endif

	while (fgets(buffer, READ_BUFFER_SIZE, handler)) {

		if (strlen(buffer) == READ_BUFFER_SIZE && buffer[READ_BUFFER_SIZE - 1] != '\n') {
			long_buffer += buffer;
		}
		else if (buffer[strlen(buffer) - 1] == '\n') {
			r.push_back(long_buffer + string(buffer).substr(0, strlen(buffer) - 1));
			long_buffer = "";
		}
		else {
			r.push_back(long_buffer + string(buffer));
			long_buffer = "";
		}
	}

	r.push_back(long_buffer);
	fclose(handler);

	return r;
}

string util::GetRawFileName(string path) {
	size_t start = 0;
	size_t end = path.length();

	for (auto & i : path) {
		if (i == '/') {
			i = '\\';
		}
	}

	size_t slash_pos = path.find_last_of('\\');

	if (slash_pos != string::npos) {
		start = slash_pos + 1;
	}

	return SubString(path, start, end);
}

//From CSDN, rewritten for a reverse usage (originally for iterate files)
vector<string> util::GetFolderList(string path) {

	vector<string> output = util::RunCMD("dir " + path + " /A:D /B", true);

	output.pop_back();

	return output;
}

vector<string> util::RunCMD(string cmd, bool is_output_enabled) {
	FILE * handler;
	vector<string> r;
	string long_buffer;
	char buffer[READ_BUFFER_SIZE];

	handler = popen(cmd.c_str(), "r");

	if (handler && is_output_enabled) {
		while (fgets(buffer, READ_BUFFER_SIZE, handler)) {

			if (strlen(buffer) == READ_BUFFER_SIZE && buffer[READ_BUFFER_SIZE - 1] != '\n') {
				long_buffer += buffer;
			}
			else if (buffer[strlen(buffer) - 1] == '\n') {
				r.push_back(long_buffer + string(buffer).substr(0, strlen(buffer) - 1));
				long_buffer = "";
			}
			else {
				r.push_back(long_buffer + string(buffer));
				long_buffer = "";
			}
		}
		r.push_back(long_buffer);
	}
	pclose(handler);

	return r;
}

void util::RunCMD(string cmd) {
	RunCMD(cmd, false);
}

string util::GetWowsVersion() {

	vector<string> file = ReadFile("./paths.xml");

	for (auto & line : file) {
		if (line.find("<Path>res_mods") != -1) {
			return SubString(line,
				line.find_first_of("res_mods/") + 9,
				line.find_last_of("</Path>") - 6);
		}
	}
	return string();
}

void util::RenameFile(string old_name, string new_name) {
	RunCMD("ren \"" + old_name + "\" " + GetRawFileName(new_name));
}

void util::PercentageBar(int current, int total) {

	if (!visualizing) return;

	int length = 50;
	int dots = double(current) / double(total) * length;
	int blanks = length - dots;
	string print_buf = "\r[";

	for (int i = 0; i < dots - 1; i++) {
		print_buf += '=';
	}

	print_buf += '>';

	for (int i = 0; i < blanks; i++) {
		print_buf += " ";
	}

	print_buf += ("] " + to_string(current) + '/' + to_string(total));

	printf(print_buf.c_str());
}

bool util::IsFileExist(string path) {

	FILE * handler;
	vector<string> r;
	string long_buffer;
	char buffer[READ_BUFFER_SIZE];

#ifdef _WIN32 

		//Convert Unix backslashes to Windows slashes
	for (auto & i : path) {
		if (i == '/') {
			i = '\\';
		}
	}

	auto result = fopen_s(&handler, path.c_str(), "r");

	if (result == EINVAL) {
		return false;
	}
#else
	handler = fopen(path.c_str(), "r");

	if (!handler) {
		return false;
	}
#endif

	fclose(handler);
	return true;
}

void util::CopyFiles(string source, string target) {

	//RunCommand("robocopy " + source + " " + target + " /is /e /MT:8");
	RunCMD("robocopy \"" + source + "\" \"" + target + "\" /e /MT:8 /NFL /NDL /NJH /NJS /nc /ns /np");
}

void util::Report(string type, string message) {
	util::Report(type, message, false);
}

void util::Report(string type, string message, bool is_same_line) {

	if (!visualizing) { return; }

	printf((
		(is_same_line ? "\r" : "")
		+ (type.empty() ? "" : ("[" + type + "]"))
		+ message
		+ (is_same_line ? "" : "\n")
		).c_str()
	);
}

bool util::WriteToFile(string path, vector<string> & content) {

	FILE*handler;

#ifdef _WIN32

		//Convert Unix backslashes to Windows slashes
	for (auto & i : path) {
		if (i == '/') {
			i = '\\';
		}
	}

	auto result = fopen_s(&handler, path.c_str(), "w");

	if (result == EINVAL) return false; //Fail to open file
	
#else

	handler = fopen(path.c_str(), "w");

	if (!handler) return false; //Fail to open file
#endif

	string write_buffer;

	for (auto & i : content) {
		write_buffer += i + '\n';
	}

	fputs(write_buffer.c_str(), handler);
	fclose(handler);

	return true;
}

void util::SetVisualizing(bool is_visualizing) {
	visualizing = is_visualizing;
}

string util::SubString(string & str, int left, int stop) {
	size_t length = stop - left;

	//Out-of-bound fix
	if (left < 0) left = 0;
	if (stop > str.length()) stop = str.length();

	return str.substr(left, length);
}

void util::QuickSort::Sort(vector<SortItem> &arr, size_t low, size_t high) {
	if (low < high) {
		size_t pivot = Partition(arr, low, high);
		Sort(arr, low, pivot);
		Sort(arr, pivot + 1, high);
	}
}

/**
 * Overloaded Sort method #1: When user passes a raw size_t array with bounds
 * @param arr
 * @return vector containing indexes of sorted items
 */

vector<size_t> util::QuickSort::Sort(vector<long long> &arr, size_t low, size_t high) {

    auto * new_arr = new vector<SortItem>();

    vector<size_t> r;

    //Reserve space for efficiency
    new_arr->reserve(high - low + 1);
    r.reserve(high - low + 1);

    for (size_t i = low; i <= high; ++i) {
        new_arr->push_back({i, arr[i]});
    }

    Sort(*new_arr, low, high);

    for (auto &element : *new_arr) {
        r.push_back(element.old_index);
    }

    delete new_arr;

    return r;
}

/**
 * Overloaded Sort method #1: When user passes a raw size_t array without bounds
 * @param arr
 * @return vector containing indexes of sorted items
 */
vector<size_t> util::QuickSort::Sort(vector<long long> &arr) {
	return Sort(arr, 0, arr.size() - 1);
}

/**
 * Sort using Hoare Partition Scheme
 * @param arr  Array (By reference)
 * @param low  lower index.
 * @param high High index.
 * @return Index of pivot
 */
size_t util::QuickSort::Partition(vector<SortItem> &arr, size_t low, size_t high) {
	long long pivot = arr[low].key;
	size_t left_index = low - 1,
		right_index = high + 1;

	while (true) {
		do {
			left_index += 1;
		} while (arr[left_index].key < pivot);

		do {
			right_index -= 1;
		} while (arr[right_index].key > pivot);

		if (left_index >= right_index) {
			return right_index;
		}

		std::swap(arr[left_index], arr[right_index]);
	}
}

YAML util::GetYaml(string line) {
	YAML out;

	if (line.find(':') == -1) {
		return YAML();
	}

	out.level = line.find_first_not_of(' ');
	out.left = SubString(line, out.level, line.find(':'));
	out.right = SubString(line, line.find(':') + 1, line.find_last_not_of(' ') + 1);

	//Remove Trailing & starting spaces
	out.left = SubString(out.left, 0, out.left.find_last_not_of(' ') + 1);
	out.right = SubString(out.right, out.right.find_first_not_of(' '), out.right.size());


	if (out.left.find('\"') != -1) {
		out.left = SubString(out.left, 1, out.left.size() - 1);
	}

	if (out.right.find('\"') != -1) {
		out.right = SubString(out.right, 1, out.right.size() - 1);
	}

	return out;
}