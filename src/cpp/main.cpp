#include "includes.h"
#include "Utils.h"
#include "WMLKit.h"

int main(int argc,char*const argv[]) {
	WMLKit object;
	
	Utils::runCMD("rd /q /s res_mods");
	
	if (argv[2] == "--silent" || argv[2] == "-s" || argv[2] == "--quiet" || argv[2] == "-s")
		object.visualizing = false;
	
	object.mountDuoWanMods();
	object.mountCustomMods();
	return 0;
}
