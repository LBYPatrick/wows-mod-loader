#include "includes.h"
#include "Utils.h"
#include "WMLKit.h"

int main(int argc,char*const argv[]) {

	const vector<string> paramList = {
		"--silent",
		"-s",
		"--quiet",
		"-q",
		"--duowan-only",
		"-d",
		"--custom-only",
		"-c"
	};
	WMLKit object;
	string param_buffer;
	boolean is_duowan_needed = false;
	boolean is_custom_needed = false;

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
						object.visualizing = false;
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
					default: break;
				}
				if (is_param_match) break;
			}
		}
	}

	if(!is_duowan_needed && !is_custom_needed) {
		Utils::RunCommand("rd /q /s res_mods");
		is_duowan_needed = true;
		is_custom_needed = true;
	}

	if(is_duowan_needed) {
		object.MountDuoWanMods();
	}

	if(is_custom_needed) {
		object.MountCustomMods();
	}

	return 0;
}
