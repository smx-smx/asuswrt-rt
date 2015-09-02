######################################################################################
#This Makefile is the main entrance for MediaTek CPE SDK maintaining,including:
#firmware building, SDK release, customer SDK release and other test functions for SDK.
#######################################################################################
#######################################################################################
#In 3 cases you need to pay attention to this makefile:
#(1).You have added or modified private source code created by MediaTeK which can not be released to customers directly but can be provided in object format.
#(2).You will be responsbile for new version SDK release, which means you need to update or upgrade this makefile.
#(3).You need to add special target for SDK maintaining or etc.
########################################################################################

#Entrance for firmware building, which will provide default target if no arguments specified.
ifeq ($(strip $(RELEASEBSP)),)
ifeq ($(strip $(CUSTOMERRELEASE)),)
include Project/MakeFile_Main
endif
endif

#Entrance for SDK release (by MediaTek).
ifneq ($(strip $(RELEASEBSP)),)	
ifeq ($(strip $(CUSTOMERRELEASE)),)
release:confirm_infor release_all_check release_backup release_profile release_makefile release_config release_doc release_swept release_tar
endif
endif

#Entrance for customer SDK release (by customers)
ifeq ($(strip $(RELEASEBSP)),)	
ifneq ($(strip $(CUSTOMERRELEASE)),)
customer_release:confirm_infor customer_release_backup customer_release_makefile customer_release_swept customer_release_tar
endif
endif

#Error arguments
ifneq ($(strip $(RELEASEBSP)),)	
ifneq ($(strip $(CUSTOMERRELEASE)),)
error_conflict:
	echo "Error argument! You can not release with RELEASEBSP and CUSTOMERRELEASE at the same time!"
	exit 1;
endif
endif

#PROFILE_SET are the set of release profile supported in this release version 
ifneq ($(strip $(KERNEL_2_6_36)),)
PROFILE_SET=rt65168_release rt63365_release rt63368_release mt7510_release mt7520_release
else
PROFILE_SET=tc3162u_release rt65168_release rt63260_release tc3182_release rt63365_release rt63368_release
endif

#CUSTOM is the target custom that will be released, and the default custom will be RT.
ifeq ($(strip $(CUSTOM)),)
CUSTOM=RT
endif

include Project/dir.mak

#UNOPEN_FILE_DELETE means unopen files needed to be deleted
UNOPEN_FILE_DELETE=$(MODULES_PRIV_SRC_DIR)/auto_bench \
$(MODULES_PRIV_SRC_DIR)/sif \
$(MODULES_PRIV_SRC_DIR)/hwnat \
$(MODULES_PRIV_SRC_DIR)/HWNAT_Emulation  \
$(MODULES_PRIV_SRC_DIR)/HWNAT_Emulation_Accelerated  \
$(MODULES_PRIV_SRC_DIR)/net  \
$(MODULES_PRIV_SRC_DIR)/voip/le88drv \
$(MODULES_PRIV_SRC_DIR)/voip/module_test \
$(MODULES_PRIV_SRC_DIR)/voip/slic_driver \
$(MODULES_PRIV_SRC_DIR)/voip/tcRtp \
$(MODULES_PRIV_SRC_DIR)/voip/voice_DSP \
$(MODULES_PRIV_SRC_DIR)/voip/telephony_drv/zarlink/VE890_ACA_Rev2_5profiles8.c \
$(MODULES_PRIV_SRC_DIR)/voip/pcm_slic/queue.c \
$(MODULES_PRIV_SRC_DIR)/voip/pcm_slic/pcmdriver.c \
$(MODULES_PRIV_SRC_DIR)/voip/telephony_drv/zarlink/vp_api_ii/common/*.c \
$(MODULES_PRIV_SRC_DIR)/voip/telephony_drv/zarlink/vp_api_ii/custom/*.c \
$(MODULES_PRIV_SRC_DIR)/voip/telephony_drv/zarlink/vp_api_ii/vp890_api/*.c \
$(MODULES_PRIV_SRC_DIR)/voip/telephony_drv/zarlink/vp_api_ii/vp580_api \
$(MODULES_PRIV_SRC_DIR)/voip/telephony_drv/zarlink/vp_api_ii/vp790_api \
$(MODULES_PRIV_SRC_DIR)/voip/telephony_drv/zarlink/vp_api_ii/vp880_api \
$(MODULES_PRIV_SRC_DIR)/voip/telephony_drv/zarlink/vp_api_ii/vpp_api \
$(MODULES_PRIV_SRC_DIR)/voip/telephony_drv/silab/tic_silabs_api/custom/*.c \
$(MODULES_PRIV_SRC_DIR)/voip/telephony_drv/silab/tic_silabs_api/src/*.c \
$(APP_AUTOMOUNT_DIR)\
$(APP_RESTORE_LINOS_INFO_DIR) \
$(APP_SNMPD_DIR) \
$(APP_ZEBRA_DIR) \
$(APP_TCAPI_DIR) \
$(APP_TCWDOG_DIR) \
$(APP_TC_VOIP_API_DIR)/voip_api_client/tcVoIPAPI.c \
$(APP_TC_VOIP_API_DIR)/voip_api_client/tcVoIPAPIClient.c \
$(APP_TC_VOIP_API_DIR)/voip_api_server/*.* \
$(APP_TC_VOIP_API_DIR)/voip_api_server/DSP_API/EVA/*.* \
$(APP_TC_VOIP_API_DIR)/voip_api_server/DSP_API/AC \
$(APP_TC_VOIP_API_DIR)/voip_api_server/DSP_API/D2/EDDY1_R_1_5_99 \
$(APP_TC_VOIP_API_DIR)/voip_api_server/DSP_API/D2/EDDY1_R_1_5_99_TDI \
$(APP_TC_VOIP_API_DIR)/voip_api_server/DSP_API/D2/tcDSPAPI.c \
$(APP_TC_VOIP_API_DIR)/voip_api_server/DSP_API/D2/Makefile \
$(APP_TC_VOIP_API_DIR)/voip_api_server/DSP_API/D2/EDDY1_R_1_5_105_1_final/Makefile \
$(APP_TC_VOIP_API_DIR)/voip_api_server/DSP_API/D2/EDDY1_R_1_5_105_1_final/build.m34k2 \
$(APP_TC_VOIP_API_DIR)/voip_api_server/DSP_API/D2/EDDY1_R_1_5_105_1_final/extern \
$(APP_TC_VOIP_API_DIR)/voip_api_server/DSP_API/D2/EDDY1_R_1_5_105_1_final/include.m34k2 \
$(APP_TC_VOIP_API_DIR)/voip_api_server/DSP_API/D2/EDDY1_R_1_5_105_1_final/lib.m34k2 \
$(APP_TC_VOIP_API_DIR)/voip_api_server/DSP_API/D2/EDDY1_R_1_5_105_1_final/make \
$(APP_TC_VOIP_API_DIR)/voip_api_server/DSP_API/D2/EDDY1_R_1_5_105_1_final/obj.m34k2 \
$(APP_TC_VOIP_API_DIR)/voip_api_server/DSP_API/D2/EDDY1_R_1_5_105_1_final/system \
$(APP_TC_VOIP_API_DIR)/voip_api_server/DSP_API/D2/EDDY1_R_1_5_105_1_final/tools \
$(APP_TC_VOIP_API_DIR)/voip_api_server/DSP_API/D2/EDDY1_R_1_5_105_1_final/ve/vtsp_ut \
$(APP_TC_VOIP_API_DIR)/voip_api_server/DSP_API/D2/EDDY1_R_1_5_105_1_final//ve/vtsp_hw/config.mk \
$(APP_TC_VOIP_API_DIR)/voip_api_server/DSP_API/D2/EDDY1_R_1_5_105_1_final//ve/vtsp_hw/Makefile \
$(APP_TC_VOIP_API_DIR)/voip_api_server/DSP_API/D2/EDDY1_R_1_5_105_1_final//ve/vtsp_hw/vhw_silabs0.m34k2 \
$(APP_TC_VOIP_API_DIR)/voip_api_server/DSP_API/D2/EDDY1_R_1_5_105_1_final//ve/vtsp_hw/tic_silabs_api \
$(MODULES_HWNAT_DIR) \
$(MODULES_RA_HWNAT_DIR) \
$(APP_RA_HWNAT_DIR) \
$(APP_SMUXCTL_DIR) \
$(APP_CENTRAL_COORDINATOR_DIR) \
$(APP_CFG_PARSER_DIR) \
$(APP_CODE_REDUCE_PATCH_DIR) \
$(APP_FWUPGRADE_DIR) \
$(APP_HUAWEI_CI_DIR) \
$(APP_PRIVATE_DIR)/igmpv3proxy \
$(APP_SNMPD_DIR) \
$(APP_PCM_TEST_DIR) \
$(APP_QUAGGA_DIR) \
$(APP_RCS_DIR) \
$(APP_MTF_DIR) \
$(APP_PRIVATE_DIR)/RV_MTF \
$(APP_SLIC_TEST_DIR) \
$(APP_SKB_MANAGER_DIR) \
$(APP_PWCTLCMD_DIR) \
$(APP_DMS_DIR) \
$(APP_RA_MENU_DIR) \
$(APP_AUTOIP_DIR) \
$(TRUNK_DIR)/buildfs_vs_fw_table.xls \
$(TRUNK_DIR)/rootfs \
$(TRUNK_DIR)/README \
$(TRUNK_DIR)/bootrom_int \
$(TRUNK_DIR)/modules_tmp/ko/modules/2.6.22.15 \
$(MODULES_PRIV_SRC_DIR)/tc3162l2hp2h/SAR_VERIFY \
$(MODULES_PRIV_SRC_DIR)/tc3162l2hp2h/*_verify.c \
$(MODULES_PRIV_SRC_DIR)/tc3162l2hp2h/tc3162_udc20.c \
$(MODULES_PRIV_SRC_DIR)/tc3162l2hp2h/tc3162_udc.c \
$(APP_CFG_MANAGER_DIR)/cfg_manager.c \
$(APP_CFG_MANAGER_DIR)/*.o \
$(APP_TCAPILIB_DIR)/*.c \
$(APP_CWMP_DIR)/clmp \
$(APP_CWMP_DIR)/cwmp/Sources \
$(APP_CWMP_DIR)/main/main.c \
$(APP_CWMP_DIR)/rompager \
$(APP_CWMP_DIR)/utily.c \
$(APP_CMDCI_DIR)/algcmd.* \
$(APP_CMDCI_DIR)/firewallcmd.* \
$(APP_CMDCI_DIR)/portbindcmd.* \
$(APP_CMDCI_DIR)/qoscmd.* \
$(APP_CMDCI_DIR)/accesslimitcmd \
$(APP_CMDCI_DIR)/accesslimitcmd.* \
$(APP_CMDCI_DIR)/atmcmdd \
$(APP_CMDCI_DIR)/atmcmdd.* \
$(APP_CMDCI_DIR)/atmcmdc.* \
$(FILESYSTEM_DIR)/boaroot/cgi-bin/WizardOi \
$(FILESYSTEM_DIR)/boaroot/html/include	\
$(BOOTROM_DIR)/ddr_cal/*.c \
$(MODULES_RA_HWNAT_7510_DIR) \
$(APP_RA_HWNAT_7510_DIR)


#delete adsl or vdsl code for mt7520
UNOPEN_FILE_DELETE += $(MODULES_PRIV_SRC_DIR)/epon
ifneq ($(strip $(TCSUPPORT_CPU_MT7520)),)
UNOPEN_FILE_DELETE += $(MODULES_PRIV_SRC_DIR)/dmt \
$(MODULES_PRIV_SRC_DIR)/mt7510_ptm \
$(MODULES_PRIV_SRC_DIR)/mtk_sar \
$(MODULES_PRIV_SRC_DIR)/tc3262
endif

#delete linux kernel dir for this release don't support the other kernel to compile
ifneq ($(strip $(KERNEL_2_6_36)),)
UNOPEN_FILE_DELETE += $(TRUNK_DIR)/linux
else
UNOPEN_FILE_DELETE += $(TRUNK_DIR)/linux-2.6.36
endif

#delete extra code of CT if customer is not CT and do the other when CT defined 
ifneq ($(strip $(CUSTOM)),CT)
UNOPEN_FILE_DELETE += $(APP_PRIVATE_DIR)/cfg_manager_ct \
$(APP_PRIVATE_DIR)/ethcmd_ct \
$(APP_PRIVATE_DIR)/TR69_64/cwmp/cwmpParameter_ct \
$(APP_PRIVATE_DIR)/backuprestore 
else
UNOPEN_FILE_DELETE += $(APP_PRIVATE_DIR)/cfg_manager \
$(APP_PRIVATE_DIR)/cfg_manager_ct \
$(APP_PRIVATE_DIR)/ethcmd \
$(APP_PRIVATE_DIR)/TR69_64/cwmp/cwmpParameter
endif

OTHER_FILE_DELETE= \
$(PROJECT_DIR)/images/tcboot.bin \
$(PROJECT_DIR)/images/tclinux.bin \
$(PROJECT_DIR)/images/tclinux_allinone \
$(PROJECT_DIR)/images/tclinux_allinone.swap \
$(PROJECT_DIR)/config/menuconfig/*.config \
$(FILESYSTEM_DIR)/dev/* \
$(TRUNK_DIR)/backup \
$(MODULES_PRIV_SRC_DIR)/wifi/ATHR-GEMINI-AP9.0.0.33.tgz \
$(MODULES_PRIV_SRC_DIR)/wifi/2008_0506_RT61_Linux_SoftAP_Drv1.1.3.0 \
$(MODULES_PRIV_SRC_DIR)/wifi/2009_0209_RT3090_LinuxAP_V2.1.3.0 \
$(MODULES_PRIV_SRC_DIR)/wifi/2009_0904_RT3090_LinuxAP_v2.3.0.0_TC_for_3092 \

#Backup_Folder are the backup location for release requisite
BACKUP_FOLDERS=$(APP_BINARY_DIR) \
$(APP_CFG_MANAGER_DIR) \
$(MODULES_PRIV_SRC_DIR)/ko \
$(APP_TCAPILIB_DIR) \
$(APP_PRIVATE_DIR)/TR69_64 \


INFOR0="Attention!!!!"
ifneq ($(strip $(KERNEL_2_6_36)),)
INFOR1="This command can only release SDK with 2.6.36.0 kernel"
INFOR2="If you want to release 2.6.22.15 kernel, please use cmd: make RELEASEBSP=y"
else
INFOR1="This command can only release SDK with 2.6.22.15 kernel"
INFOR2="If you want to release 2.6.36 kernel, please use cmd: make RELEASEBSP=y KERNEL_2_6_36=y"
endif
INFOR3="This command will release Multi-chip SDK, which will remove the un-open files and can't be recovered!!!!!"
INFOR4="Please select the release profile desired. Example: 1 2 3"
INFOR5="Please input your selection:"
INFOR6="Profile you selected is as following:"
INFOR7="if you agree, please input "y",else if you wanna break please input "N". [y/N] ?"
EXIT_INFOR="Your input is not y! The make process will be breaked!"
CONTINUE_INFOR="Entering make process......"
INFOR_CUSTOM="If you wanan release a customer version from SDK, please make sure your command is: "make -f MultiChip.make CUSTOM_RELASE=y""
SEPARATOR="....................................................................................................."


#if RD wanna release with specified profile, the select and confirm will be canceld
ifneq ($(strip $(PROFILE)),)
PROFILE_SELECT = $(PROFILE)
confirm_infor:
	@echo "release with $(PROFILE) profile now..."
	@sleep 1
else
#if RD wanna release with some dedicated profiles, the select and confirm will be triggerd
confirm_infor:
	@echo "****************************************************************************************************"
	@echo $(INFOR0)
	@echo "....................................................................................................."
	@echo $(INFOR1)
	@echo $(INFOR2)
	@echo "....................................................................................................."
	@echo $(INFOR3)
	@echo $(INFOR4)
	@id=0;\
	for profile in $(PROFILE_SET);\
	do \
		echo [$$id] $$profile;\
		id=`expr $$id + 1`; \
	done;
	@echo "****************************************************************************************************"
	@echo $(INFOR5);
	@rm -rf "./.profile_select.tmp";
	@profile_set=(${PROFILE_SET}); \
	PROFILE_SET_NUM=$${profile_set[@]}; \
	read select;\
	echo $(SEPARATOR);\
	while [ "$$select"x == ""x ] ;\
	do \
		echo $(INFOR5);read select;\
	done; \
	while [ "$$select"x == ""x ] ;\
	do \
		fail=0;\
		for index in $$select; \
		do \
			if [[ ( $$index -lt 0 ) || ( $$index -ge $${PROFILE_SET_NUM} ) ]]; \
			then\
				echo "******Wrong input! Please check again!*****"; \
				fail=1; \
				echo $(INFOR5);read select;\
				while [ "$$select"x == ""x ]];\
				do \
					echo $(INFOR5);read select;\
				done; \
				break; \
			fi; \
		done ;\
		if [ $$fail -eq 1 ];\
		then \
			continue; \
		else \
			break; \
		fi; \
	done;\
	echo $(INFOR6);\
	for index in $$select; \
	do \
		echo "******$${profile_set[$$index]}*****"; \
		echo $${profile_set[$$index]}>>"./.profile_select.tmp"; \
	done
	@echo $(INFOR7);
	@read var ;\
	if [[ $$var == y ]]; \
	then \
		echo $(CONTINUE_INFOR); \
	else \
		echo "" >"./.profile_select.tmp";\
		echo $(EXIT_INFOR);\
		exit 1;\
	fi
	@echo $(SEPARATOR)
	@sleep 1;

PROFILE_SELECT=`cat ./.profile_select.tmp`
endif	

release_all_check:release_checktools
	rm -rf ./.profile_summary
ifneq ($(strip $(KERNEL_2_6_36)),)
	for release in ${PROFILE_SELECT} ; \
	do \
			cat Project/profile/$(CUSTOM)/$${release}/$${release}_2_6_36.profile >>./.profile_summary ;\
	done
else
	for release in ${PROFILE_SELECT} ; \
	do \
		cat Project/profile/$(CUSTOM)/$${release}/$${release}.profile >>./.profile_summary ;\
	done
endif


	if test -e $(TOOLS_DIR)/cplopts_checktool/tmp/restore.sh; \
	then echo "Please restore the files first before any modification!"; exit 1; \
	else echo "executing all check"; \
	fi

ifneq ($(strip $(CHKDIR)),)
	echo "checking CHDIR"
	$(TOOLS_DIR)/cplopts_formcheck $(TRUNK_DIR)/$(CHKDIR) ./.profile_summary
else
	echo "checking all files in trunk dir"
	$(TOOLS_DIR)/cplopts_formcheck $(TRUNK_DIR) ./.profile_summary
endif
	if test -e $(TOOLS_DIR)/cplopts_checktool/tmp/error; \
	then echo "Compile code error"; exit 1; \
	else echo "Compile code success"; \
	fi

	rm -rf ./.profile_summary 

release_restore:release_checktools
	if test -e $(TOOLS_DIR)/cplopts_checktool/tmp/restore.sh; \
	then echo "Compile bootbase success"; \
	/bin/sh $(TOOLS_DIR)/cplopts_checktool/tmp/restore.sh; \
	rm -f $(TOOLS_DIR)/cplopts_checktool/tmp/restore.sh; \
	rm -rf $(TRUNK_DIR)/backup; \
	fi
	echo "Files  restored!"

release_checktools:
	echo making compile options checking  tools
#       make -C $(TOOLS_DIR)/cplopts_checktool clean
	make -C $(TOOLS_DIR)/cplopts_checktool
	cp -f $(TOOLS_DIR)/cplopts_checktool/cplopts_formcheck $(TOOLS_DIR)
	cp -f $(TOOLS_DIR)/cplopts_checktool/cplopts_remove $(TOOLS_DIR)

release_checktools_clean:
	echo cleaning compile options checking  tools
	make -C $(TOOLS_DIR)/cplopts_checktool clean


#this target will do "make" process for SDK release, with loop of clean>>>make profile[i]>>>backup *.ko , *.o>>> clean....
release_backup:
	cp -rf $(TRUNK_DIR)/filesystem $(TRUNK_DIR)/filesystem_clean;
ifneq ($(strip $(KERNEL_2_6_36)),)
	for release in ${PROFILE_SELECT} ; \
	do \
		echo "*************************************************"; \
		echo "*************************************************"; \
		echo "*****$$release making and backup in progress********"; \
		echo "*************************************************"; \
		sleep 3;\
		echo "*************************************************"; \
		${MAKE} -f "$(PROJECT_DIR)/MakeFile_Main" PROFILE=$${release} KERNEL_2_6_36=1 CUSTOM=${CUSTOM} clean ; if [ $$? != 0 ];then exit 1;fi; \
		echo "*************************************************"; \
		${MAKE} -f "$(PROJECT_DIR)/MakeFile_Main"  PROFILE=$${release} KERNEL_2_6_36=1 CUSTOM=${CUSTOM} RELEASEBSP=y bootbase kernel modules apps; if [ $$? != 0 ];then exit 1;fi; \
		echo "*************************************************"; \
		${MAKE} -f "$(PROJECT_DIR)/MakeFile_Main" PROFILE=$${release} KERNEL_2_6_36=1 CUSTOM=${CUSTOM} RELEASEBSP=y release_end;if [ $$? != 0 ];then exit 1;fi; \
		echo "**************************************************"; \
		${MAKE} -f "$(PROJECT_DIR)/MakeFile_Main" PROFILE=$${release} KERNEL_2_6_36=1 CUSTOM=${CUSTOM} clean ; if [ $$? != 0 ];then exit 1;fi; \
		echo "**************************************************"; \
	done
else
	for release in ${PROFILE_SELECT} ; \
	do \
		echo "*************************************************"; \
		echo "*************************************************"; \
		echo "*****$$release making and backup in progress********"; \
		echo "*************************************************"; \
		sleep 3;\
		echo "*************************************************"; \
		${MAKE} -f "$(PROJECT_DIR)/MakeFile_Main" PROFILE=$${release} CUSTOM=${CUSTOM} clean ; if [ $$? != 0 ];then exit 1;fi; \
		echo "*************************************************"; \
		${MAKE} -f "$(PROJECT_DIR)/MakeFile_Main"  PROFILE=$${release} CUSTOM=${CUSTOM} RELEASEBSP=y kernel modules apps; if [ $$? != 0 ];then exit 1;fi; \
		echo "*************************************************"; \
		${MAKE} -f "$(PROJECT_DIR)/MakeFile_Main" PROFILE=$${release} CUSTOM=${CUSTOM} RELEASEBSP=y release_end;if [ $$? != 0 ];then exit 1;fi; \
		echo "**************************************************"; \
		${MAKE} -f "$(PROJECT_DIR)/MakeFile_Main" PROFILE=$${release} CUSTOM=${CUSTOM} clean ; if [ $$? != 0 ];then exit 1;fi; \
		echo "**************************************************"; \
	done
endif

	rm -rf $(TRUNK_DIR)/filesystem;
	mv -f  $(TRUNK_DIR)/filesystem_clean $(TRUNK_DIR)/filesystem

#Make sure: (1)Profile locates in CUSTOM has been removed and (2) Profile that not named with *_demo in CUSTOM foder have been removed
release_profile:
	find $(TRUNK_DIR)/Project/profile/* -maxdepth 0 -type d -not -name "$(CUSTOM)" | xargs rm -rf
	find $(TRUNK_DIR)/Project/profile/$(CUSTOM)/* -maxdepth 0 -not -name '*_demo' | xargs rm -rf
	

#Substitue makefile in special folder. These targets mostly maintain different codes before and after SDK release, and hence need to use makefile.relese instead.
release_makefile:
	#copy makefiles
	cp $(MODULES_PRIV_SRC_DIR)/tc3162l2hp2h/Makefile.release $(MODULES_PRIV_SRC_DIR)/tc3162l2hp2h/Makefile
ifeq ($(strip $(TCSUPPORT_CT)),)
	cp $(APP_CFG_MANAGER_DIR)/Makefile.release $(APP_CFG_MANAGER_DIR)/Makefile
endif
ifneq ($(strip $(TCSUPPORT_VOIP)),)	
	cp $(MODULES_PRIV_SRC_DIR)/voip/pcm_slic/Makefile.release $(MODULES_PRIV_SRC_DIR)/voip/pcm_slic/Makefile
	cp $(MODULES_PRIV_SRC_DIR)/voip/telephony_drv/Makefile.release $(MODULES_PRIV_SRC_DIR)/voip/telephony_drv/Makefile
endif
	cp $(APP_TCAPILIB_DIR)/Makefile.release $(APP_TCAPILIB_DIR)/Makefile

	cp $(APP_CMDCI_DIR)/Makefile.release $(APP_CMDCI_DIR)/Makefile
	cp $(APP_TC_VOIP_API_DIR)/Makefile.release $(APP_TC_VOIP_API_DIR)/Makefile
	cp $(APP_CWMP_DIR)/Makefile.release $(APP_CWMP_DIR)/Makefile
	cp $(BOOTROM_DIR)/ddr_cal/Makefile.release $(BOOTROM_DIR)/ddr_cal/Makefile

#clear up the documents in SDK
release_doc:
	chmod 777 -R $(TRUNK_DIR)/doc
	cp -f $(TRUNK_DIR)/doc/MediaTek_Linux_SDK_Release_Notes.pdf $(TRUNK_DIR)/
	find $(TRUNK_DIR)/doc -mindepth 1 -maxdepth 1 -not -name "*SOP" -not -name "*MANUAL" | xargs rm -rf
	find $(TRUNK_DIR)/doc/SOP -type f -not -name "*.pdf" |xargs rm -rf
	find $(TRUNK_DIR)/doc/MANUAL -type f -not -name "*.pdf" |xargs rm -rf
	
#This target is to add key parameter in Config.in to indicate that the code package has been released. The option "TCSUPPORT_RELEASED_CODE" will be used in controlling of customer configure.
release_config:
	echo "choice" >> Project/config/menuconfig/Config.in
	echo "	  prompt \"Release Type\""  >> Project/config/menuconfig/Config.in
	echo "config TCSUPPORT_RELEASED_CODE" >> Project/config/menuconfig/Config.in
	echo "    bool \"TCSUPPORT_RELEASED_CODE RT\"" >> Project/config/menuconfig/Config.in
	echo "    help"  >> Project/config/menuconfig/Config.in
	echo "         The flag of code release" >> Project/config/menuconfig/Config.in
	echo "endchoice" >> Project/config/menuconfig/Config.in
ifneq ($(strip $(KERNEL_2_6_36)),)
	echo "config TC_RELEASE_2_6_36_KERNEL" >> Project/config/menuconfig/Config.in
	echo "    bool \"TC_RELEASE_2_6_36_KERNEL\"" >> Project/config/menuconfig/Config.in
	echo "		default y" >> Project/config/menuconfig/Config.in
else
	echo "config TC_RELEASE_2_6_22_15_KERNEL" >> Project/config/menuconfig/Config.in
	echo "    bool \"TC_RELEASE_2_6_22_15_KERNEL\"" >> Project/config/menuconfig/Config.in
	echo "		default y" >> Project/config/menuconfig/Config.in
endif
#swept unopen source codes,customerized scripts,webpages,configure profile and other extra files
release_swept:
	for FILE in $(UNOPEN_FILE_DELETE) $(OTHER_FILE_DELETE); \
	do\
		echo "******rm -rf $$FILE***********************************";\
		rm -rf $$FILE; \
		echo "******************************************************";\
	done \
	
	if [ $$(ls -A $(BOOTROM_DIR)/ddr_cal/reserved|wc -w) -gt 0 ]; then\
		cp -Rf $(BOOTROM_DIR)/ddr_cal/reserved/* $(BOOTROM_DIR)/ddr_cal/output/.;\
		cp -Rf $(BOOTROM_DIR)/ddr_cal/reserved/spram.c $(BOOTROM_DIR)/ddr_cal/.;\
		rm -rf $(BOOTROM_DIR)/ddr_cal/output/*.c;\
		rm -rf $(BOOTROM_DIR)/ddr_cal/reserved;\
	fi
	find $(APP_PRIVATE_DIR)/etc_script/* -type d  | xargs rm -rf
	if [ $$(ls -A $(APP_PRIVATE_DIR)/etc_script.reserved|wc -w) -gt 0 ]; then\
		cp -rf $(APP_PRIVATE_DIR)/etc_script.reserved/* $(APP_PRIVATE_DIR)/etc_script/ ;\
	fi
	rm -rf $(APP_PRIVATE_DIR)/etc_script.reserved

	find $(APP_WEBPAGE_DIR)/* -type d | xargs rm -rf
	if [ $$(ls -A $(APP_WEBPAGE_DIR).reserved|wc -w) -gt 0 ]; then\
		cp -rf $(APP_WEBPAGE_DIR).reserved/*  $(APP_WEBPAGE_DIR);\
	fi
	rm -rf $(APP_WEBPAGE_DIR).reserved
	
	find $(APP_PRIVATE_DIR)/led_conf/* -type d | xargs rm -rf
	if [ $$(ls -A $(APP_PRIVATE_DIR)/led_conf.reserved|wc -w) -gt 0 ]; then\
		cp -rf $(APP_PRIVATE_DIR)/led_conf.reserved/* $(APP_PRIVATE_DIR)/led_conf ;\
	fi
	rm -rf $(APP_PRIVATE_DIR)/led_conf.reserved

	find $(APP_IPTABLES_DIR) -name "*.d"  -not -name "." | xargs rm -rf
	find $(TRUNK_DIR) -name "CVS" | xargs rm -rf
#echo special keys to avoid duplicated release process.	
	echo "TC_BUILD_RELEASECODE=y" > Project/release.chk
	echo "export TC_BUILD_RELEASECODE" >>Project/release.chk
	
	
release_tar:
ifneq ($(strip $(CUSTOM)),)
	tar -czvf ../releasesdk_$(CUSTOM)_`date +%Y%m%d`.tgz .
else
	tar -czvf ../releasesdk_`date +%Y%m%d`.tgz .
endif


#release make for custom, similarly to release_make
customer_release_backup:
ifneq ($(strip $(KERNEL_2_6_36)),)
	for release in ${PROFILE_SELECT} ; \
	do \
		echo "*************************************************"; \
		echo "*************************************************"; \
		echo "*****$$release making and backup in progress********"; \
		echo "*************************************************"; \
		sleep 1;\
		echo "*************************************************"; \
		${MAKE} -f "$(PROJECT_DIR)/MakeFile_Main" PROFILE=$${release} KERNEL_2_6_36=1 CUSTOM=${CUSTOM} clean ; if [ $$? != 0 ];then exit 1;fi;\
		echo "*************************************************"; \
		${MAKE} -f "$(PROJECT_DIR)/MakeFile_Main" PROFILE=$${release} KERNEL_2_6_36=1 CUSTOM=${CUSTOM} CUSTOMERRELEASE=y bootbase kernel modules apps;if [ $$? != 0 ];then exit 1;fi; \
		echo "*************************************************"; \
		${MAKE} -f "$(PROJECT_DIR)/MakeFile_Main" PROFILE=$${release} KERNEL_2_6_36=1 CUSTOM=${CUSTOM} CUSTOMERRELEASE=y customer_release_end; if [ $$? != 0 ];then exit 1;fi;\
		echo "**************************************************"; \
		${MAKE} -f "$(PROJECT_DIR)/MakeFile_Main" PROFILE=$${release} KERNEL_2_6_36=1 CUSTOM=${CUSTOM} clean ;if [ $$? != 0 ];then exit 1;fi; \
		echo "**************************************************"; \
	done
else
	for release in ${PROFILE_SELECT} ; \
	do \
		echo "*************************************************"; \
		echo "*************************************************"; \
		echo "*****$$release making and backup in progress********"; \
		echo "*************************************************"; \
		sleep 1;\
		echo "*************************************************"; \
		${MAKE} -f "$(PROJECT_DIR)/MakeFile_Main" PROFILE=$${release} CUSTOM=${CUSTOM} clean ; if [ $$? != 0 ];then exit 1;fi;\
		echo "*************************************************"; \
		${MAKE} -f "$(PROJECT_DIR)/MakeFile_Main" PROFILE=$${release} CUSTOM=${CUSTOM} CUSTOMERRELEASE=y bootbase kernel modules apps;if [ $$? != 0 ];then exit 1;fi; \
		echo "*************************************************"; \
		${MAKE} -f "$(PROJECT_DIR)/MakeFile_Main" PROFILE=$${release} CUSTOM=${CUSTOM} CUSTOMERRELEASE=y customer_release_end; if [ $$? != 0 ];then exit 1;fi;\
		echo "**************************************************"; \
		${MAKE} -f "$(PROJECT_DIR)/MakeFile_Main" PROFILE=$${release} CUSTOM=${CUSTOM} clean ;if [ $$? != 0 ];then exit 1;fi; \
		echo "**************************************************"; \
	done
endif


customer_release_swept:
	rm -rf $(APP_CMDCI_DIR)
	rm -rf $(APP_ETHCMD_DIR)
	rm -rf $(APP_CWMP_DIR)
	rm -rf $(APP_TCCI_DIR)
	rm -rf $(APP_PRIVATE_DIR)/sendicmp/
	rm -rf $(FILESYSTEM_DIR)/dev/*
	rm -rf $(TRUNK_DIR)/$(MODULES_PRIV_SRC_DIR)/modules/2.6.22.15
	rm -rf $(TRUNK_DIR)/$(MODULES_PRIV_SRC_DIR)/modules/2.6.36
	
	find $(APP_CFG_MANAGER_DIR) -name "*.c"|grep -v 'init.c' |xargs rm -rf
	
	find $(APP_IPTABLES_DIR) -name "*.d" | xargs rm -rf
	echo "CUSTOMER_BUILD_RELEASECODE=y" > Project/customer_release/c_release.chk
	
customer_release_makefile:
	cp $(BOOTROM_DIR)/Makefile_C.release $(TRUNK_DIR)/bootrom/Makefile
	cp $(APP_CFG_MANAGER_DIR)/Makefile_C.release $(APP_CFG_MANAGER_DIR)/Makefile
	
customer_release_tar:
ifneq ($(strip $(CUSTOM)),)
	tar -czvf ../releasesdk_$(CUSTOM)_`date +%Y%m%d`.tgz .
else
	tar -czvf ../releasesdk_`date +%Y%m%d`.tgz .
endif

	
Option_Check:
ifeq ($(strip $(OPTION)),)
	@echo "Command input error!";
	@echo "Command example: sudo make PROFILE=tc3162u Option_Check OPTION=TCSUPPORT_XXX_SUPPORT";
	@exit 1;
endif
	@echo "Checking compile option "${OPTION}" in un-open files now and may last for sevreal minutes, please wait!";
	@for file in $(UNOPEN_FILE_DELETE);\
	do \
		echo "searching in $${file}";\
		find $${file} -name "*.c"|xargs grep -in "${OPTION}" >>"./.tmp_optioncheck"; \
		find $${file} -name "*.h"|xargs grep -in "${OPTION}" >>"./.tmp_optioncheck"; \
		echo ".........................................";\
	done

	@if [ -s "./.tmp_optioncheck" ]; then \
		echo "SORRY,YOUR CHECK HAS FAILED!!!!!!!!";\
		sleep 1;\
		echo "This compile option has been found in following files:";\
		cat "./.tmp_optioncheck";\
		rm -f "./.tmp_optioncheck";\
		exit 1;\
		else \
	echo "Congratulations, your check has PASSED! This compile option has not been found in un-open codes!";\
	echo "..............................................................................................";\
	fi
