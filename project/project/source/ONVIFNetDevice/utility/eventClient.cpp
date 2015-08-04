#if defined(__BORLANDC__)
#pragma option push -w-8060
#pragma option push -w-8004
#endif
#include "eventH.h"

#ifdef __cplusplus
extern "C" {
#endif

SOAP_FMAC5 int SOAP_FMAC6 soap_recv___denc__Notify(struct soap *soap, struct __denc__Notify *_param_1)
{
	soap_default___denc__Notify(soap, _param_1);
	soap_begin(soap);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get___denc__Notify(soap, _param_1, "-denc:Notify", NULL);
	if (soap->error == SOAP_TAG_MISMATCH && soap->level == 2)
		soap->error = SOAP_NO_METHOD;
	if (soap->error
	 || soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___denf__Subscribe(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _wsnt__Subscribe *wsnt__Subscribe, struct _wsnt__SubscribeResponse *wsnt__SubscribeResponse)
{	
	struct __denf__Subscribe soap_tmp___denf__Subscribe;
	if (!soap_action)
		soap_action = "http://docs.oasis-open.org/wsn/bw-2/NotificationProducer/SubscribeRequest";
	soap->encodingStyle = NULL;
	soap_tmp___denf__Subscribe.wsnt__Subscribe = wsnt__Subscribe;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___denf__Subscribe(soap, &soap_tmp___denf__Subscribe);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___denf__Subscribe(soap, &soap_tmp___denf__Subscribe, "-denf:Subscribe", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___denf__Subscribe(soap, &soap_tmp___denf__Subscribe, "-denf:Subscribe", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!wsnt__SubscribeResponse)
		return soap_closesock(soap);
	soap_default__wsnt__SubscribeResponse(soap, wsnt__SubscribeResponse);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__wsnt__SubscribeResponse(soap, wsnt__SubscribeResponse, "wsnt:SubscribeResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___desm__Renew(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _wsnt__Renew *wsnt__Renew, struct _wsnt__RenewResponse *wsnt__RenewResponse)
{	struct __desm__Renew soap_tmp___desm__Renew;
	if (!soap_action)
		soap_action = "http://docs.oasis-open.org/wsn/bw-2/SubscriptionManager/RenewRequest";
	soap->encodingStyle = NULL;
	soap_tmp___desm__Renew.wsnt__Renew = wsnt__Renew;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___desm__Renew(soap, &soap_tmp___desm__Renew);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___desm__Renew(soap, &soap_tmp___desm__Renew, "-desm:Renew", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___desm__Renew(soap, &soap_tmp___desm__Renew, "-desm:Renew", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!wsnt__RenewResponse)
		return soap_closesock(soap);
	soap_default__wsnt__RenewResponse(soap, wsnt__RenewResponse);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__wsnt__RenewResponse(soap, wsnt__RenewResponse, "wsnt:RenewResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___desm__Unsubscribe(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _wsnt__Unsubscribe *wsnt__Unsubscribe, struct _wsnt__UnsubscribeResponse *wsnt__UnsubscribeResponse)
{	struct __desm__Unsubscribe soap_tmp___desm__Unsubscribe;
	if (!soap_action)
		soap_action = "http://docs.oasis-open.org/wsn/bw-2/SubscriptionManager/UnsubscribeRequest";
	soap->encodingStyle = NULL;
	soap_tmp___desm__Unsubscribe.wsnt__Unsubscribe = wsnt__Unsubscribe;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___desm__Unsubscribe(soap, &soap_tmp___desm__Unsubscribe);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___desm__Unsubscribe(soap, &soap_tmp___desm__Unsubscribe, "-desm:Unsubscribe", NULL)
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put___desm__Unsubscribe(soap, &soap_tmp___desm__Unsubscribe, "-desm:Unsubscribe", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!wsnt__UnsubscribeResponse)
		return soap_closesock(soap);
	soap_default__wsnt__UnsubscribeResponse(soap, wsnt__UnsubscribeResponse);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__wsnt__UnsubscribeResponse(soap, wsnt__UnsubscribeResponse, "wsnt:UnsubscribeResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___dee__GetEventProperties(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tev__GetEventProperties *tev__GetEventProperties, struct _tev__GetEventPropertiesResponse *tev__GetEventPropertiesResponse)
{	
	struct __dee__GetEventProperties soap_tmp___dee__GetEventProperties;
	if (!soap_action)
		soap_action = "http://www.onvif.org/ver10/events/wsdl/EventPortType/GetEventPropertiesRequest";
	soap->encodingStyle = NULL;
	soap_tmp___dee__GetEventProperties.tev__GetEventProperties = tev__GetEventProperties;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___dee__GetEventProperties(soap, &soap_tmp___dee__GetEventProperties);
	if (soap_begin_count(soap))
	return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___dee__GetEventProperties(soap, &soap_tmp___dee__GetEventProperties, "-dee:GetEventProperties", NULL)
			|| soap_body_end_out(soap)
			|| soap_envelope_end_out(soap))
			return soap->error;
	}
	if (soap_end_count(soap))
	return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
		|| soap_envelope_begin_out(soap)
		|| soap_putheader(soap)
		|| soap_body_begin_out(soap)
		|| soap_put___dee__GetEventProperties(soap, &soap_tmp___dee__GetEventProperties, "-dee:GetEventProperties", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tev__GetEventPropertiesResponse)
	return soap_closesock(soap);
	soap_default__tev__GetEventPropertiesResponse(soap, tev__GetEventPropertiesResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tev__GetEventPropertiesResponse(soap, tev__GetEventPropertiesResponse, "tev:GetEventPropertiesResponse", "");
	if (soap->error)
	return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___dee__CreatePullPointSubscription(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tev__CreatePullPointSubscription *tev__CreatePullPointSubscription, struct _tev__CreatePullPointSubscriptionResponse *tev__CreatePullPointSubscriptionResponse)
{	
	struct __dee__CreatePullPointSubscription soap_tmp___dee__CreatePullPointSubscription;
	if (!soap_action)
	soap_action = "http://www.onvif.org/ver10/events/wsdl/EventPortType/CreatePullPointSubscriptionRequest";
	soap->encodingStyle = NULL;
	soap_tmp___dee__CreatePullPointSubscription.tev__CreatePullPointSubscription = tev__CreatePullPointSubscription;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___dee__CreatePullPointSubscription(soap, &soap_tmp___dee__CreatePullPointSubscription);
	if (soap_begin_count(soap))
	return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___dee__CreatePullPointSubscription(soap, &soap_tmp___dee__CreatePullPointSubscription, "-dee:CreatePullPointSubscription", NULL)
			|| soap_body_end_out(soap)
			|| soap_envelope_end_out(soap))
			return soap->error;
	}
	if (soap_end_count(soap))
	return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
		|| soap_envelope_begin_out(soap)
		|| soap_putheader(soap)
		|| soap_body_begin_out(soap)
		|| soap_put___dee__CreatePullPointSubscription(soap, &soap_tmp___dee__CreatePullPointSubscription, "-dee:CreatePullPointSubscription", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tev__CreatePullPointSubscriptionResponse)
	return soap_closesock(soap);
	soap_default__tev__CreatePullPointSubscriptionResponse(soap, tev__CreatePullPointSubscriptionResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tev__CreatePullPointSubscriptionResponse(soap, tev__CreatePullPointSubscriptionResponse, "tev:CreatePullPointSubscriptionResponse", "");
	if (soap->error)
	return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}


SOAP_FMAC5 int SOAP_FMAC6 soap_call___depps__PullMessages(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tev__PullMessages *tev__PullMessages, struct _tev__PullMessagesResponse *tev__PullMessagesResponse)
{	
	struct __depps__PullMessages soap_tmp___depps__PullMessages;
	if (!soap_action)
		soap_action = "http://www.onvif.org/ver10/events/wsdl/PullPointSubscription/PullMessagesRequest";
	soap->encodingStyle = NULL;
	soap_tmp___depps__PullMessages.tev__PullMessages = tev__PullMessages;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___depps__PullMessages(soap, &soap_tmp___depps__PullMessages);
	if (soap_begin_count(soap))
	return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___depps__PullMessages(soap, &soap_tmp___depps__PullMessages, "-depps:PullMessages", NULL)
			|| soap_body_end_out(soap)
			|| soap_envelope_end_out(soap))
			return soap->error;
	}
	if (soap_end_count(soap))
	return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
		|| soap_envelope_begin_out(soap)
		|| soap_putheader(soap)
		|| soap_body_begin_out(soap)
		|| soap_put___depps__PullMessages(soap, &soap_tmp___depps__PullMessages, "-depps:PullMessages", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tev__PullMessagesResponse)
	return soap_closesock(soap);
	soap_default__tev__PullMessagesResponse(soap, tev__PullMessagesResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tev__PullMessagesResponse(soap, tev__PullMessagesResponse, "tev:PullMessagesResponse", "");
	if (soap->error)
	return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

#if 1
SOAP_FMAC5 int SOAP_FMAC6 soap_call___depps__SetSynchronizationPoint(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tev__SetSynchronizationPoint *tev__SetSynchronizationPoint, struct _tev__SetSynchronizationPointResponse *tev__SetSynchronizationPointResponse)
{	
	struct __depps__SetSynchronizationPoint soap_tmp___depps__SetSynchronizationPoint;
	if (!soap_action)
		soap_action = "http://www.onvif.org/ver10/events/wsdl/PullPointSubscription/SetSynchronizationPointRequest";
	soap->encodingStyle = NULL;
	soap_tmp___depps__SetSynchronizationPoint.tev__SetSynchronizationPoint = tev__SetSynchronizationPoint;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___depps__SetSynchronizationPoint(soap, &soap_tmp___depps__SetSynchronizationPoint);
	if (soap_begin_count(soap))
	return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___depps__SetSynchronizationPoint(soap, &soap_tmp___depps__SetSynchronizationPoint, "-depps:SetSynchronizationPoint", NULL)
			|| soap_body_end_out(soap)
			|| soap_envelope_end_out(soap))
			return soap->error;
	}
	if (soap_end_count(soap))
	return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
		|| soap_envelope_begin_out(soap)
		|| soap_putheader(soap)
		|| soap_body_begin_out(soap)
		|| soap_put___depps__SetSynchronizationPoint(soap, &soap_tmp___depps__SetSynchronizationPoint, "-depps:SetSynchronizationPoint", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tev__SetSynchronizationPointResponse)
	return soap_closesock(soap);
	soap_default__tev__SetSynchronizationPointResponse(soap, tev__SetSynchronizationPointResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tev__SetSynchronizationPointResponse(soap, tev__SetSynchronizationPointResponse, "tev:SetSynchronizationPointResponse", "");
	if (soap->error)
	return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SOAP_FMAC5 int SOAP_FMAC6 soap_call___dare__GetRules(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__GetRules *tan__GetRules, struct _tan__GetRulesResponse *tan__GetRulesResponse)
{	
	struct __dare__GetRules soap_tmp___dare__GetRules;
	if (!soap_action)
	soap_action = "http://www.onvif.org/ver20/analytics/wsdl/GetRules";
	soap->encodingStyle = NULL;
	soap_tmp___dare__GetRules.tan__GetRules = tan__GetRules;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___dare__GetRules(soap, &soap_tmp___dare__GetRules);
	if (soap_begin_count(soap))
	return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___dare__GetRules(soap, &soap_tmp___dare__GetRules, "-dare:GetRules", NULL)
			|| soap_body_end_out(soap)
			|| soap_envelope_end_out(soap))
			return soap->error;
	}
	if (soap_end_count(soap))
	return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
		|| soap_envelope_begin_out(soap)
		|| soap_putheader(soap)
		|| soap_body_begin_out(soap)
		|| soap_put___dare__GetRules(soap, &soap_tmp___dare__GetRules, "-dare:GetRules", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tan__GetRulesResponse)
	return soap_closesock(soap);
	soap_default__tan__GetRulesResponse(soap, tan__GetRulesResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tan__GetRulesResponse(soap, tan__GetRulesResponse, "tan:GetRulesResponse", "");
	if (soap->error)
	return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}



SOAP_FMAC5 int SOAP_FMAC6 soap_call___daae__GetAnalyticsModules(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__GetAnalyticsModules *tan__GetAnalyticsModules, struct _tan__GetAnalyticsModulesResponse *tan__GetAnalyticsModulesResponse)
{	
	struct __daae__GetAnalyticsModules soap_tmp___daae__GetAnalyticsModules;
	if (!soap_action)
		soap_action = "http://www.onvif.org/ver20/analytics/wsdl/GetAnalyticsModules";
	soap->encodingStyle = NULL;
	soap_tmp___daae__GetAnalyticsModules.tan__GetAnalyticsModules = tan__GetAnalyticsModules;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___daae__GetAnalyticsModules(soap, &soap_tmp___daae__GetAnalyticsModules);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
	|| soap_putheader(soap)
	|| soap_body_begin_out(soap)
	|| soap_put___daae__GetAnalyticsModules(soap, &soap_tmp___daae__GetAnalyticsModules, "-daae:GetAnalyticsModules", NULL)
	|| soap_body_end_out(soap)
	|| soap_envelope_end_out(soap))
	return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
		|| soap_envelope_begin_out(soap)
		|| soap_putheader(soap)
		|| soap_body_begin_out(soap)
		|| soap_put___daae__GetAnalyticsModules(soap, &soap_tmp___daae__GetAnalyticsModules, "-daae:GetAnalyticsModules", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tan__GetAnalyticsModulesResponse)
		return soap_closesock(soap);
	soap_default__tan__GetAnalyticsModulesResponse(soap, tan__GetAnalyticsModulesResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tan__GetAnalyticsModulesResponse(soap, tan__GetAnalyticsModulesResponse, "tan:GetAnalyticsModulesResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}


SOAP_FMAC5 int SOAP_FMAC6 soap_call___dare__ModifyRules(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__ModifyRules *tan__ModifyRules, struct _tan__ModifyRulesResponse *tan__ModifyRulesResponse)
{	
	struct __dare__ModifyRules soap_tmp___dare__ModifyRules;
	if (!soap_action)
	soap_action = "http://www.onvif.org/ver20/analytics/wsdl/ModifyRules";
	soap->encodingStyle = NULL;
	soap_tmp___dare__ModifyRules.tan__ModifyRules = tan__ModifyRules;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___dare__ModifyRules(soap, &soap_tmp___dare__ModifyRules);
	if (soap_begin_count(soap))
	return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___dare__ModifyRules(soap, &soap_tmp___dare__ModifyRules, "-dare:ModifyRules", NULL)
			|| soap_body_end_out(soap)
			|| soap_envelope_end_out(soap))
			return soap->error;
	}
	if (soap_end_count(soap))
	return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
		|| soap_envelope_begin_out(soap)
		|| soap_putheader(soap)
		|| soap_body_begin_out(soap)
		|| soap_put___dare__ModifyRules(soap, &soap_tmp___dare__ModifyRules, "-dare:ModifyRules", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tan__ModifyRulesResponse)
	return soap_closesock(soap);
	soap_default__tan__ModifyRulesResponse(soap, tan__ModifyRulesResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tan__ModifyRulesResponse(soap, tan__ModifyRulesResponse, "tan:ModifyRulesResponse", "");
	if (soap->error)
	return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}


SOAP_FMAC5 int SOAP_FMAC6 soap_call___daae__ModifyAnalyticsModules(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tan__ModifyAnalyticsModules *tan__ModifyAnalyticsModules, struct _tan__ModifyAnalyticsModulesResponse *tan__ModifyAnalyticsModulesResponse)
{	
	struct __daae__ModifyAnalyticsModules soap_tmp___daae__ModifyAnalyticsModules;
	if (!soap_action)
	soap_action = "http://www.onvif.org/ver20/analytics/wsdl/ModifyAnalyticsModules";
	soap->encodingStyle = NULL;
	soap_tmp___daae__ModifyAnalyticsModules.tan__ModifyAnalyticsModules = tan__ModifyAnalyticsModules;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___daae__ModifyAnalyticsModules(soap, &soap_tmp___daae__ModifyAnalyticsModules);
	if (soap_begin_count(soap))
	return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___daae__ModifyAnalyticsModules(soap, &soap_tmp___daae__ModifyAnalyticsModules, "-daae:ModifyAnalyticsModules", NULL)
			|| soap_body_end_out(soap)
			|| soap_envelope_end_out(soap))
			return soap->error;
	}
	if (soap_end_count(soap))
	return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
		|| soap_envelope_begin_out(soap)
		|| soap_putheader(soap)
		|| soap_body_begin_out(soap)
		|| soap_put___daae__ModifyAnalyticsModules(soap, &soap_tmp___daae__ModifyAnalyticsModules, "-daae:ModifyAnalyticsModules", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tan__ModifyAnalyticsModulesResponse)
	return soap_closesock(soap);
	soap_default__tan__ModifyAnalyticsModulesResponse(soap, tan__ModifyAnalyticsModulesResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tan__ModifyAnalyticsModulesResponse(soap, tan__ModifyAnalyticsModulesResponse, "tan:ModifyAnalyticsModulesResponse", "");
	if (soap->error)
	return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}



#ifdef __cplusplus
}
#endif

#if defined(__BORLANDC__)
#pragma option pop
#pragma option pop
#endif