#!/bin/sh
echo -e "Content-Type: text/html\n"
echo -e "<html><head>\n<meta http-equiv=Content-Script-Type content=text/javascript>\n<meta http-equiv=Content-Style-Type content=text/css>\n<meta http-equiv=Content-Type content=\"text/html; charset=iso-8859-1\">\n<link rel=\"stylesheet\" href=\"/style.css\" type=\"text/css\" tppabs=\"http://192.168.1.1/css/style.css\"><style>body{margin-right: -15px !important;>margin-right: 0px !important;margin-right: 0px}</style></head>"
echo -e "<script language=\"JavaScript\">\nfunction back_syslog()\n{\nvar cfg = '/syslog.xlog';\nvar code = 'location.assign(\"' + cfg + '\")';\neval(code);\n}\n</script>"
echo -e "<body>\n<FORM METHOD=\"POST\" ACTION=\"/cgi-bin/status_log2.cgi\" name=\"StatusLogForm\">\n<table width=\"760\" border=\"0\" align=center cellpadding=\"0\" cellspacing=\"0\" bgcolor=\"#FFFFFF\">\n<tr>\n<td height=\"5\" class=\"light-orange\" colspan=\"5\">&nbsp;</td></tr><tr>\n<td width=\"150\" height=\"30\" class=\"title-main\">\n<font color=\"#FFFFFF\">System Log</font></td><td width=\"10\" class=\"black\">&nbsp;</td><td width=\"150\"></td><td width=\"10\" ></td><td width=\"440\"></td></tr><tr>"
echo -e "<td class=\"light-orange\">&nbsp;</td><td class=\"light-orange\"></td><td colspan=\"3\" class=\"tabdata\">&nbsp;</td></tr><tr>\n<td class=\"light-orange\">&nbsp;</td><td class=\"light-orange\">&nbsp;</td><td colspan=\"3\" class=\"tabdata\">\n<TEXTAREA NAME=\"AlphaLogDisplay\" ROWS=\"25\" COLS=\"55\" WRAP=\"ON\" EDIT=\"OFF\" READONLY>"
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
echo -e "</TEXTAREA>\n</td></tr><tr><td class=\"light-orange\">&nbsp;</td><td class=\"light-orange\"></td>\n<td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr></table><table width=\"760\" border=\"0\" align=center cellpadding=\"0\" cellspacing=\"0\" bgcolor=\"#FFFFFF\">\n<tr>\n<td width=\"160\" class=\"black\" height=\"42\" >&nbsp;</td><td width=\"160\" class=\"orange\">&nbsp;</td><td width=\"440\" class=\"orange\"> \n<INPUT Name=\"bt_refresh\" type=\"button\" value=\"REFRESH\" onClick=\"javascript:window.location='/cgi-bin/status_log2.cgi'\"><INPUT Name=\"bt_savelog\" type=\"button\" value=\"SaveLog\" onClick=\"back_syslog()\"></td></tr></table></form>\n</body></html>"
