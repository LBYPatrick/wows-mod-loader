#include "include.hpp"
#include "util.hpp"
#include "core.hpp"

int main(int argc,char*const argv[]) {

	const vector<string> paramList = {
		"--silent",
		"-s",
		"--quiet",
		"-q",
		"--duowan-only",
		"-d",
		"--custom-only",
		"-c",
		"-f",
		"--full"
	};
	string param_buffer;
	bool is_duowan_needed = false;
	bool is_custom_needed = false;
	bool is_full_load = false;

	for(int i = 0; i < argc; ++i) {
		param_buffer = argv[i];
		bool is_param_match = false;

		for(int n = 0; n < paramList.size(); ++n) {
			if(param_buffer == paramList[n]) {
				switch(n) {
					case 0:
					case 1:
					case 2:
					case 3:
						core::SetVisualizing(false);
					    is_param_match = true;
						break;
					case 4:
					case 5:
						is_duowan_needed = true;
						is_param_match = true;
						break;
					case 6:
					case 7:
						is_custom_needed = true;
						is_param_match = true;
						break;
					case 8:
					case 9:
						is_full_load = true;
						is_param_match = true;

					default: break;
				}
				if (is_param_match) break;
			}
		}
	}

	if (util::GetWowsVersion().size() == 0) {
		util::Report("ERROR", "Failed to get World of Warships version number");
		return 1;
	}

	if(is_full_load) util::RunCMD("rd /q /s res_mods");

	if(!is_duowan_needed && !is_custom_needed) {
		is_duowan_needed = true;
		is_custom_needed = true;
	}

	if(is_duowan_needed) {
		core::MountDuoWanMods();
	}

	if(is_custom_needed) {
		core::MountCustomMods();
	}

	return 0;
}
