if(mn=="624+A") mn="624A";
if(mn=="724UP+A") mn="724UPA";
pctc="wcase.gif WIDTH=135 HEIGHT=85"
if(mn=="604UP") pctc="bscase.jpg WIDTH=100 HEIGHT=72"
if(mn=="707UP") pctc="bmcase.gif WIDTH=102 HEIGHT=58"
img=mn.toLowerCase()
tbk="g"
timg="stwr"
mimg="m"+img
if((mn=="604UP")||(mn=="707UP"))
{
timg="t"+img+".gif><BR><IMG WIDTH=1 HEIGHT=17 SRC=/sp"
mimg="sp"
tbk="tbk"
}
function showImg(page)
{
if(page=="main"){
document.write("<TABLE WIDTH=765 HEIGHT=95 BORDER=0 CELLPADDING=0 CELLSPACING=0 BACKGROUND=/"+tbk+".gif><TR><TD WIDTH=360 HEIGHT=95><IMG SRC=/sp.gif WIDTH=360 HEIGHT=95 BORDER=0 USEMAP=#m_l1><MAP NAME=m_l1><AREA SHAPE=rect COORDS='18,8,152,58' HREF=http://www.dlink.com TARGET=_blank></MAP></TD>")
document.write("<TD NOWRAP VALIGN=BOTTOM ALIGN=CENTER WIDTH=340 HEIGHT=95><IMG SRC=/"+timg+".gif><BR><IMG WIDTH=1 HEIGHT=10 SRC=/sp.gif></TD><TD><IMG SRC=/sp.gif WIDTH=65 HEIGHT=95></TD></TR></TABLE>")
}else
document.write("<TR><TD VALIGN=TOP ALIGN=RIGHT BACKGROUND=/wbk.gif HEIGHT=34><IMG SRC=/b"+img+".gif><IMG SRC=/sp.gif WIDTH=157 HEIGHT=31></TD></TR>")
}
function showpct()
{
document.write("<BR><TABLE BORDER=0 ALIGN=CENTER><TR><TD BACKGROUND=/"+pctc+" VALIGN=TOP><IMG SRC=/"+mimg+".gif></TD></TR></TABLE>")
}
var SelRow
function GURL(x){var t=new Date();location=x+'&ZT='+t.getTime();}
function makesure(p,l){if(confirm(p))GURL(l);}
function sa(s){alert(s)}
function prn_err(c){
if(c=="A")sa("无法连线到网络时间服务器!")
if(c=="B")sa("与网络时间服务器联系失败!")
if(c=="X")sa("输入值错误!")
if(c=="C")sa("网络时间服务器无反应!")
if(c=="Y")sa("设定系统时间失败!")
}
var week=new Array("星期日","星期一","星期二","星期三","星期四","星期五","星期六")
var lt=new Array("A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U")
function pr_1_schd(id,schd){
j1=id*7
if(schd[j1]==0)document.write("任何时间")
else document.write(schd[j1+3]+":"+schd[j1+5]+" - "+schd[j1+4]+":"+schd[j1+6]+", "+week[parseInt(schd[j1+1])]+" - "+week[parseInt(schd[j1+2])])
document.write("</TD><TD><A HREF='javascript:doedit("+id+")'><IMG BORDER=0 SRC=/e.gif></A>&nbsp;<A HREF='javascript:delchk("+id+")'><IMG BORDER=0 SRC=/d.gif></A></TD>")
document.write("</TR>")
}
function pr_2_schd(id,schd){
j1=id*7
var sch="";
if(schd[j1]==0) sch="任何时间";
else sch=schd[j1+3]+":"+schd[j1+5]+" - "+schd[j1+4]+":"+schd[j1+6]+", "+week[parseInt(schd[j1+1])]+" - "+week[parseInt(schd[j1+2])];
return sch;
}
function pre_1_schd(id,schd){
val=new String;
j1=id*7
if(schd[j1]=="0") val="99";
else val=String(schd[j1+1])+String(schd[j1+3])+String(schd[j1+5])+"-"+String(schd[j1+2])+String(schd[j1+4])+String(schd[j1+6]);
document.write("<INPUT TYPE=HIDDEN NAME=SVC"+lt[id]+" VALUE='"+val+"'>\n")
}
function pre_schd_input(){
s1=op(0,12)
s2=op(0,23)
s3=op(0,55,5)+"</SELECT>";
var w="";
for(i=0;i<7;i++) w=w+"<OPTION VALUE="+i+">"+week[i];
document.write(
"<TABLE WIDTH=99% >"+
"<TR><TD WIDTH=20%>排程</TD>"+
"<TD WIDTH=15% NOWRAP><INPUT TYPE=RADIO NAME=zsvt VALUE='1'>任何时间</TD>"+
"<TD WIDTH=65%>&nbsp;</TD>"+
"</TR>"+
"<TR><TD>&nbsp;</TD>"+
"<TD><INPUT TYPE=RADIO NAME=zsvt VALUE='0'>从</TD>"+
"<TD>每个 <SELECT NAME=zsvh>"+s2+"</SELECT>:"+
"<SELECT NAME=zsvm>"+s3+"到 <SELECT NAME=zsvhh>"+s2+"</SELECT>:"+
"<SELECT NAME=zsvmm>"+s3+"</TD>"+
"</TR>"+
"<TR><TD>&nbsp;</TD><TD>&nbsp;</TD>"+
"<TD>日期 <SELECT NAME=zsvd>"+w+"</SELECT>"+
" 到 <SELECT NAME=zsvdd>"+w+"</SELECT>"+
"</TD>"+
"</TR></TABLE>")
}
function validate_schd(f,ix){
var fd,td,fh,fm,th,tm,xx
if(f.zsvt[1].checked!=true){
f.elements[ix].value="99";
}
else {
var val=new String;
fd=f.zsvd.selectedIndex
fh=f.zsvh.selectedIndex
fm=f.zsvm.selectedIndex*5
if(fm<10) ffm="0"+fm
else ffm=fm
td=f.zsvdd.selectedIndex
th=f.zsvhh.selectedIndex
tm=f.zsvmm.selectedIndex*5
if(tm<10) ttm="0"+tm
else ttm=tm
val =String(fd)+f.zsvh.options[fh].text+ffm
val+="-"+String(td)+f.zsvhh.options[th].text+ttm
f.elements[ix].value=val;
}
return true
}
function disp_schd(f,schd,ix){
if(schd[ix]=="1") {
f.zsvt[1].checked=true;
f.zsvd.selectedIndex =schd[ix+1]
f.zsvdd.selectedIndex=schd[ix+2]
f.zsvh.selectedIndex =schd[ix+3]
f.zsvhh.selectedIndex=schd[ix+4]
f.zsvm.selectedIndex =schd[ix+5]/5
f.zsvmm.selectedIndex=schd[ix+6]/5
}
else f.zsvt[0].checked=true;
}
function SelectRow(row){
if(SelRow!=row){
var bar=document.getElementById('tab1')
if(SelRow>=0)bar.rows[SelRow].style.backgroundColor=SelRowBC
if(row>=0){
SelRowBC=bar.rows[row].style.backgroundColor
bar.rows[row].style.backgroundColor="#FFFF00"
}
SelRow=row
}
}
function ag(str){
sa(str)
history.go(-1)
}
function smenu(bg,link,img){
if(link.indexOf('.')==-1)link="/"+link+".htm"
document.write("<TR><TH WIDTH=91 HEIGHT=37 BACKGROUND=/"+bg+".gif><A HREF="+link+"><IMG SRC=/"+img+".gif BORDER=0></A></TH></TR>")
}
function IWcheck(){
if(document.f._IW0.value==document.f._IW1.value)
{
document.f.IW0.value=(document.f._IW0.value=="**********")?"":document.f._IW0.value
t=0;
}
else
{
sa("密码不一致!");
t++
}
}
function sMAC(j)
{
var MAC=""
for(s=0;s<6;s++){
var mac_str=document.f.elements[s+j+1].value;
mac_str=mac_str.toUpperCase();
if(mac_str.length==1) mac_str="0"+mac_str;
if(check_hex(mac_str))
{
MAC+=mac_str;
if(s!=5) MAC+="-"
}
}
return (MAC.length==17)?MAC:""
}
function check_hex(str)
{
var c=0;
if(str.length>1)
{
for(k=0;k<str.length;k++)
{
if(str.charAt(k)!=0)
{
if(isNaN(parseInt('0x'+str.charAt(k))))c++
}
}
return (c==0)?true:false;
}
else
return !isNaN(parseInt('0x'+str))
}
function lMAC(j,mac)
{
a=mac.split("-")
for(i=0;i<6;i++) document.f.elements[i+j+1].value=a[i]
}
var act=new Array(2);
function newID(){
SelectRow(-1);
act[0]=EmpID;
document.forms[0].reset()
document.forms[0]._ID.value=EmpID+1
}
function locate(dest)
{
for(var j=0;j<document.f.elements.length;j++)
{
if(document.f.elements[j].name==dest) break;
}
return j;
}
function FindEmpty(ary,no,cp,ci)
{
if(arguments.length>=3)i=cp
else i=0
if(arguments.length==4)c=ci
else c=""
for(EmpID=0;EmpID<maxc;EmpID++)
{
if(eval(ary+"["+(EmpID*no+i)+"]==c")) break;
}
return EmpID;
}
function cffn(page){
var idx=document.UPG.FN.value.lastIndexOf(".");
var ext=document.UPG.FN.value.substring(idx+1);
if((idx!=-1)&&(ext.toUpperCase()=="BIN"))
{
if(page=="system")
document.forms[1].submit()
else document.forms[0].submit()
return true;
}
else {sa("无效的档案名称!");
}
}
function turn_ASC(str)
{
var ASC_no = new Array("%2B","%2D","%2A","%2F","%40","%5F","%2E")
var ASC_str= new Array("+",  "-",  "*",  "/",  "@",  "_",  ".")
var ss="";
for(i=0;i<str.length;i++)
{
for(j=0;j<ASC_str.length;j++)
{
if(str.substring(i,i+1)==ASC_str[j])
{
ss+=ASC_no[j]
break;
}
}
if(j==ASC_str.length) ss+=escape(str.substring(i,i+1))
}
return ss
}
function klvv(cskm,zsq,zs0,zs1,zs2,zs3)
{
var WEPv=new Array(zs0,zs1,zs2,zs3)
var klv=VV1="";
var ktype=0;
var klen=new Array("0","10","26","58")
if(cskm=="1")
{
for(i=0;i<WEPv.length;i++)
{
if(WEPv[i]=="CHECKED") break;
}
if(i==3)WEP=4;
else WEP=i
klv=klen[i];
if(zsq>128){ ktype=1;klv/=2;}
VV1="10"+ktype+WEP;
if(klv==0)klv="";
}
if(cskm=="1CHECKED")VV1="2000"
if(cskm=="1CHECKED")VV1="2000"
if(cskm=="2CHECKED")VV1="8000"
if(cskm=="3CHECKED")VV1="8000"
if(cskm=="2")VV1="4000"
if(cskm=="3")VV1="4000"
return  "rc="+klv+"&rf="+VV1
}
function PPPoEType(c)
{
document.forms[0].II0.disabled=c
document.forms[0].ID00.disabled=c
document.forms[0].ID01.disabled=c
}
function L2TPType(c)
{
if(c)
{
document.forms[0].WI.value="0.0.0.0"
document.forms[0].WN.value="0.0.0.0"
document.forms[0].WG.value="0.0.0.0"
}
document.forms[0].WI.disabled=c
document.forms[0].WN.disabled=c
document.forms[0].WG.disabled=c
}
function PPTPType(c)
{
if(c)
{
document.forms[0].WI.value="0.0.0.0"
document.forms[0].WN.value="0.0.0.0"
document.forms[0].WG.value="0.0.0.0"
}
document.forms[0].WI.disabled=c
document.forms[0].WN.disabled=c
document.forms[0].WG.disabled=c
}
