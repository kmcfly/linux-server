#ifndef OnvifPtzStub_H
#define OnvifPtzStub_H

#define SOAP_WSA_2005
#define SOAP_NAMESPACE_OF_tptz	"http://www.onvif.org/ver20/ptz/wsdl"
#define SOAP_NAMESPACE_OF_tt	"http://www.onvif.org/ver10/schema"
#define SOAP_NAMESPACE_OF_wsnt	"http://docs.oasis-open.org/wsn/b-2"
#define SOAP_NAMESPACE_OF_xop	"http://www.w3.org/2004/08/xop/include"
#define SOAP_NAMESPACE_OF_ns1	"http://docs.oasis-open.org/wsrf/bf-2"
#define SOAP_NAMESPACE_OF_wstop	"http://docs.oasis-open.org/wsn/t-1"

//#include "stdsoap2.h"
#include "OnvifSoapH.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************\
 *                                                                            *
 * Enumerations                                                               *
 *                                                                            *
\******************************************************************************/

#ifndef SOAP_TYPE_tt__MoveStatus
#define SOAP_TYPE_tt__MoveStatus (91)
	/* tt:MoveStatus */
	enum tt__MoveStatus {tt__MoveStatus__IDLE = 0, tt__MoveStatus__MOVING = 1, tt__MoveStatus__UNKNOWN = 2};
#endif

#ifndef SOAP_TYPE_tt__PTZPresetTourState
#define SOAP_TYPE_tt__PTZPresetTourState (181)
/* tt:PTZPresetTourState */
enum tt__PTZPresetTourState {tt__PTZPresetTourState__Idle = 0, tt__PTZPresetTourState__Touring = 1, tt__PTZPresetTourState__Paused = 2, tt__PTZPresetTourState__Extended = 3};
#endif

#ifndef SOAP_TYPE_tt__PTZPresetTourDirection
#define SOAP_TYPE_tt__PTZPresetTourDirection (182)
/* tt:PTZPresetTourDirection */
enum tt__PTZPresetTourDirection {tt__PTZPresetTourDirection__Forward = 0, tt__PTZPresetTourDirection__Backward = 1, tt__PTZPresetTourDirection__Extended = 2};
#endif

#ifndef SOAP_TYPE_tt__PTZPresetTourOperation
#define SOAP_TYPE_tt__PTZPresetTourOperation (183)
/* tt:PTZPresetTourOperation */
enum tt__PTZPresetTourOperation {tt__PTZPresetTourOperation__Start = 0, tt__PTZPresetTourOperation__Stop = 1, tt__PTZPresetTourOperation__Pause = 2, tt__PTZPresetTourOperation__Extended = 3};
#endif

#ifndef SOAP_TYPE_tt__PTZPresetTourSupported
#define SOAP_TYPE_tt__PTZPresetTourSupported (767)
/* tt:PTZPresetTourSupported */
struct tt__PTZPresetTourSupported
{
	int MaximumNumberOfPresetTours;	/* required element of type xsd:int */
	int __sizePTZPresetTourOperation;	/* sequence of elements <PTZPresetTourOperation> */
	enum tt__PTZPresetTourOperation *PTZPresetTourOperation;	/* optional element of type tt:PTZPresetTourOperation */
	struct tt__PTZPresetTourSupportedExtension *Extension;	/* optional element of type tt:PTZPresetTourSupportedExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__PTZPresetTourSupportedExtension
#define SOAP_TYPE_tt__PTZPresetTourSupportedExtension (772)
/* tt:PTZPresetTourSupportedExtension */
struct tt__PTZPresetTourSupportedExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__PresetTour
#define SOAP_TYPE_tt__PresetTour (826)
/* tt:PresetTour */
struct tt__PresetTour
{
	char *Name;	/* optional element of type tt:Name */
	struct tt__PTZPresetTourStatus *Status;	/* required element of type tt:PTZPresetTourStatus */
	enum xsd__boolean_ AutoStart;	/* required element of type xsd:boolean */
	struct tt__PTZPresetTourStartingCondition *StartingCondition;	/* required element of type tt:PTZPresetTourStartingCondition */
	int __sizeTourSpot;	/* sequence of elements <TourSpot> */
	struct tt__PTZPresetTourSpot *TourSpot;	/* optional element of type tt:PTZPresetTourSpot */
	struct tt__PTZPresetTourExtension *Extension;	/* optional element of type tt:PTZPresetTourExtension */
	char *token;	/* optional attribute of type tt:ReferenceToken */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__PTZPresetTourExtension
#define SOAP_TYPE_tt__PTZPresetTourExtension (833)
/* tt:PTZPresetTourExtension */
struct tt__PTZPresetTourExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__PTZPresetTourSpot
#define SOAP_TYPE_tt__PTZPresetTourSpot (831)
/* tt:PTZPresetTourSpot */
struct tt__PTZPresetTourSpot
{
	struct tt__PTZPresetTourPresetDetail *PresetDetail;	/* required element of type tt:PTZPresetTourPresetDetail */
	struct tt__PTZSpeed *Speed;	/* optional element of type tt:PTZSpeed */
	char *StayTime;	/* optional element of type xsd:string */
	struct tt__PTZPresetTourSpotExtension *Extension;	/* optional element of type tt:PTZPresetTourSpotExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__PTZPresetTourSpotExtension
#define SOAP_TYPE_tt__PTZPresetTourSpotExtension (838)
/* tt:PTZPresetTourSpotExtension */
struct tt__PTZPresetTourSpotExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE__tt__union_PTZPresetTourPresetDetail
#define SOAP_TYPE__tt__union_PTZPresetTourPresetDetail (842)
/* xsd:choice */
union _tt__union_PTZPresetTourPresetDetail
{
#define SOAP_UNION__tt__union_PTZPresetTourPresetDetail_PresetToken	(1)
	char *PresetToken;
#define SOAP_UNION__tt__union_PTZPresetTourPresetDetail_Home	(2)
	enum xsd__boolean_ Home;
#define SOAP_UNION__tt__union_PTZPresetTourPresetDetail_PTZPosition	(3)
	struct tt__PTZVector *PTZPosition;
#define SOAP_UNION__tt__union_PTZPresetTourPresetDetail_TypeExtension	(4)
	struct tt__PTZPresetTourTypeExtension *TypeExtension;
};
#endif

#ifndef SOAP_TYPE_tt__PTZPresetTourPresetDetail
#define SOAP_TYPE_tt__PTZPresetTourPresetDetail (835)
/* tt:PTZPresetTourPresetDetail */
struct tt__PTZPresetTourPresetDetail
{
	int __union_PTZPresetTourPresetDetail;	/* union discriminant (of union defined below) */
	union _tt__union_PTZPresetTourPresetDetail union_PTZPresetTourPresetDetail;	/* required element of type xsd:choice */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__PTZPresetTourTypeExtension
#define SOAP_TYPE_tt__PTZPresetTourTypeExtension (840)
/* tt:PTZPresetTourTypeExtension */
struct tt__PTZPresetTourTypeExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__PTZPresetTourStatus
#define SOAP_TYPE_tt__PTZPresetTourStatus (827)
/* tt:PTZPresetTourStatus */
struct tt__PTZPresetTourStatus
{
	enum tt__PTZPresetTourState State;	/* required element of type tt:PTZPresetTourState */
	struct tt__PTZPresetTourSpot *CurrentTourSpot;	/* optional element of type tt:PTZPresetTourSpot */
	struct tt__PTZPresetTourStatusExtension *Extension;	/* optional element of type tt:PTZPresetTourStatusExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__PTZPresetTourStatusExtension
#define SOAP_TYPE_tt__PTZPresetTourStatusExtension (843)
/* tt:PTZPresetTourStatusExtension */
struct tt__PTZPresetTourStatusExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__PTZPresetTourStartingCondition
#define SOAP_TYPE_tt__PTZPresetTourStartingCondition (829)
/* tt:PTZPresetTourStartingCondition */
struct tt__PTZPresetTourStartingCondition
{
	int *RecurringTime;	/* optional element of type xsd:int */
	char *RecurringDuration;	/* optional element of type xsd:string */
	enum tt__PTZPresetTourDirection *Direction;	/* optional element of type tt:PTZPresetTourDirection */
	struct tt__PTZPresetTourStartingConditionExtension *Extension;	/* optional element of type tt:PTZPresetTourStartingConditionExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__PTZPresetTourStartingConditionExtension
#define SOAP_TYPE_tt__PTZPresetTourStartingConditionExtension (846)
/* tt:PTZPresetTourStartingConditionExtension */
struct tt__PTZPresetTourStartingConditionExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__PTZPresetTourOptions
#define SOAP_TYPE_tt__PTZPresetTourOptions (848)
/* tt:PTZPresetTourOptions */
struct tt__PTZPresetTourOptions
{
	enum xsd__boolean_ AutoStart;	/* required element of type xsd:boolean */
	struct tt__PTZPresetTourStartingConditionOptions *StartingCondition;	/* required element of type tt:PTZPresetTourStartingConditionOptions */
	struct tt__PTZPresetTourSpotOptions *TourSpot;	/* required element of type tt:PTZPresetTourSpotOptions */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__PTZPresetTourSpotOptions
#define SOAP_TYPE_tt__PTZPresetTourSpotOptions (851)
/* tt:PTZPresetTourSpotOptions */
struct tt__PTZPresetTourSpotOptions
{
	struct tt__PTZPresetTourPresetDetailOptions *PresetDetail;	/* required element of type tt:PTZPresetTourPresetDetailOptions */
	struct tt__DurationRange *StayTime;	/* required element of type tt:DurationRange */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__PTZPresetTourPresetDetailOptions
#define SOAP_TYPE_tt__PTZPresetTourPresetDetailOptions (853)
/* tt:PTZPresetTourPresetDetailOptions */
struct tt__PTZPresetTourPresetDetailOptions
{
	int __sizePresetToken;	/* sequence of elements <PresetToken> */
	char **PresetToken;	/* optional element of type tt:ReferenceToken */
	enum xsd__boolean_ *Home;	/* optional element of type xsd:boolean */
	struct tt__Space2DDescription *PanTiltPositionSpace;	/* optional element of type tt:Space2DDescription */
	struct tt__Space1DDescription *ZoomPositionSpace;	/* optional element of type tt:Space1DDescription */
	struct tt__PTZPresetTourPresetDetailOptionsExtension *Extension;	/* optional element of type tt:PTZPresetTourPresetDetailOptionsExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__PTZPresetTourPresetDetailOptionsExtension
#define SOAP_TYPE_tt__PTZPresetTourPresetDetailOptionsExtension (855)
/* tt:PTZPresetTourPresetDetailOptionsExtension */
struct tt__PTZPresetTourPresetDetailOptionsExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__PTZPresetTourStartingConditionOptions
#define SOAP_TYPE_tt__PTZPresetTourStartingConditionOptions (849)
/* tt:PTZPresetTourStartingConditionOptions */
struct tt__PTZPresetTourStartingConditionOptions
{
	struct tt__IntRange *RecurringTime;	/* optional element of type tt:IntRange */
	struct tt__DurationRange *RecurringDuration;	/* optional element of type tt:DurationRange */
	int __sizeDirection;	/* sequence of elements <Direction> */
	enum tt__PTZPresetTourDirection *Direction;	/* optional element of type tt:PTZPresetTourDirection */
	struct tt__PTZPresetTourStartingConditionOptionsExtension *Extension;	/* optional element of type tt:PTZPresetTourStartingConditionOptionsExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__PTZPresetTourStartingConditionOptionsExtension
#define SOAP_TYPE_tt__PTZPresetTourStartingConditionOptionsExtension (857)
/* tt:PTZPresetTourStartingConditionOptionsExtension */
struct tt__PTZPresetTourStartingConditionOptionsExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE__tptz__GetPresetTours
#define SOAP_TYPE__tptz__GetPresetTours (1515)
/* tptz:GetPresetTours */
struct _tptz__GetPresetTours
{
	char *ProfileToken;	/* required element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE__tptz__GetPresetToursResponse
#define SOAP_TYPE__tptz__GetPresetToursResponse (1516)
/* tptz:GetPresetToursResponse */
struct _tptz__GetPresetToursResponse
{
	int __sizePresetTour;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* sequence of elements <PresetTour> */
	struct tt__PresetTour *PresetTour;	/* optional element of type tt:PresetTour */
};
#endif

#ifndef SOAP_TYPE__tptz__GetPresetTour
#define SOAP_TYPE__tptz__GetPresetTour (1518)
/* tptz:GetPresetTour */
struct _tptz__GetPresetTour
{
	char *ProfileToken;	/* required element of type tt:ReferenceToken */
	char *PresetTourToken;	/* required element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE__tptz__GetPresetTourResponse
#define SOAP_TYPE__tptz__GetPresetTourResponse (1519)
/* tptz:GetPresetTourResponse */
struct _tptz__GetPresetTourResponse
{
	struct tt__PresetTour *PresetTour;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type tt:PresetTour */
};
#endif

#ifndef SOAP_TYPE__tptz__GetPresetTourOptions
#define SOAP_TYPE__tptz__GetPresetTourOptions (1520)
/* tptz:GetPresetTourOptions */
struct _tptz__GetPresetTourOptions
{
	char *ProfileToken;	/* required element of type tt:ReferenceToken */
	char *PresetTourToken;	/* optional element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE__tptz__GetPresetTourOptionsResponse
#define SOAP_TYPE__tptz__GetPresetTourOptionsResponse (1521)
/* tptz:GetPresetTourOptionsResponse */
struct _tptz__GetPresetTourOptionsResponse
{
	struct tt__PTZPresetTourOptions *Options;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type tt:PTZPresetTourOptions */
};
#endif

#ifndef SOAP_TYPE__tptz__CreatePresetTour
#define SOAP_TYPE__tptz__CreatePresetTour (1523)
/* tptz:CreatePresetTour */
struct _tptz__CreatePresetTour
{
	char *ProfileToken;	/* required element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE__tptz__CreatePresetTourResponse
#define SOAP_TYPE__tptz__CreatePresetTourResponse (1524)
/* tptz:CreatePresetTourResponse */
struct _tptz__CreatePresetTourResponse
{
	char *PresetTourToken;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE__tptz__ModifyPresetTour
#define SOAP_TYPE__tptz__ModifyPresetTour (1525)
/* tptz:ModifyPresetTour */
struct _tptz__ModifyPresetTour
{
	char *ProfileToken;	/* required element of type tt:ReferenceToken */
	struct tt__PresetTour *PresetTour;	/* required element of type tt:PresetTour */
};
#endif

#ifndef SOAP_TYPE__tptz__ModifyPresetTourResponse
#define SOAP_TYPE__tptz__ModifyPresetTourResponse (1526)
/* tptz:ModifyPresetTourResponse */
struct _tptz__ModifyPresetTourResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tptz__OperatePresetTour
#define SOAP_TYPE__tptz__OperatePresetTour (1527)
/* tptz:OperatePresetTour */
struct _tptz__OperatePresetTour
{
	char *ProfileToken;	/* required element of type tt:ReferenceToken */
	char *PresetTourToken;	/* required element of type tt:ReferenceToken */
	enum tt__PTZPresetTourOperation Operation;	/* required element of type tt:PTZPresetTourOperation */
};
#endif

#ifndef SOAP_TYPE__tptz__OperatePresetTourResponse
#define SOAP_TYPE__tptz__OperatePresetTourResponse (1528)
/* tptz:OperatePresetTourResponse */
struct _tptz__OperatePresetTourResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tptz__RemovePresetTour
#define SOAP_TYPE__tptz__RemovePresetTour (1529)
/* tptz:RemovePresetTour */
struct _tptz__RemovePresetTour
{
	char *ProfileToken;	/* required element of type tt:ReferenceToken */
	char *PresetTourToken;	/* required element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE__tptz__RemovePresetTourResponse
#define SOAP_TYPE__tptz__RemovePresetTourResponse (1530)
/* tptz:RemovePresetTourResponse */
struct _tptz__RemovePresetTourResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE___tptz__GetPresetTours
#define SOAP_TYPE___tptz__GetPresetTours (2250)
/* Operation wrapper: */
struct __tptz__GetPresetTours
{
	struct _tptz__GetPresetTours *tptz__GetPresetTours;	/* optional element of type tptz:GetPresetTours */
};
#endif

#ifndef SOAP_TYPE___tptz__GetPresetTour
#define SOAP_TYPE___tptz__GetPresetTour (2254)
/* Operation wrapper: */
struct __tptz__GetPresetTour
{
	struct _tptz__GetPresetTour *tptz__GetPresetTour;	/* optional element of type tptz:GetPresetTour */
};
#endif

#ifndef SOAP_TYPE___tptz__GetPresetTourOptions
#define SOAP_TYPE___tptz__GetPresetTourOptions (2258)
/* Operation wrapper: */
struct __tptz__GetPresetTourOptions
{
	struct _tptz__GetPresetTourOptions *tptz__GetPresetTourOptions;	/* optional element of type tptz:GetPresetTourOptions */
};
#endif

#ifndef SOAP_TYPE___tptz__CreatePresetTour
#define SOAP_TYPE___tptz__CreatePresetTour (2262)
/* Operation wrapper: */
struct __tptz__CreatePresetTour
{
	struct _tptz__CreatePresetTour *tptz__CreatePresetTour;	/* optional element of type tptz:CreatePresetTour */
};
#endif

#ifndef SOAP_TYPE___tptz__ModifyPresetTour
#define SOAP_TYPE___tptz__ModifyPresetTour (2266)
/* Operation wrapper: */
struct __tptz__ModifyPresetTour
{
	struct _tptz__ModifyPresetTour *tptz__ModifyPresetTour;	/* optional element of type tptz:ModifyPresetTour */
};
#endif

#ifndef SOAP_TYPE___tptz__OperatePresetTour
#define SOAP_TYPE___tptz__OperatePresetTour (2270)
/* Operation wrapper: */
struct __tptz__OperatePresetTour
{
	struct _tptz__OperatePresetTour *tptz__OperatePresetTour;	/* optional element of type tptz:OperatePresetTour */
};
#endif

#ifndef SOAP_TYPE___tptz__RemovePresetTour
#define SOAP_TYPE___tptz__RemovePresetTour (2274)
/* Operation wrapper: */
struct __tptz__RemovePresetTour
{
	struct _tptz__RemovePresetTour *tptz__RemovePresetTour;	/* optional element of type tptz:RemovePresetTour */
};
#endif
#ifndef SOAP_TYPE_tptz__Capabilities
#define SOAP_TYPE_tptz__Capabilities (122)
/* tptz:Capabilities */
struct tptz__Capabilities
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE__tptz__GetServiceCapabilities
#define SOAP_TYPE__tptz__GetServiceCapabilities (123)
/* tptz:GetServiceCapabilities */
struct _tptz__GetServiceCapabilities
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tptz__GetServiceCapabilitiesResponse
#define SOAP_TYPE__tptz__GetServiceCapabilitiesResponse (124)
/* tptz:GetServiceCapabilitiesResponse */
struct _tptz__GetServiceCapabilitiesResponse
{
	struct tptz__Capabilities *Capabilities;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type tptz:Capabilities */
};
#endif

#ifndef SOAP_TYPE__tptz__GetNodes
#define SOAP_TYPE__tptz__GetNodes (126)
/* tptz:GetNodes */
struct _tptz__GetNodes
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tptz__GetNodesResponse
#define SOAP_TYPE__tptz__GetNodesResponse (127)
/* tptz:GetNodesResponse */
struct _tptz__GetNodesResponse
{
	int __sizePTZNode;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* sequence of elements <PTZNode> */
	struct tt__PTZNode *PTZNode;	/* optional element of type tt:PTZNode */
};
#endif

#ifndef SOAP_TYPE__tptz__GetNode
#define SOAP_TYPE__tptz__GetNode (130)
/* tptz:GetNode */
struct _tptz__GetNode
{
	char *NodeToken;	/* required element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE__tptz__GetNodeResponse
#define SOAP_TYPE__tptz__GetNodeResponse (131)
/* tptz:GetNodeResponse */
struct _tptz__GetNodeResponse
{
	struct tt__PTZNode *PTZNode;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type tt:PTZNode */
};
#endif

#ifndef SOAP_TYPE__tptz__GetConfigurations
#define SOAP_TYPE__tptz__GetConfigurations (132)
/* tptz:GetConfigurations */
struct _tptz__GetConfigurations
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tptz__GetConfigurationsResponse
#define SOAP_TYPE__tptz__GetConfigurationsResponse (133)
/* tptz:GetConfigurationsResponse */
struct _tptz__GetConfigurationsResponse
{
	int __sizePTZConfiguration;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* sequence of elements <PTZConfiguration> */
	struct tt__PTZConfiguration *PTZConfiguration;	/* optional element of type tt:PTZConfiguration */
};
#endif

#ifndef SOAP_TYPE__tptz__GetConfiguration
#define SOAP_TYPE__tptz__GetConfiguration (136)
/* tptz:GetConfiguration */
struct _tptz__GetConfiguration
{
	char *PTZConfigurationToken;	/* required element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE__tptz__GetConfigurationResponse
#define SOAP_TYPE__tptz__GetConfigurationResponse (137)
/* tptz:GetConfigurationResponse */
struct _tptz__GetConfigurationResponse
{
	struct tt__PTZConfiguration *PTZConfiguration;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type tt:PTZConfiguration */
};
#endif

#ifndef SOAP_TYPE__tptz__SetConfiguration
#define SOAP_TYPE__tptz__SetConfiguration (138)
/* tptz:SetConfiguration */
struct _tptz__SetConfiguration
{
	struct tt__PTZConfiguration *PTZConfiguration;	/* required element of type tt:PTZConfiguration */
	enum xsd__boolean_ ForcePersistence;	/* required element of type xsd:boolean */
};
#endif

#ifndef SOAP_TYPE___tptz__SetConfigurationResponse_sequence
#define SOAP_TYPE___tptz__SetConfigurationResponse_sequence (140)
/* Operation wrapper: */
struct __tptz__SetConfigurationResponse_sequence
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tptz__SetConfigurationResponse
#define SOAP_TYPE__tptz__SetConfigurationResponse (139)
/* tptz:SetConfigurationResponse */
struct _tptz__SetConfigurationResponse
{
	int __size_SetConfigurationResponse_sequence;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* sequence of elements <-SetConfigurationResponse-sequence> */
	struct __tptz__SetConfigurationResponse_sequence *__SetConfigurationResponse_sequence;
};
#endif

#ifndef SOAP_TYPE__tptz__GetConfigurationOptions
#define SOAP_TYPE__tptz__GetConfigurationOptions (142)
/* tptz:GetConfigurationOptions */
struct _tptz__GetConfigurationOptions
{
	char *ConfigurationToken;	/* required element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE__tptz__GetConfigurationOptionsResponse
#define SOAP_TYPE__tptz__GetConfigurationOptionsResponse (143)
/* tptz:GetConfigurationOptionsResponse */
struct _tptz__GetConfigurationOptionsResponse
{
	struct tt__PTZConfigurationOptions *PTZConfigurationOptions;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type tt:PTZConfigurationOptions */
};
#endif

#ifndef SOAP_TYPE__tptz__SendAuxiliaryCommand
#define SOAP_TYPE__tptz__SendAuxiliaryCommand (146)
/* tptz:SendAuxiliaryCommand */
struct _tptz__SendAuxiliaryCommand
{
	char *ProfileToken;	/* required element of type tt:ReferenceToken */
	char *AuxiliaryData;	/* required element of type tt:AuxiliaryData */
};
#endif

#ifndef SOAP_TYPE__tptz__SendAuxiliaryCommandResponse
#define SOAP_TYPE__tptz__SendAuxiliaryCommandResponse (147)
/* tptz:SendAuxiliaryCommandResponse */
struct _tptz__SendAuxiliaryCommandResponse
{
	char *AuxiliaryResponse;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type tt:AuxiliaryData */
};
#endif

#ifndef SOAP_TYPE__tptz__GetPresets
#define SOAP_TYPE__tptz__GetPresets (148)
/* tptz:GetPresets */
struct _tptz__GetPresets
{
	char *ProfileToken;	/* required element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE__tptz__GetPresetsResponse
#define SOAP_TYPE__tptz__GetPresetsResponse (149)
/* tptz:GetPresetsResponse */
struct _tptz__GetPresetsResponse
{
	int __sizePreset;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* sequence of elements <Preset> */
	struct tt__PTZPreset *Preset;	/* optional element of type tt:PTZPreset */
};
#endif

#ifndef SOAP_TYPE__tptz__SetPreset
#define SOAP_TYPE__tptz__SetPreset (152)
/* tptz:SetPreset */
struct _tptz__SetPreset
{
	char *ProfileToken;	/* required element of type tt:ReferenceToken */
	char *PresetName;	/* optional element of type xsd:string */
	char *PresetToken;	/* optional element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE__tptz__SetPresetResponse
#define SOAP_TYPE__tptz__SetPresetResponse (153)
/* tptz:SetPresetResponse */
struct _tptz__SetPresetResponse
{
	char *PresetToken;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE__tptz__RemovePreset
#define SOAP_TYPE__tptz__RemovePreset (154)
/* tptz:RemovePreset */
struct _tptz__RemovePreset
{
	char *ProfileToken;	/* required element of type tt:ReferenceToken */
	char *PresetToken;	/* required element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE__tptz__RemovePresetResponse
#define SOAP_TYPE__tptz__RemovePresetResponse (155)
/* tptz:RemovePresetResponse */
struct _tptz__RemovePresetResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tptz__GotoPreset
#define SOAP_TYPE__tptz__GotoPreset (156)
/* tptz:GotoPreset */
struct _tptz__GotoPreset
{
	char *ProfileToken;	/* required element of type tt:ReferenceToken */
	char *PresetToken;	/* required element of type tt:ReferenceToken */
	struct tt__PTZSpeed *Speed;	/* optional element of type tt:PTZSpeed */
};
#endif

#ifndef SOAP_TYPE__tptz__GotoPresetResponse
#define SOAP_TYPE__tptz__GotoPresetResponse (159)
/* tptz:GotoPresetResponse */
struct _tptz__GotoPresetResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tptz__GetStatus
#define SOAP_TYPE__tptz__GetStatus (160)
/* tptz:GetStatus */
struct _tptz__GetStatus
{
	char *ProfileToken;	/* required element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE__tptz__GetStatusResponse
#define SOAP_TYPE__tptz__GetStatusResponse (161)
/* tptz:GetStatusResponse */
struct _tptz__GetStatusResponse
{
	struct tt__PTZStatus *PTZStatus;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type tt:PTZStatus */
};
#endif

#ifndef SOAP_TYPE__tptz__GotoHomePosition
#define SOAP_TYPE__tptz__GotoHomePosition (164)
/* tptz:GotoHomePosition */
struct _tptz__GotoHomePosition
{
	char *ProfileToken;	/* required element of type tt:ReferenceToken */
	struct tt__PTZSpeed *Speed;	/* optional element of type tt:PTZSpeed */
};
#endif

#ifndef SOAP_TYPE__tptz__GotoHomePositionResponse
#define SOAP_TYPE__tptz__GotoHomePositionResponse (165)
/* tptz:GotoHomePositionResponse */
struct _tptz__GotoHomePositionResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tptz__SetHomePosition
#define SOAP_TYPE__tptz__SetHomePosition (166)
/* tptz:SetHomePosition */
struct _tptz__SetHomePosition
{
	char *ProfileToken;	/* required element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE__tptz__SetHomePositionResponse
#define SOAP_TYPE__tptz__SetHomePositionResponse (167)
/* tptz:SetHomePositionResponse */
struct _tptz__SetHomePositionResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tptz__ContinuousMove
#define SOAP_TYPE__tptz__ContinuousMove (168)
/* tptz:ContinuousMove */
struct _tptz__ContinuousMove
{
	char *ProfileToken;	/* required element of type tt:ReferenceToken */
	struct tt__PTZSpeed *Velocity;	/* required element of type tt:PTZSpeed */
	char *Timeout;	/* optional element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE__tptz__ContinuousMoveResponse
#define SOAP_TYPE__tptz__ContinuousMoveResponse (169)
/* tptz:ContinuousMoveResponse */
struct _tptz__ContinuousMoveResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tptz__RelativeMove
#define SOAP_TYPE__tptz__RelativeMove (170)
/* tptz:RelativeMove */
struct _tptz__RelativeMove
{
	char *ProfileToken;	/* required element of type tt:ReferenceToken */
	struct tt__PTZVector *Translation;	/* required element of type tt:PTZVector */
	struct tt__PTZSpeed *Speed;	/* optional element of type tt:PTZSpeed */
};
#endif

#ifndef SOAP_TYPE__tptz__RelativeMoveResponse
#define SOAP_TYPE__tptz__RelativeMoveResponse (173)
/* tptz:RelativeMoveResponse */
struct _tptz__RelativeMoveResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tptz__AbsoluteMove
#define SOAP_TYPE__tptz__AbsoluteMove (174)
/* tptz:AbsoluteMove */
struct _tptz__AbsoluteMove
{
	char *ProfileToken;	/* required element of type tt:ReferenceToken */
	struct tt__PTZVector *Position;	/* required element of type tt:PTZVector */
	struct tt__PTZSpeed *Speed;	/* optional element of type tt:PTZSpeed */
};
#endif

#ifndef SOAP_TYPE__tptz__AbsoluteMoveResponse
#define SOAP_TYPE__tptz__AbsoluteMoveResponse (175)
/* tptz:AbsoluteMoveResponse */
struct _tptz__AbsoluteMoveResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tptz__Stop
#define SOAP_TYPE__tptz__Stop (176)
/* tptz:Stop */
struct _tptz__Stop
{
	char *ProfileToken;	/* required element of type tt:ReferenceToken */
	enum xsd__boolean_ *PanTilt;	/* optional element of type xsd:boolean */
	enum xsd__boolean_ *Zoom;	/* optional element of type xsd:boolean */
};
#endif

#ifndef SOAP_TYPE__tptz__StopResponse
#define SOAP_TYPE__tptz__StopResponse (178)
/* tptz:StopResponse */
struct _tptz__StopResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE___tptz__GetServiceCapabilities
#define SOAP_TYPE___tptz__GetServiceCapabilities (1081)
/* Operation wrapper: */
struct __tptz__GetServiceCapabilities
{
	struct _tptz__GetServiceCapabilities *tptz__GetServiceCapabilities;	/* optional element of type tptz:GetServiceCapabilities */
};
#endif

#ifndef SOAP_TYPE___tptz__GetConfigurations
#define SOAP_TYPE___tptz__GetConfigurations (1085)
/* Operation wrapper: */
struct __tptz__GetConfigurations
{
	struct _tptz__GetConfigurations *tptz__GetConfigurations;	/* optional element of type tptz:GetConfigurations */
};
#endif

#ifndef SOAP_TYPE___tptz__GetPresets
#define SOAP_TYPE___tptz__GetPresets (1089)
/* Operation wrapper: */
struct __tptz__GetPresets
{
	struct _tptz__GetPresets *tptz__GetPresets;	/* optional element of type tptz:GetPresets */
};
#endif

#ifndef SOAP_TYPE___tptz__SetPreset
#define SOAP_TYPE___tptz__SetPreset (1093)
/* Operation wrapper: */
struct __tptz__SetPreset
{
	struct _tptz__SetPreset *tptz__SetPreset;	/* optional element of type tptz:SetPreset */
};
#endif

#ifndef SOAP_TYPE___tptz__RemovePreset
#define SOAP_TYPE___tptz__RemovePreset (1097)
/* Operation wrapper: */
struct __tptz__RemovePreset
{
	struct _tptz__RemovePreset *tptz__RemovePreset;	/* optional element of type tptz:RemovePreset */
};
#endif

#ifndef SOAP_TYPE___tptz__GotoPreset
#define SOAP_TYPE___tptz__GotoPreset (1101)
/* Operation wrapper: */
struct __tptz__GotoPreset
{
	struct _tptz__GotoPreset *tptz__GotoPreset;	/* optional element of type tptz:GotoPreset */
};
#endif

#ifndef SOAP_TYPE___tptz__GetStatus
#define SOAP_TYPE___tptz__GetStatus (1105)
/* Operation wrapper: */
struct __tptz__GetStatus
{
	struct _tptz__GetStatus *tptz__GetStatus;	/* optional element of type tptz:GetStatus */
};
#endif

#ifndef SOAP_TYPE___tptz__GetConfiguration
#define SOAP_TYPE___tptz__GetConfiguration (1109)
/* Operation wrapper: */
struct __tptz__GetConfiguration
{
	struct _tptz__GetConfiguration *tptz__GetConfiguration;	/* optional element of type tptz:GetConfiguration */
};
#endif

#ifndef SOAP_TYPE___tptz__GetNodes
#define SOAP_TYPE___tptz__GetNodes (1113)
/* Operation wrapper: */
struct __tptz__GetNodes
{
	struct _tptz__GetNodes *tptz__GetNodes;	/* optional element of type tptz:GetNodes */
};
#endif

#ifndef SOAP_TYPE___tptz__GetNode
#define SOAP_TYPE___tptz__GetNode (1117)
/* Operation wrapper: */
struct __tptz__GetNode
{
	struct _tptz__GetNode *tptz__GetNode;	/* optional element of type tptz:GetNode */
};
#endif

#ifndef SOAP_TYPE___tptz__SetConfiguration
#define SOAP_TYPE___tptz__SetConfiguration (1121)
/* Operation wrapper: */
struct __tptz__SetConfiguration
{
	struct _tptz__SetConfiguration *tptz__SetConfiguration;	/* optional element of type tptz:SetConfiguration */
};
#endif

#ifndef SOAP_TYPE___tptz__GetConfigurationOptions
#define SOAP_TYPE___tptz__GetConfigurationOptions (1125)
/* Operation wrapper: */
struct __tptz__GetConfigurationOptions
{
	struct _tptz__GetConfigurationOptions *tptz__GetConfigurationOptions;	/* optional element of type tptz:GetConfigurationOptions */
};
#endif

#ifndef SOAP_TYPE___tptz__GotoHomePosition
#define SOAP_TYPE___tptz__GotoHomePosition (1129)
/* Operation wrapper: */
struct __tptz__GotoHomePosition
{
	struct _tptz__GotoHomePosition *tptz__GotoHomePosition;	/* optional element of type tptz:GotoHomePosition */
};
#endif

#ifndef SOAP_TYPE___tptz__SetHomePosition
#define SOAP_TYPE___tptz__SetHomePosition (1133)
/* Operation wrapper: */
struct __tptz__SetHomePosition
{
	struct _tptz__SetHomePosition *tptz__SetHomePosition;	/* optional element of type tptz:SetHomePosition */
};
#endif

#ifndef SOAP_TYPE___tptz__ContinuousMove
#define SOAP_TYPE___tptz__ContinuousMove (1137)
/* Operation wrapper: */
struct __tptz__ContinuousMove
{
	struct _tptz__ContinuousMove *tptz__ContinuousMove;	/* optional element of type tptz:ContinuousMove */
};
#endif

#ifndef SOAP_TYPE___tptz__RelativeMove
#define SOAP_TYPE___tptz__RelativeMove (1141)
/* Operation wrapper: */
struct __tptz__RelativeMove
{
	struct _tptz__RelativeMove *tptz__RelativeMove;	/* optional element of type tptz:RelativeMove */
};
#endif

#ifndef SOAP_TYPE___tptz__SendAuxiliaryCommand
#define SOAP_TYPE___tptz__SendAuxiliaryCommand (1145)
/* Operation wrapper: */
struct __tptz__SendAuxiliaryCommand
{
	struct _tptz__SendAuxiliaryCommand *tptz__SendAuxiliaryCommand;	/* optional element of type tptz:SendAuxiliaryCommand */
};
#endif

#ifndef SOAP_TYPE___tptz__AbsoluteMove
#define SOAP_TYPE___tptz__AbsoluteMove (1149)
/* Operation wrapper: */
struct __tptz__AbsoluteMove
{
	struct _tptz__AbsoluteMove *tptz__AbsoluteMove;	/* optional element of type tptz:AbsoluteMove */
};
#endif

#ifndef SOAP_TYPE___tptz__Stop
#define SOAP_TYPE___tptz__Stop (1153)
/* Operation wrapper: */
struct __tptz__Stop
{
	struct _tptz__Stop *tptz__Stop;	/* optional element of type tptz:Stop */
};
#endif

//*********************img@@@@@@@@@@@@@@@@@@******************
#ifndef SOAP_TYPE__timg__Stop
#define SOAP_TYPE__timg__Stop (145)
/* timg:Stop */
struct _timg__Stop
{
	char *VideoSourceToken;	/* required element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE__timg__StopResponse
#define SOAP_TYPE__timg__StopResponse (146)
/* timg:StopResponse */
struct _timg__StopResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE___timg__Stop
#define SOAP_TYPE___timg__Stop (1078)
/* Operation wrapper: */
struct __timg__Stop
{
	struct _timg__Stop *timg__Stop;	/* optional element of type timg:Stop */
};
#endif

#ifndef SOAP_TYPE__timg__Move
#define SOAP_TYPE__timg__Move (137)
/* timg:Move */
struct _timg__Move
{
	char *VideoSourceToken;	/* required element of type tt:ReferenceToken */
	struct tt__FocusMove *Focus;	/* required element of type tt:FocusMove */
};
#endif


#ifndef SOAP_TYPE__timg__MoveResponse
#define SOAP_TYPE__timg__MoveResponse (140)
/* timg:MoveResponse */
struct _timg__MoveResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE___timg__Move
#define SOAP_TYPE___timg__Move (1074)
/* Operation wrapper: */
struct __timg__Move
{
	struct _timg__Move *timg__Move;	/* optional element of type timg:Move */
};
#endif

#ifndef SOAP_TYPE___timg__GetMoveOptions
#define SOAP_TYPE___timg__GetMoveOptions (1086)
/* Operation wrapper: */
struct __timg__GetMoveOptions
{
	struct _timg__GetMoveOptions *timg__GetMoveOptions;	/* optional element of type timg:GetMoveOptions */
};
#endif

#ifndef SOAP_TYPE__timg__GetMoveOptions
#define SOAP_TYPE__timg__GetMoveOptions (141)
/* timg:GetMoveOptions */
struct _timg__GetMoveOptions
{
	char *VideoSourceToken;	/* required element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE__timg__GetMoveOptionsResponse
#define SOAP_TYPE__timg__GetMoveOptionsResponse (142)
/* timg:GetMoveOptionsResponse */
struct _timg__GetMoveOptionsResponse
{
	struct tt__MoveOptions20 *MoveOptions;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type tt:MoveOptions20 */
};
#endif

#ifndef SOAP_TYPE_tt__PTZNodeExtension
#define SOAP_TYPE_tt__PTZNodeExtension (493)
/* tt:PTZNodeExtension */
struct tt__PTZNodeExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__PTZConfigurationExtension
#define SOAP_TYPE_tt__PTZConfigurationExtension (494)
/* tt:PTZConfigurationExtension */
struct tt__PTZConfigurationExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__PTZConfigurationOptions
#define SOAP_TYPE_tt__PTZConfigurationOptions (144)
/* tt:PTZConfigurationOptions */
struct tt__PTZConfigurationOptions
{
	struct tt__PTZSpaces *Spaces;	/* required element of type tt:PTZSpaces */
	struct tt__DurationRange *PTZTimeout;	/* required element of type tt:DurationRange */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__PanTiltLimits
#define SOAP_TYPE_tt__PanTiltLimits (498)
/* tt:PanTiltLimits */
struct tt__PanTiltLimits
{
	struct tt__Space2DDescription *Range;	/* required element of type tt:Space2DDescription */
};
#endif

#ifndef SOAP_TYPE_tt__ZoomLimits
#define SOAP_TYPE_tt__ZoomLimits (501)
/* tt:ZoomLimits */
struct tt__ZoomLimits
{
	struct tt__Space1DDescription *Range;	/* required element of type tt:Space1DDescription */
};
#endif

#ifndef SOAP_TYPE_tt__PTZSpaces
#define SOAP_TYPE_tt__PTZSpaces (495)
/* tt:PTZSpaces */
struct tt__PTZSpaces
{
	int __sizeAbsolutePanTiltPositionSpace;	/* sequence of elements <AbsolutePanTiltPositionSpace> */
	struct tt__Space2DDescription *AbsolutePanTiltPositionSpace;	/* optional element of type tt:Space2DDescription */
	int __sizeAbsoluteZoomPositionSpace;	/* sequence of elements <AbsoluteZoomPositionSpace> */
	struct tt__Space1DDescription *AbsoluteZoomPositionSpace;	/* optional element of type tt:Space1DDescription */
	int __sizeRelativePanTiltTranslationSpace;	/* sequence of elements <RelativePanTiltTranslationSpace> */
	struct tt__Space2DDescription *RelativePanTiltTranslationSpace;	/* optional element of type tt:Space2DDescription */
	int __sizeRelativeZoomTranslationSpace;	/* sequence of elements <RelativeZoomTranslationSpace> */
	struct tt__Space1DDescription *RelativeZoomTranslationSpace;	/* optional element of type tt:Space1DDescription */
	int __sizeContinuousPanTiltVelocitySpace;	/* sequence of elements <ContinuousPanTiltVelocitySpace> */
	struct tt__Space2DDescription *ContinuousPanTiltVelocitySpace;	/* optional element of type tt:Space2DDescription */
	int __sizeContinuousZoomVelocitySpace;	/* sequence of elements <ContinuousZoomVelocitySpace> */
	struct tt__Space1DDescription *ContinuousZoomVelocitySpace;	/* optional element of type tt:Space1DDescription */
	int __sizePanTiltSpeedSpace;	/* sequence of elements <PanTiltSpeedSpace> */
	struct tt__Space1DDescription *PanTiltSpeedSpace;	/* optional element of type tt:Space1DDescription */
	int __sizeZoomSpeedSpace;	/* sequence of elements <ZoomSpeedSpace> */
	struct tt__Space1DDescription *ZoomSpeedSpace;	/* optional element of type tt:Space1DDescription */
	struct tt__PTZSpacesExtension *Extension;	/* optional element of type tt:PTZSpacesExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__PTZSpacesExtension
#define SOAP_TYPE_tt__PTZSpacesExtension (504)
/* tt:PTZSpacesExtension */
struct tt__PTZSpacesExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__Space2DDescription
#define SOAP_TYPE_tt__Space2DDescription (499)
/* tt:Space2DDescription */
struct tt__Space2DDescription
{
	char *URI;	/* required element of type xsd:string */
	struct tt__FloatRange *XRange;	/* required element of type tt:FloatRange */
	struct tt__FloatRange *YRange;	/* required element of type tt:FloatRange */
};
#endif

#ifndef SOAP_TYPE_tt__Space1DDescription
#define SOAP_TYPE_tt__Space1DDescription (502)
/* tt:Space1DDescription */
struct tt__Space1DDescription
{
	char *URI;	/* required element of type xsd:string */
	struct tt__FloatRange *XRange;	/* required element of type tt:FloatRange */
};
#endif

#ifndef SOAP_TYPE_tt__Vector2D
#define SOAP_TYPE_tt__Vector2D (507)
/* tt:Vector2D */
struct tt__Vector2D
{
	float x;	/* required attribute of type xsd:float */
	float y;	/* required attribute of type xsd:float */
	char *space;	/* optional attribute of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_tt__Vector1D
#define SOAP_TYPE_tt__Vector1D (508)
/* tt:Vector1D */
struct tt__Vector1D
{
	float x;	/* required attribute of type xsd:float */
	char *space;	/* optional attribute of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_tt__PTZVector
#define SOAP_TYPE_tt__PTZVector (171)
/* tt:PTZVector */
struct tt__PTZVector
{
	struct tt__Vector2D *PanTilt;	/* optional element of type tt:Vector2D */
	struct tt__Vector1D *Zoom;	/* optional element of type tt:Vector1D */
};
#endif

#ifndef SOAP_TYPE_tt__PTZSpeed
#define SOAP_TYPE_tt__PTZSpeed (157)
/* tt:PTZSpeed */
struct tt__PTZSpeed
{
	struct tt__Vector2D *PanTilt;	/* optional element of type tt:Vector2D */
	struct tt__Vector1D *Zoom;	/* optional element of type tt:Vector1D */
};
#endif

#ifndef SOAP_TYPE_tt__PTZStatus
#define SOAP_TYPE_tt__PTZStatus (162)
/* tt:PTZStatus */
struct tt__PTZStatus
{
	struct tt__PTZVector *Position;	/* optional element of type tt:PTZVector */
	struct tt__PTZMoveStatus *MoveStatus;	/* optional element of type tt:PTZMoveStatus */
	char *Error;	/* optional element of type xsd:string */
	time_t UtcTime;	/* required element of type xsd:dateTime */
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__PTZPreset
#define SOAP_TYPE_tt__PTZPreset (150)
/* tt:PTZPreset */
struct tt__PTZPreset
{
	char *Name;	/* optional element of type tt:Name */
	struct tt__PTZVector *PTZPosition;	/* optional element of type tt:PTZVector */
	char *token;	/* optional attribute of type tt:ReferenceToken */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__PTZMoveStatus
#define SOAP_TYPE_tt__PTZMoveStatus (511)
/* tt:PTZMoveStatus */
struct tt__PTZMoveStatus
{
	enum tt__MoveStatus *PanTilt;	/* optional element of type tt:MoveStatus */
	enum tt__MoveStatus *Zoom;	/* optional element of type tt:MoveStatus */
};
#endif

#ifndef SOAP_TYPE_tt__DurationRange
#define SOAP_TYPE_tt__DurationRange (186)
/* tt:DurationRange */
struct tt__DurationRange
{
	char *Min;	/* required element of type xsd:string */
	char *Max;	/* required element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_tt__FocusMove
#define SOAP_TYPE_tt__FocusMove (551)
/* tt:FocusMove */
struct tt__FocusMove
{
	struct tt__AbsoluteFocus *Absolute;	/* optional element of type tt:AbsoluteFocus */
	struct tt__RelativeFocus *Relative;	/* optional element of type tt:RelativeFocus */
	struct tt__ContinuousFocus *Continuous;	/* optional element of type tt:ContinuousFocus */
};
#endif

#ifndef SOAP_TYPE_tt__AbsoluteFocus
#define SOAP_TYPE_tt__AbsoluteFocus (552)
/* tt:AbsoluteFocus */
struct tt__AbsoluteFocus
{
	float Position;	/* required element of type xsd:float */
	float *Speed;	/* optional element of type xsd:float */
};
#endif

#ifndef SOAP_TYPE_tt__RelativeFocus
#define SOAP_TYPE_tt__RelativeFocus (554)
/* tt:RelativeFocus */
struct tt__RelativeFocus
{
	float Distance;	/* required element of type xsd:float */
	float *Speed;	/* optional element of type xsd:float */
};
#endif

#ifndef SOAP_TYPE_tt__ContinuousFocus
#define SOAP_TYPE_tt__ContinuousFocus (556)
/* tt:ContinuousFocus */
struct tt__ContinuousFocus
{
	float Speed;	/* required element of type xsd:float */
};
#endif

#ifndef SOAP_TYPE_tt__MoveOptions
#define SOAP_TYPE_tt__MoveOptions (558)
/* tt:MoveOptions */
struct tt__MoveOptions
{
	struct tt__AbsoluteFocusOptions *Absolute;	/* optional element of type tt:AbsoluteFocusOptions */
	struct tt__RelativeFocusOptions *Relative;	/* optional element of type tt:RelativeFocusOptions */
	struct tt__ContinuousFocusOptions *Continuous;	/* optional element of type tt:ContinuousFocusOptions */
};
#endif

#ifndef SOAP_TYPE_tt__AbsoluteFocusOptions
#define SOAP_TYPE_tt__AbsoluteFocusOptions (559)
/* tt:AbsoluteFocusOptions */
struct tt__AbsoluteFocusOptions
{
	struct tt__FloatRange *Position;	/* required element of type tt:FloatRange */
	struct tt__FloatRange *Speed;	/* optional element of type tt:FloatRange */
};
#endif

#ifndef SOAP_TYPE_tt__RelativeFocusOptions
#define SOAP_TYPE_tt__RelativeFocusOptions (561)
/* tt:RelativeFocusOptions */
struct tt__RelativeFocusOptions
{
	struct tt__FloatRange *Distance;	/* required element of type tt:FloatRange */
	struct tt__FloatRange *Speed;	/* required element of type tt:FloatRange */
};
#endif

#ifndef SOAP_TYPE_tt__ContinuousFocusOptions
#define SOAP_TYPE_tt__ContinuousFocusOptions (563)
/* tt:ContinuousFocusOptions */
struct tt__ContinuousFocusOptions
{
	struct tt__FloatRange *Speed;	/* required element of type tt:FloatRange */
};
#endif

#ifndef SOAP_TYPE_tt__RelativeFocusOptions20
#define SOAP_TYPE_tt__RelativeFocusOptions20 (599)
/* tt:RelativeFocusOptions20 */
struct tt__RelativeFocusOptions20
{
	struct tt__FloatRange *Distance;	/* required element of type tt:FloatRange */
	struct tt__FloatRange *Speed;	/* optional element of type tt:FloatRange */
};
#endif

#ifndef SOAP_TYPE_tt__MoveOptions20
#define SOAP_TYPE_tt__MoveOptions20 (598)
/* tt:MoveOptions20 */
struct tt__MoveOptions20
{
	struct tt__AbsoluteFocusOptions *Absolute;	/* optional element of type tt:AbsoluteFocusOptions */
	struct tt__RelativeFocusOptions20 *Relative;	/* optional element of type tt:RelativeFocusOptions20 */
	struct tt__ContinuousFocusOptions *Continuous;	/* optional element of type tt:ContinuousFocusOptions */
};
#endif


/******************************************************************************\
 *                                                                            *
 * Classes and Structs                                                        *
 *                                                                            *
\******************************************************************************/


#ifndef SOAP_TYPE_tt__PTZNode
#define SOAP_TYPE_tt__PTZNode (128)
/* tt:PTZNode */
struct tt__PTZNode
{
	char *token;	/* required attribute of type tt:ReferenceToken */
	char *Name;	/* optional element of type tt:Name */
	struct tt__PTZSpaces *SupportedPTZSpaces;	/* required element of type tt:PTZSpaces */
	int MaximumNumberOfPresets;	/* required element of type xsd:int */
	enum xsd__boolean_ HomeSupported;	/* required element of type xsd:boolean */
	int __sizeAuxiliaryCommands;	/* sequence of elements <AuxiliaryCommands> */
	char **AuxiliaryCommands;	/* optional element of type tt:AuxiliaryData */
	struct tt__PTZNodeExtension *Extension;	/* optional element of type tt:PTZNodeExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__PTZConfiguration
#define SOAP_TYPE_tt__PTZConfiguration (134)
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

/******************************************************************************\
 *                                                                            *
 * Client-Side Call Stubs                                                     *
 *                                                                            *
\******************************************************************************/
SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__GetServiceCapabilities(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__GetServiceCapabilities *tptz__GetServiceCapabilities, struct _tptz__GetServiceCapabilitiesResponse *tptz__GetServiceCapabilitiesResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__GetConfigurations(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__GetConfigurations *tptz__GetConfigurations, struct _tptz__GetConfigurationsResponse *tptz__GetConfigurationsResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__GetPresets(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__GetPresets *tptz__GetPresets, struct _tptz__GetPresetsResponse *tptz__GetPresetsResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__SetPreset(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__SetPreset *tptz__SetPreset, struct _tptz__SetPresetResponse *tptz__SetPresetResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__RemovePreset(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__RemovePreset *tptz__RemovePreset, struct _tptz__RemovePresetResponse *tptz__RemovePresetResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__GotoPreset(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__GotoPreset *tptz__GotoPreset, struct _tptz__GotoPresetResponse *tptz__GotoPresetResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__GetStatus(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__GetStatus *tptz__GetStatus, struct _tptz__GetStatusResponse *tptz__GetStatusResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__GetConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__GetConfiguration *tptz__GetConfiguration, struct _tptz__GetConfigurationResponse *tptz__GetConfigurationResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__GetNodes(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__GetNodes *tptz__GetNodes, struct _tptz__GetNodesResponse *tptz__GetNodesResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__GetNode(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__GetNode *tptz__GetNode, struct _tptz__GetNodeResponse *tptz__GetNodeResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__SetConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__SetConfiguration *tptz__SetConfiguration, struct _tptz__SetConfigurationResponse *tptz__SetConfigurationResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__GetConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__GetConfigurationOptions *tptz__GetConfigurationOptions, struct _tptz__GetConfigurationOptionsResponse *tptz__GetConfigurationOptionsResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__GotoHomePosition(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__GotoHomePosition *tptz__GotoHomePosition, struct _tptz__GotoHomePositionResponse *tptz__GotoHomePositionResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__SetHomePosition(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__SetHomePosition *tptz__SetHomePosition, struct _tptz__SetHomePositionResponse *tptz__SetHomePositionResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__ContinuousMove(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__ContinuousMove *tptz__ContinuousMove, struct _tptz__ContinuousMoveResponse *tptz__ContinuousMoveResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__RelativeMove(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__RelativeMove *tptz__RelativeMove, struct _tptz__RelativeMoveResponse *tptz__RelativeMoveResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__SendAuxiliaryCommand(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__SendAuxiliaryCommand *tptz__SendAuxiliaryCommand, struct _tptz__SendAuxiliaryCommandResponse *tptz__SendAuxiliaryCommandResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__AbsoluteMove(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__AbsoluteMove *tptz__AbsoluteMove, struct _tptz__AbsoluteMoveResponse *tptz__AbsoluteMoveResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__Stop(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__Stop *tptz__Stop, struct _tptz__StopResponse *tptz__StopResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__GetPresetTours(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__GetPresetTours *tptz__GetPresetTours, struct _tptz__GetPresetToursResponse *tptz__GetPresetToursResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__GetPresetTour(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__GetPresetTour *tptz__GetPresetTour, struct _tptz__GetPresetTourResponse *tptz__GetPresetTourResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__GetPresetTourOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__GetPresetTourOptions *tptz__GetPresetTourOptions, struct _tptz__GetPresetTourOptionsResponse *tptz__GetPresetTourOptionsResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__CreatePresetTour(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__CreatePresetTour *tptz__CreatePresetTour, struct _tptz__CreatePresetTourResponse *tptz__CreatePresetTourResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__ModifyPresetTour(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__ModifyPresetTour *tptz__ModifyPresetTour, struct _tptz__ModifyPresetTourResponse *tptz__ModifyPresetTourResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__OperatePresetTour(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__OperatePresetTour *tptz__OperatePresetTour, struct _tptz__OperatePresetTourResponse *tptz__OperatePresetTourResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__RemovePresetTour(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__RemovePresetTour *tptz__RemovePresetTour, struct _tptz__RemovePresetTourResponse *tptz__RemovePresetTourResponse);


SOAP_FMAC5 int SOAP_FMAC6 soap_call___timg__Stop(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__Stop *timg__Stop, struct _timg__StopResponse *timg__StopResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___timg__Move(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__Move *timg__Move, struct _timg__MoveResponse *timg__MoveResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___timg__GetMoveOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__GetMoveOptions *timg__GetMoveOptions, struct _timg__GetMoveOptionsResponse *timg__GetMoveOptionsResponse);

#ifdef __cplusplus
}
#endif

#endif

/* End of soapStub.h */
