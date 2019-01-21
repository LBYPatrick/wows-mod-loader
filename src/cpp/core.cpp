#include "core.hpp"
#include "util.hpp"

ifstream reader;
ofstream writer;

void core::SetVisualizing(bool is_visualizing) {
	util::SetVisualizing(is_visualizing);
}

vector<FileInfo> core::ReadDuowanFileList(string filemap_path) {
	
	string read_buffer;
	FileInfo info;
	vector<FileInfo> return_buffer;
	
	vector<string> file_buffer = util::ReadFile(filemap_path);

	if (file_buffer.size() == 0) return {};

	for (auto & line : file_buffer) {
		
		if (line.find("</Mapping>") != string::npos) {
			
			return_buffer.push_back(info);
			info = {"",""};
			continue;

		}
		else if (line.find("SourceFile") != string::npos) {

			info.pre_path = util::SubString(line, line.find_first_of('"') + 1, line.find_last_of('"'));

			for (auto & i : info.pre_path) {

				if (i == '/') {
					i = '\\';
				}
			}
		}
		else if (line.find("DestFile") != string::npos) {
			
			info.post_path = util::SubString(line, line.find_first_of("<DestFile>") + 10, line.find_last_of("</DestFile>") - 10);

			for (auto & i : info.post_path) {

				if (i == '/') {
					i = '\\';
				}
			}
		}		
	}

	return return_buffer;
}

void core::MountCustomMods() {

	vector<string> folder_list = util::GetFolderList(R"(.\res_custom)");
	string version = util::GetWowsVersion();

	int count = 0;

	util::Report("INFO", "Mounting Custom Mods...");
	util::Report("", "");

	for (auto & mod : folder_list) {
		++count;
		printf(("\tMounting " + mod + "(" + to_string(count) + " / " + to_string(folder_list.size()) + ")\n").c_str());
		util::CopyFiles(".\\res_custom\\" + mod, ".\\res_mods\\" + version);
	}
	util::Report("","\n");
}

void core::MountDuoWanMods() {
	
	vector<FileInfo> file_list = ReadDuowanFileList(R"(.\\res_wsp\\filemap.xml)");

	size_t mod_count = file_list.size();
	string version = util::GetWowsVersion();
	size_t count = 0;


	if (file_list.size() == 0) {
		util::Report("WARNING", "Duowan mod folder no found or filemap.xml cannot be opened.");
		return;
	}

	for (auto & i : file_list) {
		++count;

		util::Report("INFO", "Mounting Duowan-Box Mods (" + to_string(count) + " / " + to_string(mod_count) + ")",true);
		util::RunCMD(R"(echo f|xcopy "res_wsp\)"

			+ i.post_path + R"(" /E /Y "res_mods\)"
			+ version
			+ R"(\)"
			+ RemoveVersionNumber(i.pre_path)
			+ R"(")");
	}
	util::Report("","\n");
}

bool core::MountMods(ModMountType type) {
	
	switch (type) {
	case DUOWAN_ONLY:
		MountDuoWanMods;
		break;
	case CUSTOM_ONLY:
		MountCustomMods();
		break;
	case BOTH:
		MountDuoWanMods();
		MountCustomMods();
		break;
	}
	return true;
}

string core::RemoveVersionNumber(string full_value) {

	return full_value.substr(full_value.find_first_of(R"(\)") + 1);
}