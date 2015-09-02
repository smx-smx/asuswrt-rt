<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<meta http-equiv=Content-Script-Type content=text/javascript>
<meta http-equiv=Content-Style-Type content=text/css>
<link rel="stylesheet" type="text/css" href="/style.css">

<body text=#000000 bgcolor=#ffffff>
<table cellspacing=0 cellpadding=0 width=750 border=0>
<tbody>
  <tr>
    <td height="50" class="orange"><span class="help-header">&nbsp;
    
    Interface Setup
    
    </span></td>
  </tr>
  <tr>
    <td>&nbsp;</td></tr>
                                                
  <tr>
    <td>
      <span class="help-main"><a name="Internet_Settings">
      
      Internet Settings
      
      </a></span><br>	</td>
  </tr>
  <tr>
	<td>
      <span class="help-sub">
      
      Internet :: ATM VC
      
      </span><br>
	  <span class="help-text">
	  
	    ATM settings are used to connect to your ISP. Your ISP provides VPI, VCI settings to you. In this Device, you
        can totally setup 8 VCs on different encapsulations, if you apply 8 different virtual circuits from your ISP. 
      
      
        You need to activate
		the VC to take effect. For PVCs management, you can use ATM QOS to setup each PVC traffic line's priority.<br>
        <b><i>Virtual Circuit</i></b>&nbsp;
		Select the VC number you want to setup.<br>
	  
	  
        <b><i>VPI</i></b>&nbsp;
		Virtual Path Identifier.<br>
        <b><i>VCI</i></b>&nbsp;
		Virtual Channel Identifier.<br>
        <b><i>ATM QoS </i></b>&nbsp; 
		Select the Quality of Service types for this Virtual Circuit. The ATM QoS types include CBR (Constant Bit Rate), 
		VBR (Variable Bit Rate) and UBR (Unspecified Bit Rate). 
	  
	  
		These QoS types are all controlled by the parameters
		specified below, including PCR, SCR and MBS.<br>
        <b><span class="helplink">PCR</span></b>&nbsp;
		Peak Cell Rate.<br>
		<b><span class="helplink">SCR</span></b>&nbsp;
		Sustained Cell Rate.<br>
	    <b><span class="helplink">MBS</span></b>&nbsp;
		Maximum Burst Size.
	  
		</span>
    </td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td>
      <span class="help-sub">
      
      Internet :: Encapsulation
      
      </span><br>      
      <span class="help-text">
		
		<b><i>Dynamic IP </i></b>&nbsp;
	    Select this option if your ISP provides you an IP address automatically. This option is typically used for Cable 
        services. Please enter the Dynamic IP information accordingly.<br>
		<b><i>Static IP </i></b>&nbsp;
		
		
		Select this option to set static IP information. You will need to 
        enter in the Connection type, IP address, subnet mask, and gateway address,  
        provided to you by your ISP. Each IP address entered in the fields must be 
        in the appropriate IP form, which is four IP octets separated by a dot (x.x.x.x). The Router will not accept the IP address if it is not in this 
        format.<br>
        
        
        <b><i>PPPoA/PPPoE</i></b>&nbsp;
		Select this option if your ISP requires you to use a PPPoE connection. This option is typically used for DSL services. 
		Select Dynamic PPPoE to obtain an IP address automatically for your PPPoE connection. Select Static PPPoE to use a 
		static IP address for your PPPoE connection. Please enter the information accordingly.<br>
		
		
		<b><i>Bridge Mode </i></b>
		The modem can be configured to act as a bridging device between your LAN and your ISP. Bridges are devices that enable 
		two or more networks to communicate as if they are two segments of the same physical LAN. Please set the Connection type.
		
	    </span>
	</td>
  <tr>
    <td>&nbsp;</td>
  </tr>
      <td>
        <span class="help-sub">
        
        Internet :: PPPoE/PPPoA 
        
        </span><br>
	    <span class="help-text">
	    
		Select this option if your ISP requires you to use a PPPoE connection. This option is typically used for DSL services.
		Select Dynamic PPPoE to obtain an IP address automatically for your PPPoE connection. Select
        Static PPPoE to use a static IP address for your PPPoE connection. Please
        enter the information accordingly. <br>
        
        
		<b><i>Username </i></b>&nbsp;
	    Enter your username for your PPPoE/PPPoA connection.<br>
		<b><i>Password </i></b>&nbsp;
		Enter your passward for your PPPoE/PPPoA connection<br>
        <b><i>Encapsulation</i></b>&nbsp;
		For both PPPoE/PPPoA connection, you need to specify the type of Multiplexing, either LLC or VC mux.<br>
		<b><i>Connection Setting</i></b>&nbsp;
		
		
		For PPPoE/PPPoA connection, you can select <b>Always on</b> or <b>Connect on-demand</b>. Connect on demand is dependent on
		the traffic. If there is no traffic (or <b>Idle</b>) for a pre-specified period of time), the connection will tear down
		automatically. And once there is traffic send or receive, the connection will be automatically on.
		<br>
		
		
		<b><i>Static/Dynamic IP Address</i></b>&nbsp;
		For PPPoE/PPPoA connection, you need to specify the public IP address for this ADSL Router. The IP address can 
		be either dynamically (via DHCP) or given IP address provide by your ISP. For Static IP, you need to specify the IP address, 
		Subnet Mask and Gateway IP address.
		
	    </span>
	</td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td>
      <span class="help-sub">
      
      Internet :: NAT 
      
      </span><br>
      <span class="help-text">
      
		<b><i>NAT</i></b>&nbsp;
		Select this option to Activate/Deactivated the NAT (Network Address Translation) function for this VC. 
		The NAT function can be activated or deactivated per PVC basis.
      	
		</span>
	</td>
  </tr>
    <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td>
      <span class="help-sub">
      
      Internet :: Default Route 
      
      </span><br>
      <span class="help-text">
      
		<b><i>Default Route </i></b>&nbsp;
		<span class="MsoNormal"><span lang=EN-US>if enable this function, the current PVC will be the default gateway to internet from this device.</span></span>
	  	
		</span>
	</td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td>
      <span class="help-sub">
      
      Internet :: Dynamic Route 
      
      </span><br>
      <span class="help-text">
      
		<b><i>RIP</i></b>&nbsp;
		(Routing Information protocol) Select this option to specify the RIP version, including <b>RIP-1</b>, <b>RIP-2M</b> and <b>RIP-2B</b>. 
		RIP-2M and RIP-2B are both sent in RIP-2 format; the difference is that RIP-2M using Multicast and RIP-2 
		using Broadcast format. <br>
	  
	  
		<b><i>RIP Direction</i></b>&nbsp;
		Select this option to specify the RIP direction. <b>None</b> is for disabling the RIP function. <b>Both</b> means the
		ADSL Router will periodically send routing information and accept routing information then incorporate
		into routing table. <b>IN only </b> means the ADLS router will only accept but will not send RIP packet.
	  <b>OUT only</b> means the ADLS router will only send but will not accept RIP packet.
	  
	  </span></td>
  </tr>
    <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td>
      <span class="help-sub">
      
      Internet :: Multicast 
      
      </span><br>
      <span class="help-text">
	  	
		<b><i>IGMP</i></b> (Internet Group Multicast Protocol) is a session-layer protocol used to establish membership in a multicast group. The ADSL ATU-R supports both IGMP version 1 (<b>IGMP-v1</b>) and <b>IGMP-v2</b>. Select <b>None</b> to disable it.
	  	
		</span>
	</td>
  </tr>
  <tr>
    <td height="40"><hr noshade></td>
  </tr>
                                		                 
  <tr>
    <td>
      <span class="help-main"><a name="LAN_settings">
      
      LAN settings 
      
      </a>
      </span><br>
      <span class="help-text">
      
	    These are the IP settings of the LAN interface for the device. These settings may be referred to as Private settings. 
		You may change the LAN IP address if needed. The LAN IP address is private to your internal network and cannot be seen 
		on the Internet.
	  
	  </span>
    </td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td>
      <span class="help-sub">
      
      LAN :: Dynamic Route 
      
      </span><br>
      <span class="help-text">
	    
	    Please refer to <b>Internet::Dynamic Route</b>. The only difference is the interface.
	  
	  </span>
    </td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
    <tr>
    <td>
      <span class="help-sub">
      
      LAN :: Multicast
      
      </span><br>
      <span class="help-text">
      
	    Please refer to <b>Internet::Multicast</b>. The only difference is the interface.
	  
	  </span>
    </td>
  </tr>
    <tr>
      <td>&nbsp;</td>
    </tr>
    <tr>
      <td> 
        <span class="help-sub">
        
        LAN :: DHCP Server 
        
        </span><br>
        <span class="help-text"> 
        
          DHCP stands for Dynamic Host Control Protocol. The DHCP Server gives out IP addresses when a device is 
          booting up and request an IP address to be logged on to the network. That device must be set as a DHCP 
          client to obtain the IP address automatically. By default, the DHCP Server is enabled. The DHCP address 
        
        
          pool contains the range of the IP address that will automatically be assigned to the clients on the network.<br>
          <b><i>Starting IP Address </i></b>&nbsp;
          The starting IP address for the DHCP server's IP assignment.<br>
          <b><i>IP Pool Count</i></b>&nbsp;
          The max user pool size.<br>
          <b><i>Lease Time</i></b>&nbsp;
          The length of time for the IP lease.
        
        </span>
      </td>
    </tr>
    <tr>
      <td>&nbsp;</td>
    </tr>
    <tr>
      <td> 
	    <span class="help-sub">
	    
	    LAN :: DHCP Relay 
	    
	    </span><br>
        <span class="help-text"> 
        
        A DHCP relay is a computer that forwards DHCP data between computers that request IP addresses and the 
        DHCP server that assigns the addresses. Each of the device's interfaces can be configured as a DHCP relay. 
        If it is enable, the DHCP requests from local PCs will forward to the DHCP server runs on WAN side. 
        
        
        To have this function working properly, please run on router mode only, disable the DHCP server on the LAN port, 
        and make sure the routing table has the correct routing entry.<br>
        <b><i>DHCP Server IP for relay agent </i></b>&nbsp;
        The DHCP server IP Address runs on WAN side. 
        
		</span> 
	  </td>
    </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td>
      <span class="help-sub">
      
      LAN :: DNS Server 
      
      </span><br>
	  <span class="help-text">
	  
        The DNS Configuration allows the user to set the configuration of DNS<br>
        <b><i>DNS Relay selection</i></b>&nbsp;
		If user want to disable this feature, he just need to set both Primary and secondary DNS IP to 0.0.0.0.
        Using DNS relay, users can setup DNS server IP to 192.168.1.1 on their Computer. If not, device will perform as no DNS relay.
        
	</span></td>
  </tr>
  
  <% if tcWebApi_get("Info_WLan","isExist","h") = "N/A" then%>
  <tr>
    <td height="40"><hr noshade></td>
  </tr>
  <%else%>
  <tr>
    <td height="40"><hr noshade></td>
  </tr>
  <tr>
    <td> 
      <span class="help-main"><a name ="Wireless_LAN_Settings">
      
      Wireless LAN Settings
      
      </a></span><br>
      <span class="help-text">  
            
        <b><i>SSID</i></b>&nbsp;
		The SSID is a unique name to identify the ADSL Router in the wireless LAN. Wireless clients associating to the ADSL Router must have the same SSID.<br>
		<b><i>Broadcast SSID</i></b>&nbsp;
		Select <b>No</b> to hide the SSID such that a station can not obtain the SSID through passive scanning. 
      
      
		Select <b>Yes</b> to make the SSID visible so a station can obtain the SSID through passive scanning.<br>
		<b><i>Channel ID</i></b>&nbsp;
		The range of radio frequencies used by IEEE 802.11b/g wireless devices is called a channel.<br>
	  
	  </span>
    </td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td> 
      <span class="help-sub">
      
      Wireless LAN :: WEP
      
      </span><br>
      <span class="help-text">
              
		WEP (Wired Equivalent Privacy) encrypts data frames before transmitting over the wireless network. Select <b>Disable</b> to allow all wireless computers 
		to communicate with the access points without any data encryption.
		Select <b>64-bit WEP</b> or <b>128-bit WEP</b> to use data encryption.<br>
		<b><i>Key#1~Key#4</i></b>&nbsp;
	  
	  	
		The WEP keys are used to encrypt data. Both the ADSL Router and the wireless clients must use the same WEP 
		key for data transmission.If you chose 64-bit WEP, then enter any 10 hexadecimal digits ("0-9", "A-F") preceded by 0x for each key (1-4). 
		If you chose 128-bit WEP, then enter 26 hexadecimal digits ("0-9", "AF") preceded by 0x for each key (1-4).The values must be set up exactly 
	  
	  
		the same on the Access Points as they are on the wireless client stations. The same value must be assigned to Key 1 on both the access point 
		(your ADSL Router) and the client adapters, the same value must be assigned to Key 2 on both the access point and the client stations and so on, 
		for all four WEP keys.
	  	
	  </span>
    </td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td> 
      <span class="help-sub">
      
      Wireless LAN :: WPA-PSK
      
      </span><br>
      <span class="help-text">
              
		Wi-Fi Protected Access, pre-shared key. Encrypts data frames before transmitting over the wireless network.<br>
		<b><i>Pre-shared Key</i></b>&nbsp;
		The Pre-shared Key are used to encrypt data. Both the ADSL Router and the wireless clients must use the same WPA-PSK 
		key for data transmission.
	  	
	  </span>
    </td>
  </tr>   
  <tr>
    <td>&nbsp;</td>
  </tr>

  <tr>
    <td> 
      <span class="help-sub">
      
      Wireless LAN :: Advanced setting
      
      </span><br>
      <span class="help-text">  
            
		<b><i>Beacon Interval</i></b>&nbsp;
		The Beacon Interval value indicates the frequency interval of the beacon. Enter a 
		value between 20 and 1000. A beacon is a packet broadcast by the Router to synchronize the wireless network.<br>
		<b><i>RTS Threshold</i></b>&nbsp;
	  
	  	
		The RTS (Request To Send) threshold (number of bytes) for enabling RTS/CTS handshake. 
		Data with its frame size larger than this value will perform the RTS/CTS handshake. 
		Setting this attribute to be larger than the maximum MSDU (MAC service data unit) size turns off the RTS/CTS 
		handshake. Setting this attribute to zero turns on the RTS/CTS handshake. Enter a value between 0 and 2432.<br>
	  	
	  
		<b><i>Fragment Threshold</i></b>&nbsp;
		The threshold (number of bytes) for the fragmentation boundary for directed messages. It is the maximum data fragment size that 
		can be sent. Enter a value between 256 and 2432.<br>
		<b><i>DTIM</i></b>&nbsp;
		This value, between 1 and 255, indicates the interval of the Delivery Traffic Indication Message (DTIM).<br>	
		<b><i>802.11b/g</i></b>&nbsp;
	  	
	  </span>
    </td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
    <td> 
      <span class="help-sub">
      
      Wireless LAN :: MAC Filter
      
      </span><br>
      <span class="help-text">   
           
		You can allow or deny a list of MAC addresses associated with the wireless stations access to the ADSL Router.<br>
		<b><i>Status</i></b>&nbsp;
		Use the drop down list box to enable or disable MAC address filtering.<br>
		<b><i>Action</i></b>&nbsp;
	  
	  	
		Select <b>Deny Association</b> to block access to the router, MAC addresses not listed will be allowed to access the router. 
		Select <b>Allow Association</b> to permit access to the router, MAC addresses not listed will be denied access to the router. <b>
	  	
	  </b></span>
    </td>
  </tr>  
  <tr>
    <td>&nbsp;</td>
  </tr>
  <%end if%>
  
  <tr>
    <td>&nbsp;</td>
  </tr>
  
</table>
</body></html>
