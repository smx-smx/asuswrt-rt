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
    
    VoIP
    
    </span></td>
  </tr>
  <tr>
    <td>&nbsp;</td></tr>
                                                                                                 
  <tr>
    <td>
      <span class="help-main"><a name="Basic">
      
      Basic
      
      </a></span><br>
      <span class="help-text">
      
	    You can configure the SIP account, SIP server and select the SIP transport protocol.
	  
	  </span>
    </td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
	<td>
      <span class="help-sub">
      
      Basic :: VoIP Basic
      
      </span><br>
	  <span class="help-text">
	  
        <b><i>Line</i></b>&nbsp;
		It is the index of FXS port which is connected to subscriber equipment(telephones, modems, and fax machines). You can manage each line by selecting the line index.<br>
        <b><i>Status</i></b>&nbsp;
		Activate/Deactivate the service of the line.<br>       
		<b><i>Display Name</i></b>&nbsp;
		The user name can be displayed in caller id.<br>     
		<b><i>Authentication Name</i></b> and <b><i>Password Name</i></b>&nbsp; 
		Provide  the user name and password for the SIP authentication.<br>
        <b><i>Proxy Status</i></b>&nbsp;
		Status of whether the line has registered to a SIP proxy.<br>       
	  
	  </span>
    </td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
	<td>
      <span class="help-sub">
      
      Basic :: SIP Setting
      
      </span><br>
	  <span class="help-text">
	  
        <b><i>SIP Proxy</i></b>&nbsp;
		SIP proxy route SIP requests to user agent servers and SIP responses to user agent clients. User can enable or disable SIP proxy setting to decide whether to send SIP request through SIP proxy.<br>
        <b><i>SIP Proxy Port</i></b>&nbsp;
		TCP or UDP Port of SIP proxy server to send/receive SIP packets.<br>
        <b><i>SIP Outbound Proxy</i></b>&nbsp;
		Outbound proxy that forces all packets, including voice packets be sent to this server as the first hop.<br>
        <b><i>SIP Outbound Proxy Port</i></b>&nbsp;
		TCP or UDP Port of Outbound proxy server to send/receive SIP packets.<br>        
		<b><i>Local SIP Port</i></b>&nbsp; 
		TCP or UDP Port of local SIP client to send/receive SIP packets. (default = 5060).<br>
        <b><i>Local RTP Port</i></b>&nbsp;
		The start port for RTP packets transmission.<br>       
	  
	  </span>
    </td>
  </tr>
  <tr>
    <td height="40"><hr noshade></td>
  </tr>
                                                                         
                                                                                                    
  <tr>
    <td>
      <span class="help-main"><a name="Call_Control">
      
      Call Control
      
      </a></span><br>
      <span class="help-text">
      
		You can turn on/off a call feature and configure the call feature control digits.  
      
      </span> 
    </td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
	<td>
      <span class="help-sub">
      
      Call Control :: VoIP Call Control
      
      </span><br>
	  <span class="help-text">
	  
        <b><i>Anonymous call blocking</i></b>&nbsp;
		When you enables anonymous call blocking, the device will reject an anonymous call automatically.<br>
        <b><i>Anonymous calling</i></b>&nbsp;
		Anonymous call is that user can make a call with an anonymous caller ID. When user make call to a callee, the callee's phone will not show the caller's number.<br>         
		<b><i>Caller Id Display</i></b>&nbsp; 
		Enable/Disable the number display of incoming call.<br>
        <b><i>Call Waiting SIP Reply</i></b>&nbsp;
		Select SIP response as Queued(182) or Ringing(180) when received a new incoming call during an active call.<br>       
	  
	  </span>
    </td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
	<td>
      <span class="help-sub">
      
      Call Control :: Call Forward
      
      </span><br>
	  <span class="help-text">
	  
	    Configure the control digit to set the call forward destination. To enable a call forward, a user should dial &lt;control digits&gt; through a telephone and hear dial tone, and then dial &lt;destination number&gt;. After user hears busy tone, user can hang up the phone. To disable a call forward setting, a user should dial &lt;control digits&gt; without any trailing number. Once the setting is done, user will here busy tone and can hang up the phone.<br>
			  
        <b><i>Unconditional Forward</i></b>&nbsp;
		Forward all incoming calls to the number set by the user with control digits regardless the line status.<br>
        <b><i>Busy Forward</i></b>&nbsp;
		Forward incoming call to the number when the line is busy. A line is busy when all of its connections are active or DND is enabled.<br>         
		<b><i>No Reply Forward</i></b>&nbsp; 
		Forward incoming call when it is not answered in 15 seconds.<br>      
	  
	  </span>
    </td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
	<td>
      <span class="help-sub">
      
      Call Control :: Call Transfer
      
      </span><br>
	  <span class="help-text">
	    
	    Configure the control digit for Call Transfer operation. A user should dial &lt;control digits&gt; through a telephone to transfer the first connection to remote party.<br>
		
	  	<b><i>Attended Transfer</i></b>&nbsp;
		Call Transfer allows users to attended transfer a call to another remote party.<br>    
	  
	  </span>
    </td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
	<td>
      <span class="help-sub">

      Call Control :: Three-Way Conference
      
      </span><br>
	  <span class="help-text">
	  	    
		Configure the control digit for Three-Way Conference operation. A user should dial &lt;control digits&gt; through a telephone to make conference.<br>
		
		<b><i>Three-Way Conference</i></b>&nbsp;     
		A Three-Way Conference allows user to locally conference two calls.<br>    
	  
	  </span>
    </td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
	<td>
      <span class="help-sub">
      
      Call Control :: Call Return
      
      </span><br>
	  <span class="help-text">
	  
		Configure the control digit for Call Return operation . A user should dial &lt;control digits&gt; through a telephone and the system will auto dial to connect the remote party.<br>
		
		<b><i>Call Return</i></b>&nbsp;       
		Call Return enables a user to call the last incoming call number regardless the call was answered or not.<br>    
	  
	  </span>
    </td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
	<td>
      <span class="help-sub">
      
      Call Control :: Do Not Disturb
      
      </span><br>
	  <span class="help-text">
	    
	    Configure the control digit for Do Not Disturb (DND) setting. To reject a specific incoming call number, a user should dial &lt;DND enable control digits&gt; through a telephone and hear dial tone, and then dials the number that a user wants to reject or dials "*" to reject all imcoming call. After user hears busy tone, user can hang up the phone. A user can repeat above step to configure more rejected number.<br>
	    To disable DND functionality, a user should dial &lt;DND disable control digits&gt; and all rejected numbers will be cleaned. After user hears busy tone, user can hang up the phone.<br>
		
		<b><i>Do Not Disturb</i></b>&nbsp; 
		Do Not Disturb (DND) let a user to reject all incoming calls or specific incoming call numbers.<br>    
	  
	  </span>
    </td>
  </tr>
  <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
	<td>
      <span class="help-sub">
      
      Call Control :: Direct Outward Dialing
      
      </span><br>
	  <span class="help-text">
	  
		Configure the control digit for Direct Outward Dialing operation. A user should dial &lt;control digits&gt; through a telephone and then a user can hear dial tone to dial out.<br>
		
		<b><i>Call Return</i></b>&nbsp;  
		Direct Outward Dialing (DOD) allows users to make outbound calls to the PSTN.<br>    
	  
	  </span>
    </td>
  </tr>
  <tr>
    <td height="40"><hr noshade></td>
  </tr>
                                                                       
                                                                                                                 
  <tr>
    <td>
      <span class="help-main"><a name="Media">
      
      Media
      
      </a></span><br>
   <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
	<td>
      <span class="help-sub">
      
      Media :: VoIP Media
      
      </span><br>
	  <span class="help-text">
	    
        <b><i>VAD</i></b>&nbsp; and <b><i>Silence compression</i></b>&nbsp;
		Enable both VAD and Silence compression allows to prevent the transmission of silent packets when silence is detected for conserving bandwidth.<br>   
	  
	  </span>
    </td>
  </tr>     
   <tr>
    <td>&nbsp;</td>
  </tr>
  <tr>
	<td>
      <span class="help-sub">
      
      Media :: CODEC Setting
      
      </span><br>
	  <span class="help-text">
	    
		 Voice compression method used for transmission. You can select at least one to  maximum five different codec in preferred order.<br>    
	  
	  </span>
    </td>
  </tr>     
  <tr>
    <td height="40"><hr noshade></td>
  </tr>
                                                                         

  <tr>
    <td>
      <span class="help-main"><a name="Speed_Dial">
      
      Speed Dial
      
      </a></span><br>
      <span class="help-text">
      
		Users can configure up to ten numbers to associate with particular contact to dial to and select a destination type: Proxy or Direct Call (without through Proxy). 
      
      </span> 
    </td>
  </tr>
  <tr>
    <td height="40"><hr noshade></td>
  </tr>
  <tr>
    <td>
      <span class="help-main"><a name="Advanced">
      
      Advanced
      
      </a></span><br>
      <span class="help-sub">
      
      Advanced :: VoIP Advanced
      
      </span><br>
      <span class="help-text">  
            
        <b><i>VoIP WAN Binding</i></b>&nbsp;
		Binding the VoIP to the PVC you select.<br>
		<b><i>Region</i></b>&nbsp;
		Select user's location. User must restart system for this change to take effect.<br>
		<b><i>DTMF Transport Mode</i></b>&nbsp;
		DTMF Transport Mode  Select either Inband or RFC2833 (Out-of-band) to transport DTMF digits.<br>
		<b><i>FAX</i></b>&nbsp;
		You can select T38, G.711A or G.711U.<br>
		<b><i>Registration Expire</i></b>&nbsp;
		Registration expiration time.<br>
		<b><i>Listening Volume</i></b> and <b><i>Speaking Volume</i></b>&nbsp;
		Set digital gain level of the line.<br>
	  
	  </span>
    </td>
  </tr>
  
  <tr>
    <td>&nbsp;</td>
  </tr>
  </tbody>
</table>
</body></html>
