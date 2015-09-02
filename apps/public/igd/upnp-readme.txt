1.介绍
	UPNP协议是在网络环境下的通用即插即用协议。其实现是基于TCP/IP上的多种已有的协议，和UPNP工作组定义的其他协议
来实现的。在本程序中实现了一个IGD(internet gateway device)设备，在对UPNP协议的实现上，我们只完成了service point端的
部分功能。
2.程序
	该程序有.c,.h文件和portmap.conf,*.XML配置文件组成，全部的文件如下：
	＃xmlpharse.c,xmlpharse.h：
		1.)实现对gatedesc.xml文件的解析，得到相应的设备信息。
		2.)实现UPNP协议的设备通知协议，在组播地址：239.255.255.250，端口：1900上发送设备通知协议。
		3.)实现一个定时器，用来在超时后重发设备通知协议。
		
	＃miniserver.c,miniserver.h:
		1.)建立一个mini server,端口号为：5460，可以改动，但是必须和gatedesc.xml中的端口号一致。
		   该mini server接收control point的请求包，并进行处理。
		2.)解析请求包，解析出请求命令（如：GET,POST,SUBSCRIBLE,UNSUBSCRIBLE,NOTIFY等）解析出有用的
		   一些字段的值。
		3.)处理SUBSCRIBLE请求包，生成NOTIFY包。
	＃soappost.c,soappost.h:
		1.)主要处理post请求，实现SOAP协议。
		2.)完成PORTMAP功能，以及其他的一些和XP中的UPNP交互的功能如：设备的当前状态，设备的运行时间
		   设备的速率等。
	＃tools.c,tools.h:
		1.)得到当前设备网络接口的IP地址和链接状态。
		
	＃*.XML文件：gatedesc.xml,gateinfoSCPD.XML,gateicfgSCPD.XML,gateconnSCPD.XML
		1.)gatedesc.xml是设备的描述文件，包含了设备的名称，UUID等一系列重要信息。在我们的程序中
		   需要将这个文件中的<URLBase>字段中的ip地址改为设备的实际LAN IP 地址，地址后面的端口号
		   要和miniserver.c中的mini server绑定的端口号一致。
		2.)其余几个文件描述了该IGD设备具体提供的服务和功能。在这里如果不熟悉的话最好不要改动。
		3.)这几个XML文件最好放在当前目录下。
		
	注意：该XML文件的格式，和关键字是由UPNP工作组根据XML语言规范定义的。详细的信息可以看UPNP协议中
	       关于XML描述语言的定义。
	
	
	＃配置文件：portmap.conf
		该文件中存储了端口映射的一些信息，用以系统在初始化或者退出时记录已设置的信息。
		在程序中有读这个文件的一段代码，需要注意的是在soappost.c中
			#define PORTMAPFILENAME     "./portmap.conf"
		定义了该配置文件的存储位置，注意保持实际存储位置和程序中定义的要一致。
		
	程序的编译：
		该程序的Makefile已写好，直接编译即可，编译平台是redhat Linux 7.1以上版本。如果使用uClinux，arm-elf-gcc
	编译工具的话，也几乎不用修改。
	
	
3.程序的运行：
	该程序由于要使用UDP的组播所以在运行前要把组播地址的加入路由，命令如下：
			route add -net 224.0.0.0 netmask 240.0.0.0 dev eth0
	然后运行程序：
			./upnpd
注意网络接口得名称:LAN 口为eth0,WAN 口为eth1
4.测试环境：
	1.)控制台控制功能测试：一台运行LINUX7.1以上的双网卡PC,一台运行Windows XP 的PC,将XP与LINUX机器的eth0接口
				连接。然后启动upnpd,这时打开XP的网络邻居，查看网络连接，会看到一个InternetGateway
				网关图标，双击该图标打开，可以看到网关设备的网络速率，传送字节数，运行时间等。
				点击属性，进入NAT端口映射控制界面，在这里可以添加，删除端口映射。
	2.)NAT功能测试：一台运行LINUX7.1以上的双网卡PC,两台运行Windows XP的PC,将一台XP与LINUX机器的eth0接口连接，
			 LINUX机器的WAN口（eth1）接入互联网，另一台XP机器通过Modem或别的设备接入Internet。在LINUX
			 机器上启动upnpd,然后在两台XP上用MSN进行通话和视频。（Miscrosoft的MSN支持NAT功能）
							