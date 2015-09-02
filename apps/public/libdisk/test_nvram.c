#include <stdio.h>
#include <bcmnvram.h>

int main(){
	printf("Start to test the nvram:.\n");
	printf("test 1: %s.\n", nvram_get("usb_path1_vvid"));
	printf("test 2: %s.\n", nvram_safe_get("usb_path1_vvid"));

	nvram_set("st_ftp_mode", "1");

	nvram_commit();

	return 0;
}

