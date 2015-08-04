#ifndef _OnvifSoapStub_H_
#define _OnvifSoapStub_H_

//device discovery
#define SOAP_WSA_200408
#define SOAP_NAMESPACE_OF_dn	"http://www.onvif.org/ver10/network/wsdl"
#define SOAP_NAMESPACE_OF_d	   "http://schemas.xmlsoap.org/ws/2005/04/discovery"

//device system management
#define SOAP_WSA_2005
#define SOAP_NAMESPACE_OF_tds	"http://www.onvif.org/ver10/device/wsdl"
#define SOAP_NAMESPACE_OF_tt	"http://www.onvif.org/ver10/schema"
#define SOAP_NAMESPACE_OF_wsnt	"http://docs.oasis-open.org/wsn/b-2"
#define SOAP_NAMESPACE_OF_xop	"http://www.w3.org/2004/08/xop/include"
#define SOAP_NAMESPACE_OF_ns1	"http://docs.oasis-open.org/wsrf/bf-2"
#define SOAP_NAMESPACE_OF_wstop	"http://docs.oasis-open.org/wsn/t-1"

//device configuration
#define SOAP_NAMESPACE_OF_trt	"http://www.onvif.org/ver10/media/wsdl"

//device imaging control

//device IO control

//device video

//device alarm


#include "stdsoap2.h"
#include "OnvifSoapH.h"

#ifdef __cplusplus
extern "C"
{
#endif 
/******************************************************************************\
*                                                                            *
* Enumerations                                                               *
*                                                                            *
\******************************************************************************/
//public  resource
#ifndef SOAP_TYPE_xsd__boolean_
#define SOAP_TYPE_xsd__boolean_ (41)
/* xsd:boolean */
enum xsd__boolean_ 
{
	_false = 0, 
	_true = 1
};
#endif

#ifndef SOAP_TYPE_tt__SearchState
#define SOAP_TYPE_tt__SearchState (109)
/* tt:SearchState */
enum tt__SearchState 
{
	tt__SearchState__Queued = 0, 
	tt__SearchState__Searching = 1, 
	tt__SearchState__Completed = 2, 
	tt__SearchState__Unknown = 3
};
#endif

#ifndef SOAP_TYPE_tt__NetworkHostType
#define SOAP_TYPE_tt__NetworkHostType (66)
/* tt:NetworkHostType */
enum tt__NetworkHostType 
{
	tt__NetworkHostType__IPv4 = 0, 
	tt__NetworkHostType__IPv6 = 1, 
	tt__NetworkHostType__DNS = 2
};
#endif

#ifndef SOAP_TYPE_tt__IPType
#define SOAP_TYPE_tt__IPType (70)
/* tt:IPType */
enum tt__IPType 
{
	tt__IPType__IPv4 = 0, 
	tt__IPType__IPv6 = 1
};
#endif

//soap resource
#ifndef SOAP_TYPE_wsa__RelationshipTypeValues
#define SOAP_TYPE_wsa__RelationshipTypeValues (12)
/* wsa:RelationshipTypeValues */
enum wsa__RelationshipTypeValues {wsa__Reply = 0};
typedef enum wsa__RelationshipTypeValues wsa__RelationshipTypeValues;
#endif

#ifndef SOAP_TYPE_wsa__FaultSubcodeValues
#define SOAP_TYPE_wsa__FaultSubcodeValues (13)
/* wsa:FaultSubcodeValues */
enum wsa__FaultSubcodeValues 
{
	wsa__InvalidMessageInformationHeader = 0, 
	wsa__MessageInformationHeaderRequired = 1, 
	wsa__DestinationUnreachable = 2, 
	wsa__ActionNotSupported = 3,
	wsa__EndpointUnavailable = 4
};
typedef enum wsa__FaultSubcodeValues wsa__FaultSubcodeValues;
#endif

#ifndef SOAP_TYPE_d__RelationshipType
#define SOAP_TYPE_d__RelationshipType (39)
/* d:RelationshipType */
enum d__RelationshipType 
{
	d__RelationshipType__d__Suppression = 0
};
#endif

#ifndef SOAP_TYPE_d__FaultCodeType
#define SOAP_TYPE_d__FaultCodeType (40)
/* d:FaultCodeType */
enum d__FaultCodeType 
{
	d__FaultCodeType__d__MatchingRuleNotSupported = 0
};
#endif

//ONVIF Device system Management
#ifndef SOAP_TYPE_tt__CapabilityCategory
#define SOAP_TYPE_tt__CapabilityCategory (83)
/* tt:CapabilityCategory */
enum tt__CapabilityCategory 
{
	tt__CapabilityCategory__All = 0, 
	tt__CapabilityCategory__Analytics = 1, 
	tt__CapabilityCategory__Device = 2, 
	tt__CapabilityCategory__Events = 3, 
	tt__CapabilityCategory__Imaging = 4, 
	tt__CapabilityCategory__Media = 5, 
	tt__CapabilityCategory__PTZ = 6
};
#endif

#ifndef SOAP_TYPE_tt__SetDateTimeType
#define SOAP_TYPE_tt__SetDateTimeType (86)
/* tt:SetDateTimeType */
enum tt__SetDateTimeType 
{
	tt__SetDateTimeType__Manual = 0, 
	tt__SetDateTimeType__NTP = 1
};
#endif

#ifndef SOAP_TYPE_tt__FactoryDefaultType
#define SOAP_TYPE_tt__FactoryDefaultType (85)
/* tt:FactoryDefaultType */
enum tt__FactoryDefaultType 
{
	tt__FactoryDefaultType__Hard = 0, 
	tt__FactoryDefaultType__Soft = 1
};
#endif

//device sercurity
#ifndef SOAP_TYPE_tt__UserLevel
#define SOAP_TYPE_tt__UserLevel (87)
/* tt:UserLevel */
enum tt__UserLevel 
{
	tt__UserLevel__Administrator = 0, 
	tt__UserLevel__Operator = 1, 
	tt__UserLevel__User = 2, 
	tt__UserLevel__Anonymous = 3, 
	tt__UserLevel__Extended = 4
};
#endif

//device network
#ifndef SOAP_TYPE_tt__IPv6DHCPConfiguration
#define SOAP_TYPE_tt__IPv6DHCPConfiguration (64)
/* tt:IPv6DHCPConfiguration */
enum tt__IPv6DHCPConfiguration 
{
	tt__IPv6DHCPConfiguration__Auto = 0,
	tt__IPv6DHCPConfiguration__Stateful = 1,
	tt__IPv6DHCPConfiguration__Stateless = 2, 
	tt__IPv6DHCPConfiguration__Off = 3
};
#endif

#ifndef SOAP_TYPE_tt__Duplex
#define SOAP_TYPE_tt__Duplex (62)
/* tt:Duplex */
enum tt__Duplex 
{
	tt__Duplex__Full = 0, 
	tt__Duplex__Half = 1
};
#endif

#ifndef SOAP_TYPE_tt__Dot11StationMode
#define SOAP_TYPE_tt__Dot11StationMode (76)
/* tt:Dot11StationMode */
enum tt__Dot11StationMode 
{
	tt__Dot11StationMode__Ad_hoc = 0, 
	tt__Dot11StationMode__Infrastructure = 1, 
	tt__Dot11StationMode__Extended = 2
};
#endif

#ifndef SOAP_TYPE_tt__Dot11SecurityMode
#define SOAP_TYPE_tt__Dot11SecurityMode (77)
/* tt:Dot11SecurityMode */
enum tt__Dot11SecurityMode 
{
	tt__Dot11SecurityMode__None = 0, 
	tt__Dot11SecurityMode__WEP = 1, 
	tt__Dot11SecurityMode__PSK = 2, 
	tt__Dot11SecurityMode__Dot1X = 3, 
	tt__Dot11SecurityMode__Extended = 4
};
#endif

#ifndef SOAP_TYPE_tt__Dot11Cipher
#define SOAP_TYPE_tt__Dot11Cipher (78)
/* tt:Dot11Cipher */
enum tt__Dot11Cipher 
{
	tt__Dot11Cipher__CCMP = 0, 
	tt__Dot11Cipher__TKIP = 1, 
	tt__Dot11Cipher__Any = 2, 
	tt__Dot11Cipher__Extended = 3
};
#endif

#ifndef SOAP_TYPE_tt__NetworkProtocolType
#define SOAP_TYPE_tt__NetworkProtocolType (65)
/* tt:NetworkProtocolType */
enum tt__NetworkProtocolType 
{
	tt__NetworkProtocolType__HTTP = 0, 
	tt__NetworkProtocolType__HTTPS = 1, 
	tt__NetworkProtocolType__RTSP = 2
};
#endif

#ifndef SOAP_TYPE_tt__DynamicDNSType
#define SOAP_TYPE_tt__DynamicDNSType (74)
/* tt:DynamicDNSType */
enum tt__DynamicDNSType 
{
	tt__DynamicDNSType__NoUpdate = 0, 
	tt__DynamicDNSType__ClientUpdates = 1, 
	tt__DynamicDNSType__ServerUpdates = 2
};
#endif

#ifndef SOAP_TYPE_tt__IPAddressFilterType
#define SOAP_TYPE_tt__IPAddressFilterType (73)
/* tt:IPAddressFilterType */
enum tt__IPAddressFilterType 
{
	tt__IPAddressFilterType__Allow = 0, 
	tt__IPAddressFilterType__Deny = 1
};
#endif

//device configuration
#ifndef SOAP_TYPE_tt__VideoEncoding
#define SOAP_TYPE_tt__VideoEncoding (52)
/* tt:VideoEncoding */
enum tt__VideoEncoding 
{
	tt__VideoEncoding__JPEG = 0, 
	tt__VideoEncoding__MPEG4 = 1, 
	tt__VideoEncoding__H264 = 2
};
#endif

#ifndef SOAP_TYPE_tt__Mpeg4Profile
#define SOAP_TYPE_tt__Mpeg4Profile (53)
/* tt:Mpeg4Profile */
enum tt__Mpeg4Profile 
{
	tt__Mpeg4Profile__SP = 0, 
	tt__Mpeg4Profile__ASP = 1
};
#endif

#ifndef SOAP_TYPE_tt__H264Profile
#define SOAP_TYPE_tt__H264Profile (54)
/* tt:H264Profile */
enum tt__H264Profile 
{
	tt__H264Profile__Baseline = 0, 
	tt__H264Profile__Main = 1, 
	tt__H264Profile__Extended = 2, 
	tt__H264Profile__High = 3
};
#endif

#ifndef SOAP_TYPE_tt__AudioEncoding
#define SOAP_TYPE_tt__AudioEncoding (55)
/* tt:AudioEncoding */
enum tt__AudioEncoding 
{
	tt__AudioEncoding__G711 = 0, 
	tt__AudioEncoding__G726 = 1, 
	tt__AudioEncoding__AAC = 2
};
#endif

//device video
#ifndef SOAP_TYPE_tt__StreamType
#define SOAP_TYPE_tt__StreamType (56)
/* tt:StreamType */
enum tt__StreamType 
{
	tt__StreamType__RTP_Unicast = 0, 
	tt__StreamType__RTP_Multicast = 1
};
#endif

#ifndef SOAP_TYPE_tt__TransportProtocol
#define SOAP_TYPE_tt__TransportProtocol (57)
/* tt:TransportProtocol */
enum tt__TransportProtocol 
{
	tt__TransportProtocol__UDP = 0, 
	tt__TransportProtocol__TCP = 1, 
	tt__TransportProtocol__RTSP = 2, 
	tt__TransportProtocol__HTTP = 3
};
#endif

//device imageing control
#ifndef SOAP_TYPE_tt__BacklightCompensationMode
#define SOAP_TYPE_tt__BacklightCompensationMode (94)
/* tt:BacklightCompensationMode */
enum tt__BacklightCompensationMode 
{
	tt__BacklightCompensationMode__OFF = 0, 
	tt__BacklightCompensationMode__ON = 1
};
#endif

#ifndef SOAP_TYPE_tt__ExposurePriority
#define SOAP_TYPE_tt__ExposurePriority (95)
/* tt:ExposurePriority */
enum tt__ExposurePriority 
{
	tt__ExposurePriority__LowNoise = 0, 
	tt__ExposurePriority__FrameRate = 1
};
#endif

#ifndef SOAP_TYPE_tt__ExposureMode
#define SOAP_TYPE_tt__ExposureMode (96)
/* tt:ExposureMode */
enum tt__ExposureMode 
{
	tt__ExposureMode__AUTO = 0, 
	tt__ExposureMode__MANUAL = 1
};
#endif

#ifndef SOAP_TYPE_tt__AutoFocusMode
#define SOAP_TYPE_tt__AutoFocusMode (92)
/* tt:AutoFocusMode */
enum tt__AutoFocusMode 
{
	tt__AutoFocusMode__AUTO = 0, 
	tt__AutoFocusMode__MANUAL = 1
};
#endif

#ifndef SOAP_TYPE_tt__WideDynamicMode
#define SOAP_TYPE_tt__WideDynamicMode (93)
/* tt:WideDynamicMode */
enum tt__WideDynamicMode 
{
	tt__WideDynamicMode__OFF = 0, 
	tt__WideDynamicMode__ON = 1
};
#endif

#ifndef SOAP_TYPE_tt__WhiteBalanceMode
#define SOAP_TYPE_tt__WhiteBalanceMode (98)
/* tt:WhiteBalanceMode */
enum tt__WhiteBalanceMode 
{
	tt__WhiteBalanceMode__AUTO = 0, 
	tt__WhiteBalanceMode__MANUAL = 1
};
#endif

#ifndef SOAP_TYPE_tt__IrCutFilterMode
#define SOAP_TYPE_tt__IrCutFilterMode (99)
/* tt:IrCutFilterMode */
enum tt__IrCutFilterMode 
{
	tt__IrCutFilterMode__ON = 0, 
	tt__IrCutFilterMode__OFF = 1, 
	tt__IrCutFilterMode__AUTO = 2
};
#endif

//device IO control
#ifndef SOAP_TYPE_tt__RelayLogicalState
#define SOAP_TYPE_tt__RelayLogicalState (88)
/* tt:RelayLogicalState */
enum tt__RelayLogicalState 
{
	tt__RelayLogicalState__active = 0, 
	tt__RelayLogicalState__inactive = 1
};
#endif

#ifndef SOAP_TYPE_tt__RelayIdleState
#define SOAP_TYPE_tt__RelayIdleState (89)
/* tt:RelayIdleState */
enum tt__RelayIdleState 
{
	tt__RelayIdleState__closed = 0, 
	tt__RelayIdleState__open = 1
};
#endif

#ifndef SOAP_TYPE_tt__RelayMode
#define SOAP_TYPE_tt__RelayMode (90)
/* tt:RelayMode */
enum tt__RelayMode 
{
	tt__RelayMode__Monostable = 0, 
	tt__RelayMode__Bistable = 1
};
#endif

/******************************************************************************\
*                                                                            *
* Types with Custom Serializers                                              *
*                                                                            *
\******************************************************************************/


/******************************************************************************\
*                                                                            *
* Classes and Structs                                                        *
*                                                                            *
\******************************************************************************/
//public resource
#ifndef SOAP_TYPE_wsa5__MetadataType
#define SOAP_TYPE_wsa5__MetadataType (9)
/* wsa5:MetadataType */
struct wsa5__MetadataType
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
typedef struct wsa5__MetadataType wsa5__MetadataType;
#endif

#ifndef SOAP_TYPE_xsd__hexBinary
#define SOAP_TYPE_xsd__hexBinary (42)
/* hexBinary schema type: */
struct xsd__hexBinary
{
	unsigned char *__ptr;
	int __size;
};
#endif

#ifndef SOAP_TYPE_tt__IPAddress
#define SOAP_TYPE_tt__IPAddress (241)
/* tt:IPAddress */
struct tt__IPAddress
{
	enum tt__IPType Type;	/* required element of type tt:IPType */
	char **IPv4Address;	/* optional element of type tt:IPv4Address */
	char **IPv6Address;	/* optional element of type tt:IPv6Address */
};
#endif

#ifndef SOAP_TYPE_tt__NetworkHostExtension
#define SOAP_TYPE_tt__NetworkHostExtension (518)
/* tt:NetworkHostExtension */
struct tt__NetworkHostExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif


#ifndef SOAP_TYPE_tt__NetworkHost
#define SOAP_TYPE_tt__NetworkHost (198)
/* tt:NetworkHost */
struct tt__NetworkHost
{
	enum tt__NetworkHostType Type;	/* required element of type tt:NetworkHostType */
	char **IPv4Address;	/* optional element of type tt:IPv4Address */
	char **IPv6Address;	/* optional element of type tt:IPv6Address */
	char **DNSname;	/* optional element of type tt:DNSName */
	struct tt__NetworkHostExtension *Extension;	/* optional element of type tt:NetworkHostExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__IntRange
#define SOAP_TYPE_tt__IntRange (317)
/* tt:IntRange */
struct tt__IntRange
{
	int Min;	/* required element of type xsd:int */
	int Max;	/* required element of type xsd:int */
};
#endif

#ifndef SOAP_TYPE_tt__FloatRange
#define SOAP_TYPE_tt__FloatRange (319)
/* tt:FloatRange */
struct tt__FloatRange
{
	float Min;	/* required element of type xsd:float */
	float Max;	/* required element of type xsd:float */
};
#endif

#ifndef SOAP_TYPE__tt__union_EventStream
#define SOAP_TYPE__tt__union_EventStream (899)
/* xsd:choice */
union _tt__union_EventStream
{
#define SOAP_UNION__tt__union_EventStream_wsnt__NotificationMessage	(1)
	struct wsnt__NotificationMessageHolderType *wsnt__NotificationMessage;
#define SOAP_UNION__tt__union_EventStream_Extension	(2)
	struct tt__EventStreamExtension *Extension;
};
#endif

#ifndef SOAP_TYPE___tt__union_EventStream
#define SOAP_TYPE___tt__union_EventStream (894)
/* Operation wrapper: */
struct __tt__union_EventStream
{
	int __union_EventStream;	/* union discriminant (of union defined below) */
	union _tt__union_EventStream union_EventStream;	/* required element of type xsd:choice */
};
#endif

#ifndef SOAP_TYPE_tt__EventStream
#define SOAP_TYPE_tt__EventStream (877)
/* tt:EventStream */
struct tt__EventStream
{
	int __size_EventStream;	/* sequence of elements <-union-EventStream> */
	struct __tt__union_EventStream *__union_EventStream;
};
#endif

#ifndef SOAP_TYPE_tt__EventStreamExtension
#define SOAP_TYPE_tt__EventStreamExtension (897)
/* tt:EventStreamExtension */
struct tt__EventStreamExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE__tt__ItemList_SimpleItem
#define SOAP_TYPE__tt__ItemList_SimpleItem (775)
/* tt:ItemList-SimpleItem */
struct _tt__ItemList_SimpleItem
{
	char *Name;	/* required attribute of type xsd:string */
	char *Value;	/* required attribute of type xsd:anySimpleType */
};
#endif

#ifndef SOAP_TYPE__tt__ItemList_ElementItem
#define SOAP_TYPE__tt__ItemList_ElementItem (777)
/* tt:ItemList-ElementItem */
struct _tt__ItemList_ElementItem
{
	char *__any;
	char *Name;	/* required attribute of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_tt__ItemList
#define SOAP_TYPE_tt__ItemList (774)
/* tt:ItemList */
struct tt__ItemList
{
	int __sizeSimpleItem;	/* sequence of elements <SimpleItem> */
	struct _tt__ItemList_SimpleItem *SimpleItem;	/* optional element of type tt:ItemList-SimpleItem */
	int __sizeElementItem;	/* sequence of elements <ElementItem> */
	struct _tt__ItemList_ElementItem *ElementItem;	/* optional element of type tt:ItemList-ElementItem */
	struct tt__ItemListExtension *Extension;	/* optional element of type tt:ItemListExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__ItemListExtension
#define SOAP_TYPE_tt__ItemListExtension (779)
/* tt:ItemListExtension */
struct tt__ItemListExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

//soap resource
#ifndef SOAP_TYPE_wsa__ServiceNameType
#define SOAP_TYPE_wsa__ServiceNameType (10)
/* Primitive wsa:ServiceNameType schema type: */
struct wsa__ServiceNameType
{
	char *__item;
	char *PortName;	/* optional attribute of type xsd:string */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
typedef struct wsa__ServiceNameType wsa__ServiceNameType;
#endif

#ifndef SOAP_TYPE_wsa__Relationship
#define SOAP_TYPE_wsa__Relationship (11)
/* Primitive wsa:Relationship schema type: */
struct wsa__Relationship
{
	char *__item;
	char *RelationshipType;	/* optional attribute of type xsd:QName */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
typedef struct wsa__Relationship wsa__Relationship;
#endif

#ifndef SOAP_TYPE__wsse__Password
#define SOAP_TYPE__wsse__Password (69)
/* Primitive wsse:Password schema type: */
struct _wsse__Password
{
	char *__item;
	char *Type;	/* optional attribute of type xsd:string */
};
typedef struct _wsse__Password _wsse__Password;
#endif

#ifndef SOAP_TYPE__wsu__Timestamp
#define SOAP_TYPE__wsu__Timestamp (66)
/* wsu:Timestamp */
struct _wsu__Timestamp
{
	char *wsu__Id;	/* optional attribute of type xsd:string */
	char *Created;	/* optional element of type xsd:string */
	char *Expires;	/* optional element of type xsd:string */
};
typedef struct _wsu__Timestamp _wsu__Timestamp;
#endif

#ifndef SOAP_TYPE__wsse__UsernameToken
#define SOAP_TYPE__wsse__UsernameToken (68)
/* wsse:UsernameToken */
struct _wsse__UsernameToken
{
	char *Username;	/* optional element of type xsd:string */
	struct _wsse__Password *Password;	/* optional element of type wsse:Password */
	char *Nonce;	/* optional element of type xsd:string */
	char *wsu__Created;	/* optional element of type xsd:string */
	char *wsu__Id;	/* optional attribute of type xsd:string */
};
typedef struct _wsse__UsernameToken _wsse__UsernameToken;
#endif

#ifndef SOAP_TYPE__wsse__BinarySecurityToken
#define SOAP_TYPE__wsse__BinarySecurityToken (71)
/* Primitive wsse:BinarySecurityToken schema type: */
struct _wsse__BinarySecurityToken
{
	char *__item;
	char *wsu__Id;	/* optional attribute of type xsd:string */
	char *ValueType;	/* optional attribute of type xsd:string */
	char *EncodingType;	/* optional attribute of type xsd:string */
};
typedef struct _wsse__BinarySecurityToken _wsse__BinarySecurityToken;
#endif

#ifndef SOAP_TYPE_xenc__EncryptionMethodType
#define SOAP_TYPE_xenc__EncryptionMethodType (117)
/* xenc:EncryptionMethodType */
struct xenc__EncryptionMethodType
{
	int *KeySize;	/* optional element of type xsd:int */
	char *OAEPparams;	/* optional element of type xsd:string */
	char *Algorithm;	/* required attribute of type xsd:string */
	char *__mixed;
};
typedef struct xenc__EncryptionMethodType xenc__EncryptionMethodType;
#endif

#ifndef SOAP_TYPE_ds__DSAKeyValueType
#define SOAP_TYPE_ds__DSAKeyValueType (103)
/* ds:DSAKeyValueType */
struct ds__DSAKeyValueType
{
	char *G;	/* optional element of type xsd:string */
	char *Y;	/* required element of type xsd:string */
	char *J;	/* optional element of type xsd:string */
	char *P;	/* required element of type xsd:string */
	char *Q;	/* required element of type xsd:string */
	char *Seed;	/* required element of type xsd:string */
	char *PgenCounter;	/* required element of type xsd:string */
};
typedef struct ds__DSAKeyValueType ds__DSAKeyValueType;
#endif

#ifndef SOAP_TYPE_ds__RSAKeyValueType
#define SOAP_TYPE_ds__RSAKeyValueType (104)
/* ds:RSAKeyValueType */
struct ds__RSAKeyValueType
{
	char *Modulus;	/* required element of type xsd:string */
	char *Exponent;	/* required element of type xsd:string */
};
typedef struct ds__RSAKeyValueType ds__RSAKeyValueType;
#endif

#ifndef SOAP_TYPE_ds__KeyValueType
#define SOAP_TYPE_ds__KeyValueType (94)
/* ds:KeyValueType */
struct ds__KeyValueType
{
	struct ds__DSAKeyValueType *DSAKeyValue;	/* required element of type ds:DSAKeyValueType */
	struct ds__RSAKeyValueType *RSAKeyValue;	/* required element of type ds:RSAKeyValueType */
};
typedef struct ds__KeyValueType ds__KeyValueType;
#endif

#ifndef SOAP_TYPE__c14n__InclusiveNamespaces
#define SOAP_TYPE__c14n__InclusiveNamespaces (89)
/* c14n:InclusiveNamespaces */
struct _c14n__InclusiveNamespaces
{
	char *PrefixList;	/* optional attribute of type xsd:string */
};
typedef struct _c14n__InclusiveNamespaces _c14n__InclusiveNamespaces;
#endif

#ifndef SOAP_TYPE_ds__TransformType
#define SOAP_TYPE_ds__TransformType (90)
/* ds:TransformType */
struct ds__TransformType
{
	struct _c14n__InclusiveNamespaces *c14n__InclusiveNamespaces;	/* optional element of type c14n:InclusiveNamespaces */
	char *__any;
	char *Algorithm;	/* optional attribute of type xsd:string */
};
typedef struct ds__TransformType ds__TransformType;
#endif
#ifndef SOAP_TYPE_ds__TransformsType
#define SOAP_TYPE_ds__TransformsType (88)
/* ds:TransformsType */
struct ds__TransformsType
{
	int __sizeTransform;	/* sequence of elements <Transform> */
	struct ds__TransformType *Transform;	/* required element of type ds:TransformType */
};
typedef struct ds__TransformsType ds__TransformsType;
#endif

#ifndef SOAP_TYPE_ds__RetrievalMethodType
#define SOAP_TYPE_ds__RetrievalMethodType (96)
/* ds:RetrievalMethodType */
struct ds__RetrievalMethodType
{
	struct ds__TransformsType *Transforms;	/* optional element of type ds:TransformsType */
	char *URI;	/* optional attribute of type xsd:string */
	char *Type;	/* optional attribute of type xsd:string */
};
typedef struct ds__RetrievalMethodType ds__RetrievalMethodType;
#endif

#ifndef SOAP_TYPE_ds__X509IssuerSerialType
#define SOAP_TYPE_ds__X509IssuerSerialType (102)
/* ds:X509IssuerSerialType */
struct ds__X509IssuerSerialType
{
	char *X509IssuerName;	/* required element of type xsd:string */
	int X509SerialNumber;	/* required element of type xsd:int */
};
typedef struct ds__X509IssuerSerialType ds__X509IssuerSerialType;
#endif

#ifndef SOAP_TYPE_ds__X509DataType
#define SOAP_TYPE_ds__X509DataType (98)
/* ds:X509DataType */
struct ds__X509DataType
{
	struct ds__X509IssuerSerialType *X509IssuerSerial;	/* required element of type ds:X509IssuerSerialType */
	char *X509SKI;	/* required element of type xsd:string */
	char *X509SubjectName;	/* required element of type xsd:string */
	char *X509Certificate;	/* required element of type xsd:string */
	char *X509CRL;	/* required element of type xsd:string */
};
typedef struct ds__X509DataType ds__X509DataType;
#endif

#ifndef SOAP_TYPE__wsse__Reference
#define SOAP_TYPE__wsse__Reference (72)
/* wsse:Reference */
struct _wsse__Reference
{
	char *URI;	/* optional attribute of type xsd:string */
	char *ValueType;	/* optional attribute of type xsd:string */
};
typedef struct _wsse__Reference _wsse__Reference;
#endif

#ifndef SOAP_TYPE__wsse__KeyIdentifier
#define SOAP_TYPE__wsse__KeyIdentifier (74)
/* Primitive wsse:KeyIdentifier schema type: */
struct _wsse__KeyIdentifier
{
	char *__item;
	char *wsu__Id;	/* optional attribute of type xsd:string */
	char *ValueType;	/* optional attribute of type xsd:string */
	char *EncodingType;	/* optional attribute of type xsd:string */
};
typedef struct _wsse__KeyIdentifier _wsse__KeyIdentifier;
#endif

#ifndef SOAP_TYPE__wsse__Embedded
#define SOAP_TYPE__wsse__Embedded (73)
/* wsse:Embedded */
struct _wsse__Embedded
{
	char *wsu__Id;	/* optional attribute of type xsd:string */
	char *ValueType;	/* optional attribute of type xsd:string */
};
typedef struct _wsse__Embedded _wsse__Embedded;
#endif


#ifndef SOAP_TYPE__wsse__SecurityTokenReference
#define SOAP_TYPE__wsse__SecurityTokenReference (75)
/* wsse:SecurityTokenReference */
struct _wsse__SecurityTokenReference
{
	struct _wsse__Reference *Reference;	/* optional element of type wsse:Reference */
	struct _wsse__KeyIdentifier *KeyIdentifier;	/* optional element of type wsse:KeyIdentifier */
	struct _wsse__Embedded *Embedded;	/* optional element of type wsse:Embedded */
	char *wsu__Id;	/* optional attribute of type xsd:string */
	char *Usage;	/* optional attribute of type xsd:string */
};
typedef struct _wsse__SecurityTokenReference _wsse__SecurityTokenReference;
#endif

#ifndef SOAP_TYPE_ds__KeyInfoType
#define SOAP_TYPE_ds__KeyInfoType (82)
/* ds:KeyInfoType */
struct ds__KeyInfoType
{
	char *KeyName;	/* optional element of type xsd:string */
	struct ds__KeyValueType *KeyValue;	/* optional element of type ds:KeyValueType */
	struct ds__RetrievalMethodType *RetrievalMethod;	/* optional element of type ds:RetrievalMethodType */
	struct ds__X509DataType *X509Data;	/* optional element of type ds:X509DataType */
	struct _wsse__SecurityTokenReference *wsse__SecurityTokenReference;	/* optional element of type wsse:SecurityTokenReference */
	char *Id;	/* optional attribute of type xsd:string */
};
typedef struct ds__KeyInfoType ds__KeyInfoType;
#endif

#ifndef SOAP_TYPE_xenc__TransformsType
#define SOAP_TYPE_xenc__TransformsType (120)
/* xenc:TransformsType */
struct xenc__TransformsType
{
	struct ds__TransformType ds__Transform;	/* required element of type ds:Transform */
};
typedef struct xenc__TransformsType xenc__TransformsType;
#endif

#ifndef SOAP_TYPE_xenc__CipherReferenceType
#define SOAP_TYPE_xenc__CipherReferenceType (119)
/* xenc:CipherReferenceType */
struct xenc__CipherReferenceType
{
	struct xenc__TransformsType *Transforms;	/* optional element of type xenc:TransformsType */
	char *URI;	/* required attribute of type xsd:string */
};
typedef struct xenc__CipherReferenceType xenc__CipherReferenceType;
#endif

#ifndef SOAP_TYPE_xenc__CipherDataType
#define SOAP_TYPE_xenc__CipherDataType (118)
/* xenc:CipherDataType */
struct xenc__CipherDataType
{
	char *CipherValue;	/* optional element of type xsd:string */
	struct xenc__CipherReferenceType *CipherReference;	/* optional element of type xenc:CipherReferenceType */
};
typedef struct xenc__CipherDataType xenc__CipherDataType;
#endif

#ifndef SOAP_TYPE_xenc__EncryptionPropertyType
#define SOAP_TYPE_xenc__EncryptionPropertyType (126)
/* xenc:EncryptionPropertyType */
struct xenc__EncryptionPropertyType
{
	char *Target;	/* optional attribute of type xsd:string */
	char *Id;	/* optional attribute of type xsd:string */
};
typedef struct xenc__EncryptionPropertyType xenc__EncryptionPropertyType;
#endif

#ifndef SOAP_TYPE_xenc__EncryptionPropertiesType
#define SOAP_TYPE_xenc__EncryptionPropertiesType (125)
/* xenc:EncryptionPropertiesType */
struct xenc__EncryptionPropertiesType
{
	int __sizeEncryptionProperty;	/* sequence of elements <EncryptionProperty> */
	struct xenc__EncryptionPropertyType *EncryptionProperty;	/* required element of type xenc:EncryptionPropertyType */
	char *Id;	/* optional attribute of type xsd:string */
};
typedef struct xenc__EncryptionPropertiesType xenc__EncryptionPropertiesType;
#endif


#ifndef SOAP_TYPE_xenc__EncryptedKeyType
#define SOAP_TYPE_xenc__EncryptedKeyType (122)
/* xenc:EncryptedKeyType */
struct xenc__EncryptedKeyType
{
	struct xenc__EncryptionMethodType *EncryptionMethod;	/* optional element of type xenc:EncryptionMethodType */
	struct ds__KeyInfoType *ds__KeyInfo;	/* optional element of type ds:KeyInfo */
	struct xenc__CipherDataType *CipherData;	/* required element of type xenc:CipherDataType */
	struct xenc__EncryptionPropertiesType *EncryptionProperties;	/* optional element of type xenc:EncryptionPropertiesType */
	char *Id;	/* optional attribute of type xsd:string */
	char *Type;	/* optional attribute of type xsd:string */
	char *MimeType;	/* optional attribute of type xsd:string */
	char *Encoding;	/* optional attribute of type xsd:string */
	struct _xenc__ReferenceList *ReferenceList;	/* optional element of type xenc:ReferenceList */
	char *CarriedKeyName;	/* optional element of type xsd:string */
	char *Recipient;	/* optional attribute of type xsd:string */
};
typedef struct xenc__EncryptedKeyType xenc__EncryptedKeyType;
#endif

#ifndef SOAP_TYPE_xenc__ReferenceType
#define SOAP_TYPE_xenc__ReferenceType (124)
/* xenc:ReferenceType */
struct xenc__ReferenceType
{
	char *URI;	/* required attribute of type xsd:string */
};
typedef struct xenc__ReferenceType xenc__ReferenceType;
#endif


#ifndef SOAP_TYPE___xenc__union_ReferenceList
#define SOAP_TYPE___xenc__union_ReferenceList (135)
/* Operation wrapper: */
struct __xenc__union_ReferenceList
{
	struct xenc__ReferenceType *DataReference;	/* optional element of type xenc:ReferenceType */
	struct xenc__ReferenceType *KeyReference;	/* optional element of type xenc:ReferenceType */
};
#endif


#ifndef SOAP_TYPE__xenc__ReferenceList
#define SOAP_TYPE__xenc__ReferenceList (127)
/* xenc:ReferenceList */
struct _xenc__ReferenceList
{
	int __size_ReferenceList;	/* sequence of elements <-union-ReferenceList> */
	struct __xenc__union_ReferenceList *__union_ReferenceList;
};
typedef struct _xenc__ReferenceList _xenc__ReferenceList;
#endif

#ifndef SOAP_TYPE_ds__CanonicalizationMethodType
#define SOAP_TYPE_ds__CanonicalizationMethodType (85)
/* ds:CanonicalizationMethodType */
struct ds__CanonicalizationMethodType
{
	char *Algorithm;	/* required attribute of type xsd:string */
	struct _c14n__InclusiveNamespaces *c14n__InclusiveNamespaces;	/* optional element of type c14n:InclusiveNamespaces */
};
typedef struct ds__CanonicalizationMethodType ds__CanonicalizationMethodType;
#endif

#ifndef SOAP_TYPE_ds__SignatureMethodType
#define SOAP_TYPE_ds__SignatureMethodType (86)
/* ds:SignatureMethodType */
struct ds__SignatureMethodType
{
	int *HMACOutputLength;	/* optional element of type xsd:int */
	char *Algorithm;	/* required attribute of type xsd:string */
};
typedef struct ds__SignatureMethodType ds__SignatureMethodType;
#endif

#ifndef SOAP_TYPE_ds__DigestMethodType
#define SOAP_TYPE_ds__DigestMethodType (93)
/* ds:DigestMethodType */
struct ds__DigestMethodType
{
	char *Algorithm;	/* required attribute of type xsd:string */
};
typedef struct ds__DigestMethodType ds__DigestMethodType;
#endif

#ifndef SOAP_TYPE_ds__ReferenceType
#define SOAP_TYPE_ds__ReferenceType (87)
/* ds:ReferenceType */
struct ds__ReferenceType
{
	struct ds__TransformsType *Transforms;	/* optional element of type ds:TransformsType */
	struct ds__DigestMethodType *DigestMethod;	/* required element of type ds:DigestMethodType */
	char *DigestValue;	/* required element of type xsd:string */
	char *Id;	/* optional attribute of type xsd:string */
	char *URI;	/* optional attribute of type xsd:string */
	char *Type;	/* optional attribute of type xsd:string */
};
typedef struct ds__ReferenceType ds__ReferenceType;
#endif

#ifndef SOAP_TYPE_ds__SignedInfoType
#define SOAP_TYPE_ds__SignedInfoType (80)
/* ds:SignedInfoType */
struct ds__SignedInfoType
{
	struct ds__CanonicalizationMethodType *CanonicalizationMethod;	/* required element of type ds:CanonicalizationMethodType */
	struct ds__SignatureMethodType *SignatureMethod;	/* required element of type ds:SignatureMethodType */
	int __sizeReference;	/* sequence of elements <Reference> */
	struct ds__ReferenceType **Reference;	/* required element of type ds:ReferenceType */
	char *Id;	/* optional attribute of type xsd:string */
};
typedef struct ds__SignedInfoType ds__SignedInfoType;
#endif

#ifndef SOAP_TYPE_ds__SignatureType
#define SOAP_TYPE_ds__SignatureType (79)
/* ds:SignatureType */
struct ds__SignatureType
{
	struct ds__SignedInfoType *SignedInfo;	/* optional element of type ds:SignedInfoType */
	char *SignatureValue;	/* optional element of type xsd:string */
	struct ds__KeyInfoType *KeyInfo;	/* optional element of type ds:KeyInfoType */
	char *Id;	/* optional attribute of type xsd:string */
};
typedef struct ds__SignatureType ds__SignatureType;
#endif

#ifndef SOAP_TYPE__wsse__Security
#define SOAP_TYPE__wsse__Security (139)
/* wsse:Security */
struct _wsse__Security
{
	struct _wsu__Timestamp *wsu__Timestamp;	/* optional element of type wsu:Timestamp */
	struct _wsse__UsernameToken *UsernameToken;	/* optional element of type wsse:UsernameToken */
	struct _wsse__BinarySecurityToken *BinarySecurityToken;	/* optional element of type wsse:BinarySecurityToken */
	struct xenc__EncryptedKeyType *xenc__EncryptedKey;	/* optional element of type xenc:EncryptedKeyType */
	struct _xenc__ReferenceList *xenc__ReferenceList;	/* optional element of type xenc:ReferenceList */
	struct ds__SignatureType *ds__Signature;	/* optional element of type ds:SignatureType */
	char *SOAP_ENV__actor;	/* optional attribute of type xsd:string */
	char *SOAP_ENV__role;	/* optional attribute of type xsd:string */
};
typedef struct _wsse__Security _wsse__Security;
#endif

#ifndef SOAP_TYPE_wsa5__RelatesToType
#define SOAP_TYPE_wsa5__RelatesToType (37)
/* Primitive wsa5:RelatesToType schema type: */
struct wsa5__RelatesToType
{
	char *__item;
	char *RelationshipType;	/* optional attribute of type wsa5:RelationshipTypeOpenEnum */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
typedef struct wsa5__RelatesToType wsa5__RelatesToType;
#endif

#ifndef SOAP_TYPE_wsa5__EndpointReferenceType
#define SOAP_TYPE_wsa5__EndpointReferenceType (34)
/* wsa5:EndpointReferenceType */
struct wsa5__EndpointReferenceType
{
	char *Address;	/* required element of type xsd:string */
	struct wsa5__ReferenceParametersType *ReferenceParameters;	/* optional element of type wsa5:ReferenceParametersType */
	struct wsa5__MetadataType *Metadata;	/* optional element of type wsa5:MetadataType */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
typedef struct wsa5__EndpointReferenceType wsa5__EndpointReferenceType;
#endif

#ifndef SOAP_TYPE_wsa5__ReferenceParametersType
#define SOAP_TYPE_wsa5__ReferenceParametersType (35)
/* wsa5:ReferenceParametersType */
struct wsa5__ReferenceParametersType
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
typedef struct wsa5__ReferenceParametersType wsa5__ReferenceParametersType;
#endif

#if 0//ndef SOAP_TYPE_wsa5__MetadataType
#define SOAP_TYPE_wsa5__MetadataType (36)
/* wsa5:MetadataType */
struct wsa5__MetadataType
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
typedef struct wsa5__MetadataType wsa5__MetadataType;
#endif

#ifndef WITH_NOGLOBAL
#ifndef SOAP_TYPE_SOAP_ENV__Header
#define SOAP_TYPE_SOAP_ENV__Header (29)
/* SOAP Header: */
struct SOAP_ENV__Header
{
	char *wsa__MessageID;	/* optional element of type wsa:MessageID */
	struct wsa__Relationship *wsa__RelatesTo;	/* optional element of type wsa:RelatesTo */
	struct wsa__EndpointReferenceType *wsa__From;	/* optional element of type wsa:From */
	struct wsa__EndpointReferenceType *wsa__ReplyTo;	/* mustUnderstand */
	struct wsa__EndpointReferenceType *wsa__FaultTo;	/* mustUnderstand */
	char *wsa__To;	/* mustUnderstand */
	char *wsa__Action;	/* mustUnderstand */
    
	//not discovery
	enum xsd__boolean_ bDiscover; 
	//not encyption
    enum xsd__boolean_ bDigest; //if the bDigest is _false,the username and password is not encyption
	char  *username;
	char  *password;
	char  *ns1_customerId;
    
	//encyption
	char *wsa5__MessageID;	/* optional element of type wsa5:MessageID */
	struct wsa5__RelatesToType *wsa5__RelatesTo;	/* optional element of type wsa5:RelatesTo */
	struct wsa5__EndpointReferenceType *wsa5__From;	/* optional element of type wsa5:From */
	struct wsa5__EndpointReferenceType *wsa5__ReplyTo;	/* mustUnderstand */
	struct wsa5__EndpointReferenceType *wsa5__FaultTo;	/* mustUnderstand */
	char *wsa5__To;	/* mustUnderstand */
	char *wsa5__Action;	/* mustUnderstand */
	struct _wsse__Security *wsse__Security;	/* mustUnderstand */


};
#endif
#endif


#ifndef SOAP_TYPE_d__AppSequenceType
#define SOAP_TYPE_d__AppSequenceType (57)
/* d:AppSequenceType */
struct d__AppSequenceType
{
	unsigned int InstanceId;	/* required attribute of type xsd:unsignedInt */
	char *SequenceId;	/* optional attribute of type xsd:string */
	unsigned int MessageNumber;	/* required attribute of type xsd:unsignedInt */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif


#ifndef WITH_NOGLOBAL
#ifndef SOAP_TYPE_SOAP_ENV__Code
#define SOAP_TYPE_SOAP_ENV__Code (69)
/* SOAP Fault Code: */
struct SOAP_ENV__Code
{
	char *SOAP_ENV__Value;	/* optional element of type xsd:QName */
	struct SOAP_ENV__Code *SOAP_ENV__Subcode;	/* optional element of type SOAP-ENV:Code */
};
#endif
#endif

#ifndef WITH_NOGLOBAL
#ifndef SOAP_TYPE_SOAP_ENV__Detail
#define SOAP_TYPE_SOAP_ENV__Detail (71)
/* SOAP-ENV:Detail */
struct SOAP_ENV__Detail
{
	char *__any;
	int __type;	/* any type of element <fault> (defined below) */
	void *fault;	/* transient */
};
#endif
#endif

#ifndef WITH_NOGLOBAL
#ifndef SOAP_TYPE_SOAP_ENV__Reason
#define SOAP_TYPE_SOAP_ENV__Reason (74)
/* SOAP-ENV:Reason */
struct SOAP_ENV__Reason
{
	char *SOAP_ENV__Text;	/* optional element of type xsd:string */
};
#endif
#endif

#ifndef WITH_NOGLOBAL
#ifndef SOAP_TYPE_SOAP_ENV__Fault
#define SOAP_TYPE_SOAP_ENV__Fault (75)
/* SOAP Fault: */
struct SOAP_ENV__Fault
{
	char *faultcode;	/* optional element of type xsd:QName */
	char *faultstring;	/* optional element of type xsd:string */
	char *faultactor;	/* optional element of type xsd:string */
	struct SOAP_ENV__Detail *detail;	/* optional element of type SOAP-ENV:Detail */
	struct SOAP_ENV__Code *SOAP_ENV__Code;	/* optional element of type SOAP-ENV:Code */
	struct SOAP_ENV__Reason *SOAP_ENV__Reason;	/* optional element of type SOAP-ENV:Reason */
	char *SOAP_ENV__Node;	/* optional element of type xsd:string */
	char *SOAP_ENV__Role;	/* optional element of type xsd:string */
	struct SOAP_ENV__Detail *SOAP_ENV__Detail;	/* optional element of type SOAP-ENV:Detail */
};
#endif
#endif


//device discover
#ifndef SOAP_TYPE_wsa__EndpointReferenceType
#define SOAP_TYPE_wsa__EndpointReferenceType (7)
/* wsa:EndpointReferenceType */
struct wsa__EndpointReferenceType
{
	char *Address;	/* required element of type xsd:string */
	struct wsa__ReferencePropertiesType *ReferenceProperties;	/* optional element of type wsa:ReferencePropertiesType */
	struct wsa__ReferenceParametersType *ReferenceParameters;	/* optional element of type wsa:ReferenceParametersType */
	char **PortType;	/* optional element of type xsd:QName */
	struct wsa__ServiceNameType *ServiceName;	/* optional element of type wsa:ServiceNameType */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
typedef struct wsa__EndpointReferenceType wsa__EndpointReferenceType;
#endif

#ifndef SOAP_TYPE_wsa__ReferencePropertiesType
#define SOAP_TYPE_wsa__ReferencePropertiesType (8)
/* wsa:ReferencePropertiesType */
struct wsa__ReferencePropertiesType
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
typedef struct wsa__ReferencePropertiesType wsa__ReferencePropertiesType;
#endif

#ifndef SOAP_TYPE_wsa__ReferenceParametersType
#define SOAP_TYPE_wsa__ReferenceParametersType (9)
/* wsa:ReferenceParametersType */
struct wsa__ReferenceParametersType
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
typedef struct wsa__ReferenceParametersType wsa__ReferenceParametersType;
#endif

#ifndef SOAP_TYPE_d__ProbeType
#define SOAP_TYPE_d__ProbeType (46)
/* d:ProbeType */
struct d__ProbeType
{
	char *Types;	/* optional element of type d:QNameListType */
	struct d__ScopesType *Scopes;	/* optional element of type d:ScopesType */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_d__ProbeMatchesType
#define SOAP_TYPE_d__ProbeMatchesType (47)
/* d:ProbeMatchesType */
struct d__ProbeMatchesType
{
	int __sizeProbeMatch;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* sequence of elements <ProbeMatch> */
	struct d__ProbeMatchType *ProbeMatch;	/* optional element of type d:ProbeMatchType */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_d__ProbeMatchType
#define SOAP_TYPE_d__ProbeMatchType (48)
/* d:ProbeMatchType */
struct d__ProbeMatchType
{
	struct wsa__EndpointReferenceType wsa__EndpointReference;	/* required element of type wsa:EndpointReference */
	char *Types;	/* optional element of type d:QNameListType */
	struct d__ScopesType *Scopes;	/* optional element of type d:ScopesType */
	char *XAddrs;	/* optional element of type d:UriListType */
	unsigned int MetadataVersion;	/* required element of type xsd:unsignedInt */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE___dndl__Probe
#define SOAP_TYPE___dndl__Probe (61)
/* Operation wrapper: */
struct __dndl__Probe
{
	struct d__ProbeType *d__Probe;	/* optional element of type d:ProbeType */
};
#endif

#ifndef SOAP_TYPE_d__ScopesType
#define SOAP_TYPE_d__ScopesType (42)
/* Primitive d:ScopesType schema type: */
struct d__ScopesType
{
	char *__item;
	char *MatchBy;	/* optional attribute of type xsd:string */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_d__SigType
#define SOAP_TYPE_d__SigType (55)
/* d:SigType */
struct d__SigType
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *Scheme;	/* required attribute of type xsd:string */
	char *KeyId;	/* optional attribute of type xsd:string */
	char *Refs;	/* required attribute of type xsd:IDREFS */
	char *Sig;	/* required attribute of type xsd:string */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_d__SecurityType
#define SOAP_TYPE_d__SecurityType (54)
/* d:SecurityType */
struct d__SecurityType
{
	struct d__SigType *Sig;	/* optional element of type d:SigType */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif


//
#ifndef SOAP_TYPE___dnrd__Hello
#define SOAP_TYPE___dnrd__Hello (65)
/* Operation wrapper: */
struct __dnrd__Hello
{
	struct d__HelloType *d__Hello;	/* optional element of type d:HelloType */
};
#endif

#ifndef SOAP_TYPE___dnrd__Bye
#define SOAP_TYPE___dnrd__Bye (68)
/* Operation wrapper: */
struct __dnrd__Bye
{
	struct d__ByeType *d__Bye;	/* optional element of type d:ByeType */
};
#endif

#ifndef SOAP_TYPE_d__HelloType
#define SOAP_TYPE_d__HelloType (41)
/* d:HelloType */
struct d__HelloType
{
	struct wsa__EndpointReferenceType wsa__EndpointReference;	/* required element of type wsa:EndpointReference */
	char *Types;	/* optional element of type d:QNameListType */
	struct d__ScopesType *Scopes;	/* optional element of type d:ScopesType */
	char *XAddrs;	/* optional element of type d:UriListType */
	unsigned int MetadataVersion;	/* required element of type xsd:unsignedInt */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_d__ByeType
#define SOAP_TYPE_d__ByeType (44)
/* d:ByeType */
struct d__ByeType
{
	struct wsa__EndpointReferenceType wsa__EndpointReference;	/* required element of type wsa:EndpointReference */
	char *Types;	/* optional element of type d:QNameListType */
	struct d__ScopesType *Scopes;	/* optional element of type d:ScopesType */
	char *XAddrs;	/* optional element of type d:UriListType */
	unsigned int *MetadataVersion;	/* optional element of type xsd:unsignedInt */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_d__ResolveType
#define SOAP_TYPE_d__ResolveType (50)
/* d:ResolveType */
struct d__ResolveType
{
	struct wsa__EndpointReferenceType wsa__EndpointReference;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type wsa:EndpointReference */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_d__ResolveMatchesType
#define SOAP_TYPE_d__ResolveMatchesType (51)
/* d:ResolveMatchesType */
struct d__ResolveMatchesType
{
	struct d__ResolveMatchType *ResolveMatch;	/* optional element of type d:ResolveMatchType */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_d__ResolveMatchType
#define SOAP_TYPE_d__ResolveMatchType (52)
/* d:ResolveMatchType */
struct d__ResolveMatchType
{
	struct wsa__EndpointReferenceType wsa__EndpointReference;	/* required element of type wsa:EndpointReference */
	char *Types;	/* optional element of type d:QNameListType */
	struct d__ScopesType *Scopes;	/* optional element of type d:ScopesType */
	char *XAddrs;	/* required element of type d:UriListType */
	unsigned int MetadataVersion;	/* required element of type xsd:unsignedInt */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

//device management
//ONVIF Device Management
#ifndef SOAP_TYPE__tds__GetCapabilities
#define SOAP_TYPE__tds__GetCapabilities (222)
/* tds:GetCapabilities */
struct _tds__GetCapabilities
{
	int __sizeCategory;	/* sequence of elements <Category> */
	enum tt__CapabilityCategory *Category;	/* optional element of type tt:CapabilityCategory */
};
#endif

#ifndef SOAP_TYPE___tds__GetCapabilities
#define SOAP_TYPE___tds__GetCapabilities (1360)
/* Operation wrapper: */
struct __tds__GetCapabilities
{
	struct _tds__GetCapabilities *tds__GetCapabilities;	/* optional element of type tds:GetCapabilities */
};
#endif

#ifndef SOAP_TYPE_tt__AnalyticsCapabilities
#define SOAP_TYPE_tt__AnalyticsCapabilities (383)
/* tt:AnalyticsCapabilities */
struct tt__AnalyticsCapabilities
{
	char *XAddr;	/* required element of type xsd:string */
	enum xsd__boolean_ RuleSupport;	/* required element of type xsd:boolean */
	enum xsd__boolean_ AnalyticsModuleSupport;	/* required element of type xsd:boolean */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__DeviceCapabilitiesExtension
#define SOAP_TYPE_tt__DeviceCapabilitiesExtension (593)
/* tt:DeviceCapabilitiesExtension */
struct tt__DeviceCapabilitiesExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__NetworkCapabilitiesExtension2
#define SOAP_TYPE_tt__NetworkCapabilitiesExtension2 (610)
/* tt:NetworkCapabilitiesExtension2 */
struct tt__NetworkCapabilitiesExtension2
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__NetworkCapabilitiesExtension
#define SOAP_TYPE_tt__NetworkCapabilitiesExtension (608)
/* tt:NetworkCapabilitiesExtension */
struct tt__NetworkCapabilitiesExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
	enum xsd__boolean_ *Dot11Configuration;	/* optional element of type xsd:boolean */
	struct tt__NetworkCapabilitiesExtension2 *Extension;	/* optional element of type tt:NetworkCapabilitiesExtension2 */
};
#endif


#ifndef SOAP_TYPE_tt__NetworkCapabilities
#define SOAP_TYPE_tt__NetworkCapabilities (585)
/* tt:NetworkCapabilities */
struct tt__NetworkCapabilities
{
	enum xsd__boolean_ *IPFilter;	/* optional element of type xsd:boolean */
	enum xsd__boolean_ *ZeroConfiguration;	/* optional element of type xsd:boolean */
	enum xsd__boolean_ *IPVersion6;	/* optional element of type xsd:boolean */
	enum xsd__boolean_ *DynDNS;	/* optional element of type xsd:boolean */
	struct tt__NetworkCapabilitiesExtension *Extension;	/* optional element of type tt:NetworkCapabilitiesExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__SystemCapabilitiesExtension2
#define SOAP_TYPE_tt__SystemCapabilitiesExtension2 (618)
/* tt:SystemCapabilitiesExtension2 */
struct tt__SystemCapabilitiesExtension2
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__SystemCapabilitiesExtension
#define SOAP_TYPE_tt__SystemCapabilitiesExtension (616)
/* tt:SystemCapabilitiesExtension */
struct tt__SystemCapabilitiesExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
	enum xsd__boolean_ *HttpFirmwareUpgrade;	/* optional element of type xsd:boolean */
	enum xsd__boolean_ *HttpSystemBackup;	/* optional element of type xsd:boolean */
	enum xsd__boolean_ *HttpSystemLogging;	/* optional element of type xsd:boolean */
	enum xsd__boolean_ *HttpSupportInformation;	/* optional element of type xsd:boolean */
	struct tt__SystemCapabilitiesExtension2 *Extension;	/* optional element of type tt:SystemCapabilitiesExtension2 */
};
#endif

#ifndef SOAP_TYPE_tt__OnvifVersion
#define SOAP_TYPE_tt__OnvifVersion (126)
/* tt:OnvifVersion */
struct tt__OnvifVersion
{
	int Major;	/* required element of type xsd:int */
	int Minor;	/* required element of type xsd:int */
};
#endif

#ifndef SOAP_TYPE_tt__SystemCapabilities
#define SOAP_TYPE_tt__SystemCapabilities (587)
/* tt:SystemCapabilities */
struct tt__SystemCapabilities
{
	enum xsd__boolean_ DiscoveryResolve;	/* required element of type xsd:boolean */
	enum xsd__boolean_ DiscoveryBye;	/* required element of type xsd:boolean */
	enum xsd__boolean_ RemoteDiscovery;	/* required element of type xsd:boolean */
	enum xsd__boolean_ SystemBackup;	/* required element of type xsd:boolean */
	enum xsd__boolean_ SystemLogging;	/* required element of type xsd:boolean */
	enum xsd__boolean_ FirmwareUpgrade;	/* required element of type xsd:boolean */
	int __sizeSupportedVersions;	/* sequence of elements <SupportedVersions> */
	struct tt__OnvifVersion *SupportedVersions;	/* required element of type tt:OnvifVersion */
	struct tt__SystemCapabilitiesExtension *Extension;	/* optional element of type tt:SystemCapabilitiesExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__IOCapabilitiesExtension2
#define SOAP_TYPE_tt__IOCapabilitiesExtension2 (598)
/* tt:IOCapabilitiesExtension2 */
struct tt__IOCapabilitiesExtension2
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__IOCapabilitiesExtension
#define SOAP_TYPE_tt__IOCapabilitiesExtension (595)
/* tt:IOCapabilitiesExtension */
struct tt__IOCapabilitiesExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
	enum xsd__boolean_ *Auxiliary;	/* optional element of type xsd:boolean */
	int __sizeAuxiliaryCommands;	/* sequence of elements <AuxiliaryCommands> */
	char **AuxiliaryCommands;	/* optional element of type tt:AuxiliaryData */
	struct tt__IOCapabilitiesExtension2 *Extension;	/* required element of type tt:IOCapabilitiesExtension2 */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__IOCapabilities
#define SOAP_TYPE_tt__IOCapabilities (589)
/* tt:IOCapabilities */
struct tt__IOCapabilities
{
	int *InputConnectors;	/* optional element of type xsd:int */
	int *RelayOutputs;	/* optional element of type xsd:int */
	struct tt__IOCapabilitiesExtension *Extension;	/* optional element of type tt:IOCapabilitiesExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__SecurityCapabilitiesExtension2
#define SOAP_TYPE_tt__SecurityCapabilitiesExtension2 (614)
/* tt:SecurityCapabilitiesExtension2 */
struct tt__SecurityCapabilitiesExtension2
{
	enum xsd__boolean_ Dot1X;	/* required element of type xsd:boolean */
	int __sizeSupportedEAPMethod;	/* sequence of elements <SupportedEAPMethod> */
	int *SupportedEAPMethod;	/* optional element of type xsd:int */
	enum xsd__boolean_ RemoteUserHandling;	/* required element of type xsd:boolean */
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__SecurityCapabilitiesExtension
#define SOAP_TYPE_tt__SecurityCapabilitiesExtension (612)
/* tt:SecurityCapabilitiesExtension */
struct tt__SecurityCapabilitiesExtension
{
	enum xsd__boolean_ TLS1_x002e0;	/* required element of type xsd:boolean */
	struct tt__SecurityCapabilitiesExtension2 *Extension;	/* optional element of type tt:SecurityCapabilitiesExtension2 */
};
#endif

#ifndef SOAP_TYPE_tt__SecurityCapabilities
#define SOAP_TYPE_tt__SecurityCapabilities (591)
/* tt:SecurityCapabilities */
struct tt__SecurityCapabilities
{
	enum xsd__boolean_ TLS1_x002e1;	/* required element of type xsd:boolean */
	enum xsd__boolean_ TLS1_x002e2;	/* required element of type xsd:boolean */
	enum xsd__boolean_ OnboardKeyGeneration;	/* required element of type xsd:boolean */
	enum xsd__boolean_ AccessPolicyConfig;	/* required element of type xsd:boolean */
	enum xsd__boolean_ X_x002e509Token;	/* required element of type xsd:boolean */
	enum xsd__boolean_ SAMLToken;	/* required element of type xsd:boolean */
	enum xsd__boolean_ KerberosToken;	/* required element of type xsd:boolean */
	enum xsd__boolean_ RELToken;	/* required element of type xsd:boolean */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	struct tt__SecurityCapabilitiesExtension *Extension;	/* optional element of type tt:SecurityCapabilitiesExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif



#ifndef SOAP_TYPE_tt__DeviceCapabilities
#define SOAP_TYPE_tt__DeviceCapabilities (557)
/* tt:DeviceCapabilities */
struct tt__DeviceCapabilities
{
	char *XAddr;	/* required element of type xsd:string */
	struct tt__NetworkCapabilities *Network;	/* optional element of type tt:NetworkCapabilities */
	struct tt__SystemCapabilities *System;	/* optional element of type tt:SystemCapabilities */
	struct tt__IOCapabilities *IO;	/* optional element of type tt:IOCapabilities */
	struct tt__SecurityCapabilities *Security;	/* optional element of type tt:SecurityCapabilities */
	struct tt__DeviceCapabilitiesExtension *Extension;	/* optional element of type tt:DeviceCapabilitiesExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__EventCapabilities
#define SOAP_TYPE_tt__EventCapabilities (559)
/* tt:EventCapabilities */
struct tt__EventCapabilities
{
	char *XAddr;	/* required element of type xsd:string */
	enum xsd__boolean_ WSSubscriptionPolicySupport;	/* required element of type xsd:boolean */
	enum xsd__boolean_ WSPullPointSupport;	/* required element of type xsd:boolean */
	enum xsd__boolean_ WSPausableSubscriptionManagerInterfaceSupport;	/* required element of type xsd:boolean */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif
#ifndef SOAP_TYPE_tt__ImagingCapabilities
#define SOAP_TYPE_tt__ImagingCapabilities (561)
/* tt:ImagingCapabilities */
struct tt__ImagingCapabilities
{
	char *XAddr;	/* required element of type xsd:string */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__RealTimeStreamingCapabilitiesExtension
#define SOAP_TYPE_tt__RealTimeStreamingCapabilitiesExtension (606)
/* tt:RealTimeStreamingCapabilitiesExtension */
struct tt__RealTimeStreamingCapabilitiesExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__RealTimeStreamingCapabilities
#define SOAP_TYPE_tt__RealTimeStreamingCapabilities (600)
/* tt:RealTimeStreamingCapabilities */
struct tt__RealTimeStreamingCapabilities
{
	enum xsd__boolean_ *RTPMulticast;	/* optional element of type xsd:boolean */
	enum xsd__boolean_ *RTP_USCORETCP;	/* optional element of type xsd:boolean */
	enum xsd__boolean_ *RTP_USCORERTSP_USCORETCP;	/* optional element of type xsd:boolean */
	struct tt__RealTimeStreamingCapabilitiesExtension *Extension;	/* optional element of type tt:RealTimeStreamingCapabilitiesExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__ProfileCapabilities
#define SOAP_TYPE_tt__ProfileCapabilities (604)
/* tt:ProfileCapabilities */
struct tt__ProfileCapabilities
{
	int MaximumNumberOfProfiles;	/* required element of type xsd:int */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__MediaCapabilitiesExtension
#define SOAP_TYPE_tt__MediaCapabilitiesExtension (602)
/* tt:MediaCapabilitiesExtension */
struct tt__MediaCapabilitiesExtension
{
	struct tt__ProfileCapabilities *ProfileCapabilities;	/* required element of type tt:ProfileCapabilities */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__MediaCapabilities
#define SOAP_TYPE_tt__MediaCapabilities (563)
/* tt:MediaCapabilities */
struct tt__MediaCapabilities
{
	char *XAddr;	/* required element of type xsd:string */
	struct tt__RealTimeStreamingCapabilities *StreamingCapabilities;	/* required element of type tt:RealTimeStreamingCapabilities */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	struct tt__MediaCapabilitiesExtension *Extension;	/* optional element of type tt:MediaCapabilitiesExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__PTZCapabilities
#define SOAP_TYPE_tt__PTZCapabilities (565)
/* tt:PTZCapabilities */
struct tt__PTZCapabilities
{
	char *XAddr;	/* required element of type xsd:string */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__DeviceIOCapabilities
#define SOAP_TYPE_tt__DeviceIOCapabilities (569)
/* tt:DeviceIOCapabilities */
struct tt__DeviceIOCapabilities
{
	char *XAddr;	/* required element of type xsd:string */
	int VideoSources;	/* required element of type xsd:int */
	int VideoOutputs;	/* required element of type xsd:int */
	int AudioSources;	/* required element of type xsd:int */
	int AudioOutputs;	/* required element of type xsd:int */
	int RelayOutputs;	/* required element of type xsd:int */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__DisplayCapabilities
#define SOAP_TYPE_tt__DisplayCapabilities (571)
/* tt:DisplayCapabilities */
struct tt__DisplayCapabilities
{
	char *XAddr;	/* required element of type xsd:string */
	enum xsd__boolean_ FixedLayout;	/* required element of type xsd:boolean */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__RecordingCapabilities
#define SOAP_TYPE_tt__RecordingCapabilities (573)
/* tt:RecordingCapabilities */
struct tt__RecordingCapabilities
{
	char *XAddr;	/* required element of type xsd:string */
	enum xsd__boolean_ ReceiverSource;	/* required element of type xsd:boolean */
	enum xsd__boolean_ MediaProfileSource;	/* required element of type xsd:boolean */
	enum xsd__boolean_ DynamicRecordings;	/* required element of type xsd:boolean */
	enum xsd__boolean_ DynamicTracks;	/* required element of type xsd:boolean */
	int MaxStringLength;	/* required element of type xsd:int */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__SearchCapabilities
#define SOAP_TYPE_tt__SearchCapabilities (575)
/* tt:SearchCapabilities */
struct tt__SearchCapabilities
{
	char *XAddr;	/* required element of type xsd:string */
	enum xsd__boolean_ MetadataSearch;	/* required element of type xsd:boolean */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__ReplayCapabilities
#define SOAP_TYPE_tt__ReplayCapabilities (577)
/* tt:ReplayCapabilities */
struct tt__ReplayCapabilities
{
	char *XAddr;	/* required element of type xsd:string */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__ReceiverCapabilities
#define SOAP_TYPE_tt__ReceiverCapabilities (579)
/* tt:ReceiverCapabilities */
struct tt__ReceiverCapabilities
{
	char *XAddr;	/* required element of type xsd:string */
	enum xsd__boolean_ RTP_USCOREMulticast;	/* required element of type xsd:boolean */
	enum xsd__boolean_ RTP_USCORETCP;	/* required element of type xsd:boolean */
	enum xsd__boolean_ RTP_USCORERTSP_USCORETCP;	/* required element of type xsd:boolean */
	int SupportedReceivers;	/* required element of type xsd:int */
	int MaximumRTSPURILength;	/* required element of type xsd:int */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__AnalyticsDeviceExtension
#define SOAP_TYPE_tt__AnalyticsDeviceExtension (620)
/* tt:AnalyticsDeviceExtension */
struct tt__AnalyticsDeviceExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif


#ifndef SOAP_TYPE_tt__AnalyticsDeviceCapabilities
#define SOAP_TYPE_tt__AnalyticsDeviceCapabilities (581)
/* tt:AnalyticsDeviceCapabilities */
struct tt__AnalyticsDeviceCapabilities
{
	char *XAddr;	/* required element of type xsd:string */
	enum xsd__boolean_ *RuleSupport;	/* optional element of type xsd:boolean */
	struct tt__AnalyticsDeviceExtension *Extension;	/* optional element of type tt:AnalyticsDeviceExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__CapabilitiesExtension
#define SOAP_TYPE_tt__CapabilitiesExtension (567)
/* tt:CapabilitiesExtension */
struct tt__CapabilitiesExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
	struct tt__DeviceIOCapabilities *DeviceIO;	/* optional element of type tt:DeviceIOCapabilities */
	struct tt__DisplayCapabilities *Display;	/* optional element of type tt:DisplayCapabilities */
	struct tt__RecordingCapabilities *Recording;	/* optional element of type tt:RecordingCapabilities */
	struct tt__SearchCapabilities *Search;	/* optional element of type tt:SearchCapabilities */
	struct tt__ReplayCapabilities *Replay;	/* optional element of type tt:ReplayCapabilities */
	struct tt__ReceiverCapabilities *Receiver;	/* optional element of type tt:ReceiverCapabilities */
	struct tt__AnalyticsDeviceCapabilities *AnalyticsDevice;	/* optional element of type tt:AnalyticsDeviceCapabilities */
	struct tt__CapabilitiesExtension2 *Extensions;	/* optional element of type tt:CapabilitiesExtension2 */
};
#endif

#ifndef SOAP_TYPE_tt__Capabilities
#define SOAP_TYPE_tt__Capabilities (225)
/* tt:Capabilities */
struct tt__Capabilities
{
	struct tt__AnalyticsCapabilities *Analytics;	/* optional element of type tt:AnalyticsCapabilities */
	struct tt__DeviceCapabilities *Device;	/* optional element of type tt:DeviceCapabilities */
	struct tt__EventCapabilities *Events;	/* optional element of type tt:EventCapabilities */
	struct tt__ImagingCapabilities *Imaging;	/* optional element of type tt:ImagingCapabilities */
	struct tt__MediaCapabilities *Media;	/* optional element of type tt:MediaCapabilities */
	struct tt__PTZCapabilities *PTZ;	/* optional element of type tt:PTZCapabilities */
	struct tt__CapabilitiesExtension *Extension;	/* optional element of type tt:CapabilitiesExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE__tds__GetCapabilitiesResponse
#define SOAP_TYPE__tds__GetCapabilitiesResponse (224)
/* tds:GetCapabilitiesResponse */
struct _tds__GetCapabilitiesResponse
{
	struct tt__Capabilities *Capabilities;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type tt:Capabilities */
};
#endif

#ifndef SOAP_TYPE_tt__CapabilitiesExtension2
#define SOAP_TYPE_tt__CapabilitiesExtension2 (583)
/* tt:CapabilitiesExtension2 */
struct tt__CapabilitiesExtension2
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__AnalyticsCapabilities
#define SOAP_TYPE_tt__AnalyticsCapabilities (555)
/* tt:AnalyticsCapabilities */
struct tt__AnalyticsCapabilities
{
	char *XAddr;	/* required element of type xsd:string */
	enum xsd__boolean_ RuleSupport;	/* required element of type xsd:boolean */
	enum xsd__boolean_ AnalyticsModuleSupport;	/* required element of type xsd:boolean */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

//system management
#ifndef SOAP_TYPE__tds__GetDeviceInformation
#define SOAP_TYPE__tds__GetDeviceInformation (143)
/* tds:GetDeviceInformation */
struct _tds__GetDeviceInformation
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE___tds__GetDeviceInformation
#define SOAP_TYPE___tds__GetDeviceInformation (1252)
/* Operation wrapper: */
struct __tds__GetDeviceInformation
{
	struct _tds__GetDeviceInformation *tds__GetDeviceInformation;	/* optional element of type tds:GetDeviceInformation */
};
#endif

#ifndef SOAP_TYPE__tds__GetDeviceInformationResponse
#define SOAP_TYPE__tds__GetDeviceInformationResponse (144)
/* tds:GetDeviceInformationResponse */
struct _tds__GetDeviceInformationResponse
{
	char *Manufacturer;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type xsd:string */
	char *Model;	/* required element of type xsd:string */
	char *FirmwareVersion;	/* required element of type xsd:string */
	char *SerialNumber;	/* required element of type xsd:string */
	char *HardwareId;	/* required element of type xsd:string */
};
#endif


#ifndef SOAP_TYPE_tt__Date
#define SOAP_TYPE_tt__Date (630)
/* tt:Date */
struct tt__Date
{
	int Year;	/* required element of type xsd:int */
	int Month;	/* required element of type xsd:int */
	int Day;	/* required element of type xsd:int */
};
#endif

#ifndef SOAP_TYPE_tt__Time
#define SOAP_TYPE_tt__Time (628)
/* tt:Time */
struct tt__Time
{
	int Hour;	/* required element of type xsd:int */
	int Minute;	/* required element of type xsd:int */
	int Second;	/* required element of type xsd:int */
};
#endif

#ifndef SOAP_TYPE_tt__TimeZone
#define SOAP_TYPE_tt__TimeZone (146)
/* tt:TimeZone */
struct tt__TimeZone
{
	char *TZ;	/* required element of type xsd:token */
};
#endif

#ifndef SOAP_TYPE_tt__DateTime
#define SOAP_TYPE_tt__DateTime (148)
/* tt:DateTime */
struct tt__DateTime
{
	struct tt__Time *Time;	/* required element of type tt:Time */
	struct tt__Date *Date;	/* required element of type tt:Date */
};
#endif

#ifndef SOAP_TYPE_tt__SystemDateTimeExtension
#define SOAP_TYPE_tt__SystemDateTimeExtension (626)
/* tt:SystemDateTimeExtension */
struct tt__SystemDateTimeExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__SystemDateTime
#define SOAP_TYPE_tt__SystemDateTime (153)
/* tt:SystemDateTime */
struct tt__SystemDateTime
{
	enum tt__SetDateTimeType DateTimeType;	/* required element of type tt:SetDateTimeType */
	enum xsd__boolean_ DaylightSavings;	/* required element of type xsd:boolean */
	struct tt__TimeZone *TimeZone;	/* optional element of type tt:TimeZone */
	struct tt__DateTime *UTCDateTime;	/* optional element of type tt:DateTime */
	struct tt__DateTime *LocalDateTime;	/* optional element of type tt:DateTime */
	struct tt__SystemDateTimeExtension *Extension;	/* optional element of type tt:SystemDateTimeExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE__tds__GetSystemDateAndTime
#define SOAP_TYPE__tds__GetSystemDateAndTime (151)
/* tds:GetSystemDateAndTime */
struct _tds__GetSystemDateAndTime
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tds__GetSystemDateAndTimeResponse
#define SOAP_TYPE__tds__GetSystemDateAndTimeResponse (152)
/* tds:GetSystemDateAndTimeResponse */
struct _tds__GetSystemDateAndTimeResponse
{
	struct tt__SystemDateTime *SystemDateAndTime;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type tt:SystemDateTime */
};
#endif

#ifndef SOAP_TYPE___tds__GetSystemDateAndTime
#define SOAP_TYPE___tds__GetSystemDateAndTime (1260)
/* Operation wrapper: */
struct __tds__GetSystemDateAndTime
{
	struct _tds__GetSystemDateAndTime *tds__GetSystemDateAndTime;	/* optional element of type tds:GetSystemDateAndTime */
};
#endif

#ifndef SOAP_TYPE__tds__SetSystemDateAndTime
#define SOAP_TYPE__tds__SetSystemDateAndTime (145)
/* tds:SetSystemDateAndTime */
struct _tds__SetSystemDateAndTime
{
	enum tt__SetDateTimeType DateTimeType;	/* required element of type tt:SetDateTimeType */
	enum xsd__boolean_ DaylightSavings;	/* required element of type xsd:boolean */
	struct tt__TimeZone *TimeZone;	/* optional element of type tt:TimeZone */
	struct tt__DateTime *UTCDateTime;	/* optional element of type tt:DateTime */
};
#endif

#ifndef SOAP_TYPE__tds__SetSystemDateAndTimeResponse
#define SOAP_TYPE__tds__SetSystemDateAndTimeResponse (150)
/* tds:SetSystemDateAndTimeResponse */
struct _tds__SetSystemDateAndTimeResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE___tds__SetSystemDateAndTime
#define SOAP_TYPE___tds__SetSystemDateAndTime (1256)
/* Operation wrapper: */
struct __tds__SetSystemDateAndTime
{
	struct _tds__SetSystemDateAndTime *tds__SetSystemDateAndTime;	/* optional element of type tds:SetSystemDateAndTime */
};
#endif

#ifndef SOAP_TYPE__tds__SystemReboot
#define SOAP_TYPE__tds__SystemReboot (161)
/* tds:SystemReboot */
struct _tds__SystemReboot
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tds__SystemRebootResponse
#define SOAP_TYPE__tds__SystemRebootResponse (162)
/* tds:SystemRebootResponse */
struct _tds__SystemRebootResponse
{
	char *Message;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE___tds__SystemReboot
#define SOAP_TYPE___tds__SystemReboot (1272)
/* Operation wrapper: */
struct __tds__SystemReboot
{
	struct _tds__SystemReboot *tds__SystemReboot;	/* optional element of type tds:SystemReboot */
};
#endif

#ifndef SOAP_TYPE__tds__SetSystemFactoryDefault
#define SOAP_TYPE__tds__SetSystemFactoryDefault (155)
/* tds:SetSystemFactoryDefault */
struct _tds__SetSystemFactoryDefault
{
	enum tt__FactoryDefaultType FactoryDefault;	/* required element of type tt:FactoryDefaultType */
};
#endif

#ifndef SOAP_TYPE__tds__SetSystemFactoryDefaultResponse
#define SOAP_TYPE__tds__SetSystemFactoryDefaultResponse (156)
/* tds:SetSystemFactoryDefaultResponse */
struct _tds__SetSystemFactoryDefaultResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE___tds__SetSystemFactoryDefault
#define SOAP_TYPE___tds__SetSystemFactoryDefault (1264)
/* Operation wrapper: */
struct __tds__SetSystemFactoryDefault
{
	struct _tds__SetSystemFactoryDefault *tds__SetSystemFactoryDefault;	/* optional element of type tds:SetSystemFactoryDefault */
};
#endif

//device sercurity

#ifndef SOAP_TYPE_tt__UserExtension
#define SOAP_TYPE_tt__UserExtension (632)
/* tt:UserExtension */
struct tt__UserExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__User
#define SOAP_TYPE_tt__User (212)
/* tt:User */
struct tt__User
{
	char *Username;	/* required element of type xsd:string */
	char *Password;	/* optional element of type xsd:string */
	enum tt__UserLevel UserLevel;	/* required element of type tt:UserLevel */
	struct tt__UserExtension *Extension;	/* optional element of type tt:UserExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE__tds__GetUsers
#define SOAP_TYPE__tds__GetUsers (210)
/* tds:GetUsers */
struct _tds__GetUsers
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE___tds__GetUsers
#define SOAP_TYPE___tds__GetUsers (1340)
/* Operation wrapper: */
struct __tds__GetUsers
{
	struct _tds__GetUsers *tds__GetUsers;	/* optional element of type tds:GetUsers */
};
#endif

#ifndef SOAP_TYPE__tds__GetUsersResponse
#define SOAP_TYPE__tds__GetUsersResponse (211)
/* tds:GetUsersResponse */
struct _tds__GetUsersResponse
{
	int __sizeUser;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* sequence of elements <User> */
	struct tt__User *User;	/* optional element of type tt:User */
};
#endif

#ifndef SOAP_TYPE__tds__CreateUsers
#define SOAP_TYPE__tds__CreateUsers (214)
/* tds:CreateUsers */
struct _tds__CreateUsers
{
	int __sizeUser;	/* sequence of elements <User> */
	struct tt__User *User;	/* required element of type tt:User */
};
#endif

#ifndef SOAP_TYPE__tds__CreateUsersResponse
#define SOAP_TYPE__tds__CreateUsersResponse (215)
/* tds:CreateUsersResponse */
struct _tds__CreateUsersResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE___tds__CreateUsers
#define SOAP_TYPE___tds__CreateUsers (1344)
/* Operation wrapper: */
struct __tds__CreateUsers
{
	struct _tds__CreateUsers *tds__CreateUsers;	/* optional element of type tds:CreateUsers */
};
#endif

#ifndef SOAP_TYPE__tds__DeleteUsers
#define SOAP_TYPE__tds__DeleteUsers (216)
/* tds:DeleteUsers */
struct _tds__DeleteUsers
{
	int __sizeUsername;	/* sequence of elements <Username> */
	char **Username;	/* required element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE__tds__DeleteUsersResponse
#define SOAP_TYPE__tds__DeleteUsersResponse (217)
/* tds:DeleteUsersResponse */
struct _tds__DeleteUsersResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE___tds__DeleteUsers
#define SOAP_TYPE___tds__DeleteUsers (1348)
/* Operation wrapper: */
struct __tds__DeleteUsers
{
	struct _tds__DeleteUsers *tds__DeleteUsers;	/* optional element of type tds:DeleteUsers */
};
#endif

#ifndef SOAP_TYPE__tds__SetUser
#define SOAP_TYPE__tds__SetUser (218)
/* tds:SetUser */
struct _tds__SetUser
{
	int __sizeUser;	/* sequence of elements <User> */
	struct tt__User *User;	/* required element of type tt:User */
};
#endif

#ifndef SOAP_TYPE__tds__SetUserResponse
#define SOAP_TYPE__tds__SetUserResponse (219)
/* tds:SetUserResponse */
struct _tds__SetUserResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE___tds__SetUser
#define SOAP_TYPE___tds__SetUser (1352)
/* Operation wrapper: */
struct __tds__SetUser
{
	struct _tds__SetUser *tds__SetUser;	/* optional element of type tds:SetUser */
};
#endif

//device network

#ifndef SOAP_TYPE_tt__PrefixedIPv4Address
#define SOAP_TYPE_tt__PrefixedIPv4Address (510)
/* tt:PrefixedIPv4Address */
struct tt__PrefixedIPv4Address
{
	char *Address;	/* required element of type tt:IPv4Address */
	int PrefixLength;	/* required element of type xsd:int */
};
#endif

#ifndef SOAP_TYPE_tt__IPv4Configuration
#define SOAP_TYPE_tt__IPv4Configuration (508)
/* tt:IPv4Configuration */
struct tt__IPv4Configuration
{
	int __sizeManual;	/* sequence of elements <Manual> */
	struct tt__PrefixedIPv4Address *Manual;	/* optional element of type tt:PrefixedIPv4Address */
	struct tt__PrefixedIPv4Address *LinkLocal;	/* optional element of type tt:PrefixedIPv4Address */
	struct tt__PrefixedIPv4Address *FromDHCP;	/* optional element of type tt:PrefixedIPv4Address */
	enum xsd__boolean_ DHCP;	/* required element of type xsd:boolean */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__IPv4NetworkInterface
#define SOAP_TYPE_tt__IPv4NetworkInterface (507)
/* tt:IPv4NetworkInterface */
struct tt__IPv4NetworkInterface
{
	enum xsd__boolean_ Enabled;	/* required element of type xsd:boolean */
	struct tt__IPv4Configuration *Config;	/* required element of type tt:IPv4Configuration */
};
#endif

#ifndef SOAP_TYPE_tt__PrefixedIPv6Address
#define SOAP_TYPE_tt__PrefixedIPv6Address (512)
/* tt:PrefixedIPv6Address */
struct tt__PrefixedIPv6Address
{
	char *Address;	/* required element of type tt:IPv6Address */
	int PrefixLength;	/* required element of type xsd:int */
};
#endif

#ifndef SOAP_TYPE_tt__IPv6ConfigurationExtension
#define SOAP_TYPE_tt__IPv6ConfigurationExtension (514)
/* tt:IPv6ConfigurationExtension */
struct tt__IPv6ConfigurationExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__IPv6Configuration
#define SOAP_TYPE_tt__IPv6Configuration (505)
/* tt:IPv6Configuration */
struct tt__IPv6Configuration
{
	enum xsd__boolean_ *AcceptRouterAdvert;	/* optional element of type xsd:boolean */
	enum tt__IPv6DHCPConfiguration DHCP;	/* required element of type tt:IPv6DHCPConfiguration */
	int __sizeManual;	/* sequence of elements <Manual> */
	struct tt__PrefixedIPv6Address *Manual;	/* optional element of type tt:PrefixedIPv6Address */
	int __sizeLinkLocal;	/* sequence of elements <LinkLocal> */
	struct tt__PrefixedIPv6Address *LinkLocal;	/* optional element of type tt:PrefixedIPv6Address */
	int __sizeFromDHCP;	/* sequence of elements <FromDHCP> */
	struct tt__PrefixedIPv6Address *FromDHCP;	/* optional element of type tt:PrefixedIPv6Address */
	int __sizeFromRA;	/* sequence of elements <FromRA> */
	struct tt__PrefixedIPv6Address *FromRA;	/* optional element of type tt:PrefixedIPv6Address */
	struct tt__IPv6ConfigurationExtension *Extension;	/* optional element of type tt:IPv6ConfigurationExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__IPv6NetworkInterface
#define SOAP_TYPE_tt__IPv6NetworkInterface (504)
/* tt:IPv6NetworkInterface */
struct tt__IPv6NetworkInterface
{
	enum xsd__boolean_ Enabled;	/* required element of type xsd:boolean */
	struct tt__IPv6Configuration *Config;	/* optional element of type tt:IPv6Configuration */
};
#endif

#ifndef SOAP_TYPE_tt__NetworkInterfaceInfo
#define SOAP_TYPE_tt__NetworkInterfaceInfo (503)
/* tt:NetworkInterfaceInfo */
struct tt__NetworkInterfaceInfo
{
	char *Name;	/* optional element of type xsd:string */
	char *HwAddress;	/* required element of type tt:HwAddress */
	int *MTU;	/* optional element of type xsd:int */
};
#endif

#ifndef SOAP_TYPE_tt__NetworkInterfaceConnectionSetting
#define SOAP_TYPE_tt__NetworkInterfaceConnectionSetting (501)
/* tt:NetworkInterfaceConnectionSetting */
struct tt__NetworkInterfaceConnectionSetting
{
	enum xsd__boolean_ AutoNegotiation;	/* required element of type xsd:boolean */
	int Speed;	/* required element of type xsd:int */
	enum tt__Duplex Duplex;	/* required element of type tt:Duplex */
};
#endif

#ifndef SOAP_TYPE_tt__NetworkInterfaceLink
#define SOAP_TYPE_tt__NetworkInterfaceLink (500)
/* tt:NetworkInterfaceLink */
struct tt__NetworkInterfaceLink
{
	struct tt__NetworkInterfaceConnectionSetting *AdminSettings;	/* required element of type tt:NetworkInterfaceConnectionSetting */
	struct tt__NetworkInterfaceConnectionSetting *OperSettings;	/* required element of type tt:NetworkInterfaceConnectionSetting */
	int InterfaceType;	/* required element of type tt:IANA-IfTypes */
};
#endif

#ifndef SOAP_TYPE_tt__Dot3Configuration
#define SOAP_TYPE_tt__Dot3Configuration (494)
/* tt:Dot3Configuration */
struct tt__Dot3Configuration
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__Dot11PSKSetExtension
#define SOAP_TYPE_tt__Dot11PSKSetExtension (549)
/* tt:Dot11PSKSetExtension */
struct tt__Dot11PSKSetExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__Dot11PSKSet
#define SOAP_TYPE_tt__Dot11PSKSet (544)
/* tt:Dot11PSKSet */
struct tt__Dot11PSKSet
{
	struct xsd__hexBinary *Key;	/* optional element of type tt:Dot11PSK */
	char *Passphrase;	/* optional element of type tt:Dot11PSKPassphrase */
	struct tt__Dot11PSKSetExtension *Extension;	/* optional element of type tt:Dot11PSKSetExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__Dot11SecurityConfigurationExtension
#define SOAP_TYPE_tt__Dot11SecurityConfigurationExtension (546)
/* tt:Dot11SecurityConfigurationExtension */
struct tt__Dot11SecurityConfigurationExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif


#ifndef SOAP_TYPE_tt__Dot11SecurityConfiguration
#define SOAP_TYPE_tt__Dot11SecurityConfiguration (541)
/* tt:Dot11SecurityConfiguration */
struct tt__Dot11SecurityConfiguration
{
	enum tt__Dot11SecurityMode Mode;	/* required element of type tt:Dot11SecurityMode */
	enum tt__Dot11Cipher *Algorithm;	/* optional element of type tt:Dot11Cipher */
	struct tt__Dot11PSKSet *PSK;	/* optional element of type tt:Dot11PSKSet */
	char *Dot1X;	/* optional element of type tt:ReferenceToken */
	struct tt__Dot11SecurityConfigurationExtension *Extension;	/* optional element of type tt:Dot11SecurityConfigurationExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__Dot11Configuration
#define SOAP_TYPE_tt__Dot11Configuration (496)
/* tt:Dot11Configuration */
struct tt__Dot11Configuration
{
	struct xsd__hexBinary SSID;	/* required element of type tt:Dot11SSIDType */
	enum tt__Dot11StationMode Mode;	/* required element of type tt:Dot11StationMode */
	char *Alias;	/* required element of type tt:Name */
	int Priority;	/* required element of type tt:NetworkInterfaceConfigPriority */
	struct tt__Dot11SecurityConfiguration *Security;	/* required element of type tt:Dot11SecurityConfiguration */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__NetworkInterfaceExtension2
#define SOAP_TYPE_tt__NetworkInterfaceExtension2 (498)
/* tt:NetworkInterfaceExtension2 */
struct tt__NetworkInterfaceExtension2
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__NetworkInterfaceExtension
#define SOAP_TYPE_tt__NetworkInterfaceExtension (493)
/* tt:NetworkInterfaceExtension */
struct tt__NetworkInterfaceExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
	int InterfaceType;	/* required element of type tt:IANA-IfTypes */
	int __sizeDot3;	/* sequence of elements <Dot3> */
	struct tt__Dot3Configuration *Dot3;	/* optional element of type tt:Dot3Configuration */
	int __sizeDot11;	/* sequence of elements <Dot11> */
	struct tt__Dot11Configuration *Dot11;	/* optional element of type tt:Dot11Configuration */
	struct tt__NetworkInterfaceExtension2 *Extension;	/* optional element of type tt:NetworkInterfaceExtension2 */
};
#endif


#ifndef SOAP_TYPE_tt__NetworkInterface
#define SOAP_TYPE_tt__NetworkInterface (259)
/* tt:NetworkInterface */
struct tt__NetworkInterface
{
	char *token;	/* required attribute of type tt:ReferenceToken */
	enum xsd__boolean_ Enabled;	/* required element of type xsd:boolean */
	struct tt__NetworkInterfaceInfo *Info;	/* optional element of type tt:NetworkInterfaceInfo */
	struct tt__NetworkInterfaceLink *Link;	/* optional element of type tt:NetworkInterfaceLink */
	struct tt__IPv4NetworkInterface *IPv4;	/* optional element of type tt:IPv4NetworkInterface */
	struct tt__IPv6NetworkInterface *IPv6;	/* optional element of type tt:IPv6NetworkInterface */
	struct tt__NetworkInterfaceExtension *Extension;	/* optional element of type tt:NetworkInterfaceExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE__tds__GetNetworkInterfaces
#define SOAP_TYPE__tds__GetNetworkInterfaces (257)
/* tds:GetNetworkInterfaces */
struct _tds__GetNetworkInterfaces
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif



#ifndef SOAP_TYPE__tds__GetNetworkInterfacesResponse
#define SOAP_TYPE__tds__GetNetworkInterfacesResponse (258)
/* tds:GetNetworkInterfacesResponse */
struct _tds__GetNetworkInterfacesResponse
{
	int __sizeNetworkInterfaces;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* sequence of elements <NetworkInterfaces> */
	struct tt__NetworkInterface *NetworkInterfaces;	/* required element of type tt:NetworkInterface */
};
#endif

#ifndef SOAP_TYPE___tds__GetNetworkInterfaces
#define SOAP_TYPE___tds__GetNetworkInterfaces (1404)
/* Operation wrapper: */
struct __tds__GetNetworkInterfaces
{
	struct _tds__GetNetworkInterfaces *tds__GetNetworkInterfaces;	/* optional element of type tds:GetNetworkInterfaces */
};
#endif

#ifndef SOAP_TYPE_tt__IPv4NetworkInterfaceSetConfiguration
#define SOAP_TYPE_tt__IPv4NetworkInterfaceSetConfiguration (528)
/* tt:IPv4NetworkInterfaceSetConfiguration */
struct tt__IPv4NetworkInterfaceSetConfiguration
{
	enum xsd__boolean_ *Enabled;	/* optional element of type xsd:boolean */
	int __sizeManual;	/* sequence of elements <Manual> */
	struct tt__PrefixedIPv4Address *Manual;	/* optional element of type tt:PrefixedIPv4Address */
	enum xsd__boolean_ *DHCP;	/* optional element of type xsd:boolean */
};
#endif

#ifndef SOAP_TYPE_tt__IPv6NetworkInterfaceSetConfiguration
#define SOAP_TYPE_tt__IPv6NetworkInterfaceSetConfiguration (530)
/* tt:IPv6NetworkInterfaceSetConfiguration */
struct tt__IPv6NetworkInterfaceSetConfiguration
{
	enum xsd__boolean_ *Enabled;	/* optional element of type xsd:boolean */
	enum xsd__boolean_ *AcceptRouterAdvert;	/* optional element of type xsd:boolean */
	int __sizeManual;	/* sequence of elements <Manual> */
	struct tt__PrefixedIPv6Address *Manual;	/* optional element of type tt:PrefixedIPv6Address */
	enum tt__IPv6DHCPConfiguration *DHCP;	/* optional element of type tt:IPv6DHCPConfiguration */
};
#endif

#ifndef SOAP_TYPE_tt__NetworkInterfaceSetConfigurationExtension2
#define SOAP_TYPE_tt__NetworkInterfaceSetConfigurationExtension2 (534)
/* tt:NetworkInterfaceSetConfigurationExtension2 */
struct tt__NetworkInterfaceSetConfigurationExtension2
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__NetworkInterfaceSetConfigurationExtension
#define SOAP_TYPE_tt__NetworkInterfaceSetConfigurationExtension (532)
/* tt:NetworkInterfaceSetConfigurationExtension */
struct tt__NetworkInterfaceSetConfigurationExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
	int __sizeDot3;	/* sequence of elements <Dot3> */
	struct tt__Dot3Configuration *Dot3;	/* optional element of type tt:Dot3Configuration */
	struct tt__Dot11Configuration *Dot11;	/* optional element of type tt:Dot11Configuration */
	struct tt__NetworkInterfaceSetConfigurationExtension2 *Extension;	/* optional element of type tt:NetworkInterfaceSetConfigurationExtension2 */
};
#endif

#ifndef SOAP_TYPE_tt__NetworkInterfaceSetConfiguration
#define SOAP_TYPE_tt__NetworkInterfaceSetConfiguration (262)
/* tt:NetworkInterfaceSetConfiguration */
struct tt__NetworkInterfaceSetConfiguration
{
	enum xsd__boolean_ *Enabled;	/* optional element of type xsd:boolean */
	struct tt__NetworkInterfaceConnectionSetting *Link;	/* optional element of type tt:NetworkInterfaceConnectionSetting */
	int *MTU;	/* optional element of type xsd:int */
	struct tt__IPv4NetworkInterfaceSetConfiguration *IPv4;	/* optional element of type tt:IPv4NetworkInterfaceSetConfiguration */
	struct tt__IPv6NetworkInterfaceSetConfiguration *IPv6;	/* optional element of type tt:IPv6NetworkInterfaceSetConfiguration */
	struct tt__NetworkInterfaceSetConfigurationExtension *Extension;	/* optional element of type tt:NetworkInterfaceSetConfigurationExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE__tds__SetNetworkInterfaces
#define SOAP_TYPE__tds__SetNetworkInterfaces (261)
/* tds:SetNetworkInterfaces */
struct _tds__SetNetworkInterfaces
{
	char *InterfaceToken;	/* required element of type tt:ReferenceToken */
	struct tt__NetworkInterfaceSetConfiguration *NetworkInterface;	/* required element of type tt:NetworkInterfaceSetConfiguration */
};
#endif

#ifndef SOAP_TYPE__tds__SetNetworkInterfacesResponse
#define SOAP_TYPE__tds__SetNetworkInterfacesResponse (264)
/* tds:SetNetworkInterfacesResponse */
struct _tds__SetNetworkInterfacesResponse
{
	enum xsd__boolean_ RebootNeeded;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type xsd:boolean */
};
#endif

#ifndef SOAP_TYPE___tds__SetNetworkInterfaces
#define SOAP_TYPE___tds__SetNetworkInterfaces (1408)
/* Operation wrapper: */
struct __tds__SetNetworkInterfaces
{
	struct _tds__SetNetworkInterfaces *tds__SetNetworkInterfaces;	/* optional element of type tds:SetNetworkInterfaces */
};
#endif

#ifndef SOAP_TYPE_tt__NetworkProtocolExtension
#define SOAP_TYPE_tt__NetworkProtocolExtension (516)
/* tt:NetworkProtocolExtension */
struct tt__NetworkProtocolExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__NetworkProtocol
#define SOAP_TYPE_tt__NetworkProtocol (267)
/* tt:NetworkProtocol */
struct tt__NetworkProtocol
{
	enum tt__NetworkProtocolType Name;	/* required element of type tt:NetworkProtocolType */
	enum xsd__boolean_ Enabled;	/* required element of type xsd:boolean */
	int __sizePort;	/* sequence of elements <Port> */
	int *Port;	/* required element of type xsd:int */
	struct tt__NetworkProtocolExtension *Extension;	/* optional element of type tt:NetworkProtocolExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE__tds__GetNetworkProtocols
#define SOAP_TYPE__tds__GetNetworkProtocols (265)
/* tds:GetNetworkProtocols */
struct _tds__GetNetworkProtocols
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tds__GetNetworkProtocolsResponse
#define SOAP_TYPE__tds__GetNetworkProtocolsResponse (266)
/* tds:GetNetworkProtocolsResponse */
struct _tds__GetNetworkProtocolsResponse
{
	int __sizeNetworkProtocols;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* sequence of elements <NetworkProtocols> */
	struct tt__NetworkProtocol *NetworkProtocols;	/* optional element of type tt:NetworkProtocol */
};
#endif

#ifndef SOAP_TYPE__tds__SetNetworkProtocols
#define SOAP_TYPE__tds__SetNetworkProtocols (269)
/* tds:SetNetworkProtocols */
struct _tds__SetNetworkProtocols
{
	int __sizeNetworkProtocols;	/* sequence of elements <NetworkProtocols> */
	struct tt__NetworkProtocol *NetworkProtocols;	/* required element of type tt:NetworkProtocol */
};
#endif

#ifndef SOAP_TYPE__tds__SetNetworkProtocolsResponse
#define SOAP_TYPE__tds__SetNetworkProtocolsResponse (270)
/* tds:SetNetworkProtocolsResponse */
struct _tds__SetNetworkProtocolsResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE___tds__GetNetworkProtocols
#define SOAP_TYPE___tds__GetNetworkProtocols (1412)
/* Operation wrapper: */
struct __tds__GetNetworkProtocols
{
	struct _tds__GetNetworkProtocols *tds__GetNetworkProtocols;	/* optional element of type tds:GetNetworkProtocols */
};
#endif

#ifndef SOAP_TYPE___tds__SetNetworkProtocols
#define SOAP_TYPE___tds__SetNetworkProtocols (1416)
/* Operation wrapper: */
struct __tds__SetNetworkProtocols
{
	struct _tds__SetNetworkProtocols *tds__SetNetworkProtocols;	/* optional element of type tds:SetNetworkProtocols */
};
#endif

#ifndef SOAP_TYPE_tt__DNSInformationExtension
#define SOAP_TYPE_tt__DNSInformationExtension (522)
/* tt:DNSInformationExtension */
struct tt__DNSInformationExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__DNSInformation
#define SOAP_TYPE_tt__DNSInformation (237)
/* tt:DNSInformation */
struct tt__DNSInformation
{
	enum xsd__boolean_ FromDHCP;	/* required element of type xsd:boolean */
	int __sizeSearchDomain;	/* sequence of elements <SearchDomain> */
	char **SearchDomain;	/* optional element of type xsd:token */
	int __sizeDNSFromDHCP;	/* sequence of elements <DNSFromDHCP> */
	struct tt__IPAddress *DNSFromDHCP;	/* optional element of type tt:IPAddress */
	int __sizeDNSManual;	/* sequence of elements <DNSManual> */
	struct tt__IPAddress *DNSManual;	/* optional element of type tt:IPAddress */
	struct tt__DNSInformationExtension *Extension;	/* optional element of type tt:DNSInformationExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE__tds__GetDNS
#define SOAP_TYPE__tds__GetDNS (235)
/* tds:GetDNS */
struct _tds__GetDNS
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tds__GetDNSResponse
#define SOAP_TYPE__tds__GetDNSResponse (236)
/* tds:GetDNSResponse */
struct _tds__GetDNSResponse
{
	struct tt__DNSInformation *DNSInformation;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type tt:DNSInformation */
};
#endif

#ifndef SOAP_TYPE__tds__SetDNS
#define SOAP_TYPE__tds__SetDNS (239)
/* tds:SetDNS */
struct _tds__SetDNS
{
	enum xsd__boolean_ FromDHCP;	/* required element of type xsd:boolean */
	int __sizeSearchDomain;	/* sequence of elements <SearchDomain> */
	char **SearchDomain;	/* optional element of type xsd:token */
	int __sizeDNSManual;	/* sequence of elements <DNSManual> */
	struct tt__IPAddress *DNSManual;	/* optional element of type tt:IPAddress */
};
#endif

#ifndef SOAP_TYPE__tds__SetDNSResponse
#define SOAP_TYPE__tds__SetDNSResponse (243)
/* tds:SetDNSResponse */
struct _tds__SetDNSResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE___tds__GetDNS
#define SOAP_TYPE___tds__GetDNS (1380)
/* Operation wrapper: */
struct __tds__GetDNS
{
	struct _tds__GetDNS *tds__GetDNS;	/* optional element of type tds:GetDNS */
};
#endif

#ifndef SOAP_TYPE___tds__SetDNS
#define SOAP_TYPE___tds__SetDNS (1384)
/* Operation wrapper: */
struct __tds__SetDNS
{
	struct _tds__SetDNS *tds__SetDNS;	/* optional element of type tds:SetDNS */
};
#endif

#ifndef SOAP_TYPE_tt__NetworkGateway
#define SOAP_TYPE_tt__NetworkGateway (273)
/* tt:NetworkGateway */
struct tt__NetworkGateway
{
	int __sizeIPv4Address;	/* sequence of elements <IPv4Address> */
	char **IPv4Address;	/* optional element of type tt:IPv4Address */
	int __sizeIPv6Address;	/* sequence of elements <IPv6Address> */
	char **IPv6Address;	/* optional element of type tt:IPv6Address */
};
#endif

#ifndef SOAP_TYPE__tds__GetNetworkDefaultGateway
#define SOAP_TYPE__tds__GetNetworkDefaultGateway (271)
/* tds:GetNetworkDefaultGateway */
struct _tds__GetNetworkDefaultGateway
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tds__GetNetworkDefaultGatewayResponse
#define SOAP_TYPE__tds__GetNetworkDefaultGatewayResponse (272)
/* tds:GetNetworkDefaultGatewayResponse */
struct _tds__GetNetworkDefaultGatewayResponse
{
	struct tt__NetworkGateway *NetworkGateway;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type tt:NetworkGateway */
};
#endif

#ifndef SOAP_TYPE__tds__SetNetworkDefaultGateway
#define SOAP_TYPE__tds__SetNetworkDefaultGateway (275)
/* tds:SetNetworkDefaultGateway */
struct _tds__SetNetworkDefaultGateway
{
	int __sizeIPv4Address;	/* sequence of elements <IPv4Address> */
	char **IPv4Address;	/* optional element of type tt:IPv4Address */
	int __sizeIPv6Address;	/* sequence of elements <IPv6Address> */
	char **IPv6Address;	/* optional element of type tt:IPv6Address */
};
#endif

#ifndef SOAP_TYPE__tds__SetNetworkDefaultGatewayResponse
#define SOAP_TYPE__tds__SetNetworkDefaultGatewayResponse (278)
/* tds:SetNetworkDefaultGatewayResponse */
struct _tds__SetNetworkDefaultGatewayResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE___tds__GetNetworkDefaultGateway
#define SOAP_TYPE___tds__GetNetworkDefaultGateway (1420)
/* Operation wrapper: */
struct __tds__GetNetworkDefaultGateway
{
	struct _tds__GetNetworkDefaultGateway *tds__GetNetworkDefaultGateway;	/* optional element of type tds:GetNetworkDefaultGateway */
};
#endif

#ifndef SOAP_TYPE___tds__SetNetworkDefaultGateway
#define SOAP_TYPE___tds__SetNetworkDefaultGateway (1424)
/* Operation wrapper: */
struct __tds__SetNetworkDefaultGateway
{
	struct _tds__SetNetworkDefaultGateway *tds__SetNetworkDefaultGateway;	/* optional element of type tds:SetNetworkDefaultGateway */
};
#endif

#ifndef SOAP_TYPE_tt__NTPInformationExtension
#define SOAP_TYPE_tt__NTPInformationExtension (524)
/* tt:NTPInformationExtension */
struct tt__NTPInformationExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__NTPInformation
#define SOAP_TYPE_tt__NTPInformation (246)
/* tt:NTPInformation */
struct tt__NTPInformation
{
	enum xsd__boolean_ FromDHCP;	/* required element of type xsd:boolean */
	int __sizeNTPFromDHCP;	/* sequence of elements <NTPFromDHCP> */
	struct tt__NetworkHost *NTPFromDHCP;	/* optional element of type tt:NetworkHost */
	int __sizeNTPManual;	/* sequence of elements <NTPManual> */
	struct tt__NetworkHost *NTPManual;	/* optional element of type tt:NetworkHost */
	struct tt__NTPInformationExtension *Extension;	/* optional element of type tt:NTPInformationExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif


#ifndef SOAP_TYPE__tds__GetNTP
#define SOAP_TYPE__tds__GetNTP (244)
/* tds:GetNTP */
struct _tds__GetNTP
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tds__GetNTPResponse
#define SOAP_TYPE__tds__GetNTPResponse (245)
/* tds:GetNTPResponse */
struct _tds__GetNTPResponse
{
	struct tt__NTPInformation *NTPInformation;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type tt:NTPInformation */
};
#endif

#ifndef SOAP_TYPE__tds__SetNTP
#define SOAP_TYPE__tds__SetNTP (248)
/* tds:SetNTP */
struct _tds__SetNTP
{
	enum xsd__boolean_ FromDHCP;	/* required element of type xsd:boolean */
	int __sizeNTPManual;	/* sequence of elements <NTPManual> */
	struct tt__NetworkHost *NTPManual;	/* optional element of type tt:NetworkHost */
};
#endif

#ifndef SOAP_TYPE__tds__SetNTPResponse
#define SOAP_TYPE__tds__SetNTPResponse (249)
/* tds:SetNTPResponse */
struct _tds__SetNTPResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE___tds__GetNTP
#define SOAP_TYPE___tds__GetNTP (1388)
/* Operation wrapper: */
struct __tds__GetNTP
{
	struct _tds__GetNTP *tds__GetNTP;	/* optional element of type tds:GetNTP */
};
#endif

#ifndef SOAP_TYPE___tds__SetNTP
#define SOAP_TYPE___tds__SetNTP (1392)
/* Operation wrapper: */
struct __tds__SetNTP
{
	struct _tds__SetNTP *tds__SetNTP;	/* optional element of type tds:SetNTP */
};
#endif

#ifndef SOAP_TYPE_tt__DynamicDNSInformationExtension
#define SOAP_TYPE_tt__DynamicDNSInformationExtension (526)
/* tt:DynamicDNSInformationExtension */
struct tt__DynamicDNSInformationExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__DynamicDNSInformation
#define SOAP_TYPE_tt__DynamicDNSInformation (252)
/* tt:DynamicDNSInformation */
struct tt__DynamicDNSInformation
{
	enum tt__DynamicDNSType Type;	/* required element of type tt:DynamicDNSType */
	char **Name;	/* optional element of type tt:DNSName */
	char *TTL;	/* optional element of type xsd:string */
	struct tt__DynamicDNSInformationExtension *Extension;	/* optional element of type tt:DynamicDNSInformationExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE__tds__GetDynamicDNS
#define SOAP_TYPE__tds__GetDynamicDNS (250)
/* tds:GetDynamicDNS */
struct _tds__GetDynamicDNS
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tds__GetDynamicDNSResponse
#define SOAP_TYPE__tds__GetDynamicDNSResponse (251)
/* tds:GetDynamicDNSResponse */
struct _tds__GetDynamicDNSResponse
{
	struct tt__DynamicDNSInformation *DynamicDNSInformation;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type tt:DynamicDNSInformation */
};
#endif

#ifndef SOAP_TYPE__tds__SetDynamicDNS
#define SOAP_TYPE__tds__SetDynamicDNS (254)
/* tds:SetDynamicDNS */
struct _tds__SetDynamicDNS
{
	enum tt__DynamicDNSType Type;	/* required element of type tt:DynamicDNSType */
	char **Name;	/* optional element of type tt:DNSName */
	char *TTL;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE__tds__SetDynamicDNSResponse
#define SOAP_TYPE__tds__SetDynamicDNSResponse (256)
/* tds:SetDynamicDNSResponse */
struct _tds__SetDynamicDNSResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE___tds__GetDynamicDNS
#define SOAP_TYPE___tds__GetDynamicDNS (1396)
/* Operation wrapper: */
struct __tds__GetDynamicDNS
{
	struct _tds__GetDynamicDNS *tds__GetDynamicDNS;	/* optional element of type tds:GetDynamicDNS */
};
#endif

#ifndef SOAP_TYPE___tds__SetDynamicDNS
#define SOAP_TYPE___tds__SetDynamicDNS (1400)
/* Operation wrapper: */
struct __tds__SetDynamicDNS
{
	struct _tds__SetDynamicDNS *tds__SetDynamicDNS;	/* optional element of type tds:SetDynamicDNS */
};
#endif

#ifndef SOAP_TYPE_tt__HostnameInformationExtension
#define SOAP_TYPE_tt__HostnameInformationExtension (520)
/* tt:HostnameInformationExtension */
struct tt__HostnameInformationExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__HostnameInformation
#define SOAP_TYPE_tt__HostnameInformation (229)
/* tt:HostnameInformation */
struct tt__HostnameInformation
{
	enum xsd__boolean_ FromDHCP;	/* required element of type xsd:boolean */
	char *Name;	/* optional element of type xsd:token */
	struct tt__HostnameInformationExtension *Extension;	/* optional element of type tt:HostnameInformationExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif


#ifndef SOAP_TYPE__tds__GetHostname
#define SOAP_TYPE__tds__GetHostname (227)
/* tds:GetHostname */
struct _tds__GetHostname
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tds__GetHostnameResponse
#define SOAP_TYPE__tds__GetHostnameResponse (228)
/* tds:GetHostnameResponse */
struct _tds__GetHostnameResponse
{
	struct tt__HostnameInformation *HostnameInformation;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type tt:HostnameInformation */
};
#endif

#ifndef SOAP_TYPE__tds__SetHostname
#define SOAP_TYPE__tds__SetHostname (231)
/* tds:SetHostname */
struct _tds__SetHostname
{
	char *Name;	/* required element of type xsd:token */
};
#endif

#ifndef SOAP_TYPE__tds__SetHostnameResponse
#define SOAP_TYPE__tds__SetHostnameResponse (232)
/* tds:SetHostnameResponse */
struct _tds__SetHostnameResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tds__SetHostnameFromDHCP
#define SOAP_TYPE__tds__SetHostnameFromDHCP (233)
/* tds:SetHostnameFromDHCP */
struct _tds__SetHostnameFromDHCP
{
	enum xsd__boolean_ FromDHCP;	/* required element of type xsd:boolean */
};
#endif

#ifndef SOAP_TYPE__tds__SetHostnameFromDHCPResponse
#define SOAP_TYPE__tds__SetHostnameFromDHCPResponse (234)
/* tds:SetHostnameFromDHCPResponse */
struct _tds__SetHostnameFromDHCPResponse
{
	enum xsd__boolean_ RebootNeeded;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type xsd:boolean */
};
#endif

#ifndef SOAP_TYPE___tds__GetHostname
#define SOAP_TYPE___tds__GetHostname (1368)
/* Operation wrapper: */
struct __tds__GetHostname
{
	struct _tds__GetHostname *tds__GetHostname;	/* optional element of type tds:GetHostname */
};
#endif

#ifndef SOAP_TYPE___tds__SetHostname
#define SOAP_TYPE___tds__SetHostname (1372)
/* Operation wrapper: */
struct __tds__SetHostname
{
	struct _tds__SetHostname *tds__SetHostname;	/* optional element of type tds:SetHostname */
};
#endif

#ifndef SOAP_TYPE___tds__SetHostnameFromDHCP
#define SOAP_TYPE___tds__SetHostnameFromDHCP (1376)
/* Operation wrapper: */
struct __tds__SetHostnameFromDHCP
{
	struct _tds__SetHostnameFromDHCP *tds__SetHostnameFromDHCP;	/* optional element of type tds:SetHostnameFromDHCP */
};
#endif

#ifndef SOAP_TYPE_tt__IPAddressFilterExtension
#define SOAP_TYPE_tt__IPAddressFilterExtension (539)
/* tt:IPAddressFilterExtension */
struct tt__IPAddressFilterExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__IPAddressFilter
#define SOAP_TYPE_tt__IPAddressFilter (287)
/* tt:IPAddressFilter */
struct tt__IPAddressFilter
{
	enum tt__IPAddressFilterType Type;	/* required element of type tt:IPAddressFilterType */
	int __sizeIPv4Address;	/* sequence of elements <IPv4Address> */
	struct tt__PrefixedIPv4Address *IPv4Address;	/* optional element of type tt:PrefixedIPv4Address */
	int __sizeIPv6Address;	/* sequence of elements <IPv6Address> */
	struct tt__PrefixedIPv6Address *IPv6Address;	/* optional element of type tt:PrefixedIPv6Address */
	struct tt__IPAddressFilterExtension *Extension;	/* optional element of type tt:IPAddressFilterExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif


#ifndef SOAP_TYPE__tds__GetIPAddressFilter
#define SOAP_TYPE__tds__GetIPAddressFilter (285)
/* tds:GetIPAddressFilter */
struct _tds__GetIPAddressFilter
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tds__GetIPAddressFilterResponse
#define SOAP_TYPE__tds__GetIPAddressFilterResponse (286)
/* tds:GetIPAddressFilterResponse */
struct _tds__GetIPAddressFilterResponse
{
	struct tt__IPAddressFilter *IPAddressFilter;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type tt:IPAddressFilter */
};
#endif

#ifndef SOAP_TYPE__tds__SetIPAddressFilter
#define SOAP_TYPE__tds__SetIPAddressFilter (289)
/* tds:SetIPAddressFilter */
struct _tds__SetIPAddressFilter
{
	struct tt__IPAddressFilter *IPAddressFilter;	/* required element of type tt:IPAddressFilter */
};
#endif

#ifndef SOAP_TYPE__tds__SetIPAddressFilterResponse
#define SOAP_TYPE__tds__SetIPAddressFilterResponse (290)
/* tds:SetIPAddressFilterResponse */
struct _tds__SetIPAddressFilterResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tds__AddIPAddressFilter
#define SOAP_TYPE__tds__AddIPAddressFilter (291)
/* tds:AddIPAddressFilter */
struct _tds__AddIPAddressFilter
{
	struct tt__IPAddressFilter *IPAddressFilter;	/* required element of type tt:IPAddressFilter */
};
#endif

#ifndef SOAP_TYPE__tds__AddIPAddressFilterResponse
#define SOAP_TYPE__tds__AddIPAddressFilterResponse (292)
/* tds:AddIPAddressFilterResponse */
struct _tds__AddIPAddressFilterResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tds__RemoveIPAddressFilter
#define SOAP_TYPE__tds__RemoveIPAddressFilter (293)
/* tds:RemoveIPAddressFilter */
struct _tds__RemoveIPAddressFilter
{
	struct tt__IPAddressFilter *IPAddressFilter;	/* required element of type tt:IPAddressFilter */
};
#endif

#ifndef SOAP_TYPE__tds__RemoveIPAddressFilterResponse
#define SOAP_TYPE__tds__RemoveIPAddressFilterResponse (294)
/* tds:RemoveIPAddressFilterResponse */
struct _tds__RemoveIPAddressFilterResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE___tds__GetIPAddressFilter
#define SOAP_TYPE___tds__GetIPAddressFilter (1436)
/* Operation wrapper: */
struct __tds__GetIPAddressFilter
{
	struct _tds__GetIPAddressFilter *tds__GetIPAddressFilter;	/* optional element of type tds:GetIPAddressFilter */
};
#endif

#ifndef SOAP_TYPE___tds__SetIPAddressFilter
#define SOAP_TYPE___tds__SetIPAddressFilter (1440)
/* Operation wrapper: */
struct __tds__SetIPAddressFilter
{
	struct _tds__SetIPAddressFilter *tds__SetIPAddressFilter;	/* optional element of type tds:SetIPAddressFilter */
};
#endif

#ifndef SOAP_TYPE___tds__AddIPAddressFilter
#define SOAP_TYPE___tds__AddIPAddressFilter (1444)
/* Operation wrapper: */
struct __tds__AddIPAddressFilter
{
	struct _tds__AddIPAddressFilter *tds__AddIPAddressFilter;	/* optional element of type tds:AddIPAddressFilter */
};
#endif

#ifndef SOAP_TYPE___tds__RemoveIPAddressFilter
#define SOAP_TYPE___tds__RemoveIPAddressFilter (1448)
/* Operation wrapper: */
struct __tds__RemoveIPAddressFilter
{
	struct _tds__RemoveIPAddressFilter *tds__RemoveIPAddressFilter;	/* optional element of type tds:RemoveIPAddressFilter */
};
#endif


//device configuration
#ifndef SOAP_TYPE_tt__IntRectangle
#define SOAP_TYPE_tt__IntRectangle (315)
/* tt:IntRectangle */
struct tt__IntRectangle
{
	int x;	/* required attribute of type xsd:int */
	int y;	/* required attribute of type xsd:int */
	int width;	/* required attribute of type xsd:int */
	int height;	/* required attribute of type xsd:int */
};
#endif

#ifndef SOAP_TYPE_tt__VideoSourceConfiguration
#define SOAP_TYPE_tt__VideoSourceConfiguration (196)
/* tt:VideoSourceConfiguration */
struct tt__VideoSourceConfiguration
{
	char *Name;	/* required element of type tt:Name */
	int UseCount;	/* required element of type xsd:int */
	char *token;	/* required attribute of type tt:ReferenceToken */
	char *SourceToken;	/* required element of type tt:ReferenceToken */
	struct tt__IntRectangle *Bounds;	/* required element of type tt:IntRectangle */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__AudioSourceConfiguration
#define SOAP_TYPE_tt__AudioSourceConfiguration (204)
/* tt:AudioSourceConfiguration */
struct tt__AudioSourceConfiguration
{
	char *Name;	/* required element of type tt:Name */
	int UseCount;	/* required element of type xsd:int */
	char *token;	/* required attribute of type tt:ReferenceToken */
	char *SourceToken;	/* required element of type tt:ReferenceToken */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__VideoResolution
#define SOAP_TYPE_tt__VideoResolution (339)
/* tt:VideoResolution */
struct tt__VideoResolution
{
	int Width;	/* required element of type xsd:int */
	int Height;	/* required element of type xsd:int */
};
#endif

#ifndef SOAP_TYPE_tt__VideoRateControl
#define SOAP_TYPE_tt__VideoRateControl (340)
/* tt:VideoRateControl */
struct tt__VideoRateControl
{
	int FrameRateLimit;	/* required element of type xsd:int */
	int EncodingInterval;	/* required element of type xsd:int */
	int BitrateLimit;	/* required element of type xsd:int */
};
#endif

#ifndef SOAP_TYPE_tt__Mpeg4Configuration
#define SOAP_TYPE_tt__Mpeg4Configuration (341)
/* tt:Mpeg4Configuration */
struct tt__Mpeg4Configuration
{
	int GovLength;	/* required element of type xsd:int */
	enum tt__Mpeg4Profile Mpeg4Profile;	/* required element of type tt:Mpeg4Profile */
};
#endif

#ifndef SOAP_TYPE_tt__H264Configuration
#define SOAP_TYPE_tt__H264Configuration (342)
/* tt:H264Configuration */
struct tt__H264Configuration
{
	int GovLength;	/* required element of type xsd:int */
	enum tt__H264Profile H264Profile;	/* required element of type tt:H264Profile */
};
#endif

#ifndef SOAP_TYPE_tt__MulticastConfiguration
#define SOAP_TYPE_tt__MulticastConfiguration (396)
/* tt:MulticastConfiguration */
struct tt__MulticastConfiguration
{
	struct tt__IPAddress *Address;	/* required element of type tt:IPAddress */
	int Port;	/* required element of type xsd:int */
	int TTL;	/* required element of type xsd:int */
	enum xsd__boolean_ AutoStart;	/* required element of type xsd:boolean */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__VideoEncoderConfiguration
#define SOAP_TYPE_tt__VideoEncoderConfiguration (192)
/* tt:VideoEncoderConfiguration */
struct tt__VideoEncoderConfiguration
{
	char *Name;	/* required element of type tt:Name */
	int UseCount;	/* required element of type xsd:int */
	char *token;	/* required attribute of type tt:ReferenceToken */
	enum tt__VideoEncoding Encoding;	/* required element of type tt:VideoEncoding */
	struct tt__VideoResolution *Resolution;	/* required element of type tt:VideoResolution */
	float Quality;	/* required element of type xsd:float */
	struct tt__VideoRateControl *RateControl;	/* optional element of type tt:VideoRateControl */
	struct tt__Mpeg4Configuration *MPEG4;	/* optional element of type tt:Mpeg4Configuration */
	struct tt__H264Configuration *H264;	/* optional element of type tt:H264Configuration */
	struct tt__MulticastConfiguration *Multicast;	/* required element of type tt:MulticastConfiguration */
	char *SessionTimeout;	/* required element of type xsd:string */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif
#ifndef SOAP_TYPE_tt__AudioEncoderConfiguration
#define SOAP_TYPE_tt__AudioEncoderConfiguration (200)
/* tt:AudioEncoderConfiguration */
struct tt__AudioEncoderConfiguration
{
	char *Name;	/* required element of type tt:Name */
	int UseCount;	/* required element of type xsd:int */
	char *token;	/* required attribute of type tt:ReferenceToken */
	enum tt__AudioEncoding Encoding;	/* required element of type tt:AudioEncoding */
	int Bitrate;	/* required element of type xsd:int */
	int SampleRate;	/* required element of type xsd:int */
	struct tt__MulticastConfiguration *Multicast;	/* required element of type tt:MulticastConfiguration */
	char *SessionTimeout;	/* required element of type xsd:string */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__Config
#define SOAP_TYPE_tt__Config (702)
/* tt:Config */
struct tt__Config
{
	struct tt__ItemList *Parameters;	/* required element of type tt:ItemList */
	char *Name;	/* required attribute of type xsd:string */
	char *Type;	/* required attribute of type xsd:QName */
};
#endif

#ifndef SOAP_TYPE_tt__AnalyticsEngineConfigurationExtension
#define SOAP_TYPE_tt__AnalyticsEngineConfigurationExtension (805)
/* tt:AnalyticsEngineConfigurationExtension */
struct tt__AnalyticsEngineConfigurationExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__AnalyticsEngineConfiguration
#define SOAP_TYPE_tt__AnalyticsEngineConfiguration (802)
/* tt:AnalyticsEngineConfiguration */
struct tt__AnalyticsEngineConfiguration
{
	int __sizeAnalyticsModule;	/* sequence of elements <AnalyticsModule> */
	struct tt__Config *AnalyticsModule;	/* optional element of type tt:Config */
	struct tt__AnalyticsEngineConfigurationExtension *Extension;	/* optional element of type tt:AnalyticsEngineConfigurationExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__RuleEngineConfigurationExtension
#define SOAP_TYPE_tt__RuleEngineConfigurationExtension (808)
/* tt:RuleEngineConfigurationExtension */
struct tt__RuleEngineConfigurationExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__RuleEngineConfiguration
#define SOAP_TYPE_tt__RuleEngineConfiguration (807)
/* tt:RuleEngineConfiguration */
struct tt__RuleEngineConfiguration
{
	int __sizeRule;	/* sequence of elements <Rule> */
	struct tt__Config *Rule;	/* optional element of type tt:Config */
	struct tt__RuleEngineConfigurationExtension *Extension;	/* optional element of type tt:RuleEngineConfigurationExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__VideoAnalyticsConfiguration
#define SOAP_TYPE_tt__VideoAnalyticsConfiguration (208)
/* tt:VideoAnalyticsConfiguration */
struct tt__VideoAnalyticsConfiguration
{
	char *Name;	/* required element of type tt:Name */
	int UseCount;	/* required element of type xsd:int */
	char *token;	/* required attribute of type tt:ReferenceToken */
	struct tt__AnalyticsEngineConfiguration *AnalyticsEngineConfiguration;	/* required element of type tt:AnalyticsEngineConfiguration */
	struct tt__RuleEngineConfiguration *RuleEngineConfiguration;	/* required element of type tt:RuleEngineConfiguration */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__Vector2D
#define SOAP_TYPE_tt__Vector2D (667)
/* tt:Vector2D */
struct tt__Vector2D
{
	float x;	/* required attribute of type xsd:float */
	float y;	/* required attribute of type xsd:float */
	char *space;	/* optional attribute of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_tt__Vector1D
#define SOAP_TYPE_tt__Vector1D (668)
/* tt:Vector1D */
struct tt__Vector1D
{
	float x;	/* required attribute of type xsd:float */
	char *space;	/* optional attribute of type xsd:string */
};
#endif


#ifndef SOAP_TYPE_tt__PTZSpeed
#define SOAP_TYPE_tt__PTZSpeed (672)
/* tt:PTZSpeed */
struct tt__PTZSpeed
{
	struct tt__Vector2D *PanTilt;	/* optional element of type tt:Vector2D */
	struct tt__Vector1D *Zoom;	/* optional element of type tt:Vector1D */
};
#endif


#ifndef SOAP_TYPE_tt__Space2DDescription
#define SOAP_TYPE_tt__Space2DDescription (659)
/* tt:Space2DDescription */
struct tt__Space2DDescription
{
	char *URI;	/* required element of type xsd:string */
	struct tt__FloatRange *XRange;	/* required element of type tt:FloatRange */
	struct tt__FloatRange *YRange;	/* required element of type tt:FloatRange */
};
#endif

#ifndef SOAP_TYPE_tt__PanTiltLimits
#define SOAP_TYPE_tt__PanTiltLimits (658)
/* tt:PanTiltLimits */
struct tt__PanTiltLimits
{
	struct tt__Space2DDescription *Range;	/* required element of type tt:Space2DDescription */
};
#endif

#ifndef SOAP_TYPE_tt__Space1DDescription
#define SOAP_TYPE_tt__Space1DDescription (662)
/* tt:Space1DDescription */
struct tt__Space1DDescription
{
	char *URI;	/* required element of type xsd:string */
	struct tt__FloatRange *XRange;	/* required element of type tt:FloatRange */
};
#endif

#ifndef SOAP_TYPE_tt__ZoomLimits
#define SOAP_TYPE_tt__ZoomLimits (661)
/* tt:ZoomLimits */
struct tt__ZoomLimits
{
	struct tt__Space1DDescription *Range;	/* required element of type tt:Space1DDescription */
};
#endif

#ifndef SOAP_TYPE_tt__PTZConfigurationExtension
#define SOAP_TYPE_tt__PTZConfigurationExtension (653)
/* tt:PTZConfigurationExtension */
struct tt__PTZConfigurationExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__PTZConfiguration
#define SOAP_TYPE_tt__PTZConfiguration (328)
/* tt:PTZConfiguration */
struct tt__PTZConfiguration
{
	char *Name;	/* required element of type tt:Name */
	int UseCount;	/* required element of type xsd:int */
	char *token;	/* required attribute of type tt:ReferenceToken */
	char *NodeToken;	/* required element of type tt:ReferenceToken */
	char *DefaultAbsolutePantTiltPositionSpace;	/* optional element of type xsd:string */
	char *DefaultAbsoluteZoomPositionSpace;	/* optional element of type xsd:string */
	char *DefaultRelativePanTiltTranslationSpace;	/* optional element of type xsd:string */
	char *DefaultRelativeZoomTranslationSpace;	/* optional element of type xsd:string */
	char *DefaultContinuousPanTiltVelocitySpace;	/* optional element of type xsd:string */
	char *DefaultContinuousZoomVelocitySpace;	/* optional element of type xsd:string */
	struct tt__PTZSpeed *DefaultPTZSpeed;	/* optional element of type tt:PTZSpeed */
	char *DefaultPTZTimeout;	/* optional element of type xsd:string */
	struct tt__PanTiltLimits *PanTiltLimits;	/* optional element of type tt:PanTiltLimits */
	struct tt__ZoomLimits *ZoomLimits;	/* optional element of type tt:ZoomLimits */
	struct tt__PTZConfigurationExtension *Extension;	/* optional element of type tt:PTZConfigurationExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif
//PTZ
#ifndef SOAP_TYPE_tt__PTZFilter
#define SOAP_TYPE_tt__PTZFilter (367)
/* tt:PTZFilter */
struct tt__PTZFilter
{
	enum xsd__boolean_ Status;	/* required element of type xsd:boolean */
	enum xsd__boolean_ Position;	/* required element of type xsd:boolean */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

//event
#ifndef SOAP_TYPE_wsnt__FilterType
#define SOAP_TYPE_wsnt__FilterType (369)
/* wsnt:FilterType */
struct wsnt__FilterType
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE__tt__EventSubscription_SubscriptionPolicy
#define SOAP_TYPE__tt__EventSubscription_SubscriptionPolicy (371)
/* tt:EventSubscription-SubscriptionPolicy */
struct _tt__EventSubscription_SubscriptionPolicy
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__EventSubscription
#define SOAP_TYPE_tt__EventSubscription (368)
/* tt:EventSubscription */
struct tt__EventSubscription
{
	struct wsnt__FilterType *Filter;	/* optional element of type wsnt:FilterType */
	struct _tt__EventSubscription_SubscriptionPolicy *SubscriptionPolicy;	/* optional element of type tt:EventSubscription-SubscriptionPolicy */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__MetadataConfiguration
#define SOAP_TYPE_tt__MetadataConfiguration (212)
/* tt:MetadataConfiguration */
struct tt__MetadataConfiguration
{
	char *Name;	/* required element of type tt:Name */
	int UseCount;	/* required element of type xsd:int */
	char *token;	/* required attribute of type tt:ReferenceToken */
	struct tt__PTZFilter *PTZStatus;	/* optional element of type tt:PTZFilter */
	struct tt__EventSubscription *Events;	/* optional element of type tt:EventSubscription */
	enum xsd__boolean_ *Analytics;	/* optional element of type xsd:boolean */
	struct tt__MulticastConfiguration *Multicast;	/* required element of type tt:MulticastConfiguration */
	char *SessionTimeout;	/* required element of type xsd:string */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

//profile extension
#ifndef SOAP_TYPE_tt__AudioOutputConfiguration
#define SOAP_TYPE_tt__AudioOutputConfiguration (216)
/* tt:AudioOutputConfiguration */
struct tt__AudioOutputConfiguration
{
	char *Name;	/* required element of type tt:Name */
	int UseCount;	/* required element of type xsd:int */
	char *token;	/* required attribute of type tt:ReferenceToken */
	char *OutputToken;	/* required element of type tt:ReferenceToken */
	char *SendPrimacy;	/* optional element of type xsd:string */
	int OutputLevel;	/* required element of type xsd:int */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__AudioDecoderConfiguration
#define SOAP_TYPE_tt__AudioDecoderConfiguration (220)
/* tt:AudioDecoderConfiguration */
struct tt__AudioDecoderConfiguration
{
	char *Name;	/* required element of type tt:Name */
	int UseCount;	/* required element of type xsd:int */
	char *token;	/* required attribute of type tt:ReferenceToken */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__ProfileExtension2
#define SOAP_TYPE_tt__ProfileExtension2 (332)
/* tt:ProfileExtension2 */
struct tt__ProfileExtension2
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__ProfileExtension
#define SOAP_TYPE_tt__ProfileExtension (330)
/* tt:ProfileExtension */
struct tt__ProfileExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
	struct tt__AudioOutputConfiguration *AudioOutputConfiguration;	/* optional element of type tt:AudioOutputConfiguration */
	struct tt__AudioDecoderConfiguration *AudioDecoderConfiguration;	/* optional element of type tt:AudioDecoderConfiguration */
	struct tt__ProfileExtension2 *Extension;	/* optional element of type tt:ProfileExtension2 */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif


#ifndef SOAP_TYPE_tt__Profile
#define SOAP_TYPE_tt__Profile (146)
/* tt:Profile */
struct tt__Profile
{
	char *Name;	/* required element of type tt:Name */
	struct tt__VideoSourceConfiguration *VideoSourceConfiguration;	/* optional element of type tt:VideoSourceConfiguration */
	struct tt__AudioSourceConfiguration *AudioSourceConfiguration;	/* optional element of type tt:AudioSourceConfiguration */
	struct tt__VideoEncoderConfiguration *VideoEncoderConfiguration;	/* optional element of type tt:VideoEncoderConfiguration */
	struct tt__AudioEncoderConfiguration *AudioEncoderConfiguration;	/* optional element of type tt:AudioEncoderConfiguration */
	struct tt__VideoAnalyticsConfiguration *VideoAnalyticsConfiguration;	/* optional element of type tt:VideoAnalyticsConfiguration */
	struct tt__PTZConfiguration *PTZConfiguration;	/* optional element of type tt:PTZConfiguration */
	struct tt__MetadataConfiguration *MetadataConfiguration;	/* optional element of type tt:MetadataConfiguration */
	struct tt__ProfileExtension *Extension;	/* optional element of type tt:ProfileExtension */
	char *token;	/* required attribute of type tt:ReferenceToken */
	enum xsd__boolean_ *fixed;	/* optional attribute of type xsd:boolean */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE__trt__GetProfiles
#define SOAP_TYPE__trt__GetProfiles (150)
/* trt:GetProfiles */
struct _trt__GetProfiles
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__trt__GetProfilesResponse
#define SOAP_TYPE__trt__GetProfilesResponse (151)
/* trt:GetProfilesResponse */
struct _trt__GetProfilesResponse
{
	int __sizeProfiles;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* sequence of elements <Profiles> */
	struct tt__Profile *Profiles;	/* optional element of type tt:Profile */
};
#endif

#ifndef SOAP_TYPE___trt__GetProfiles
#define SOAP_TYPE___trt__GetProfiles (1218)
/* Operation wrapper: */
struct __trt__GetProfiles
{
	struct _trt__GetProfiles *trt__GetProfiles;	/* optional element of type trt:GetProfiles */
};
#endif

#ifndef SOAP_TYPE__trt__GetProfileResponse
#define SOAP_TYPE__trt__GetProfileResponse (149)
/* trt:GetProfileResponse */
struct _trt__GetProfileResponse
{
	struct tt__Profile *Profile;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type tt:Profile */
};
#endif

#ifndef SOAP_TYPE__trt__GetVideoEncoderConfiguration
#define SOAP_TYPE__trt__GetVideoEncoderConfiguration (224)
/* trt:GetVideoEncoderConfiguration */
struct _trt__GetVideoEncoderConfiguration
{
	char *ConfigurationToken;	/* required element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE__trt__GetVideoEncoderConfigurationResponse
#define SOAP_TYPE__trt__GetVideoEncoderConfigurationResponse (225)
/* trt:GetVideoEncoderConfigurationResponse */
struct _trt__GetVideoEncoderConfigurationResponse
{
	struct tt__VideoEncoderConfiguration *Configuration;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type tt:VideoEncoderConfiguration */
};
#endif

#ifndef SOAP_TYPE___trt__GetVideoEncoderConfiguration
#define SOAP_TYPE___trt__GetVideoEncoderConfiguration (1334)
/* Operation wrapper: */
struct __trt__GetVideoEncoderConfiguration
{
	struct _trt__GetVideoEncoderConfiguration *trt__GetVideoEncoderConfiguration;	/* optional element of type trt:GetVideoEncoderConfiguration */
};
#endif

#ifndef SOAP_TYPE__trt__SetVideoEncoderConfiguration
#define SOAP_TYPE__trt__SetVideoEncoderConfiguration (254)
/* trt:SetVideoEncoderConfiguration */
struct _trt__SetVideoEncoderConfiguration
{
	struct tt__VideoEncoderConfiguration *Configuration;	/* required element of type tt:VideoEncoderConfiguration */
	enum xsd__boolean_ ForcePersistence;	/* required element of type xsd:boolean */
};
#endif

#ifndef SOAP_TYPE__trt__SetVideoEncoderConfigurationResponse
#define SOAP_TYPE__trt__SetVideoEncoderConfigurationResponse (255)
/* trt:SetVideoEncoderConfigurationResponse */
struct _trt__SetVideoEncoderConfigurationResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE___trt__SetVideoEncoderConfiguration
#define SOAP_TYPE___trt__SetVideoEncoderConfiguration (1398)
/* Operation wrapper: */
struct __trt__SetVideoEncoderConfiguration
{
	struct _trt__SetVideoEncoderConfiguration *trt__SetVideoEncoderConfiguration;	/* optional element of type trt:SetVideoEncoderConfiguration */
};
#endif

//video encoder configuration options
#ifndef SOAP_TYPE_tt__JpegOptions
#define SOAP_TYPE_tt__JpegOptions (343)
/* tt:JpegOptions */
struct tt__JpegOptions
{
	int __sizeResolutionsAvailable;	/* sequence of elements <ResolutionsAvailable> */
	struct tt__VideoResolution *ResolutionsAvailable;	/* required element of type tt:VideoResolution */
	struct tt__IntRange *FrameRateRange;	/* required element of type tt:IntRange */
	struct tt__IntRange *EncodingIntervalRange;	/* required element of type tt:IntRange */
};
#endif

#ifndef SOAP_TYPE_tt__Mpeg4Options
#define SOAP_TYPE_tt__Mpeg4Options (345)
/* tt:Mpeg4Options */
struct tt__Mpeg4Options
{
	int __sizeResolutionsAvailable;	/* sequence of elements <ResolutionsAvailable> */
	struct tt__VideoResolution *ResolutionsAvailable;	/* required element of type tt:VideoResolution */
	struct tt__IntRange *GovLengthRange;	/* required element of type tt:IntRange */
	struct tt__IntRange *FrameRateRange;	/* required element of type tt:IntRange */
	struct tt__IntRange *EncodingIntervalRange;	/* required element of type tt:IntRange */
	int __sizeMpeg4ProfilesSupported;	/* sequence of elements <Mpeg4ProfilesSupported> */
	enum tt__Mpeg4Profile *Mpeg4ProfilesSupported;	/* required element of type tt:Mpeg4Profile */
};
#endif

#ifndef SOAP_TYPE_tt__H264Options
#define SOAP_TYPE_tt__H264Options (347)
/* tt:H264Options */
struct tt__H264Options
{
	int __sizeResolutionsAvailable;	/* sequence of elements <ResolutionsAvailable> */
	struct tt__VideoResolution *ResolutionsAvailable;	/* required element of type tt:VideoResolution */
	struct tt__IntRange *GovLengthRange;	/* required element of type tt:IntRange */
	struct tt__IntRange *FrameRateRange;	/* required element of type tt:IntRange */
	struct tt__IntRange *EncodingIntervalRange;	/* required element of type tt:IntRange */
	int __sizeH264ProfilesSupported;	/* sequence of elements <H264ProfilesSupported> */
	enum tt__H264Profile *H264ProfilesSupported;	/* required element of type tt:H264Profile */
};
#endif

#ifndef SOAP_TYPE_tt__JpegOptions2
#define SOAP_TYPE_tt__JpegOptions2 (351)
/* tt:JpegOptions2 */
struct tt__JpegOptions2
{
	int __sizeResolutionsAvailable;	/* sequence of elements <ResolutionsAvailable> */
	struct tt__VideoResolution *ResolutionsAvailable;	/* required element of type tt:VideoResolution */
	struct tt__IntRange *FrameRateRange;	/* required element of type tt:IntRange */
	struct tt__IntRange *EncodingIntervalRange;	/* required element of type tt:IntRange */
	struct tt__IntRange *BitrateRange;	/* required element of type tt:IntRange */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__Mpeg4Options2
#define SOAP_TYPE_tt__Mpeg4Options2 (353)
/* tt:Mpeg4Options2 */
struct tt__Mpeg4Options2
{
	int __sizeResolutionsAvailable;	/* sequence of elements <ResolutionsAvailable> */
	struct tt__VideoResolution *ResolutionsAvailable;	/* required element of type tt:VideoResolution */
	struct tt__IntRange *GovLengthRange;	/* required element of type tt:IntRange */
	struct tt__IntRange *FrameRateRange;	/* required element of type tt:IntRange */
	struct tt__IntRange *EncodingIntervalRange;	/* required element of type tt:IntRange */
	int __sizeMpeg4ProfilesSupported;	/* sequence of elements <Mpeg4ProfilesSupported> */
	enum tt__Mpeg4Profile *Mpeg4ProfilesSupported;	/* required element of type tt:Mpeg4Profile */
	struct tt__IntRange *BitrateRange;	/* required element of type tt:IntRange */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__H264Options2
#define SOAP_TYPE_tt__H264Options2 (355)
/* tt:H264Options2 */
struct tt__H264Options2
{
	int __sizeResolutionsAvailable;	/* sequence of elements <ResolutionsAvailable> */
	struct tt__VideoResolution *ResolutionsAvailable;	/* required element of type tt:VideoResolution */
	struct tt__IntRange *GovLengthRange;	/* required element of type tt:IntRange */
	struct tt__IntRange *FrameRateRange;	/* required element of type tt:IntRange */
	struct tt__IntRange *EncodingIntervalRange;	/* required element of type tt:IntRange */
	int __sizeH264ProfilesSupported;	/* sequence of elements <H264ProfilesSupported> */
	enum tt__H264Profile *H264ProfilesSupported;	/* required element of type tt:H264Profile */
	struct tt__IntRange *BitrateRange;	/* required element of type tt:IntRange */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__VideoEncoderOptionsExtension2
#define SOAP_TYPE_tt__VideoEncoderOptionsExtension2 (357)
/* tt:VideoEncoderOptionsExtension2 */
struct tt__VideoEncoderOptionsExtension2
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__VideoEncoderOptionsExtension
#define SOAP_TYPE_tt__VideoEncoderOptionsExtension (349)
/* tt:VideoEncoderOptionsExtension */
struct tt__VideoEncoderOptionsExtension
{
	struct tt__JpegOptions2 *JPEG;	/* optional element of type tt:JpegOptions2 */
	struct tt__Mpeg4Options2 *MPEG4;	/* optional element of type tt:Mpeg4Options2 */
	struct tt__H264Options2 *H264;	/* optional element of type tt:H264Options2 */
	struct tt__VideoEncoderOptionsExtension2 *Extension;	/* optional element of type tt:VideoEncoderOptionsExtension2 */
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__VideoEncoderConfigurationOptions
#define SOAP_TYPE_tt__VideoEncoderConfigurationOptions (276)
/* tt:VideoEncoderConfigurationOptions */
struct tt__VideoEncoderConfigurationOptions
{
	struct tt__IntRange *QualityRange;	/* required element of type tt:IntRange */
	struct tt__JpegOptions *JPEG;	/* optional element of type tt:JpegOptions */
	struct tt__Mpeg4Options *MPEG4;	/* optional element of type tt:Mpeg4Options */
	struct tt__H264Options *H264;	/* optional element of type tt:H264Options */
	struct tt__VideoEncoderOptionsExtension *Extension;	/* optional element of type tt:VideoEncoderOptionsExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE__trt__GetVideoEncoderConfigurationOptions
#define SOAP_TYPE__trt__GetVideoEncoderConfigurationOptions (274)
/* trt:GetVideoEncoderConfigurationOptions */
struct _trt__GetVideoEncoderConfigurationOptions
{
	char *ConfigurationToken;	/* optional element of type tt:ReferenceToken */
	char *ProfileToken;	/* optional element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE__trt__GetVideoEncoderConfigurationOptionsResponse
#define SOAP_TYPE__trt__GetVideoEncoderConfigurationOptionsResponse (275)
/* trt:GetVideoEncoderConfigurationOptionsResponse */
struct _trt__GetVideoEncoderConfigurationOptionsResponse
{
	struct tt__VideoEncoderConfigurationOptions *Options;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type tt:VideoEncoderConfigurationOptions */
};
#endif

#ifndef SOAP_TYPE___trt__GetVideoEncoderConfigurationOptions
#define SOAP_TYPE___trt__GetVideoEncoderConfigurationOptions (1430)
/* Operation wrapper: */
struct __trt__GetVideoEncoderConfigurationOptions
{
	struct _trt__GetVideoEncoderConfigurationOptions *trt__GetVideoEncoderConfigurationOptions;	/* optional element of type trt:GetVideoEncoderConfigurationOptions */
};
#endif

//device video
#ifndef SOAP_TYPE_tt__Transport
#define SOAP_TYPE_tt__Transport (399)
/* tt:Transport */
struct tt__Transport
{
	enum tt__TransportProtocol Protocol;	/* required element of type tt:TransportProtocol */
	struct tt__Transport *Tunnel;	/* optional element of type tt:Transport */
};
#endif

#ifndef SOAP_TYPE_tt__StreamSetup
#define SOAP_TYPE_tt__StreamSetup (301)
/* tt:StreamSetup */
struct tt__StreamSetup
{
	enum tt__StreamType Stream;	/* required element of type tt:StreamType */
	struct tt__Transport *Transport;	/* required element of type tt:Transport */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__MediaUri
#define SOAP_TYPE_tt__MediaUri (304)
/* tt:MediaUri */
struct tt__MediaUri
{
	char *Uri;	/* required element of type xsd:string */
	enum xsd__boolean_ InvalidAfterConnect;	/* required element of type xsd:boolean */
	enum xsd__boolean_ InvalidAfterReboot;	/* required element of type xsd:boolean */
	char *Timeout;	/* required element of type xsd:string */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif
#ifndef SOAP_TYPE__trt__GetStreamUri
#define SOAP_TYPE__trt__GetStreamUri (300)
/* trt:GetStreamUri */
struct _trt__GetStreamUri
{
	struct tt__StreamSetup *StreamSetup;	/* required element of type tt:StreamSetup */
	char *ProfileToken;	/* required element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE__trt__GetStreamUriResponse
#define SOAP_TYPE__trt__GetStreamUriResponse (303)
/* trt:GetStreamUriResponse */
struct _trt__GetStreamUriResponse
{
	struct tt__MediaUri *MediaUri;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type tt:MediaUri */
};
#endif

#ifndef SOAP_TYPE__trt__StartMulticastStreaming
#define SOAP_TYPE__trt__StartMulticastStreaming (306)
/* trt:StartMulticastStreaming */
struct _trt__StartMulticastStreaming
{
	char *ProfileToken;	/* required element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE__trt__StartMulticastStreamingResponse
#define SOAP_TYPE__trt__StartMulticastStreamingResponse (307)
/* trt:StartMulticastStreamingResponse */
struct _trt__StartMulticastStreamingResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__trt__StopMulticastStreaming
#define SOAP_TYPE__trt__StopMulticastStreaming (308)
/* trt:StopMulticastStreaming */
struct _trt__StopMulticastStreaming
{
	char *ProfileToken;	/* required element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE__trt__StopMulticastStreamingResponse
#define SOAP_TYPE__trt__StopMulticastStreamingResponse (309)
/* trt:StopMulticastStreamingResponse */
struct _trt__StopMulticastStreamingResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__trt__SetSynchronizationPoint
#define SOAP_TYPE__trt__SetSynchronizationPoint (310)
/* trt:SetSynchronizationPoint */
struct _trt__SetSynchronizationPoint
{
	char *ProfileToken;	/* required element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE__trt__SetSynchronizationPointResponse
#define SOAP_TYPE__trt__SetSynchronizationPointResponse (311)
/* trt:SetSynchronizationPointResponse */
struct _trt__SetSynchronizationPointResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__trt__GetSnapshotUri
#define SOAP_TYPE__trt__GetSnapshotUri (312)
/* trt:GetSnapshotUri */
struct _trt__GetSnapshotUri
{
	char *ProfileToken;	/* required element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE__trt__GetSnapshotUriResponse
#define SOAP_TYPE__trt__GetSnapshotUriResponse (313)
/* trt:GetSnapshotUriResponse */
struct _trt__GetSnapshotUriResponse
{
	struct tt__MediaUri *MediaUri;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type tt:MediaUri */
};
#endif

#ifndef SOAP_TYPE___trt__GetStreamUri
#define SOAP_TYPE___trt__GetStreamUri (1458)
/* Operation wrapper: */
struct __trt__GetStreamUri
{
	struct _trt__GetStreamUri *trt__GetStreamUri;	/* optional element of type trt:GetStreamUri */
};
#endif

#ifndef SOAP_TYPE___trt__StartMulticastStreaming
#define SOAP_TYPE___trt__StartMulticastStreaming (1462)
/* Operation wrapper: */
struct __trt__StartMulticastStreaming
{
	struct _trt__StartMulticastStreaming *trt__StartMulticastStreaming;	/* optional element of type trt:StartMulticastStreaming */
};
#endif

#ifndef SOAP_TYPE___trt__StopMulticastStreaming
#define SOAP_TYPE___trt__StopMulticastStreaming (1466)
/* Operation wrapper: */
struct __trt__StopMulticastStreaming
{
	struct _trt__StopMulticastStreaming *trt__StopMulticastStreaming;	/* optional element of type trt:StopMulticastStreaming */
};
#endif

#ifndef SOAP_TYPE___trt__SetSynchronizationPoint
#define SOAP_TYPE___trt__SetSynchronizationPoint (1470)
/* Operation wrapper: */
struct __trt__SetSynchronizationPoint
{
	struct _trt__SetSynchronizationPoint *trt__SetSynchronizationPoint;	/* optional element of type trt:SetSynchronizationPoint */
};
#endif

#ifndef SOAP_TYPE___trt__GetSnapshotUri
#define SOAP_TYPE___trt__GetSnapshotUri (1474)
/* Operation wrapper: */
struct __trt__GetSnapshotUri
{
	struct _trt__GetSnapshotUri *trt__GetSnapshotUri;	/* optional element of type trt:GetSnapshotUri */
};
#endif

//device imaging control
#ifndef SOAP_TYPE_tt__Rectangle
#define SOAP_TYPE_tt__Rectangle (511)
/* tt:Rectangle */
struct tt__Rectangle
{
	float *bottom;	/* optional attribute of type xsd:float */
	float *top;	/* optional attribute of type xsd:float */
	float *right;	/* optional attribute of type xsd:float */
	float *left;	/* optional attribute of type xsd:float */
};
#endif

#ifndef SOAP_TYPE_tt__BacklightCompensation20
#define SOAP_TYPE_tt__BacklightCompensation20 (548)
/* tt:BacklightCompensation20 */
struct tt__BacklightCompensation20
{
	enum tt__BacklightCompensationMode Mode;	/* required element of type tt:BacklightCompensationMode */
	float *Level;	/* optional element of type xsd:float */
};
#endif

#ifndef SOAP_TYPE_tt__Exposure20
#define SOAP_TYPE_tt__Exposure20 (550)
/* tt:Exposure20 */
struct tt__Exposure20
{
	enum tt__ExposureMode Mode;	/* required element of type tt:ExposureMode */
	enum tt__ExposurePriority *Priority;	/* optional element of type tt:ExposurePriority */
	struct tt__Rectangle *Window;	/* optional element of type tt:Rectangle */
	float *MinExposureTime;	/* optional element of type xsd:float */
	float *MaxExposureTime;	/* optional element of type xsd:float */
	float *MinGain;	/* optional element of type xsd:float */
	float *MaxGain;	/* optional element of type xsd:float */
	float *MinIris;	/* optional element of type xsd:float */
	float *MaxIris;	/* optional element of type xsd:float */
	float *ExposureTime;	/* optional element of type xsd:float */
	float *Gain;	/* optional element of type xsd:float */
	float *Iris;	/* optional element of type xsd:float */
};
#endif


#ifndef SOAP_TYPE_tt__FocusConfiguration20Extension
#define SOAP_TYPE_tt__FocusConfiguration20Extension (577)
/* tt:FocusConfiguration20Extension */
struct tt__FocusConfiguration20Extension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__FocusConfiguration20
#define SOAP_TYPE_tt__FocusConfiguration20 (552)
/* tt:FocusConfiguration20 */
struct tt__FocusConfiguration20
{
	enum tt__AutoFocusMode AutoFocusMode;	/* required element of type tt:AutoFocusMode */
	float *DefaultSpeed;	/* optional element of type xsd:float */
	float *NearLimit;	/* optional element of type xsd:float */
	float *FarLimit;	/* optional element of type xsd:float */
	struct tt__FocusConfiguration20Extension *Extension;	/* optional element of type tt:FocusConfiguration20Extension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__WideDynamicRange20
#define SOAP_TYPE_tt__WideDynamicRange20 (554)
/* tt:WideDynamicRange20 */
struct tt__WideDynamicRange20
{
	enum tt__WideDynamicMode Mode;	/* required element of type tt:WideDynamicMode */
	float *Level;	/* optional element of type xsd:float */
};
#endif

#ifndef SOAP_TYPE_tt__WhiteBalance20Extension
#define SOAP_TYPE_tt__WhiteBalance20Extension (575)
/* tt:WhiteBalance20Extension */
struct tt__WhiteBalance20Extension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif
#ifndef SOAP_TYPE_tt__WhiteBalance20
#define SOAP_TYPE_tt__WhiteBalance20 (556)
/* tt:WhiteBalance20 */
struct tt__WhiteBalance20
{
	enum tt__WhiteBalanceMode Mode;	/* required element of type tt:WhiteBalanceMode */
	float *CrGain;	/* optional element of type xsd:float */
	float *CbGain;	/* optional element of type xsd:float */
	struct tt__WhiteBalance20Extension *Extension;	/* optional element of type tt:WhiteBalance20Extension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__ImagingSettingsExtension20
#define SOAP_TYPE_tt__ImagingSettingsExtension20 (558)
/* tt:ImagingSettingsExtension20 */
struct tt__ImagingSettingsExtension20
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__ImagingSettings20
#define SOAP_TYPE_tt__ImagingSettings20 (128)
/* tt:ImagingSettings20 */
struct tt__ImagingSettings20
{
	struct tt__BacklightCompensation20 *BacklightCompensation;	/* optional element of type tt:BacklightCompensation20 */
	float *Brightness;	/* optional element of type xsd:float */
	float *ColorSaturation;	/* optional element of type xsd:float */
	float *Contrast;	/* optional element of type xsd:float */
	struct tt__Exposure20 *Exposure;	/* optional element of type tt:Exposure20 */
	struct tt__FocusConfiguration20 *Focus;	/* optional element of type tt:FocusConfiguration20 */
	enum tt__IrCutFilterMode *IrCutFilter;	/* optional element of type tt:IrCutFilterMode */
	float *Sharpness;	/* optional element of type xsd:float */
	struct tt__WideDynamicRange20 *WideDynamicRange;	/* optional element of type tt:WideDynamicRange20 */
	struct tt__WhiteBalance20 *WhiteBalance;	/* optional element of type tt:WhiteBalance20 */
	struct tt__ImagingSettingsExtension20 *Extension;	/* optional element of type tt:ImagingSettingsExtension20 */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE__timg__GetImagingSettingsResponse
#define SOAP_TYPE__timg__GetImagingSettingsResponse (127)
/* timg:GetImagingSettingsResponse */
struct _timg__GetImagingSettingsResponse
{
	struct tt__ImagingSettings20 *ImagingSettings;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type tt:ImagingSettings20 */
};
#endif

#ifndef SOAP_TYPE__timg__GetImagingSettings
#define SOAP_TYPE__timg__GetImagingSettings (126)
/* timg:GetImagingSettings */
struct _timg__GetImagingSettings
{
	char *VideoSourceToken;	/* required element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE___timg__GetImagingSettings
#define SOAP_TYPE___timg__GetImagingSettings (1062)
/* Operation wrapper: */
struct __timg__GetImagingSettings
{
	struct _timg__GetImagingSettings *timg__GetImagingSettings;	/* optional element of type timg:GetImagingSettings */
};
#endif
#ifndef SOAP_TYPE__timg__SetImagingSettings
#define SOAP_TYPE__timg__SetImagingSettings (130)
/* timg:SetImagingSettings */
struct _timg__SetImagingSettings
{
	char *VideoSourceToken;	/* required element of type tt:ReferenceToken */
	struct tt__ImagingSettings20 *ImagingSettings;	/* required element of type tt:ImagingSettings20 */
	enum xsd__boolean_ *ForcePersistence_x0020;	/* optional element of type xsd:boolean */
};
#endif

#ifndef SOAP_TYPE__timg__SetImagingSettingsResponse
#define SOAP_TYPE__timg__SetImagingSettingsResponse (132)
/* timg:SetImagingSettingsResponse */
struct _timg__SetImagingSettingsResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE___timg__SetImagingSettings
#define SOAP_TYPE___timg__SetImagingSettings (1066)
/* Operation wrapper: */
struct __timg__SetImagingSettings
{
	struct _timg__SetImagingSettings *timg__SetImagingSettings;	/* optional element of type timg:SetImagingSettings */
};
#endif

#ifndef SOAP_TYPE_tt__FloatRange
#define SOAP_TYPE_tt__FloatRange (156)
/* tt:FloatRange */
struct tt__FloatRange
{
	float Min;	/* required element of type xsd:float */
	float Max;	/* required element of type xsd:float */
};
#endif

#ifndef SOAP_TYPE_tt__BacklightCompensationOptions20
#define SOAP_TYPE_tt__BacklightCompensationOptions20 (560)
/* tt:BacklightCompensationOptions20 */
struct tt__BacklightCompensationOptions20
{
	int __sizeMode;	/* sequence of elements <Mode> */
	enum tt__BacklightCompensationMode *Mode;	/* required element of type tt:BacklightCompensationMode */
	struct tt__FloatRange *Level;	/* optional element of type tt:FloatRange */
};
#endif
#ifndef SOAP_TYPE_tt__ExposureOptions20
#define SOAP_TYPE_tt__ExposureOptions20 (562)
/* tt:ExposureOptions20 */
struct tt__ExposureOptions20
{
	int __sizeMode;	/* sequence of elements <Mode> */
	enum tt__ExposureMode *Mode;	/* required element of type tt:ExposureMode */
	int __sizePriority;	/* sequence of elements <Priority> */
	enum tt__ExposurePriority *Priority;	/* optional element of type tt:ExposurePriority */
	struct tt__FloatRange *MinExposureTime;	/* optional element of type tt:FloatRange */
	struct tt__FloatRange *MaxExposureTime;	/* optional element of type tt:FloatRange */
	struct tt__FloatRange *MinGain;	/* optional element of type tt:FloatRange */
	struct tt__FloatRange *MaxGain;	/* optional element of type tt:FloatRange */
	struct tt__FloatRange *MinIris;	/* optional element of type tt:FloatRange */
	struct tt__FloatRange *MaxIris;	/* optional element of type tt:FloatRange */
	struct tt__FloatRange *ExposureTime;	/* optional element of type tt:FloatRange */
	struct tt__FloatRange *Gain;	/* optional element of type tt:FloatRange */
	struct tt__FloatRange *Iris;	/* optional element of type tt:FloatRange */
};
#endif

#ifndef SOAP_TYPE_tt__FocusOptions20Extension
#define SOAP_TYPE_tt__FocusOptions20Extension (581)
/* tt:FocusOptions20Extension */
struct tt__FocusOptions20Extension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__FocusOptions20
#define SOAP_TYPE_tt__FocusOptions20 (564)
/* tt:FocusOptions20 */
struct tt__FocusOptions20
{
	int __sizeAutoFocusModes;	/* sequence of elements <AutoFocusModes> */
	enum tt__AutoFocusMode *AutoFocusModes;	/* optional element of type tt:AutoFocusMode */
	struct tt__FloatRange *DefaultSpeed;	/* optional element of type tt:FloatRange */
	struct tt__FloatRange *NearLimit;	/* optional element of type tt:FloatRange */
	struct tt__FloatRange *FarLimit;	/* optional element of type tt:FloatRange */
	struct tt__FocusOptions20Extension *Extension;	/* optional element of type tt:FocusOptions20Extension */
};
#endif

#ifndef SOAP_TYPE_tt__WideDynamicRangeOptions20
#define SOAP_TYPE_tt__WideDynamicRangeOptions20 (566)
/* tt:WideDynamicRangeOptions20 */
struct tt__WideDynamicRangeOptions20
{
	int __sizeMode;	/* sequence of elements <Mode> */
	enum tt__WideDynamicMode *Mode;	/* required element of type tt:WideDynamicMode */
	struct tt__FloatRange *Level;	/* optional element of type tt:FloatRange */
};
#endif

#ifndef SOAP_TYPE_tt__WhiteBalanceOptions20Extension
#define SOAP_TYPE_tt__WhiteBalanceOptions20Extension (579)
/* tt:WhiteBalanceOptions20Extension */
struct tt__WhiteBalanceOptions20Extension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__WhiteBalanceOptions20
#define SOAP_TYPE_tt__WhiteBalanceOptions20 (568)
/* tt:WhiteBalanceOptions20 */
struct tt__WhiteBalanceOptions20
{
	int __sizeMode;	/* sequence of elements <Mode> */
	enum tt__WhiteBalanceMode *Mode;	/* required element of type tt:WhiteBalanceMode */
	struct tt__FloatRange *YrGain;	/* optional element of type tt:FloatRange */
	struct tt__FloatRange *YbGain;	/* optional element of type tt:FloatRange */
	struct tt__WhiteBalanceOptions20Extension *Extension;	/* optional element of type tt:WhiteBalanceOptions20Extension */
};
#endif

#ifndef SOAP_TYPE_tt__ImagingOptions20Extension
#define SOAP_TYPE_tt__ImagingOptions20Extension (570)
/* tt:ImagingOptions20Extension */
struct tt__ImagingOptions20Extension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__ImagingOptions20
#define SOAP_TYPE_tt__ImagingOptions20 (135)
/* tt:ImagingOptions20 */
struct tt__ImagingOptions20
{
	struct tt__BacklightCompensationOptions20 *BacklightCompensation;	/* optional element of type tt:BacklightCompensationOptions20 */
	struct tt__FloatRange *Brightness;	/* optional element of type tt:FloatRange */
	struct tt__FloatRange *ColorSaturation;	/* optional element of type tt:FloatRange */
	struct tt__FloatRange *Contrast;	/* optional element of type tt:FloatRange */
	struct tt__ExposureOptions20 *Exposure;	/* optional element of type tt:ExposureOptions20 */
	struct tt__FocusOptions20 *Focus;	/* optional element of type tt:FocusOptions20 */
	int __sizeIrCutFilterModes;	/* sequence of elements <IrCutFilterModes> */
	enum tt__IrCutFilterMode *IrCutFilterModes;	/* optional element of type tt:IrCutFilterMode */
	struct tt__FloatRange *Sharpness;	/* optional element of type tt:FloatRange */
	struct tt__WideDynamicRangeOptions20 *WideDynamicRange;	/* optional element of type tt:WideDynamicRangeOptions20 */
	struct tt__WhiteBalanceOptions20 *WhiteBalance;	/* optional element of type tt:WhiteBalanceOptions20 */
	struct tt__ImagingOptions20Extension *Extension;	/* optional element of type tt:ImagingOptions20Extension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE__timg__GetOptions
#define SOAP_TYPE__timg__GetOptions (133)
/* timg:GetOptions */
struct _timg__GetOptions
{
	char *VideoSourceToken;	/* required element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE__timg__GetOptionsResponse
#define SOAP_TYPE__timg__GetOptionsResponse (134)
/* timg:GetOptionsResponse */
struct _timg__GetOptionsResponse
{
	struct tt__ImagingOptions20 *ImagingOptions;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type tt:ImagingOptions20 */
};
#endif

#ifndef SOAP_TYPE___timg__GetOptions
#define SOAP_TYPE___timg__GetOptions (1070)
/* Operation wrapper: */
struct __timg__GetOptions
{
	struct _timg__GetOptions *timg__GetOptions;	/* optional element of type timg:GetOptions */
};
#endif

//device IO control
#ifndef SOAP_TYPE__tds__GetRelayOutputs
#define SOAP_TYPE__tds__GetRelayOutputs (350)
/* tds:GetRelayOutputs */
struct _tds__GetRelayOutputs
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tds__GetRelayOutputsResponse
#define SOAP_TYPE__tds__GetRelayOutputsResponse (351)
/* tds:GetRelayOutputsResponse */
struct _tds__GetRelayOutputsResponse
{
	int __sizeRelayOutputs;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* sequence of elements <RelayOutputs> */
	struct tt__RelayOutput *RelayOutputs;	/* optional element of type tt:RelayOutput */
};
#endif

#ifndef SOAP_TYPE_tt__RelayOutput
#define SOAP_TYPE_tt__RelayOutput (352)
/* tt:RelayOutput */
struct tt__RelayOutput
{
	char *token;	/* required attribute of type tt:ReferenceToken */
	struct tt__RelayOutputSettings *Properties;	/* required element of type tt:RelayOutputSettings */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE__tds__SetRelayOutputSettings
#define SOAP_TYPE__tds__SetRelayOutputSettings (354)
/* tds:SetRelayOutputSettings */
struct _tds__SetRelayOutputSettings
{
	char *RelayOutputToken;	/* required element of type tt:ReferenceToken */
	struct tt__RelayOutputSettings *Properties;	/* required element of type tt:RelayOutputSettings */
};
#endif

#ifndef SOAP_TYPE_tt__RelayOutputSettings
#define SOAP_TYPE_tt__RelayOutputSettings (355)
/* tt:RelayOutputSettings */
struct tt__RelayOutputSettings
{
	enum tt__RelayMode Mode;	/* required element of type tt:RelayMode */
	char *DelayTime;	/* required element of type xsd:string */
	enum tt__RelayIdleState IdleState;	/* required element of type tt:RelayIdleState */
};
#endif

#ifndef SOAP_TYPE__tds__SetRelayOutputSettingsResponse
#define SOAP_TYPE__tds__SetRelayOutputSettingsResponse (357)
/* tds:SetRelayOutputSettingsResponse */
struct _tds__SetRelayOutputSettingsResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tds__SetRelayOutputState
#define SOAP_TYPE__tds__SetRelayOutputState (358)
/* tds:SetRelayOutputState */
struct _tds__SetRelayOutputState
{
	char *RelayOutputToken;	/* required element of type tt:ReferenceToken */
	enum tt__RelayLogicalState LogicalState;	/* required element of type tt:RelayLogicalState */
};
#endif

#ifndef SOAP_TYPE__tds__SetRelayOutputStateResponse
#define SOAP_TYPE__tds__SetRelayOutputStateResponse (359)
/* tds:SetRelayOutputStateResponse */
struct _tds__SetRelayOutputStateResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tds__SendAuxiliaryCommand
#define SOAP_TYPE__tds__SendAuxiliaryCommand (360)
/* tds:SendAuxiliaryCommand */
struct _tds__SendAuxiliaryCommand
{
	char *AuxiliaryCommand;	/* required element of type tt:AuxiliaryData */
};
#endif

#ifndef SOAP_TYPE__tds__SendAuxiliaryCommandResponse
#define SOAP_TYPE__tds__SendAuxiliaryCommandResponse (361)
/* tds:SendAuxiliaryCommandResponse */
struct _tds__SendAuxiliaryCommandResponse
{
	char *AuxiliaryCommandResponse;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* optional element of type tt:AuxiliaryData */
};
#endif


#ifndef SOAP_TYPE___tds__GetRelayOutputs
#define SOAP_TYPE___tds__GetRelayOutputs (1496)
/* Operation wrapper: */
struct __tds__GetRelayOutputs
{
	struct _tds__GetRelayOutputs *tds__GetRelayOutputs;	/* optional element of type tds:GetRelayOutputs */
};
#endif

#ifndef SOAP_TYPE___tds__SetRelayOutputSettings
#define SOAP_TYPE___tds__SetRelayOutputSettings (1500)
/* Operation wrapper: */
struct __tds__SetRelayOutputSettings
{
	struct _tds__SetRelayOutputSettings *tds__SetRelayOutputSettings;	/* optional element of type tds:SetRelayOutputSettings */
};
#endif

#ifndef SOAP_TYPE___tds__SetRelayOutputState
#define SOAP_TYPE___tds__SetRelayOutputState (1504)
/* Operation wrapper: */
struct __tds__SetRelayOutputState
{
	struct _tds__SetRelayOutputState *tds__SetRelayOutputState;	/* optional element of type tds:SetRelayOutputState */
};
#endif

#ifndef SOAP_TYPE___tds__SendAuxiliaryCommand
#define SOAP_TYPE___tds__SendAuxiliaryCommand (1508)
/* Operation wrapper: */
struct __tds__SendAuxiliaryCommand
{
	struct _tds__SendAuxiliaryCommand *tds__SendAuxiliaryCommand;	/* optional element of type tds:SendAuxiliaryCommand */
};
#endif

//device alarm

/******************************************************************************\
*                                                                            *
* Typedefs                                                                   *
*                                                                            *
\******************************************************************************/
#ifndef SOAP_TYPE__QName
#define SOAP_TYPE__QName (5)
typedef char *_QName;
#endif

#ifndef SOAP_TYPE_xsd__token
#define SOAP_TYPE_xsd__token (46)
typedef char *xsd__token;
#endif

#ifndef SOAP_TYPE__xmime__contentType
#define SOAP_TYPE__xmime__contentType (47)
typedef char *_xmime__contentType;
#endif

#ifndef SOAP_TYPE_tt__ReferenceToken
#define SOAP_TYPE_tt__ReferenceToken (50)
typedef char *tt__ReferenceToken;
#endif

#ifndef SOAP_TYPE_tt__Name
#define SOAP_TYPE_tt__Name (51)
typedef char *tt__Name;
#endif

#ifndef SOAP_TYPE_tt__IPv4Address
#define SOAP_TYPE_tt__IPv4Address (67)
typedef char *tt__IPv4Address;
#endif

#ifndef SOAP_TYPE_tt__IPv6Address
#define SOAP_TYPE_tt__IPv6Address (68)
typedef char *tt__IPv6Address;
#endif

#ifndef SOAP_TYPE__XML
#define SOAP_TYPE__XML (6)
typedef char *_XML;
#endif

#ifndef SOAP_TYPE__wsa__EndpointReference
#define SOAP_TYPE__wsa__EndpointReference (19)
typedef struct wsa__EndpointReferenceType _wsa__EndpointReference;
#endif

#ifndef SOAP_TYPE__wsa__MessageID
#define SOAP_TYPE__wsa__MessageID (20)
typedef char *_wsa__MessageID;
#endif

#ifndef SOAP_TYPE__wsa__RelatesTo
#define SOAP_TYPE__wsa__RelatesTo (21)
typedef struct wsa__Relationship _wsa__RelatesTo;
#endif

#ifndef SOAP_TYPE__wsa__To
#define SOAP_TYPE__wsa__To (22)
typedef char *_wsa__To;
#endif

#ifndef SOAP_TYPE__wsa__Action
#define SOAP_TYPE__wsa__Action (23)
typedef char *_wsa__Action;
#endif

#ifndef SOAP_TYPE__wsa__From
#define SOAP_TYPE__wsa__From (24)
typedef struct wsa__EndpointReferenceType _wsa__From;
#endif

#ifndef SOAP_TYPE__wsa__ReplyTo
#define SOAP_TYPE__wsa__ReplyTo (25)
typedef struct wsa__EndpointReferenceType _wsa__ReplyTo;
#endif

#ifndef SOAP_TYPE__wsa__FaultTo
#define SOAP_TYPE__wsa__FaultTo (26)
typedef struct wsa__EndpointReferenceType _wsa__FaultTo;
#endif

#ifndef SOAP_TYPE__wsa__ReplyAfter
#define SOAP_TYPE__wsa__ReplyAfter (28)
typedef unsigned int _wsa__ReplyAfter;
#endif

#ifndef SOAP_TYPE_xsd__IDREFS
#define SOAP_TYPE_xsd__IDREFS (34)
typedef char *xsd__IDREFS;
#endif

#ifndef SOAP_TYPE_d__OpenRelationshipType
#define SOAP_TYPE_d__OpenRelationshipType (35)
typedef char *d__OpenRelationshipType;
#endif

#ifndef SOAP_TYPE_d__QNameListType
#define SOAP_TYPE_d__QNameListType (36)
typedef char *d__QNameListType;
#endif

#ifndef SOAP_TYPE_d__UriListType
#define SOAP_TYPE_d__UriListType (37)
typedef char *d__UriListType;
#endif

#ifndef SOAP_TYPE_d__FaultCodeOpenType
#define SOAP_TYPE_d__FaultCodeOpenType (38)
typedef char *d__FaultCodeOpenType;
#endif

#ifndef SOAP_TYPE_tt__Dot11SSIDType
#define SOAP_TYPE_tt__Dot11SSIDType (75)
typedef struct xsd__hexBinary tt__Dot11SSIDType;
#endif

#ifndef SOAP_TYPE_tt__Dot11PSK
#define SOAP_TYPE_tt__Dot11PSK (79)
typedef struct xsd__hexBinary tt__Dot11PSK;
#endif

#ifndef SOAP_TYPE_tt__Dot11PSKPassphrase
#define SOAP_TYPE_tt__Dot11PSKPassphrase (80)
typedef char *tt__Dot11PSKPassphrase;
#endif

#ifndef SOAP_TYPE_tt__DNSName
#define SOAP_TYPE_tt__DNSName (71)
typedef char *tt__DNSName;
#endif


//device IO
#ifndef SOAP_TYPE_tt__AuxiliaryData
#define SOAP_TYPE_tt__AuxiliaryData (91)
typedef char *tt__AuxiliaryData;
#endif
/******************************************************************************\
*                                                                            *
* Externals                                                                  *
*                                                                            *
\******************************************************************************/

/******************************************************************************\
*                                                                            *
* Client-Side Call Stubs                                                     *
*                                                                            *
\******************************************************************************/
SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetCapabilities(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetCapabilities *tds__GetCapabilities, struct _tds__GetCapabilitiesResponse *tds__GetCapabilitiesResponse);

//device system managment
SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetDeviceInformation(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetDeviceInformation *tds__GetDeviceInformation, struct _tds__GetDeviceInformationResponse *tds__GetDeviceInformationResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetSystemDateAndTime(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetSystemDateAndTime *tds__GetSystemDateAndTime, struct _tds__GetSystemDateAndTimeResponse *tds__GetSystemDateAndTimeResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetSystemDateAndTime(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetSystemDateAndTime *tds__SetSystemDateAndTime, struct _tds__SetSystemDateAndTimeResponse *tds__SetSystemDateAndTimeResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SystemReboot(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SystemReboot *tds__SystemReboot, struct _tds__SystemRebootResponse *tds__SystemRebootResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetSystemFactoryDefault(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetSystemFactoryDefault *tds__SetSystemFactoryDefault, struct _tds__SetSystemFactoryDefaultResponse *tds__SetSystemFactoryDefaultResponse);

//device security

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetUsers(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetUsers *tds__GetUsers, struct _tds__GetUsersResponse *tds__GetUsersResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__CreateUsers(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__CreateUsers *tds__CreateUsers, struct _tds__CreateUsersResponse *tds__CreateUsersResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__DeleteUsers(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__DeleteUsers *tds__DeleteUsers, struct _tds__DeleteUsersResponse *tds__DeleteUsersResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetUser(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetUser *tds__SetUser, struct _tds__SetUserResponse *tds__SetUserResponse);

//device network
SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetNetworkInterfaces(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetNetworkInterfaces *tds__GetNetworkInterfaces, struct _tds__GetNetworkInterfacesResponse *tds__GetNetworkInterfacesResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetNetworkInterfaces(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetNetworkInterfaces *tds__SetNetworkInterfaces, struct _tds__SetNetworkInterfacesResponse *tds__SetNetworkInterfacesResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetNetworkProtocols(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetNetworkProtocols *tds__GetNetworkProtocols, struct _tds__GetNetworkProtocolsResponse *tds__GetNetworkProtocolsResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetNetworkProtocols(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetNetworkProtocols *tds__SetNetworkProtocols, struct _tds__SetNetworkProtocolsResponse *tds__SetNetworkProtocolsResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetNetworkDefaultGateway(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetNetworkDefaultGateway *tds__GetNetworkDefaultGateway, struct _tds__GetNetworkDefaultGatewayResponse *tds__GetNetworkDefaultGatewayResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetNetworkDefaultGateway(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetNetworkDefaultGateway *tds__SetNetworkDefaultGateway, struct _tds__SetNetworkDefaultGatewayResponse *tds__SetNetworkDefaultGatewayResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetNTP(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetNTP *tds__GetNTP, struct _tds__GetNTPResponse *tds__GetNTPResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetNTP(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetNTP *tds__SetNTP, struct _tds__SetNTPResponse *tds__SetNTPResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetDNS(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetDNS *tds__GetDNS, struct _tds__GetDNSResponse *tds__GetDNSResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetDNS(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetDNS *tds__SetDNS, struct _tds__SetDNSResponse *tds__SetDNSResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetDynamicDNS(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetDynamicDNS *tds__GetDynamicDNS, struct _tds__GetDynamicDNSResponse *tds__GetDynamicDNSResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetDynamicDNS(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetDynamicDNS *tds__SetDynamicDNS, struct _tds__SetDynamicDNSResponse *tds__SetDynamicDNSResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetHostname(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetHostname *tds__GetHostname, struct _tds__GetHostnameResponse *tds__GetHostnameResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetHostname(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetHostname *tds__SetHostname, struct _tds__SetHostnameResponse *tds__SetHostnameResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetHostnameFromDHCP(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetHostnameFromDHCP *tds__SetHostnameFromDHCP, struct _tds__SetHostnameFromDHCPResponse *tds__SetHostnameFromDHCPResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetIPAddressFilter(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetIPAddressFilter *tds__GetIPAddressFilter, struct _tds__GetIPAddressFilterResponse *tds__GetIPAddressFilterResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetIPAddressFilter(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetIPAddressFilter *tds__SetIPAddressFilter, struct _tds__SetIPAddressFilterResponse *tds__SetIPAddressFilterResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__AddIPAddressFilter(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__AddIPAddressFilter *tds__AddIPAddressFilter, struct _tds__AddIPAddressFilterResponse *tds__AddIPAddressFilterResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__RemoveIPAddressFilter(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__RemoveIPAddressFilter *tds__RemoveIPAddressFilter, struct _tds__RemoveIPAddressFilterResponse *tds__RemoveIPAddressFilterResponse);

//device configuration
SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetProfiles(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetProfiles *trt__GetProfiles, struct _trt__GetProfilesResponse *trt__GetProfilesResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetVideoEncoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetVideoEncoderConfiguration *trt__GetVideoEncoderConfiguration, struct _trt__GetVideoEncoderConfigurationResponse *trt__GetVideoEncoderConfigurationResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__SetVideoEncoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__SetVideoEncoderConfiguration *trt__SetVideoEncoderConfiguration, struct _trt__SetVideoEncoderConfigurationResponse *trt__SetVideoEncoderConfigurationResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetVideoEncoderConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetVideoEncoderConfigurationOptions *trt__GetVideoEncoderConfigurationOptions, struct _trt__GetVideoEncoderConfigurationOptionsResponse *trt__GetVideoEncoderConfigurationOptionsResponse);

//device video
SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetStreamUri(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetStreamUri *trt__GetStreamUri, struct _trt__GetStreamUriResponse *trt__GetStreamUriResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__StartMulticastStreaming(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__StartMulticastStreaming *trt__StartMulticastStreaming, struct _trt__StartMulticastStreamingResponse *trt__StartMulticastStreamingResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__StopMulticastStreaming(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__StopMulticastStreaming *trt__StopMulticastStreaming, struct _trt__StopMulticastStreamingResponse *trt__StopMulticastStreamingResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__SetSynchronizationPoint(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__SetSynchronizationPoint *trt__SetSynchronizationPoint, struct _trt__SetSynchronizationPointResponse *trt__SetSynchronizationPointResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetSnapshotUri(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetSnapshotUri *trt__GetSnapshotUri, struct _trt__GetSnapshotUriResponse *trt__GetSnapshotUriResponse);

//device control
//device imaging
SOAP_FMAC5 int SOAP_FMAC6 soap_call___timg__GetImagingSettings(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__GetImagingSettings *timg__GetImagingSettings, struct _timg__GetImagingSettingsResponse *timg__GetImagingSettingsResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___timg__SetImagingSettings(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__SetImagingSettings *timg__SetImagingSettings, struct _timg__SetImagingSettingsResponse *timg__SetImagingSettingsResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___timg__GetOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__GetOptions *timg__GetOptions, struct _timg__GetOptionsResponse *timg__GetOptionsResponse);

//device IO
SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetRelayOutputs(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetRelayOutputs *tds__GetRelayOutputs, struct _tds__GetRelayOutputsResponse *tds__GetRelayOutputsResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetRelayOutputSettings(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetRelayOutputSettings *tds__SetRelayOutputSettings, struct _tds__SetRelayOutputSettingsResponse *tds__SetRelayOutputSettingsResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetRelayOutputState(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetRelayOutputState *tds__SetRelayOutputState, struct _tds__SetRelayOutputStateResponse *tds__SetRelayOutputStateResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SendAuxiliaryCommand(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SendAuxiliaryCommand *tds__SendAuxiliaryCommand, struct _tds__SendAuxiliaryCommandResponse *tds__SendAuxiliaryCommandResponse);


//device alarm


#ifdef __cplusplus
}
#endif

#endif