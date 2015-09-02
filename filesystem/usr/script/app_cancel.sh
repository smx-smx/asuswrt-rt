#!/bin/sh
# ASUS app stop script


tcapi set Apps_Entry apps_state_cancel 0 # INITIALIZING
tcapi set Apps_Entry apps_state_error 0
APPS_INSTALL_FOLDER=`tcapi get Apps_Entry apps_install_folder`
SWAP_ENABLE=`tcapi get Apps_Entry apps_swap_enable`
SWAP_FILE=`tcapi get Apps_Entry apps_swap_file`
APPS_MOUNTED_PATH=`tcapi get Apps_Entry apps_mounted_path`

tcapi set Apps_Entry apps_state_cancel 1 # STOPPING
# stop sh.
# killall -SIGTERM sh
# killall -SIGKILL sh

tcapi set Apps_Entry apps_state_cancel 2 # FINISHED
