Please follow below steps to build the GPL release code

1. prepare environment
 a.Centos 6.3
	sudo yum groupinstall "Development Tools"
	sudo yum install gettext-devel libxml2-devel mtd-utils-ubi ncurses-devel zlib-devel intltool sharutils docbook-dtds docbook-style-xsl dos2unix compat-libstdc++-33-3.2.3-69.el6.i686

 b.Ubuntu 12.04
	Install these packages

	build-essential
	bison
	flex
	gawk
	dos2unix
	libstdc++5

2. extract toolchain
	tar -zxvf ./toolchain/mips-linux-uclibc.tgz -C /opt/trendchip/

3. setup environment parameter
	fakeroot
	(suggest upgrade fakeroot to 1.18 or use root account directly in CentOS 6.3,
	 or chmod would run very slow...)
	export PROFILE={Profile Name}

	Profile Name include
		dsl-n10-c1
		dsl-n12e-c1
		dsl-n12u-c1
		dsl-n14u
		dsl-n16
		dsl-n16u
		dsl-n17u
		dsl-n55u-c1
		dsl-n55u-d1
		dsl-n66u

	Please choose proper Profile Name,
	ex.
	If DSL-N14U GPL is downloaded, please type following command
		export PROFILE=dsl-n14u

4. build code
	make

5.clean code
	make clean
