#include "DDPublic.h"
#include "soapServ.h"
#include "OnvifSoapH.h"
#include "DigestGen.h"

#ifndef WIN32
#include "uuid.h"
#endif

#ifdef __cplusplus
extern "C"
{
#endif

char *gDiscoverWsaTo	="urn:schemas-xmlsoap-org:ws:2005:04:discovery";
char *gDiscoverActionTo	="http://schemas.xmlsoap.org/ws/2005/04/discovery/Probe";
char *gPasswordType		="http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest";
char *gHttpVer			="1.1";


SOAP_NMAC struct Namespace discovery_namespaces[] =
{
	{"SOAP-ENV", "http://www.w3.org/2003/05/soap-envelope", "http://schemas.xmlsoap.org/soap/envelope/", NULL},
	{"SOAP-ENC", "http://www.w3.org/2003/05/soap-encoding", "http://schemas.xmlsoap.org/soap/encoding/", NULL},
	{"xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL},
	{"xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL},
	{"wsa", "http://schemas.xmlsoap.org/ws/2004/08/addressing", NULL, NULL},
	{"dndl", "http://www.onvif.org/ver10/network/wsdl/DiscoveryLookupBinding", NULL, NULL},
	{"dnrd", "http://www.onvif.org/ver10/network/wsdl/RemoteDiscoveryBinding", NULL, NULL},
	{"d", "http://schemas.xmlsoap.org/ws/2005/04/discovery", NULL, NULL},
	{"dn", "http://www.onvif.org/ver10/network/wsdl", NULL, NULL},
    {"wsse", "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd", NULL, NULL},
	{"wsu",  "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd",NULL,NULL},
	{NULL, NULL, NULL, NULL}
};

SOAP_NMAC struct Namespace devicemgmt_namespaces[] =
{
	{"SOAP-ENV", "http://www.w3.org/2003/05/soap-envelope", "http://schemas.xmlsoap.org/soap/envelope/", NULL},
	{"SOAP-ENC", "http://www.w3.org/2003/05/soap-encoding", "http://schemas.xmlsoap.org/soap/encoding/", NULL},
	{"xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL},
	{"xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL},
	{"wsa5", "http://www.w3.org/2005/08/addressing", "http://schemas.xmlsoap.org/ws/2004/08/addressing", NULL},
	{"xmime", "http://tempuri.org/xmime.xsd", NULL, NULL},
	{"xop", "http://www.w3.org/2004/08/xop/include", NULL, NULL},
	{"tt", "http://www.onvif.org/ver10/schema", NULL, NULL},
	{"ns1", "http://docs.oasis-open.org/wsrf/bf-2", NULL, NULL},
	{"wsnt", "http://docs.oasis-open.org/wsn/b-2", NULL, NULL},
	{"wstop", "http://docs.oasis-open.org/wsn/t-1", NULL, NULL},
	{"tds", "http://www.onvif.org/ver10/device/wsdl", NULL, NULL},
	{"wsse", "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd", NULL, NULL},
	{"wsu",  "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd",NULL,NULL},
	{NULL, NULL, NULL, NULL}
};

SOAP_NMAC struct Namespace imageing_namespaces[] =
{
	{"SOAP-ENV", "http://www.w3.org/2003/05/soap-envelope", "http://schemas.xmlsoap.org/soap/envelope/", NULL},
	{"SOAP-ENC", "http://www.w3.org/2003/05/soap-encoding", "http://schemas.xmlsoap.org/soap/encoding/", NULL},
	{"xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL},
	{"xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL},
	{"wsa5", "http://www.w3.org/2005/08/addressing", "http://schemas.xmlsoap.org/ws/2004/08/addressing", NULL},
	{"xmime", "http://tempuri.org/xmime.xsd", NULL, NULL},
	{"xop", "http://www.w3.org/2004/08/xop/include", NULL, NULL},
	{"tt", "http://www.onvif.org/ver10/schema", NULL, NULL},
	{"ns1", "http://docs.oasis-open.org/wsrf/bf-2", NULL, NULL},
	{"wsnt", "http://docs.oasis-open.org/wsn/b-2", NULL, NULL},
	{"wstop", "http://docs.oasis-open.org/wsn/t-1", NULL, NULL},
	{"timg", "http://www.onvif.org/ver20/imaging/wsdl", NULL, NULL},
    {"wsse", "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd", NULL, NULL},
	{"wsu",  "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd",NULL,NULL},
	{NULL, NULL, NULL, NULL}
};

SOAP_NMAC struct Namespace media_namespaces[] =
{
	{"SOAP-ENV", "http://www.w3.org/2003/05/soap-envelope", "http://schemas.xmlsoap.org/soap/envelope/", NULL},
	{"SOAP-ENC", "http://www.w3.org/2003/05/soap-encoding", "http://schemas.xmlsoap.org/soap/encoding/", NULL},
	{"xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL},
	{"xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL},
	{"wsa5", "http://www.w3.org/2005/08/addressing", "http://schemas.xmlsoap.org/ws/2004/08/addressing", NULL},
	{"xmime", "http://tempuri.org/xmime.xsd", NULL, NULL},
	{"xop", "http://www.w3.org/2004/08/xop/include", NULL, NULL},
	{"tt", "http://www.onvif.org/ver10/schema", NULL, NULL},
	{"ns1", "http://docs.oasis-open.org/wsrf/bf-2", NULL, NULL},
	{"wsnt", "http://docs.oasis-open.org/wsn/b-2", NULL, NULL},
	{"wstop", "http://docs.oasis-open.org/wsn/t-1", NULL, NULL},
	{"trt", "http://www.onvif.org/ver10/media/wsdl", NULL, NULL},
	{"wsse", "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd", NULL, NULL},
	{"wsu",  "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd",NULL,NULL},
	{NULL, NULL, NULL, NULL}
};

SOAP_NMAC struct Namespace event_namespaces[] =
{
	{"SOAP-ENV", "http://www.w3.org/2003/05/soap-envelope", "http://schemas.xmlsoap.org/soap/envelope/", NULL},
	{"SOAP-ENC", "http://www.w3.org/2003/05/soap-encoding", "http://schemas.xmlsoap.org/soap/encoding/", NULL},
	{"xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL},
	{"xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL},
	{"wsa5", "http://www.w3.org/2005/08/addressing", "http://schemas.xmlsoap.org/ws/2004/08/addressing", NULL},
	{"xmime", "http://tempuri.org/xmime.xsd", NULL, NULL},
	{"xop", "http://www.w3.org/2004/08/xop/include", NULL, NULL},
	{"ns4", "http://docs.oasis-open.org/wsrf/bf-2", NULL, NULL},
	{"wstop", "http://docs.oasis-open.org/wsn/t-1", NULL, NULL},
	{"tt", "http://www.onvif.org/ver10/schema", NULL, NULL},
	{"ns3", "http://docs.oasis-open.org/wsrf/r-2", NULL, NULL},
	{"ns10", "http://www.onvif.org/ver10/events/wsdl/PullPointBinding", NULL, NULL},
	{"ns11", "http://www.onvif.org/ver10/events/wsdl/CreatePullPointBinding", NULL, NULL},
	{"ns12", "http://www.onvif.org/ver10/events/wsdl/PausableSubscriptionManagerBinding", NULL, NULL},
	{"ns5", "http://www.onvif.org/ver10/events/wsdl/PullPointSubscriptionBinding", NULL, NULL},
	{"ns6", "http://www.onvif.org/ver10/events/wsdl/EventBinding", NULL, NULL},
	{"tev", "http://www.onvif.org/ver10/events/wsdl", NULL, NULL},
	{"ns7", "http://www.onvif.org/ver10/events/wsdl/SubscriptionManagerBinding", NULL, NULL},
	{"ns8", "http://www.onvif.org/ver10/events/wsdl/NotificationProducerBinding", NULL, NULL},
	{"wsnt", "http://docs.oasis-open.org/wsn/b-2", NULL, NULL},
	{"ns9", "http://www.onvif.org/ver10/events/wsdl/NotificationConsumerBinding", NULL, NULL},
	{"tns1","http://www.onvif.org/ver10/topics",NULL,NULL},
	{"wsse", "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd", NULL, NULL},
	{"wsu",  "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd",NULL,NULL},
	{NULL, NULL, NULL, NULL}
};

SOAP_NMAC struct Namespace analytics_namespaces[] =
{
	{"SOAP-ENV", "http://www.w3.org/2003/05/soap-envelope", "http://schemas.xmlsoap.org/soap/envelope/", NULL},
	{"SOAP-ENC", "http://www.w3.org/2003/05/soap-encoding", "http://schemas.xmlsoap.org/soap/encoding/", NULL},
	{"xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL},
	{"xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL},
	{"wsa5", "http://www.w3.org/2005/08/addressing", "http://schemas.xmlsoap.org/ws/2004/08/addressing", NULL},
	{"xmime", "http://tempuri.org/xmime.xsd", NULL, NULL},
	{"xop", "http://www.w3.org/2004/08/xop/include", NULL, NULL},
	{"tt", "http://www.onvif.org/ver10/schema", NULL, NULL},
	{"ns1", "http://docs.oasis-open.org/wsrf/bf-2", NULL, NULL},
	{"wsnt", "http://docs.oasis-open.org/wsn/b-2", NULL, NULL},
	{"wstop", "http://docs.oasis-open.org/wsn/t-1", NULL, NULL},
	{"ns2", "http://www.onvif.org/ver20/analytics/wsdl/RuleEngineBinding", NULL, NULL},
	{"ns3", "http://www.onvif.org/ver20/analytics/wsdl/AnalyticsEngineBinding", NULL, NULL},
	{"tan", "http://www.onvif.org/ver20/analytics/wsdl", NULL, NULL},
	{"wsse", "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd", NULL, NULL},
	{"wsu",  "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd",NULL,NULL},
	{NULL, NULL, NULL, NULL}
};

SOAP_NMAC struct Namespace ptz_namespaces[] =
{
	{"SOAP-ENV", "http://www.w3.org/2003/05/soap-envelope", "http://schemas.xmlsoap.org/soap/envelope/", NULL},
	{"SOAP-ENC", "http://www.w3.org/2003/05/soap-encoding", "http://schemas.xmlsoap.org/soap/encoding/", NULL},
	{"xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL},
	{"xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL},
	{"wsa5", "http://www.w3.org/2005/08/addressing", "http://schemas.xmlsoap.org/ws/2004/08/addressing", NULL},
	{"xmime", "http://tempuri.org/xmime.xsd", NULL, NULL},
	{"xop", "http://www.w3.org/2004/08/xop/include", NULL, NULL},
	{"tt", "http://www.onvif.org/ver10/schema", NULL, NULL},
	{"ns1", "http://docs.oasis-open.org/wsrf/bf-2", NULL, NULL},
	{"wsnt", "http://docs.oasis-open.org/wsn/b-2", NULL, NULL},
	{"wstop", "http://docs.oasis-open.org/wsn/t-1", NULL, NULL},
	{"tptz", "http://www.onvif.org/ver20/ptz/wsdl", NULL, NULL},
	{"wsse", "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd", NULL, NULL},
	{"wsu",  "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd",NULL,NULL},
	{NULL, NULL, NULL, NULL}
};

SOAP_NMAC struct Namespace namespaces[] =
{
	{"SOAP-ENV", "http://www.w3.org/2003/05/soap-envelope", "http://schemas.xmlsoap.org/soap/envelope/", NULL},
	{"SOAP-ENC", "http://www.w3.org/2003/05/soap-encoding", "http://schemas.xmlsoap.org/soap/encoding/", NULL},
	{"xsi", "http://www.w3.org/2001/XMLSchema-instance", "http://www.w3.org/*/XMLSchema-instance", NULL},
	{"xsd", "http://www.w3.org/2001/XMLSchema", "http://www.w3.org/*/XMLSchema", NULL},
	{"wsa5", "http://www.w3.org/2005/08/addressing", "http://schemas.xmlsoap.org/ws/2004/08/addressing", NULL},
	{"c14n", "http://www.w3.org/2001/10/xml-exc-c14n#", NULL, NULL},
	{"wsu", "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd", NULL, NULL},
	{"xenc", "http://www.w3.org/2001/04/xmlenc#", NULL, NULL},
	{"ds", "http://www.w3.org/2000/09/xmldsig#", NULL, NULL},
	{"wsse", "http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd", "http://docs.oasis-open.org/wss/oasis-wss-wssecurity-secext-1.1.xsd", NULL},
	{"xmime", "http://tempuri.org/xmime.xsd", NULL, NULL},
	{"xop", "http://www.w3.org/2004/08/xop/include", NULL, NULL},
	{"wsa", "http://schemas.xmlsoap.org/ws/2004/08/addressing", NULL, NULL},
	{"tt", "http://www.onvif.org/ver10/schema", NULL, NULL},
	{"wsbf2", "http://docs.oasis-open.org/wsrf/bf-2", NULL, NULL},
	{"wstop", "http://docs.oasis-open.org/wsn/t-1", NULL, NULL},
	{"d", "http://schemas.xmlsoap.org/ws/2005/04/discovery", NULL, NULL},
	{"wsr2", "http://docs.oasis-open.org/wsrf/r-2", NULL, NULL},
	{"daae", "http://www.onvif.org/ver20/analytics/wsdl/AnalyticsEngineBinding", NULL, NULL},
	{"dare", "http://www.onvif.org/ver20/analytics/wsdl/RuleEngineBinding", NULL, NULL},
	{"tan", "http://www.onvif.org/ver20/analytics/wsdl", NULL, NULL},
	{"decpp", "http://www.onvif.org/ver10/events/wsdl/CreatePullPointBinding", NULL, NULL},
	{"dee", "http://www.onvif.org/ver10/events/wsdl/EventBinding", NULL, NULL},
	{"denc", "http://www.onvif.org/ver10/events/wsdl/NotificationConsumerBinding", NULL, NULL},
	{"denf", "http://www.onvif.org/ver10/events/wsdl/NotificationProducerBinding", NULL, NULL},
	{"depp", "http://www.onvif.org/ver10/events/wsdl/PullPointBinding", NULL, NULL},
	{"depps", "http://www.onvif.org/ver10/events/wsdl/PullPointSubscriptionBinding", NULL, NULL},
	{"tev", "http://www.onvif.org/ver10/events/wsdl", NULL, NULL},
	{"depsm", "http://www.onvif.org/ver10/events/wsdl/PausableSubscriptionManagerBinding", NULL, NULL},
	{"wsnt", "http://docs.oasis-open.org/wsn/b-2", NULL, NULL},
	{"desm", "http://www.onvif.org/ver10/events/wsdl/SubscriptionManagerBinding", NULL, NULL},
	{"dndl", "http://www.onvif.org/ver10/network/wsdl/DiscoveryLookupBinding", NULL, NULL},
	{"dnrd", "http://www.onvif.org/ver10/network/wsdl/RemoteDiscoveryBinding", NULL, NULL},
	{"dn", "http://www.onvif.org/ver10/network/wsdl", NULL, NULL},
	{"tad", "http://www.onvif.org/ver10/analyticsdevice/wsdl", NULL, NULL},
	{"tds", "http://www.onvif.org/ver10/device/wsdl", NULL, NULL},
	{"timg", "http://www.onvif.org/ver20/imaging/wsdl", NULL, NULL},
	{"tls", "http://www.onvif.org/ver10/display/wsdl", NULL, NULL},
	{"tmd", "http://www.onvif.org/ver10/deviceIO/wsdl", NULL, NULL},
	{"tptz", "http://www.onvif.org/ver20/ptz/wsdl", NULL, NULL},
	{"trc", "http://www.onvif.org/ver10/recording/wsdl", NULL, NULL},
	{"trp", "http://www.onvif.org/ver10/replay/wsdl", NULL, NULL},
	{"trt", "http://www.onvif.org/ver10/media/wsdl", NULL, NULL},
	{"trv", "http://www.onvif.org/ver10/receiver/wsdl", NULL, NULL},
	{"tse", "http://www.onvif.org/ver10/search/wsdl", NULL, NULL},
	{NULL, NULL, NULL, NULL}
};


CDigestGen  g_cls_digestKeyObj;
CPUB_Lock  g_lockDigest;


struct soap *onvif_init_soap(enum ENAMESPACETYPE aNameType,bool bDiscover)
{
	struct soap *in_soap=NULL;
    const struct Namespace *p = NULL;
	
	in_soap = soap_new(); 

	if(NULL == in_soap)
	{
		return NULL;
	}
   switch(aNameType)
   {
		case TDEVICE_DISCOVERY:
			p= discovery_namespaces;
			break;
		case  TDEVICE_REMOTE_DISCOVERY:
           p=NULL;
			break;
		case TDEVICE_MGT:
			p=devicemgmt_namespaces;
			break;
		case  TDEVICE_IMAGING:
			p=imageing_namespaces;
			break;
		case  TDEVICE_MEDIA:
			p=media_namespaces;
			break;
		case TDEVICE_EVENT:
			p=event_namespaces;
			break;
		case TDEVICE_PTZ:
			p =ptz_namespaces;
			break;
		case TDEVICE_ANALYTICS:
			p=analytics_namespaces;
			break;
		default:
			p =NULL;
			break;
   }
	soap_set_namespaces(in_soap, p);

	if(bDiscover)
	{
		in_soap->recv_timeout = 2;
		//in_soap->send_timeout = 2;
	}
	else
	{
		in_soap->recv_timeout = 6;
		//in_soap->send_timeout = 1; 
       in_soap->connect_timeout = 6;

		in_soap->mode = 1;
		in_soap->imode = 1;
		in_soap->omode = 1;
		in_soap->dime_id_format = NULL;
		in_soap->http_version = gHttpVer;//"1.1";
	}

	return in_soap;
}


void onvif_free_soap(struct soap *in_soap)
{
    in_soap->namespaces = NULL;

	soap_destroy(in_soap); // remove deserialized class instances (C++ only)
	soap_end(in_soap); // clean up and remove deserialized data

	//soap_done(in_soap); // detach context (last use and no longer in scope)
	soap_free(in_soap);//detach and free runtime context
}

struct SOAP_ENV__Header *onvif_init_soap_header(struct soap *aSoap,bool bDiscover,char *messageID,char *aUserName,char *aPassKey,bool bDigest,ONVIF_INIT_INPUT_PARAM* pInputParam)
{
	struct SOAP_ENV__Header *soapHeader = NULL;
	struct _wsse__Password *tPassword =NULL;
	struct _wsse__Security *tSecurity =NULL;
	struct _wsse__UsernameToken *tUsernameToken = NULL;
    //CDigestGen  digestKeyObj;
	unsigned int  digestKeyLen = 0;

	char *nonce =NULL;
	char *created   =NULL;
	unsigned char *digestKey = NULL;

	soapHeader = (struct SOAP_ENV__Header *)malloc(sizeof(struct SOAP_ENV__Header));
	if(soapHeader == NULL)
	{
		return NULL;
	}
     
    memset(soapHeader,0,sizeof(struct SOAP_ENV__Header));

	//soap_default_SOAP_ENV__Header(aSoap, soapHeader);
    
	soapHeader->bDigest        = _false;
	soapHeader->ns1_customerId  = NULL;
	soapHeader->password       = NULL;
	soapHeader->username       = NULL;
	soapHeader->wsa__MessageID  = messageID;
	if(bDiscover)
	{
		soapHeader->bDiscover = _true;
		soapHeader->wsa__Action = gDiscoverActionTo;
		soapHeader->wsa__To = gDiscoverWsaTo;
	}
	else
	{
		soapHeader->bDiscover = _false;
		soapHeader->wsa__Action = NULL;
		soapHeader->wsa__To = NULL;
	//}
	//if(!bDiscover)
	//{
		if(!bDigest)
		{
			soapHeader->ns1_customerId = NULL;

			soapHeader->username = aUserName;
			soapHeader->password = aPassKey;
			soapHeader->wsse__Security = NULL;
			soapHeader->bDigest  = _false;
		}
		else
		{

			soapHeader->bDigest  = _true;

			tSecurity = (struct _wsse__Security *)malloc(sizeof(struct _wsse__Security));
			if(tSecurity == NULL)
			{
				free( soapHeader );
				soapHeader = NULL;
				return NULL;
			 }
			 memset(tSecurity,0,sizeof(struct _wsse__Security));

			 tUsernameToken = (struct _wsse__UsernameToken*)malloc(sizeof(struct _wsse__UsernameToken));
			 if(tUsernameToken == NULL)
			 {
				 free(tSecurity);
				 tSecurity = NULL;
				 free( soapHeader );
				 soapHeader = NULL;
				 return NULL;
			 }
			 memset(tUsernameToken,0,sizeof(struct _wsse__UsernameToken));


			 tPassword = (struct _wsse__Password *)malloc(sizeof(struct _wsse__Password));
			 if(tPassword == NULL)
			 {
				 free(tSecurity);
				 tSecurity = NULL;
				 free(tUsernameToken);
				 tUsernameToken = NULL;
				 free( soapHeader );
				 soapHeader = NULL;
				 return NULL;
			 }
			 memset(tPassword,0,sizeof(struct _wsse__Password));

			 digestKey = (unsigned char *)malloc(sizeof(unsigned char)*64);
			 nonce     = (char *)malloc(sizeof(char)*64);
			 created   = (char *)malloc(sizeof(char)*64);

            memset(digestKey,0,sizeof(char)*64);
			 memset(nonce,0,sizeof(char)*64);
			 memset(created,0,sizeof(char)*64);
            
			 g_lockDigest.Lock();
			 g_cls_digestKeyObj.GetWSSPassWdDiget(aPassKey,(unsigned int)strlen(aPassKey),digestKey,digestKeyLen,nonce,created,&pInputParam->stDevUtcTime);
			 g_lockDigest.UnLock();
		 
			 tPassword->__item				= (char *)digestKey;
			 tPassword->Type				 = gPasswordType;

			 tUsernameToken->Nonce			 = nonce;
			 tUsernameToken->Password       = tPassword;
			 tUsernameToken->Username		 = aUserName;
			 tUsernameToken->wsu__Created	 = created;
			 tUsernameToken->wsu__Id		 = NULL; 

			 tSecurity->BinarySecurityToken	 = NULL;
			 tSecurity->ds__Signature		 = NULL;
			 tSecurity->SOAP_ENV__actor		 = NULL;
			 tSecurity->SOAP_ENV__role		 = NULL;
			 tSecurity->UsernameToken		 = tUsernameToken;
			 tSecurity->wsu__Timestamp		 = NULL;
			 tSecurity->xenc__EncryptedKey	 = NULL;
			 tSecurity->xenc__ReferenceList	 = NULL;
		   
			 soapHeader->wsse__Security = tSecurity;
		}
	}
	return soapHeader;
}

struct SOAP_ENV__Header *onvif_init_discover_soap_header(char *messageID)
{
	struct SOAP_ENV__Header *soapHeader = NULL;

	soapHeader = (struct SOAP_ENV__Header *)malloc(sizeof(struct SOAP_ENV__Header));
	if(soapHeader == NULL)
	{
		return NULL;
	}

	memset(soapHeader,0,sizeof(struct SOAP_ENV__Header));

	//soap_default_SOAP_ENV__Header(aSoap, soapHeader);

	soapHeader->wsa__MessageID = messageID;	
	soapHeader->wsa__RelatesTo = NULL;	
	soapHeader->wsa__From = NULL;	
	soapHeader->wsa__ReplyTo = NULL;	
	soapHeader->wsa__FaultTo =NULL;	
	soapHeader->wsa__To = gDiscoverWsaTo;	
	soapHeader->wsa__Action = gDiscoverActionTo;	

	//not discovery
	soapHeader->bDiscover = _true; 

	//not encyption
    soapHeader->bDigest = _false; 
	soapHeader->username = NULL;
	soapHeader->password =NULL;
	soapHeader->ns1_customerId =NULL;

	//encyption
	soapHeader->wsa5__MessageID = NULL;	
	soapHeader->wsa5__RelatesTo = NULL;	
	soapHeader->wsa5__From = NULL;	
	soapHeader->wsa5__ReplyTo = NULL;	
	soapHeader->wsa5__FaultTo = NULL;	
	soapHeader->wsa5__To = NULL;	
	soapHeader->wsa5__Action = NULL;	
	soapHeader->wsse__Security = NULL;	

    return soapHeader;
}

void onvif_free_soap_header(struct SOAP_ENV__Header *aHeader)
{
	if(aHeader != NULL)
	{
		aHeader->username = NULL;
		aHeader->password = NULL;
		if(aHeader->wsse__Security != NULL)
		{		
			if(aHeader->wsse__Security->UsernameToken != NULL)
			{
				if(aHeader->wsse__Security->UsernameToken->Password != NULL)
				{
					aHeader->wsse__Security->UsernameToken->Username = NULL;
					if(aHeader->wsse__Security->UsernameToken->Password->__item != NULL)
						free(aHeader->wsse__Security->UsernameToken->Password->__item);
					aHeader->wsse__Security->UsernameToken->Password->__item = NULL;
					aHeader->wsse__Security->UsernameToken->Password->Type  =NULL;
					
					if(aHeader->wsse__Security->UsernameToken->Nonce != NULL)
						free(aHeader->wsse__Security->UsernameToken->Nonce);
					aHeader->wsse__Security->UsernameToken->Nonce = NULL;
                  
					if(aHeader->wsse__Security->UsernameToken->wsu__Created != NULL)
						free(aHeader->wsse__Security->UsernameToken->wsu__Created);
					aHeader->wsse__Security->UsernameToken->wsu__Created = NULL;
                   
					if(aHeader->wsse__Security->UsernameToken->Password != NULL)
						free(aHeader->wsse__Security->UsernameToken->Password);
					aHeader->wsse__Security->UsernameToken->Password = NULL;
				}

				free(aHeader->wsse__Security->UsernameToken);
				aHeader->wsse__Security->UsernameToken = NULL;
			}

			free(aHeader->wsse__Security);
			aHeader->wsse__Security = NULL;
		}
		free(aHeader);
	}
	aHeader = NULL;
}

bool onvif_soap_generate_uuid(char *out_uuid)
{
#ifdef WIN32

	GUID tmpuuid;

	ZeroMemory(&tmpuuid,sizeof(GUID));
	if(CoCreateGuid(&tmpuuid)==S_OK)
	{
		sprintf(out_uuid,"urn:uuid:%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
			tmpuuid.Data1,
			tmpuuid.Data2,
			tmpuuid.Data3,
			tmpuuid.Data4[0], 
			tmpuuid.Data4[1], 
			tmpuuid.Data4[2], 
			tmpuuid.Data4[3], 
			tmpuuid.Data4[4], 
			tmpuuid.Data4[5], 
			tmpuuid.Data4[6], 
			tmpuuid.Data4[7]);
		return TRUE;
	}
	return FALSE;

#else

	unsigned char uuOut[16];
	char  tmpBuf[64];
	memset(tmpBuf,'\0',sizeof(tmpBuf));
	uuid_generate(uuOut);
	sprintf(tmpBuf,"urn:uuid:%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		uuOut[0],
		uuOut[1],
		uuOut[2],
		uuOut[3],
		uuOut[4],
		uuOut[5],
		uuOut[6],
		uuOut[7],
		uuOut[8], 
		uuOut[9], 
		uuOut[10], 
		uuOut[11], 
		uuOut[12], 
		uuOut[13], 
		uuOut[14], 
		uuOut[15]);

        //printf("****uuid=%s\n",out_uuid);
	strcpy(out_uuid,tmpBuf);
	return true;
#endif

}

#ifdef __cplusplus
}
#endif


COnvifSoap::COnvifSoap()
{
	m_pSoap = NULL;
	m_pHeader = NULL;

	memset( &stOnvifInputParam, 0, sizeof( ONVIF_INIT_INPUT_PARAM));

}

COnvifSoap::~COnvifSoap()
{
	//ExitSoap();
}

int COnvifSoap::ProduceSoapInfo(enum ENAMESPACETYPE aNameType,bool bWsse,char *aUser,char *aPassKey)
{
	int result = -1;
	result  = InitSoap(aNameType,false);
	if(result < 0)
	{
		return SOAP_INIT_FAULT;
	}

	result = InitSoapHeader();
	if(result < 0)
	{
		FreeSoap();
		return SOAP_INIT_FAULT;
	}
	AddUsernamePassKey(bWsse,aUser,aPassKey);
	m_pSoap->header = m_pHeader;

	return SOAP_INIT_SUCCESS;
}

int COnvifSoap::ProduceSoapInfo(enum ENAMESPACETYPE aNameType,bool bWsse,char *aUser,char *aPassKey,ONVIF_PUBLIC_UTC_TIME * aDevUtcTime)
{
	int result = -1;
	if(aDevUtcTime == NULL)
	{
		return -1;
	}

	result  = InitSoap(aNameType,false,aDevUtcTime);
	if(result < 0)
	{
		return SOAP_INIT_FAULT;
	}

	result = InitSoapHeader();
	if(result < 0)
	{
		FreeSoap();
		return SOAP_INIT_FAULT;
	}
	if(aDevUtcTime->uYear== 0)
	{
		AddUsernamePassKey(bWsse,aUser,aPassKey);
	}
	else
	{
		AddUsernamePassKey(bWsse,aUser,aPassKey,false);
	}
	m_pSoap->header = m_pHeader;

	return SOAP_INIT_SUCCESS;
}

void COnvifSoap::ExitSoap(void)
{
	if(m_pSoap != NULL)
	{
		if(m_pSoap->header != NULL)
		{
			m_pSoap->header = NULL;
		}
	}
	FreeSoapHeader();
	m_pHeader = NULL;
	FreeSoap();
	m_pSoap =NULL;
}

void COnvifSoap::InitUtcTimeByExist(ONVIF_INIT_INPUT_PARAM* pstDevTime)
{
	memcpy( &stOnvifInputParam, pstDevTime, sizeof(ONVIF_INIT_INPUT_PARAM) );
}

void COnvifSoap::InitDevUtcTimes( bool bClearUtcTime, char* aServAddr, unsigned int ipcCtrlType )
{
	if( bClearUtcTime )
	{
		memset( &stOnvifInputParam, 0, sizeof( ONVIF_INIT_INPUT_PARAM));
		return;
	}

	if( ONVIF_CTRL_DEV_USED_UTC_TIEM == ipcCtrlType )
	{
		Onvif_GetDev_UtcTime(aServAddr,&stOnvifInputParam);
	}
}


int COnvifSoap::InitSoap(enum ENAMESPACETYPE aNameType,bool bDiscover)
{
	const struct Namespace *p = NULL;

	m_pSoap = soap_new(); 

	if(NULL == m_pSoap)
	{
		return -1;
	}
	switch(aNameType)
	{
	case TDEVICE_DISCOVERY:
		p= discovery_namespaces;
		break;
	case  TDEVICE_REMOTE_DISCOVERY:
		p=NULL;
		break;
	case TDEVICE_MGT:
		p=devicemgmt_namespaces;
		break;
	case  TDEVICE_IMAGING:
		p=imageing_namespaces;
		break;
	case  TDEVICE_MEDIA:
		p=media_namespaces;
		break;
	case TDEVICE_EVENT:
		p=event_namespaces;
		break;
	case TDEVICE_PTZ:
		p =ptz_namespaces;
		break;
	case TDEVICE_ANALYTICS:
		p=analytics_namespaces;
		break;
	default:
		p =namespaces;
		break;
	}
	soap_set_namespaces(m_pSoap, p);

	if(bDiscover)
	{
		m_pSoap->recv_timeout = 6;
		m_pSoap->send_timeout = 6;
		m_pSoap->port = 3702; //discovery port
	}
	else
	{
		m_pSoap->recv_timeout = 6;
		m_pSoap->send_timeout = 6;
		m_pSoap->connect_timeout = 6;
		m_pSoap->accept_timeout  = 6;

		m_pSoap->mode = 1;
		m_pSoap->imode = 1;
		m_pSoap->omode = 1;
		m_pSoap->dime_id_format = NULL;
		m_pSoap->http_version = gHttpVer;//"1.1";
	}
	return 0;
}

int COnvifSoap::InitSoap(enum ENAMESPACETYPE aNameType,bool bDiscover,ONVIF_PUBLIC_UTC_TIME *aDevUtcTime)
{
	const struct Namespace *p = NULL;

	m_pSoap = soap_new(); 

	if(NULL == m_pSoap || aDevUtcTime == NULL)
	{
		return -1;
	}

	memcpy(&stOnvifInputParam.stDevUtcTime,aDevUtcTime,sizeof(ONVIF_PUBLIC_UTC_TIME));

	switch(aNameType)
	{
	case TDEVICE_DISCOVERY:
		p= discovery_namespaces;
		break;
	case  TDEVICE_REMOTE_DISCOVERY:
		p=NULL;
		break;
	case TDEVICE_MGT:
		p=devicemgmt_namespaces;
		break;
	case  TDEVICE_IMAGING:
		p=imageing_namespaces;
		break;
	case  TDEVICE_MEDIA:
		p=media_namespaces;
		break;
	case TDEVICE_EVENT:
		p=event_namespaces;
		break;
	case TDEVICE_PTZ:
		p =ptz_namespaces;
		break;
	case TDEVICE_ANALYTICS:
		p=analytics_namespaces;
		break;
	default:
		p =namespaces;
		break;
	}
	soap_set_namespaces(m_pSoap, p);

	if(bDiscover)
	{
		m_pSoap->recv_timeout = 3;
		m_pSoap->send_timeout = 2;
		m_pSoap->port = 3702; //discovery port
	}
	else
	{
		m_pSoap->recv_timeout = 3;
		m_pSoap->send_timeout = 2;
		m_pSoap->connect_timeout = 1;
		m_pSoap->accept_timeout  = 1;

		m_pSoap->mode = 1;
		m_pSoap->imode = 1;
		m_pSoap->omode = 1;
		m_pSoap->dime_id_format = NULL;
		m_pSoap->http_version = gHttpVer;//"1.1";
	}

	return 0;
}

void COnvifSoap::FreeSoap(void)
{
	//m_pSoap->header = NULL;
	//soap_destroy(m_pSoap); // remove deserialized class instances (C++ only)

	soap_end(m_pSoap); // clean up and remove deserialized data

	if(m_pSoap != NULL)
	{
		soap_free(m_pSoap);//detach and free runtime context
	}
	m_pSoap = NULL;

	//soap_done(in_soap); // detach context (last use and no longer in scope)
}

int COnvifSoap::InitSoapHeader(bool bDiscover)
{

	m_pHeader = (struct SOAP_ENV__Header *)malloc(sizeof(struct SOAP_ENV__Header));
	if(m_pHeader == NULL)
	{
		return -1;
	}
	memset(m_pHeader,0,sizeof(struct SOAP_ENV__Header));
	if(m_pSoap != NULL)
	{
		soap_default_SOAP_ENV__Header(m_pSoap, m_pHeader);	
	}
	m_pHeader->bDiscover = (enum xsd__boolean_)bDiscover;
	return 0;
}

void COnvifSoap::FreeSoapHeader(void)
{
	if(m_pSoap != NULL)
	{
		m_pSoap->header = NULL;
	}
	if(m_pHeader != NULL)
	{
		m_pHeader->username = NULL;
		m_pHeader->password = NULL;

		m_pHeader->wsa__To = NULL;
		m_pHeader->wsa__Action = NULL;

		m_pHeader->wsa5__Action = NULL;
		m_pHeader->wsa5__To = NULL;


		if(m_pHeader->wsa__MessageID != NULL)
		{
			free(m_pHeader->wsa__MessageID);
		}
		m_pHeader->wsa__MessageID = NULL;

		if(m_pHeader->wsa5__MessageID != NULL)
		{
			free(m_pHeader->wsa5__MessageID);
		}
		m_pHeader->wsa5__MessageID =NULL;

		if(m_pHeader->wsse__Security != NULL)
		{

			if(m_pHeader->wsse__Security->UsernameToken != NULL)
			{
				if(m_pHeader->wsse__Security->UsernameToken->Password != NULL)
				{
					if(m_pHeader->wsse__Security->UsernameToken->Password->__item != NULL)
					{
						free(m_pHeader->wsse__Security->UsernameToken->Password->__item);
					}
					m_pHeader->wsse__Security->UsernameToken->Password->__item = NULL;
					m_pHeader->wsse__Security->UsernameToken->Password->Type  =NULL;

					if(m_pHeader->wsse__Security->UsernameToken->Nonce != NULL)
					{
						free(m_pHeader->wsse__Security->UsernameToken->Nonce);
					}
					m_pHeader->wsse__Security->UsernameToken->Nonce = NULL;

					if(m_pHeader->wsse__Security->UsernameToken->wsu__Created != NULL)
					{
						free(m_pHeader->wsse__Security->UsernameToken->wsu__Created);
					}
					m_pHeader->wsse__Security->UsernameToken->wsu__Created = NULL;

					free(m_pHeader->wsse__Security->UsernameToken->Password);
					m_pHeader->wsse__Security->UsernameToken->Password = NULL;
				}

				free(m_pHeader->wsse__Security->UsernameToken);
				m_pHeader->wsse__Security->UsernameToken = NULL;
			}

			free(m_pHeader->wsse__Security);
			m_pHeader->wsse__Security = NULL;
		}
		free(m_pHeader);
	}
	m_pHeader = NULL;
}

void COnvifSoap::AddHeaderToSoap(void)
{
	if(m_pSoap != NULL && m_pHeader != NULL)
	{
		m_pSoap->header = m_pHeader;
	}
}

void COnvifSoap::AddUsernamePassKey(int bWsse,char *aUsername,char *aPassKey)
{
	struct _wsse__Password *tPassword =NULL;
	struct _wsse__Security *tSecurity =NULL;
	struct _wsse__UsernameToken *tUsernameToken = NULL;

	//CDigestGen  digestKeyObj;
	unsigned char *digestKey=NULL;
	unsigned int  digestKeyLen = 0;
	char *aNonce=NULL;
	char *aCreated=NULL;
	int len = 64;

	if(m_pHeader == NULL)
	{
		return;
	}

	if(!bWsse)
	{
		m_pHeader->bDigest = _false;
		m_pHeader->username = aUsername;
		m_pHeader->password = aPassKey;

	}
	else
	{
		bool bAllocMemOk = false;
		do 
		{
			m_pHeader->bDigest = _true;
			tSecurity = (struct _wsse__Security *)malloc(sizeof(struct _wsse__Security));
			tUsernameToken = (struct _wsse__UsernameToken*)malloc(sizeof(struct _wsse__UsernameToken));
			tPassword = (struct _wsse__Password *)malloc(sizeof(struct _wsse__Password));
			if( !tSecurity || !tPassword || !tUsernameToken )
			{
				break;
			}
			memset(tPassword,0,sizeof(struct _wsse__Password));
			memset(tSecurity,0,sizeof(struct _wsse__Security));
			memset(tUsernameToken,0,sizeof(struct _wsse__UsernameToken));

			digestKey = (unsigned char *)malloc(len*sizeof(unsigned char));
			aNonce    = (char *)malloc(len*sizeof(char));
			aCreated  = (char *)malloc(len*sizeof(char));
			if( !digestKey || !aNonce || !aCreated )
			{
				break;
			}

			memset(digestKey,0,sizeof(unsigned char)*len);
			memset(aNonce,0,sizeof(char)*len);
			memset(aCreated,0,sizeof(char)*len);

			bAllocMemOk = true;

		} while (0);

		if( bAllocMemOk )
		{
			g_lockDigest.Lock();
			g_cls_digestKeyObj.GetWSSPassWdDiget(aPassKey,strlen(aPassKey),(unsigned char *)digestKey,digestKeyLen,aNonce,aCreated,&stOnvifInputParam.stDevUtcTime);
			g_lockDigest.UnLock();

			tPassword->__item				= (char *)digestKey;
			tPassword->Type					= gPasswordType;	//"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-username-token-profile-1.0#PasswordDigest";

			tUsernameToken->Nonce			 = aNonce;
			tUsernameToken->Password       = tPassword;
			tUsernameToken->Username		 = aUsername;
			tUsernameToken->wsu__Created	 = aCreated;
			tUsernameToken->wsu__Id		 = NULL; 

			tSecurity->BinarySecurityToken	 = NULL;
			tSecurity->ds__Signature		 = NULL;
			tSecurity->SOAP_ENV__actor		 = NULL;
			tSecurity->SOAP_ENV__role		 = NULL;
			tSecurity->UsernameToken		 = tUsernameToken;
			tSecurity->wsu__Timestamp		 = NULL;
			tSecurity->xenc__EncryptedKey	 = NULL;
			tSecurity->xenc__ReferenceList	 = NULL;

			m_pHeader->wsse__Security = tSecurity;
		}
		else
		{
			if( tSecurity )
			{
				free( tSecurity );
				tSecurity = NULL;
			}

			if( tPassword )
			{
				free( tPassword );
				tPassword = NULL;
			}

			if( tUsernameToken )
			{
				free( tUsernameToken );
				tUsernameToken = NULL;
			}

			if( digestKey )
			{
				free( digestKey );
				digestKey = NULL;
			}

			if( aNonce )
			{
				free( aNonce );
				aNonce = NULL;
			}

			if( aCreated )
			{
				free( aCreated );
				aCreated = NULL;
			}
		}
	}
}

void COnvifSoap::AddUsernamePassKey(int bWsse,char *aUsername,char *aPassKey,bool bLocalTime)
{
	struct _wsse__Password *tPassword =NULL;
	struct _wsse__Security *tSecurity =NULL;
	struct _wsse__UsernameToken *tUsernameToken = NULL;

	//CDigestGen  digestKeyObj;
	unsigned char *digestKey=NULL;
	unsigned int  digestKeyLen = 0;
	char *aNonce=NULL;
	char *aCreated=NULL;
	int len = 64;

	if(m_pHeader == NULL)
	{
		return;
	}

	if(!bWsse)
	{
		m_pHeader->bDigest = _false;
		m_pHeader->username = aUsername;
		m_pHeader->password = aPassKey;

	}
	else
	{
		m_pHeader->bDigest = _true;
		tSecurity = (struct _wsse__Security *)malloc(sizeof(struct _wsse__Security));
		if(tSecurity == NULL)
		{
			return;
		}
		memset(tSecurity,0,sizeof(struct _wsse__Security));

		tUsernameToken = (struct _wsse__UsernameToken*)malloc(sizeof(struct _wsse__UsernameToken));
		if(tUsernameToken == NULL)
		{
			free(tSecurity);
			tSecurity = NULL;
			return;
		}
		memset(tUsernameToken,0,sizeof(struct _wsse__UsernameToken));

		tPassword = (struct _wsse__Password *)malloc(sizeof(struct _wsse__Password));
		if(tPassword == NULL)
		{
			free(tSecurity);
			tSecurity = NULL;
			free(tUsernameToken);
			tUsernameToken = NULL;
			return;
		}
		memset(tPassword,0,sizeof(struct _wsse__Password));

		digestKey = (unsigned char *)malloc(len*sizeof(unsigned char));
		if(digestKey == NULL)
		{
			free(tSecurity);
			tSecurity = NULL;
			free(tUsernameToken);
			tUsernameToken = NULL;
			free(tPassword);
			tPassword = NULL;
			return;
		}
		aNonce    = (char *)malloc(len*sizeof(char));
		if(aNonce == NULL)
		{
			free(tSecurity);
			tSecurity = NULL;
			free(tUsernameToken);
			tUsernameToken = NULL;
			free(tPassword);
			tPassword = NULL;
			free(digestKey);
			digestKey = NULL;
			return;
		}
		aCreated  = (char *)malloc(len*sizeof(char));
		if(aCreated == NULL)
		{
			free(tSecurity);
			tSecurity = NULL;
			free(tUsernameToken);
			tUsernameToken = NULL;
			free(tPassword);
			tPassword = NULL;
			free(digestKey);
			digestKey = NULL;
			free(aNonce);
			aNonce = NULL;
			return;
		}

		memset(digestKey,0,sizeof(unsigned char)*len);
		memset(aNonce,0,sizeof(char)*len);
		memset(aCreated,0,sizeof(char)*len);


		g_lockDigest.Lock();
		if(bLocalTime)
		{
			//digestKeyObj.GetWSSPassWdDiget(aPassKey,strlen(aPassKey),(unsigned char *)digestKey,digestKeyLen,aNonce,aCreated);
			g_cls_digestKeyObj.GetWSSPassWdDiget(aPassKey,strlen(aPassKey),(unsigned char *)digestKey,digestKeyLen,aNonce,aCreated);
		}
		else
		{
			//digestKeyObj.GetWSSPassWdDiget(aPassKey,strlen(aPassKey),(unsigned char *)digestKey,digestKeyLen,aNonce,aCreated,&m_devUtcTime);
			g_cls_digestKeyObj.GetWSSPassWdDiget(aPassKey,strlen(aPassKey),(unsigned char *)digestKey,digestKeyLen,aNonce,aCreated,&stOnvifInputParam.stDevUtcTime);
		}
		g_lockDigest.UnLock();

		tPassword->__item				= (char *)digestKey;
		tPassword->Type					= gPasswordType;

		tUsernameToken->Nonce			 = aNonce;
		tUsernameToken->Password         = tPassword;
		tUsernameToken->Username		 = aUsername;
		tUsernameToken->wsu__Created	 = aCreated;
		tUsernameToken->wsu__Id		 = NULL; 

		tSecurity->BinarySecurityToken	 = NULL;
		tSecurity->ds__Signature		 = NULL;
		tSecurity->SOAP_ENV__actor		 = NULL;
		tSecurity->SOAP_ENV__role		 = NULL;
		tSecurity->UsernameToken		 = tUsernameToken;
		tSecurity->wsu__Timestamp		 = NULL;
		tSecurity->xenc__EncryptedKey	 = NULL;
		tSecurity->xenc__ReferenceList	 = NULL;

		m_pHeader->wsse__Security = tSecurity;
	}
}

void COnvifSoap::AddMessageId(void)
{
	char msgID[48];
	char *outID= NULL;
	int len = 48;


#ifdef WIN32
	GUID tmpuuid;
	if(m_pHeader == NULL)
	{
		return;
	}
	memset(msgID,0,sizeof(msgID));
	ZeroMemory(&tmpuuid,sizeof(GUID));
	if(CoCreateGuid(&tmpuuid)==S_OK)
	{
		sprintf(msgID,"urn:uuid:%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
			tmpuuid.Data1,
			tmpuuid.Data2,
			tmpuuid.Data3,
			tmpuuid.Data4[0], 
			tmpuuid.Data4[1], 
			tmpuuid.Data4[2], 
			tmpuuid.Data4[3], 
			tmpuuid.Data4[4], 
			tmpuuid.Data4[5], 
			tmpuuid.Data4[6], 
			tmpuuid.Data4[7]);

	}
#else
	uuid_t uuOut;
	unsigned char *pUUOut = reinterpret_cast<unsigned char *>(&uuOut);
	if(m_pHeader == NULL)
	{
		return;
	}
	memset(msgID,0,sizeof(msgID));

	uuid_generate(uuOut);
	sprintf(msgID,"urn:uuid:%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		pUUOut[0],
		pUUOut[1],
		pUUOut[2],
		pUUOut[3],
		pUUOut[4],
		pUUOut[5],
		pUUOut[6],
		pUUOut[7],
		pUUOut[8], 
		pUUOut[9], 
		pUUOut[10], 
		pUUOut[11], 
		pUUOut[12], 
		pUUOut[13], 
		pUUOut[14], 
		pUUOut[15]);

	//printf("****uuid=%s\n",out_uuid);
	//strcpy(out_uuid,"04b61002-11ed-4bdb-86f5-e3614cb64150");
#endif
	outID = (char *)malloc(len*sizeof(char));
	memset(outID,0,sizeof(char)*len);
	memcpy(outID,msgID,len*sizeof(char));
	m_pHeader->wsa5__MessageID  = outID;
	outID = NULL;
	return ;
}

void COnvifSoap::AddMessageId(bool bWsa)
{
	char msgID[48];
	char *outID= NULL;
	int len = 48;


#ifdef WIN32
	GUID tmpuuid;
	if(m_pHeader == NULL)
	{
		return;
	}
	memset(msgID,0,sizeof(msgID));
	ZeroMemory(&tmpuuid,sizeof(GUID));
	if(CoCreateGuid(&tmpuuid)==S_OK)
	{
		sprintf(msgID,"uuid:%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
			tmpuuid.Data1,
			tmpuuid.Data2,
			tmpuuid.Data3,
			tmpuuid.Data4[0], 
			tmpuuid.Data4[1], 
			tmpuuid.Data4[2], 
			tmpuuid.Data4[3], 
			tmpuuid.Data4[4], 
			tmpuuid.Data4[5], 
			tmpuuid.Data4[6], 
			tmpuuid.Data4[7]);

	}
#else
	uuid_t uuOut;
	unsigned char *pUUOut = reinterpret_cast<unsigned char *>(&uuOut);
	if(m_pHeader == NULL)
	{
		return;
	}
	memset(msgID,0,sizeof(msgID));

	uuid_generate(uuOut);
	sprintf(msgID,"uuid:%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		pUUOut[0],
		pUUOut[1],
		pUUOut[2],
		pUUOut[3],
		pUUOut[4],
		pUUOut[5],
		pUUOut[6],
		pUUOut[7],
		pUUOut[8], 
		pUUOut[9], 
		pUUOut[10], 
		pUUOut[11], 
		pUUOut[12], 
		pUUOut[13], 
		pUUOut[14], 
		pUUOut[15]);

	//printf("****uuid=%s\n",out_uuid);
	//strcpy(out_uuid,"04b61002-11ed-4bdb-86f5-e3614cb64150");
#endif

	if(bWsa)
	{
		outID = (char *)malloc(len*sizeof(char));
		memset(outID,0,sizeof(char)*len);
		memcpy(outID,msgID,len*sizeof(char));
		m_pHeader->wsa__MessageID  = outID;
	}
	outID = NULL;
	return ;
}

void COnvifSoap::AddAction(char *aAction)
{
	if(m_pHeader == NULL)
	{
		return;
	}
	m_pHeader->wsa5__Action = aAction;
	return;
}

void COnvifSoap::AddActionTo(char *aActionTo)
{
	if(m_pHeader == NULL)
	{
		return;
	}
	m_pHeader->wsa5__To = aActionTo;
	return;
}

void COnvifSoap::AddAction(char *aAction,bool bWsa)
{
	if(m_pHeader == NULL)
	{
		return;
	}
	if(bWsa)
	{
		m_pHeader->wsa__Action = aAction;
	}
	return;
}

void COnvifSoap::AddActionTo(char *aActionTo,bool bWsa)
{
	if(m_pHeader == NULL)
	{
		return;
	}
	if(bWsa)
	{
		m_pHeader->wsa__To = aActionTo;
	}
	return;
}

void COnvifSoap::AddUserIdPasswd(char *aUserName,char *aPasswd)
{
	if(m_pSoap == NULL)
	{
		return;
	}
	m_pSoap->userid = aUserName;
	m_pSoap->passwd = aPasswd;
}



