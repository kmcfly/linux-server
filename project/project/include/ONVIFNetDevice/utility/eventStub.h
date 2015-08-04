#ifndef eventStub_H
#define eventStub_H

#include "OnvifSoapH.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SOAP_TYPE___denf__Subscribe
#define SOAP_TYPE___denf__Subscribe (1768)
/* Operation wrapper: */
struct __denf__Subscribe
{
	struct _wsnt__Subscribe *wsnt__Subscribe;	/* optional element of type wsnt:Subscribe */
};
#endif


#ifndef SOAP_TYPE___denc__Notify
#define SOAP_TYPE___denc__Notify (1764)
/* Operation wrapper: */
struct __denc__Notify
{
	struct _wsnt__Notify *wsnt__Notify;	/* optional element of type wsnt:Notify */
};
#endif

#if 1//ndef SOAP_TYPE__wsnt__Subscribe
#define SOAP_TYPE__wsnt__Subscribe (1239)
/* wsnt:Subscribe */
struct _wsnt__Subscribe
{
	struct wsa5__EndpointReferenceType ConsumerReference;	/* required element of type wsa5:EndpointReferenceType */
	struct wsnt__FilterType *Filter;	/* optional element of type wsnt:FilterType */
	char *InitialTerminationTime;	/* optional element of type wsnt:AbsoluteOrRelativeTimeType */
	struct _wsnt__Subscribe_SubscriptionPolicy *SubscriptionPolicy;	/* optional element of type wsnt:Subscribe-SubscriptionPolicy */
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE__wsnt__SubscribeResponse
#define SOAP_TYPE__wsnt__SubscribeResponse (1242)
/* wsnt:SubscribeResponse */
struct _wsnt__SubscribeResponse
{
	struct wsa5__EndpointReferenceType SubscriptionReference;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type wsa5:EndpointReferenceType */
	time_t *CurrentTime;	/* optional element of type xsd:dateTime */
	time_t *TerminationTime;	/* optional element of type xsd:dateTime */
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE__wsnt__Renew
#define SOAP_TYPE__wsnt__Renew (1252)
/* wsnt:Renew */
struct _wsnt__Renew
{
	char *TerminationTime;	/* required element of type wsnt:AbsoluteOrRelativeTimeType */
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE__wsnt__RenewResponse
#define SOAP_TYPE__wsnt__RenewResponse (1253)
/* wsnt:RenewResponse */
struct _wsnt__RenewResponse
{
	time_t TerminationTime;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type xsd:dateTime */
	time_t *CurrentTime;	/* optional element of type xsd:dateTime */
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE___desm__Renew
#define SOAP_TYPE___desm__Renew (1808)
/* Operation wrapper: */
struct __desm__Renew
{
	struct _wsnt__Renew *wsnt__Renew;	/* optional element of type wsnt:Renew */
};
#endif

#ifndef SOAP_TYPE__wsnt__Notify
#define SOAP_TYPE__wsnt__Notify (1237)
/* wsnt:Notify */
struct _wsnt__Notify
{
	int __sizeNotificationMessage;	/* sequence of elements <NotificationMessage> */
	struct wsnt__NotificationMessageHolderType *NotificationMessage;	/* required element of type wsnt:NotificationMessageHolderType */
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE__wsnt__NotificationMessageHolderType_Message
#define SOAP_TYPE__wsnt__NotificationMessageHolderType_Message (1231)
/* wsnt:NotificationMessageHolderType-Message */
struct _wsnt__NotificationMessageHolderType_Message
{
	char *__any;
};
#endif

#ifndef SOAP_TYPE_wsnt__NotificationMessageHolderType
#define SOAP_TYPE_wsnt__NotificationMessageHolderType (1106)
/* wsnt:NotificationMessageHolderType */
struct wsnt__NotificationMessageHolderType
{
	struct wsa5__EndpointReferenceType *SubscriptionReference;	/* optional element of type wsa5:EndpointReferenceType */
	struct wsnt__TopicExpressionType *Topic;	/* optional element of type wsnt:TopicExpressionType */
	struct wsa5__EndpointReferenceType *ProducerReference;	/* optional element of type wsa5:EndpointReferenceType */
	struct _wsnt__NotificationMessageHolderType_Message Message;	/* required element of type wsnt:NotificationMessageHolderType-Message */
};
#endif

#ifndef SOAP_TYPE__wsnt__Subscribe_SubscriptionPolicy
#define SOAP_TYPE__wsnt__Subscribe_SubscriptionPolicy (1240)
/* wsnt:Subscribe-SubscriptionPolicy */
struct _wsnt__Subscribe_SubscriptionPolicy
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_wsnt__TopicExpressionType
#define SOAP_TYPE_wsnt__TopicExpressionType (1227)
/* wsnt:TopicExpressionType */
struct wsnt__TopicExpressionType
{
	char *__any;
	char *Dialect;	/* required attribute of type xsd:string */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
	char *__mixed;
};
#endif

#ifndef SOAP_TYPE___desm__Unsubscribe
#define SOAP_TYPE___desm__Unsubscribe (1810)
/* Operation wrapper: */
struct __desm__Unsubscribe
{
	struct _wsnt__Unsubscribe *wsnt__Unsubscribe;	/* optional element of type wsnt:Unsubscribe */
};
#endif

#ifndef SOAP_TYPE__wsnt__UnsubscribeResponse
#define SOAP_TYPE__wsnt__UnsubscribeResponse (1255)
/* wsnt:UnsubscribeResponse */
struct _wsnt__UnsubscribeResponse
{
	int __size;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE__wsnt__Unsubscribe
#define SOAP_TYPE__wsnt__Unsubscribe (1254)
/* wsnt:Unsubscribe */
struct _wsnt__Unsubscribe
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif


#ifndef SOAP_TYPE_wstop__Documentation
#define SOAP_TYPE_wstop__Documentation (1117)
/* wstop:Documentation */
struct wstop__Documentation
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
	char *__mixed;
};
#endif

#ifndef SOAP_TYPE_wstop__TopicSetType
#define SOAP_TYPE_wstop__TopicSetType (1083)
/* wstop:TopicSetType */
struct wstop__TopicSetType
{
	struct wstop__Documentation *documentation;	/* optional element of type wstop:Documentation */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE__tev__GetEventProperties
#define SOAP_TYPE__tev__GetEventProperties (1575)
/* tev:GetEventProperties */
struct _tev__GetEventProperties
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tev__GetEventPropertiesResponse
#define SOAP_TYPE__tev__GetEventPropertiesResponse (1576)
/* tev:GetEventPropertiesResponse */
struct _tev__GetEventPropertiesResponse
{
	int __sizeTopicNamespaceLocation;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* sequence of elements <TopicNamespaceLocation> */
	char **TopicNamespaceLocation;	/* required element of type xsd:string */
	enum xsd__boolean_ wsnt__FixedTopicSet;	/* required element of type xsd:boolean */
	struct wstop__TopicSetType *wstop__TopicSet;	/* required element of type wstop:TopicSetType */
	int __sizeTopicExpressionDialect;	/* sequence of elements <wsnt:TopicExpressionDialect> */
	char **wsnt__TopicExpressionDialect;	/* required element of type xsd:string */
	int __sizeMessageContentFilterDialect;	/* sequence of elements <MessageContentFilterDialect> */
	char **MessageContentFilterDialect;	/* required element of type xsd:string */
	int __sizeProducerPropertiesFilterDialect;	/* sequence of elements <ProducerPropertiesFilterDialect> */
	char **ProducerPropertiesFilterDialect;	/* optional element of type xsd:string */
	int __sizeMessageContentSchemaLocation;	/* sequence of elements <MessageContentSchemaLocation> */
	char **MessageContentSchemaLocation;	/* required element of type xsd:string */
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif


#ifndef SOAP_TYPE___dee__GetEventProperties
#define SOAP_TYPE___dee__GetEventProperties (2132)
/* Operation wrapper: */
struct __dee__GetEventProperties
{
	struct _tev__GetEventProperties *tev__GetEventProperties;	/* optional element of type tev:GetEventProperties */
};
#endif

#ifndef SOAP_TYPE__tev__CreatePullPointSubscription_SubscriptionPolicy
#define SOAP_TYPE__tev__CreatePullPointSubscription_SubscriptionPolicy (1567)
/* tev:CreatePullPointSubscription-SubscriptionPolicy */
struct _tev__CreatePullPointSubscription_SubscriptionPolicy
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif


#ifndef SOAP_TYPE__tev__CreatePullPointSubscription
#define SOAP_TYPE__tev__CreatePullPointSubscription (1566)
/* tev:CreatePullPointSubscription */
struct _tev__CreatePullPointSubscription
{
	struct wsnt__FilterType *Filter;	/* optional element of type wsnt:FilterType */
	char *InitialTerminationTime;	/* optional element of type wsnt:AbsoluteOrRelativeTimeType */
	struct _tev__CreatePullPointSubscription_SubscriptionPolicy *SubscriptionPolicy;	/* optional element of type tev:CreatePullPointSubscription-SubscriptionPolicy */
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE__tev__CreatePullPointSubscriptionResponse
#define SOAP_TYPE__tev__CreatePullPointSubscriptionResponse (1569)
/* tev:CreatePullPointSubscriptionResponse */
struct _tev__CreatePullPointSubscriptionResponse
{
	struct wsa5__EndpointReferenceType SubscriptionReference;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type wsa5:EndpointReferenceType */
	time_t wsnt__CurrentTime;	/* required element of type xsd:dateTime */
	time_t wsnt__TerminationTime;	/* required element of type xsd:dateTime */
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE__tev__PullMessages
#define SOAP_TYPE__tev__PullMessages (1570)
/* tev:PullMessages */
struct _tev__PullMessages
{
	char *Timeout;	/* required element of type xsd:string */
	int MessageLimit;	/* required element of type xsd:int */
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE__tev__PullMessagesResponse
#define SOAP_TYPE__tev__PullMessagesResponse (1571)
/* tev:PullMessagesResponse */
struct _tev__PullMessagesResponse
{
	time_t CurrentTime;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type xsd:dateTime */
	time_t TerminationTime;	/* required element of type xsd:dateTime */
	int __sizeNotificationMessage;	/* sequence of elements <wsnt:NotificationMessage> */
	struct wsnt__NotificationMessageHolderType *wsnt__NotificationMessage;	/* optional element of type wsnt:NotificationMessageHolderType */
};
#endif

#ifndef SOAP_TYPE__tev__PullMessagesFaultResponse
#define SOAP_TYPE__tev__PullMessagesFaultResponse (1572)
/* tev:PullMessagesFaultResponse */
struct _tev__PullMessagesFaultResponse
{
	char *MaxTimeout;	/* required element of type xsd:string */
	int MaxMessageLimit;	/* required element of type xsd:int */
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE__tev__SetSynchronizationPoint
#define SOAP_TYPE__tev__SetSynchronizationPoint (1573)
/* tev:SetSynchronizationPoint */
struct _tev__SetSynchronizationPoint
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tev__SetSynchronizationPointResponse
#define SOAP_TYPE__tev__SetSynchronizationPointResponse (1574)
/* tev:SetSynchronizationPointResponse */
struct _tev__SetSynchronizationPointResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE___depps__PullMessages
#define SOAP_TYPE___depps__PullMessages (2157)
/* Operation wrapper: */
struct __depps__PullMessages
{
	struct _tev__PullMessages *tev__PullMessages;	/* optional element of type tev:PullMessages */
};
#endif

#ifndef SOAP_TYPE___depps__SetSynchronizationPoint
#define SOAP_TYPE___depps__SetSynchronizationPoint (2161)
/* Operation wrapper: */
struct __depps__SetSynchronizationPoint
{
	struct _tev__SetSynchronizationPoint *tev__SetSynchronizationPoint;	/* optional element of type tev:SetSynchronizationPoint */
};
#endif


#ifndef SOAP_TYPE___dee__CreatePullPointSubscription
#define SOAP_TYPE___dee__CreatePullPointSubscription (2128)
/* Operation wrapper: */
struct __dee__CreatePullPointSubscription
{
	struct _tev__CreatePullPointSubscription *tev__CreatePullPointSubscription;	/* optional element of type tev:CreatePullPointSubscription */
};
#endif


SOAP_FMAC5 int SOAP_FMAC6 soap_call___dee__GetEventProperties(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tev__GetEventProperties *tev__GetEventProperties, struct _tev__GetEventPropertiesResponse *tev__GetEventPropertiesResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___dee__CreatePullPointSubscription(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tev__CreatePullPointSubscription *tev__CreatePullPointSubscription, struct _tev__CreatePullPointSubscriptionResponse *tev__CreatePullPointSubscriptionResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___depps__PullMessages(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tev__PullMessages *tev__PullMessages, struct _tev__PullMessagesResponse *tev__PullMessagesResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___depps__SetSynchronizationPoint(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tev__SetSynchronizationPoint *tev__SetSynchronizationPoint, struct _tev__SetSynchronizationPointResponse *tev__SetSynchronizationPointResponse);


SOAP_FMAC5 int SOAP_FMAC6 soap_call___denf__Subscribe(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _wsnt__Subscribe *wsnt__Subscribe, struct _wsnt__SubscribeResponse *wsnt__SubscribeResponse);
SOAP_FMAC5 int SOAP_FMAC6 soap_call___desm__Renew(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _wsnt__Renew *wsnt__Renew, struct _wsnt__RenewResponse *wsnt__RenewResponse);
SOAP_FMAC5 int SOAP_FMAC6 soap_recv___denc__Notify(struct soap *soap, struct __denc__Notify *_param_1);
SOAP_FMAC5 int SOAP_FMAC6 soap_call___desm__Unsubscribe(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _wsnt__Unsubscribe *wsnt__Unsubscribe, struct _wsnt__UnsubscribeResponse *wsnt__UnsubscribeResponse);



///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef SOAP_TYPE_tt__ConfigDescription
#define SOAP_TYPE_tt__ConfigDescription (831)
/* tt:ConfigDescription */
struct tt__ConfigDescription
{
	struct tt__ItemListDescription *Parameters;	/* required element of type tt:ItemListDescription */
	int __sizeMessages;	/* sequence of elements <Messages> */
	struct _tt__ConfigDescription_Messages *Messages;	/* optional element of type tt:ConfigDescription-Messages */
	struct tt__ConfigDescriptionExtension *Extension;	/* optional element of type tt:ConfigDescriptionExtension */
	char *Name;	/* required attribute of type xsd:QName */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif


#ifndef SOAP_TYPE__tan__GetSupportedRules
#define SOAP_TYPE__tan__GetSupportedRules (231)
/* tan:GetSupportedRules */
struct _tan__GetSupportedRules
{
	char *ConfigurationToken;	/* required element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE__tan__GetSupportedRulesResponse
#define SOAP_TYPE__tan__GetSupportedRulesResponse (232)
/* tan:GetSupportedRulesResponse */
struct _tan__GetSupportedRulesResponse
{
	struct tt__SupportedRules *SupportedRules;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type tt:SupportedRules */
};
#endif

#ifndef SOAP_TYPE__tan__CreateRules
#define SOAP_TYPE__tan__CreateRules (235)
/* tan:CreateRules */
struct _tan__CreateRules
{
	char *ConfigurationToken;	/* required element of type tt:ReferenceToken */
	int __sizeRule;	/* sequence of elements <Rule> */
	struct tt__Config *Rule;	/* required element of type tt:Config */
};
#endif

#ifndef SOAP_TYPE__tan__CreateRulesResponse
#define SOAP_TYPE__tan__CreateRulesResponse (238)
/* tan:CreateRulesResponse */
struct _tan__CreateRulesResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tan__DeleteRules
#define SOAP_TYPE__tan__DeleteRules (239)
/* tan:DeleteRules */
struct _tan__DeleteRules
{
	char *ConfigurationToken;	/* required element of type tt:ReferenceToken */
	int __sizeRuleName;	/* sequence of elements <RuleName> */
	char **RuleName;	/* required element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE__tan__DeleteRulesResponse
#define SOAP_TYPE__tan__DeleteRulesResponse (241)
/* tan:DeleteRulesResponse */
struct _tan__DeleteRulesResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tan__ModifyRules
#define SOAP_TYPE__tan__ModifyRules (242)
/* tan:ModifyRules */
struct _tan__ModifyRules
{
	char *ConfigurationToken;	/* required element of type tt:ReferenceToken */
	int __sizeRule;	/* sequence of elements <Rule> */
	struct tt__Config *Rule;	/* required element of type tt:Config */
};
#endif

#ifndef SOAP_TYPE__tan__ModifyRulesResponse
#define SOAP_TYPE__tan__ModifyRulesResponse (243)
/* tan:ModifyRulesResponse */
struct _tan__ModifyRulesResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tan__GetRules
#define SOAP_TYPE__tan__GetRules (244)
/* tan:GetRules */
struct _tan__GetRules
{
	char *ConfigurationToken;	/* required element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE__tan__GetRulesResponse
#define SOAP_TYPE__tan__GetRulesResponse (245)
/* tan:GetRulesResponse */
struct _tan__GetRulesResponse
{
	int __sizeRule;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* sequence of elements <Rule> */
	struct tt__Config *Rule;	/* optional element of type tt:Config */
};
#endif

#ifndef SOAP_TYPE__tan__GetSupportedAnalyticsModules
#define SOAP_TYPE__tan__GetSupportedAnalyticsModules (246)
/* tan:GetSupportedAnalyticsModules */
struct _tan__GetSupportedAnalyticsModules
{
	char *ConfigurationToken;	/* required element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE__tan__GetSupportedAnalyticsModulesResponse
#define SOAP_TYPE__tan__GetSupportedAnalyticsModulesResponse (247)
/* tan:GetSupportedAnalyticsModulesResponse */
struct _tan__GetSupportedAnalyticsModulesResponse
{
	struct tt__SupportedAnalyticsModules *SupportedAnalyticsModules;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* required element of type tt:SupportedAnalyticsModules */
};
#endif

#ifndef SOAP_TYPE__tan__CreateAnalyticsModules
#define SOAP_TYPE__tan__CreateAnalyticsModules (250)
/* tan:CreateAnalyticsModules */
struct _tan__CreateAnalyticsModules
{
	char *ConfigurationToken;	/* required element of type tt:ReferenceToken */
	int __sizeAnalyticsModule;	/* sequence of elements <AnalyticsModule> */
	struct tt__Config *AnalyticsModule;	/* required element of type tt:Config */
};
#endif

#ifndef SOAP_TYPE__tan__CreateAnalyticsModulesResponse
#define SOAP_TYPE__tan__CreateAnalyticsModulesResponse (251)
/* tan:CreateAnalyticsModulesResponse */
struct _tan__CreateAnalyticsModulesResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tan__DeleteAnalyticsModules
#define SOAP_TYPE__tan__DeleteAnalyticsModules (252)
/* tan:DeleteAnalyticsModules */
struct _tan__DeleteAnalyticsModules
{
	char *ConfigurationToken;	/* required element of type tt:ReferenceToken */
	int __sizeAnalyticsModuleName;	/* sequence of elements <AnalyticsModuleName> */
	char **AnalyticsModuleName;	/* required element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE__tan__DeleteAnalyticsModulesResponse
#define SOAP_TYPE__tan__DeleteAnalyticsModulesResponse (253)
/* tan:DeleteAnalyticsModulesResponse */
struct _tan__DeleteAnalyticsModulesResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tan__ModifyAnalyticsModules
#define SOAP_TYPE__tan__ModifyAnalyticsModules (254)
/* tan:ModifyAnalyticsModules */
struct _tan__ModifyAnalyticsModules
{
	char *ConfigurationToken;	/* required element of type tt:ReferenceToken */
	int __sizeAnalyticsModule;	/* sequence of elements <AnalyticsModule> */
	struct tt__Config *AnalyticsModule;	/* required element of type tt:Config */
};
#endif

#ifndef SOAP_TYPE__tan__ModifyAnalyticsModulesResponse
#define SOAP_TYPE__tan__ModifyAnalyticsModulesResponse (255)
/* tan:ModifyAnalyticsModulesResponse */
struct _tan__ModifyAnalyticsModulesResponse
{
#if 1//def WITH_NOEMPTYSTRUCT
	char dummy;	/* dummy member to enable compilation */
#endif
};
#endif

#ifndef SOAP_TYPE__tan__GetAnalyticsModules
#define SOAP_TYPE__tan__GetAnalyticsModules (256)
/* tan:GetAnalyticsModules */
struct _tan__GetAnalyticsModules
{
	char *ConfigurationToken;	/* required element of type tt:ReferenceToken */
};
#endif

#ifndef SOAP_TYPE__tan__GetAnalyticsModulesResponse
#define SOAP_TYPE__tan__GetAnalyticsModulesResponse (257)
/* tan:GetAnalyticsModulesResponse */
struct _tan__GetAnalyticsModulesResponse
{
	int __sizeAnalyticsModule;	/* SOAP 1.2 RPC return element (when namespace qualified) */	/* sequence of elements <AnalyticsModule> */
	struct tt__Config *AnalyticsModule;	/* optional element of type tt:Config */
};
#endif


#ifndef SOAP_TYPE_tt__SupportedRules
#define SOAP_TYPE_tt__SupportedRules (233)
/* tt:SupportedRules */
struct tt__SupportedRules
{
	int __sizeRuleContentSchemaLocation;	/* sequence of elements <RuleContentSchemaLocation> */
	char **RuleContentSchemaLocation;	/* optional element of type xsd:string */
	int __sizeRuleDescription;	/* sequence of elements <RuleDescription> */
	struct tt__ConfigDescription *RuleDescription;	/* optional element of type tt:ConfigDescription */
	struct tt__SupportedRulesExtension *Extension;	/* optional element of type tt:SupportedRulesExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__SupportedRulesExtension
#define SOAP_TYPE_tt__SupportedRulesExtension (837)
/* tt:SupportedRulesExtension */
struct tt__SupportedRulesExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif

#ifndef SOAP_TYPE_tt__SupportedAnalyticsModules
#define SOAP_TYPE_tt__SupportedAnalyticsModules (248)
/* tt:SupportedAnalyticsModules */
struct tt__SupportedAnalyticsModules
{
	int __sizeAnalyticsModuleContentSchemaLocation;	/* sequence of elements <AnalyticsModuleContentSchemaLocation> */
	char **AnalyticsModuleContentSchemaLocation;	/* optional element of type xsd:string */
	int __sizeAnalyticsModuleDescription;	/* sequence of elements <AnalyticsModuleDescription> */
	struct tt__ConfigDescription *AnalyticsModuleDescription;	/* optional element of type tt:ConfigDescription */
	struct tt__SupportedAnalyticsModulesExtension *Extension;	/* optional element of type tt:SupportedAnalyticsModulesExtension */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__SupportedAnalyticsModulesExtension
#define SOAP_TYPE_tt__SupportedAnalyticsModulesExtension (839)
/* tt:SupportedAnalyticsModulesExtension */
struct tt__SupportedAnalyticsModulesExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif



#ifndef SOAP_TYPE___daae__GetSupportedAnalyticsModules
#define SOAP_TYPE___daae__GetSupportedAnalyticsModules (1726)
/* Operation wrapper: */
struct __daae__GetSupportedAnalyticsModules
{
	struct _tan__GetSupportedAnalyticsModules *tan__GetSupportedAnalyticsModules;	/* optional element of type tan:GetSupportedAnalyticsModules */
};
#endif

#ifndef SOAP_TYPE___daae__CreateAnalyticsModules
#define SOAP_TYPE___daae__CreateAnalyticsModules (1730)
/* Operation wrapper: */
struct __daae__CreateAnalyticsModules
{
	struct _tan__CreateAnalyticsModules *tan__CreateAnalyticsModules;	/* optional element of type tan:CreateAnalyticsModules */
};
#endif

#ifndef SOAP_TYPE___daae__DeleteAnalyticsModules
#define SOAP_TYPE___daae__DeleteAnalyticsModules (1734)
/* Operation wrapper: */
struct __daae__DeleteAnalyticsModules
{
	struct _tan__DeleteAnalyticsModules *tan__DeleteAnalyticsModules;	/* optional element of type tan:DeleteAnalyticsModules */
};
#endif

#ifndef SOAP_TYPE___daae__GetAnalyticsModules
#define SOAP_TYPE___daae__GetAnalyticsModules (1738)
/* Operation wrapper: */
struct __daae__GetAnalyticsModules
{
	struct _tan__GetAnalyticsModules *tan__GetAnalyticsModules;	/* optional element of type tan:GetAnalyticsModules */
};
#endif

#ifndef SOAP_TYPE___daae__ModifyAnalyticsModules
#define SOAP_TYPE___daae__ModifyAnalyticsModules (1742)
/* Operation wrapper: */
struct __daae__ModifyAnalyticsModules
{
	struct _tan__ModifyAnalyticsModules *tan__ModifyAnalyticsModules;	/* optional element of type tan:ModifyAnalyticsModules */
};
#endif

#ifndef SOAP_TYPE___dare__GetSupportedRules
#define SOAP_TYPE___dare__GetSupportedRules (1746)
/* Operation wrapper: */
struct __dare__GetSupportedRules
{
	struct _tan__GetSupportedRules *tan__GetSupportedRules;	/* optional element of type tan:GetSupportedRules */
};
#endif

#ifndef SOAP_TYPE___dare__CreateRules
#define SOAP_TYPE___dare__CreateRules (1750)
/* Operation wrapper: */
struct __dare__CreateRules
{
	struct _tan__CreateRules *tan__CreateRules;	/* optional element of type tan:CreateRules */
};
#endif

#ifndef SOAP_TYPE___dare__DeleteRules
#define SOAP_TYPE___dare__DeleteRules (1754)
/* Operation wrapper: */
struct __dare__DeleteRules
{
	struct _tan__DeleteRules *tan__DeleteRules;	/* optional element of type tan:DeleteRules */
};
#endif

#ifndef SOAP_TYPE___dare__GetRules
#define SOAP_TYPE___dare__GetRules (1758)
/* Operation wrapper: */
struct __dare__GetRules
{
	struct _tan__GetRules *tan__GetRules;	/* optional element of type tan:GetRules */
};
#endif

#ifndef SOAP_TYPE___dare__ModifyRules
#define SOAP_TYPE___dare__ModifyRules (1762)
/* Operation wrapper: */
struct __dare__ModifyRules
{
	struct _tan__ModifyRules *tan__ModifyRules;	/* optional element of type tan:ModifyRules */
};
#endif


#ifndef SOAP_TYPE_tt__Config
#define SOAP_TYPE_tt__Config (236)
/* tt:Config */
struct tt__Config
{
	struct tt__ItemList *Parameters;	/* required element of type tt:ItemList */
	char *Name;	/* required attribute of type xsd:string */
	char *Type;	/* required attribute of type xsd:QName */
};
#endif

#ifndef SOAP_TYPE__tt__ConfigDescription_Messages
#define SOAP_TYPE__tt__ConfigDescription_Messages (832)
/* tt:ConfigDescription-Messages */
struct _tt__ConfigDescription_Messages
{
	struct tt__ItemListDescription *Source;	/* optional element of type tt:ItemListDescription */
	struct tt__ItemListDescription *Key;	/* optional element of type tt:ItemListDescription */
	struct tt__ItemListDescription *Data;	/* optional element of type tt:ItemListDescription */
	struct tt__MessageDescriptionExtension *Extension;	/* optional element of type tt:MessageDescriptionExtension */
	enum xsd__boolean_ *IsProperty;	/* optional attribute of type xsd:boolean */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
	char *ParentTopic;	/* required element of type xsd:string */
};
#endif

#ifndef SOAP_TYPE_tt__ConfigDescription
#define SOAP_TYPE_tt__ConfigDescription (831)
/* tt:ConfigDescription */
struct tt__ConfigDescription
{
	struct tt__ItemListDescription *Parameters;	/* required element of type tt:ItemListDescription */
	int __sizeMessages;	/* sequence of elements <Messages> */
	struct _tt__ConfigDescription_Messages *Messages;	/* optional element of type tt:ConfigDescription-Messages */
	struct tt__ConfigDescriptionExtension *Extension;	/* optional element of type tt:ConfigDescriptionExtension */
	char *Name;	/* required attribute of type xsd:QName */
	char *__anyAttribute;	/* optional attribute of type xsd:anyType */
};
#endif

#ifndef SOAP_TYPE_tt__ConfigDescriptionExtension
#define SOAP_TYPE_tt__ConfigDescriptionExtension (834)
/* tt:ConfigDescriptionExtension */
struct tt__ConfigDescriptionExtension
{
	int __size;	/* sequence of elements <-any> */
	char **__any;
};
#endif



SOAP_FMAC5 int SOAP_FMAC6 soap_call___dare__ModifyRules(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__ModifyRules *tan__ModifyRules, struct _tan__ModifyRulesResponse *tan__ModifyRulesResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___daae__GetAnalyticsModules(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__GetAnalyticsModules *tan__GetAnalyticsModules, struct _tan__GetAnalyticsModulesResponse *tan__GetAnalyticsModulesResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___dare__GetRules(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__GetRules *tan__GetRules, struct _tan__GetRulesResponse *tan__GetRulesResponse);

SOAP_FMAC5 int SOAP_FMAC6 soap_call___daae__ModifyAnalyticsModules(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__ModifyAnalyticsModules *tan__ModifyAnalyticsModules, struct _tan__ModifyAnalyticsModulesResponse *tan__ModifyAnalyticsModulesResponse);

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////



#ifdef __cplusplus
}
#endif

#endif