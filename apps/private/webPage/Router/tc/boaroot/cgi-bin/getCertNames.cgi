#!/bin/sh
echo -e "Content-Type: text/html\n"
echo -e "<html><head>\n<meta http-equiv=Content-Script-Type content=text/javascript>\n<meta http-equiv=Content-Style-Type content=text/css>\n<meta http-equiv=Content-Type content=\"text/html; charset=iso-8859-1\">\n<link rel=\"stylesheet\"href=\"/style.css\" type=\"text/css\" tppabs=\"http://192.168.1.1/css/style.css\"><style>body{margin-right: -15px;>margin-right: 0px;margin-right: 0px}</style></head><script language=\"JavaScript\">function OnCertFileSelected(dropdown) { top.main.document.Alpha_WAN.wan_certificate.value=dropdown.options[dropdown.selectedIndex].value;}</script>"
tcapi_path=../../userfs/bin/
cur_pvc=`"$tcapi_path"tcapi get WebCurSet_Entry wan_pvc`
node_name="Wan_PVC"$cur_pvc
Selected_file=`"$tcapi_path"tcapi get $node_name CERTIFICATE`
echo -e "<body>"
echo -e "<form name=\"Alpha_WAN\" method=\"post\">"
echo -e "<select name=\"wan_cert\" onchange=\"OnCertFileSelected(this.form.wan_cert);\">\n"
for i in `ls ../../usr/etc/802_1X/Certificates/`
do                                      
if [ "$Selected_file" = "$i" ]; then
	echo -e "<option selected value=\"$i\">$i</option>"
else                                    
  echo -e "<option value=\"$i\">$i</option>"
fi
done
echo -e "</select></body></html>"
