#!/bin/sh
# $1: package name.


APPS_INSTALL_FOLDER=`tcapi get Apps_Entry apps_install_folder`
APPS_PATH=/opt
APPS_DEV=`tcapi get Apps_Entry apps_dev`
APPS_MOUNTED_PATH=`tcapi get Apps_Entry apps_mounted_path`
APPS_INSTALL_PATH=$APPS_MOUNTED_PATH/$APPS_INSTALL_FOLDER


tcapi set Apps_Entry apps_state_remove 0 # INITIALIZING
tcapi set Apps_Entry apps_state_error 0
if [ -z "$1" ]; then
	echo "Usage: app_remove.sh <Package name>"
	tcapi set Apps_Entry apps_state_error 1
	exit 1
fi

if [ -z "$APPS_MOUNTED_PATH" ]; then
	echo "Had not mounted yet!"
	tcapi set Apps_Entry apps_state_error 2
	exit 1
fi

pkg_control_file=$APPS_PATH/lib/ipkg/info/$1.control
if [ ! -f "$pkg_control_file" ]; then
	echo "The \"$1\" is not installed yet!"
	tcapi set Apps_Entry apps_state_remove 2 # FINISHED
	exit 0
fi


tcapi set Apps_Entry apps_state_remove 1 # REMOVING
app_set_enabled.sh $1 no

ipkg remove $1
if [ "$?" != "0" ]; then
	tcapi set Apps_Entry apps_state_error 9
	exit 1
fi

APPS_MOUNTED_TYPE=`mount |grep "/dev/$APPS_DEV on " |awk '{print $5}'`
if [ "$APPS_MOUNTED_TYPE" != "vfat" ]; then
	tcapi set Apps_Entry apps_state_remove 2 # FINISHED
	exit 0
fi

objs=`ls -a $APPS_INSTALL_PATH`
for obj in $objs; do
	if [ "$obj" = "." ] || [ "$obj" = ".." ]; then
		continue
	fi

	if [ "$obj" != "bin" ] && [ "$obj" != "lib" ] && [ ! -e "$APPS_PATH/$obj" ]; then
		rm -rf $APPS_INSTALL_PATH/$obj
	fi
done

objs=`ls -a $APPS_INSTALL_PATH/bin`
for obj in $objs; do
	if [ "$obj" = "." ] || [ "$obj" = ".." ]; then
		continue
	fi

	if [ ! -e "$APPS_PATH/bin/$obj" ]; then
		rm -rf $APPS_INSTALL_PATH/bin/$obj
	fi
done

objs=`ls -a $APPS_INSTALL_PATH/lib`
for obj in $objs; do
	if [ "$obj" = "." ] || [ "$obj" = ".." ]; then
		continue
	fi

	if [ ! -e "$APPS_PATH/lib/$obj" ]; then
		rm -rf $APPS_INSTALL_PATH/lib/$obj
	fi
done

app_base_link.sh
if [ "$?" != "0" ]; then
	# apps_state_error was already set by app_base_link.sh.
	exit 1
fi

tcapi set Apps_Entry apps_state_remove 2 # FINISHED
