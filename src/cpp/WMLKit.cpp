#include "WMLKit.h"

ifstream reader;
ofstream writer;

void WMLKit::SetVisualizing(bool value) {
	Utils::SetVisualizing(value);
}

vector<FileInfo> WMLKit::ReadDuowanFileList(string filemap_path) {
	
	string read_buffer;
	FileInfo file_info_buffer;
	vector<FileInfo> return_buffer;
	
	vector<string> file_buffer = Utils::ReadFile(filemap_path);

	if (file_buffer.size() == 0) return {};

	for (string read_buffer : file_buffer) {
		
		if (read_buffer.find("</Mapping>") != string::npos) {
			
			return_buffer.push_back(file_info_buffer);
			file_info_buffer = {"",""};
			continue;

		}
		else if (read_buffer.find("SourceFile") != string::npos) {

			int start_position = read_buffer.find_first_of('"') + 1;
			int end_position = read_buffer.find_last_of('"') - 1;

			for (int i = start_position; i <= end_position; ++i) {

				if (read_buffer[i] == '/') {
					file_info_buffer.prePath += R"(\)";
					continue;
				}
				file_info_buffer.prePath += read_buffer[i];
			}
		}
		else if (read_buffer.find("DestFile") != string::npos) {
			int startPosition = read_buffer.find_first_of("<DestFile>") + 10;
			int endPosition = read_buffer.find_last_of("</DestFile>") - 11;
			
			for (int i = startPosition; i <= endPosition; ++i) {
				if (read_buffer[i] == '/') {
					file_info_buffer.postPath += R"(\)";
					continue;
				}
				file_info_buffer.postPath += read_buffer[i];
			}
		}		
	}
	reader.close();

	return return_buffer;
}

void WMLKit::MountCustomMods() {

	vector<string> folder_list = Utils::GetFolderList(R"(.\res_custom)");
	string version = Utils::GetWowsVersion();

	//For experimental mode
	vector<string> paths_xml_buffer;
	vector<string> mod_xml_buffer;
	vector<string> output_xml_buffer;

	const int total_mod_count = folder_list.size();

	Utils::Report(MsgType::INFO, "Mounting Custom Mods...\n");

#ifdef EXPERIMENTAL_MODE == true
		//Convert raw mod list to xml for later usage -- I mean, putting them to paths.xml
		for (string current_mod : folder_list) {
			mod_xml_buffer.push_back("\t\t<Path>res_custom/" + current_mod + "</Path>");
		}

		paths_xml_buffer = Utils::IsFileExist("paths.xml.original") ? Utils::ReadFile("paths.xml.original") : Utils::ReadFile("paths.xml");

		if (paths_xml_buffer.size() == 0) {
			Utils::Report(MsgType::ERR, "Cannot read paths.xml");
			return;
		}

		//Make a backup for paths.xml
		if (!Utils::IsFileExist("paths.xml.original")) {

			bool writable = Utils::WriteToFile("paths.xml.original", paths_xml_buffer);

			if (!writable) {
				Utils::Report(MsgType::ERR, "Cannot write to paths.xml");
				return;
			}
		}

		//Start mounting Mods
		for (string current_line : paths_xml_buffer) {

			output_xml_buffer.push_back(current_line);

			if (current_line.find("<Paths>") != string::npos) {

				for (int i = 0; i < mod_xml_buffer.size(); ++i) {

					Utils::PercentageBar(i + 1, mod_xml_buffer.size());
					output_xml_buffer.push_back(mod_xml_buffer[i]);

				}
			}
		}

		//Write to Paths.xml
		Utils::WriteToFile("paths.xml", output_xml_buffer);
#else
	for (int i = 0; i < total_mod_count; ++i) {
		Utils::PercentageBar(i + 1, total_mod_count);
		Utils::CopyFiles(".\\res_custom\\" + folder_list[i], ".\\res_mods\\" + version);
	}
#endif
	Utils::Report(MsgType::PLAIN,"\n");
}

void WMLKit::MountDuoWanMods() {
	
	vector<FileInfo> file_list = ReadDuowanFileList(R"(.\\res_wsp\\filemap.xml)");

	const int totalModCount = file_list.size();
	const string versionNumber = Utils::GetWowsVersion();

	if (file_list.size() == 0) {
		Utils::Report(MsgType::WARNING, "Duowan mod folder no found.");
		return;
	}

	Utils::Report(MsgType::INFO,"Mounting Duowan Box Mods...\n");

	for (int i = 0; i < totalModCount; ++i) {
		
			Utils::PercentageBar(i+1, totalModCount);

		Utils::RunCommand(R"(echo f|xcopy "res_wsp\)"
		
		+file_list[i].postPath + R"(" /E /Y "res_mods\)"
		+ versionNumber 
		+ R"(\)" 
		+ RemoveVersionNumber(file_list[i].prePath)
		+ R"(")");
	}
	Utils::Report(MsgType::PLAIN,"\n");
}

string WMLKit::RemoveVersionNumber(string origin_path) {

	string return_buffer;
	int start_position = origin_path.find_first_of(R"(\)") + 1;
	int end_position   = origin_path.size() - 1;

	for(int i = start_position; i <= end_position; ++i) {
		return_buffer += origin_path[i];
	}

	return return_buffer;
}