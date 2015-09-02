#!/bin/sh
sed -i 's/\^M//g' /var/log/currLogFile
echo -e "Content-Type: text/html\n"
echo -e "<html><head>\n<meta http-equiv=Content-Script-Type content=text/javascript>\n<meta http-equiv=Content-Style-Type content=text/css>\n<meta http-equiv=Content-Type content=\"text/html; charset=iso-8859-1\">\n<style>body{margin-right: -15px !important;>margin-right: 0px !important;margin-right: 0px}</style></head>"
echo -e "<body BGCOLOR="#4D595D">\n<textarea cols='63' rows='27' wrap='off' readonly='readonly' id='textarea' style=\"width:98%; font-family:'Courier New', Courier, mono; font-size:13px;background:#475A5F;color:#FFFFFF;\">"
loglevel=`/userfs/bin/tcapi get Syslog_Entry DisplayLevel`
	case "$loglevel" in
	"0" )
		LogFileParser 0;;
	esac

	case "$loglevel" in
	"1" )
		LogFileParser 1;;
	esac

	case "$loglevel" in
	"2" )
		LogFileParser 2;;
	esac

	case "$loglevel" in
	"3" )
		LogFileParser 3;;
	esac

	case "$loglevel" in
	"4" )
		LogFileParser 4;;
	esac

	case "$loglevel" in
	"5" )
		LogFileParser 5;;
	esac

	case "$loglevel" in
	"6" )
		LogFileParser 6;;
	esac

	case "$loglevel" in
	"7" )
		LogFileParser 7;;
	esac
echo -e "</TEXTAREA>\n"\
"<script type='text/javascript'>\n"\
"document.getElementById('textarea').scrollTop = document.getElementById('textarea').scrollHeight;\n"\
"</script>\n"\
"</body></html>";
