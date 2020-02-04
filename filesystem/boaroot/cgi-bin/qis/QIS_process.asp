<%
If Request_Form("current_page") = "/cgi-bin/qis/QIS_admin_pass.asp" Then
	tcWebApi_set("GUITemp_Entry0","username","uiViewUsername")
	tcWebApi_set("GUITemp_Entry0","web_passwd","uiViewPassword")

elseif Request_Form("current_page") = "/cgi-bin/qis/QIS_detect.asp" Then
	if Request_Form("action_script") = "restart_dsl_setting" Then
		tcWebApi_set("Adsl_Entry","ANNEXTYPEA","AnnexTypeA")
		tcWebApi_commit("Adsl_Entry")
	elseif Request_Form("action_script") = "restart_autodet" Then
		auto_detection()
	end if

elseif Request_Form("current_page") = "/cgi-bin/qis/QIS_wireless.asp" Then
	tcWebApi_Set("GUITemp_Entry0","dsltmp_cfg_wl0_ssid","wl0_ssid")
	tcWebApi_Set("GUITemp_Entry0","dsltmp_cfg_wl0_wpa_psk","wl0_wpa_psk")
	tcWebApi_Set("GUITemp_Entry0","dsltmp_cfg_wl0_auth_mode_x","wl0_auth_mode_x")
	tcWebApi_Set("GUITemp_Entry0","dsltmp_cfg_wl1_ssid","wl1_ssid")
	tcWebApi_Set("GUITemp_Entry0","dsltmp_cfg_wl1_wpa_psk","wl1_wpa_psk")
	tcWebApi_Set("GUITemp_Entry0","dsltmp_cfg_wl1_auth_mode_x","wl1_auth_mode_x")
	tcWebApi_Set("SysInfo_Entry","w_Setting","w_Setting")
	tcWebApi_Set("WLan_Entry","WPSConfStatus","WPSConfigured")
	tcWebApi_commit("SysInfo_Entry")

elseif Request_Form("current_page") = "/cgi-bin/qis/QIS_annex_setting.asp" Then
	tcWebApi_set("Adsl_Entry","ANNEXTYPEA","AnnexTypeA")
	tcWebApi_commit("Adsl_Entry")

elseif Request_Form("current_page") = "/cgi-bin/qis/QIS_manual_setting.asp" Then
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_vpi","dsltmp_cfg_vpi")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_vci","dsltmp_cfg_vci")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_prctl","dsltmp_cfg_prctl")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_encap","dsltmp_cfg_encap")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_vlanid","dsltmp_cfg_vlanid")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_iptv_mr","dsltmp_cfg_iptv_mr")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_iptv_rmvlan","dsltmp_cfg_iptv_rmvlan")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_iptv_num_pvc","dsltmp_cfg_iptv_num_pvc")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_iptv_pvclist","dsltmp_cfg_iptv_pvclist")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_iptv_enable","dsltmp_cfg_iptv_enable")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_ispname","dsltmp_cfg_ispname")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_ispservice","dsltmp_cfg_ispservice")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_country","dsltmp_cfg_country")
	tcWebApi_set("GUITemp_Entry0","dsltmp_transfer_mode","dsltmp_transfer_mode")
	tcWebApi_set("GUITemp_Entry0","dsltmp_wanTypeOption","dsltmp_wanTypeOption")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_th3bb","dsltmp_cfg_th3bb")
	if Request_Form("dsltmp_cfg_th3bb") = "1" Then
		tcWebApi_set("Adsl_Entry","MODULATIONTYPE","dsltmp_cfg_modulation")
		tcWebApi_set("Adsl_Entry","ANNEXTYPEA","dsltmp_cfg_annex")
	end if
	tcWebApi_set("GUITemp_Entry0","dsltmp_auto_detect_bng_flag","dsltmp_auto_detect_bng_flag")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_trtt","dsltmp_cfg_trtt")
	qis_dsl_early_restart()

elseif Request_Form("current_page") = "/cgi-bin/qis/QIS_PTM_manual_setting.asp" Then
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_prctl","dsltmp_cfg_prctl")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_vlanid","dsltmp_cfg_vlanid")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_iptv_mr","dsltmp_cfg_iptv_mr")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_iptv_rmvlan","dsltmp_cfg_iptv_rmvlan")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_iptv_num_pvc","dsltmp_cfg_iptv_num_pvc")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_iptv_pvclist","dsltmp_cfg_iptv_pvclist")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_iptv_enable","dsltmp_cfg_iptv_enable")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_ispname","dsltmp_cfg_ispname")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_ispservice","dsltmp_cfg_ispservice")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_country","dsltmp_cfg_country")
	tcWebApi_set("GUITemp_Entry0","dsltmp_transfer_mode","dsltmp_transfer_mode")
	tcWebApi_set("GUITemp_Entry0","dsltmp_wanTypeOption","dsltmp_wanTypeOption")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_th3bb","dsltmp_cfg_th3bb")
	if Request_Form("dsltmp_cfg_th3bb") = "1" Then
		tcWebApi_set("Adsl_Entry","MODULATIONTYPE","dsltmp_cfg_modulation")
		tcWebApi_set("Adsl_Entry","ANNEXTYPEA","dsltmp_cfg_annex")
		tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_vpi","dsltmp_cfg_vpi")
		tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_vci","dsltmp_cfg_vci")
		tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_encap","dsltmp_cfg_encap")
	end if
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_de17a","dsltmp_cfg_de17a")
	if Request_Form("dsltmp_cfg_de17a") = "1" Then
		tcWebApi_set("Adsl_Entry","vdsl_profile","dsltmp_cfg_vdslprofile")
	end if	
	tcWebApi_set("GUITemp_Entry0","dsltmp_auto_detect_bng_flag","dsltmp_auto_detect_bng_flag")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_uksky","dsltmp_cfg_uksky")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_trtt","dsltmp_cfg_trtt")
	qis_dsl_early_restart()

elseif Request_Form("current_page") = "/cgi-bin/qis/QIS_ppp_cfg.asp" Then
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_vpi","dsltmp_cfg_vpi")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_vci","dsltmp_cfg_vci")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_prctl","dsltmp_cfg_prctl")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_encap","dsltmp_cfg_encap")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_vlanid","dsltmp_cfg_vlanid")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_iptv_rmvlan","dsltmp_cfg_iptv_rmvlan")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_iptv_mr","dsltmp_cfg_iptv_mr")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_iptv_num_pvc","dsltmp_cfg_iptv_num_pvc")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_iptv_pvclist","dsltmp_cfg_iptv_pvclist")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_iptv_enable","dsltmp_cfg_iptv_enable")
	tcWebApi_set("GUITemp_Entry0","dsltmp_transfer_mode","dsltmp_transfer_mode")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_pppoe_username","dsltmp_cfg_pppoe_username")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_pppoe_passwd","dsltmp_cfg_pppoe_passwd")
	tcWebApi_set("GUITemp_Entry0","dsltmp_wanTypeOption","dsltmp_wanTypeOption")
	tcWebApi_set("GUITemp_Entry0","with_wan_setting","with_wan_setting")
	tcWebApi_set("GUITemp_Entry0","dsltmp_auto_detect_bng_flag","dsltmp_auto_detect_bng_flag")
	qis_dsl_early_restart()

elseif Request_Form("current_page") = "/cgi-bin/qis/QIS_ppp_cfg_tmp.asp" Then
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_pppoe_username","dsltmp_cfg_pppoe_username")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_pppoe_passwd","dsltmp_cfg_pppoe_passwd")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_hwaddr","dsltmp_cfg_hwaddr")
	tcWebApi_set("GUITemp_Entry0","with_wan_setting","with_wan_setting")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_iptv_username","dsltmp_cfg_iptv_username")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_iptv_passwd","dsltmp_cfg_iptv_passwd")
	qis_dsl_early_restart()

elseif Request_Form("current_page") = "/cgi-bin/qis/QIS_mer_cfg.asp" Then
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_vpi","dsltmp_cfg_vpi")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_vci","dsltmp_cfg_vci")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_prctl","dsltmp_cfg_prctl")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_encap","dsltmp_cfg_encap")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_iptv_rmvlan","dsltmp_cfg_iptv_rmvlan")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_iptv_mr","dsltmp_cfg_iptv_mr")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_iptv_num_pvc","dsltmp_cfg_iptv_num_pvc")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_iptv_pvclist","dsltmp_cfg_iptv_pvclist")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_iptv_enable","dsltmp_cfg_iptv_enable")
	tcWebApi_set("GUITemp_Entry0","dsltmp_transfer_mode","dsltmp_transfer_mode")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_DHCPClient","dsltmp_cfg_DHCPClient")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_dnsenable","dsltmp_cfg_dnsenable")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_vlanid","dsltmp_cfg_vlanid")

	tcWebApi_set("GUITemp_Entry0","dsltmp_dhcp_vendorid","dsltmp_dhcp_vendorid")
	tcWebApi_set("GUITemp_Entry0","dsltmp_dhcp_clientid_type","dsltmp_dhcp_clientid_type")
	tcWebApi_set("GUITemp_Entry0","dsltmp_dhcp_clientid","dsltmp_dhcp_clientid")

	tcWebApi_set("GUITemp_Entry0","dsltmp_wanTypeOption","dsltmp_wanTypeOption")
	tcWebApi_set("GUITemp_Entry0","with_wan_setting","with_wan_setting")
	tcWebApi_set("GUITemp_Entry0","dsltmp_dhcp_hostname","dsltmp_dhcp_hostname")
	qis_dsl_early_restart()

elseif Request_Form("current_page") = "/cgi-bin/qis/QIS_mer_cfg_tmp.asp" Then
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_DHCPClient","dsltmp_cfg_DHCPClient")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_ipaddr","dsltmp_cfg_ipaddr")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_netmask","dsltmp_cfg_netmask")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_gateway","dsltmp_cfg_gateway")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_dnsenable","dsltmp_cfg_dnsenable")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_dns1","dsltmp_cfg_dns1")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_dns2","dsltmp_cfg_dns2")

	tcWebApi_set("GUITemp_Entry0","dsltmp_dhcp_vendorid","dsltmp_dhcp_vendorid")
	tcWebApi_set("GUITemp_Entry0","dsltmp_dhcp_clientid_type","dsltmp_dhcp_clientid_type")
	tcWebApi_set("GUITemp_Entry0","dsltmp_dhcp_clientid","dsltmp_dhcp_clientid")

	tcWebApi_set("GUITemp_Entry0","dsltmp_wanTypeOption","dsltmp_wanTypeOption")
	tcWebApi_set("GUITemp_Entry0","with_wan_setting","with_wan_setting")
	tcWebApi_set("GUITemp_Entry0","dsltmp_dhcp_hostname","dsltmp_dhcp_hostname")
	qis_dsl_early_restart()

elseif Request_Form("current_page") = "/cgi-bin/qis/QIS_ipoa_cfg_tmp.asp" Then
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_DHCPClient","dsltmp_cfg_DHCPClient")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_ipaddr","dsltmp_cfg_ipaddr")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_netmask","dsltmp_cfg_netmask")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_gateway","dsltmp_cfg_gateway")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_dnsenable","dsltmp_cfg_dnsenable")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_dns1","dsltmp_cfg_dns1")
	tcWebApi_set("GUITemp_Entry0","dsltmp_cfg_dns2","dsltmp_cfg_dns2")
	tcWebApi_set("GUITemp_Entry0","with_wan_setting","with_wan_setting")
	qis_dsl_early_restart()

elseif Request_Form("current_page") = "/cgi-bin/qis/QIS_bridge_cfg_tmp.asp" Then
	tcWebApi_set("GUITemp_Entry0","with_wan_setting","with_wan_setting")
	qis_dsl_early_restart()

elseif Request_Form("current_page") = "/cgi-bin/qis/QIS_finish.asp" Then
	'QIS_admin_pass.asp
	if tcWebApi_get("GUITemp_Entry0","web_passwd","h") <> "" then
		'clear temp user/passwd
		tcWebApi_set("GUITemp_Entry0","username","uiViewResetValue")
		tcWebApi_set("GUITemp_Entry0","web_passwd","uiViewResetValue")
		'set user/passwd
		tcWebApi_set("Account_Entry0","username","uiViewUsername")
		tcWebApi_set("Account_Entry0","web_passwd","uiViewPassword")
		TCWebApi_set("Account_Entry0","console_passwd","uiViewPassword")
		tcWebApi_commit("Account_Entry0")
		modify_aidisk_account()
	end if

	'QIS_wireless.asp
	tcWebApi_Set("SysInfo_Entry","w_Setting","w_Setting")
	tcWebApi_commit("SysInfo_Entry")

	tcWebApi_set("WLan_Common","QIS_Flag","QIS_Flag")
	tcWebApi_Set("WLan_Entry","wl0_ssid","wl0_ssid")
	tcWebApi_Set("WLan_Entry","wl0_wpa_psk","wl0_wpa_psk")
	tcWebApi_Set("WLan_Entry","wl0_auth_mode_x","wl0_auth_mode_x")
	tcWebApi_Set("WLan_Entry","wl1_ssid","wl1_ssid")
	tcWebApi_Set("WLan_Entry","wl1_wpa_psk","wl1_wpa_psk")
	tcWebApi_Set("WLan_Entry","wl1_auth_mode_x","wl1_auth_mode_x")
	tcWebApi_Set("WLan_Entry","wl0_crypto","wl0_crypto")
	tcWebApi_Set("WLan_Entry","wl1_crypto","wl1_crypto")
  tcWebApi_commit("WLan_Entry")

	'Set WAN
	if tcWebApi_get("GUITemp_Entry0","with_wan_setting","h") <> "" then
		'QIS_*_cfg*.asp
		tcWebApi_set("Wan_Common","TransMode","dsltmp_transfer_mode")
		tcWebApi_set("WebCurSet_Entry","dev_pvc","wan_pvc")
		tcWebApi_set("WebCurSet_Entry","wan_pvc","wan_pvc")
		tcWebApi_set("Wan_PVC","Active","value_yes")

		if tcWebApi_get("GUITemp_Entry0","dsltmp_transfer_mode","h") = "ATM" then
			tcWebApi_set("Wan_PVC","VPI","dsltmp_cfg_vpi")
			tcWebApi_set("Wan_PVC","VCI","dsltmp_cfg_vci")
			tcWebApi_set("Wan_PVC","QOS","value_ubr")
			tcWebApi_set("Wan_PVC","PCR","value_zero")
			tcWebApi_set("Wan_PVC","SCR","value_empty")
			tcWebApi_set("Wan_PVC","MBS","value_empty")

		end if

		if tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_vlanid","h") <> "" then
			tcWebApi_set("Wan_PVC","dot1q","value_yes")
			tcWebApi_set("Wan_PVC","VLANID","dsltmp_cfg_vlanid")
		else
			tcWebApi_set("Wan_PVC","dot1q","value_no")
		end if

		tcWebApi_set("Wan_PVC","ISP","dsltmp_wanTypeOption")
		tcWebApi_set("Wan_PVC","IPVERSION","ipVerRadio")
		tcWebApi_set("Wan_PVC","IPADDR6","value_empty")
		tcWebApi_set("Wan_PVC","PREFIX6","value_empty")
		tcWebApi_set("Wan_PVC","DEFGATEWAY6","value_empty")

		disable_other_wan()

		'DSL Setting
		if tcWebApi_get("GUITemp_Entry0","dsltmp_dsl_restart","h") = "1" then
			tcWebApi_CommitWithoutSave("Adsl_Entry")
		end if

		if tcWebApi_get("GUITemp_Entry0","dsltmp_wanTypeOption","h") = "0" then 'Automatic IP

			if tcWebApi_get("GUITemp_Entry0","dsltmp_transfer_mode","h") = "ATM" then
				if tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_encap","h") = "1" then
					tcWebApi_set("Wan_PVC","ENCAP","Bridged_IP_VC")
				else
					tcWebApi_set("Wan_PVC","ENCAP","Bridged_IP_LLC")
				end if
			end if

			tcWebApi_set("Wan_PVC","dhcp_vendorid","dsltmp_dhcp_vendorid")
			tcWebApi_set("Wan_PVC","dhcp_clientid_type","dsltmp_dhcp_clientid_type")
			tcWebApi_set("Wan_PVC","dhcp_clientid","dsltmp_dhcp_clientid")

			tcWebApi_set("Wan_PVC","BridgeInterface","value_no")
			tcWebApi_set("Wan_PVC","wan_hostname","dsltmp_dhcp_hostname")

			if tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_dnsenable","h") = "0" then
				tcWebApi_set("Wan_PVC","DNS_type","value_one")
				tcWebApi_set("Wan_PVC","Primary_DNS","dsltmp_cfg_dns1")
		    tcWebApi_set("Wan_PVC","Secondary_DNS","dsltmp_cfg_dns2")
			else
				tcWebApi_set("Wan_PVC","DNS_type","value_zero")
				tcWebApi_set("Wan_PVC","Primary_DNS","value_empty")
		    tcWebApi_set("Wan_PVC","Secondary_DNS","value_empty")
			end if

			tcWebApi_set("Wan_Common","NATENABLE","wan_NAT")
			tcWebApi_set("Wan_PVC","DEFAULTROUTE","value_yes")
			tcWebApi_set("Wan_PVC","MTU","value_zero")
			tcWebApi_set("Wan_PVC","RIPVERSION","wan_RIP")

			tcWebApi_set("Wan_PVC","DIRECTION","value_none")
			tcWebApi_set("Wan_PVC","IGMPproxy","value_no")

			tcWebApi_set("Wan_PVC","IPVERSION","ipVerRadio")
			tcWebApi_set("Wan_PVC","EnableDynIPv6","value_one")
			tcWebApi_set("Wan_PVC","MLDproxy","value_no")
			tcWebApi_set("Wan_PVC","PPPv6PD","value_yes")

			tcWebApi_set("Wan_PVC","IPOE_DOT1X_STATUS","wan_status")
			tcWebApi_set("Wan_PVC","EAPIDENTITY","value_empty")
			tcWebApi_set("Wan_PVC","BIDIRECTIONALAUTHENTICATION","value_empty")
			tcWebApi_set("Wan_PVC","CERTIFICATE","value_empty")
			tcWebApi_set("Wan_PVC","TRUSTEDCA","value_empty")


		elseif tcWebApi_get("GUITemp_Entry0","dsltmp_wanTypeOption","h") = "1" Then 'Static IP

			if tcWebApi_get("GUITemp_Entry0","dsltmp_transfer_mode","h") = "ATM" then
				if tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_encap","h") = "1" then
					if tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_prctl","h") = "2" then 'MER
						tcWebApi_set("Wan_PVC","ENCAP","Bridged_IP_VC")
						tcWebApi_set("Wan_PVC","BridgeInterface","value_no")
					else 'IPoA
						tcWebApi_set("Wan_PVC","ENCAP","Routed_IP_VC")
					end if
				else
					if tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_prctl","h") = "2" then 'MER
						tcWebApi_set("Wan_PVC","ENCAP","Bridged_IP_LLC")
						tcWebApi_set("Wan_PVC","BridgeInterface","value_no")
					else 'IPoA
						tcWebApi_set("Wan_PVC","ENCAP","Routed_IP_LLC")
					end if
				end if
			end if

			if tcWebApi_get("GUITemp_Entry0","dsltmp_transfer_mode","h") = "PTM" then
				tcWebApi_set("Wan_PVC","BridgeInterface","value_no")
			end if

			tcWebApi_set("Wan_PVC","IPADDR","dsltmp_cfg_ipaddr")
			tcWebApi_set("Wan_PVC","NETMASK","dsltmp_cfg_netmask")
			tcWebApi_set("Wan_PVC","GATEWAY","dsltmp_cfg_gateway")

			tcWebApi_set("Wan_PVC","DNS_type","value_one")
			tcWebApi_set("Wan_PVC","Primary_DNS","dsltmp_cfg_dns1")
	    tcWebApi_set("Wan_PVC","Secondary_DNS","dsltmp_cfg_dns2")

			tcWebApi_set("Wan_Common","NATENABLE","wan_NAT")
			tcWebApi_set("Wan_PVC","DEFAULTROUTE","value_yes")
			tcWebApi_set("Wan_PVC","MTU","value_zero")
			tcWebApi_set("Wan_PVC","RIPVERSION","wan_RIP")

			tcWebApi_set("Wan_PVC","DIRECTION","value_none")
			tcWebApi_set("Wan_PVC","IGMPproxy","value_no")

			tcWebApi_set("Wan_PVC","IPADDR6","value_empty")
			tcWebApi_set("Wan_PVC","PREFIX6","value_empty")
			tcWebApi_set("Wan_PVC","DEFGATEWAY6","value_empty")
			tcWebApi_set("Wan_PVC","DNSIPv61st","value_empty")
			tcWebApi_set("Wan_PVC","DNSIPv62nd","value_empty")
			tcWebApi_set("Wan_PVC","MLDproxy","value_no")

			tcWebApi_set("Wan_PVC","IPOE_DOT1X_STATUS","wan_status")
			tcWebApi_set("Wan_PVC","EAPIDENTITY","value_empty")
			tcWebApi_set("Wan_PVC","BIDIRECTIONALAUTHENTICATION","value_empty")
			tcWebApi_set("Wan_PVC","CERTIFICATE","value_empty")
			tcWebApi_set("Wan_PVC","TRUSTEDCA","value_empty")


		elseif tcWebApi_get("GUITemp_Entry0","dsltmp_wanTypeOption","h") = "2" Then

			if tcWebApi_get("GUITemp_Entry0","dsltmp_transfer_mode","h") = "ATM" then
				if tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_encap","h") = "1" then
					if tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_prctl","h") = "0" then 'PPPoE
						tcWebApi_set("Wan_PVC","ENCAP","PPPoE_VC")
						tcWebApi_set("Wan_PVC","BridgeInterface","value_no")
					else 'PPPoA
						tcWebApi_set("Wan_PVC","ENCAP","PPPoA_VC")
					end if
				else
					if tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_prctl","h") = "0" then 'PPPoE
						tcWebApi_set("Wan_PVC","ENCAP","PPPoE_LLC")
						tcWebApi_set("Wan_PVC","BridgeInterface","value_no")
					else 'PPPoA
						tcWebApi_set("Wan_PVC","ENCAP","PPPoA_LLC")
					end if
				end if
			end if

			if tcWebApi_get("GUITemp_Entry0","dsltmp_transfer_mode","h") = "PTM" then
				tcWebApi_set("Wan_PVC","BridgeInterface","value_no")
			end if

			tcWebApi_set("Wan_PVC","USERNAME","dsltmp_cfg_pppoe_username")
			tcWebApi_set("Wan_PVC","PASSWORD","dsltmp_cfg_pppoe_passwd")
			tcWebApi_set("Wan_PVC","WAN_MAC", "dsltmp_cfg_hwaddr")
			tcWebApi_set("Wan_PVC", "AUTHEN", "PPPAuthen")
			if tcWebApi_get("SysInfo_Entry", "Customer", "h") = "POL" then
				tcWebApi_set("Wan_PVC","CONNECTION","wan_ConnectSelect_POL")
				tcWebApi_set("Wan_PVC","CLOSEIFIDLE","value_30")
			else	
				tcWebApi_set("Wan_PVC","CONNECTION","wan_ConnectSelect")
				tcWebApi_set("Wan_PVC","CLOSEIFIDLE","value_zero")
			end if	
			tcWebApi_set("Wan_PVC","MSS","value_zero")
			tcWebApi_set("Wan_PVC","MTU","value_zero")

			tcWebApi_set("Wan_PVC","PPPGETIP","wan_PPPGetIP")
			tcWebApi_set("Wan_PVC","IPADDR","value_empty")
			tcWebApi_set("Wan_PVC","NETMASK","value_empty")
			tcWebApi_set("Wan_PVC","GATEWAY","value_empty")
			TCWebApi_set("Wan_PVC","DNS_type","value_zero")
			TCWebApi_set("Wan_PVC","Primary_DNS","value_empty")
			TCWebApi_set("Wan_PVC","Secondary_DNS","value_empty")

			tcWebApi_set("Wan_Common","NATENABLE","wan_NAT")
			tcWebApi_set("Wan_PVC","DEFAULTROUTE","value_yes")
			tcWebApi_set("Wan_PVC","RIPVERSION","wan_RIP")
			tcWebApi_set("Wan_PVC","DIRECTION","value_none")
			tcWebApi_set("Wan_PVC","IGMPproxy","value_no")

			tcWebApi_set("Wan_PVC","PPPv6Mode","value_one")
			tcWebApi_set("Wan_PVC","PPPv6PD","value_yes")
			tcWebApi_set("Wan_PVC","MLDproxy","value_no")


		elseif tcWebApi_get("GUITemp_Entry0","dsltmp_wanTypeOption","h") = "3" Then

			if tcWebApi_get("GUITemp_Entry0","dsltmp_transfer_mode","h") = "ATM" then
				if tcWebApi_get("GUITemp_Entry0","dsltmp_cfg_encap","h") = "1" then
					tcWebApi_set("Wan_PVC","ENCAP","Bridged_Only_VC")
				else
					tcWebApi_set("Wan_PVC","ENCAP","Bridged_Only_LLC")
				end if
			end if

		end if
		
		/* Deutsche Telekom IPTV detect */
		tcWebApi_set("GUITemp_Entry0","auto_detect_bng","dsltmp_auto_detect_bng")
		
		TCWebApi_set("Upnpd_Entry","Active","Yes")
		tcWebApi_commit("Wan_PVC")
		tcWebApi_commit("Upnpd_Entry")
		qis_do_dsl_iptv()

		if Request_Form("detected_lang_type") <> "" Then
			tcWebApi_set("LanguageSwitch_Entry","Type","detected_lang_type")
			tcWebApi_commit("LanguageSwitch_Entry")
		end if
	end if
	'End Set WAN

elseif Request_Form("current_page") = "init_detection" Then 'init auto detection
	auto_detection()

End If

tcWebApi_set("GUITemp_Entry0","QIS_nextPage","next_page")
tcWebApi_set("GUITemp_Entry0","QIS_currentPage","current_page")
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">

<!--qis/QIS_process.asp-->
<html xmlns:v>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
<meta HTTP-EQUIV="Pragma" CONTENT="no-cache">
<meta HTTP-EQUIV="Expires" CONTENT="-1">
</head>
<script type="text/javascript">
	if("<% tcWebApi_get("GUITemp_Entry0","QIS_currentPage","s") %>" != "/cgi-bin/qis/QIS_finish.asp"
		&& "<% tcWebApi_get("GUITemp_Entry0","QIS_currentPage","s") %>" != "<% tcWebApi_get("GUITemp_Entry0","QIS_nextPage","s") %>"
	)
		document.location.href = "<% tcWebApi_get("GUITemp_Entry0","QIS_nextPage","s") %>";
</script>

<!--qis/QIS_process.asp-->
</html>
