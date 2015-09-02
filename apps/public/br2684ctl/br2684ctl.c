#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string.h>
#include <syslog.h>
#include <atm.h>
#include <linux/atmdev.h>
#include <linux/atmbr2684.h>

#define fetch __atmlib_fetch

/* Written by Marcell GAL <cell@sch.bme.hu> to make use of the */
/* ioctls defined in the br2684... kernel patch */
/* Compile with cc -o br2684ctl br2684ctl.c -latm */

/*
  Modified feb 2001 by Stephen Aaskov (saa@lasat.com)
  - Added daemonization code
  - Added syslog
  
  TODO: Delete interfaces after exit?
*/


#define LOG_NAME "RFC1483/2684 bridge"
#define LOG_OPTION     LOG_PERROR
#define LOG_FACILITY   LOG_LOCAL0

struct br2684_params {
  int itfnum;
  int encap;
  int sndbuf;
  int payload;
  char *astr; /* temporary */
  int qos_type; 
  int pcr; 
  int scr; 
  int mbs; 
};

int lastsock, lastitf;


void fatal(char *str, int i)
{
  syslog (LOG_ERR,"Fatal: %s",str);
  exit(-2);
};


void exitFunc(void)
{
  syslog (LOG_PID,"Daemon terminated\n");	
}


int create_br(int itfnum, int payload)
{
  int err;
  
  if(lastsock<0) {
    lastsock = socket(PF_ATMPVC, SOCK_DGRAM, ATM_AAL5);
  }
  if (lastsock<0) {
    syslog(LOG_ERR, "socket creation failed: %s",strerror(errno));
  } else {
    /* create the device with ioctl: */
    if( itfnum>=0 && itfnum<1234567890){
      struct atm_newif_br2684 ni;
      ni.backend_num = ATM_BACKEND_BR2684;
      ni.media = BR2684_MEDIA_ETHERNET;
	  if (payload == 0)
        ni.media |= BR2684_FLAG_ROUTED;
      ni.mtu = 1500;
      sprintf(ni.ifname, "nas%d", itfnum);
      err=ioctl (lastsock, ATM_NEWBACKENDIF, &ni);
  
      if (err == 0)
		syslog(LOG_INFO, "Interface \"%s\" created sucessfully\n",ni.ifname);
      else
		syslog(LOG_INFO, "Interface \"%s\" could not be created, reason: %s\n",
	       ni.ifname,
	       strerror(errno));
      lastitf=itfnum;	/* even if we didn't create, because existed, assign_vcc will want to know it! */
    } else {
      syslog(LOG_ERR,"err: strange interface number %d", itfnum );
    }
  }
  return 0;
}

int get_type(const char *type)
{
  int result;
  int item;
  
  item = fetch(&type,"!none","ubr","cbr","rt-vbr", "nrt-vbr", NULL);
  switch (item) {
    case 1: /* ATM_UBR */
      break;
    case 2: /* ATM_CBR */
      break;
    case 3: /* ATM_VBR */
      break;
    case 4:
	return ATM_nrtVBR;
    default: 
      return ATM_UBR;
  }
  
  return result=item;
}

int assign_vcc(char *astr, int encap, int payload,
		int qos_type, int pcr, int scr, int mbs, int bufsize)
{
    int err, errno;
    struct atm_qos qos;
    struct sockaddr_atmpvc addr;
    int fd;
    struct atm_backend_br2684 be;

    memset(&addr, 0, sizeof(addr));
    err=text2atm(astr,(struct sockaddr *)(&addr), sizeof(addr), T2A_PVC);
    if (err!=0)
      syslog(LOG_ERR,"Could not parse ATM parameters (error=%d)\n",err);
    
#if 0
    addr.sap_family = AF_ATMPVC;
    addr.sap_addr.itf = itf;
    addr.sap_addr.vpi = 0;
    addr.sap_addr.vci = vci;
#endif
    syslog(LOG_INFO,"Communicating over ATM %d.%d.%d, encapsulation: %s\n", addr.sap_addr.itf,
	   addr.sap_addr.vpi,
	   addr.sap_addr.vci,
	   encap?"VC mux":"LLC");
    
    if ((fd = socket(PF_ATMPVC, SOCK_DGRAM, ATM_AAL5)) < 0)
      syslog(LOG_ERR,"failed to create socket %d, reason: %s", errno,strerror(errno));
    
    
    memset(&qos, 0, sizeof(qos));
    qos.aal                     = ATM_AAL5;
    /* qos.txtp.traffic_class      = ATM_UBR; */
    qos.txtp.traffic_class      = qos_type;
    qos.txtp.max_sdu            = 1524;
    /* qos.txtp.pcr                = ATM_MAX_PCR; */
    qos.txtp.pcr                = pcr; 
    qos.txtp.scr                = scr; 
    qos.txtp.mbs                = mbs; 
    qos.rxtp = qos.txtp;

    fprintf(stderr, "qos.txtp.traffic_class = %d\n", qos.txtp.traffic_class);

    if ( (err=setsockopt(fd,SOL_SOCKET,SO_SNDBUF, &bufsize ,sizeof(bufsize))) )
      syslog(LOG_ERR,"setsockopt SO_SNDBUF: (%d) %s\n",err, strerror(err));
    
    if (setsockopt(fd, SOL_ATM, SO_ATMQOS, &qos, sizeof(qos)) < 0)
      syslog(LOG_ERR,"setsockopt SO_ATMQOS %d", errno);

    err = connect(fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_atmpvc));
    
    if (err < 0)
      fatal("failed to connect on socket", err);
    
    /* attach the vcc to device: */
    
    be.backend_num = ATM_BACKEND_BR2684;
    be.ifspec.method = BR2684_FIND_BYIFNAME;
    sprintf(be.ifspec.spec.ifname, "nas%d", lastitf);
    be.fcs_in = BR2684_FCSIN_NO;
    be.fcs_out = BR2684_FCSOUT_NO;
    be.fcs_auto = 0;
    be.encaps = encap ? BR2684_ENCAPS_VC : BR2684_ENCAPS_LLC; 
    be.has_vpiid = 0;
    be.send_padding = 0;
    be.min_size = 0;
    err=ioctl (fd, ATM_SETBACKEND, &be);
    if (err == 0)
      syslog (LOG_INFO,"Interface configured");
    else {
      syslog (LOG_ERR,"Could not configure interface:%s",strerror(errno));
      exit(2);
    }
    return fd ;
}

void start_interface(struct br2684_params* params)
{
  if (params->astr==NULL) {
    syslog(LOG_ERR, "Required ATM parameters not specified.");
    exit(1);
  }

  create_br(params->itfnum, params->payload);
  assign_vcc(params->astr, params->encap, params->payload, params->qos_type, 
		  params->pcr, params->scr, params->mbs, params->sndbuf);
}

void usage(char *s)
{
  printf("usage: %s [-b] [[-c number] [-e 0|1|2|3] [-t ubr|cbr|vbr] [-p pcr] [-q scr] [-m mbs] [-a [itf.]vpi.vci]*]*\n", s);
  printf("  -e 0 = Bridged LLC encapsulation\n");
  printf("  -e 1 = Bridged VC-Mux encapsulation\n");
  printf("  -e 2 = Routed LLC encapsulation\n");
  printf("  -e 3 = Routed VC-Mux encapsulation\n\n");
exit(1);
}


int main (int argc, char **argv)
{
  int c, background=0;

  struct br2684_params params;
  params.itfnum=-1;
  params.encap=0;
  params.sndbuf=8192;
  params.payload=1;
  params.astr=NULL;
  params.qos_type=ATM_UBR;
  params.pcr=0;
  params.scr=0;
  params.mbs=0;
  
  lastsock=-1;
  lastitf=0;

  openlog (LOG_NAME,LOG_OPTION,LOG_FACILITY);
  if (argc>1)
    while ((c = getopt(argc, argv,"a:bc:e:t:p:q:m:s:?h")) !=EOF)
      switch (c) {
      case 'a':
		params.astr=optarg;
	    break;
      case 'b':
	    background=1;
	    break;
      case 'c':
		/* temporary, to make it work with multiple interfaces: */
		if (params.itfnum>=0) start_interface(&params);
		params.itfnum= atoi(optarg);
		break;
      case 'e':
		params.encap=(atoi(optarg));
		if(params.encap<0){
	  	  syslog (LOG_ERR, "invalid encapsulation: %s:\n",optarg);
	 	  params.encap=0;
		} else if(params.encap >= 2) {
			params.encap -= 2;
			params.payload = 0;
		}
		break;
      case 't':
        params.qos_type=get_type(optarg);
        break;
      case 'p':
        params.pcr=(atoi(optarg));
        break;
      case 'q':
        params.scr=(atoi(optarg));
        break;
      case 'm':
        params.mbs=(atoi(optarg));
        break;
      case 's':
		params.sndbuf=(atoi(optarg));
		if(params.sndbuf<0){
	  	  syslog(LOG_ERR, "Invalid sndbuf: %s, using size of 8192 instead\n",optarg);
	  	  params.sndbuf=8192;
		}
		break;
      case '?':
      case 'h':
      default:
		usage(argv[0]);
      }
  else
    usage(argv[0]);

  if (argc != optind) usage(argv[0]);

  start_interface(&params);
  
  if(lastsock>=0) close(lastsock);
  
  if (background) {
    pid_t pid;
    
    pid=fork();
    if (pid < 0) {
      fprintf(stderr,"Error detaching\n");
      exit(2);
    } else if (pid) 
      exit(0); // This is the parent
    
    // Become a process group and session group leader
    if (setsid()<0) {
      fprintf (stderr,"Could not set process group\n");
      exit(2);
    }
    
    // Fork again to let process group leader exit
    pid = fork();
    if (pid < 0) {
      fprintf(stderr,"Error detaching during second fork\n");
      exit(2);
    } else if (pid)
      exit(0); // This is the parent
    
    // Now we're ready for buisness
    chdir("/");            // Don't keep directories in use
    close(0); close(1); close(2);  // Close stdin, -out and -error
    /*
      Note that this implementation does not keep an open 
      stdout/err.
      If we need them they can be opened now
    */
    
  }
  
  syslog (LOG_INFO, "RFC 1483/2684 bridge daemon started\n");	
  atexit (exitFunc);
  
  while (1) sleep(30);	/* to keep the sockets... */
  return 0;
}

