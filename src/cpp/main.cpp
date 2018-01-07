#include "includes.h"
#include "Utils.h"
#include "WMLKit.h"

int main(int argc,char*const argv[]) {
	WMLKit object;
	
	string paramBuffer;

	for(int i = 0; i < argc; ++i) {
		paramBuffer = argv[i];

		if(paramBuffer.find("--silent") != string::npos 
		 ||paramBuffer.find("-s") != string::npos
		 ||paramBuffer.find("--quiet") != string::npos
		 ||paramBuffer.find("-q") != string::npos) {
			object.visualizing = false;
		}
	} 

	Utils::runCMD("rd /q /s res_mods");
	
	object.mountDuoWanMods();
	object.mountCustomMods();
	return 0;
}
