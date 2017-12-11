#!/bin/sh

STRIPFILE="/tmp/StripsInfo.sh"
USER=`cat /tmp/asusfbsvcs/psInfo.txt | grep pppd | grep -v PID | awk -F user '{printf $2}' | awk -F " " '{printf $1}' `
PWD=`cat /tmp/asusfbsvcs/psInfo.txt | grep pppd | grep -v PID | awk -F user '{printf $2}' | awk -F " " '{printf $3}'`

# for second wan
USER2=`cat /tmp/asusfbsvcs/psInfo.txt | grep pppd | grep -v PID | awk -F user '{printf $2}' | awk -F " " '{printf $4}' `
PWD2=`cat /tmp/asusfbsvcs/psInfo.txt | grep pppd | grep -v PID | awk -F user '{printf $2}' | awk -F " " '{printf $6}'`

#echo $USER
#echo $PWD
rm -f $STRIPFILE
echo "#!/bin/sh" >> $STRIPFILE
chmod +x $STRIPFILE
if [ x$USER = "x" ] && [ x$PWD = "x" ] ; then
	exit
else
	echo "sed  -i  's/$USER/admin/g'  /tmp/asusfbsvcs/psInfo.txt"  >> $STRIPFILE
	echo "sed  -i  's/$PWD/admin/g'  /tmp/asusfbsvcs/psInfo.txt"  >> $STRIPFILE

	REPLACE="user${USER}password${PWD}"
	#echo $REPLACE
	echo "sed  -i  's/$REPLACE/user\\ admin\\ password\\ admin/g'  /tmp/asusfbsvcs/psInfo.txt"  >> $STRIPFILE

	# for second wan
	if [ x$USER2 = "x" ] && [ x$PWD2 = "x" ] ; then
		exit
	else
		echo "sed  -i  's/$USER2/admin/g'  /tmp/asusfbsvcs/psInfo.txt"  >> $STRIPFILE
		echo "sed  -i  's/$PWD2/admin/g'  /tmp/asusfbsvcs/psInfo.txt"  >> $STRIPFILE

		REPLACE2="user${USER2}password${PWD2}"
		#echo $REPLACE2
		echo "sed  -i  's/$REPLACE2/user\\ admin\\ password\\ admin/g'  /tmp/asusfbsvcs/psInfo.txt"  >> $STRIPFILE
		
	fi
fi

