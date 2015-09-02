#!/bin/sh
echo -e "Content-Type: text/html\n"
echo -e "<html><head>\n<meta http-equiv=Content-Script-Type content=text/javascript>\n<meta http-equiv=Content-Style-Type content=text/css>\n<meta http-equiv=Content-Type content=\"text/html; charset=iso-8859-1\">\n<link rel=\"stylesheet\" href=\"/style.css\" type=\"text/css\" tppabs=\"http://192.168.1.1/css/style.css\"></head>"
echo -e "<body>\n<FORM METHOD=\"POST\" ACTION=\"/cgi-bin/status_log.cgi\" name=\"StatusLogForm\">\n<table width=\"760\" border=\"0\" align=center cellpadding=\"0\" cellspacing=\"0\" bgcolor=\"#FFFFFF\">\n<tr>\n<td height=\"5\" class=\"light-orange\" colspan=\"5\">&nbsp;</td></tr><tr>\n<td width=\"150\" height=\"30\" class=\"title-main\">\n<font color=\"#FFFFFF\">System Log</font></td><td width=\"10\" class=\"black\">&nbsp;</td><td width=\"150\"></td><td width=\"10\" ></td><td width=\"440\"></td></tr><tr>"
echo -e "<td class=\"light-orange\">&nbsp;</td><td class=\"light-orange\"></td><td colspan=\"3\" class=\"tabdata\">&nbsp;</td></tr><tr>\n<td class=\"light-orange\">&nbsp;</td><td class=\"light-orange\">&nbsp;</td><td colspan=\"3\" class=\"tabdata\">\n<TEXTAREA NAME=\"AlphaLogDisplay\" ROWS=\"25\" COLS=\"55\" WRAP=\"ON\" EDIT=\"OFF\" READONLY>"
LogFlag=`/userfs/bin/tcapi get Syslog_Entry Log_Flag`
LogPath=`/userfs/bin/tcapi get Syslog_Entry Log_Path`
`/userfs/bin/tcapi set Syslog_Entry Log_Flag 0`
if [ "$LogPath" = "no attribute information" -o "$LogPath" = "no node information" ];
then
	LogPath="/tmp/var/log/messages"
fi
if [ $LogFlag = 1 ]; then
	cat /dev/null >$LogPath
fi
cat $LogPath
echo -e "</TEXTAREA>\n</td></tr><tr><td class=\"light-orange\">&nbsp;</td><td class=\"light-orange\"></td>\n<td>&nbsp;</td><td>&nbsp;</td><td>&nbsp;</td></tr></table><table width=\"760\" border=\"0\" align=center cellpadding=\"0\" cellspacing=\"0\" bgcolor=\"#FFFFFF\">\n<tr>\n<td width=\"160\" class=\"black\" height=\"42\" >&nbsp;</td><td width=\"160\" class=\"orange\">&nbsp;</td><td width=\"440\" class=\"orange\">&nbsp;\n<INPUT Name=\"bt_refresh\" type=\"button\" value=\"REFRESH\" onClick=\"javascript:window.location='/cgi-bin/status_log.cgi'\"></td></tr></table></form>\n</body></html>"
