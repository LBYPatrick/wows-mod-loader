#include "includes.h"
#include "Utils.h"
#include "WMLKit.h"

int main(int argc,char*const argv[]) {
	WMLKit object;
	Utils::runCMD("rd /q /s res_mods");
	object.mountDuoWanMods();
	object.mountCustomMods();
	return 0;
}
