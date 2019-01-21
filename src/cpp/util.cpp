#include "util.hpp"
#include "include.hpp"

bool visualizing = true;

vector<string> util::ReadFile(string path) {

	FILE * handler;
	vector<string> r;
	string long_buffer;
	char buffer[READ_BUFFER_SIZE];

	//Convert Unix backslashes to Windows slashes
	for (auto & i : path) {
		if (i == '/') {
			i = '\\';
		}
	}
#ifdef _WIN32 

	auto result = fopen_s(&handler, path.c_str(), "r");

	if (result == EINVAL) {
		return r;
	}
#elif
	handler = fopen(path.c_str(), 'r');

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

	//Convert Unix backslashes to Windows slashes
	for (auto & i : path) {
		if (i == '/') {
			i = '\\';
		}
	}

#ifdef _WIN32 

	auto result = fopen_s(&handler, path.c_str(), "r");

	if (result == EINVAL) {
		return false;
	}
#elif
	handler = fopen(path.c_str(), 'r');

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

	//Convert Unix backslashes to Windows slashes
	for (auto & i : path) {
		if (i == '/') {
			i = '\\';
		}
	}

#ifdef _WIN32

	auto result = fopen_s(&handler, path.c_str(), "w");

	if (result == EINVAL) return false; //Fail to open file
#elif

	handler = fopen(filename.c_str(), "w");

	if (handler) return false; //Fail to open file
#endif

	string write_buffer;

	for (auto & i : content) {
		write_buffer += i;
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