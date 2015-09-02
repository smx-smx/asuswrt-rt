#!/bin/sh
# ASUS app stop script


tcapi set Apps_Entry apps_state_stop 0 # INITIALIZING
tcapi set Apps_Entry apps_state_error 0
APPS_INSTALL_FOLDER=`tcapi get Apps_Entry apps_install_folder`
SWAP_ENABLE=`tcapi get Apps_Entry apps_swap_enable`
SWAP_FILE=`tcapi get Apps_Entry apps_swap_file`
APPS_MOUNTED_PATH=`tcapi get Apps_Entry apps_mounted_path`

tcapi set Apps_Entry apps_state_stop 1 # STOPPING
# stop all APPs by order.
app_init_run.sh allpkg stop


tcapi set Apps_Entry apps_state_stop 2 # REMOVING_SWAP
if [ "$SWAP_ENABLE" = "1" ] && [ -f "$APPS_MOUNTED_PATH/$APPS_INSTALL_FOLDER/$SWAP_FILE" ]; then
	swapoff $APPS_MOUNTED_PATH/$APPS_INSTALL_FOLDER/$SWAP_FILE
	rm -rf $APPS_MOUNTED_PATH/$APPS_INSTALL_FOLDER/$SWAP_FILE
fi


tcapi set Apps_Entry apps_state_stop 3 # FINISHED
