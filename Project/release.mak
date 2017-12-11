release_end: release_bootrom release_drivers release_apps release_clmp release_script release_webpage release_led_conf
	

release_bootrom:
#ifneq ($(strip $(TCSUPPORT_CPU_MT7510)),)
ifneq ($(strip $(TCSUPPORT_CPU_MT7510) $(TCSUPPORT_CPU_MT7520)),)
	@echo "release Bootrom for MT751020"
	mkdir -p $(BOOTROM_DIR)/ddr_cal/reserved
	cp -Rf $(BOOTROM_DIR)/ddr_cal/output/* $(BOOTROM_DIR)/ddr_cal/reserved/
	cp -Rf $(BOOTROM_DIR)/ddr_cal/spram.c $(BOOTROM_DIR)/ddr_cal/reserved/
else
	echo "nothing to do!";
endif

release_drivers:
	mkdir -p $(MODULES_PRIV_SRC_DIR)/ko/modules/$(TCPLATFORM)
ifneq ($(strip $(TCSUPPORT_VOIP)),)	
	$(MAKE) -C $(MODULES_PRIV_SRC_DIR)/voip/telephony_drv TEL_INFC=D2_1S_SILAB
	cp $(MODULES_TDI_DIR)/tdi.ko $(FILESYSTEM_DIR)/lib/modules/
	$(MAKE) -C $(MODULES_PRIV_SRC_DIR)/voip/telephony_drv TEL_INFC=D2_2S1O
	cp $(MODULES_TDI_DIR)/tdi.ko $(FILESYSTEM_DIR)/lib/modules/
	cp $(MODULES_PRIV_SRC_DIR)/voip/pcm_slic/Makefile.release $(MODULES_PRIV_SRC_DIR)/voip/pcm_slic/Makefile
	cp $(MODULES_PRIV_SRC_DIR)/voip/telephony_drv/Makefile.release $(MODULES_PRIV_SRC_DIR)/voip/telephony_drv/Makefile
endif
	cp -rf $(FILESYSTEM_DIR)/lib/modules/* $(MODULES_PRIV_SRC_DIR)/ko/modules/$(TCPLATFORM)/

	
release_apps:
	echo "release apps..."
	mkdir -p $(APP_BINARY_DIR)/$(TCPLATFORM)
#release usb_auto_mount
ifneq ($(strip $(TCSUPPORT_USB_AUTOMOUNT)),)
	cp -rf $(APP_AUTOMOUNT_DIR)/usb_auto_mount  $(APP_BINARY_DIR)/$(TCPLATFORM)
endif

#release restore_linos_info
ifneq ($(strip $(TCSUPPORT_RESTORE_LINOS_INFO)),)
	cp -rf $(APP_RESTORE_LINOS_INFO_DIR)/restore_linos_info  $(APP_BINARY_DIR)/$(TCPLATFORM)
endif

#release skbmgr
ifeq ($(strip $(TCSUPPORT_CT)),)
	cp -rf $(APP_SKB_MANAGER_DIR)/skbmgr  $(APP_BINARY_DIR)/$(TCPLATFORM)	
endif

#Release snmpd
ifneq ($(strip $(TCSUPPORT_SNMP)),)
	cp -rf $(APP_SNMPD_DIR)/agent/snmpd  $(APP_BINARY_DIR)/$(TCPLATFORM)
endif

#Release Zebra & ripd
	cp -rf $(APP_ZEBRA_DIR)/zebra/zebra  $(APP_BINARY_DIR)/$(TCPLATFORM)
	cp -rf $(APP_ZEBRA_DIR)/ripd/ripd  $(APP_BINARY_DIR)/$(TCPLATFORM)
ifneq ($(strip $(TCSUPPORT_IPV6_RIPNG)),)
	cp -rf $(APP_ZEBRA_DIR)/ripngd/ripngd $(APP_BINARY_DIR)/$(TCPLATFORM)
endif
#Release tcapi
	cp -rf $(APP_TCAPI_DIR)/tcapi  $(APP_BINARY_DIR)/$(TCPLATFORM)

#Release rstats
	cp -rf $(APP_RSTATS_DIR)/rstats  $(APP_BINARY_DIR)/$(TCPLATFORM)

#Release access log
	cp -rf $(APP_ACCESSLOG_DIR)/access_log  $(APP_BINARY_DIR)/$(TCPLATFORM)

##Release infosvr
	cp -rf $(APP_INFOSVR_DIR)/infosvr  $(APP_BINARY_DIR)/$(TCPLATFORM)

#Release monitorcfgmgr
	cp -rf $(APP_MONITORCFGMGR_DIR)/monitorcfgmgr  $(APP_BINARY_DIR)/$(TCPLATFORM)

#Release spectrum
ifneq ($(strip $(ASUS_SPECTRUM)),)
	cp -rf $(APP_SPECTRUM_DIR)/spectrum  $(APP_BINARY_DIR)/$(TCPLATFORM)
endif

#Release sysstate
ifneq ($(strip $(ASUS_SYSSTATE)),)
	cp -rf $(APP_SYSSTATE_DIR)/sysstate  $(APP_BINARY_DIR)/$(TCPLATFORM)
endif

#Release auto_det
	cp -rf $(APP_AUTO_DET_DIR)/auto_det  $(APP_BINARY_DIR)/$(TCPLATFORM)

#Release tcwdog	
	cp -rf $(APP_TCWDOG_DIR)/tcwdog  $(APP_BINARY_DIR)/$(TCPLATFORM)

#Release Driver cmd interface release	
	cp -rf $(APP_CMDCI_DIR)/algcmd  $(APP_BINARY_DIR)/$(TCPLATFORM)
	cp -rf $(APP_CMDCI_DIR)/firewallcmd  $(APP_BINARY_DIR)/$(TCPLATFORM)
	cp -rf $(APP_CMDCI_DIR)/qoscmd	$(APP_BINARY_DIR)/$(TCPLATFORM)
	cp -rf $(APP_CMDCI_DIR)/portbindcmd	$(APP_BINARY_DIR)/$(TCPLATFORM)
ifneq ($(strip $(TCSUPPORT_PRODUCTIONLINE)),)
	cp -rf $(APP_CMDCI_DIR)/prolinecmd $(APP_BINARY_DIR)/$(TCPLATFORM)
endif
ifneq ($(strip $(TCSUPPORT_CT)),)
	cp -rf $(APP_CMDCI_DIR)/accesslimitcmd $(APP_BINARY_DIR)/$(TCPLATFORM)
	cp -rf $(APP_CMDCI_DIR)/atmcmdd $(APP_BINARY_DIR)/$(TCPLATFORM)
endif

#Release igmpproxy
ifneq ($(strip $(TCSUPPORT_IGMP_PROXY_V3)),)
	cp -rf $(APP_IGMPPROXY_DIR)/igmpproxy  $(APP_BINARY_DIR)/$(TCPLATFORM)
endif

#Release hw_nat
ifneq ($(strip $(TCSUPPORT_RA_HWNAT)),)
ifeq ($(strip $(TCSUPPORT_MT7510_FE)),)
	cp -rf $(APP_RA_HWNAT_DIR)/ac  $(APP_BINARY_DIR)/$(TCPLATFORM)
	cp -rf $(APP_RA_HWNAT_DIR)/acl  $(APP_BINARY_DIR)/$(TCPLATFORM)
	cp -rf $(APP_RA_HWNAT_DIR)/mtr  $(APP_BINARY_DIR)/$(TCPLATFORM)
	cp -rf $(APP_RA_HWNAT_DIR)/hw_nat  $(APP_BINARY_DIR)/$(TCPLATFORM)
else
	cp -rf $(APP_RA_HWNAT_7510_DIR)/hw_nat  $(APP_BINARY_DIR)/$(TCPLATFORM)
endif
endif
	
#Release voip_api
ifneq ($(strip $(TCSUPPORT_VOIP)),)
	cp -rf $(APP_TC_VOIP_API_DIR)/bin/tcVoIPApiServer  $(APP_BINARY_DIR)/$(TCPLATFORM)
endif

#Release mtf
ifneq ($(strip $(TCSUPPORT_VOIP)),)
	cp -rf $(APP_MTF_DIR)/binaries/linux_redhat-AS/Release/bin/TC_MTF   $(APP_BINARY_DIR)/$(TCPLATFORM)
endif

#Release smuxctl
ifneq ($(strip $(TCSUPPORT_SMUX)),)
	cp -rf $(APP_SMUXCTL_DIR)/smuxctl  $(APP_BINARY_DIR)/$(TCPLATFORM)
endif

#Release dms
ifneq ($(strip $(TCSUPPORT_DMS)),)
	cp -rf  $(APP_DMS_DIR)/dlna_output $(APP_BINARY_DIR)/$(TCPLATFORM)/dlna
endif

#Release pwctlcmd
ifneq ($(strip $(TCSUPPORT_CPU_RT65168)),)
	cp -rf $(APP_PWCTLCMD_DIR)/pwctlcmd  $(APP_BINARY_DIR)/$(TCPLATFORM)
endif

#Release ra_menu
ifneq ($(strip $(TCSUPPORT_RA_MENU)),)
	cp -rf $(APP_RA_MENU_DIR)/ra_menu $(APP_BINARY_DIR)/$(TCPLATFORM)/ra_menu
endif

#release backuprestore
ifneq ($(strip $(TCSUPPORT_CT)),)
ifneq ($(strip $(TCSUPPORT_CT_USB_BACKUPRESTORE)),)
	cp -rf $(APP_BACKUPRESTORE_DIR)/backuprestorecmd $(APP_BINARY_DIR)/$(TCPLATFORM)
endif
endif

#release cwmp
ifneq ($(strip $(TCSUPPORT_CWMP)),)
	mkdir -p $(APP_CWMP_DIR)/$(TCPLATFORM)
	$(MAKE) -C $(APP_CWMP_DIR) -f $(APP_CWMP_DIR)/Makefile.release clean
	cp -rf $(APP_CWMP_DIR)/*.o $(APP_CWMP_DIR)/$(TCPLATFORM)
endif

#tcapi library
	mkdir -p $(APP_TCAPILIB_DIR)/$(TCPLATFORM)
	cp -rf $(APP_TCAPILIB_DIR)/*.o  $(APP_TCAPILIB_DIR)/$(TCPLATFORM)
	cp -rf $(APP_TCAPILIB_DIR)/libtcapi.so  $(APP_TCAPILIB_DIR)/$(TCPLATFORM)
	cp -rf $(APP_TCAPILIB_DIR)/libtcapi.so.*  $(APP_TCAPILIB_DIR)/$(TCPLATFORM)

#release cfg_manager.o
ifeq ($(strip $(TCSUPPORT_CT)),)
	mkdir -p $(APP_CFG_MANAGER_DIR)/$(TCPLATFORM)
	cp -rf $(APP_CFG_MANAGER_DIR)/cfg_manager.o $(APP_CFG_MANAGER_DIR)/$(TCPLATFORM)
else
	cp $(APP_PRIVATE_DIR)/cfg_manager_ct/cfg_manager  $(APP_BINARY_DIR)/$(TCPLATFORM)/ 
endif

#release IgdClmp.o ad IgdStubsClmp.o with and with out WLAN option.	
TC_CFLAGS_NO_WLAN=$(patsubst -DTCSUPPORT_WLAN,,$(TC_CFLAGS))
export TC_CFLAGS_NO_WLAN

release_clmp:
	echo "release IgdClmp and IgdStubsClmp..."
	mkdir -p $(APP_CWMP_DIR)/$(TCPLATFORM)/
	$(MAKE) -C $(APP_CWMP_DIR) clean
	$(MAKE) -C $(APP_CWMP_DIR) TC_CFLAGS="$(TC_CFLAGS_NO_WLAN)"
	cp -f $(APP_CWMP_DIR)/IgdClmp.o $(APP_CWMP_DIR)/$(TCPLATFORM)/IgdClmp_noWlan.o
	cp -f $(APP_CWMP_DIR)/IgdStubsClmp.o $(APP_CWMP_DIR)/$(TCPLATFORM)/IgdStubsClmp_noWlan.o

release_script:
	echo "release etc_script now..."
	mkdir -p $(APP_PRIVATE_DIR)/etc_script.reserved/
	


	
#if !defined(TCSUPPORT_TTNET)
ifneq ($(strip $(TCSUPPORT_TTNET)),)
	mkdir -p $(APP_PRIVATE_DIR)/etc_script.reserved/ttnet/
	cp -rf $(APP_PRIVATE_DIR)/etc_script/ttnet/* $(APP_PRIVATE_DIR)/etc_script.reserved/ttnet/
endif
#endif

release_webpage:
	echo "release webpage...."
#copy tc webpage by default
	mkdir -p $(APP_WEBPAGE_DIR).reserved/Router/tc/
	cp -rf  $(APP_ROUTE_WEBPAGE_DIR)  $(APP_WEBPAGE_DIR).reserved/Router/tc/

ifneq ($(strip $(TCSUPPORT_GENERAL_MULTILANGUAGE)),)
	mkdir -p $(APP_WEBPAGE_DIR).reserved/Router/ml
	cp -rf $(APP_WEBPAGE_DIR)/Router/ml/boaroot $(APP_WEBPAGE_DIR).reserved/Router/ml
endif






#copy led conf file, this "interface is left for future use"
release_led_conf:
	echo "copy led conf..."
	mkdir -p $(APP_PRIVATE_DIR)/led_conf.reserved	

release_chk:
	if test -d $(APP_PRIVATE_DIR)/TR69_64/clmp; \
	then echo "Origin Src Code"; \
	else echo "Release Src Code, you can not release second time!"; exit 1;\
	fi



