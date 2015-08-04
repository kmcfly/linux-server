/////////////////////////////////////////////////////////////////////////
#ifndef _SOAP_H_
#define _SOAP_H_
// soap 1.1  �ο�UPNP-arch-DeviceArchitecture-v1.1.pdf
/*
soap��Ϣ������UTF-8����
M��ʾSOAP��Ϣ�б�������ı�ǩ
O��ʾ�����ǩ�ǿ�ѡ��
P��ʾ�����ǩ����ֹ

	UPNP Request
<Envelope> element									M
encodingStyle attribute of <Envelope>				O	If present, must be "http://schemas.xmlsoap.org/soap/encoding/"
<Header> element (child element of <Envelope>)		O
actor attribute of <Header>							O
mustUnderstand attribute of <Header>				O	Only allowed if defined by the service to which it is directed
encodingStyle attribute of <Header>					O	If present, must be "http://schemas.xmlsoap.org/soap/encoding/"
<Body> element (child element of <Envelope>)		M	Exactly one <Body> child element allowed
encodingStyle attribute of <Body> element			P
root Attribute of <Body> child element				O	SHOULD NOT be used

<Envelope encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
	<Header actor="actor", encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
	</Header>
	<Body>
	......
	......
	</Body>
</Envelope>



	UPNP Response
<Envelope> element									M
encodingStyle attribute of <Envelope>				O	If present, must be "http://schemas.xmlsoap.org/soap/encoding/"
<Header> element (child element of <Envelope>)		O
actor attribute of <Header>							O
mustUnderstand attribute of <Header>				O	Only allowed if defined by the service to which it is directed
encodingStyle Attribute of <Header>					O	If present, must be "http://schemas.xmlsoap.org/soap/encoding/"
<Body> element (child element of <Envelope>)		M	Exactly one <Body> child element allowed
encodingStyle attribute of <Body> element			P
root attribute of <Body> child element				O	SHOULD NOT be used

<Envelope>
	<Header></Header>
	<Body></Body>
</Envelope>


	UPNP ERR Response
<Envelope> element									M
encodingStyle attribute of <Envelope>				O If present, must be "http://schemas.xmlsoap.org/soap/encoding/"
<Body> element (child element of <Envelope>)		M Exactly one <Body> child element allowed
containing exactly one <Fault> child element
<Fault> child element of <Body>						M
<faultcode> child element of <Fault>				M
<faultstring> child element of <Fault>				M
<detail> child element of <Fault>					M

<Envelope>
	<Body>
	......
	......
	</Body>
	<Fault>
		<faultcode></faultcode>
		<faultstring></faultstring>
		<detail></detail>
	</Fault>
</Envelope>

*/


#include "tinyxml.h"

#define GETEXTIP "GetExternalIPAddress"
class CSoap
{
public:
	CSoap();
	~CSoap();
	const char *GetSoapMsg(char *pSopMsg, unsigned long msgLen, char *pActonInfo);
	const char * GetExtIpMsg(char *pSopMsg, unsigned long msgLen);
protected:
private:
	void InitDoc();

	char *m_pActionInfo;
	TiXmlDocument	m_doc;
	TiXmlElement	*m_pBodyElement;
};
#endif /*_SOAP_H_*/
/////////////////////////////////////////////////////////////////////////
