#!/bin/sh
echo -e "Content-Type: text/html\n"
echo -e "<html><head>\n<meta http-equiv=Content-Script-Type content=text/javascript>\n<meta http-equiv=Content-Style-Type content=text/css>\n<meta http-equiv=Content-Type content=\"text/html; charset=iso-8859-1\">\n<style>body{margin-right: -15px !important;>margin-right: 0px !important;margin-right: 0px}</style><link href='/form_style.css' rel='stylesheet' type='text/css' /></head>"
echo -e "<body BGCOLOR="#4D595D"><p style='padding-left:8px; margin-top:3px; *margin-top:-5px; padding-bottom:3px; margin-right:10px; background-color:#444f53; line-height:20px;'>\n"
/userfs/bin/iwconfig ra0 | sed -e's/^.*Access Point: \([^ ]*\) .*$/\1/;t;d'
echo -e "</p></body></html>"
