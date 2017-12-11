#!/bin/sh
echo -e "Content-Type: text/html\n"
echo -e "<html><head>\n<meta http-equiv=Content-Script-Type content=text/javascript>\n<meta http-equiv=Content-Style-Type content=text/css>\n<meta http-equiv=Content-Type content=\"text/html; charset=iso-8859-1\">\n<style>body{margin-right: -15px !important;margin-right: 0px !important;margin-right: 0px}</style>\n"
echo -e "<script>function reSize(){parent.document.getElementById('status_iframe').height=document.body.scrollHeight;}</script>"
echo -e "</head>"
echo -e "<body onload=\"reSize();\" BGCOLOR=\"#4D595D\">\n<textarea cols='63' rows='27' wrap='off' readonly='readonly' id='textarea' style=\"width:98%; font-family:'Courier New', Courier, mono; font-size:13px;background:#475A5F;color:#FFFFFF;\">"
/sbin/route -n
echo -e "</TEXTAREA>\n</body></html>"
