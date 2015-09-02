function dw(str)
{
document.write(str+"\n")
}
function ptz()
{
dw("<select name=TZ style='font-size:7pt'>")
ov="<OPTION VALUE="
dw(ov+"-720>(GMT-12:00)安尼威吐克,瓜加林岛")
dw(ov+"-660>(GMT-11:00)美国的中西部,萨摩亚")
dw(ov+"-600>(GMT-10:00)夏威夷")
dw(ov+"-540>(GMT-09:00)阿拉斯加")
dw(ov+"-480>(GMT-08:00)太平洋标准时间(美国,加拿大)")
dw(ov+"-420>(GMT-07:00)亚利桑那州,山脉标准时间(美国,加拿大)")
dw(ov+"-360>(GMT-06:00)中元标准时间(美国,加拿大),墨西哥,特古西加尔巴(洪都拉斯首都)")
dw(ov+"-300>(GMT-05:00)波哥大,利玛(秘鲁首都),基多(厄瓜多尔尔首都)")
dw(ov+"-300>(GMT-05:00)东方标准时间(美国,加拿大),印第安纳州 (东边)")
dw(ov+"-240>(GMT-04:00)大西洋标准时间(美国,加拿大),卡拉卡斯(委内瑞拉首都),拉巴斯")
dw(ov+"-210>(GMT-03:30)纽芬兰")
dw(ov+"-180>(GMT-03:00)布宜诺斯艾利斯(阿根廷首都),乔治敦,巴西利亚")
dw(ov+"-120>(GMT-02:00)中部-阿根廷")
dw(ov+"-60>(GMT-01:00)亚速尔群岛,德总角.")
dw(ov+"0>(GMT) 卡萨布兰卡市,蒙罗维亚,都柏林,爱丁堡,里斯本(葡萄牙首都),伦敦")
dw(ov+"60>(GMT+01:00)阿姆斯特丹,柏林,伯尔尼,罗马,斯德哥尔摩,维也纳")
dw(ov+"60>(GMT+01:00)贝尔格来德,布拉迪斯拉发,布达佩斯,炉布尔雅那,布拉格")
dw(ov+"60>(GMT+01:00)布鲁塞尔,哥本哈根,马德里,巴黎,维尔纽斯")
dw(ov+"60>(GMT+01:00)萨拉热窝,斯科普里,索非亚,华沙,札格拉布")
dw(ov+"120>(GMT+02:00)雅典,布加莱斯特,开罗,伊斯坦布尔,明斯克")
dw(ov+"120>(GMT+02:00)哈拉雷,赫尔辛基,耶路撒冷,比勒陀利亚,里加,塔林")
dw(ov+"180>(GMT+03:00)莫斯科,圣彼得堡,窝瓦河,巴格达,科威特,利雅德")
dw(ov+"210>(GMT+03:30)德黑兰")
dw(ov+"240>(GMT+04:00)阿布扎比,巴库,马斯喀特,第比利斯")
dw(ov+"270>(GMT+04:30)喀布尔")
dw(ov+"300>(GMT+05:00)凯撒琳镇,伊斯兰堡,克拉啻港市,塔什干")
dw(ov+"330>(GMT+05:30)新德里")
dw(ov+"360>(GMT+06:00)亚斯塔蒂,阿曼蒂,可伦波,狄哈卡")
dw(ov+"420>(GMT+07:00)曼谷,河内,雅加达")
dw(ov+"480>(GMT+08:00)北京,香港,新加坡,台北")
dw(ov+"540>(GMT+09:00)汉城,东京,亚库次客")
dw(ov+"570>(GMT+09:30)爱得莱德,达埃文")
dw(ov+"600>(GMT+10:00)堪培拉,关岛,漠尔斯比港,海  威")
dw(ov+"660>(GMT+11:00)麦哲伦,所罗门岛")
dw(ov+"720>(GMT+12:00)斐济,勘查加半岛,马绍尔群岛,威尔尼顿")
dw("</SELECT>")
}
function op(b,e,d)
{
var o=""
for(i=b;i<=e;i++)
{
o+="<OPTION>"
if(i<10) o+="0"
o+=i
if(arguments.length==3)i=i+d-1
}
return o
}
function dgo(f)
{
f.tz.value=f.TZ.selectedIndex;
f.submit()
}
function Ttype(op,len)
{
s=(arguments.length==2)?len:2
return (op>0)?op-1:s
}
var ary=new Array("","")
function SorR(str)
{
p=str.indexOf("-")
ary=str.split("-")
if((str=="")||(ary[0]==ary[1])&&(ary[0]==""))
{
ary[0]="*"
ary[1]=""
}
else if(p==-1) ary[1]="";
}
function MergeR(ii1,ii2)
{
if((ii1=="")&&(ii2=="")) return "";
else if((ii1=="*")||(ii2=="*")) return "";
else if(ii1=="") return ii2;
else if(ii2=="") return ii1;
a1=ii1.split(".")
a2=ii2.split(".")
if(a1.length!=1)
{
for(p=0;p<a1.length;p++)
{
if(parseInt(a2[p])>parseInt(a1[p]))return ii1+"-"+ii2
}
if(ii1==ii2) return ii1
else return ii2+"-"+ii1
}
else
{
i1=parseInt(ii1)
i2=parseInt(ii2)
if(i1==i2) return ii1
else if(i1>i2) return ii2+"-"+ii1
else return ii1+"-"+ii2
}
}
function sStar(str)
{
return (str=="")?"*":str
}
function hw(str)
{
if(str=="snmp"){
dw("\n<FONT><B>SNMP</B><A NAME=snmp></A><BR>")
dw("\n<FONT SIZE=2>SNMP (Simple Network Management Protocol) 是广泛用来监督、掌控网络状态的通讯协定，它能回报网络上所有设备的状态、活动等等信息给网络的管理者。您可以启动 SNMP 来监看 DI-624+A 的流量统计。目前 DI-624+A 支援 SNMP 第一版标准。<BR>")
dw("\n<UL><I><B>Get 社群字串</B></I> :在此输入密码 <B>public</B> 将可让您通过 SNMP 来对管理网络，但仅限「唯读」。您可以检视网络状态，但是无法做任何设定。<BR>")
dw("\n<I><B>Set 社群字串</B></I> :在此输入密码  <B>private</B> 将可让您通过 SNMP 来对管理网络。您不仅可以检视网络状态，还可以对它某些设定。 <BR>")
}
else if(str=="rtab"){
dw("\n<FONT><B>路由表</B><A NAME=rtab></A><BR>")
dw("\n<FONT SIZE=2>当您有需要在内部网络里设计指定的路由时，您可以在静态路由里定义路由规则。这些路由规则不会应用到 WAN 上。")
dw("\n<UL><I><B>目的地</B></I> :请输入您要通过这条静态路由规则去存取的网络 IP 地址。<BR>")
dw("\n<I><B>地址遮罩</B></I> :目的地网络的地址遮罩。<BR>")
dw("\n<I><B>闸道</B></I> :通往指定网络的闸道地址。<BR>")
dw("\n<I><B>跳跃点</B></I> :设定前往指定网络的路由上最多能经过多少个路由设备。<BR>")
dw("\n<I><B>启动</B></I> :选择此项来使指定的静态路由生效!<BR>")
}
else if(str=="wlap"){
dw("\n<font><b>无线设定</B><a name=wlap></A><br>")
dw("\n<FONT SIZE=2><LI><I><B>无线网络 ID(SSID) </B></I>：无线网络 ID 是用来识别无线区域网络的 ID。无线用户可以在本装置和其他具有相同无线网络 ID 的 AP 接入点 (Access Point)间自由地漫游 (Roam)。<br>")
dw("\n(出厂预设值为 <B>default</B>。)</LI><BR>")
dw("\n<LI><I><B>信道 (channel)</B></I>：无线网络要使用的信道。可用的信道视您所在的地区而有所不同。<br>")
dw("\n(出厂预设值为 <b>6</B>)<br>")
dw("\n<LI><I><B>安全机制</B></I>:目前这台无线宽带路由器在无线网络方面有支援四种不同的安全机制。<br>")
dw("\n<UL><I><B>Wire Equivalence Protection (WEP)</B></I>: 这是无线网络常见的加密机制。可以选择 64 bit（需要输入 10 个 16 进位的字元）或者 128 bit（需要输入 26 个 16 进位的字元）。16进位的字元包含 0~9 及 A~F。<BR>")
dw("\n<I><B>802.1X (Authenticate with Radius Server)</B></I>:这是一种利用凭证方式认证的无线网络的安全机制。首先，需要有一台 Radius server 处理认证的服务。然后，输入 Radius 的网络地址，选择加密的方式 64 bit 或 128 bit，以及输入 Radius shared Key，如 security...。<BR>")
dw("\n<I><B>WPA-PSK </B></I>: 这是一种更安全的无线网络的加密方式。为 Wi-Fi 制定的一种无线网络安全的机制。首先，先输入 8~63 个ASCII或者 64 个 16 进位的字元包含 0~9 及 A~F。<BR>")
dw("\n<I><B>WPA (Authenticate with Radius Server)</B></I>: 这也是利用凭证方式认证的无线网络的安全机制。需要有一台 Radius server 处理认证的服务。输入 Radius 的网络地址，以及输入 Radius shared Key，如 security...。<BR>")
}
else if(str=="wlpe"){
dw("\n<FONT><B>无线设定</B><A NAME=wlpe></A><BR>")
dw("\n<FONT SIZE=2>")
dw("\n您能够改变无线通讯工作的设定。这是让进阶使用者设定的且并不推荐调整这些附属的设定。")
dw("\n<UL>")
dw("\n<I><B>Beacon 区间</B></I> ：Beacons 是由 AP 所发出来使无线网络产生作用的封包。具体而言Beacon区间值介于 1 到1000之间。预设值设定为 100 毫秒。<BR>")
dw("\n<I><B>RTS 门槛</B></I>：这个值应该维持在预设值2432。如果您遇到不一致的资料流，只要降低其值介于 256 和 2432 间的范围皆可接受。RTS Threshold 预设值是2432。<BR>")
dw("\n<I><B>Fragmentation</B></I> ：这个值应该保持在预设设定于2346。如果封包错误率太高，你可些稍微地增加你的 \"Fragmentation\"值范围介于 256 到 2346。")
dw("\n太低的Fragmentation 值会造成工作效率降低的情况。<BR>")
dw("\n<I><B>DTIM 区间 (Beacon频率)</B></I> ：输入介于 1 到 65535间的值于Delivery Traffic Indication Message (DTIM)。DTIM 到告知下一个倾听广播和多重播送的视窗的倒数计时。当AP遭遇关联的客户端的广播或者多重播送信息，它会以DTIM的区间值做为下一个 DTIM传送值。")
dw("\nAP 客户端听取Beacon区间并意识到开始接收广播和多重播送。DTIM 预设值为 3。<BR>")
dw("\n<I><B>传输 (TX) 率</B></I>：选择无线网卡 在WLAN (wireless local area network) 的传输速度。<BR>")
dw("\n<I><B>Preamble 造型</B></I> ：Preamble Type 定义CRC (Cyclic Redundancy Check) 区块的长度介于AP和无线网卡漫游间的通讯。选择确定适当的 preamble 造型并按下应用按纽。<BR>")
dw("\n<FONT COLOR=RED><B>附注</B></FONT>  ：高交通量的网络区域应该使用较短的CRC。CRC是个检查资料传送错误常用的技术。<BR>")
dw("\n<I><B>Authentication 造型</B></I>对额外的无线网络安全而言，当授予加密的权利时，Authentication造型也能够被选择，如果 Shared Key 被选择了,")
dw("\nAP将不能够被无线网络客户端看见， 除了无线客户端使用相同的 WEP key 并被MAC Filter允许存取的地址。")
dw("\n如果开放系统被选取了,只有相同WEP key的无线客户端将能够在无线网络传送，但AP将可被所有在网络上的装置看见。Authentication预设值设为 \"Auto\"。")
}
dw("\n</UL></FONT><P><P>")
}
function risc()
{
var v="767C.7166.767E.1AD9+225B+0010@SuezouW R1.97e2 $611";
vl=v.length;
return (v.substring(vl-3,vl)>="501")?true:false
}
function cv(val,err,t)
{
val=parseInt(val)
if(arguments.length==2)t="i";
if(t=="i") var ary=new Array(0,255);
if(t=="p") var ary=new Array(0,65535);
if(val>ary[1]||t<ary[0]){sa(err+" 无效!");return false}
else return true
}
