#ifndef __TUTK_COMMON__
#define __TUTK_COMMON__


const unsigned int RDT_WAIT_TIMEMS = 30000;
const unsigned int IOTC_MAX_BUF_SIZE = 102400;
const unsigned int IOTC_MAX_IOCTRL_BUF_LEN = 1500;

const char p2pHostPrimary[] = {"m1.iotcplatform.com"};
const char p2pHostSecondary[] = {"m2.iotcplatform.com"};
const char p2pHostThird[] = {"m3.iotcplatform.com"};
const char p2pHostFourth[] = {"m4.iotcplatform.com"};

const char IOTC_MODE[][4] = 
{
	"P2P",
	"RLY",
	"LAN"
};


#endif