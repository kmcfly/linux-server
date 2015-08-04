// For GetCurrentDate, GetCurrentTime
#include <sys/types.h>
#include <sys/stat.h>

#include <sys/time.h>

#include <stdarg.h>  //only included in implementation file
#include <stdio.h>   //only included in implementation file

#include <unistd.h>  // for gethostname
#include <netdb.h>   // for gethostbyname

// For GetMACAddress
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <sys/ioctl.h>  // For SIOCGIFCONF on Linux

#include <sys/socket.h>
//#include <sys/sockio.h>  // For SIOCGIFCONF on SunOS
#include <net/if.h>
#include <netinet/in.h>   //For IPPROTO_IP
#ifdef CMAKE_HAVE_NET_IF_DL_H
#include <net/if_dl.h>
#endif
#if defined(CMAKE_HAVE_NET_IF_ARP_H) && defined(__sun)
// This is absolutely necessary on SunOS
#include <net/if_arp.h>
#endif
#include <stdio.h>
#include <unistd.h>
#include <string.h>
//#include <stropts.h>


#include <netinet/in.h>
#include <netinet/if_ether.h>

#include <arpa/inet.h>
/*
// For GetCurrentThreadID()
#ifdef __linux__
#include <sys/types.h>
#include <linux/unistd.h>
#endif
#ifdef __sun
#include <thread.h>
#endif
*/

int GetMacAddr( unsigned char *addr,char *szLocalIP )
{

// implementation for POSIX system
	int       sd;
	struct ifreq    ifr, *ifrp;
	struct ifconf    ifc;
	char buf[1024];
	int      n, i;
	unsigned char    *a;
	struct sockaddr_in *sin;

	#if defined(AF_LINK) && (!defined(SIOCGIFHWADDR) && !defined(SIOCGENADDR))
	struct sockaddr_dl *sdlp;
	#endif


// BSD 4.4 defines the size of an ifreq to be
// max(sizeof(ifreq), sizeof(ifreq.ifr_name)+ifreq.ifr_addr.sa_len
// However, under earlier systems, sa_len isn't present, so the size is 
// just sizeof(struct ifreq)
// We should investigate the use of SIZEOF_ADDR_IFREQ
//
#ifdef HAVE_SA_LEN
	#ifndef max
      #define max(a,b) ((a) > (b) ? (a) : (b))
	#endif
	#define ifreq_size(i) max(sizeof(struct ifreq),\
      sizeof((i).ifr_name)+(i).ifr_addr.sa_len)
#else
	#define ifreq_size(i) sizeof(struct ifreq)
#endif // HAVE_SA_LEN

	if ( (sd = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP)) < 0 )
	{
	      return -1;
	}
	memset(buf, 0, sizeof(buf));
	ifc.ifc_len = sizeof(buf);
	ifc.ifc_buf = buf;
	if (ioctl (sd, SIOCGIFCONF, (char *)&ifc) < 0)
	{
	      close(sd);
	      return -1;
	}
	n = ifc.ifc_len;
	for (i = 0; i < n; i+= ifreq_size(*ifrp) )
	{
	      ifrp = (struct ifreq *)((char *) ifc.ifc_buf+i);
	      strncpy(ifr.ifr_name, ifrp->ifr_name, IFNAMSIZ);


	#ifdef SIOCGIFHWADDR
	      if (ioctl(sd, SIOCGIFHWADDR, &ifr) < 0)
	         continue;
	      a = (unsigned char *) &ifr.ifr_hwaddr.sa_data;
	#else
		#ifdef SIOCGENADDR
	      // In theory this call should also work on Sun Solaris, but apparently
	      // SIOCGENADDR is not implemented properly thus the call 
	      // ioctl(sd, SIOCGENADDR, &ifr) always returns errno=2 
	      // (No such file or directory)
	      // Furthermore the DLAPI seems to require full root access
	      if (ioctl(sd, SIOCGENADDR, &ifr) < 0)
	         continue;
	      a = (unsigned char *) ifr.ifr_enaddr;
		#else
			#ifdef AF_LINK
		      sdlp = (struct sockaddr_dl *) &ifrp->ifr_addr;
		      if ((sdlp->sdl_family != AF_LINK) || (sdlp->sdl_alen != 6))
		         continue;
		      a = (unsigned char *) &sdlp->sdl_data[sdlp->sdl_nlen];
			#else
			      perror("in Get MAC Adress (internal) : No way to access hardware");
			      close(sd);
			      return -1;
			#endif // AF_LINK
		#endif // SIOCGENADDR
	#endif // SIOCGIFHWADDR
	      if (!a[0] && !a[1] && !a[2] && !a[3] && !a[4] && !a[5]) continue;
		
	      if (addr) 
	      {
	         memcpy(addr, a, 6);
		 sin = (struct sockaddr_in *)&ifrp->ifr_addr;
             	 sprintf(szLocalIP,"%s", inet_ntoa(sin->sin_addr));
             	 //printf(" IP     %s \n", inet_ntoa(sin->sin_addr));

	         close(sd);
	         return 0;
	      }
	   }
	   close(sd);
//	#endif
	   // Not implemented platforms (or no Ethernet cable !)
	   
	   //perror("Probabely your computer is not connected on a network, therefore its MAC adress cannot be found (or there is a configuration problem on your platform)");
	
	   // But the following -> error: invalid use of 'this' in non-member function
	   //gdcmWarningMacro( "Probabely your computer is not connected on a network, therefore its MAC adress cannot be found (or there is a configuration problem on your platform)");
	
	   memset(addr,0,6);
	   return -1;
}

/*
int main()
{
	unsigned char byMac[6]="";
	char szLocalIP[32];
	int i;
	GetMacAddr (byMac,szLocalIP); 
	
	for(i=0; i<6; i++)
		printf("%02X",byMac[i]);
	printf("\n");
	return 0;
}
*/
