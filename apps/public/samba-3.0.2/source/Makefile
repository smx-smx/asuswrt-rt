#########################################################################
# Makefile.in for Samba - rewritten for autoconf support
# Copyright Andrew Tridgell 1992-1998
# Copyright (C) 2001 by Martin Pool <mbp@samba.org>
# Copyright Andrew Bartlett 2002
# Copyright (C) 2003 Jim McDonough <jmcd@us.ibm.com>
# Copyright (C) 2002-2003 Jelmer Vernooij <jelmer@samba.org>
###########################################################################

prefix=
exec_prefix=${prefix}

LIBS= -lcrypt -lnsl -ldl
SHLD=${CC} ${CFLAGS}
CFLAGS= -O2
CPPFLAGS= 
EXEEXT=
LDFLAGS= 
AR=ar
LDSHFLAGS=-shared  -Wl
WINBIND_NSS_LDSHFLAGS=-shared  -Wl
AWK=gawk
DYNEXP=-Wl,--export-dynamic
PYTHON=
PERL=


TERMLDFLAGS=
TERMLIBS=
PRINT_LIBS=
AUTH_LIBS= 
ACL_LIBS= -lattr -lacl -L../../acl/libacl/.libs -L../../attr/libattr/.libs
PASSDB_LIBS=
IDMAP_LIBS=
KRB5LIBS=
LDAP_LIBS=

LINK=$(CC) $(FLAGS) $(LDFLAGS)

INSTALLCMD=install -c
INSTALLCLIENTCMD_SH=$(INSTALLCMD)
INSTALLCLIENTCMD_A=:

INSTALLCMD=$(ROMFSINST)

srcdir=.
builddir=.
SHELL=/bin/sh
DESTDIR=$(INSTALLDIR)

# XXX: Perhaps this should be /bin/sh instead -- apparently autoconf
# will search for a POSIX-compliant shell, and that might not be
# /bin/sh on some platforms.  I guess it's not a big problem -- mbp

# See the autoconf manual "Installation Directory Variables" for a
# discussion of the subtle use of these variables.

BASEDIR= /etc
BINDIR = ${exec_prefix}/bin
# sbindir is mapped to bindir when compiling SAMBA in 2.0.x compatibility mode.
SBINDIR = ${exec_prefix}/sbin
LIBDIR = ${exec_prefix}/lib
VFSLIBDIR = $(LIBDIR)
PDBLIBDIR = $(LIBDIR)
RPCLIBDIR = $(LIBDIR)
IDMAPLIBDIR = $(LIBDIR)
CHARSETLIBDIR = $(LIBDIR)
AUTHLIBDIR = $(LIBDIR)
CONFIGDIR = ${prefix}/etc
VARDIR = ${prefix}/var
MANDIR = ${prefix}/man
DATADIR = ${prefix}/share

# The permissions to give the executables
INSTALLPERMS = 0755

# set these to where to find various files
# These can be overridden by command line switches (see smbd(8))
# or in smb.conf (see smb.conf(5))
LOGFILEBASE = ${VARDIR}
CONFIGFILE = $(CONFIGDIR)/samba/smb.conf
LMHOSTSFILE = $(CONFIGDIR)/lmhosts

# This is where smbpasswd et al go
PRIVATEDIR = ${prefix}/etc/samba

SMB_PASSWD_FILE = $(PRIVATEDIR)/smbpasswd
PRIVATE_DIR = $(PRIVATEDIR)

# This is where SWAT images and help files go
SWATDIR = ${prefix}/swat

# the directory where lock files go
LOCKDIR = ${VARDIR}/lock

# the directory where pid files go
PIDDIR = ${VARDIR}/run
# man pages language(s)
man_langs = "en"

LIBSMBCLIENT=bin/libsmbclient.a bin/libsmbclient.so
LIBSMBCLIENT_MAJOR=0
LIBSMBCLIENT_MINOR=1


FLAGS1 = $(CFLAGS) -I./popt -Iinclude -I$(srcdir)/include -I$(srcdir)/ubiqx -I$(srcdir)/smbwrapper  -I. $(CPPFLAGS) -I$(srcdir) -I../../acl/include
FLAGS2 = 
FLAGS3 = 
FLAGS4 = 
FLAGS5 = $(FLAGS1) $(FLAGS2) $(FLAGS3) $(FLAGS4)
FLAGS  = $(ISA) $(FLAGS5) 

PASSWD_FLAGS = -DSMB_PASSWD_FILE=\"$(SMB_PASSWD_FILE)\" -DPRIVATE_DIR=\"$(PRIVATE_DIR)\"
PATH_FLAGS1 = -DCONFIGFILE=\"$(CONFIGFILE)\"  -DSBINDIR=\"$(SBINDIR)\"
PATH_FLAGS2 = $(PATH_FLAGS1) -DBINDIR=\"$(BINDIR)\" -DDRIVERFILE=\"$(DRIVERFILE)\" 
PATH_FLAGS3 = $(PATH_FLAGS2) -DLMHOSTSFILE=\"$(LMHOSTSFILE)\" 
PATH_FLAGS4 = $(PATH_FLAGS3) -DSWATDIR=\"$(SWATDIR)\"  -DLOCKDIR=\"$(LOCKDIR)\" -DPIDDIR=\"$(PIDDIR)\"
PATH_FLAGS5 = $(PATH_FLAGS4) -DLIBDIR=\"$(LIBDIR)\" \
	      -DLOGFILEBASE=\"$(LOGFILEBASE)\" -DSHLIBEXT=\"so\"
PATH_FLAGS6 = $(PATH_FLAGS5) -DCONFIGDIR=\"$(CONFIGDIR)\"
PATH_FLAGS = $(PATH_FLAGS6) $(PASSWD_FLAGS)

# Note that all executable programs now provide for an optional executable suffix.


SBIN_PROGS = bin/smbd bin/nmbd \
	     #bin/swat bin/winbindd$(EXEEXT)

#BIN_PROGS1 = bin/smbclient bin/smbmessage bin/net bin/smbspool \
	      bin/testparm bin/testprns bin/smbstatus bin/smbmount bin/smbmnt bin/smbumount
#BIN_PROGS2 = bin/smbcontrol bin/smbtree bin/tdbbackup \
	      bin/nmblookup bin/pdbedit bin/tdbdump bin/tdb_restore_secrets
BIN_PROGS3 = bin/smbpasswd bin/libbigballofmud.so \
	    #bin/rpcclient bin/smbcacls bin/profiles \
	     bin/ntlm_auth bin/smbcquotas 


# editreg removed from standard build until it is portable. It needs a major rewrite to 
# achieve this (tridge)
# bin/editreg

TORTURE_PROGS = bin/smbtorture bin/msgtest \
	bin/masktest bin/locktest \
	bin/locktest2 bin/nsstest bin/vfstest

BIN_PROGS = $(BIN_PROGS1) $(BIN_PROGS2) $(BIN_PROGS3)  #bin/wbinfo$(EXEEXT)


EVERYTHING_PROGS = bin/debug2html bin/smbfilter bin/talloctort \
	bin/log2pcap

SHLIBS =  nsswitch/libnss_winbind.so nsswitch/libnss_wins.so libsmbclient

SCRIPTS = $(srcdir)/script/smbtar $(builddir)/script/findsmb

VFS_MODULES =  bin/recycle.so bin/audit.so bin/extd_audit.so bin/netatalk.so bin/fake_perms.so bin/default_quota.so bin/readonly.so bin/cap.so
PDB_MODULES = 
RPC_MODULES = 
IDMAP_MODULES = 
CHARSET_MODULES =  bin/CP850.so bin/CP437.so
AUTH_MODULES = 
MODULES = $(VFS_MODULES) $(PDB_MODULES) $(RPC_MODULES) $(IDMAP_MODULES) $(CHARSET_MODULES) $(AUTH_MODULES)



######################################################################
# object file lists
######################################################################

TDBBASE_OBJ = tdb/tdb.o tdb/spinlock.o
TDB_OBJ = $(TDBBASE_OBJ) tdb/tdbutil.o tdb/tdbback.o

SMBLDAP_OBJ = 

LIB_OBJ = lib/version.o lib/charcnv.o lib/debug.o lib/fault.o \
          lib/getsmbpass.o lib/interface.o lib/md4.o \
          lib/interfaces.o lib/pidfile.o lib/replace.o lib/replace1.o \
          lib/signal.o lib/system.o lib/sendfile.o lib/time.o \
	  lib/ufc.o lib/genrand.o lib/username.o \
	  lib/util_getent.o lib/util_pw.o lib/access.o lib/smbrun.o \
	  lib/bitmap.o lib/crc32.o lib/snprintf.o lib/dprintf.o \
	  lib/xfile.o lib/wins_srv.o \
	  lib/util_str.o lib/clobber.o lib/util_sid.o lib/util_uuid.o \
	  lib/util_unistr.o lib/util_file.o lib/data_blob.o \
	  lib/util.o lib/util_sock.o lib/sock_exec.o lib/util_sec.o \
	  lib/talloc.o lib/hash.o lib/substitute.o lib/fsusage.o \
	  lib/ms_fnmatch.o lib/select.o lib/messages.o \
	  lib/tallocmsg.o lib/dmallocmsg.o libsmb/smb_signing.o \
	  lib/md5.o lib/hmacmd5.o lib/iconv.o lib/smbpasswd.o \
	  nsswitch/wb_client.o nsswitch/wb_common.o \
	  lib/pam_errors.o intl/lang_tdb.o lib/account_pol.o \
	  lib/adt_tree.o lib/gencache.o $(TDB_OBJ) \
	  lib/module.o lib/ldap_escape.o  \
	  lib/privileges.o lib/secdesc.o lib/secace.o lib/secacl.o lib/dummysmbd.o


LIB_SMBD_OBJ = lib/system_smbd.o lib/util_smbd.o 

READLINE_OBJ = lib/readline.o

# Also depends on  $(SECRETS_OBJ) $(LIBSAMBA_OBJ)
# Be sure to include them into your application
POPT_LIB_OBJ = lib/popt_common.o 

UBIQX_OBJ = ubiqx/ubi_BinTree.o ubiqx/ubi_Cache.o ubiqx/ubi_SplayTree.o \
            ubiqx/ubi_dLinkList.o ubiqx/ubi_sLinkList.o 

PARAM_OBJ = dynconfig.o param/loadparm.o param/params.o

KRBCLIENT_OBJ = libads/kerberos.o libads/ads_status.o

LIBADS_OBJ = libads/ldap.o libads/ldap_printer.o libads/sasl.o \
	     libads/krb5_setpw.o libads/ldap_user.o \
	     libads/ads_struct.o \
             libads/disp_sec.o libads/ads_utils.o libads/ldap_utils.o \
	     libads/ads_ldap.o libads/authdata.o

LIBADS_SERVER_OBJ = libads/util.o libads/kerberos_verify.o

SECRETS_OBJ = passdb/secrets.o 

LIBNMB_OBJ = libsmb/unexpected.o libsmb/namecache.o libsmb/nmblib.o \
	     libsmb/namequery.o libsmb/conncache.o 

LIBSAMBA_OBJ = libsmb/nterr.o libsmb/smbdes.o libsmb/smbencrypt.o libsmb/ntlm_check.o \
	libsmb/ntlmssp.o libsmb/ntlmssp_parse.o libsmb/ntlmssp_sign.o

LIBSMB_OBJ = libsmb/clientgen.o libsmb/cliconnect.o libsmb/clifile.o \
	     libsmb/clikrb5.o libsmb/clispnego.o libsmb/asn1.o \
	     libsmb/clirap.o libsmb/clierror.o libsmb/climessage.o \
	     libsmb/clireadwrite.o libsmb/clilist.o libsmb/cliprint.o \
	     libsmb/clitrans.o libsmb/clisecdesc.o libsmb/clidgram.o \
	     libsmb/clistr.o lib/util_seaccess.o \
	     libsmb/cliquota.o libsmb/clifsinfo.o \
             libsmb/smberr.o libsmb/credentials.o libsmb/pwd_cache.o \
	     libsmb/clioplock.o libsmb/errormap.o libsmb/clirap2.o \
	     libsmb/passchange.o libsmb/doserr.o \
	     $(RPC_PARSE_OBJ1) $(LIBSAMBA_OBJ) $(LIBNMB_OBJ) 

LIBMSRPC_OBJ = rpc_client/cli_lsarpc.o rpc_client/cli_samr.o \
	       rpc_client/cli_netlogon.o rpc_client/cli_srvsvc.o \
	       rpc_client/cli_wkssvc.o rpc_client/cli_dfs.o \
	       rpc_client/cli_reg.o rpc_client/cli_pipe.o \
	       rpc_client/cli_spoolss.o rpc_client/cli_spoolss_notify.o  \
	       rpc_client/cli_ds.o rpc_client/cli_echo.o \
	       rpc_client/cli_shutdown.o

REGOBJS_OBJ = registry/reg_objects.o
REGISTRY_OBJ = registry/reg_frontend.o registry/reg_cachehook.o registry/reg_printing.o \
               registry/reg_db.o 

RPC_LSA_OBJ = rpc_server/srv_lsa.o rpc_server/srv_lsa_nt.o

RPC_NETLOG_OBJ = rpc_server/srv_netlog.o rpc_server/srv_netlog_nt.o

RPC_SAMR_OBJ = rpc_server/srv_samr.o rpc_server/srv_samr_nt.o \
               rpc_server/srv_samr_util.o

RPC_REG_OBJ =  rpc_server/srv_reg.o rpc_server/srv_reg_nt.o

RPC_LSA_DS_OBJ =  rpc_server/srv_lsa_ds.o rpc_server/srv_lsa_ds_nt.o

RPC_SVC_OBJ = rpc_server/srv_srvsvc.o rpc_server/srv_srvsvc_nt.o

RPC_WKS_OBJ =  rpc_server/srv_wkssvc.o rpc_server/srv_wkssvc_nt.o

RPC_DFS_OBJ =  rpc_server/srv_dfs.o rpc_server/srv_dfs_nt.o
RPC_SPOOLSS_OBJ = rpc_server/srv_spoolss.o rpc_server/srv_spoolss_nt.o 

RPC_PIPE_OBJ = rpc_server/srv_pipe_hnd.o rpc_server/srv_util.o \
               rpc_server/srv_pipe.o rpc_server/srv_lsa_hnd.o

RPC_ECHO_OBJ = rpc_server/srv_echo.o rpc_server/srv_echo_nt.o

RPC_SERVER_OBJ =  $(RPC_LSA_OBJ) $(RPC_REG_OBJ) $(RPC_LSA_DS_OBJ) $(RPC_WKS_OBJ) $(RPC_NETLOG_OBJ) $(RPC_DFS_OBJ) $(RPC_SVC_OBJ) $(RPC_SPOOLSS_OBJ) $(RPC_SAMR_OBJ) $(RPC_PIPE_OBJ)

# this includes only the low level parse code, not stuff
# that requires knowledge of security contexts
RPC_PARSE_OBJ1 = rpc_parse/parse_prs.o rpc_parse/parse_sec.o \
		 rpc_parse/parse_misc.o

RPC_PARSE_OBJ = rpc_parse/parse_lsa.o rpc_parse/parse_net.o \
                rpc_parse/parse_reg.o rpc_parse/parse_rpc.o \
                rpc_parse/parse_samr.o rpc_parse/parse_srv.o \
                rpc_parse/parse_wks.o rpc_parse/parse_ds.o \
		rpc_parse/parse_spoolss.o rpc_parse/parse_dfs.o \
		rpc_parse/parse_echo.o rpc_parse/parse_shutdown.o \
		$(REGOBJS_OBJ)


RPC_CLIENT_OBJ = rpc_client/cli_pipe.o 

LOCKING_OBJ = locking/locking.o locking/brlock.o locking/posix.o

PASSDB_GET_SET_OBJ = passdb/pdb_get_set.o

PASSDB_OBJ = $(PASSDB_GET_SET_OBJ) passdb/passdb.o passdb/pdb_interface.o \
		passdb/machine_sid.o passdb/util_sam_sid.o passdb/pdb_compat.o \
		passdb/privileges.o passdb/lookup_sid.o passdb/login_cache.o passdb/pdb_smbpasswd.o passdb/pdb_tdb.o passdb/pdb_guest.o passdb/pdb_sql.o

XML_OBJ = passdb/pdb_xml.o
MYSQL_OBJ = passdb/pdb_mysql.o
PGSQL_OBJ = passdb/pdb_pgsql.o
DEVEL_HELP_WEIRD_OBJ = modules/weird.o
CP850_OBJ = modules/CP850.o
CP437_OBJ = modules/CP437.o
CHARSET_MACOSXFS_OBJ = modules/charset_macosxfs.o

GROUPDB_OBJ = groupdb/mapping.o

PROFILE_OBJ = profile/profile.o

PROFILES_OBJ = utils/profiles.o
EDITREG_OBJ = utils/editreg.o

OPLOCK_OBJ = smbd/oplock.o smbd/oplock_irix.o smbd/oplock_linux.o

NOTIFY_OBJ = smbd/notify.o smbd/notify_hash.o smbd/notify_kernel.o

VFS_AUDIT_OBJ = modules/vfs_audit.o
VFS_EXTD_AUDIT_OBJ = modules/vfs_extd_audit.o
VFS_FAKE_PERMS_OBJ = modules/vfs_fake_perms.o
VFS_RECYCLE_OBJ = modules/vfs_recycle.o
VFS_NETATALK_OBJ = modules/vfs_netatalk.o
VFS_DEFAULT_QUOTA_OBJ = modules/vfs_default_quota.o
VFS_READONLY_OBJ = modules/vfs_readonly.o modules/getdate.o
VFS_CAP_OBJ = modules/vfs_cap.o

PLAINTEXT_AUTH_OBJ = auth/pampass.o auth/pass_check.o

SLCACHE_OBJ = libsmb/samlogon_cache.o

DCUTIL_OBJ  = libsmb/namequery_dc.o libsmb/trustdom_cache.o libsmb/trusts_util.o

AUTH_BUILTIN_OBJ = auth/auth_builtin.o
AUTH_DOMAIN_OBJ = auth/auth_domain.o
AUTH_SAM_OBJ = auth/auth_sam.o
AUTH_RHOSTS_OBJ = auth/auth_rhosts.o
AUTH_SERVER_OBJ = auth/auth_server.o
AUTH_UNIX_OBJ = auth/auth_unix.o
AUTH_WINBIND_OBJ = auth/auth_winbind.o


AUTH_OBJ = auth/auth.o  $(AUTH_RHOSTS_OBJ) $(AUTH_SAM_OBJ) $(AUTH_UNIX_OBJ) $(AUTH_WINBIND_OBJ) $(AUTH_SERVER_OBJ) $(AUTH_DOMAIN_OBJ) $(AUTH_BUILTIN_OBJ) auth/auth_util.o auth/auth_compat.o \
	   auth/auth_ntlmssp.o \
	   $(PLAINTEXT_AUTH_OBJ) $(SLCACHE_OBJ) $(DCUTIL_OBJ)

MANGLE_OBJ = smbd/mangle.o smbd/mangle_hash.o smbd/mangle_map.o smbd/mangle_hash2.o

SMBD_OBJ_MAIN = smbd/server.o

BUILDOPT_OBJ = smbd/build_options.o

SMBD_OBJ_SRV = smbd/files.o smbd/chgpasswd.o smbd/tdbutil.o smbd/connection.o \
	       smbd/utmp.o smbd/session.o \
               smbd/dfree.o smbd/dir.o smbd/password.o smbd/conn.o smbd/fileio.o \
               smbd/ipc.o smbd/lanman.o smbd/negprot.o \
               smbd/message.o smbd/nttrans.o smbd/pipes.o \
               smbd/reply.o smbd/sesssetup.o smbd/trans2.o smbd/uid.o \
	       smbd/dosmode.o smbd/filename.o smbd/open.o smbd/close.o \
	       smbd/blocking.o smbd/sec_ctx.o smbd/srvstr.o \
	       smbd/vfs.o smbd/vfs-wrap.o smbd/statcache.o \
               smbd/posix_acls.o lib/sysacls.o lib/server_mutex.o \
	       smbd/process.o smbd/service.o smbd/error.o \
	       printing/printfsp.o lib/sysquotas.o lib/sysquotas_linux.o \
	       lib/sysquotas_xfs.o lib/sysquotas_4A.o \
	       smbd/change_trust_pw.o smbd/fake_file.o \
	       smbd/quotas.o smbd/ntquotas.o lib/afs.o \
	       $(MANGLE_OBJ) 

#SMBD_OBJ_BASE = $(PARAM_OBJ) $(SMBD_OBJ_SRV) $(MSDFS_OBJ) $(LIBSMB_OBJ) \
#		$(RPC_SERVER_OBJ) $(RPC_PARSE_OBJ) $(SECRETS_OBJ) \
#		$(LOCKING_OBJ) $(PASSDB_OBJ) $(PRINTING_OBJ) $(PROFILE_OBJ) \
#		$(LIB_OBJ) $(PRINTBACKEND_OBJ) $(OPLOCK_OBJ) \
#		$(NOTIFY_OBJ) $(GROUPDB_OBJ) $(AUTH_OBJ) \
#		$(LIBMSRPC_OBJ) \
#		$(LIBADS_OBJ) $(KRBCLIENT_OBJ) $(LIBADS_SERVER_OBJ) \
#		$(LIB_SMBD_OBJ) $(REGISTRY_OBJ) $(POPT_LIB_OBJ) \
#		$(UBIQX_OBJ) $(BUILDOPT_OBJ) $(SMBLDAP_OBJ)
#SMBD_OBJ_BASE = $(SMBD_OBJ_SRV) $(MSDFS_OBJ) \
#		$(RPC_SERVER_OBJ) \
#		$(LOCKING_OBJ) $(PRINTING_OBJ) $(PROFILE_OBJ) \
#		$(PRINTBACKEND_OBJ) $(OPLOCK_OBJ) \
#		$(NOTIFY_OBJ) $(AUTH_OBJ) \
#		$(LIBADS_OBJ) $(LIBADS_SERVER_OBJ) \
#		$(LIB_SMBD_OBJ) $(REGISTRY_OBJ) $(POPT_LIB_OBJ) \
#		$(BUILDOPT_OBJ)
SMBD_OBJ_BASE = $(SMBD_OBJ_SRV) $(MSDFS_OBJ) \
		$(RPC_SERVER_OBJ) \
		$(LOCKING_OBJ) $(PRINTING_OBJ) $(PROFILE_OBJ) \
		$(PRINTBACKEND_OBJ) $(OPLOCK_OBJ) \
		$(NOTIFY_OBJ) $(AUTH_OBJ) \
		$(LIB_SMBD_OBJ) $(REGISTRY_OBJ) \
		$(BUILDOPT_OBJ)

#		$(LIBADS_OBJ) $(LIBADS_SERVER_OBJ) \


PRINTING_OBJ = printing/pcap.o printing/print_svid.o \
				printing/print_cups.o printing/print_generic.o \
				printing/lpq_parse.o printing/load.o

PRINTBACKEND_OBJ = printing/printing.o printing/nt_printing.o printing/notify.o \
		printing/printing_db.o

MSDFS_OBJ = msdfs/msdfs.o 


SMBD_OBJ = $(SMBD_OBJ_BASE) $(SMBD_OBJ_MAIN)
NMBD_OBJ1 = nmbd/asyncdns.o nmbd/nmbd.o nmbd/nmbd_become_dmb.o \
            nmbd/nmbd_become_lmb.o nmbd/nmbd_browserdb.o \
            nmbd/nmbd_browsesync.o nmbd/nmbd_elections.o \
            nmbd/nmbd_incomingdgrams.o nmbd/nmbd_incomingrequests.o \
            nmbd/nmbd_lmhosts.o nmbd/nmbd_logonnames.o nmbd/nmbd_mynames.o \
            nmbd/nmbd_namelistdb.o nmbd/nmbd_namequery.o \
            nmbd/nmbd_nameregister.o nmbd/nmbd_namerelease.o \
            nmbd/nmbd_nodestatus.o nmbd/nmbd_packets.o \
            nmbd/nmbd_processlogon.o nmbd/nmbd_responserecordsdb.o \
            nmbd/nmbd_sendannounce.o nmbd/nmbd_serverlistdb.o \
            nmbd/nmbd_subnetdb.o nmbd/nmbd_winsproxy.o nmbd/nmbd_winsserver.o \
            nmbd/nmbd_workgroupdb.o nmbd/nmbd_synclists.o

#NMBD_OBJ = $(NMBD_OBJ1) $(PARAM_OBJ) $(LIBSMB_OBJ) $(KRBCLIENT_OBJ) $(UBIQX_OBJ) \
#           $(PROFILE_OBJ) $(LIB_OBJ) $(SECRETS_OBJ) $(POPT_LIB_OBJ)
NMBD_OBJ = $(NMBD_OBJ1) \
           $(PROFILE_OBJ)

WREPL_OBJ1 = wrepld/server.o wrepld/process.o wrepld/parser.o wrepld/socket.o \
             wrepld/partners.o

WREPL_OBJ = $(WREPL_OBJ1)  $(PARAM_OBJ) $(UBIQX_OBJ) \
	    $(PROFILE_OBJ) $(LIB_OBJ) $(POPT_LIB_OBJ) $(SECRETS_OBJ) \
	    $(LIBSAMBA_OBJ)

SWAT_OBJ1 = web/cgi.o web/diagnose.o web/startstop.o web/statuspage.o \
           web/swat.o web/neg_lang.o 

SWAT_OBJ = $(SWAT_OBJ1) $(PARAM_OBJ) $(PRINTING_OBJ) $(LIBSMB_OBJ) \
	   $(LOCKING_OBJ) $(PASSDB_OBJ) $(SECRETS_OBJ) $(KRBCLIENT_OBJ) \
	   $(UBIQX_OBJ) $(LIB_OBJ) $(GROUPDB_OBJ) $(PLAINTEXT_AUTH_OBJ) \
	   $(POPT_LIB_OBJ) $(SMBLDAP_OBJ) lib/dummyroot.o

SMBSH_OBJ = smbwrapper/smbsh.o smbwrapper/shared.o \
            $(PARAM_OBJ) $(UBIQX_OBJ) $(LIB_OBJ)

#STATUS_OBJ = utils/status.o $(LOCKING_OBJ) $(PARAM_OBJ) \
#             $(UBIQX_OBJ) $(PROFILE_OBJ) $(LIB_OBJ) $(POPT_LIB_OBJ) \
#	     $(SECRETS_OBJ) $(LIBSAMBA_OBJ) lib/dummyroot.o libsmb/errormap.o 
STATUS_OBJ = utils/status.o $(LOCKING_OBJ) \
             $(PROFILE_OBJ) \
	     $(LIBSAMBA_OBJ) lib/dummyroot.o libsmb/errormap.o 

#SMBCONTROL_OBJ = utils/smbcontrol.o $(LOCKING_OBJ) $(PARAM_OBJ) \
#	$(UBIQX_OBJ) $(PROFILE_OBJ) $(LIB_OBJ) $(POPT_LIB_OBJ) \
#	$(SECRETS_OBJ) $(LIBSAMBA_OBJ) \
#	printing/notify.o printing/printing_db.o lib/dummyroot.o libsmb/errormap.o
SMBCONTROL_OBJ = utils/smbcontrol.o $(LOCKING_OBJ) \
	$(PROFILE_OBJ) \
	$(LIBSAMBA_OBJ) \
	printing/notify.o printing/printing_db.o lib/dummyroot.o libsmb/errormap.o

SMBTREE_OBJ = utils/smbtree.o $(LOCKING_OBJ) $(PARAM_OBJ) \
             $(UBIQX_OBJ) $(PROFILE_OBJ) $(LIB_OBJ) $(LIBSMB_OBJ) \
	     $(KRBCLIENT_OBJ) $(POPT_LIB_OBJ) $(SECRETS_OBJ) 

TESTPARM_OBJ = utils/testparm.o \
               $(PARAM_OBJ) $(UBIQX_OBJ) $(LIB_OBJ) $(POPT_LIB_OBJ) \
	       $(SECRETS_OBJ) $(LIBSAMBA_OBJ)

TESTPRNS_OBJ = utils/testprns.o $(PARAM_OBJ) $(PRINTING_OBJ) $(UBIQX_OBJ) \
               $(LIB_OBJ)

#SMBPASSWD_OBJ = utils/smbpasswd.o $(PARAM_OBJ) $(SECRETS_OBJ) \
#		$(LIBSMB_OBJ) $(PASSDB_OBJ) $(GROUPDB_OBJ)\
#                $(UBIQX_OBJ) $(LIB_OBJ) $(KRBCLIENT_OBJ) \
#		$(SMBLDAP_OBJ) lib/dummyroot.o
SMBPASSWD_OBJ = utils/smbpasswd.o \
		lib/dummyroot.o

PDBEDIT_OBJ = utils/pdbedit.o $(PARAM_OBJ) $(PASSDB_OBJ) $(LIBSAMBA_OBJ) \
		$(UBIQX_OBJ) $(LIB_OBJ) $(GROUPDB_OBJ) $(SECRETS_OBJ) \
		$(POPT_LIB_OBJ) $(SMBLDAP_OBJ) lib/dummyroot.o

SMBGET_OBJ = utils/smbget.o $(POPT_LIB_OBJ) $(LIBSMBCLIENT_OBJ) $(SECRETS_OBJ)

RPCCLIENT_OBJ1 = rpcclient/rpcclient.o rpcclient/cmd_lsarpc.o \
	         rpcclient/cmd_samr.o rpcclient/cmd_spoolss.o \
		 rpcclient/cmd_netlogon.o rpcclient/cmd_srvsvc.o \
		 rpcclient/cmd_dfs.o rpcclient/cmd_reg.o \
		 rpcclient/display_sec.o rpcclient/cmd_ds.o \
		 rpcclient/cmd_echo.o rpcclient/cmd_shutdown.o

RPCCLIENT_OBJ = $(RPCCLIENT_OBJ1) \
             $(PARAM_OBJ) $(LIBSMB_OBJ) $(UBIQX_OBJ) $(LIB_OBJ) \
             $(RPC_PARSE_OBJ) $(PASSDB_OBJ) $(LIBMSRPC_OBJ) \
             $(READLINE_OBJ) $(GROUPDB_OBJ) $(KRBCLIENT_OBJ) \
	     $(LIBADS_OBJ) $(SECRETS_OBJ) $(POPT_LIB_OBJ) \
	     $(SMBLDAP_OBJ) $(DCUTIL_OBJ) lib/dummyroot.o

PAM_WINBIND_PICOBJ = nsswitch/pam_winbind.po \
	nsswitch/wb_common.po lib/replace1.po \
	lib/snprintf.po

SMBW_OBJ1 = smbwrapper/smbw.o \
		smbwrapper/smbw_dir.o smbwrapper/smbw_stat.o \
		smbwrapper/realcalls.o smbwrapper/shared.o \
		smbwrapper/smbw_cache.o

SMBW_OBJ = $(SMBW_OBJ1) $(LIBSMB_OBJ) $(KRBCLIENT_OBJ) $(PARAM_OBJ) \
                $(UBIQX_OBJ) $(LIB_OBJ)

SMBWRAPPER_OBJ1 = smbwrapper/wrapped.o

SMBWRAPPER_OBJ = $(SMBW_OBJ) $(SMBWRAPPER_OBJ1)

LIBSMBCLIENT_OBJ = libsmb/libsmbclient.o libsmb/libsmb_compat.o \
		   libsmb/libsmb_cache.o \
		   $(PARAM_OBJ) $(LIB_OBJ) \
	  	   $(LIBSMB_OBJ) $(KRBCLIENT_OBJ) $(UBIQX_OBJ) \
		   $(LIBMSRPC_OBJ) $(RPC_PARSE_OBJ) $(PASSDB_GET_SET_OBJ)

# This shared library is intended for linking with unit test programs
# to test Samba internals.  It's called libbigballofmud.so to
# discourage casual usage.

LIBBIGBALLOFMUD_MAJOR = 0

LIBBIGBALLOFMUD_OBJ = $(PARAM_OBJ) $(LIB_OBJ) $(UBIQX_OBJ) $(SECRETS_OBJ) \
	$(LIBSMB_OBJ) $(LIBMSRPC_OBJ) $(RPC_PARSE_OBJ) $(PASSDB_OBJ) \
	$(GROUPDB_OBJ) $(KRBCLIENT_OBJ) $(SMBLDAP_OBJ) \
	$(LIBADS_OBJ) $(LIBADS_SERVER_OBJ) $(POPT_LIB_OBJ) lib/dummyroot.o


LIBBIGBALLOFMUD_PICOBJS = $(LIBBIGBALLOFMUD_OBJ:.o=.po)

CLIENT_OBJ1 = client/client.o client/clitar.o 

#CLIENT_OBJ = $(CLIENT_OBJ1) $(PARAM_OBJ) $(LIBSMB_OBJ) $(UBIQX_OBJ) \
#	     $(LIB_OBJ) $(KRBCLIENT_OBJ) \
#             $(READLINE_OBJ) $(POPT_LIB_OBJ) $(SECRETS_OBJ)
CLIENT_OBJ = $(CLIENT_OBJ1) \
             $(READLINE_OBJ) 

MESSAGE_OBJ1 = client/message.o

#MESSAGE_OBJ = $(MESSAGE_OBJ1) $(PARAM_OBJ) $(LIBSMB_OBJ) $(UBIQX_OBJ) \
#	     $(LIB_OBJ) $(KRBCLIENT_OBJ) \
#             $(READLINE_OBJ) $(POPT_LIB_OBJ) $(SECRETS_OBJ)
MESSAGE_OBJ = $(MESSAGE_OBJ1) \
             $(READLINE_OBJ) 

NET_OBJ1 = utils/net.o utils/net_ads.o utils/net_ads_cldap.o utils/net_help.o \
	   utils/net_rap.o utils/net_rpc.o utils/net_rpc_samsync.o \
	   utils/net_rpc_join.o utils/net_time.o utils/net_lookup.o \
	   utils/net_cache.o utils/net_groupmap.o utils/net_idmap.o \
	   utils/net_status.o

#NET_OBJ = $(NET_OBJ1) $(PARAM_OBJ) $(SECRETS_OBJ) $(LIBSMB_OBJ) \
#	  $(RPC_PARSE_OBJ) $(PASSDB_OBJ) $(GROUPDB_OBJ) \
#	  $(KRBCLIENT_OBJ) $(UBIQX_OBJ) $(LIB_OBJ) \
#	  $(LIBMSRPC_OBJ) $(IDMAP_OBJ) \
#	  $(LIBADS_OBJ) $(LIBADS_SERVER_OBJ) $(POPT_LIB_OBJ) \
#	  $(SMBLDAP_OBJ) $(DCUTIL_OBJ) lib/dummyroot.o lib/server_mutex.o lib/afs.o
NET_OBJ = $(NET_OBJ1) \
	  $(IDMAP_OBJ) \
	  $(DCUTIL_OBJ) lib/dummyroot.o lib/server_mutex.o lib/afs.o

#	  $(POPT_LIB_OBJ) \
#$(LIBADS_OBJ) $(LIBADS_SERVER_OBJ) 

CUPS_OBJ = client/smbspool.o $(PARAM_OBJ) $(LIBSMB_OBJ) $(UBIQX_OBJ) \
	  $(LIB_OBJ) $(KRBCLIENT_OBJ) $(SECRETS_OBJ)

#MOUNT_OBJ = client/smbmount.o \
             $(PARAM_OBJ) $(LIBSMB_OBJ) $(KRBCLIENT_OBJ) $(UBIQX_OBJ) $(LIB_OBJ) 
MOUNT_OBJ = client/smbmount.o

MNT_OBJ = client/smbmnt.o lib/version.o lib/snprintf.o

UMOUNT_OBJ = client/smbumount.o

NMBLOOKUP_OBJ = utils/nmblookup.o $(PARAM_OBJ) $(UBIQX_OBJ) $(LIBNMB_OBJ) \
               $(LIB_OBJ) $(POPT_LIB_OBJ) $(SECRETS_OBJ) $(LIBSAMBA_OBJ)

SMBTORTURE_OBJ1 = torture/torture.o torture/nbio.o torture/scanner.o torture/utable.o \
		torture/denytest.o torture/mangle_test.o 

SMBTORTURE_OBJ = $(SMBTORTURE_OBJ1) $(PARAM_OBJ) \
	$(LIBSMB_OBJ) $(KRBCLIENT_OBJ) $(UBIQX_OBJ) $(LIB_OBJ)

MASKTEST_OBJ = torture/masktest.o $(PARAM_OBJ) $(LIBSMB_OBJ) $(KRBCLIENT_OBJ) \
                 $(UBIQX_OBJ) $(LIB_OBJ)

MSGTEST_OBJ = torture/msgtest.o $(PARAM_OBJ) $(LIBSMB_OBJ) $(KRBCLIENT_OBJ) \
                 $(UBIQX_OBJ) $(LIB_OBJ)

LOCKTEST_OBJ = torture/locktest.o $(PARAM_OBJ) $(LOCKING_OBJ) $(KRBCLIENT_OBJ) \
			   $(LIBSMB_OBJ) $(UBIQX_OBJ) $(LIB_OBJ)

NSSTEST_OBJ = torture/nsstest.o $(PARAM_OBJ) $(LIBSMB_OBJ) $(KRBCLIENT_OBJ) \
                 $(UBIQX_OBJ) $(LIB_OBJ)

VFSTEST_OBJ = torture/cmd_vfs.o torture/vfstest.o $(SMBD_OBJ_BASE) $(READLINE_OBJ)

SMBICONV_OBJ = $(PARAM_OBJ) torture/smbiconv.o $(LIB_OBJ) $(UBIQX_OBJ) $(POPT_LIB_OBJ) $(SECRETS_OBJ) $(LIBSAMBA_OBJ)

LOG2PCAP_OBJ = utils/log2pcaphex.o

LOCKTEST2_OBJ = torture/locktest2.o $(PARAM_OBJ) $(LOCKING_OBJ) $(LIBSMB_OBJ) \
		$(KRBCLIENT_OBJ) $(UBIQX_OBJ) $(LIB_OBJ)

SMBCACLS_OBJ = utils/smbcacls.o $(PARAM_OBJ) $(LOCKING_OBJ) $(LIBSMB_OBJ) \
			   $(KRBCLIENT_OBJ) $(UBIQX_OBJ) $(LIB_OBJ) $(RPC_PARSE_OBJ) \
			   $(PASSDB_GET_SET_OBJ) $(LIBMSRPC_OBJ) $(SECRETS_OBJ) \
			   $(POPT_LIB_OBJ) $(DCUTIL_OBJ) $(LIBADS_OBJ)

SMBCQUOTAS_OBJ = utils/smbcquotas.o $(LOCKING_OBJ) $(LIBSMB_OBJ) $(KRBCLIENT_OBJ) \
		$(PARAM_OBJ) \
		$(UBIQX_OBJ) $(LIB_OBJ) $(RPC_PARSE_OBJ) $(PASSDB_GET_SET_OBJ) \
		$(LIBMSRPC_OBJ) $(SECRETS_OBJ) $(POPT_LIB_OBJ)

TALLOCTORT_OBJ = lib/talloctort.o $(PARAM_OBJ) $(LIB_OBJ) $(UBIQX_OBJ)

RPCTORTURE_OBJ = torture/rpctorture.o \
             rpcclient/display.o \
             rpcclient/cmd_lsarpc.o \
             rpcclient/cmd_wkssvc.o \
             rpcclient/cmd_samr.o \
             rpcclient/cmd_srvsvc.o \
             rpcclient/cmd_netlogon.o \
             $(PARAM_OBJ) $(LIBSMB_OBJ) $(UBIQX_OBJ) $(LIB_OBJ) $(KRBCLIENT_OBJ) \
             $(RPC_CLIENT_OBJ) $(RPC_PARSE_OBJ) $(PASSDB_GET_SET_OBJ)

DEBUG2HTML_OBJ = utils/debug2html.o ubiqx/debugparse.o

SMBFILTER_OBJ = utils/smbfilter.o $(PARAM_OBJ) $(LIBSMB_OBJ) \
                 $(UBIQX_OBJ) $(LIB_OBJ) $(KRBCLIENT_OBJ) 

PROTO_OBJ = $(SMBD_OBJ_MAIN) \
	    $(SMBD_OBJ_SRV) $(NMBD_OBJ1) $(SWAT_OBJ1) $(LIB_OBJ) $(LIBSMB_OBJ) \
	    $(SMBW_OBJ1) $(SMBWRAPPER_OBJ1) $(SMBTORTURE_OBJ1) $(RPCCLIENT_OBJ1) \
	    $(LIBMSRPC_OBJ) $(RPC_CLIENT_OBJ) \
	    $(RPC_PIPE_OBJ) $(RPC_PARSE_OBJ) $(KRBCLIENT_OBJ) \
	    $(AUTH_OBJ) $(PARAM_OBJ) $(LOCKING_OBJ) $(SECRETS_OBJ) \
	    $(PRINTING_OBJ) $(PRINTBACKEND_OBJ) $(OPLOCK_OBJ) $(NOTIFY_OBJ) \
	    $(PASSDB_OBJ) $(GROUPDB_OBJ) $(MSDFS_OBJ) \
	    $(READLINE_OBJ) $(PROFILE_OBJ) $(LIBADS_OBJ) $(LIBADS_SERVER_OBJ) \
	    $(LIB_SMBD_OBJ) $(SAM_OBJ) $(REGISTRY_OBJ) $(POPT_LIB_OBJ) \
	    $(RPC_LSA_OBJ) $(RPC_NETLOG_OBJ) $(RPC_SAMR_OBJ) $(RPC_REG_OBJ) $(RPC_LSA_DS_OBJ) \
	    $(RPC_SVC_OBJ) $(RPC_WKS_OBJ) $(RPC_DFS_OBJ) $(RPC_SPOOLSS_OBJ) \
	    $(RPC_ECHO_OBJ) $(SMBLDAP_OBJ) $(IDMAP_OBJ) libsmb/spnego.o

WINBIND_WINS_NSS_OBJ = nsswitch/wins.o $(PARAM_OBJ) $(UBIQX_OBJ) \
	$(LIBSMB_OBJ) $(LIB_OBJ) $(NSSWINS_OBJ) $(KRBCLIENT_OBJ)

WINBIND_WINS_NSS_PICOBJS = $(WINBIND_WINS_NSS_OBJ:.o=.po)

PICOBJS = $(SMBWRAPPER_OBJ:.o=.po)
LIBSMBCLIENT_PICOBJS = $(LIBSMBCLIENT_OBJ:.o=.po)

PAM_SMBPASS_OBJ_0 = pam_smbpass/pam_smb_auth.o pam_smbpass/pam_smb_passwd.o \
               pam_smbpass/pam_smb_acct.o pam_smbpass/support.o \
               lib/dummyroot.o $(PARAM_OBJ) $(LIB_OBJ) $(PASSDB_OBJ) $(GROUPDB_OBJ) \
               $(SECRETS_OBJ) $(UBIQX_OBJ) $(SMBLDAP_OBJ) $(LIBSAMBA_OBJ)

PAM_SMBPASS_PICOOBJ = $(PAM_SMBPASS_OBJ_0:.o=.po)

IDMAP_OBJ     = sam/idmap.o sam/idmap_util.o  sam/idmap_tdb.o

WINBINDD_OBJ1 = \
		nsswitch/winbindd.o       \
		nsswitch/winbindd_user.o  \
		nsswitch/winbindd_group.o \
		nsswitch/winbindd_util.o  \
		nsswitch/winbindd_cache.o \
		nsswitch/winbindd_pam.o   \
		nsswitch/winbindd_sid.o   \
		nsswitch/winbindd_misc.o  \
		nsswitch/winbindd_cm.o \
		nsswitch/winbindd_wins.o \
		nsswitch/winbindd_rpc.o \
		nsswitch/winbindd_ads.o \
		nsswitch/winbindd_dual.o \
		nsswitch/winbindd_acct.o

WINBINDD_OBJ = \
		$(WINBINDD_OBJ1) $(PASSDB_OBJ) $(GROUPDB_OBJ) \
		$(PARAM_OBJ) $(UBIQX_OBJ) $(LIB_OBJ) \
		$(LIBSMB_OBJ) $(LIBMSRPC_OBJ) $(RPC_PARSE_OBJ) \
		$(PROFILE_OBJ) $(SLCACHE_OBJ) $(SMBLDAP_OBJ) \
		$(SECRETS_OBJ) $(LIBADS_OBJ) $(KRBCLIENT_OBJ) $(POPT_LIB_OBJ) \
		$(DCUTIL_OBJ) $(IDMAP_OBJ) lib/dummyroot.o

WBINFO_OBJ = nsswitch/wbinfo.o $(LIBSAMBA_OBJ) $(PARAM_OBJ) $(LIB_OBJ) \
		$(UBIQX_OBJ) $(SECRETS_OBJ) $(POPT_LIB_OBJ)

WINBIND_NSS_OBJ = nsswitch/wb_common.o lib/replace1.o nsswitch/winbind_nss_linux.o

WINBIND_NSS_PICOBJS = $(WINBIND_NSS_OBJ:.o=.po) lib/snprintf.po

POPT_OBJS=popt/findme.o popt/popt.o popt/poptconfig.o \
          popt/popthelp.o popt/poptparse.o

TDBBACKUP_OBJ = tdb/tdbbackup.o tdb/tdbback.o lib/snprintf.o $(TDBBASE_OBJ)

TDBDUMP_OBJ = tdb/tdbdump.o $(TDBBASE_OBJ)

TDB_RESTORE_SECRETS_OBJ = tdb/tdb_restore_secrets.o $(TDBBASE_OBJ)

NTLM_AUTH_OBJ = utils/ntlm_auth.o $(LIBSAMBA_OBJ) $(POPT_LIB_OBJ) \
		libsmb/asn1.o libsmb/spnego.o libsmb/clikrb5.o libads/kerberos.o \
		libads/kerberos_verify.o $(SECRETS_OBJ) lib/server_mutex.o \
		libads/authdata.o rpc_parse/parse_prs.o rpc_parse/parse_misc.o \
		libsmb/doserr.o

######################################################################
# now the rules...
######################################################################
all : SHOWFLAGS
	$(MAKE) smbd/build_options.c; \
	$(MAKE) include/proto.h; \
	$(MAKE) include/build_env.h; \
	$(MAKE) include/wrepld_proto.h; \
	$(MAKE) nsswitch/winbindd_proto.h; \
	$(MAKE) web/swat_proto.h; \
	$(MAKE) client/client_proto.h; \
	$(MAKE) utils/net_proto.h
	$(MAKE) $(SBIN_PROGS) $(BIN_PROGS) $(SHLIBS) $(MODULES)

pam_smbpass : SHOWFLAGS bin/pam_smbpass.so

smbwrapper : SHOWFLAGS 

torture : SHOWFLAGS $(TORTURE_PROGS)

smbtorture : SHOWFLAGS bin/smbtorture

masktest : SHOWFLAGS bin/masktest

msgtest : SHOWFLAGS bin/msgtest

locktest : SHOWFLAGS bin/locktest

smbcacls : SHOWFLAGS bin/smbcacls

smbcquotas : SHOWFLAGS bin/smbcquotas

locktest2 : SHOWFLAGS bin/locktest2

rpctorture : SHOWFLAGS bin/rpctorture

debug2html : SHOWFLAGS bin/debug2html

smbfilter : SHOWFLAGS bin/smbfilter

talloctort : SHOWFLAGS bin/talloctort

nsswitch : SHOWFLAGS bin/winbindd bin/wbinfo nsswitch/libnss_winbind.so \
	nsswitch/libnss_wins.so nsswitch/pam_winbind.so

wins : SHOWFLAGS nsswitch/libnss_wins.so

modules: SHOWFLAGS proto_exists $(MODULES)

everything: all libsmbclient debug2html smbfilter talloctort modules torture \
	$(EVERYTHING_PROGS)

.SUFFIXES:
.SUFFIXES: .c .o .po .lo

SHOWFLAGS:
	@echo "Using FLAGS = $(FLAGS)"
	@echo "      LIBS = $(LIBS)"
	@echo "      LDSHFLAGS = $(LDSHFLAGS)"
	@echo "      LDFLAGS = $(LDFLAGS)"

MAKEDIR = || exec false; \
	  if test -d "$$dir"; then :; else \
	  echo mkdir "$$dir"; \
	  mkdir -p "$$dir" >/dev/null 2>&1 || \
	  test -d "$$dir" || \
	  mkdir "$$dir" || \
	  exec false; fi || exec false

.c.o:
	@if (: >> $@ || : > $@) >/dev/null 2>&1; then rm -f $@; else \
	 dir=`echo $@ | sed 's,/[^/]*$$,,;s,^$$,.,'` $(MAKEDIR); fi
	@echo Compiling $*.c
	@$(CC) -I. -I$(srcdir) $(FLAGS) -c $< \
	  -o $@ 
#	-mv `echo $@ | sed 's%^.*/%%g'` $@

# this adds support for precompiled headers. To use it, install a snapshot
# of gcc-3.4 and run 'make pch' before you do the main build. 
pch:
	rm -f $(srcdir)/include/includes.h.gch
	$(CC) -I. -I$(srcdir) $(FLAGS) -c $(srcdir)/include/includes.h -o $(srcdir)/include/includes.h.gch

# These dependencies are only approximately correct: we want to make
# sure Samba's paths are updated if ./configure is re-run.  Really it
# would be nice if "make prefix=/opt/samba all" also rebuilt things,
# but since we also require "make install prefix=/opt/samba" *not* to
# rebuild it's a bit hard.

dynconfig.o: dynconfig.c Makefile
	@echo Compiling $*.c
	@$(CC) $(FLAGS) $(PATH_FLAGS) -c $< -o $@ 

dynconfig.po: dynconfig.c Makefile
	@if (: >> $@ || : > $@) >/dev/null 2>&1; then rm -f $@; else \
	  dir=`echo $@ | sed 's,/[^/]*$$,,;s,^$$,.,'` $(MAKEDIR); fi
	@echo Compiling $*.c with -fPIC
	@$(CC) -I. -I$(srcdir) $(FLAGS) $(PATH_FLAGS) -fPIC -c $< -o $*.po
#	-mv `echo $@ | sed -e 's%^.*/%%g' -e 's%\.po$$%.o%'` $@

lib/version.o: lib/version.c include/version.h
	@echo Compiling $*.c
	@$(CC) -I. -I$(srcdir) $(FLAGS) $(PATH_FLAGS) -c $< -o $@ 

lib/version.po: lib/version.c include/version.h
	@if (: >> $@ || : > $@) >/dev/null 2>&1; then rm -f $@; else \
	  dir=`echo $@ | sed 's,/[^/]*$$,,;s,^$$,.,'` $(MAKEDIR); fi
	@echo Compiling $*.c with -fPIC
	@$(CC) -I. -I$(srcdir) $(FLAGS) $(PATH_FLAGS) -fPIC -c $< -o $*.po
#	-mv `echo $@ | sed -e 's%^.*/%%g' -e 's%\.po$$%.o%'` $@

smbd/build_options.o: smbd/build_options.c Makefile include/config.h include/build_env.h include/proto.h
	@echo Compiling $*.c
	@$(CC) $(FLAGS) $(PATH_FLAGS) -c $< -o $@

smbd/build_options.c: include/config.h.in script/mkbuildoptions.awk
	@echo Generating $@
	@dir=smbd $(MAKEDIR) && $(AWK) -f $(srcdir)/script/mkbuildoptions.awk > $(builddir)/smbd/build_options.c < $(srcdir)/include/config.h.in

.c.po: 
	@if (: >> $@ || : > $@) >/dev/null 2>&1; then rm -f $@; else \
	  dir=`echo $@ | sed 's,/[^/]*$$,,;s,^$$,.,'` $(MAKEDIR); fi
	@echo Compiling $*.c with -fPIC
	@$(CC) -I. -I$(srcdir) $(FLAGS) -fPIC -c $< -o $*.po
#	-mv `echo $@ | sed -e 's%^.*/%%g' -e 's%\.po$$%.o%'` $@

bin/.dummy:
	@if (: >> $@ || : > $@) >/dev/null 2>&1; then :; else \
	  dir=bin $(MAKEDIR); fi
	@: >> $@ || : > $@ # what a fancy emoticon!

bin/smbd: $(SMBD_OBJ) $(POPT_OBJS) bin/libbigballofmud.so bin/.dummy
	@echo Linking $@
	$(CC) $(FLAGS) -o $@ $(SMBD_OBJ) $(LDFLAGS) $(LDAP_LIBS) \
		$(KRB5LIBS) $(DYNEXP) $(PRINT_LIBS) $(AUTH_LIBS) \
		$(PASSDB_LIBS) $(LIBS) $(POPT_OBJS) -Lbin -lbigballofmud

bin/nmbd: $(NMBD_OBJ) $(POPT_OBJS) bin/libbigballofmud.so bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(NMBD_OBJ) $(LDFLAGS) $(DYNEXP) $(LIBS) $(POPT_OBJS) $(KRB5LIBS) $(LDAP_LIBS) -Lbin -lbigballofmud

bin/wrepld: $(WREPL_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(WREPL_OBJ) $(LDFLAGS) $(DYNEXP) $(LIBS) $(POPT_OBJS)

bin/swat: $(SWAT_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(SWAT_OBJ) $(LDFLAGS) $(DYNEXP) $(PRINT_LIBS) \
	  $(AUTH_LIBS) $(LIBS) $(PASSDB_LIBS) $(POPT_OBJS) $(KRB5LIBS) $(LDAP_LIBS)

bin/rpcclient: $(RPCCLIENT_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(LDFLAGS) $(PASSDB_LIBS) $(RPCCLIENT_OBJ) \
		$(DYNEXP) $(TERMLDFLAGS) $(TERMLIBS) $(LIBS) $(POPT_OBJS) \
		$(KRB5LIBS) $(LDAP_LIBS)

bin/smbclient: $(CLIENT_OBJ) $(POPT_OBJS) bin/libbigballofmud.so bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(CLIENT_OBJ) $(LDFLAGS) $(DYNEXP) $(TERMLDFLAGS) $(TERMLIBS) $(LIBS) $(POPT_OBJS) $(KRB5LIBS) $(LDAP_LIBS) -Lbin -lbigballofmud

bin/smbmessage: $(MESSAGE_OBJ) $(POPT_OBJS) bin/libbigballofmud.so bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(MESSAGE_OBJ) $(LDFLAGS) $(DYNEXP) $(TERMLDFLAGS) $(TERMLIBS) $(LIBS) $(POPT_OBJS) $(KRB5LIBS) $(LDAP_LIBS) -Lbin -lbigballofmud

#bin/net: $(NET_OBJ) $(POPT_OBJS) bin/.dummy
#	@echo Linking $@
#	@$(CC) $(FLAGS) -o $@ $(NET_OBJ) $(DYNEXP) $(LDFLAGS) $(LIBS) $(POPT_OBJS) $(KRB5LIBS) $(LDAP_LIBS) $(PASSDB_LIBS)
bin/net: $(NET_OBJ) $(POPT_OBJS) bin/libbigballofmud.so bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(NET_OBJ) $(DYNEXP) $(LDFLAGS) $(LIBS) $(POPT_OBJS) $(KRB5LIBS) $(LDAP_LIBS) $(PASSDB_LIBS) -Lbin -lbigballofmud

bin/profiles: $(PROFILES_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(PROFILES_OBJ) $(LDFLAGS) $(LIBS) $(POPT_OBJS)

bin/editreg: $(EDITREG_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(EDITREG_OBJ) $(LDFLAGS) $(LIBS) $(POPT_OBJS)

bin/smbspool: $(CUPS_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(CUPS_OBJ) $(LDFLAGS) $(LIBS) $(KRB5LIBS) $(LDAP_LIBS)

#bin/smbmount: $(MOUNT_OBJ) bin/.dummy
#	@echo Linking $@
#	@$(CC) $(FLAGS) -o $@ $(MOUNT_OBJ) $(LDFLAGS) $(LIBS) $(KRB5LIBS) $(LDAP_LIBS)
bin/smbmount: $(MOUNT_OBJ) $(POPT_OBJS) bin/libbigballofmud.so bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(MOUNT_OBJ) $(LDFLAGS) $(LIBS) $(KRB5LIBS) $(POPT_OBJS) \
	$(LDAP_LIBS) -Lbin -lbigballofmud


bin/smbmnt: $(MNT_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(MNT_OBJ) $(LDFLAGS) 

bin/smbumount: $(UMOUNT_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(UMOUNT_OBJ) $(LDFLAGS)

bin/testparm: $(TESTPARM_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(TESTPARM_OBJ) $(LDFLAGS) $(DYNEXP) $(LIBS) $(POPT_OBJS)

bin/testprns: $(TESTPRNS_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(TESTPRNS_OBJ) $(LDFLAGS) $(DYNEXP) $(PRINT_LIBS) $(LIBS)

bin/smbstatus: $(STATUS_OBJ) $(POPT_OBJS) bin/libbigballofmud.so bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(STATUS_OBJ) $(LDFLAGS) $(DYNEXP) $(LIBS) \
	$(POPT_OBJS) -Lbin -lbigballofmud

bin/smbcontrol: $(SMBCONTROL_OBJ) $(POPT_OBJS) bin/libbigballofmud.so bin/.dummy
	@echo Linking $@
	@$(CC) -DUSING_SMBCONTROL $(FLAGS) -o $@ $(SMBCONTROL_OBJ) $(DYNEXP) \
	$(LDFLAGS) $(LIBS) \
	$(POPT_OBJS) -Lbin -lbigballofmud

bin/smbtree: $(SMBTREE_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(SMBTREE_OBJ) $(LDFLAGS) $(DYNEXP) $(LIBS) $(POPT_OBJS) $(KRB5LIBS) $(LDAP_LIBS) 

bin/smbpasswd: $(SMBPASSWD_OBJ) $(POPT_OBJS) bin/libbigballofmud.so bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(SMBPASSWD_OBJ) $(LDFLAGS) $(PASSDB_LIBS) \
		$(DYNEXP) $(LIBS) $(POPT_OBJS) $(KRB5LIBS) $(LDAP_LIBS) -Lbin -lbigballofmud

bin/pdbedit: $(PDBEDIT_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(PDBEDIT_OBJ) $(LDFLAGS) $(DYNEXP) $(LIBS) $(POPT_OBJS) $(PASSDB_LIBS) $(LDAP_LIBS)

bin/smbget: $(SMBGET_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(SMBGET_OBJ) $(LDFLAGS) $(DYNEXP) $(LIBS) $(POPT_OBJS)  $(KRB5LIBS) $(LDAP_LIBS)

bin/samtest: $(SAMTEST_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(SAMTEST_OBJ) $(LDFLAGS) $(TERMLDFLAGS) $(TERMLIBS) $(DYNEXP) $(LIBS) $(POPT_OBJS) $(PASSDB_LIBS) $(KRB5LIBS) $(LDAP_LIBS)

bin/nmblookup: $(NMBLOOKUP_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(NMBLOOKUP_OBJ) $(LDFLAGS) $(DYNEXP) $(LIBS) $(POPT_OBJS) $(LDAP_LIBS)

bin/smbtorture: $(SMBTORTURE_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(SMBTORTURE_OBJ) $(LDFLAGS) $(DYNEXP) $(LIBS) $(KRB5LIBS) $(LDAP_LIBS) $(SECRETS_OBJ)

bin/talloctort: $(TALLOCTORT_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(TALLOCTORT_OBJ) $(LDFLAGS) $(LIBS)

bin/masktest: $(MASKTEST_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(MASKTEST_OBJ) $(LDFLAGS) $(LIBS) $(KRB5LIBS) $(LDAP_LIBS) $(SECRETS_OBJ)

bin/msgtest: $(MSGTEST_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(MSGTEST_OBJ) $(LDFLAGS) $(LIBS) $(KRB5LIBS) $(LDAP_LIBS) 

bin/smbcacls: $(SMBCACLS_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(SMBCACLS_OBJ) $(DYNEXP) $(LDFLAGS) $(LIBS) $(POPT_OBJS) $(KRB5LIBS) $(LDAP_LIBS)

bin/smbcquotas: $(SMBCQUOTAS_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(SMBCQUOTAS_OBJ) $(DYNEXP) $(LDFLAGS) $(LIBS) $(POPT_OBJS) $(KRB5LIBS) $(LDAP_LIBS)

bin/locktest: $(LOCKTEST_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(LOCKTEST_OBJ) $(LDFLAGS) $(LIBS) $(KRB5LIBS) $(LDAP_LIBS)

bin/nsstest: $(NSSTEST_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(NSSTEST_OBJ) $(LDFLAGS) $(LIBS) $(KRB5LIBS)  $(LDAP_LIBS)

bin/vfstest: $(VFSTEST_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(VFSTEST_OBJ) $(LDFLAGS) $(TERMLDFLAGS) $(TERMLIBS) $(DYNEXP) $(PRINT_LIBS) $(AUTH_LIBS) $(ACL_LIBS) $(LIBS) $(POPT_OBJS) $(KRB5LIBS) $(LDAP_LIBS)

bin/smbiconv: $(SMBICONV_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(SMBICONV_OBJ) $(LDFLAGS) $(TERMLDFLAGS) $(TERMLIBS) $(DYNEXP) $(LIBS) $(POPT_OBJS)

bin/log2pcap: $(LOG2PCAP_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(LOG2PCAP_OBJ) $(LDFLAGS) $(POPT_OBJS) $(LIBS)

bin/locktest2: $(LOCKTEST2_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(LOCKTEST2_OBJ) $(LDFLAGS) $(LIBS) $(KRB5LIBS) $(LDAP_LIBS)

bin/rpctorture: $(RPCTORTURE_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(RPCTORTURE_OBJ) $(DYNEXP) $(LDFLAGS) $(LIBS) $(KRB5LIBS) $(LDAP_LIBS)

bin/debug2html: $(DEBUG2HTML_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(DEBUG2HTML_OBJ) $(LDFLAGS) $(LIBS)

bin/smbfilter: $(SMBFILTER_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(SMBFILTER_OBJ) $(LDFLAGS) $(LIBS) $(KRB5LIBS) $(LDAP_LIBS)

bin/smbw_sample: $(SMBW_OBJ) utils/smbw_sample.o bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(SMBW_OBJ) utils/smbw_sample.o $(LDFLAGS) $(LIBS) $(KRB5LIBS) $(LDAP_LIBS)

bin/smbsh: $(SMBSH_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(SMBSH_OBJ) $(LDFLAGS) $(DYNEXP) $(LIBS)

bin/smbwrapper.so: $(PICOBJS) bin/.dummy
	@echo Linking shared library $@
	@$(SHLD) $(LDSHFLAGS) -o $@ $(PICOBJS) $(LIBS) \
		$(KRB5LIBS) $(LDAP_LIBS) \
		-Wl,-soname=`basename $@`

bin/libsmbclient.so: $(LIBSMBCLIENT_PICOBJS)
	@echo Linking libsmbclient shared library $@
	@$(SHLD) $(LDSHFLAGS) -o $@ $(LIBSMBCLIENT_PICOBJS) $(LDFLAGS) $(DYNEXP) $(LIBS) \
		$(KRB5LIBS) $(LDAP_LIBS) \
		-Wl,-soname=`basename $@`.$(LIBSMBCLIENT_MAJOR)

bin/libsmbclient.a: $(LIBSMBCLIENT_PICOBJS)
	@echo Linking libsmbclient non-shared library $@
	@-$(AR) -rc $@ $(LIBSMBCLIENT_PICOBJS)

# This is probably wrong for anything other than the GNU linker. 
bin/libbigballofmud.so: $(LIBBIGBALLOFMUD_PICOBJS)
	@echo Linking bigballofmud shared library $@
	@$(SHLD) $(LDSHFLAGS) -o $@ $(LIBBIGBALLOFMUD_PICOBJS) \
		$(PASSDB_LIBS) $(IDMAP_LIBS) $(KRB5LIBS) $(LDAP_LIBS) \
		-Wl,-soname=`basename $@`.$(LIBBIGBALLOFMUD_MAJOR) -lc
	ln -snf libbigballofmud.so bin/libbigballofmud.so.0

# It would be nice to build a static bigballofmud too, but when I try
# I get linker errors about dl_open and similar things.  I'm not sure if
# it can be fixed or if they just can't be called from a static
# library.

libsmbclient: $(LIBSMBCLIENT)

bin/librpc_lsarpc.so: $(RPC_LSA_OBJ)
	@echo "Linking $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(RPC_LSA_OBJ) -lc \
		-Wl,-soname=`basename $@`

bin/librpc_samr.so: $(RPC_SAMR_OBJ)
	@echo "Linking $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(RPC_SAMR_OBJ) -lc \
		-Wl,-soname=`basename $@`

bin/librpc_srvsvc.so: $(RPC_SVC_OBJ)
	@echo "Linking $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(RPC_SVC_OBJ) -lc \
		-Wl,-soname=`basename $@`

bin/librpc_wkssvc.so: $(RPC_WKS_OBJ)
	@echo "Linking $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(RPC_WKS_OBJ) -lc \
		-Wl,-soname=`basename $@`

bin/librpc_NETLOGON.so: $(RPC_NETLOG_OBJ)
	@echo "Linking $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(RPC_NETLOG_OBJ) -lc \
		-Wl,-soname=`basename $@`

bin/librpc_winreg.so: $(RPC_REG_OBJ)
	@echo "Linking $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(RPC_REG_OBJ) -lc \
		-Wl,-soname=`basename $@`

bin/librpc_lsa_ds.so: $(RPC_LSA_DS_OBJ)
	@echo "Linking $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(RPC_LSA_DS_OBJ) -lc \
		-Wl,-soname=`basename $@`

bin/librpc_spoolss.so: $(RPC_SPOOLSS_OBJ)
	@echo "Linking $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(RPC_SPOOLSS_OBJ) -lc \
		-Wl,-soname=`basename $@`

bin/librpc_netdfs.so: $(RPC_DFS_OBJ)
	@echo "Linking $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(RPC_DFS_OBJ) -lc \
		-Wl,-soname=`basename $@`

bin/librpc_echo.so: $(RPC_ECHO_OBJ)
	@echo "Linking $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(RPC_ECHO_OBJ) -lc \
		-Wl,-soname=`basename $@`

bin/winbindd: $(WINBINDD_OBJ) $(POPT_OBJS) bin/.dummy
	@echo "Linking $@"
	@$(LINK) -o $@ $(WINBINDD_OBJ) $(DYNEXP) $(LIBS) $(POPT_OBJS) $(KRB5LIBS) $(LDAP_LIBS) $(PASSDB_LIBS)

# Please don't add .o files to libnss_winbind, libnss_wins, or the pam_winbind
# libraries.  Add to the appropriate PICOBJ variable instead.

nsswitch/libnss_winbind.so: $(WINBIND_NSS_PICOBJS)
	@echo "Linking $@"
	@$(SHLD) $(WINBIND_NSS_LDSHFLAGS) -o $@ $(WINBIND_NSS_PICOBJS) \
		 -Wl,-soname=`basename $@`

nsswitch/libnss_wins.so: $(WINBIND_WINS_NSS_PICOBJS)
	@echo "Linking $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(WINBIND_WINS_NSS_PICOBJS) \
		$(LDAP_LIBS) $(KRB5LIBS) -lc \
		-Wl,-soname=`basename $@`

nsswitch/pam_winbind.so: $(PAM_WINBIND_PICOBJ) bin/.dummy
	@echo "Linking $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(PAM_WINBIND_PICOBJ) \
		-Wl,-soname=`basename $@` -lpam

bin/rhosts.so: $(AUTH_RHOSTS_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(AUTH_RHOSTS_OBJ:.o=.po) -Wl,-soname=`basename $@`

bin/builtin.so: $(AUTH_BUILTIN_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(AUTH_BUILTIN_OBJ:.o=.po) -Wl,-soname=`basename $@`

bin/domain.so: $(AUTH_DOMAIN_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(AUTH_DOMAIN_OBJ:.o=.po) -Wl,-soname=`basename $@`

bin/smbserver.so: $(AUTH_SERVER_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(AUTH_SERVER_OBJ:.o=.po) -Wl,-soname=`basename $@`

bin/winbind.so: $(AUTH_WINBIND_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(AUTH_WINBIND_OBJ:.o=.po) -Wl,-soname=`basename $@`

bin/unix.so: $(AUTH_UNIX_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(AUTH_UNIX_OBJ:.o=.po) -Wl,-soname=`basename $@`

bin/sam.so: $(AUTH_SAM_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(AUTH_SAM_OBJ:.o=.po) -Wl,-soname=`basename $@`

bin/mysql.so: $(MYSQL_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(MYSQL_OBJ:.o=.po)  \
		-Wl,-soname=`basename $@`

bin/pgsql.so: $(PGSQL_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(PGSQL_OBJ:.o=.po)  \
		-Wl,-soname=`basename $@`

bin/ldapsam.so: passdb/pdb_ldap.po
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) $(LDAP_LIBS) -o $@ passdb/pdb_ldap.po \
		-Wl,-soname=`basename $@`

bin/tdbsam.so: passdb/pdb_tdb.po
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ passdb/pdb_tdb.po \
		-Wl,-soname=`basename $@`

bin/smbpasswd.so: passdb/pdb_smbpasswd.po
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ passdb/pdb_smbpasswd.po \
		-Wl,-soname=`basename $@`

bin/weird.so: $(DEVEL_HELP_WEIRD_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(DEVEL_HELP_WEIRD_OBJ:.o=.po) \
		-Wl,-soname=`basename $@`

bin/CP850.so: $(CP850_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(CP850_OBJ:.o=.po) \
		-Wl,-soname=`basename $@`

bin/CP437.so: $(CP437_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(CP437_OBJ:.o=.po) \
		-Wl,-soname=`basename $@`

bin/macosxfs.so: $(CHARSET_MACOSXFS_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(CHARSET_MACOSXFS_OBJ:.o=.po) \
		-framework CoreFoundation -Wl,-soname=`basename $@`

bin/xml.so: $(XML_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(XML_OBJ:.o=.po)  \
		-Wl,-soname=`basename $@`

bin/audit.so: $(VFS_AUDIT_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(VFS_AUDIT_OBJ:.o=.po) \
		-Wl,-soname=`basename $@`

bin/extd_audit.so: $(VFS_EXTD_AUDIT_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(VFS_EXTD_AUDIT_OBJ:.o=.po) \
		-Wl,-soname=`basename $@`

bin/recycle.so: $(VFS_RECYCLE_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(VFS_RECYCLE_OBJ:.o=.po) \
		-Wl,-soname=`basename $@`

bin/netatalk.so: $(VFS_NETATALK_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(VFS_NETATALK_OBJ:.o=.po) \
		-Wl,-soname=`basename $@`

bin/fake_perms.so: $(VFS_FAKE_PERMS_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(VFS_FAKE_PERMS_OBJ:.o=.po) \
		-Wl,-soname=`basename $@`

bin/default_quota.so: $(VFS_DEFAULT_QUOTA_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(VFS_DEFAULT_QUOTA_OBJ:.o=.po) \
		-Wl,-soname=`basename $@`

bin/readonly.so: $(VFS_READONLY_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(VFS_READONLY_OBJ:.o=.po) \
		-Wl,-soname=`basename $@`

bin/cap.so: $(VFS_CAP_OBJ:.o=.po)
	@echo "Building plugin $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(VFS_CAP_OBJ:.o=.po) \
		-Wl,-soname=`basename $@`

bin/wbinfo: $(WBINFO_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(LINK) -o $@ $(WBINFO_OBJ) $(LIBS) $(POPT_OBJS)

bin/ntlm_auth: $(NTLM_AUTH_OBJ) $(PARAM_OBJ) $(LIB_OBJ) \
		$(UBIQX_OBJ) $(POPT_OBJS) bin/.dummy
	@echo Linking $@
	@$(LINK) -o $@ $(NTLM_AUTH_OBJ) $(PARAM_OBJ) $(LIB_OBJ) \
		$(UBIQX_OBJ) $(LIBS) $(POPT_OBJS) $(KRB5LIBS) $(LDAP_LIBS)

bin/pam_smbpass.so: $(PAM_SMBPASS_PICOOBJ)
	@echo "Linking shared library $@"
	@$(SHLD) $(LDSHFLAGS) -o $@ $(PAM_SMBPASS_PICOOBJ) -lpam $(DYNEXP) $(LIBS) -lc $(LDAP_LIBS) $(KRB5LIBS)

bin/libmsrpc.a: $(LIBMSRPC_PICOBJ)
	@-$(AR) -rc $@ $(LIBMSRPC_PICOBJ) 

bin/tdbbackup: $(TDBBACKUP_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(TDBBACKUP_OBJ)

bin/tdbdump: $(TDBDUMP_OBJ) bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(TDBDUMP_OBJ)

#GEW: Big ball of mud is probably overkill.
bin/tdb_restore_secrets: $(TDB_RESTORE_SECRETS_OBJ) $(POPT_OBJS) bin/libbigballofmud.so bin/.dummy
	@echo Linking $@
	@$(CC) $(FLAGS) -o $@ $(TDB_RESTORE_SECRETS_OBJ) $(LDFLAGS) $(PASSDB_LIBS) \
		$(LIBS) $(POPT_OBJS) -Lbin -lbigballofmud

bin/t_strcmp: bin/libbigballofmud.so torture/t_strcmp.o
	$(CC) $(FLAGS) -o $@ $(LIBS) torture/t_strcmp.o -L ./bin -lbigballofmud

bin/t_stringoverflow: bin/libbigballofmud.so torture/t_stringoverflow.o
	$(CC) $(FLAGS) -o $@ torture/t_stringoverflow.o -L./bin -lbigballofmud

bin/t_doschar: bin/libbigballofmud.so torture/t_doschar.o
	$(CC) $(FLAGS) -o $@ $(LIBS) torture/t_doschar.o -L ./bin -lbigballofmud
bin/t_push_ucs2: bin/libbigballofmud.so torture/t_push_ucs2.o
	$(CC) $(FLAGS) -o $@ $(LIBS) torture/t_push_ucs2.o -L ./bin -lbigballofmud

bin/t_snprintf: lib/snprintf.c
	$(CC) $(FLAGS) -o $@ -DTEST_SNPRINTF lib/snprintf.c -lm

#install: installbin installman installscripts installdat installswat installmodules installclientlib
install: installbin


install-everything: install installmodules

# DESTDIR is used here to prevent packagers wasting their time
# duplicating the Makefile. Remove it and you will have the privelege
# of package each samba release for muliple versions of multiple
# distributions and operating systems, or at least supplying patches
# to all the packaging files required for this, prior to committing
# the removal of DESTDIR. Do not remove it even though you think it
# is not used

installdirs:
	@$(SHELL) $(srcdir)/script/installdirs.sh $(DESTDIR)$(BASEDIR) $(DESTDIR)$(BINDIR) $(DESTDIR)$(SBINDIR) $(DESTDIR)$(LIBDIR) $(DESTDIR)$(VARDIR) $(DESTDIR)$(PRIVATEDIR) $(DESTDIR)$(PIDDIR) $(DESTDIR)$(LOCKDIR) $(DESTDIR)$(MANDIR)

installservers: all installdirs
	@$(SHELL) $(srcdir)/script/installbin.sh $(INSTALLPERMS) $(DESTDIR)$(BASEDIR) $(DESTDIR)$(SBINDIR) $(DESTDIR)$(LIBDIR) $(DESTDIR)$(VARDIR) $(SBIN_PROGS)

installbin: all installdirs
	@$(SHELL) $(srcdir)/script/installbin.sh $(INSTALLPERMS) $(DESTDIR) $(DESTDIR)$(BASEDIR) $(DESTDIR)$(SBINDIR) $(DESTDIR)$(LIBDIR) $(DESTDIR)$(VARDIR) $(SBIN_PROGS)
	@$(SHELL) $(srcdir)/script/installbin.sh $(INSTALLPERMS) $(DESTDIR) $(DESTDIR)$(BASEDIR) $(DESTDIR)$(BINDIR) $(DESTDIR)$(LIBDIR) $(DESTDIR)$(VARDIR) $(BIN_PROGS)


# These symlinks are required for the 'probing' of modules. 
# This mechanism should go at some point..
installmodules: modules installdirs
	@$(SHELL) $(srcdir)/script/installmodules.sh $(INSTALLPERMS) $(DESTDIR)$(BASEDIR) $(DESTDIR)$(VFSLIBDIR) $(VFS_MODULES)
	@$(SHELL) $(srcdir)/script/installmodules.sh $(INSTALLPERMS) $(DESTDIR)$(BASEDIR) $(DESTDIR)$(PDBLIBDIR) $(PDB_MODULES)
	@$(SHELL) $(srcdir)/script/installmodules.sh $(INSTALLPERMS) $(DESTDIR)$(BASEDIR) $(DESTDIR)$(RPCLIBDIR) $(RPC_MODULES)
	@$(SHELL) $(srcdir)/script/installmodules.sh $(INSTALLPERMS) $(DESTDIR)$(BASEDIR) $(DESTDIR)$(IDMAPLIBDIR) $(IDMAP_MODULES)
	@$(SHELL) $(srcdir)/script/installmodules.sh $(INSTALLPERMS) $(DESTDIR)$(BASEDIR) $(DESTDIR)$(CHARSETLIBDIR) $(CHARSET_MODULES)
	@$(SHELL) $(srcdir)/script/installmodules.sh $(INSTALLPERMS) $(DESTDIR)$(BASEDIR) $(DESTDIR)$(AUTHLIBDIR) $(AUTH_MODULES)
	@$(SHELL) $(srcdir)/script/linkmodules.sh $(DESTDIR)$(PDBLIBDIR) ldapsam.so ldapsam_compat.so
	@$(SHELL) $(srcdir)/script/linkmodules.sh $(DESTDIR)$(AUTHLIBDIR) rhosts.so hostsequiv.so
	@$(SHELL) $(srcdir)/script/linkmodules.sh $(DESTDIR)$(AUTHLIBDIR) sam.so sam_ignoredomain.so
	@$(SHELL) $(srcdir)/script/linkmodules.sh $(DESTDIR)$(AUTHLIBDIR) domain.so trustdomain.so ntdomain.so
	@$(SHELL) $(srcdir)/script/linkmodules.sh $(DESTDIR)$(AUTHLIBDIR) builtin.so guest.so fixed_challenge.so name_to_ntstatus.so

installscripts: installdirs
	@$(SHELL) $(srcdir)/script/installscripts.sh $(INSTALLPERMS) $(DESTDIR)$(BINDIR) $(SCRIPTS)

installdat: installdirs
	@$(SHELL) $(srcdir)/script/installdat.sh $(DESTDIR)$(LIBDIR) $(srcdir)

installmsg: installdirs
	@$(SHELL) $(srcdir)/script/installmsg.sh $(DESTDIR)$(LIBDIR) $(srcdir)

installswat: installdirs installmsg
	@$(SHELL) $(srcdir)/script/installswat.sh $(DESTDIR)$(SWATDIR) $(srcdir)

installclientlib: installdirs libsmbclient
	@$(SHELL) $(srcdir)/script/installdirs.sh $(DESTDIR)${prefix}/lib
	-$(INSTALLCLIENTCMD_SH) bin/libsmbclient.so $(DESTDIR)${prefix}/lib
	-$(INSTALLCLIENTCMD_A) bin/libsmbclient.a $(DESTDIR)${prefix}/lib
	@$(SHELL) $(srcdir)/script/installdirs.sh $(DESTDIR)${prefix}/include
	-$(INSTALLCMD) $(srcdir)/include/libsmbclient.h $(DESTDIR)${prefix}/include

# Python extensions

PYTHON_OBJS = $(PARAM_OBJ) $(LIB_OBJ) $(LIBSMB_OBJ) $(RPC_PARSE_OBJ) \
	$(UBIQX_OBJ) $(LIBMSRPC_OBJ) $(PASSDB_OBJ) $(GROUPDB_OBJ) \
	$(SECRETS_OBJ) $(KRBCLIENT_OBJ) $(SMBLDAP_OBJ) lib/dummyroot.o

PYTHON_PICOBJS = $(PYTHON_OBJS:.o=.po)

python_ext: $(PYTHON_PICOBJS)
	@if test -z "$(PYTHON)"; then \
		echo Use the option --with-python to configure python; \
		exit 1; \
	fi
	PYTHON_OBJS="$(PYTHON_PICOBJS)" \
	PYTHON_CFLAGS="$(CFLAGS) $(CPPFLAGS) $(FLAGS)" \
	LIBS="$(LIBS) $(PASSDB_LIBS) $(IDMAP_LIBS) $(KRB5LIBS) $(LDAP_LIBS)" \
		$(PYTHON) python/setup.py build

python_install: $(PYTHON_PICOBJS)
	@if test -z "$(PYTHON)"; then \
		echo Use the option --with-python to configure python; \
		exit 1; \
	fi
	PYTHON_OBJS="$(PYTHON_PICOBJS)" \
	PYTHON_CFLAGS="$(CFLAGS) $(CPPFLAGS)" \
	LIBS="$(LIBS)" \
		$(PYTHON) python/setup.py install

python_clean:
	@-if test -n "$(PYTHON)"; then $(PYTHON) python/setup.py clean; fi

# revert to the previously installed version
revert:
	@$(SHELL) $(srcdir)/script/revert.sh $(SBINDIR) $(SBIN_PROGS) 
	@$(SHELL) $(srcdir)/script/revert.sh $(BINDIR) $(BIN_PROGS) $(SCRIPTS)

installman: installdirs
	@$(SHELL) $(srcdir)/script/installman.sh $(DESTDIR)$(MANDIR) $(srcdir) $(man_langs) ""

.PHONY: showlayout

showlayout: 
	@echo "Samba will be installed into:"
	@echo "  basedir:     $(BASEDIR)"
	@echo "  bindir:      $(BINDIR)"
	@echo "  sbindir:     $(SBINDIR)"
	@echo "  libdir:      $(LIBDIR)"
	@echo "  vardir:      $(VARDIR)"
	@echo "  mandir:      $(MANDIR)"
	@echo "  privatedir:  $(PRIVATE_DIR)"
	@echo "  configdir:   $(CONFIGDIR)"
	@echo "  lockdir:     $(LOCKDIR)"
	@echo "  piddir:      $(PIDDIR)"


uninstall: uninstallman uninstallbin uninstallscripts uninstallmodules

uninstallman:
	@$(SHELL) $(srcdir)/script/uninstallman.sh $(DESTDIR)$(MANDIR) $(srcdir) $(man_langs)

uninstallbin:
	@$(SHELL) $(srcdir)/script/uninstallbin.sh $(INSTALLPERMS) $(DESTDIR)$(BASEDIR) $(DESTDIR)$(SBINDIR) $(DESTDIR)$(LIBDIR) $(DESTDIR)$(VARDIR) $(DESTDIR)$(SBIN_PROGS)
	@$(SHELL) $(srcdir)/script/uninstallbin.sh $(INSTALLPERMS) $(DESTDIR)$(BASEDIR) $(DESTDIR)$(BINDIR) $(DESTDIR)$(LIBDIR) $(DESTDIR)$(VARDIR) $(DESTDIR)$(BIN_PROGS)

uninstallmodules:
	@$(SHELL) $(srcdir)/script/uninstallmodules.sh $(INSTALLPERMS) $(DESTDIR)$(BASEDIR) $(DESTDIR)$(VFSLIBDIR) $(DESTDIR)$(VFS_MODULES)
	@$(SHELL) $(srcdir)/script/uninstallmodules.sh $(INSTALLPERMS) $(DESTDIR)$(BASEDIR) $(DESTDIR)$(PDBLIBDIR) $(DESTDIR)$(PDB_MODULES)
	@$(SHELL) $(srcdir)/script/uninstallmodules.sh $(INSTALLPERMS) $(DESTDIR)$(BASEDIR) $(DESTDIR)$(RPCLIBDIR) $(DESTDIR)$(RPC_MODULES)
	@$(SHELL) $(srcdir)/script/uninstallmodules.sh $(INSTALLPERMS) $(DESTDIR)$(BASEDIR) $(DESTDIR)$(CHARSETLIBDIR) $(DESTDIR)$(CHARSET_MODULES)
	@$(SHELL) $(srcdir)/script/uninstallmodules.sh $(INSTALLPERMS) $(DESTDIR)$(BASEDIR) $(DESTDIR)$(AUTHLIBDIR) $(DESTDIR)$(AUTH_MODULES)

uninstallscripts:
	@$(SHELL) $(srcdir)/script/uninstallscripts.sh $(INSTALLPERMS) $(DESTDIR)$(BINDIR) $(SCRIPTS)

# Toplevel clean files
TOPFILES=dynconfig.o dynconfig.po

clean: delheaders python_clean
	-rm -f core */*~ *~ */*.o */*.po */*.so \
		$(TOPFILES) $(BIN_PROGS) $(SBIN_PROGS) $(MODULES) $(TORTURE_PROGS) \
		$(LIBSMBCLIENT) $(EVERYTHING_PROGS) .headers.stamp

# Making this target will just make sure that the prototype files
# exist, not necessarily that they are up to date.  Since they're
# removed by "make clean" this will always be run when you do anything
# afterwards.
proto_exists: include/proto.h include/wrepld_proto.h include/build_env.h \
	nsswitch/winbindd_proto.h web/swat_proto.h \
	client/client_proto.h utils/net_proto.h smbd/build_options.c

delheaders:
	@echo Removing prototype headers
	@rm -f include/proto.h include/build_env.h  include/wrepld_proto.h \
		nsswitch/winbindd_proto.h web/swat_proto.h \
		client/client_proto.h utils/net_proto.h \
		smbd/build_options.c

MKPROTO_SH = $(srcdir)/script/mkproto.sh

include/proto.h: smbd/build_options.c
	@echo Building include/proto.h
	@cd $(srcdir) && $(SHELL) $(MKPROTO_SH) $(AWK) \
	  -h _PROTO_H_ $(builddir)/include/proto.h \
	  $(PROTO_OBJ)

include/build_env.h: script/build_env.sh
	@echo Building include/build_env.h
	@$(SHELL) $(srcdir)/script/build_env.sh $(srcdir) $(builddir) $(CC) \
	  > $(builddir)/include/build_env.h

include/wrepld_proto.h:
	@echo Building include/wrepld_proto.h
	@cd $(srcdir) && $(SHELL) $(MKPROTO_SH) $(AWK) \
	  -h _WREPLD_PROTO_H_ $(builddir)/include/wrepld_proto.h \
	  $(WREPL_OBJ1)

nsswitch/winbindd_proto.h: 
	@cd $(srcdir) && $(SHELL) $(MKPROTO_SH) $(AWK) \
	  -h _WINBINDD_PROTO_H_ $(builddir)/nsswitch/winbindd_proto.h \
	  $(WINBINDD_OBJ1)

web/swat_proto.h: 
	@cd $(srcdir) && $(SHELL) $(MKPROTO_SH) $(AWK) \
	  -h _SWAT_PROTO_H_ $(builddir)/web/swat_proto.h \
	  $(SWAT_OBJ1)

client/client_proto.h: 
	@cd $(srcdir) && $(SHELL) $(MKPROTO_SH) $(AWK) \
	  -h _CLIENT_PROTO_H_ $(builddir)/client/client_proto.h \
	  $(CLIENT_OBJ1)

utils/net_proto.h: 
	@cd $(srcdir) && $(SHELL) $(MKPROTO_SH) $(AWK) \
	  -h _CLIENT_PROTO_H_ $(builddir)/utils/net_proto.h \
	  $(NET_OBJ1)

# "make headers" or "make proto" calls a subshell because we need to
# make sure these commands are executed in sequence even for a
# parallel make.
headers: 
	$(MAKE) delheaders; \
	$(MAKE) smbd/build_options.c; \
	$(MAKE) include/proto.h; \
	$(MAKE) include/build_env.h; \
	$(MAKE) include/wrepld_proto.h; \
	$(MAKE) nsswitch/winbindd_proto.h; \
	$(MAKE) web/swat_proto.h; \
	$(MAKE) client/client_proto.h; \
	$(MAKE) utils/net_proto.h

proto: headers 

.PHONY: headers proto

etags:
	etags `find $(srcdir) -name "*.[ch]" | grep -v /CVS/`

ctags:
	ctags `find $(srcdir) -name "*.[ch]" | grep -v /CVS/`

realclean: clean delheaders
	-rm -f config.log bin/.dummy script/findsmb

distclean: realclean
	-rm -f include/stamp-h
	-rm -f include/config.h Makefile
	-rm -f config.status config.cache so_locations
	-rm -rf .deps TAGS

realdistclean: distclean
	-rm -f include/config.h.in
	-rm -f include/version.h
	-rm -f configure

# this target is really just for my use. It only works on a limited
# range of machines and is used to produce a list of potentially
# dead (ie. unused) functions in the code. (tridge)
finddead:
	nm */*.o |grep 'U ' | awk '{print $$2}' | sort -u > nmused.txt
	nm */*.o |grep 'T ' | awk '{print $$3}' | sort -u > nmfns.txt
	comm -13 nmused.txt nmfns.txt 


# when configure.in is updated, reconfigure
$(srcdir)/configure: $(srcdir)/configure.in
	@echo "WARNING: you need to rerun ./autogen.sh"

#config.status: $(srcdir)/configure
#	@echo "WARNING: you need to run ./configure"

#Makefile: $(srcdir)/Makefile.in config.status
#	@echo "WARNING: you need to run ./config.status"

######################################################################
# Samba Testing Framework

# FIXME: LD_LIBRARY_PATH is not portable, but in the absence of
# libtool I don't know a better way to do it.  Perhaps we should fix
# libbigballofmud to link statically?

check: check-programs
	LD_LIBRARY_PATH="`pwd`/bin:$$LD_LIBRARY_PATH" \
	PATH="`pwd`/bin:$$PATH" \
	python stf/standardcheck.py; \
	if test -n "$(PYTHON)"; then \
		python stf/pythoncheck.py; \
	fi

# These are called by the test suite and need to be built before
# running it.  For the time being we don't build all of BIN_PROGS,
# because they're not all needed.
check-programs: bin/t_strcmp bin/t_push_ucs2 bin/smbcontrol bin/t_snprintf
