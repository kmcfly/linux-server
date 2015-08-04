#include "eventH.h"

#ifdef __cplusplus
extern "C" {
#endif

SOAP_FMAC3 void SOAP_FMAC4 soap_default___denc__Notify(struct soap *soap, struct __denc__Notify *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->wsnt__Notify = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___denc__Notify(struct soap *soap, const struct __denc__Notify *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_wsnt__Notify(soap, &a->wsnt__Notify);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___denc__Notify(struct soap *soap, const char *tag, int id, const struct __denc__Notify *a, const char *type)
{
	if (soap_out_PointerTo_wsnt__Notify(soap, "wsnt:Notify", -1, &a->wsnt__Notify, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __denc__Notify * SOAP_FMAC4 soap_in___denc__Notify(struct soap *soap, const char *tag, struct __denc__Notify *a, const char *type)
{
	size_t soap_flag_wsnt__Notify = 1;
	short soap_flag;
	a = (struct __denc__Notify *)soap_id_enter(soap, "", a, SOAP_TYPE___denc__Notify, sizeof(struct __denc__Notify), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___denc__Notify(soap, a);
		for (soap_flag = 0;; soap_flag = 1)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_wsnt__Notify && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTo_wsnt__Notify(soap, "wsnt:Notify", &a->wsnt__Notify, ""))
				{	soap_flag_wsnt__Notify--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				if (soap_flag)
				{	soap->error = SOAP_OK;
					break;
				}
			if (soap_flag && soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put___denc__Notify(struct soap *soap, const struct __denc__Notify *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___denc__Notify(soap, tag?tag:"-denc:Notify", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __denc__Notify * SOAP_FMAC4 soap_get___denc__Notify(struct soap *soap, struct __denc__Notify *p, const char *tag, const char *type)
{
	if ((p = soap_in___denc__Notify(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___denf__Subscribe(struct soap *soap, struct __denf__Subscribe *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->wsnt__Subscribe = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___denf__Subscribe(struct soap *soap, const struct __denf__Subscribe *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_wsnt__Subscribe(soap, &a->wsnt__Subscribe);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___denf__Subscribe(struct soap *soap, const char *tag, int id, const struct __denf__Subscribe *a, const char *type)
{
	if (soap_out_PointerTo_wsnt__Subscribe(soap, "wsnt:Subscribe", -1, &a->wsnt__Subscribe, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __denf__Subscribe * SOAP_FMAC4 soap_in___denf__Subscribe(struct soap *soap, const char *tag, struct __denf__Subscribe *a, const char *type)
{
	size_t soap_flag_wsnt__Subscribe = 1;
	short soap_flag;
	a = (struct __denf__Subscribe *)soap_id_enter(soap, "", a, SOAP_TYPE___denf__Subscribe, sizeof(struct __denf__Subscribe), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___denf__Subscribe(soap, a);
		for (soap_flag = 0;; soap_flag = 1)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_wsnt__Subscribe && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTo_wsnt__Subscribe(soap, "wsnt:Subscribe", &a->wsnt__Subscribe, ""))
				{	soap_flag_wsnt__Subscribe--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				if (soap_flag)
				{	soap->error = SOAP_OK;
					break;
				}
			if (soap_flag && soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put___denf__Subscribe(struct soap *soap, const struct __denf__Subscribe *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___denf__Subscribe(soap, tag?tag:"-denf:Subscribe", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __denf__Subscribe * SOAP_FMAC4 soap_get___denf__Subscribe(struct soap *soap, struct __denf__Subscribe *p, const char *tag, const char *type)
{
	if ((p = soap_in___denf__Subscribe(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__wsnt__SubscribeResponse(struct soap *soap, struct _wsnt__SubscribeResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_wsa5__EndpointReferenceType(soap, &a->SubscriptionReference);
	a->CurrentTime = NULL;
	a->TerminationTime = NULL;
	a->__size = 0;
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__wsnt__SubscribeResponse(struct soap *soap, const struct _wsnt__SubscribeResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_embedded(soap, &a->SubscriptionReference, SOAP_TYPE_wsa5__EndpointReferenceType);
	soap_serialize_wsa5__EndpointReferenceType(soap, &a->SubscriptionReference);
	soap_serialize_PointerTotime(soap, &a->CurrentTime);
	soap_serialize_PointerTotime(soap, &a->TerminationTime);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__wsnt__SubscribeResponse(struct soap *soap, const char *tag, int id, const struct _wsnt__SubscribeResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__wsnt__SubscribeResponse), type))
		return soap->error;
	soap_element_result(soap, "wsnt:SubscriptionReference");
	if (soap_out_wsa5__EndpointReferenceType(soap, "wsnt:SubscriptionReference", -1, &a->SubscriptionReference, ""))
		return soap->error;
	if (soap_out_PointerTotime(soap, "wsnt:CurrentTime", -1, &a->CurrentTime, ""))
		return soap->error;
	if (soap_out_PointerTotime(soap, "wsnt:TerminationTime", -1, &a->TerminationTime, ""))
		return soap->error;
	if (a->__any)
	{	int i;
		for (i = 0; i < a->__size; i++)
			soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _wsnt__SubscribeResponse * SOAP_FMAC4 soap_in__wsnt__SubscribeResponse(struct soap *soap, const char *tag, struct _wsnt__SubscribeResponse *a, const char *type)
{
	size_t soap_flag_SubscriptionReference = 1;
	size_t soap_flag_CurrentTime = 1;
	size_t soap_flag_TerminationTime = 1;
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _wsnt__SubscribeResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__wsnt__SubscribeResponse, sizeof(struct _wsnt__SubscribeResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__wsnt__SubscribeResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_SubscriptionReference && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_wsa5__EndpointReferenceType(soap, "wsnt:SubscriptionReference", &a->SubscriptionReference, "wsa5:EndpointReferenceType"))
				{	soap_flag_SubscriptionReference--;
					continue;
				}
			if (soap_flag_CurrentTime && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTotime(soap, "wsnt:CurrentTime", &a->CurrentTime, "xsd:dateTime"))
				{	soap_flag_CurrentTime--;
					continue;
				}
			if (soap_flag_TerminationTime && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTotime(soap, "wsnt:TerminationTime", &a->TerminationTime, "xsd:dateTime"))
				{	soap_flag_TerminationTime--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH && !soap_peek_element(soap))
			{	if (a->__any == NULL)
				{	if (soap_blist___any == NULL)
						soap_blist___any = soap_new_block(soap);
					a->__any = (char **)soap_push_block(soap, soap_blist___any, sizeof(char *));
					if (a->__any == NULL)
						return NULL;
					*a->__any = NULL;
				}
				if (soap_inliteral(soap, "-any", a->__any))
				{	a->__size++;
					a->__any = NULL;
					continue;
				}
			}
			soap_check_result(soap, "wsnt:SubscriptionReference");
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if (a->__any)
			soap_pop_block(soap, soap_blist___any);
		if (a->__size)
			a->__any = (char **)soap_save_block(soap, soap_blist___any, NULL, 1);
		else
		{	a->__any = NULL;
			if (soap_blist___any)
				soap_end_block(soap, soap_blist___any);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _wsnt__SubscribeResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__wsnt__SubscribeResponse, 0, sizeof(struct _wsnt__SubscribeResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_SubscriptionReference > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__wsnt__SubscribeResponse(struct soap *soap, const struct _wsnt__SubscribeResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__wsnt__SubscribeResponse);
	if (soap_out__wsnt__SubscribeResponse(soap, tag?tag:"wsnt:SubscribeResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _wsnt__SubscribeResponse * SOAP_FMAC4 soap_get__wsnt__SubscribeResponse(struct soap *soap, struct _wsnt__SubscribeResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__wsnt__SubscribeResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___desm__Renew(struct soap *soap, struct __desm__Renew *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->wsnt__Renew = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___desm__Renew(struct soap *soap, const struct __desm__Renew *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_wsnt__Renew(soap, &a->wsnt__Renew);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___desm__Renew(struct soap *soap, const char *tag, int id, const struct __desm__Renew *a, const char *type)
{
	if (soap_out_PointerTo_wsnt__Renew(soap, "wsnt:Renew", -1, &a->wsnt__Renew, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __desm__Renew * SOAP_FMAC4 soap_in___desm__Renew(struct soap *soap, const char *tag, struct __desm__Renew *a, const char *type)
{
	size_t soap_flag_wsnt__Renew = 1;
	short soap_flag;
	a = (struct __desm__Renew *)soap_id_enter(soap, "", a, SOAP_TYPE___desm__Renew, sizeof(struct __desm__Renew), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___desm__Renew(soap, a);
		for (soap_flag = 0;; soap_flag = 1)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_wsnt__Renew && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTo_wsnt__Renew(soap, "wsnt:Renew", &a->wsnt__Renew, ""))
				{	soap_flag_wsnt__Renew--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				if (soap_flag)
				{	soap->error = SOAP_OK;
					break;
				}
			if (soap_flag && soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put___desm__Renew(struct soap *soap, const struct __desm__Renew *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___desm__Renew(soap, tag?tag:"-desm:Renew", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __desm__Renew * SOAP_FMAC4 soap_get___desm__Renew(struct soap *soap, struct __desm__Renew *p, const char *tag, const char *type)
{
	if ((p = soap_in___desm__Renew(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__wsnt__RenewResponse(struct soap *soap, struct _wsnt__RenewResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_time(soap, &a->TerminationTime);
	a->CurrentTime = NULL;
	a->__size = 0;
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__wsnt__RenewResponse(struct soap *soap, const struct _wsnt__RenewResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_embedded(soap, &a->TerminationTime, SOAP_TYPE_time);
	soap_serialize_PointerTotime(soap, &a->CurrentTime);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__wsnt__RenewResponse(struct soap *soap, const char *tag, int id, const struct _wsnt__RenewResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__wsnt__RenewResponse), type))
		return soap->error;
	soap_element_result(soap, "wsnt:TerminationTime");
	if (soap_out_time(soap, "wsnt:TerminationTime", -1, &a->TerminationTime, ""))
		return soap->error;
	if (soap_out_PointerTotime(soap, "wsnt:CurrentTime", -1, &a->CurrentTime, ""))
		return soap->error;
	if (a->__any)
	{	int i;
		for (i = 0; i < a->__size; i++)
			soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _wsnt__RenewResponse * SOAP_FMAC4 soap_in__wsnt__RenewResponse(struct soap *soap, const char *tag, struct _wsnt__RenewResponse *a, const char *type)
{
	size_t soap_flag_TerminationTime = 1;
	size_t soap_flag_CurrentTime = 1;
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _wsnt__RenewResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__wsnt__RenewResponse, sizeof(struct _wsnt__RenewResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__wsnt__RenewResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_TerminationTime && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_time(soap, "wsnt:TerminationTime", &a->TerminationTime, "xsd:dateTime"))
				{	soap_flag_TerminationTime--;
					continue;
				}
			if (soap_flag_CurrentTime && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTotime(soap, "wsnt:CurrentTime", &a->CurrentTime, "xsd:dateTime"))
				{	soap_flag_CurrentTime--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH && !soap_peek_element(soap))
			{	if (a->__any == NULL)
				{	if (soap_blist___any == NULL)
						soap_blist___any = soap_new_block(soap);
					a->__any = (char **)soap_push_block(soap, soap_blist___any, sizeof(char *));
					if (a->__any == NULL)
						return NULL;
					*a->__any = NULL;
				}
				if (soap_inliteral(soap, "-any", a->__any))
				{	a->__size++;
					a->__any = NULL;
					continue;
				}
			}
			soap_check_result(soap, "wsnt:TerminationTime");
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if (a->__any)
			soap_pop_block(soap, soap_blist___any);
		if (a->__size)
			a->__any = (char **)soap_save_block(soap, soap_blist___any, NULL, 1);
		else
		{	a->__any = NULL;
			if (soap_blist___any)
				soap_end_block(soap, soap_blist___any);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _wsnt__RenewResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__wsnt__RenewResponse, 0, sizeof(struct _wsnt__RenewResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_TerminationTime > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__wsnt__RenewResponse(struct soap *soap, const struct _wsnt__RenewResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__wsnt__RenewResponse);
	if (soap_out__wsnt__RenewResponse(soap, tag?tag:"wsnt:RenewResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _wsnt__RenewResponse * SOAP_FMAC4 soap_get__wsnt__RenewResponse(struct soap *soap, struct _wsnt__RenewResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__wsnt__RenewResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_wsnt__Notify(struct soap *soap, struct _wsnt__Notify *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__wsnt__Notify))
		soap_serialize__wsnt__Notify(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_wsnt__Notify(struct soap *soap, const char *tag, int id, struct _wsnt__Notify *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__wsnt__Notify);
	if (id < 0)
		return soap->error;
	return soap_out__wsnt__Notify(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _wsnt__Notify ** SOAP_FMAC4 soap_in_PointerTo_wsnt__Notify(struct soap *soap, const char *tag, struct _wsnt__Notify **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _wsnt__Notify **)soap_malloc(soap, sizeof(struct _wsnt__Notify *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in__wsnt__Notify(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct _wsnt__Notify **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__wsnt__Notify, sizeof(struct _wsnt__Notify), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_wsnt__Notify(struct soap *soap, struct _wsnt__Notify *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_wsnt__Notify);
	if (soap_out_PointerTo_wsnt__Notify(soap, tag?tag:"wsnt:Notify", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _wsnt__Notify ** SOAP_FMAC4 soap_get_PointerTo_wsnt__Notify(struct soap *soap, struct _wsnt__Notify **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_wsnt__Notify(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_wsnt__Subscribe(struct soap *soap, struct _wsnt__Subscribe *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__wsnt__Subscribe))
		soap_serialize__wsnt__Subscribe(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_wsnt__Subscribe(struct soap *soap, const char *tag, int id, struct _wsnt__Subscribe *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__wsnt__Subscribe);
	if (id < 0)
		return soap->error;
	return soap_out__wsnt__Subscribe(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _wsnt__Subscribe ** SOAP_FMAC4 soap_in_PointerTo_wsnt__Subscribe(struct soap *soap, const char *tag, struct _wsnt__Subscribe **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _wsnt__Subscribe **)soap_malloc(soap, sizeof(struct _wsnt__Subscribe *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in__wsnt__Subscribe(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct _wsnt__Subscribe **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__wsnt__Subscribe, sizeof(struct _wsnt__Subscribe), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_wsnt__Subscribe(struct soap *soap, struct _wsnt__Subscribe *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_wsnt__Subscribe);
	if (soap_out_PointerTo_wsnt__Subscribe(soap, tag?tag:"wsnt:Subscribe", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _wsnt__Subscribe ** SOAP_FMAC4 soap_get_PointerTo_wsnt__Subscribe(struct soap *soap, struct _wsnt__Subscribe **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_wsnt__Subscribe(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_wsnt__Renew(struct soap *soap, struct _wsnt__Renew *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__wsnt__Renew))
		soap_serialize__wsnt__Renew(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_wsnt__Renew(struct soap *soap, const char *tag, int id, struct _wsnt__Renew *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__wsnt__Renew);
	if (id < 0)
		return soap->error;
	return soap_out__wsnt__Renew(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _wsnt__Renew ** SOAP_FMAC4 soap_in_PointerTo_wsnt__Renew(struct soap *soap, const char *tag, struct _wsnt__Renew **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _wsnt__Renew **)soap_malloc(soap, sizeof(struct _wsnt__Renew *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in__wsnt__Renew(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct _wsnt__Renew **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__wsnt__Renew, sizeof(struct _wsnt__Renew), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_wsnt__Renew(struct soap *soap, struct _wsnt__Renew *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_wsnt__Renew);
	if (soap_out_PointerTo_wsnt__Renew(soap, tag?tag:"wsnt:Renew", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _wsnt__Renew ** SOAP_FMAC4 soap_get_PointerTo_wsnt__Renew(struct soap *soap, struct _wsnt__Renew **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_wsnt__Renew(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

//SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__wsnt__Notify(struct soap *soap, const struct _wsnt__Notify *a)
//{
//	(void)soap; (void)a; /* appease -Wall -Werror */
//	if (a->NotificationMessage)
//	{	int i;
//		for (i = 0; i < a->__sizeNotificationMessage; i++)
//		{
//			soap_embedded(soap, a->NotificationMessage + i, SOAP_TYPE_wsnt__NotificationMessageHolderType);
//			soap_serialize_wsnt__NotificationMessageHolderType(soap, a->NotificationMessage + i);
//		}
//	}
//}
//
//SOAP_FMAC3 int SOAP_FMAC4 soap_out__wsnt__Notify(struct soap *soap, const char *tag, int id, const struct _wsnt__Notify *a, const char *type)
//{
//	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__wsnt__Notify), type))
//		return soap->error;
//	if (a->NotificationMessage)
//	{	int i;
//		for (i = 0; i < a->__sizeNotificationMessage; i++)
//			if (soap_out_wsnt__NotificationMessageHolderType(soap, "wsnt:NotificationMessage", -1, a->NotificationMessage + i, ""))
//				return soap->error;
//	}
//	if (a->__any)
//	{	int i;
//		for (i = 0; i < a->__size; i++)
//			soap_outliteral(soap, "-any", a->__any + i, NULL);
//	}
//	return soap_element_end_out(soap, tag);
//}
//
//SOAP_FMAC3 struct _wsnt__Notify * SOAP_FMAC4 soap_in__wsnt__Notify(struct soap *soap, const char *tag, struct _wsnt__Notify *a, const char *type)
//{
//	struct soap_blist *soap_blist_NotificationMessage = NULL;
//	struct soap_blist *soap_blist___any = NULL;
//	if (soap_element_begin_in(soap, tag, 0, type))
//		return NULL;
//	a = (struct _wsnt__Notify *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__wsnt__Notify, sizeof(struct _wsnt__Notify), 0, NULL, NULL, NULL);
//	if (!a)
//		return NULL;
//	soap_default__wsnt__Notify(soap, a);
//	if (soap->body && !*soap->href)
//	{
//		for (;;)
//		{	soap->error = SOAP_TAG_MISMATCH;
//			if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "wsnt:NotificationMessage", 1, NULL))
//			{	if (a->NotificationMessage == NULL)
//				{	if (soap_blist_NotificationMessage == NULL)
//						soap_blist_NotificationMessage = soap_new_block(soap);
//					a->NotificationMessage = (struct wsnt__NotificationMessageHolderType *)soap_push_block(soap, soap_blist_NotificationMessage, sizeof(struct wsnt__NotificationMessageHolderType));
//					if (a->NotificationMessage == NULL)
//						return NULL;
//					soap_default_wsnt__NotificationMessageHolderType(soap, a->NotificationMessage);
//				}
//				soap_revert(soap);
//				if (soap_in_wsnt__NotificationMessageHolderType(soap, "wsnt:NotificationMessage", a->NotificationMessage, "wsnt:NotificationMessageHolderType"))
//				{	a->__sizeNotificationMessage++;
//					a->NotificationMessage = NULL;
//					continue;
//				}
//			}
//			if (soap->error == SOAP_TAG_MISMATCH && !soap_peek_element(soap))
//			{	if (a->__any == NULL)
//				{	if (soap_blist___any == NULL)
//						soap_blist___any = soap_new_block(soap);
//					a->__any = (char **)soap_push_block(soap, soap_blist___any, sizeof(char *));
//					if (a->__any == NULL)
//						return NULL;
//					*a->__any = NULL;
//				}
//				if (soap_inliteral(soap, "-any", a->__any))
//				{	a->__size++;
//					a->__any = NULL;
//					continue;
//				}
//			}
//			if (soap->error == SOAP_TAG_MISMATCH)
//				soap->error = soap_ignore_element(soap);
//			if (soap->error == SOAP_NO_TAG)
//				break;
//			if (soap->error)
//				return NULL;
//		}
//		if (a->NotificationMessage)
//			soap_pop_block(soap, soap_blist_NotificationMessage);
//		if (a->__sizeNotificationMessage)
//			a->NotificationMessage = (struct wsnt__NotificationMessageHolderType *)soap_save_block(soap, soap_blist_NotificationMessage, NULL, 1);
//		else
//		{	a->NotificationMessage = NULL;
//			if (soap_blist_NotificationMessage)
//				soap_end_block(soap, soap_blist_NotificationMessage);
//		}
//		if (a->__any)
//			soap_pop_block(soap, soap_blist___any);
//		if (a->__size)
//			a->__any = (char **)soap_save_block(soap, soap_blist___any, NULL, 1);
//		else
//		{	a->__any = NULL;
//			if (soap_blist___any)
//				soap_end_block(soap, soap_blist___any);
//		}
//		if (soap_element_end_in(soap, tag))
//			return NULL;
//	}
//	else
//	{	a = (struct _wsnt__Notify *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__wsnt__Notify, 0, sizeof(struct _wsnt__Notify), 0, NULL);
//		if (soap->body && soap_element_end_in(soap, tag))
//			return NULL;
//	}
//	if ((soap->mode & SOAP_XML_STRICT) && (a->__sizeNotificationMessage < 1))
//	{	soap->error = SOAP_OCCURS;
//		return NULL;
//	}
//	return a;
//}
//
//SOAP_FMAC3 int SOAP_FMAC4 soap_put__wsnt__Notify(struct soap *soap, const struct _wsnt__Notify *a, const char *tag, const char *type)
//{
//	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__wsnt__Notify);
//	if (soap_out__wsnt__Notify(soap, tag?tag:"wsnt:Notify", id, a, type))
//		return soap->error;
//	return soap_putindependent(soap);
//}
//
//SOAP_FMAC3 struct _wsnt__Notify * SOAP_FMAC4 soap_get__wsnt__Notify(struct soap *soap, struct _wsnt__Notify *p, const char *tag, const char *type)
//{
//	if ((p = soap_in__wsnt__Notify(soap, tag, p, type)))
//		if (soap_getindependent(soap))
//			return NULL;
//	return p;
//}

//SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__wsnt__Subscribe(struct soap *soap, const struct _wsnt__Subscribe *a)
//{
//	(void)soap; (void)a; /* appease -Wall -Werror */
//	soap_embedded(soap, &a->ConsumerReference, SOAP_TYPE_wsa5__EndpointReferenceType);
//	soap_serialize_wsa5__EndpointReferenceType(soap, &a->ConsumerReference);
//	soap_serialize_PointerTowsnt__FilterType(soap, &a->Filter);
//	soap_serialize_wsnt__AbsoluteOrRelativeTimeType(soap, &a->InitialTerminationTime);
//	soap_serialize_PointerTo_wsnt__Subscribe_SubscriptionPolicy(soap, &a->SubscriptionPolicy);
//}
//
//SOAP_FMAC3 int SOAP_FMAC4 soap_out__wsnt__Subscribe(struct soap *soap, const char *tag, int id, const struct _wsnt__Subscribe *a, const char *type)
//{
//	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__wsnt__Subscribe), type))
//		return soap->error;
//	if (soap_out_wsa5__EndpointReferenceType(soap, "wsnt:ConsumerReference", -1, &a->ConsumerReference, ""))
//		return soap->error;
//	if (soap_out_PointerTowsnt__FilterType(soap, "wsnt:Filter", -1, &a->Filter, ""))
//		return soap->error;
//	if (soap_out_wsnt__AbsoluteOrRelativeTimeType(soap, "wsnt:InitialTerminationTime", -1, &a->InitialTerminationTime, ""))
//		return soap->error;
//	if (soap_out_PointerTo_wsnt__Subscribe_SubscriptionPolicy(soap, "wsnt:SubscriptionPolicy", -1, &a->SubscriptionPolicy, ""))
//		return soap->error;
//	if (a->__any)
//	{	int i;
//		for (i = 0; i < a->__size; i++)
//			soap_outliteral(soap, "-any", a->__any + i, NULL);
//	}
//	return soap_element_end_out(soap, tag);
//}
//
//SOAP_FMAC3 struct _wsnt__Subscribe * SOAP_FMAC4 soap_in__wsnt__Subscribe(struct soap *soap, const char *tag, struct _wsnt__Subscribe *a, const char *type)
//{
//	size_t soap_flag_ConsumerReference = 1;
//	size_t soap_flag_Filter = 1;
//	size_t soap_flag_InitialTerminationTime = 1;
//	size_t soap_flag_SubscriptionPolicy = 1;
//	struct soap_blist *soap_blist___any = NULL;
//	if (soap_element_begin_in(soap, tag, 0, type))
//		return NULL;
//	a = (struct _wsnt__Subscribe *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__wsnt__Subscribe, sizeof(struct _wsnt__Subscribe), 0, NULL, NULL, NULL);
//	if (!a)
//		return NULL;
//	soap_default__wsnt__Subscribe(soap, a);
//	if (soap->body && !*soap->href)
//	{
//		for (;;)
//		{	soap->error = SOAP_TAG_MISMATCH;
//			if (soap_flag_ConsumerReference && soap->error == SOAP_TAG_MISMATCH)
//				if (soap_in_wsa5__EndpointReferenceType(soap, "wsnt:ConsumerReference", &a->ConsumerReference, "wsa5:EndpointReferenceType"))
//				{	soap_flag_ConsumerReference--;
//					continue;
//				}
//			if (soap_flag_Filter && soap->error == SOAP_TAG_MISMATCH)
//				if (soap_in_PointerTowsnt__FilterType(soap, "wsnt:Filter", &a->Filter, "wsnt:FilterType"))
//				{	soap_flag_Filter--;
//					continue;
//				}
//			if (soap_flag_InitialTerminationTime && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
//				if (soap_in_wsnt__AbsoluteOrRelativeTimeType(soap, "wsnt:InitialTerminationTime", &a->InitialTerminationTime, "wsnt:AbsoluteOrRelativeTimeType"))
//				{	soap_flag_InitialTerminationTime--;
//					continue;
//				}
//			if (soap_flag_SubscriptionPolicy && soap->error == SOAP_TAG_MISMATCH)
//				if (soap_in_PointerTo_wsnt__Subscribe_SubscriptionPolicy(soap, "wsnt:SubscriptionPolicy", &a->SubscriptionPolicy, ""))
//				{	soap_flag_SubscriptionPolicy--;
//					continue;
//				}
//			if (soap->error == SOAP_TAG_MISMATCH && !soap_peek_element(soap))
//			{	if (a->__any == NULL)
//				{	if (soap_blist___any == NULL)
//						soap_blist___any = soap_new_block(soap);
//					a->__any = (char **)soap_push_block(soap, soap_blist___any, sizeof(char *));
//					if (a->__any == NULL)
//						return NULL;
//					*a->__any = NULL;
//				}
//				if (soap_inliteral(soap, "-any", a->__any))
//				{	a->__size++;
//					a->__any = NULL;
//					continue;
//				}
//			}
//			if (soap->error == SOAP_TAG_MISMATCH)
//				soap->error = soap_ignore_element(soap);
//			if (soap->error == SOAP_NO_TAG)
//				break;
//			if (soap->error)
//				return NULL;
//		}
//		if (a->__any)
//			soap_pop_block(soap, soap_blist___any);
//		if (a->__size)
//			a->__any = (char **)soap_save_block(soap, soap_blist___any, NULL, 1);
//		else
//		{	a->__any = NULL;
//			if (soap_blist___any)
//				soap_end_block(soap, soap_blist___any);
//		}
//		if (soap_element_end_in(soap, tag))
//			return NULL;
//	}
//	else
//	{	a = (struct _wsnt__Subscribe *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__wsnt__Subscribe, 0, sizeof(struct _wsnt__Subscribe), 0, NULL);
//		if (soap->body && soap_element_end_in(soap, tag))
//			return NULL;
//	}
//	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_ConsumerReference > 0))
//	{	soap->error = SOAP_OCCURS;
//		return NULL;
//	}
//	return a;
//}
//
//SOAP_FMAC3 int SOAP_FMAC4 soap_put__wsnt__Subscribe(struct soap *soap, const struct _wsnt__Subscribe *a, const char *tag, const char *type)
//{
//	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__wsnt__Subscribe);
//	if (soap_out__wsnt__Subscribe(soap, tag?tag:"wsnt:Subscribe", id, a, type))
//		return soap->error;
//	return soap_putindependent(soap);
//}
//
//SOAP_FMAC3 struct _wsnt__Subscribe * SOAP_FMAC4 soap_get__wsnt__Subscribe(struct soap *soap, struct _wsnt__Subscribe *p, const char *tag, const char *type)
//{
//	if ((p = soap_in__wsnt__Subscribe(soap, tag, p, type)))
//		if (soap_getindependent(soap))
//			return NULL;
//	return p;
//}

//SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__wsnt__Renew(struct soap *soap, const struct _wsnt__Renew *a)
//{
//	(void)soap; (void)a; /* appease -Wall -Werror */
//	soap_serialize_wsnt__AbsoluteOrRelativeTimeType(soap, &a->TerminationTime);
//}
//
//SOAP_FMAC3 int SOAP_FMAC4 soap_out__wsnt__Renew(struct soap *soap, const char *tag, int id, const struct _wsnt__Renew *a, const char *type)
//{
//	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__wsnt__Renew), type))
//		return soap->error;
//	if (a->TerminationTime)
//	{	if (soap_out_wsnt__AbsoluteOrRelativeTimeType(soap, "wsnt:TerminationTime", -1, &a->TerminationTime, ""))
//			return soap->error;
//	}
//	else if (soap_element_nil(soap, "wsnt:TerminationTime"))
//		return soap->error;
//	if (a->__any)
//	{	int i;
//		for (i = 0; i < a->__size; i++)
//			soap_outliteral(soap, "-any", a->__any + i, NULL);
//	}
//	return soap_element_end_out(soap, tag);
//}
//
//SOAP_FMAC3 struct _wsnt__Renew * SOAP_FMAC4 soap_in__wsnt__Renew(struct soap *soap, const char *tag, struct _wsnt__Renew *a, const char *type)
//{
//	size_t soap_flag_TerminationTime = 1;
//	struct soap_blist *soap_blist___any = NULL;
//	if (soap_element_begin_in(soap, tag, 0, type))
//		return NULL;
//	a = (struct _wsnt__Renew *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__wsnt__Renew, sizeof(struct _wsnt__Renew), 0, NULL, NULL, NULL);
//	if (!a)
//		return NULL;
//	soap_default__wsnt__Renew(soap, a);
//	if (soap->body && !*soap->href)
//	{
//		for (;;)
//		{	soap->error = SOAP_TAG_MISMATCH;
//			if (soap_flag_TerminationTime && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
//				if (soap_in_wsnt__AbsoluteOrRelativeTimeType(soap, "wsnt:TerminationTime", &a->TerminationTime, "wsnt:AbsoluteOrRelativeTimeType"))
//				{	soap_flag_TerminationTime--;
//					continue;
//				}
//			if (soap->error == SOAP_TAG_MISMATCH && !soap_peek_element(soap))
//			{	if (a->__any == NULL)
//				{	if (soap_blist___any == NULL)
//						soap_blist___any = soap_new_block(soap);
//					a->__any = (char **)soap_push_block(soap, soap_blist___any, sizeof(char *));
//					if (a->__any == NULL)
//						return NULL;
//					*a->__any = NULL;
//				}
//				if (soap_inliteral(soap, "-any", a->__any))
//				{	a->__size++;
//					a->__any = NULL;
//					continue;
//				}
//			}
//			if (soap->error == SOAP_TAG_MISMATCH)
//				soap->error = soap_ignore_element(soap);
//			if (soap->error == SOAP_NO_TAG)
//				break;
//			if (soap->error)
//				return NULL;
//		}
//		if (a->__any)
//			soap_pop_block(soap, soap_blist___any);
//		if (a->__size)
//			a->__any = (char **)soap_save_block(soap, soap_blist___any, NULL, 1);
//		else
//		{	a->__any = NULL;
//			if (soap_blist___any)
//				soap_end_block(soap, soap_blist___any);
//		}
//		if (soap_element_end_in(soap, tag))
//			return NULL;
//	}
//	else
//	{	a = (struct _wsnt__Renew *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__wsnt__Renew, 0, sizeof(struct _wsnt__Renew), 0, NULL);
//		if (soap->body && soap_element_end_in(soap, tag))
//			return NULL;
//	}
//	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_TerminationTime > 0))
//	{	soap->error = SOAP_OCCURS;
//		return NULL;
//	}
//	return a;
//}
//
//SOAP_FMAC3 int SOAP_FMAC4 soap_put__wsnt__Renew(struct soap *soap, const struct _wsnt__Renew *a, const char *tag, const char *type)
//{
//	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__wsnt__Renew);
//	if (soap_out__wsnt__Renew(soap, tag?tag:"wsnt:Renew", id, a, type))
//		return soap->error;
//	return soap_putindependent(soap);
//}
//
//SOAP_FMAC3 struct _wsnt__Renew * SOAP_FMAC4 soap_get__wsnt__Renew(struct soap *soap, struct _wsnt__Renew *p, const char *tag, const char *type)
//{
//	if ((p = soap_in__wsnt__Renew(soap, tag, p, type)))
//		if (soap_getindependent(soap))
//			return NULL;
//	return p;
//}

//SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_wsnt__NotificationMessageHolderType(struct soap *soap, const struct wsnt__NotificationMessageHolderType *a)
//{
//	(void)soap; (void)a; /* appease -Wall -Werror */
//	soap_serialize_PointerTowsa5__EndpointReferenceType(soap, &a->SubscriptionReference);
//	soap_serialize_PointerTowsnt__TopicExpressionType(soap, &a->Topic);
//	soap_serialize_PointerTowsa5__EndpointReferenceType(soap, &a->ProducerReference);
//	soap_serialize__wsnt__NotificationMessageHolderType_Message(soap, &a->Message);
//}
//
//SOAP_FMAC3 int SOAP_FMAC4 soap_out_wsnt__NotificationMessageHolderType(struct soap *soap, const char *tag, int id, const struct wsnt__NotificationMessageHolderType *a, const char *type)
//{
//	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_wsnt__NotificationMessageHolderType), type))
//		return soap->error;
//	if (soap_out_PointerTowsa5__EndpointReferenceType(soap, "wsnt:SubscriptionReference", -1, &a->SubscriptionReference, ""))
//		return soap->error;
//	if (soap_out_PointerTowsnt__TopicExpressionType(soap, "wsnt:Topic", -1, &a->Topic, ""))
//		return soap->error;
//	if (soap_out_PointerTowsa5__EndpointReferenceType(soap, "wsnt:ProducerReference", -1, &a->ProducerReference, ""))
//		return soap->error;
//	if (soap_out__wsnt__NotificationMessageHolderType_Message(soap, "wsnt:Message", -1, &a->Message, ""))
//		return soap->error;
//	return soap_element_end_out(soap, tag);
//}
//
//SOAP_FMAC3 struct wsnt__NotificationMessageHolderType * SOAP_FMAC4 soap_in_wsnt__NotificationMessageHolderType(struct soap *soap, const char *tag, struct wsnt__NotificationMessageHolderType *a, const char *type)
//{
//	size_t soap_flag_SubscriptionReference = 1;
//	size_t soap_flag_Topic = 1;
//	size_t soap_flag_ProducerReference = 1;
//	size_t soap_flag_Message = 1;
//	if (soap_element_begin_in(soap, tag, 0, type))
//		return NULL;
//	a = (struct wsnt__NotificationMessageHolderType *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_wsnt__NotificationMessageHolderType, sizeof(struct wsnt__NotificationMessageHolderType), 0, NULL, NULL, NULL);
//	if (!a)
//		return NULL;
//	soap_default_wsnt__NotificationMessageHolderType(soap, a);
//	if (soap->body && !*soap->href)
//	{
//		for (;;)
//		{	soap->error = SOAP_TAG_MISMATCH;
//			if (soap_flag_SubscriptionReference && soap->error == SOAP_TAG_MISMATCH)
//				if (soap_in_PointerTowsa5__EndpointReferenceType(soap, "wsnt:SubscriptionReference", &a->SubscriptionReference, "wsa5:EndpointReferenceType"))
//				{	soap_flag_SubscriptionReference--;
//					continue;
//				}
//			if (soap_flag_Topic && soap->error == SOAP_TAG_MISMATCH)
//				if (soap_in_PointerTowsnt__TopicExpressionType(soap, "wsnt:Topic", &a->Topic, "wsnt:TopicExpressionType"))
//				{	soap_flag_Topic--;
//					continue;
//				}
//			if (soap_flag_ProducerReference && soap->error == SOAP_TAG_MISMATCH)
//				if (soap_in_PointerTowsa5__EndpointReferenceType(soap, "wsnt:ProducerReference", &a->ProducerReference, "wsa5:EndpointReferenceType"))
//				{	soap_flag_ProducerReference--;
//					continue;
//				}
//			if (soap_flag_Message && soap->error == SOAP_TAG_MISMATCH)
//				if (soap_in__wsnt__NotificationMessageHolderType_Message(soap, "wsnt:Message", &a->Message, ""))
//				{	soap_flag_Message--;
//					continue;
//				}
//			if (soap->error == SOAP_TAG_MISMATCH)
//				soap->error = soap_ignore_element(soap);
//			if (soap->error == SOAP_NO_TAG)
//				break;
//			if (soap->error)
//				return NULL;
//		}
//		if (soap_element_end_in(soap, tag))
//			return NULL;
//	}
//	else
//	{	a = (struct wsnt__NotificationMessageHolderType *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_wsnt__NotificationMessageHolderType, 0, sizeof(struct wsnt__NotificationMessageHolderType), 0, NULL);
//		if (soap->body && soap_element_end_in(soap, tag))
//			return NULL;
//	}
//	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_Message > 0))
//	{	soap->error = SOAP_OCCURS;
//		return NULL;
//	}
//	return a;
//}
//
//SOAP_FMAC3 int SOAP_FMAC4 soap_put_wsnt__NotificationMessageHolderType(struct soap *soap, const struct wsnt__NotificationMessageHolderType *a, const char *tag, const char *type)
//{
//	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_wsnt__NotificationMessageHolderType);
//	if (soap_out_wsnt__NotificationMessageHolderType(soap, tag?tag:"wsnt:NotificationMessageHolderType", id, a, type))
//		return soap->error;
//	return soap_putindependent(soap);
//}
//
//SOAP_FMAC3 struct wsnt__NotificationMessageHolderType * SOAP_FMAC4 soap_get_wsnt__NotificationMessageHolderType(struct soap *soap, struct wsnt__NotificationMessageHolderType *p, const char *tag, const char *type)
//{
//	if ((p = soap_in_wsnt__NotificationMessageHolderType(soap, tag, p, type)))
//		if (soap_getindependent(soap))
//			return NULL;
//	return p;
//}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_wsnt__NotificationMessageHolderType(struct soap *soap, struct wsnt__NotificationMessageHolderType *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->SubscriptionReference = NULL;
	a->Topic = NULL;
	a->ProducerReference = NULL;
	soap_default__wsnt__NotificationMessageHolderType_Message(soap, &a->Message);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_wsnt__NotificationMessageHolderType(struct soap *soap, const struct wsnt__NotificationMessageHolderType *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTowsa5__EndpointReferenceType(soap, &a->SubscriptionReference);
	soap_serialize_PointerTowsnt__TopicExpressionType(soap, &a->Topic);
	soap_serialize_PointerTowsa5__EndpointReferenceType(soap, &a->ProducerReference);
	soap_serialize__wsnt__NotificationMessageHolderType_Message(soap, &a->Message);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_wsnt__NotificationMessageHolderType(struct soap *soap, const char *tag, int id, const struct wsnt__NotificationMessageHolderType *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_wsnt__NotificationMessageHolderType), type))
		return soap->error;
	if (soap_out_PointerTowsa5__EndpointReferenceType(soap, "wsnt:SubscriptionReference", -1, &a->SubscriptionReference, ""))
		return soap->error;
	if (soap_out_PointerTowsnt__TopicExpressionType(soap, "wsnt:Topic", -1, &a->Topic, ""))
		return soap->error;
	if (soap_out_PointerTowsa5__EndpointReferenceType(soap, "wsnt:ProducerReference", -1, &a->ProducerReference, ""))
		return soap->error;
	if (soap_out__wsnt__NotificationMessageHolderType_Message(soap, "wsnt:Message", -1, &a->Message, ""))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct wsnt__NotificationMessageHolderType * SOAP_FMAC4 soap_in_wsnt__NotificationMessageHolderType(struct soap *soap, const char *tag, struct wsnt__NotificationMessageHolderType *a, const char *type)
{
	size_t soap_flag_SubscriptionReference = 1;
	size_t soap_flag_Topic = 1;
	size_t soap_flag_ProducerReference = 1;
	size_t soap_flag_Message = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct wsnt__NotificationMessageHolderType *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_wsnt__NotificationMessageHolderType, sizeof(struct wsnt__NotificationMessageHolderType), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_wsnt__NotificationMessageHolderType(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_SubscriptionReference && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTowsa5__EndpointReferenceType(soap, "wsnt:SubscriptionReference", &a->SubscriptionReference, "wsa5:EndpointReferenceType"))
				{	soap_flag_SubscriptionReference--;
					continue;
				}
			if (soap_flag_Topic && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTowsnt__TopicExpressionType(soap, "wsnt:Topic", &a->Topic, "wsnt:TopicExpressionType"))
				{	soap_flag_Topic--;
					continue;
				}
			if (soap_flag_ProducerReference && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTowsa5__EndpointReferenceType(soap, "wsnt:ProducerReference", &a->ProducerReference, "wsa5:EndpointReferenceType"))
				{	soap_flag_ProducerReference--;
					continue;
				}
			if (soap_flag_Message && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in__wsnt__NotificationMessageHolderType_Message(soap, "wsnt:Message", &a->Message, ""))
				{	soap_flag_Message--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct wsnt__NotificationMessageHolderType *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_wsnt__NotificationMessageHolderType, 0, sizeof(struct wsnt__NotificationMessageHolderType), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_Message > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_wsnt__NotificationMessageHolderType(struct soap *soap, const struct wsnt__NotificationMessageHolderType *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_wsnt__NotificationMessageHolderType);
	if (soap_out_wsnt__NotificationMessageHolderType(soap, tag?tag:"wsnt:NotificationMessageHolderType", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct wsnt__NotificationMessageHolderType * SOAP_FMAC4 soap_get_wsnt__NotificationMessageHolderType(struct soap *soap, struct wsnt__NotificationMessageHolderType *p, const char *tag, const char *type)
{
	if ((p = soap_in_wsnt__NotificationMessageHolderType(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__wsnt__Notify(struct soap *soap, struct _wsnt__Notify *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__sizeNotificationMessage = 0;
	a->NotificationMessage = NULL;
	a->__size = 0;
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__wsnt__Notify(struct soap *soap, const struct _wsnt__Notify *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	if (a->NotificationMessage)
	{	int i;
		for (i = 0; i < a->__sizeNotificationMessage; i++)
		{
			soap_embedded(soap, a->NotificationMessage + i, SOAP_TYPE_wsnt__NotificationMessageHolderType);
			soap_serialize_wsnt__NotificationMessageHolderType(soap, a->NotificationMessage + i);
		}
	}
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__wsnt__Notify(struct soap *soap, const char *tag, int id, const struct _wsnt__Notify *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__wsnt__Notify), type))
		return soap->error;
	if (a->NotificationMessage)
	{	int i;
		for (i = 0; i < a->__sizeNotificationMessage; i++)
			if (soap_out_wsnt__NotificationMessageHolderType(soap, "wsnt:NotificationMessage", -1, a->NotificationMessage + i, ""))
				return soap->error;
	}
	if (a->__any)
	{	int i;
		for (i = 0; i < a->__size; i++)
			soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _wsnt__Notify * SOAP_FMAC4 soap_in__wsnt__Notify(struct soap *soap, const char *tag, struct _wsnt__Notify *a, const char *type)
{
	struct soap_blist *soap_blist_NotificationMessage = NULL;
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _wsnt__Notify *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__wsnt__Notify, sizeof(struct _wsnt__Notify), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__wsnt__Notify(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "wsnt:NotificationMessage", 1, NULL))
			{	if (a->NotificationMessage == NULL)
				{	if (soap_blist_NotificationMessage == NULL)
						soap_blist_NotificationMessage = soap_new_block(soap);
					a->NotificationMessage = (struct wsnt__NotificationMessageHolderType *)soap_push_block(soap, soap_blist_NotificationMessage, sizeof(struct wsnt__NotificationMessageHolderType));
					if (a->NotificationMessage == NULL)
						return NULL;
					soap_default_wsnt__NotificationMessageHolderType(soap, a->NotificationMessage);
				}
				soap_revert(soap);
				if (soap_in_wsnt__NotificationMessageHolderType(soap, "wsnt:NotificationMessage", a->NotificationMessage, "wsnt:NotificationMessageHolderType"))
				{	a->__sizeNotificationMessage++;
					a->NotificationMessage = NULL;
					continue;
				}
			}
			if (soap->error == SOAP_TAG_MISMATCH && !soap_peek_element(soap))
			{	if (a->__any == NULL)
				{	if (soap_blist___any == NULL)
						soap_blist___any = soap_new_block(soap);
					a->__any = (char **)soap_push_block(soap, soap_blist___any, sizeof(char *));
					if (a->__any == NULL)
						return NULL;
					*a->__any = NULL;
				}
				if (soap_inliteral(soap, "-any", a->__any))
				{	a->__size++;
					a->__any = NULL;
					continue;
				}
			}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if (a->NotificationMessage)
			soap_pop_block(soap, soap_blist_NotificationMessage);
		if (a->__sizeNotificationMessage)
			a->NotificationMessage = (struct wsnt__NotificationMessageHolderType *)soap_save_block(soap, soap_blist_NotificationMessage, NULL, 1);
		else
		{	a->NotificationMessage = NULL;
			if (soap_blist_NotificationMessage)
				soap_end_block(soap, soap_blist_NotificationMessage);
		}
		if (a->__any)
			soap_pop_block(soap, soap_blist___any);
		if (a->__size)
			a->__any = (char **)soap_save_block(soap, soap_blist___any, NULL, 1);
		else
		{	a->__any = NULL;
			if (soap_blist___any)
				soap_end_block(soap, soap_blist___any);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _wsnt__Notify *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__wsnt__Notify, 0, sizeof(struct _wsnt__Notify), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (a->__sizeNotificationMessage < 1))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__wsnt__Notify(struct soap *soap, const struct _wsnt__Notify *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__wsnt__Notify);
	if (soap_out__wsnt__Notify(soap, tag?tag:"wsnt:Notify", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _wsnt__Notify * SOAP_FMAC4 soap_get__wsnt__Notify(struct soap *soap, struct _wsnt__Notify *p, const char *tag, const char *type)
{
	if ((p = soap_in__wsnt__Notify(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_wsnt__Subscribe_SubscriptionPolicy(struct soap *soap, struct _wsnt__Subscribe_SubscriptionPolicy *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__wsnt__Subscribe_SubscriptionPolicy))
		soap_serialize__wsnt__Subscribe_SubscriptionPolicy(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_wsnt__Subscribe_SubscriptionPolicy(struct soap *soap, const char *tag, int id, struct _wsnt__Subscribe_SubscriptionPolicy *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__wsnt__Subscribe_SubscriptionPolicy);
	if (id < 0)
		return soap->error;
	return soap_out__wsnt__Subscribe_SubscriptionPolicy(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _wsnt__Subscribe_SubscriptionPolicy ** SOAP_FMAC4 soap_in_PointerTo_wsnt__Subscribe_SubscriptionPolicy(struct soap *soap, const char *tag, struct _wsnt__Subscribe_SubscriptionPolicy **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _wsnt__Subscribe_SubscriptionPolicy **)soap_malloc(soap, sizeof(struct _wsnt__Subscribe_SubscriptionPolicy *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in__wsnt__Subscribe_SubscriptionPolicy(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct _wsnt__Subscribe_SubscriptionPolicy **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__wsnt__Subscribe_SubscriptionPolicy, sizeof(struct _wsnt__Subscribe_SubscriptionPolicy), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_wsnt__Subscribe_SubscriptionPolicy(struct soap *soap, struct _wsnt__Subscribe_SubscriptionPolicy *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_wsnt__Subscribe_SubscriptionPolicy);
	if (soap_out_PointerTo_wsnt__Subscribe_SubscriptionPolicy(soap, tag?tag:"wsnt:Subscribe-SubscriptionPolicy", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _wsnt__Subscribe_SubscriptionPolicy ** SOAP_FMAC4 soap_get_PointerTo_wsnt__Subscribe_SubscriptionPolicy(struct soap *soap, struct _wsnt__Subscribe_SubscriptionPolicy **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_wsnt__Subscribe_SubscriptionPolicy(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTowsnt__FilterType(struct soap *soap, struct wsnt__FilterType *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_wsnt__FilterType))
		soap_serialize_wsnt__FilterType(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTowsnt__FilterType(struct soap *soap, const char *tag, int id, struct wsnt__FilterType *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_wsnt__FilterType);
	if (id < 0)
		return soap->error;
	return soap_out_wsnt__FilterType(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct wsnt__FilterType ** SOAP_FMAC4 soap_in_PointerTowsnt__FilterType(struct soap *soap, const char *tag, struct wsnt__FilterType **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct wsnt__FilterType **)soap_malloc(soap, sizeof(struct wsnt__FilterType *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_wsnt__FilterType(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct wsnt__FilterType **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_wsnt__FilterType, sizeof(struct wsnt__FilterType), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTowsnt__FilterType(struct soap *soap, struct wsnt__FilterType *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTowsnt__FilterType);
	if (soap_out_PointerTowsnt__FilterType(soap, tag?tag:"wsnt:FilterType", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct wsnt__FilterType ** SOAP_FMAC4 soap_get_PointerTowsnt__FilterType(struct soap *soap, struct wsnt__FilterType **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTowsnt__FilterType(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_wsnt__AbsoluteOrRelativeTimeType(struct soap *soap, const char *tag, int id, char *const*a, const char *type)
{
	return soap_outstring(soap, tag, id, a, type, SOAP_TYPE_wsnt__AbsoluteOrRelativeTimeType);
}

SOAP_FMAC3 char * * SOAP_FMAC4 soap_in_wsnt__AbsoluteOrRelativeTimeType(struct soap *soap, const char *tag, char **a, const char *type)
{	char **p;
	p = soap_instring(soap, tag, a, type, SOAP_TYPE_wsnt__AbsoluteOrRelativeTimeType, 1, 0, -1);
	return p;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_wsnt__AbsoluteOrRelativeTimeType(struct soap *soap, char *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_wsnt__AbsoluteOrRelativeTimeType);
	if (soap_out_wsnt__AbsoluteOrRelativeTimeType(soap, tag?tag:"byte", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 char ** SOAP_FMAC4 soap_get_wsnt__AbsoluteOrRelativeTimeType(struct soap *soap, char **p, const char *tag, const char *type)
{
	if ((p = soap_in_wsnt__AbsoluteOrRelativeTimeType(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__wsnt__Subscribe(struct soap *soap, struct _wsnt__Subscribe *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_wsa5__EndpointReferenceType(soap, &a->ConsumerReference);
	a->Filter = NULL;
	soap_default_wsnt__AbsoluteOrRelativeTimeType(soap, &a->InitialTerminationTime);
	a->SubscriptionPolicy = NULL;
	a->__size = 0;
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__wsnt__Subscribe(struct soap *soap, const struct _wsnt__Subscribe *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_embedded(soap, &a->ConsumerReference, SOAP_TYPE_wsa5__EndpointReferenceType);
	soap_serialize_wsa5__EndpointReferenceType(soap, &a->ConsumerReference);
	soap_serialize_PointerTowsnt__FilterType(soap, &a->Filter);
	soap_serialize_wsnt__AbsoluteOrRelativeTimeType(soap, &a->InitialTerminationTime);
	soap_serialize_PointerTo_wsnt__Subscribe_SubscriptionPolicy(soap, &a->SubscriptionPolicy);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__wsnt__Subscribe(struct soap *soap, const char *tag, int id, const struct _wsnt__Subscribe *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__wsnt__Subscribe), type))
		return soap->error;
	if (soap_out_wsa5__EndpointReferenceType(soap, "wsnt:ConsumerReference", -1, &a->ConsumerReference, ""))
		return soap->error;
	if (soap_out_PointerTowsnt__FilterType(soap, "wsnt:Filter", -1, &a->Filter, ""))
		return soap->error;
	if (soap_out_wsnt__AbsoluteOrRelativeTimeType(soap, "wsnt:InitialTerminationTime", -1, &a->InitialTerminationTime, ""))
		return soap->error;
	if (soap_out_PointerTo_wsnt__Subscribe_SubscriptionPolicy(soap, "wsnt:SubscriptionPolicy", -1, &a->SubscriptionPolicy, ""))
		return soap->error;
	if (a->__any)
	{	int i;
		for (i = 0; i < a->__size; i++)
			soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _wsnt__Subscribe * SOAP_FMAC4 soap_in__wsnt__Subscribe(struct soap *soap, const char *tag, struct _wsnt__Subscribe *a, const char *type)
{
	size_t soap_flag_ConsumerReference = 1;
	size_t soap_flag_Filter = 1;
	size_t soap_flag_InitialTerminationTime = 1;
	size_t soap_flag_SubscriptionPolicy = 1;
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _wsnt__Subscribe *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__wsnt__Subscribe, sizeof(struct _wsnt__Subscribe), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__wsnt__Subscribe(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_ConsumerReference && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_wsa5__EndpointReferenceType(soap, "wsnt:ConsumerReference", &a->ConsumerReference, "wsa5:EndpointReferenceType"))
				{	soap_flag_ConsumerReference--;
					continue;
				}
			if (soap_flag_Filter && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTowsnt__FilterType(soap, "wsnt:Filter", &a->Filter, "wsnt:FilterType"))
				{	soap_flag_Filter--;
					continue;
				}
			if (soap_flag_InitialTerminationTime && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_wsnt__AbsoluteOrRelativeTimeType(soap, "wsnt:InitialTerminationTime", &a->InitialTerminationTime, "wsnt:AbsoluteOrRelativeTimeType"))
				{	soap_flag_InitialTerminationTime--;
					continue;
				}
			if (soap_flag_SubscriptionPolicy && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTo_wsnt__Subscribe_SubscriptionPolicy(soap, "wsnt:SubscriptionPolicy", &a->SubscriptionPolicy, ""))
				{	soap_flag_SubscriptionPolicy--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH && !soap_peek_element(soap))
			{	if (a->__any == NULL)
				{	if (soap_blist___any == NULL)
						soap_blist___any = soap_new_block(soap);
					a->__any = (char **)soap_push_block(soap, soap_blist___any, sizeof(char *));
					if (a->__any == NULL)
						return NULL;
					*a->__any = NULL;
				}
				if (soap_inliteral(soap, "-any", a->__any))
				{	a->__size++;
					a->__any = NULL;
					continue;
				}
			}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if (a->__any)
			soap_pop_block(soap, soap_blist___any);
		if (a->__size)
			a->__any = (char **)soap_save_block(soap, soap_blist___any, NULL, 1);
		else
		{	a->__any = NULL;
			if (soap_blist___any)
				soap_end_block(soap, soap_blist___any);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _wsnt__Subscribe *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__wsnt__Subscribe, 0, sizeof(struct _wsnt__Subscribe), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_ConsumerReference > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__wsnt__Subscribe(struct soap *soap, const struct _wsnt__Subscribe *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__wsnt__Subscribe);
	if (soap_out__wsnt__Subscribe(soap, tag?tag:"wsnt:Subscribe", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _wsnt__Subscribe * SOAP_FMAC4 soap_get__wsnt__Subscribe(struct soap *soap, struct _wsnt__Subscribe *p, const char *tag, const char *type)
{
	if ((p = soap_in__wsnt__Subscribe(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__wsnt__Renew(struct soap *soap, struct _wsnt__Renew *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_wsnt__AbsoluteOrRelativeTimeType(soap, &a->TerminationTime);
	a->__size = 0;
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__wsnt__Renew(struct soap *soap, const struct _wsnt__Renew *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_wsnt__AbsoluteOrRelativeTimeType(soap, &a->TerminationTime);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__wsnt__Renew(struct soap *soap, const char *tag, int id, const struct _wsnt__Renew *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__wsnt__Renew), type))
		return soap->error;
	if (a->TerminationTime)
	{	if (soap_out_wsnt__AbsoluteOrRelativeTimeType(soap, "wsnt:TerminationTime", -1, &a->TerminationTime, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "wsnt:TerminationTime"))
		return soap->error;
	if (a->__any)
	{	int i;
		for (i = 0; i < a->__size; i++)
			soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _wsnt__Renew * SOAP_FMAC4 soap_in__wsnt__Renew(struct soap *soap, const char *tag, struct _wsnt__Renew *a, const char *type)
{
	size_t soap_flag_TerminationTime = 1;
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _wsnt__Renew *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__wsnt__Renew, sizeof(struct _wsnt__Renew), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__wsnt__Renew(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_TerminationTime && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_wsnt__AbsoluteOrRelativeTimeType(soap, "wsnt:TerminationTime", &a->TerminationTime, "wsnt:AbsoluteOrRelativeTimeType"))
				{	soap_flag_TerminationTime--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH && !soap_peek_element(soap))
			{	if (a->__any == NULL)
				{	if (soap_blist___any == NULL)
						soap_blist___any = soap_new_block(soap);
					a->__any = (char **)soap_push_block(soap, soap_blist___any, sizeof(char *));
					if (a->__any == NULL)
						return NULL;
					*a->__any = NULL;
				}
				if (soap_inliteral(soap, "-any", a->__any))
				{	a->__size++;
					a->__any = NULL;
					continue;
				}
			}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if (a->__any)
			soap_pop_block(soap, soap_blist___any);
		if (a->__size)
			a->__any = (char **)soap_save_block(soap, soap_blist___any, NULL, 1);
		else
		{	a->__any = NULL;
			if (soap_blist___any)
				soap_end_block(soap, soap_blist___any);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _wsnt__Renew *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__wsnt__Renew, 0, sizeof(struct _wsnt__Renew), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_TerminationTime > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__wsnt__Renew(struct soap *soap, const struct _wsnt__Renew *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__wsnt__Renew);
	if (soap_out__wsnt__Renew(soap, tag?tag:"wsnt:Renew", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _wsnt__Renew * SOAP_FMAC4 soap_get__wsnt__Renew(struct soap *soap, struct _wsnt__Renew *p, const char *tag, const char *type)
{
	if ((p = soap_in__wsnt__Renew(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__wsnt__NotificationMessageHolderType_Message(struct soap *soap, struct _wsnt__NotificationMessageHolderType_Message *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__wsnt__NotificationMessageHolderType_Message(struct soap *soap, const struct _wsnt__NotificationMessageHolderType_Message *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__wsnt__NotificationMessageHolderType_Message(struct soap *soap, const char *tag, int id, const struct _wsnt__NotificationMessageHolderType_Message *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__wsnt__NotificationMessageHolderType_Message), type))
		return soap->error;
	soap_outliteral(soap, "-any", &a->__any, NULL);
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _wsnt__NotificationMessageHolderType_Message * SOAP_FMAC4 soap_in__wsnt__NotificationMessageHolderType_Message(struct soap *soap, const char *tag, struct _wsnt__NotificationMessageHolderType_Message *a, const char *type)
{
	size_t soap_flag___any = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _wsnt__NotificationMessageHolderType_Message *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__wsnt__NotificationMessageHolderType_Message, sizeof(struct _wsnt__NotificationMessageHolderType_Message), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__wsnt__NotificationMessageHolderType_Message(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag___any && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_inliteral(soap, "-any", &a->__any))
				{	soap_flag___any--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _wsnt__NotificationMessageHolderType_Message *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__wsnt__NotificationMessageHolderType_Message, 0, sizeof(struct _wsnt__NotificationMessageHolderType_Message), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__wsnt__NotificationMessageHolderType_Message(struct soap *soap, const struct _wsnt__NotificationMessageHolderType_Message *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__wsnt__NotificationMessageHolderType_Message);
	if (soap_out__wsnt__NotificationMessageHolderType_Message(soap, tag?tag:"wsnt:NotificationMessageHolderType-Message", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _wsnt__NotificationMessageHolderType_Message * SOAP_FMAC4 soap_get__wsnt__NotificationMessageHolderType_Message(struct soap *soap, struct _wsnt__NotificationMessageHolderType_Message *p, const char *tag, const char *type)
{
	if ((p = soap_in__wsnt__NotificationMessageHolderType_Message(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTowsnt__TopicExpressionType(struct soap *soap, struct wsnt__TopicExpressionType *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_wsnt__TopicExpressionType))
		soap_serialize_wsnt__TopicExpressionType(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTowsnt__TopicExpressionType(struct soap *soap, const char *tag, int id, struct wsnt__TopicExpressionType *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_wsnt__TopicExpressionType);
	if (id < 0)
		return soap->error;
	return soap_out_wsnt__TopicExpressionType(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct wsnt__TopicExpressionType ** SOAP_FMAC4 soap_in_PointerTowsnt__TopicExpressionType(struct soap *soap, const char *tag, struct wsnt__TopicExpressionType **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct wsnt__TopicExpressionType **)soap_malloc(soap, sizeof(struct wsnt__TopicExpressionType *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_wsnt__TopicExpressionType(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct wsnt__TopicExpressionType **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_wsnt__TopicExpressionType, sizeof(struct wsnt__TopicExpressionType), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTowsnt__TopicExpressionType(struct soap *soap, struct wsnt__TopicExpressionType *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTowsnt__TopicExpressionType);
	if (soap_out_PointerTowsnt__TopicExpressionType(soap, tag?tag:"wsnt:TopicExpressionType", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct wsnt__TopicExpressionType ** SOAP_FMAC4 soap_get_PointerTowsnt__TopicExpressionType(struct soap *soap, struct wsnt__TopicExpressionType **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTowsnt__TopicExpressionType(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTowsa5__EndpointReferenceType(struct soap *soap, struct wsa5__EndpointReferenceType *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_wsa5__EndpointReferenceType))
		soap_serialize_wsa5__EndpointReferenceType(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTowsa5__EndpointReferenceType(struct soap *soap, const char *tag, int id, struct wsa5__EndpointReferenceType *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_wsa5__EndpointReferenceType);
	if (id < 0)
		return soap->error;
	return soap_out_wsa5__EndpointReferenceType(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct wsa5__EndpointReferenceType ** SOAP_FMAC4 soap_in_PointerTowsa5__EndpointReferenceType(struct soap *soap, const char *tag, struct wsa5__EndpointReferenceType **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct wsa5__EndpointReferenceType **)soap_malloc(soap, sizeof(struct wsa5__EndpointReferenceType *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_wsa5__EndpointReferenceType(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct wsa5__EndpointReferenceType **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_wsa5__EndpointReferenceType, sizeof(struct wsa5__EndpointReferenceType), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTowsa5__EndpointReferenceType(struct soap *soap, struct wsa5__EndpointReferenceType *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTowsa5__EndpointReferenceType);
	if (soap_out_PointerTowsa5__EndpointReferenceType(soap, tag?tag:"wsa5:EndpointReferenceType", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct wsa5__EndpointReferenceType ** SOAP_FMAC4 soap_get_PointerTowsa5__EndpointReferenceType(struct soap *soap, struct wsa5__EndpointReferenceType **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTowsa5__EndpointReferenceType(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__wsnt__Subscribe_SubscriptionPolicy(struct soap *soap, const struct _wsnt__Subscribe_SubscriptionPolicy *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__wsnt__Subscribe_SubscriptionPolicy(struct soap *soap, const char *tag, int id, const struct _wsnt__Subscribe_SubscriptionPolicy *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__wsnt__Subscribe_SubscriptionPolicy), type))
		return soap->error;
	if (a->__any)
	{	int i;
		for (i = 0; i < a->__size; i++)
			soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _wsnt__Subscribe_SubscriptionPolicy * SOAP_FMAC4 soap_in__wsnt__Subscribe_SubscriptionPolicy(struct soap *soap, const char *tag, struct _wsnt__Subscribe_SubscriptionPolicy *a, const char *type)
{
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _wsnt__Subscribe_SubscriptionPolicy *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__wsnt__Subscribe_SubscriptionPolicy, sizeof(struct _wsnt__Subscribe_SubscriptionPolicy), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__wsnt__Subscribe_SubscriptionPolicy(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap->error == SOAP_TAG_MISMATCH && !soap_peek_element(soap))
			{	if (a->__any == NULL)
				{	if (soap_blist___any == NULL)
						soap_blist___any = soap_new_block(soap);
					a->__any = (char **)soap_push_block(soap, soap_blist___any, sizeof(char *));
					if (a->__any == NULL)
						return NULL;
					*a->__any = NULL;
				}
				if (soap_inliteral(soap, "-any", a->__any))
				{	a->__size++;
					a->__any = NULL;
					continue;
				}
			}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if (a->__any)
			soap_pop_block(soap, soap_blist___any);
		if (a->__size)
			a->__any = (char **)soap_save_block(soap, soap_blist___any, NULL, 1);
		else
		{	a->__any = NULL;
			if (soap_blist___any)
				soap_end_block(soap, soap_blist___any);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _wsnt__Subscribe_SubscriptionPolicy *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__wsnt__Subscribe_SubscriptionPolicy, 0, sizeof(struct _wsnt__Subscribe_SubscriptionPolicy), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__wsnt__Subscribe_SubscriptionPolicy(struct soap *soap, const struct _wsnt__Subscribe_SubscriptionPolicy *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__wsnt__Subscribe_SubscriptionPolicy);
	if (soap_out__wsnt__Subscribe_SubscriptionPolicy(soap, tag?tag:"wsnt:Subscribe-SubscriptionPolicy", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _wsnt__Subscribe_SubscriptionPolicy * SOAP_FMAC4 soap_get__wsnt__Subscribe_SubscriptionPolicy(struct soap *soap, struct _wsnt__Subscribe_SubscriptionPolicy *p, const char *tag, const char *type)
{
	if ((p = soap_in__wsnt__Subscribe_SubscriptionPolicy(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_wsnt__FilterType(struct soap *soap, struct wsnt__FilterType *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__size = 0;
	a->__any = NULL;
}
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_wsnt__FilterType(struct soap *soap, const struct wsnt__FilterType *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_wsnt__FilterType(struct soap *soap, const char *tag, int id, const struct wsnt__FilterType *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_wsnt__FilterType), type))
		return soap->error;
	if (a->__any)
	{	int i;
		for (i = 0; i < a->__size; i++)
			soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct wsnt__FilterType * SOAP_FMAC4 soap_in_wsnt__FilterType(struct soap *soap, const char *tag, struct wsnt__FilterType *a, const char *type)
{
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct wsnt__FilterType *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_wsnt__FilterType, sizeof(struct wsnt__FilterType), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_wsnt__FilterType(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap->error == SOAP_TAG_MISMATCH && !soap_peek_element(soap))
			{	if (a->__any == NULL)
				{	if (soap_blist___any == NULL)
						soap_blist___any = soap_new_block(soap);
					a->__any = (char **)soap_push_block(soap, soap_blist___any, sizeof(char *));
					if (a->__any == NULL)
						return NULL;
					*a->__any = NULL;
				}
				if (soap_inliteral(soap, "-any", a->__any))
				{	a->__size++;
					a->__any = NULL;
					continue;
				}
			}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if (a->__any)
			soap_pop_block(soap, soap_blist___any);
		if (a->__size)
			a->__any = (char **)soap_save_block(soap, soap_blist___any, NULL, 1);
		else
		{	a->__any = NULL;
			if (soap_blist___any)
				soap_end_block(soap, soap_blist___any);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct wsnt__FilterType *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_wsnt__FilterType, 0, sizeof(struct wsnt__FilterType), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_wsnt__FilterType(struct soap *soap, const struct wsnt__FilterType *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_wsnt__FilterType);
	if (soap_out_wsnt__FilterType(soap, tag?tag:"wsnt:FilterType", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct wsnt__FilterType * SOAP_FMAC4 soap_get_wsnt__FilterType(struct soap *soap, struct wsnt__FilterType *p, const char *tag, const char *type)
{
	if ((p = soap_in_wsnt__FilterType(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_wsnt__TopicExpressionType(struct soap *soap, struct wsnt__TopicExpressionType *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__any = NULL;
	soap_default_string(soap, &a->Dialect);
	a->__anyAttribute = NULL;
	a->__mixed = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_wsnt__TopicExpressionType(struct soap *soap, const struct wsnt__TopicExpressionType *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_wsnt__TopicExpressionType(struct soap *soap, const char *tag, int id, const struct wsnt__TopicExpressionType *a, const char *type)
{
	if (a->Dialect)
		soap_set_attr(soap, "Dialect", a->Dialect, 1);
	if (a->__anyAttribute)
		soap_set_attr(soap, "-anyAttribute", a->__anyAttribute, 1);
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_wsnt__TopicExpressionType), type))
		return soap->error;
	soap_outliteral(soap, "-any", &a->__any, NULL);
	soap_outliteral(soap, "-mixed", &a->__mixed, NULL);
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct wsnt__TopicExpressionType * SOAP_FMAC4 soap_in_wsnt__TopicExpressionType(struct soap *soap, const char *tag, struct wsnt__TopicExpressionType *a, const char *type)
{
	size_t soap_flag___any = 1;
	size_t soap_flag___mixed = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct wsnt__TopicExpressionType *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_wsnt__TopicExpressionType, sizeof(struct wsnt__TopicExpressionType), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_wsnt__TopicExpressionType(soap, a);
	if (soap_s2string(soap, soap_attr_value(soap, "Dialect", 1), &a->Dialect, 0, -1))
		return NULL;
	if (soap_s2string(soap, soap_attr_value(soap, "-anyAttribute", 0), &a->__anyAttribute, 0, -1))
		return NULL;
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag___any && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_inliteral(soap, "-any", &a->__any))
				{	soap_flag___any--;
					continue;
				}
			if (soap_flag___mixed && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_inliteral(soap, "-mixed", &a->__mixed))
				{	soap_flag___mixed--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct wsnt__TopicExpressionType *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_wsnt__TopicExpressionType, 0, sizeof(struct wsnt__TopicExpressionType), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_wsnt__TopicExpressionType(struct soap *soap, const struct wsnt__TopicExpressionType *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_wsnt__TopicExpressionType);
	if (soap_out_wsnt__TopicExpressionType(soap, tag?tag:"wsnt:TopicExpressionType", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct wsnt__TopicExpressionType * SOAP_FMAC4 soap_get_wsnt__TopicExpressionType(struct soap *soap, struct wsnt__TopicExpressionType *p, const char *tag, const char *type)
{
	if ((p = soap_in_wsnt__TopicExpressionType(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__wsnt__Subscribe_SubscriptionPolicy(struct soap *soap, struct _wsnt__Subscribe_SubscriptionPolicy *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__size = 0;
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___desm__Unsubscribe(struct soap *soap, struct __desm__Unsubscribe *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->wsnt__Unsubscribe = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___desm__Unsubscribe(struct soap *soap, const struct __desm__Unsubscribe *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_wsnt__Unsubscribe(soap, &a->wsnt__Unsubscribe);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___desm__Unsubscribe(struct soap *soap, const char *tag, int id, const struct __desm__Unsubscribe *a, const char *type)
{
	if (soap_out_PointerTo_wsnt__Unsubscribe(soap, "wsnt:Unsubscribe", -1, &a->wsnt__Unsubscribe, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __desm__Unsubscribe * SOAP_FMAC4 soap_in___desm__Unsubscribe(struct soap *soap, const char *tag, struct __desm__Unsubscribe *a, const char *type)
{
	size_t soap_flag_wsnt__Unsubscribe = 1;
	short soap_flag;
	a = (struct __desm__Unsubscribe *)soap_id_enter(soap, "", a, SOAP_TYPE___desm__Unsubscribe, sizeof(struct __desm__Unsubscribe), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___desm__Unsubscribe(soap, a);
		for (soap_flag = 0;; soap_flag = 1)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_wsnt__Unsubscribe && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTo_wsnt__Unsubscribe(soap, "wsnt:Unsubscribe", &a->wsnt__Unsubscribe, ""))
				{	soap_flag_wsnt__Unsubscribe--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				if (soap_flag)
				{	soap->error = SOAP_OK;
					break;
				}
			if (soap_flag && soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put___desm__Unsubscribe(struct soap *soap, const struct __desm__Unsubscribe *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___desm__Unsubscribe(soap, tag?tag:"-desm:Unsubscribe", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __desm__Unsubscribe * SOAP_FMAC4 soap_get___desm__Unsubscribe(struct soap *soap, struct __desm__Unsubscribe *p, const char *tag, const char *type)
{
	if ((p = soap_in___desm__Unsubscribe(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

//SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_wsnt__UnsubscribeResponse(struct soap *soap, struct _wsnt__UnsubscribeResponse *const*a)
//{
//	if (!soap_reference(soap, *a, SOAP_TYPE__wsnt__UnsubscribeResponse))
//		soap_serialize__wsnt__UnsubscribeResponse(soap, *a);
//}
//
//SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_wsnt__UnsubscribeResponse(struct soap *soap, const char *tag, int id, struct _wsnt__UnsubscribeResponse *const*a, const char *type)
//{
//	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__wsnt__UnsubscribeResponse);
//	if (id < 0)
//		return soap->error;
//	return soap_out__wsnt__UnsubscribeResponse(soap, tag, id, *a, type);
//}
//
//SOAP_FMAC3 struct _wsnt__UnsubscribeResponse ** SOAP_FMAC4 soap_in_PointerTo_wsnt__UnsubscribeResponse(struct soap *soap, const char *tag, struct _wsnt__UnsubscribeResponse **a, const char *type)
//{
//	if (soap_element_begin_in(soap, tag, 1, NULL))
//		return NULL;
//	if (!a)
//		if (!(a = (struct _wsnt__UnsubscribeResponse **)soap_malloc(soap, sizeof(struct _wsnt__UnsubscribeResponse *))))
//			return NULL;
//	*a = NULL;
//	if (!soap->null && *soap->href != '#')
//	{	soap_revert(soap);
//		if (!(*a = soap_in__wsnt__UnsubscribeResponse(soap, tag, *a, type)))
//			return NULL;
//	}
//	else
//	{	a = (struct _wsnt__UnsubscribeResponse **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__wsnt__UnsubscribeResponse, sizeof(struct _wsnt__UnsubscribeResponse), 0);
//		if (soap->body && soap_element_end_in(soap, tag))
//			return NULL;
//	}
//	return a;
//}
//
//SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_wsnt__UnsubscribeResponse(struct soap *soap, struct _wsnt__UnsubscribeResponse *const*a, const char *tag, const char *type)
//{
//	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_wsnt__UnsubscribeResponse);
//	if (soap_out_PointerTo_wsnt__UnsubscribeResponse(soap, tag?tag:"wsnt:UnsubscribeResponse", id, a, type))
//		return soap->error;
//	return soap_putindependent(soap);
//}
//
//SOAP_FMAC3 struct _wsnt__UnsubscribeResponse ** SOAP_FMAC4 soap_get_PointerTo_wsnt__UnsubscribeResponse(struct soap *soap, struct _wsnt__UnsubscribeResponse **p, const char *tag, const char *type)
//{
//	if ((p = soap_in_PointerTo_wsnt__UnsubscribeResponse(soap, tag, p, type)))
//		if (soap_getindependent(soap))
//			return NULL;
//	return p;
//}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_wsnt__Unsubscribe(struct soap *soap, struct _wsnt__Unsubscribe *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__wsnt__Unsubscribe))
		soap_serialize__wsnt__Unsubscribe(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_wsnt__Unsubscribe(struct soap *soap, const char *tag, int id, struct _wsnt__Unsubscribe *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__wsnt__Unsubscribe);
	if (id < 0)
		return soap->error;
	return soap_out__wsnt__Unsubscribe(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _wsnt__Unsubscribe ** SOAP_FMAC4 soap_in_PointerTo_wsnt__Unsubscribe(struct soap *soap, const char *tag, struct _wsnt__Unsubscribe **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _wsnt__Unsubscribe **)soap_malloc(soap, sizeof(struct _wsnt__Unsubscribe *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in__wsnt__Unsubscribe(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct _wsnt__Unsubscribe **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__wsnt__Unsubscribe, sizeof(struct _wsnt__Unsubscribe), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_wsnt__Unsubscribe(struct soap *soap, struct _wsnt__Unsubscribe *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_wsnt__Unsubscribe);
	if (soap_out_PointerTo_wsnt__Unsubscribe(soap, tag?tag:"wsnt:Unsubscribe", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _wsnt__Unsubscribe ** SOAP_FMAC4 soap_get_PointerTo_wsnt__Unsubscribe(struct soap *soap, struct _wsnt__Unsubscribe **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_wsnt__Unsubscribe(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_wsnt__UnsubscribeResponse(struct soap *soap, struct _wsnt__UnsubscribeResponse *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__wsnt__UnsubscribeResponse))
		soap_serialize__wsnt__UnsubscribeResponse(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_wsnt__UnsubscribeResponse(struct soap *soap, const char *tag, int id, struct _wsnt__UnsubscribeResponse *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__wsnt__UnsubscribeResponse);
	if (id < 0)
		return soap->error;
	return soap_out__wsnt__UnsubscribeResponse(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _wsnt__UnsubscribeResponse ** SOAP_FMAC4 soap_in_PointerTo_wsnt__UnsubscribeResponse(struct soap *soap, const char *tag, struct _wsnt__UnsubscribeResponse **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _wsnt__UnsubscribeResponse **)soap_malloc(soap, sizeof(struct _wsnt__UnsubscribeResponse *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in__wsnt__UnsubscribeResponse(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct _wsnt__UnsubscribeResponse **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__wsnt__UnsubscribeResponse, sizeof(struct _wsnt__UnsubscribeResponse), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_wsnt__UnsubscribeResponse(struct soap *soap, struct _wsnt__UnsubscribeResponse *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_wsnt__UnsubscribeResponse);
	if (soap_out_PointerTo_wsnt__UnsubscribeResponse(soap, tag?tag:"wsnt:UnsubscribeResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _wsnt__UnsubscribeResponse ** SOAP_FMAC4 soap_get_PointerTo_wsnt__UnsubscribeResponse(struct soap *soap, struct _wsnt__UnsubscribeResponse **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_wsnt__UnsubscribeResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__wsnt__Unsubscribe(struct soap *soap, struct _wsnt__Unsubscribe *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__size = 0;
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__wsnt__Unsubscribe(struct soap *soap, const struct _wsnt__Unsubscribe *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__wsnt__Unsubscribe(struct soap *soap, const char *tag, int id, const struct _wsnt__Unsubscribe *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__wsnt__Unsubscribe), type))
		return soap->error;
	if (a->__any)
	{	int i;
		for (i = 0; i < a->__size; i++)
			soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _wsnt__Unsubscribe * SOAP_FMAC4 soap_in__wsnt__Unsubscribe(struct soap *soap, const char *tag, struct _wsnt__Unsubscribe *a, const char *type)
{
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _wsnt__Unsubscribe *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__wsnt__Unsubscribe, sizeof(struct _wsnt__Unsubscribe), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__wsnt__Unsubscribe(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap->error == SOAP_TAG_MISMATCH && !soap_peek_element(soap))
			{	if (a->__any == NULL)
				{	if (soap_blist___any == NULL)
						soap_blist___any = soap_new_block(soap);
					a->__any = (char **)soap_push_block(soap, soap_blist___any, sizeof(char *));
					if (a->__any == NULL)
						return NULL;
					*a->__any = NULL;
				}
				if (soap_inliteral(soap, "-any", a->__any))
				{	a->__size++;
					a->__any = NULL;
					continue;
				}
			}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if (a->__any)
			soap_pop_block(soap, soap_blist___any);
		if (a->__size)
			a->__any = (char **)soap_save_block(soap, soap_blist___any, NULL, 1);
		else
		{	a->__any = NULL;
			if (soap_blist___any)
				soap_end_block(soap, soap_blist___any);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _wsnt__Unsubscribe *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__wsnt__Unsubscribe, 0, sizeof(struct _wsnt__Unsubscribe), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__wsnt__Unsubscribe(struct soap *soap, const struct _wsnt__Unsubscribe *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__wsnt__Unsubscribe);
	if (soap_out__wsnt__Unsubscribe(soap, tag?tag:"wsnt:Unsubscribe", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _wsnt__Unsubscribe * SOAP_FMAC4 soap_get__wsnt__Unsubscribe(struct soap *soap, struct _wsnt__Unsubscribe *p, const char *tag, const char *type)
{
	if ((p = soap_in__wsnt__Unsubscribe(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__wsnt__UnsubscribeResponse(struct soap *soap, struct _wsnt__UnsubscribeResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__size = 0;
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__wsnt__UnsubscribeResponse(struct soap *soap, const struct _wsnt__UnsubscribeResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__wsnt__UnsubscribeResponse(struct soap *soap, const char *tag, int id, const struct _wsnt__UnsubscribeResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__wsnt__UnsubscribeResponse), type))
		return soap->error;
	soap_element_result(soap, "-size");
	if (a->__any)
	{	int i;
		for (i = 0; i < a->__size; i++)
			soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _wsnt__UnsubscribeResponse * SOAP_FMAC4 soap_in__wsnt__UnsubscribeResponse(struct soap *soap, const char *tag, struct _wsnt__UnsubscribeResponse *a, const char *type)
{
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _wsnt__UnsubscribeResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__wsnt__UnsubscribeResponse, sizeof(struct _wsnt__UnsubscribeResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__wsnt__UnsubscribeResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap->error == SOAP_TAG_MISMATCH && !soap_peek_element(soap))
			{	if (a->__any == NULL)
				{	if (soap_blist___any == NULL)
						soap_blist___any = soap_new_block(soap);
					a->__any = (char **)soap_push_block(soap, soap_blist___any, sizeof(char *));
					if (a->__any == NULL)
						return NULL;
					*a->__any = NULL;
				}
				if (soap_inliteral(soap, "-any", a->__any))
				{	a->__size++;
					a->__any = NULL;
					continue;
				}
			}
			soap_check_result(soap, "-size");
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if (a->__any)
			soap_pop_block(soap, soap_blist___any);
		if (a->__size)
			a->__any = (char **)soap_save_block(soap, soap_blist___any, NULL, 1);
		else
		{	a->__any = NULL;
			if (soap_blist___any)
				soap_end_block(soap, soap_blist___any);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _wsnt__UnsubscribeResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__wsnt__UnsubscribeResponse, 0, sizeof(struct _wsnt__UnsubscribeResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__wsnt__UnsubscribeResponse(struct soap *soap, const struct _wsnt__UnsubscribeResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__wsnt__UnsubscribeResponse);
	if (soap_out__wsnt__UnsubscribeResponse(soap, tag?tag:"wsnt:UnsubscribeResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _wsnt__UnsubscribeResponse * SOAP_FMAC4 soap_get__wsnt__UnsubscribeResponse(struct soap *soap, struct _wsnt__UnsubscribeResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__wsnt__UnsubscribeResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

//SOAP_FMAC3 void SOAP_FMAC4 soap_default_time(struct soap *soap, time_t *a)
//{
//	(void)soap; /* appease -Wall -Werror */
//#ifdef SOAP_DEFAULT_time
//	*a = SOAP_DEFAULT_time;
//#else
//	*a = (time_t)0;
//#endif
//}
//
//SOAP_FMAC3 int SOAP_FMAC4 soap_out_time(struct soap *soap, const char *tag, int id, const time_t *a, const char *type)
//{
//	return soap_outdateTime(soap, tag, id, a, type, SOAP_TYPE_time);
//}
//
//SOAP_FMAC3 time_t * SOAP_FMAC4 soap_in_time(struct soap *soap, const char *tag, time_t *a, const char *type)
//{	time_t *p;
//	p = soap_indateTime(soap, tag, a, type, SOAP_TYPE_time);
//	return p;
//}
//
//SOAP_FMAC3 int SOAP_FMAC4 soap_put_time(struct soap *soap, const time_t *a, const char *tag, const char *type)
//{
//	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_time);
//	if (soap_out_time(soap, tag?tag:"dateTime", id, a, type))
//		return soap->error;
//	return soap_putindependent(soap);
//}
//
//SOAP_FMAC3 time_t * SOAP_FMAC4 soap_get_time(struct soap *soap, time_t *p, const char *tag, const char *type)
//{
//	if ((p = soap_in_time(soap, tag, p, type)))
//		if (soap_getindependent(soap))
//			return NULL;
//	return p;
//}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_wstop__Documentation(struct soap *soap, struct wstop__Documentation *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__size = 0;
	a->__any = NULL;
	a->__mixed = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_wstop__Documentation(struct soap *soap, const struct wstop__Documentation *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_wstop__Documentation(struct soap *soap, const char *tag, int id, const struct wstop__Documentation *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_wstop__Documentation), type))
		return soap->error;
	if (a->__any)
	{	int i;
	for (i = 0; i < a->__size; i++)
		soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	soap_outliteral(soap, "-mixed", &a->__mixed, NULL);
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct wstop__Documentation * SOAP_FMAC4 soap_in_wstop__Documentation(struct soap *soap, const char *tag, struct wstop__Documentation *a, const char *type)
{
	struct soap_blist *soap_blist___any = NULL;
	size_t soap_flag___mixed = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct wstop__Documentation *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_wstop__Documentation, sizeof(struct wstop__Documentation), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_wstop__Documentation(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap->error == SOAP_TAG_MISMATCH && !soap_peek_element(soap))
		{	if (a->__any == NULL)
		{	if (soap_blist___any == NULL)
		soap_blist___any = soap_new_block(soap);
		a->__any = (char **)soap_push_block(soap, soap_blist___any, sizeof(char *));
		if (a->__any == NULL)
			return NULL;
		*a->__any = NULL;
		}
		if (soap_inliteral(soap, "-any", a->__any))
		{	a->__size++;
		a->__any = NULL;
		continue;
		}
		}
		if (soap_flag___mixed && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_inliteral(soap, "-mixed", &a->__mixed))
			{	soap_flag___mixed--;
		continue;
		}
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (a->__any)
			soap_pop_block(soap, soap_blist___any);
		if (a->__size)
			a->__any = (char **)soap_save_block(soap, soap_blist___any, NULL, 1);
		else
		{	a->__any = NULL;
		if (soap_blist___any)
			soap_end_block(soap, soap_blist___any);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct wstop__Documentation *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_wstop__Documentation, 0, sizeof(struct wstop__Documentation), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_wstop__Documentation(struct soap *soap, const struct wstop__Documentation *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_wstop__Documentation);
	if (soap_out_wstop__Documentation(soap, tag?tag:"wstop:Documentation", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct wstop__Documentation * SOAP_FMAC4 soap_get_wstop__Documentation(struct soap *soap, struct wstop__Documentation *p, const char *tag, const char *type)
{
	if ((p = soap_in_wstop__Documentation(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}


SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTowstop__Documentation(struct soap *soap, struct wstop__Documentation *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_wstop__Documentation))
		soap_serialize_wstop__Documentation(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTowstop__Documentation(struct soap *soap, const char *tag, int id, struct wstop__Documentation *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_wstop__Documentation);
	if (id < 0)
		return soap->error;
	return soap_out_wstop__Documentation(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct wstop__Documentation ** SOAP_FMAC4 soap_in_PointerTowstop__Documentation(struct soap *soap, const char *tag, struct wstop__Documentation **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct wstop__Documentation **)soap_malloc(soap, sizeof(struct wstop__Documentation *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in_wstop__Documentation(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct wstop__Documentation **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_wstop__Documentation, sizeof(struct wstop__Documentation), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTowstop__Documentation(struct soap *soap, struct wstop__Documentation *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTowstop__Documentation);
	if (soap_out_PointerTowstop__Documentation(soap, tag?tag:"wstop:Documentation", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct wstop__Documentation ** SOAP_FMAC4 soap_get_PointerTowstop__Documentation(struct soap *soap, struct wstop__Documentation **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTowstop__Documentation(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_wstop__TopicSetType(struct soap *soap, struct wstop__TopicSetType *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->documentation = NULL;
	a->__anyAttribute = NULL;
	a->__size = 0;
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_wstop__TopicSetType(struct soap *soap, const struct wstop__TopicSetType *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTowstop__Documentation(soap, &a->documentation);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_wstop__TopicSetType(struct soap *soap, const char *tag, int id, const struct wstop__TopicSetType *a, const char *type)
{
	if (a->__anyAttribute)
		soap_set_attr(soap, "-anyAttribute", a->__anyAttribute, 1);
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_wstop__TopicSetType), type))
		return soap->error;
	if (soap_out_PointerTowstop__Documentation(soap, "wstop:documentation", -1, &a->documentation, ""))
		return soap->error;
	if (a->__any)
	{	int i;
	for (i = 0; i < a->__size; i++)
		soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct wstop__TopicSetType * SOAP_FMAC4 soap_in_wstop__TopicSetType(struct soap *soap, const char *tag, struct wstop__TopicSetType *a, const char *type)
{
	size_t soap_flag_documentation = 1;
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct wstop__TopicSetType *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_wstop__TopicSetType, sizeof(struct wstop__TopicSetType), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_wstop__TopicSetType(soap, a);
	if (soap_s2string(soap, soap_attr_value(soap, "-anyAttribute", 0), &a->__anyAttribute, 0, -1))
		return NULL;
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_documentation && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_PointerTowstop__Documentation(soap, "wstop:documentation", &a->documentation, "wstop:Documentation"))
			{	soap_flag_documentation--;
		continue;
		}
		if (soap->error == SOAP_TAG_MISMATCH && !soap_peek_element(soap))
		{	if (a->__any == NULL)
		{	if (soap_blist___any == NULL)
		soap_blist___any = soap_new_block(soap);
		a->__any = (char **)soap_push_block(soap, soap_blist___any, sizeof(char *));
		if (a->__any == NULL)
			return NULL;
		*a->__any = NULL;
		}
		if (soap_inliteral(soap, "-any", a->__any))
		{	a->__size++;
		a->__any = NULL;
		continue;
		}
		}
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (a->__any)
			soap_pop_block(soap, soap_blist___any);
		if (a->__size)
			a->__any = (char **)soap_save_block(soap, soap_blist___any, NULL, 1);
		else
		{	a->__any = NULL;
		if (soap_blist___any)
			soap_end_block(soap, soap_blist___any);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct wstop__TopicSetType *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_wstop__TopicSetType, 0, sizeof(struct wstop__TopicSetType), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_wstop__TopicSetType(struct soap *soap, const struct wstop__TopicSetType *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_wstop__TopicSetType);
	if (soap_out_wstop__TopicSetType(soap, tag?tag:"wstop:TopicSetType", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct wstop__TopicSetType * SOAP_FMAC4 soap_get_wstop__TopicSetType(struct soap *soap, struct wstop__TopicSetType *p, const char *tag, const char *type)
{
	if ((p = soap_in_wstop__TopicSetType(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTowstop__TopicSetType(struct soap *soap, struct wstop__TopicSetType *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_wstop__TopicSetType))
		soap_serialize_wstop__TopicSetType(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTowstop__TopicSetType(struct soap *soap, const char *tag, int id, struct wstop__TopicSetType *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_wstop__TopicSetType);
	if (id < 0)
		return soap->error;
	return soap_out_wstop__TopicSetType(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct wstop__TopicSetType ** SOAP_FMAC4 soap_in_PointerTowstop__TopicSetType(struct soap *soap, const char *tag, struct wstop__TopicSetType **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct wstop__TopicSetType **)soap_malloc(soap, sizeof(struct wstop__TopicSetType *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in_wstop__TopicSetType(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct wstop__TopicSetType **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_wstop__TopicSetType, sizeof(struct wstop__TopicSetType), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTowstop__TopicSetType(struct soap *soap, struct wstop__TopicSetType *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTowstop__TopicSetType);
	if (soap_out_PointerTowstop__TopicSetType(soap, tag?tag:"wstop:TopicSetType", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct wstop__TopicSetType ** SOAP_FMAC4 soap_get_PointerTowstop__TopicSetType(struct soap *soap, struct wstop__TopicSetType **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTowstop__TopicSetType(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}


SOAP_FMAC3 void SOAP_FMAC4 soap_default__tev__GetEventPropertiesResponse(struct soap *soap, struct _tev__GetEventPropertiesResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__sizeTopicNamespaceLocation = 0;
	a->TopicNamespaceLocation = NULL;
	soap_default_xsd__boolean_(soap, &a->wsnt__FixedTopicSet);
	a->wstop__TopicSet = NULL;
	a->__sizeTopicExpressionDialect = 0;
	a->wsnt__TopicExpressionDialect = NULL;
	a->__sizeMessageContentFilterDialect = 0;
	a->MessageContentFilterDialect = NULL;
	a->__sizeProducerPropertiesFilterDialect = 0;
	a->ProducerPropertiesFilterDialect = NULL;
	a->__sizeMessageContentSchemaLocation = 0;
	a->MessageContentSchemaLocation = NULL;
	a->__size = 0;
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tev__GetEventPropertiesResponse(struct soap *soap, const struct _tev__GetEventPropertiesResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	if (a->TopicNamespaceLocation)
	{	int i;
	for (i = 0; i < a->__sizeTopicNamespaceLocation; i++)
	{
		soap_serialize_string(soap, a->TopicNamespaceLocation + i);
	}
	}
	soap_embedded(soap, &a->wsnt__FixedTopicSet, SOAP_TYPE_xsd__boolean_);
	soap_serialize_PointerTowstop__TopicSetType(soap, &a->wstop__TopicSet);
	if (a->wsnt__TopicExpressionDialect)
	{	int i;
	for (i = 0; i < a->__sizeTopicExpressionDialect; i++)
	{
		soap_serialize_string(soap, a->wsnt__TopicExpressionDialect + i);
	}
	}
	if (a->MessageContentFilterDialect)
	{	int i;
	for (i = 0; i < a->__sizeMessageContentFilterDialect; i++)
	{
		soap_serialize_string(soap, a->MessageContentFilterDialect + i);
	}
	}
	if (a->ProducerPropertiesFilterDialect)
	{	int i;
	for (i = 0; i < a->__sizeProducerPropertiesFilterDialect; i++)
	{
		soap_serialize_string(soap, a->ProducerPropertiesFilterDialect + i);
	}
	}
	if (a->MessageContentSchemaLocation)
	{	int i;
	for (i = 0; i < a->__sizeMessageContentSchemaLocation; i++)
	{
		soap_serialize_string(soap, a->MessageContentSchemaLocation + i);
	}
	}
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tev__GetEventPropertiesResponse(struct soap *soap, const char *tag, int id, const struct _tev__GetEventPropertiesResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tev__GetEventPropertiesResponse), type))
		return soap->error;
	soap_element_result(soap, "-sizeTopicNamespaceLocation");
	if (a->TopicNamespaceLocation)
	{	int i;
	for (i = 0; i < a->__sizeTopicNamespaceLocation; i++)
		if (soap_out_string(soap, "tev:TopicNamespaceLocation", -1, a->TopicNamespaceLocation + i, ""))
			return soap->error;
	}
	if (soap_out_xsd__boolean_(soap, "wsnt:FixedTopicSet", -1, &a->wsnt__FixedTopicSet, ""))
		return soap->error;
	if (a->wstop__TopicSet)
	{	if (soap_out_PointerTowstop__TopicSetType(soap, "wstop:TopicSet", -1, &a->wstop__TopicSet, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "wstop:TopicSet"))
		return soap->error;
	if (a->wsnt__TopicExpressionDialect)
	{	int i;
	for (i = 0; i < a->__sizeTopicExpressionDialect; i++)
		if (soap_out_string(soap, "wsnt:TopicExpressionDialect", -1, a->wsnt__TopicExpressionDialect + i, ""))
			return soap->error;
	}
	if (a->MessageContentFilterDialect)
	{	int i;
	for (i = 0; i < a->__sizeMessageContentFilterDialect; i++)
		if (soap_out_string(soap, "tev:MessageContentFilterDialect", -1, a->MessageContentFilterDialect + i, ""))
			return soap->error;
	}
	if (a->ProducerPropertiesFilterDialect)
	{	int i;
	for (i = 0; i < a->__sizeProducerPropertiesFilterDialect; i++)
		if (soap_out_string(soap, "tev:ProducerPropertiesFilterDialect", -1, a->ProducerPropertiesFilterDialect + i, ""))
			return soap->error;
	}
	if (a->MessageContentSchemaLocation)
	{	int i;
	for (i = 0; i < a->__sizeMessageContentSchemaLocation; i++)
		if (soap_out_string(soap, "tev:MessageContentSchemaLocation", -1, a->MessageContentSchemaLocation + i, ""))
			return soap->error;
	}
	if (a->__any)
	{	int i;
	for (i = 0; i < a->__size; i++)
		soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tev__GetEventPropertiesResponse * SOAP_FMAC4 soap_in__tev__GetEventPropertiesResponse(struct soap *soap, const char *tag, struct _tev__GetEventPropertiesResponse *a, const char *type)
{
	struct soap_blist *soap_blist_TopicNamespaceLocation = NULL;
	size_t soap_flag_wsnt__FixedTopicSet = 1;
	size_t soap_flag_wstop__TopicSet = 1;
	struct soap_blist *soap_blist_wsnt__TopicExpressionDialect = NULL;
	struct soap_blist *soap_blist_MessageContentFilterDialect = NULL;
	struct soap_blist *soap_blist_ProducerPropertiesFilterDialect = NULL;
	struct soap_blist *soap_blist_MessageContentSchemaLocation = NULL;
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tev__GetEventPropertiesResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tev__GetEventPropertiesResponse, sizeof(struct _tev__GetEventPropertiesResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tev__GetEventPropertiesResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tev:TopicNamespaceLocation", 1, NULL))
		{	if (a->TopicNamespaceLocation == NULL)
		{	if (soap_blist_TopicNamespaceLocation == NULL)
		soap_blist_TopicNamespaceLocation = soap_new_block(soap);
		a->TopicNamespaceLocation = (char **)soap_push_block(soap, soap_blist_TopicNamespaceLocation, sizeof(char *));
		if (a->TopicNamespaceLocation == NULL)
			return NULL;
		*a->TopicNamespaceLocation = NULL;
		}
		soap_revert(soap);
		if (soap_in_string(soap, "tev:TopicNamespaceLocation", a->TopicNamespaceLocation, "xsd:string"))
		{	a->__sizeTopicNamespaceLocation++;
		a->TopicNamespaceLocation = NULL;
		continue;
		}
		}
		if (soap_flag_wsnt__FixedTopicSet && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_xsd__boolean_(soap, "wsnt:FixedTopicSet", &a->wsnt__FixedTopicSet, "xsd:boolean"))
			{	soap_flag_wsnt__FixedTopicSet--;
		continue;
		}
		if (soap_flag_wstop__TopicSet && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_PointerTowstop__TopicSetType(soap, "wstop:TopicSet", &a->wstop__TopicSet, "wstop:TopicSetType"))
			{	soap_flag_wstop__TopicSet--;
		continue;
		}
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "wsnt:TopicExpressionDialect", 1, NULL))
		{	if (a->wsnt__TopicExpressionDialect == NULL)
		{	if (soap_blist_wsnt__TopicExpressionDialect == NULL)
		soap_blist_wsnt__TopicExpressionDialect = soap_new_block(soap);
		a->wsnt__TopicExpressionDialect = (char **)soap_push_block(soap, soap_blist_wsnt__TopicExpressionDialect, sizeof(char *));
		if (a->wsnt__TopicExpressionDialect == NULL)
			return NULL;
		*a->wsnt__TopicExpressionDialect = NULL;
		}
		soap_revert(soap);
		if (soap_in_string(soap, "wsnt:TopicExpressionDialect", a->wsnt__TopicExpressionDialect, "xsd:string"))
		{	a->__sizeTopicExpressionDialect++;
		a->wsnt__TopicExpressionDialect = NULL;
		continue;
		}
		}
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tev:MessageContentFilterDialect", 1, NULL))
		{	if (a->MessageContentFilterDialect == NULL)
		{	if (soap_blist_MessageContentFilterDialect == NULL)
		soap_blist_MessageContentFilterDialect = soap_new_block(soap);
		a->MessageContentFilterDialect = (char **)soap_push_block(soap, soap_blist_MessageContentFilterDialect, sizeof(char *));
		if (a->MessageContentFilterDialect == NULL)
			return NULL;
		*a->MessageContentFilterDialect = NULL;
		}
		soap_revert(soap);
		if (soap_in_string(soap, "tev:MessageContentFilterDialect", a->MessageContentFilterDialect, "xsd:string"))
		{	a->__sizeMessageContentFilterDialect++;
		a->MessageContentFilterDialect = NULL;
		continue;
		}
		}
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tev:ProducerPropertiesFilterDialect", 1, NULL))
		{	if (a->ProducerPropertiesFilterDialect == NULL)
		{	if (soap_blist_ProducerPropertiesFilterDialect == NULL)
		soap_blist_ProducerPropertiesFilterDialect = soap_new_block(soap);
		a->ProducerPropertiesFilterDialect = (char **)soap_push_block(soap, soap_blist_ProducerPropertiesFilterDialect, sizeof(char *));
		if (a->ProducerPropertiesFilterDialect == NULL)
			return NULL;
		*a->ProducerPropertiesFilterDialect = NULL;
		}
		soap_revert(soap);
		if (soap_in_string(soap, "tev:ProducerPropertiesFilterDialect", a->ProducerPropertiesFilterDialect, "xsd:string"))
		{	a->__sizeProducerPropertiesFilterDialect++;
		a->ProducerPropertiesFilterDialect = NULL;
		continue;
		}
		}
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tev:MessageContentSchemaLocation", 1, NULL))
		{	if (a->MessageContentSchemaLocation == NULL)
		{	if (soap_blist_MessageContentSchemaLocation == NULL)
		soap_blist_MessageContentSchemaLocation = soap_new_block(soap);
		a->MessageContentSchemaLocation = (char **)soap_push_block(soap, soap_blist_MessageContentSchemaLocation, sizeof(char *));
		if (a->MessageContentSchemaLocation == NULL)
			return NULL;
		*a->MessageContentSchemaLocation = NULL;
		}
		soap_revert(soap);
		if (soap_in_string(soap, "tev:MessageContentSchemaLocation", a->MessageContentSchemaLocation, "xsd:string"))
		{	a->__sizeMessageContentSchemaLocation++;
		a->MessageContentSchemaLocation = NULL;
		continue;
		}
		}
		if (soap->error == SOAP_TAG_MISMATCH && !soap_peek_element(soap))
		{	if (a->__any == NULL)
		{	if (soap_blist___any == NULL)
		soap_blist___any = soap_new_block(soap);
		a->__any = (char **)soap_push_block(soap, soap_blist___any, sizeof(char *));
		if (a->__any == NULL)
			return NULL;
		*a->__any = NULL;
		}
		if (soap_inliteral(soap, "-any", a->__any))
		{	a->__size++;
		a->__any = NULL;
		continue;
		}
		}
		soap_check_result(soap, "-sizeTopicNamespaceLocation");
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (a->TopicNamespaceLocation)
			soap_pop_block(soap, soap_blist_TopicNamespaceLocation);
		if (a->__sizeTopicNamespaceLocation)
			a->TopicNamespaceLocation = (char **)soap_save_block(soap, soap_blist_TopicNamespaceLocation, NULL, 1);
		else
		{	a->TopicNamespaceLocation = NULL;
		if (soap_blist_TopicNamespaceLocation)
			soap_end_block(soap, soap_blist_TopicNamespaceLocation);
		}
		if (a->wsnt__TopicExpressionDialect)
			soap_pop_block(soap, soap_blist_wsnt__TopicExpressionDialect);
		if (a->__sizeTopicExpressionDialect)
			a->wsnt__TopicExpressionDialect = (char **)soap_save_block(soap, soap_blist_wsnt__TopicExpressionDialect, NULL, 1);
		else
		{	a->wsnt__TopicExpressionDialect = NULL;
		if (soap_blist_wsnt__TopicExpressionDialect)
			soap_end_block(soap, soap_blist_wsnt__TopicExpressionDialect);
		}
		if (a->MessageContentFilterDialect)
			soap_pop_block(soap, soap_blist_MessageContentFilterDialect);
		if (a->__sizeMessageContentFilterDialect)
			a->MessageContentFilterDialect = (char **)soap_save_block(soap, soap_blist_MessageContentFilterDialect, NULL, 1);
		else
		{	a->MessageContentFilterDialect = NULL;
		if (soap_blist_MessageContentFilterDialect)
			soap_end_block(soap, soap_blist_MessageContentFilterDialect);
		}
		if (a->ProducerPropertiesFilterDialect)
			soap_pop_block(soap, soap_blist_ProducerPropertiesFilterDialect);
		if (a->__sizeProducerPropertiesFilterDialect)
			a->ProducerPropertiesFilterDialect = (char **)soap_save_block(soap, soap_blist_ProducerPropertiesFilterDialect, NULL, 1);
		else
		{	a->ProducerPropertiesFilterDialect = NULL;
		if (soap_blist_ProducerPropertiesFilterDialect)
			soap_end_block(soap, soap_blist_ProducerPropertiesFilterDialect);
		}
		if (a->MessageContentSchemaLocation)
			soap_pop_block(soap, soap_blist_MessageContentSchemaLocation);
		if (a->__sizeMessageContentSchemaLocation)
			a->MessageContentSchemaLocation = (char **)soap_save_block(soap, soap_blist_MessageContentSchemaLocation, NULL, 1);
		else
		{	a->MessageContentSchemaLocation = NULL;
		if (soap_blist_MessageContentSchemaLocation)
			soap_end_block(soap, soap_blist_MessageContentSchemaLocation);
		}
		if (a->__any)
			soap_pop_block(soap, soap_blist___any);
		if (a->__size)
			a->__any = (char **)soap_save_block(soap, soap_blist___any, NULL, 1);
		else
		{	a->__any = NULL;
		if (soap_blist___any)
			soap_end_block(soap, soap_blist___any);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _tev__GetEventPropertiesResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tev__GetEventPropertiesResponse, 0, sizeof(struct _tev__GetEventPropertiesResponse), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (a->__sizeTopicNamespaceLocation < 1 || soap_flag_wsnt__FixedTopicSet > 0 || soap_flag_wstop__TopicSet > 0 || a->__sizeTopicExpressionDialect < 1 || a->__sizeMessageContentFilterDialect < 1 || a->__sizeMessageContentSchemaLocation < 1))
	{	soap->error = SOAP_OCCURS;
	return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tev__GetEventPropertiesResponse(struct soap *soap, const struct _tev__GetEventPropertiesResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tev__GetEventPropertiesResponse);
	if (soap_out__tev__GetEventPropertiesResponse(soap, tag?tag:"tev:GetEventPropertiesResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tev__GetEventPropertiesResponse * SOAP_FMAC4 soap_get__tev__GetEventPropertiesResponse(struct soap *soap, struct _tev__GetEventPropertiesResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tev__GetEventPropertiesResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tev__GetEventProperties(struct soap *soap, struct _tev__GetEventProperties *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tev__GetEventProperties(struct soap *soap, const struct _tev__GetEventProperties *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tev__GetEventProperties(struct soap *soap, const char *tag, int id, const struct _tev__GetEventProperties *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tev__GetEventProperties), type))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tev__GetEventProperties * SOAP_FMAC4 soap_in__tev__GetEventProperties(struct soap *soap, const char *tag, struct _tev__GetEventProperties *a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tev__GetEventProperties *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tev__GetEventProperties, sizeof(struct _tev__GetEventProperties), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tev__GetEventProperties(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _tev__GetEventProperties *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tev__GetEventProperties, 0, sizeof(struct _tev__GetEventProperties), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tev__GetEventProperties(struct soap *soap, const struct _tev__GetEventProperties *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tev__GetEventProperties);
	if (soap_out__tev__GetEventProperties(soap, tag?tag:"tev:GetEventProperties", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tev__GetEventProperties * SOAP_FMAC4 soap_get__tev__GetEventProperties(struct soap *soap, struct _tev__GetEventProperties *p, const char *tag, const char *type)
{
	if ((p = soap_in__tev__GetEventProperties(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tev__GetEventPropertiesResponse(struct soap *soap, struct _tev__GetEventPropertiesResponse *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tev__GetEventPropertiesResponse))
		soap_serialize__tev__GetEventPropertiesResponse(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tev__GetEventPropertiesResponse(struct soap *soap, const char *tag, int id, struct _tev__GetEventPropertiesResponse *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tev__GetEventPropertiesResponse);
	if (id < 0)
		return soap->error;
	return soap_out__tev__GetEventPropertiesResponse(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tev__GetEventPropertiesResponse ** SOAP_FMAC4 soap_in_PointerTo_tev__GetEventPropertiesResponse(struct soap *soap, const char *tag, struct _tev__GetEventPropertiesResponse **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tev__GetEventPropertiesResponse **)soap_malloc(soap, sizeof(struct _tev__GetEventPropertiesResponse *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tev__GetEventPropertiesResponse(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tev__GetEventPropertiesResponse **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tev__GetEventPropertiesResponse, sizeof(struct _tev__GetEventPropertiesResponse), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tev__GetEventPropertiesResponse(struct soap *soap, struct _tev__GetEventPropertiesResponse *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tev__GetEventPropertiesResponse);
	if (soap_out_PointerTo_tev__GetEventPropertiesResponse(soap, tag?tag:"tev:GetEventPropertiesResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tev__GetEventPropertiesResponse ** SOAP_FMAC4 soap_get_PointerTo_tev__GetEventPropertiesResponse(struct soap *soap, struct _tev__GetEventPropertiesResponse **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tev__GetEventPropertiesResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tev__GetEventProperties(struct soap *soap, struct _tev__GetEventProperties *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tev__GetEventProperties))
		soap_serialize__tev__GetEventProperties(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tev__GetEventProperties(struct soap *soap, const char *tag, int id, struct _tev__GetEventProperties *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tev__GetEventProperties);
	if (id < 0)
		return soap->error;
	return soap_out__tev__GetEventProperties(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tev__GetEventProperties ** SOAP_FMAC4 soap_in_PointerTo_tev__GetEventProperties(struct soap *soap, const char *tag, struct _tev__GetEventProperties **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tev__GetEventProperties **)soap_malloc(soap, sizeof(struct _tev__GetEventProperties *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tev__GetEventProperties(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tev__GetEventProperties **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tev__GetEventProperties, sizeof(struct _tev__GetEventProperties), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tev__GetEventProperties(struct soap *soap, struct _tev__GetEventProperties *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tev__GetEventProperties);
	if (soap_out_PointerTo_tev__GetEventProperties(soap, tag?tag:"tev:GetEventProperties", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tev__GetEventProperties ** SOAP_FMAC4 soap_get_PointerTo_tev__GetEventProperties(struct soap *soap, struct _tev__GetEventProperties **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tev__GetEventProperties(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___dee__GetEventProperties(struct soap *soap, struct __dee__GetEventProperties *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->tev__GetEventProperties = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___dee__GetEventProperties(struct soap *soap, const struct __dee__GetEventProperties *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_tev__GetEventProperties(soap, &a->tev__GetEventProperties);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___dee__GetEventProperties(struct soap *soap, const char *tag, int id, const struct __dee__GetEventProperties *a, const char *type)
{
	if (soap_out_PointerTo_tev__GetEventProperties(soap, "tev:GetEventProperties", -1, &a->tev__GetEventProperties, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __dee__GetEventProperties * SOAP_FMAC4 soap_in___dee__GetEventProperties(struct soap *soap, const char *tag, struct __dee__GetEventProperties *a, const char *type)
{
	size_t soap_flag_tev__GetEventProperties = 1;
	short soap_flag;
	a = (struct __dee__GetEventProperties *)soap_id_enter(soap, "", a, SOAP_TYPE___dee__GetEventProperties, sizeof(struct __dee__GetEventProperties), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___dee__GetEventProperties(soap, a);
	for (soap_flag = 0;; soap_flag = 1)
	{	soap->error = SOAP_TAG_MISMATCH;
	if (soap_flag_tev__GetEventProperties && soap->error == SOAP_TAG_MISMATCH)
		if (soap_in_PointerTo_tev__GetEventProperties(soap, "tev:GetEventProperties", &a->tev__GetEventProperties, ""))
		{	soap_flag_tev__GetEventProperties--;
	continue;
	}
	if (soap->error == SOAP_TAG_MISMATCH)
		if (soap_flag)
		{	soap->error = SOAP_OK;
	break;
	}
	if (soap_flag && soap->error == SOAP_NO_TAG)
		break;
	if (soap->error)
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put___dee__GetEventProperties(struct soap *soap, const struct __dee__GetEventProperties *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___dee__GetEventProperties(soap, tag?tag:"-dee:GetEventProperties", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __dee__GetEventProperties * SOAP_FMAC4 soap_get___dee__GetEventProperties(struct soap *soap, struct __dee__GetEventProperties *p, const char *tag, const char *type)
{
	if ((p = soap_in___dee__GetEventProperties(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___dee__CreatePullPointSubscription(struct soap *soap, struct __dee__CreatePullPointSubscription *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->tev__CreatePullPointSubscription = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___dee__CreatePullPointSubscription(struct soap *soap, const struct __dee__CreatePullPointSubscription *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_tev__CreatePullPointSubscription(soap, &a->tev__CreatePullPointSubscription);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___dee__CreatePullPointSubscription(struct soap *soap, const char *tag, int id, const struct __dee__CreatePullPointSubscription *a, const char *type)
{
	if (soap_out_PointerTo_tev__CreatePullPointSubscription(soap, "tev:CreatePullPointSubscription", -1, &a->tev__CreatePullPointSubscription, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __dee__CreatePullPointSubscription * SOAP_FMAC4 soap_in___dee__CreatePullPointSubscription(struct soap *soap, const char *tag, struct __dee__CreatePullPointSubscription *a, const char *type)
{
	size_t soap_flag_tev__CreatePullPointSubscription = 1;
	short soap_flag;
	a = (struct __dee__CreatePullPointSubscription *)soap_id_enter(soap, "", a, SOAP_TYPE___dee__CreatePullPointSubscription, sizeof(struct __dee__CreatePullPointSubscription), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___dee__CreatePullPointSubscription(soap, a);
	for (soap_flag = 0;; soap_flag = 1)
	{	soap->error = SOAP_TAG_MISMATCH;
	if (soap_flag_tev__CreatePullPointSubscription && soap->error == SOAP_TAG_MISMATCH)
		if (soap_in_PointerTo_tev__CreatePullPointSubscription(soap, "tev:CreatePullPointSubscription", &a->tev__CreatePullPointSubscription, ""))
		{	soap_flag_tev__CreatePullPointSubscription--;
	continue;
	}
	if (soap->error == SOAP_TAG_MISMATCH)
		if (soap_flag)
		{	soap->error = SOAP_OK;
	break;
	}
	if (soap_flag && soap->error == SOAP_NO_TAG)
		break;
	if (soap->error)
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put___dee__CreatePullPointSubscription(struct soap *soap, const struct __dee__CreatePullPointSubscription *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___dee__CreatePullPointSubscription(soap, tag?tag:"-dee:CreatePullPointSubscription", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __dee__CreatePullPointSubscription * SOAP_FMAC4 soap_get___dee__CreatePullPointSubscription(struct soap *soap, struct __dee__CreatePullPointSubscription *p, const char *tag, const char *type)
{
	if ((p = soap_in___dee__CreatePullPointSubscription(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tev__CreatePullPointSubscriptionResponse(struct soap *soap, struct _tev__CreatePullPointSubscriptionResponse *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tev__CreatePullPointSubscriptionResponse))
		soap_serialize__tev__CreatePullPointSubscriptionResponse(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tev__CreatePullPointSubscriptionResponse(struct soap *soap, const char *tag, int id, struct _tev__CreatePullPointSubscriptionResponse *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tev__CreatePullPointSubscriptionResponse);
	if (id < 0)
		return soap->error;
	return soap_out__tev__CreatePullPointSubscriptionResponse(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tev__CreatePullPointSubscriptionResponse ** SOAP_FMAC4 soap_in_PointerTo_tev__CreatePullPointSubscriptionResponse(struct soap *soap, const char *tag, struct _tev__CreatePullPointSubscriptionResponse **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tev__CreatePullPointSubscriptionResponse **)soap_malloc(soap, sizeof(struct _tev__CreatePullPointSubscriptionResponse *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tev__CreatePullPointSubscriptionResponse(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tev__CreatePullPointSubscriptionResponse **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tev__CreatePullPointSubscriptionResponse, sizeof(struct _tev__CreatePullPointSubscriptionResponse), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tev__CreatePullPointSubscriptionResponse(struct soap *soap, struct _tev__CreatePullPointSubscriptionResponse *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tev__CreatePullPointSubscriptionResponse);
	if (soap_out_PointerTo_tev__CreatePullPointSubscriptionResponse(soap, tag?tag:"tev:CreatePullPointSubscriptionResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tev__CreatePullPointSubscriptionResponse ** SOAP_FMAC4 soap_get_PointerTo_tev__CreatePullPointSubscriptionResponse(struct soap *soap, struct _tev__CreatePullPointSubscriptionResponse **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tev__CreatePullPointSubscriptionResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tev__CreatePullPointSubscription(struct soap *soap, struct _tev__CreatePullPointSubscription *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tev__CreatePullPointSubscription))
		soap_serialize__tev__CreatePullPointSubscription(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tev__CreatePullPointSubscription(struct soap *soap, const char *tag, int id, struct _tev__CreatePullPointSubscription *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tev__CreatePullPointSubscription);
	if (id < 0)
		return soap->error;
	return soap_out__tev__CreatePullPointSubscription(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tev__CreatePullPointSubscription ** SOAP_FMAC4 soap_in_PointerTo_tev__CreatePullPointSubscription(struct soap *soap, const char *tag, struct _tev__CreatePullPointSubscription **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tev__CreatePullPointSubscription **)soap_malloc(soap, sizeof(struct _tev__CreatePullPointSubscription *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tev__CreatePullPointSubscription(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tev__CreatePullPointSubscription **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tev__CreatePullPointSubscription, sizeof(struct _tev__CreatePullPointSubscription), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tev__CreatePullPointSubscription(struct soap *soap, struct _tev__CreatePullPointSubscription *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tev__CreatePullPointSubscription);
	if (soap_out_PointerTo_tev__CreatePullPointSubscription(soap, tag?tag:"tev:CreatePullPointSubscription", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tev__CreatePullPointSubscription ** SOAP_FMAC4 soap_get_PointerTo_tev__CreatePullPointSubscription(struct soap *soap, struct _tev__CreatePullPointSubscription **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tev__CreatePullPointSubscription(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tev__CreatePullPointSubscriptionResponse(struct soap *soap, struct _tev__CreatePullPointSubscriptionResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_wsa5__EndpointReferenceType(soap, &a->SubscriptionReference);
	soap_default_time(soap, &a->wsnt__CurrentTime);
	soap_default_time(soap, &a->wsnt__TerminationTime);
	a->__size = 0;
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tev__CreatePullPointSubscriptionResponse(struct soap *soap, const struct _tev__CreatePullPointSubscriptionResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_embedded(soap, &a->SubscriptionReference, SOAP_TYPE_wsa5__EndpointReferenceType);
	soap_serialize_wsa5__EndpointReferenceType(soap, &a->SubscriptionReference);
	soap_embedded(soap, &a->wsnt__CurrentTime, SOAP_TYPE_time);
	soap_embedded(soap, &a->wsnt__TerminationTime, SOAP_TYPE_time);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tev__CreatePullPointSubscriptionResponse(struct soap *soap, const char *tag, int id, const struct _tev__CreatePullPointSubscriptionResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tev__CreatePullPointSubscriptionResponse), type))
		return soap->error;
	soap_element_result(soap, "tev:SubscriptionReference");
	if (soap_out_wsa5__EndpointReferenceType(soap, "tev:SubscriptionReference", -1, &a->SubscriptionReference, ""))
		return soap->error;
	if (soap_out_time(soap, "wsnt:CurrentTime", -1, &a->wsnt__CurrentTime, ""))
		return soap->error;
	if (soap_out_time(soap, "wsnt:TerminationTime", -1, &a->wsnt__TerminationTime, ""))
		return soap->error;
	if (a->__any)
	{	int i;
	for (i = 0; i < a->__size; i++)
		soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tev__CreatePullPointSubscriptionResponse * SOAP_FMAC4 soap_in__tev__CreatePullPointSubscriptionResponse(struct soap *soap, const char *tag, struct _tev__CreatePullPointSubscriptionResponse *a, const char *type)
{
	size_t soap_flag_SubscriptionReference = 1;
	size_t soap_flag_wsnt__CurrentTime = 1;
	size_t soap_flag_wsnt__TerminationTime = 1;
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tev__CreatePullPointSubscriptionResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tev__CreatePullPointSubscriptionResponse, sizeof(struct _tev__CreatePullPointSubscriptionResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tev__CreatePullPointSubscriptionResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_SubscriptionReference && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_wsa5__EndpointReferenceType(soap, "tev:SubscriptionReference", &a->SubscriptionReference, "wsa5:EndpointReferenceType"))
			{	soap_flag_SubscriptionReference--;
		continue;
		}
		if (soap_flag_wsnt__CurrentTime && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_time(soap, "wsnt:CurrentTime", &a->wsnt__CurrentTime, "xsd:dateTime"))
			{	soap_flag_wsnt__CurrentTime--;
		continue;
		}
		if (soap_flag_wsnt__TerminationTime && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_time(soap, "wsnt:TerminationTime", &a->wsnt__TerminationTime, "xsd:dateTime"))
			{	soap_flag_wsnt__TerminationTime--;
		continue;
		}
		if (soap->error == SOAP_TAG_MISMATCH && !soap_peek_element(soap))
		{	if (a->__any == NULL)
		{	if (soap_blist___any == NULL)
		soap_blist___any = soap_new_block(soap);
		a->__any = (char **)soap_push_block(soap, soap_blist___any, sizeof(char *));
		if (a->__any == NULL)
			return NULL;
		*a->__any = NULL;
		}
		if (soap_inliteral(soap, "-any", a->__any))
		{	a->__size++;
		a->__any = NULL;
		continue;
		}
		}
		soap_check_result(soap, "tev:SubscriptionReference");
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (a->__any)
			soap_pop_block(soap, soap_blist___any);
		if (a->__size)
			a->__any = (char **)soap_save_block(soap, soap_blist___any, NULL, 1);
		else
		{	a->__any = NULL;
		if (soap_blist___any)
			soap_end_block(soap, soap_blist___any);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _tev__CreatePullPointSubscriptionResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tev__CreatePullPointSubscriptionResponse, 0, sizeof(struct _tev__CreatePullPointSubscriptionResponse), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_SubscriptionReference > 0 || soap_flag_wsnt__CurrentTime > 0 || soap_flag_wsnt__TerminationTime > 0))
	{	soap->error = SOAP_OCCURS;
	return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tev__CreatePullPointSubscriptionResponse(struct soap *soap, const struct _tev__CreatePullPointSubscriptionResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tev__CreatePullPointSubscriptionResponse);
	if (soap_out__tev__CreatePullPointSubscriptionResponse(soap, tag?tag:"tev:CreatePullPointSubscriptionResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tev__CreatePullPointSubscriptionResponse * SOAP_FMAC4 soap_get__tev__CreatePullPointSubscriptionResponse(struct soap *soap, struct _tev__CreatePullPointSubscriptionResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tev__CreatePullPointSubscriptionResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tev__CreatePullPointSubscription(struct soap *soap, struct _tev__CreatePullPointSubscription *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->Filter = NULL;
	soap_default_wsnt__AbsoluteOrRelativeTimeType(soap, &a->InitialTerminationTime);
	a->SubscriptionPolicy = NULL;
	a->__size = 0;
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tev__CreatePullPointSubscription(struct soap *soap, const struct _tev__CreatePullPointSubscription *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTowsnt__FilterType(soap, &a->Filter);
	soap_serialize_wsnt__AbsoluteOrRelativeTimeType(soap, &a->InitialTerminationTime);
	soap_serialize_PointerTo_tev__CreatePullPointSubscription_SubscriptionPolicy(soap, &a->SubscriptionPolicy);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tev__CreatePullPointSubscription(struct soap *soap, const char *tag, int id, const struct _tev__CreatePullPointSubscription *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tev__CreatePullPointSubscription), type))
		return soap->error;
	if (soap_out_PointerTowsnt__FilterType(soap, "tev:Filter", -1, &a->Filter, ""))
		return soap->error;
	if (soap_out_wsnt__AbsoluteOrRelativeTimeType(soap, "tev:InitialTerminationTime", -1, &a->InitialTerminationTime, ""))
		return soap->error;
	if (soap_out_PointerTo_tev__CreatePullPointSubscription_SubscriptionPolicy(soap, "tev:SubscriptionPolicy", -1, &a->SubscriptionPolicy, ""))
		return soap->error;
	if (a->__any)
	{	int i;
	for (i = 0; i < a->__size; i++)
		soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tev__CreatePullPointSubscription * SOAP_FMAC4 soap_in__tev__CreatePullPointSubscription(struct soap *soap, const char *tag, struct _tev__CreatePullPointSubscription *a, const char *type)
{
	size_t soap_flag_Filter = 1;
	size_t soap_flag_InitialTerminationTime = 1;
	size_t soap_flag_SubscriptionPolicy = 1;
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tev__CreatePullPointSubscription *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tev__CreatePullPointSubscription, sizeof(struct _tev__CreatePullPointSubscription), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tev__CreatePullPointSubscription(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_Filter && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_PointerTowsnt__FilterType(soap, "tev:Filter", &a->Filter, "wsnt:FilterType"))
			{	soap_flag_Filter--;
		continue;
		}
		if (soap_flag_InitialTerminationTime && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_wsnt__AbsoluteOrRelativeTimeType(soap, "tev:InitialTerminationTime", &a->InitialTerminationTime, "wsnt:AbsoluteOrRelativeTimeType"))
			{	soap_flag_InitialTerminationTime--;
		continue;
		}
		if (soap_flag_SubscriptionPolicy && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_PointerTo_tev__CreatePullPointSubscription_SubscriptionPolicy(soap, "tev:SubscriptionPolicy", &a->SubscriptionPolicy, ""))
			{	soap_flag_SubscriptionPolicy--;
		continue;
		}
		if (soap->error == SOAP_TAG_MISMATCH && !soap_peek_element(soap))
		{	if (a->__any == NULL)
		{	if (soap_blist___any == NULL)
		soap_blist___any = soap_new_block(soap);
		a->__any = (char **)soap_push_block(soap, soap_blist___any, sizeof(char *));
		if (a->__any == NULL)
			return NULL;
		*a->__any = NULL;
		}
		if (soap_inliteral(soap, "-any", a->__any))
		{	a->__size++;
		a->__any = NULL;
		continue;
		}
		}
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (a->__any)
			soap_pop_block(soap, soap_blist___any);
		if (a->__size)
			a->__any = (char **)soap_save_block(soap, soap_blist___any, NULL, 1);
		else
		{	a->__any = NULL;
		if (soap_blist___any)
			soap_end_block(soap, soap_blist___any);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _tev__CreatePullPointSubscription *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tev__CreatePullPointSubscription, 0, sizeof(struct _tev__CreatePullPointSubscription), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tev__CreatePullPointSubscription(struct soap *soap, const struct _tev__CreatePullPointSubscription *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tev__CreatePullPointSubscription);
	if (soap_out__tev__CreatePullPointSubscription(soap, tag?tag:"tev:CreatePullPointSubscription", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tev__CreatePullPointSubscription * SOAP_FMAC4 soap_get__tev__CreatePullPointSubscription(struct soap *soap, struct _tev__CreatePullPointSubscription *p, const char *tag, const char *type)
{
	if ((p = soap_in__tev__CreatePullPointSubscription(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tev__CreatePullPointSubscription_SubscriptionPolicy(struct soap *soap, struct _tev__CreatePullPointSubscription_SubscriptionPolicy *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__size = 0;
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tev__CreatePullPointSubscription_SubscriptionPolicy(struct soap *soap, const struct _tev__CreatePullPointSubscription_SubscriptionPolicy *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tev__CreatePullPointSubscription_SubscriptionPolicy(struct soap *soap, const char *tag, int id, const struct _tev__CreatePullPointSubscription_SubscriptionPolicy *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tev__CreatePullPointSubscription_SubscriptionPolicy), type))
		return soap->error;
	if (a->__any)
	{	int i;
	for (i = 0; i < a->__size; i++)
		soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tev__CreatePullPointSubscription_SubscriptionPolicy * SOAP_FMAC4 soap_in__tev__CreatePullPointSubscription_SubscriptionPolicy(struct soap *soap, const char *tag, struct _tev__CreatePullPointSubscription_SubscriptionPolicy *a, const char *type)
{
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tev__CreatePullPointSubscription_SubscriptionPolicy *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tev__CreatePullPointSubscription_SubscriptionPolicy, sizeof(struct _tev__CreatePullPointSubscription_SubscriptionPolicy), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tev__CreatePullPointSubscription_SubscriptionPolicy(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap->error == SOAP_TAG_MISMATCH && !soap_peek_element(soap))
		{	if (a->__any == NULL)
		{	if (soap_blist___any == NULL)
		soap_blist___any = soap_new_block(soap);
		a->__any = (char **)soap_push_block(soap, soap_blist___any, sizeof(char *));
		if (a->__any == NULL)
			return NULL;
		*a->__any = NULL;
		}
		if (soap_inliteral(soap, "-any", a->__any))
		{	a->__size++;
		a->__any = NULL;
		continue;
		}
		}
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (a->__any)
			soap_pop_block(soap, soap_blist___any);
		if (a->__size)
			a->__any = (char **)soap_save_block(soap, soap_blist___any, NULL, 1);
		else
		{	a->__any = NULL;
		if (soap_blist___any)
			soap_end_block(soap, soap_blist___any);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _tev__CreatePullPointSubscription_SubscriptionPolicy *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tev__CreatePullPointSubscription_SubscriptionPolicy, 0, sizeof(struct _tev__CreatePullPointSubscription_SubscriptionPolicy), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tev__CreatePullPointSubscription_SubscriptionPolicy(struct soap *soap, const struct _tev__CreatePullPointSubscription_SubscriptionPolicy *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tev__CreatePullPointSubscription_SubscriptionPolicy);
	if (soap_out__tev__CreatePullPointSubscription_SubscriptionPolicy(soap, tag?tag:"tev:CreatePullPointSubscription-SubscriptionPolicy", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tev__CreatePullPointSubscription_SubscriptionPolicy * SOAP_FMAC4 soap_get__tev__CreatePullPointSubscription_SubscriptionPolicy(struct soap *soap, struct _tev__CreatePullPointSubscription_SubscriptionPolicy *p, const char *tag, const char *type)
{
	if ((p = soap_in__tev__CreatePullPointSubscription_SubscriptionPolicy(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tev__CreatePullPointSubscription_SubscriptionPolicy(struct soap *soap, struct _tev__CreatePullPointSubscription_SubscriptionPolicy *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tev__CreatePullPointSubscription_SubscriptionPolicy))
		soap_serialize__tev__CreatePullPointSubscription_SubscriptionPolicy(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tev__CreatePullPointSubscription_SubscriptionPolicy(struct soap *soap, const char *tag, int id, struct _tev__CreatePullPointSubscription_SubscriptionPolicy *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tev__CreatePullPointSubscription_SubscriptionPolicy);
	if (id < 0)
		return soap->error;
	return soap_out__tev__CreatePullPointSubscription_SubscriptionPolicy(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tev__CreatePullPointSubscription_SubscriptionPolicy ** SOAP_FMAC4 soap_in_PointerTo_tev__CreatePullPointSubscription_SubscriptionPolicy(struct soap *soap, const char *tag, struct _tev__CreatePullPointSubscription_SubscriptionPolicy **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tev__CreatePullPointSubscription_SubscriptionPolicy **)soap_malloc(soap, sizeof(struct _tev__CreatePullPointSubscription_SubscriptionPolicy *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tev__CreatePullPointSubscription_SubscriptionPolicy(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tev__CreatePullPointSubscription_SubscriptionPolicy **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tev__CreatePullPointSubscription_SubscriptionPolicy, sizeof(struct _tev__CreatePullPointSubscription_SubscriptionPolicy), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tev__CreatePullPointSubscription_SubscriptionPolicy(struct soap *soap, struct _tev__CreatePullPointSubscription_SubscriptionPolicy *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tev__CreatePullPointSubscription_SubscriptionPolicy);
	if (soap_out_PointerTo_tev__CreatePullPointSubscription_SubscriptionPolicy(soap, tag?tag:"tev:CreatePullPointSubscription-SubscriptionPolicy", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tev__CreatePullPointSubscription_SubscriptionPolicy ** SOAP_FMAC4 soap_get_PointerTo_tev__CreatePullPointSubscription_SubscriptionPolicy(struct soap *soap, struct _tev__CreatePullPointSubscription_SubscriptionPolicy **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tev__CreatePullPointSubscription_SubscriptionPolicy(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___depps__PullMessages(struct soap *soap, struct __depps__PullMessages *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->tev__PullMessages = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___depps__PullMessages(struct soap *soap, const struct __depps__PullMessages *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_tev__PullMessages(soap, &a->tev__PullMessages);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___depps__PullMessages(struct soap *soap, const char *tag, int id, const struct __depps__PullMessages *a, const char *type)
{
	if (soap_out_PointerTo_tev__PullMessages(soap, "tev:PullMessages", -1, &a->tev__PullMessages, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __depps__PullMessages * SOAP_FMAC4 soap_in___depps__PullMessages(struct soap *soap, const char *tag, struct __depps__PullMessages *a, const char *type)
{
	size_t soap_flag_tev__PullMessages = 1;
	short soap_flag;
	a = (struct __depps__PullMessages *)soap_id_enter(soap, "", a, SOAP_TYPE___depps__PullMessages, sizeof(struct __depps__PullMessages), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___depps__PullMessages(soap, a);
	for (soap_flag = 0;; soap_flag = 1)
	{	soap->error = SOAP_TAG_MISMATCH;
	if (soap_flag_tev__PullMessages && soap->error == SOAP_TAG_MISMATCH)
		if (soap_in_PointerTo_tev__PullMessages(soap, "tev:PullMessages", &a->tev__PullMessages, ""))
		{	soap_flag_tev__PullMessages--;
	continue;
	}
	if (soap->error == SOAP_TAG_MISMATCH)
		if (soap_flag)
		{	soap->error = SOAP_OK;
	break;
	}
	if (soap_flag && soap->error == SOAP_NO_TAG)
		break;
	if (soap->error)
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put___depps__PullMessages(struct soap *soap, const struct __depps__PullMessages *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___depps__PullMessages(soap, tag?tag:"-depps:PullMessages", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __depps__PullMessages * SOAP_FMAC4 soap_get___depps__PullMessages(struct soap *soap, struct __depps__PullMessages *p, const char *tag, const char *type)
{
	if ((p = soap_in___depps__PullMessages(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tev__PullMessages(struct soap *soap, struct _tev__PullMessages *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tev__PullMessages))
		soap_serialize__tev__PullMessages(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tev__PullMessages(struct soap *soap, const char *tag, int id, struct _tev__PullMessages *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tev__PullMessages);
	if (id < 0)
		return soap->error;
	return soap_out__tev__PullMessages(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tev__PullMessages ** SOAP_FMAC4 soap_in_PointerTo_tev__PullMessages(struct soap *soap, const char *tag, struct _tev__PullMessages **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tev__PullMessages **)soap_malloc(soap, sizeof(struct _tev__PullMessages *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tev__PullMessages(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tev__PullMessages **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tev__PullMessages, sizeof(struct _tev__PullMessages), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tev__PullMessages(struct soap *soap, struct _tev__PullMessages *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tev__PullMessages);
	if (soap_out_PointerTo_tev__PullMessages(soap, tag?tag:"tev:PullMessages", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tev__PullMessages ** SOAP_FMAC4 soap_get_PointerTo_tev__PullMessages(struct soap *soap, struct _tev__PullMessages **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tev__PullMessages(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}


SOAP_FMAC3 void SOAP_FMAC4 soap_default__tev__PullMessages(struct soap *soap, struct _tev__PullMessages *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_string(soap, &a->Timeout);
	soap_default_int(soap, &a->MessageLimit);
	a->__size = 0;
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tev__PullMessages(struct soap *soap, const struct _tev__PullMessages *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_string(soap, &a->Timeout);
	soap_embedded(soap, &a->MessageLimit, SOAP_TYPE_int);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tev__PullMessages(struct soap *soap, const char *tag, int id, const struct _tev__PullMessages *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tev__PullMessages), type))
		return soap->error;
	if (a->Timeout)
	{	if (soap_out_string(soap, "tev:Timeout", -1, &a->Timeout, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tev:Timeout"))
		return soap->error;
	if (soap_out_int(soap, "tev:MessageLimit", -1, &a->MessageLimit, ""))
		return soap->error;
	if (a->__any)
	{	int i;
	for (i = 0; i < a->__size; i++)
		soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tev__PullMessages * SOAP_FMAC4 soap_in__tev__PullMessages(struct soap *soap, const char *tag, struct _tev__PullMessages *a, const char *type)
{
	size_t soap_flag_Timeout = 1;
	size_t soap_flag_MessageLimit = 1;
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tev__PullMessages *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tev__PullMessages, sizeof(struct _tev__PullMessages), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tev__PullMessages(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_Timeout && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_string(soap, "tev:Timeout", &a->Timeout, "xsd:string"))
			{	soap_flag_Timeout--;
		continue;
		}
		if (soap_flag_MessageLimit && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_int(soap, "tev:MessageLimit", &a->MessageLimit, "xsd:int"))
			{	soap_flag_MessageLimit--;
		continue;
		}
		if (soap->error == SOAP_TAG_MISMATCH && !soap_peek_element(soap))
		{	if (a->__any == NULL)
		{	if (soap_blist___any == NULL)
		soap_blist___any = soap_new_block(soap);
		a->__any = (char **)soap_push_block(soap, soap_blist___any, sizeof(char *));
		if (a->__any == NULL)
			return NULL;
		*a->__any = NULL;
		}
		if (soap_inliteral(soap, "-any", a->__any))
		{	a->__size++;
		a->__any = NULL;
		continue;
		}
		}
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (a->__any)
			soap_pop_block(soap, soap_blist___any);
		if (a->__size)
			a->__any = (char **)soap_save_block(soap, soap_blist___any, NULL, 1);
		else
		{	a->__any = NULL;
		if (soap_blist___any)
			soap_end_block(soap, soap_blist___any);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _tev__PullMessages *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tev__PullMessages, 0, sizeof(struct _tev__PullMessages), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_Timeout > 0 || soap_flag_MessageLimit > 0))
	{	soap->error = SOAP_OCCURS;
	return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tev__PullMessages(struct soap *soap, const struct _tev__PullMessages *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tev__PullMessages);
	if (soap_out__tev__PullMessages(soap, tag?tag:"tev:PullMessages", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tev__PullMessages * SOAP_FMAC4 soap_get__tev__PullMessages(struct soap *soap, struct _tev__PullMessages *p, const char *tag, const char *type)
{
	if ((p = soap_in__tev__PullMessages(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tev__PullMessagesResponse(struct soap *soap, struct _tev__PullMessagesResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_time(soap, &a->CurrentTime);
	soap_default_time(soap, &a->TerminationTime);
	a->__sizeNotificationMessage = 0;
	a->wsnt__NotificationMessage = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tev__PullMessagesResponse(struct soap *soap, const struct _tev__PullMessagesResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_embedded(soap, &a->CurrentTime, SOAP_TYPE_time);
	soap_embedded(soap, &a->TerminationTime, SOAP_TYPE_time);
	if (a->wsnt__NotificationMessage)
	{	int i;
	for (i = 0; i < a->__sizeNotificationMessage; i++)
	{
		soap_embedded(soap, a->wsnt__NotificationMessage + i, SOAP_TYPE_wsnt__NotificationMessageHolderType);
		soap_serialize_wsnt__NotificationMessageHolderType(soap, a->wsnt__NotificationMessage + i);
	}
	}
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tev__PullMessagesResponse(struct soap *soap, const char *tag, int id, const struct _tev__PullMessagesResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tev__PullMessagesResponse), type))
		return soap->error;
	soap_element_result(soap, "tev:CurrentTime");
	if (soap_out_time(soap, "tev:CurrentTime", -1, &a->CurrentTime, ""))
		return soap->error;
	if (soap_out_time(soap, "tev:TerminationTime", -1, &a->TerminationTime, ""))
		return soap->error;
	if (a->wsnt__NotificationMessage)
	{	int i;
	for (i = 0; i < a->__sizeNotificationMessage; i++)
		if (soap_out_wsnt__NotificationMessageHolderType(soap, "wsnt:NotificationMessage", -1, a->wsnt__NotificationMessage + i, ""))
			return soap->error;
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tev__PullMessagesResponse * SOAP_FMAC4 soap_in__tev__PullMessagesResponse(struct soap *soap, const char *tag, struct _tev__PullMessagesResponse *a, const char *type)
{
	size_t soap_flag_CurrentTime = 1;
	size_t soap_flag_TerminationTime = 1;
	struct soap_blist *soap_blist_wsnt__NotificationMessage = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tev__PullMessagesResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tev__PullMessagesResponse, sizeof(struct _tev__PullMessagesResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tev__PullMessagesResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_CurrentTime && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_time(soap, "tev:CurrentTime", &a->CurrentTime, "xsd:dateTime"))
			{	soap_flag_CurrentTime--;
		continue;
		}
		if (soap_flag_TerminationTime && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_time(soap, "tev:TerminationTime", &a->TerminationTime, "xsd:dateTime"))
			{	soap_flag_TerminationTime--;
		continue;
		}
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "wsnt:NotificationMessage", 1, NULL))
		{	if (a->wsnt__NotificationMessage == NULL)
		{	if (soap_blist_wsnt__NotificationMessage == NULL)
		soap_blist_wsnt__NotificationMessage = soap_new_block(soap);
		a->wsnt__NotificationMessage = (struct wsnt__NotificationMessageHolderType *)soap_push_block(soap, soap_blist_wsnt__NotificationMessage, sizeof(struct wsnt__NotificationMessageHolderType));
		if (a->wsnt__NotificationMessage == NULL)
			return NULL;
		soap_default_wsnt__NotificationMessageHolderType(soap, a->wsnt__NotificationMessage);
		}
		soap_revert(soap);
		if (soap_in_wsnt__NotificationMessageHolderType(soap, "wsnt:NotificationMessage", a->wsnt__NotificationMessage, "wsnt:NotificationMessageHolderType"))
		{	a->__sizeNotificationMessage++;
		a->wsnt__NotificationMessage = NULL;
		continue;
		}
		}
		soap_check_result(soap, "tev:CurrentTime");
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (a->wsnt__NotificationMessage)
			soap_pop_block(soap, soap_blist_wsnt__NotificationMessage);
		if (a->__sizeNotificationMessage)
			a->wsnt__NotificationMessage = (struct wsnt__NotificationMessageHolderType *)soap_save_block(soap, soap_blist_wsnt__NotificationMessage, NULL, 1);
		else
		{	a->wsnt__NotificationMessage = NULL;
		if (soap_blist_wsnt__NotificationMessage)
			soap_end_block(soap, soap_blist_wsnt__NotificationMessage);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _tev__PullMessagesResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tev__PullMessagesResponse, 0, sizeof(struct _tev__PullMessagesResponse), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_CurrentTime > 0 || soap_flag_TerminationTime > 0))
	{	soap->error = SOAP_OCCURS;
	return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tev__PullMessagesResponse(struct soap *soap, const struct _tev__PullMessagesResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tev__PullMessagesResponse);
	if (soap_out__tev__PullMessagesResponse(soap, tag?tag:"tev:PullMessagesResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tev__PullMessagesResponse * SOAP_FMAC4 soap_get__tev__PullMessagesResponse(struct soap *soap, struct _tev__PullMessagesResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tev__PullMessagesResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___depps__SetSynchronizationPoint(struct soap *soap, struct __depps__SetSynchronizationPoint *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->tev__SetSynchronizationPoint = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___depps__SetSynchronizationPoint(struct soap *soap, const struct __depps__SetSynchronizationPoint *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_tev__SetSynchronizationPoint(soap, &a->tev__SetSynchronizationPoint);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___depps__SetSynchronizationPoint(struct soap *soap, const char *tag, int id, const struct __depps__SetSynchronizationPoint *a, const char *type)
{
	if (soap_out_PointerTo_tev__SetSynchronizationPoint(soap, "tev:SetSynchronizationPoint", -1, &a->tev__SetSynchronizationPoint, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __depps__SetSynchronizationPoint * SOAP_FMAC4 soap_in___depps__SetSynchronizationPoint(struct soap *soap, const char *tag, struct __depps__SetSynchronizationPoint *a, const char *type)
{
	size_t soap_flag_tev__SetSynchronizationPoint = 1;
	short soap_flag;
	a = (struct __depps__SetSynchronizationPoint *)soap_id_enter(soap, "", a, SOAP_TYPE___depps__SetSynchronizationPoint, sizeof(struct __depps__SetSynchronizationPoint), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___depps__SetSynchronizationPoint(soap, a);
	for (soap_flag = 0;; soap_flag = 1)
	{	soap->error = SOAP_TAG_MISMATCH;
	if (soap_flag_tev__SetSynchronizationPoint && soap->error == SOAP_TAG_MISMATCH)
		if (soap_in_PointerTo_tev__SetSynchronizationPoint(soap, "tev:SetSynchronizationPoint", &a->tev__SetSynchronizationPoint, ""))
		{	soap_flag_tev__SetSynchronizationPoint--;
	continue;
	}
	if (soap->error == SOAP_TAG_MISMATCH)
		if (soap_flag)
		{	soap->error = SOAP_OK;
	break;
	}
	if (soap_flag && soap->error == SOAP_NO_TAG)
		break;
	if (soap->error)
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put___depps__SetSynchronizationPoint(struct soap *soap, const struct __depps__SetSynchronizationPoint *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___depps__SetSynchronizationPoint(soap, tag?tag:"-depps:SetSynchronizationPoint", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __depps__SetSynchronizationPoint * SOAP_FMAC4 soap_get___depps__SetSynchronizationPoint(struct soap *soap, struct __depps__SetSynchronizationPoint *p, const char *tag, const char *type)
{
	if ((p = soap_in___depps__SetSynchronizationPoint(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tev__SetSynchronizationPoint(struct soap *soap, struct _tev__SetSynchronizationPoint *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tev__SetSynchronizationPoint))
		soap_serialize__tev__SetSynchronizationPoint(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tev__SetSynchronizationPoint(struct soap *soap, const char *tag, int id, struct _tev__SetSynchronizationPoint *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tev__SetSynchronizationPoint);
	if (id < 0)
		return soap->error;
	return soap_out__tev__SetSynchronizationPoint(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tev__SetSynchronizationPoint ** SOAP_FMAC4 soap_in_PointerTo_tev__SetSynchronizationPoint(struct soap *soap, const char *tag, struct _tev__SetSynchronizationPoint **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tev__SetSynchronizationPoint **)soap_malloc(soap, sizeof(struct _tev__SetSynchronizationPoint *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tev__SetSynchronizationPoint(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tev__SetSynchronizationPoint **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tev__SetSynchronizationPoint, sizeof(struct _tev__SetSynchronizationPoint), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tev__SetSynchronizationPoint(struct soap *soap, struct _tev__SetSynchronizationPoint *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tev__SetSynchronizationPoint);
	if (soap_out_PointerTo_tev__SetSynchronizationPoint(soap, tag?tag:"tev:SetSynchronizationPoint", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tev__SetSynchronizationPoint ** SOAP_FMAC4 soap_get_PointerTo_tev__SetSynchronizationPoint(struct soap *soap, struct _tev__SetSynchronizationPoint **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tev__SetSynchronizationPoint(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tev__SetSynchronizationPoint(struct soap *soap, struct _tev__SetSynchronizationPoint *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tev__SetSynchronizationPoint(struct soap *soap, const struct _tev__SetSynchronizationPoint *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tev__SetSynchronizationPoint(struct soap *soap, const char *tag, int id, const struct _tev__SetSynchronizationPoint *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tev__SetSynchronizationPoint), type))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tev__SetSynchronizationPoint * SOAP_FMAC4 soap_in__tev__SetSynchronizationPoint(struct soap *soap, const char *tag, struct _tev__SetSynchronizationPoint *a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tev__SetSynchronizationPoint *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tev__SetSynchronizationPoint, sizeof(struct _tev__SetSynchronizationPoint), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tev__SetSynchronizationPoint(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _tev__SetSynchronizationPoint *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tev__SetSynchronizationPoint, 0, sizeof(struct _tev__SetSynchronizationPoint), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tev__SetSynchronizationPoint(struct soap *soap, const struct _tev__SetSynchronizationPoint *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tev__SetSynchronizationPoint);
	if (soap_out__tev__SetSynchronizationPoint(soap, tag?tag:"tev:SetSynchronizationPoint", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tev__SetSynchronizationPoint * SOAP_FMAC4 soap_get__tev__SetSynchronizationPoint(struct soap *soap, struct _tev__SetSynchronizationPoint *p, const char *tag, const char *type)
{
	if ((p = soap_in__tev__SetSynchronizationPoint(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tev__SetSynchronizationPointResponse(struct soap *soap, struct _tev__SetSynchronizationPointResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tev__SetSynchronizationPointResponse(struct soap *soap, const struct _tev__SetSynchronizationPointResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tev__SetSynchronizationPointResponse(struct soap *soap, const char *tag, int id, const struct _tev__SetSynchronizationPointResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tev__SetSynchronizationPointResponse), type))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tev__SetSynchronizationPointResponse * SOAP_FMAC4 soap_in__tev__SetSynchronizationPointResponse(struct soap *soap, const char *tag, struct _tev__SetSynchronizationPointResponse *a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tev__SetSynchronizationPointResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tev__SetSynchronizationPointResponse, sizeof(struct _tev__SetSynchronizationPointResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tev__SetSynchronizationPointResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _tev__SetSynchronizationPointResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tev__SetSynchronizationPointResponse, 0, sizeof(struct _tev__SetSynchronizationPointResponse), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tev__SetSynchronizationPointResponse(struct soap *soap, const struct _tev__SetSynchronizationPointResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tev__SetSynchronizationPointResponse);
	if (soap_out__tev__SetSynchronizationPointResponse(soap, tag?tag:"tev:SetSynchronizationPointResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tev__SetSynchronizationPointResponse * SOAP_FMAC4 soap_get__tev__SetSynchronizationPointResponse(struct soap *soap, struct _tev__SetSynchronizationPointResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tev__SetSynchronizationPointResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}





///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////



SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__ModifyRulesResponse(struct soap *soap, struct _tan__ModifyRulesResponse *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tan__ModifyRulesResponse))
		soap_serialize__tan__ModifyRulesResponse(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__ModifyRulesResponse(struct soap *soap, const char *tag, int id, struct _tan__ModifyRulesResponse *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tan__ModifyRulesResponse);
	if (id < 0)
		return soap->error;
	return soap_out__tan__ModifyRulesResponse(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tan__ModifyRulesResponse ** SOAP_FMAC4 soap_in_PointerTo_tan__ModifyRulesResponse(struct soap *soap, const char *tag, struct _tan__ModifyRulesResponse **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tan__ModifyRulesResponse **)soap_malloc(soap, sizeof(struct _tan__ModifyRulesResponse *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tan__ModifyRulesResponse(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tan__ModifyRulesResponse **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tan__ModifyRulesResponse, sizeof(struct _tan__ModifyRulesResponse), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__ModifyRulesResponse(struct soap *soap, struct _tan__ModifyRulesResponse *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tan__ModifyRulesResponse);
	if (soap_out_PointerTo_tan__ModifyRulesResponse(soap, tag?tag:"tan:ModifyRulesResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__ModifyRulesResponse ** SOAP_FMAC4 soap_get_PointerTo_tan__ModifyRulesResponse(struct soap *soap, struct _tan__ModifyRulesResponse **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tan__ModifyRulesResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__ModifyRules(struct soap *soap, struct _tan__ModifyRules *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tan__ModifyRules))
		soap_serialize__tan__ModifyRules(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__ModifyRules(struct soap *soap, const char *tag, int id, struct _tan__ModifyRules *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tan__ModifyRules);
	if (id < 0)
		return soap->error;
	return soap_out__tan__ModifyRules(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tan__ModifyRules ** SOAP_FMAC4 soap_in_PointerTo_tan__ModifyRules(struct soap *soap, const char *tag, struct _tan__ModifyRules **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tan__ModifyRules **)soap_malloc(soap, sizeof(struct _tan__ModifyRules *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tan__ModifyRules(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tan__ModifyRules **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tan__ModifyRules, sizeof(struct _tan__ModifyRules), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__ModifyRules(struct soap *soap, struct _tan__ModifyRules *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tan__ModifyRules);
	if (soap_out_PointerTo_tan__ModifyRules(soap, tag?tag:"tan:ModifyRules", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__ModifyRules ** SOAP_FMAC4 soap_get_PointerTo_tan__ModifyRules(struct soap *soap, struct _tan__ModifyRules **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tan__ModifyRules(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__GetRulesResponse(struct soap *soap, struct _tan__GetRulesResponse *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tan__GetRulesResponse))
		soap_serialize__tan__GetRulesResponse(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__GetRulesResponse(struct soap *soap, const char *tag, int id, struct _tan__GetRulesResponse *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tan__GetRulesResponse);
	if (id < 0)
		return soap->error;
	return soap_out__tan__GetRulesResponse(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tan__GetRulesResponse ** SOAP_FMAC4 soap_in_PointerTo_tan__GetRulesResponse(struct soap *soap, const char *tag, struct _tan__GetRulesResponse **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tan__GetRulesResponse **)soap_malloc(soap, sizeof(struct _tan__GetRulesResponse *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tan__GetRulesResponse(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tan__GetRulesResponse **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tan__GetRulesResponse, sizeof(struct _tan__GetRulesResponse), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__GetRulesResponse(struct soap *soap, struct _tan__GetRulesResponse *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tan__GetRulesResponse);
	if (soap_out_PointerTo_tan__GetRulesResponse(soap, tag?tag:"tan:GetRulesResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__GetRulesResponse ** SOAP_FMAC4 soap_get_PointerTo_tan__GetRulesResponse(struct soap *soap, struct _tan__GetRulesResponse **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tan__GetRulesResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__GetRules(struct soap *soap, struct _tan__GetRules *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tan__GetRules))
		soap_serialize__tan__GetRules(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__GetRules(struct soap *soap, const char *tag, int id, struct _tan__GetRules *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tan__GetRules);
	if (id < 0)
		return soap->error;
	return soap_out__tan__GetRules(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tan__GetRules ** SOAP_FMAC4 soap_in_PointerTo_tan__GetRules(struct soap *soap, const char *tag, struct _tan__GetRules **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tan__GetRules **)soap_malloc(soap, sizeof(struct _tan__GetRules *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tan__GetRules(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tan__GetRules **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tan__GetRules, sizeof(struct _tan__GetRules), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__GetRules(struct soap *soap, struct _tan__GetRules *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tan__GetRules);
	if (soap_out_PointerTo_tan__GetRules(soap, tag?tag:"tan:GetRules", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__GetRules ** SOAP_FMAC4 soap_get_PointerTo_tan__GetRules(struct soap *soap, struct _tan__GetRules **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tan__GetRules(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__DeleteRulesResponse(struct soap *soap, struct _tan__DeleteRulesResponse *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tan__DeleteRulesResponse))
		soap_serialize__tan__DeleteRulesResponse(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__DeleteRulesResponse(struct soap *soap, const char *tag, int id, struct _tan__DeleteRulesResponse *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tan__DeleteRulesResponse);
	if (id < 0)
		return soap->error;
	return soap_out__tan__DeleteRulesResponse(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tan__DeleteRulesResponse ** SOAP_FMAC4 soap_in_PointerTo_tan__DeleteRulesResponse(struct soap *soap, const char *tag, struct _tan__DeleteRulesResponse **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tan__DeleteRulesResponse **)soap_malloc(soap, sizeof(struct _tan__DeleteRulesResponse *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tan__DeleteRulesResponse(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tan__DeleteRulesResponse **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tan__DeleteRulesResponse, sizeof(struct _tan__DeleteRulesResponse), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__DeleteRulesResponse(struct soap *soap, struct _tan__DeleteRulesResponse *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tan__DeleteRulesResponse);
	if (soap_out_PointerTo_tan__DeleteRulesResponse(soap, tag?tag:"tan:DeleteRulesResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__DeleteRulesResponse ** SOAP_FMAC4 soap_get_PointerTo_tan__DeleteRulesResponse(struct soap *soap, struct _tan__DeleteRulesResponse **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tan__DeleteRulesResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__DeleteRules(struct soap *soap, struct _tan__DeleteRules *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tan__DeleteRules))
		soap_serialize__tan__DeleteRules(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__DeleteRules(struct soap *soap, const char *tag, int id, struct _tan__DeleteRules *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tan__DeleteRules);
	if (id < 0)
		return soap->error;
	return soap_out__tan__DeleteRules(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tan__DeleteRules ** SOAP_FMAC4 soap_in_PointerTo_tan__DeleteRules(struct soap *soap, const char *tag, struct _tan__DeleteRules **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tan__DeleteRules **)soap_malloc(soap, sizeof(struct _tan__DeleteRules *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tan__DeleteRules(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tan__DeleteRules **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tan__DeleteRules, sizeof(struct _tan__DeleteRules), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__DeleteRules(struct soap *soap, struct _tan__DeleteRules *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tan__DeleteRules);
	if (soap_out_PointerTo_tan__DeleteRules(soap, tag?tag:"tan:DeleteRules", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__DeleteRules ** SOAP_FMAC4 soap_get_PointerTo_tan__DeleteRules(struct soap *soap, struct _tan__DeleteRules **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tan__DeleteRules(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__CreateRulesResponse(struct soap *soap, struct _tan__CreateRulesResponse *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tan__CreateRulesResponse))
		soap_serialize__tan__CreateRulesResponse(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__CreateRulesResponse(struct soap *soap, const char *tag, int id, struct _tan__CreateRulesResponse *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tan__CreateRulesResponse);
	if (id < 0)
		return soap->error;
	return soap_out__tan__CreateRulesResponse(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tan__CreateRulesResponse ** SOAP_FMAC4 soap_in_PointerTo_tan__CreateRulesResponse(struct soap *soap, const char *tag, struct _tan__CreateRulesResponse **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tan__CreateRulesResponse **)soap_malloc(soap, sizeof(struct _tan__CreateRulesResponse *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tan__CreateRulesResponse(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tan__CreateRulesResponse **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tan__CreateRulesResponse, sizeof(struct _tan__CreateRulesResponse), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__CreateRulesResponse(struct soap *soap, struct _tan__CreateRulesResponse *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tan__CreateRulesResponse);
	if (soap_out_PointerTo_tan__CreateRulesResponse(soap, tag?tag:"tan:CreateRulesResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__CreateRulesResponse ** SOAP_FMAC4 soap_get_PointerTo_tan__CreateRulesResponse(struct soap *soap, struct _tan__CreateRulesResponse **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tan__CreateRulesResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__CreateRules(struct soap *soap, struct _tan__CreateRules *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tan__CreateRules))
		soap_serialize__tan__CreateRules(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__CreateRules(struct soap *soap, const char *tag, int id, struct _tan__CreateRules *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tan__CreateRules);
	if (id < 0)
		return soap->error;
	return soap_out__tan__CreateRules(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tan__CreateRules ** SOAP_FMAC4 soap_in_PointerTo_tan__CreateRules(struct soap *soap, const char *tag, struct _tan__CreateRules **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tan__CreateRules **)soap_malloc(soap, sizeof(struct _tan__CreateRules *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tan__CreateRules(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tan__CreateRules **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tan__CreateRules, sizeof(struct _tan__CreateRules), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__CreateRules(struct soap *soap, struct _tan__CreateRules *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tan__CreateRules);
	if (soap_out_PointerTo_tan__CreateRules(soap, tag?tag:"tan:CreateRules", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__CreateRules ** SOAP_FMAC4 soap_get_PointerTo_tan__CreateRules(struct soap *soap, struct _tan__CreateRules **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tan__CreateRules(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}


SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__ModifyAnalyticsModulesResponse(struct soap *soap, struct _tan__ModifyAnalyticsModulesResponse *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tan__ModifyAnalyticsModulesResponse))
		soap_serialize__tan__ModifyAnalyticsModulesResponse(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__ModifyAnalyticsModulesResponse(struct soap *soap, const char *tag, int id, struct _tan__ModifyAnalyticsModulesResponse *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tan__ModifyAnalyticsModulesResponse);
	if (id < 0)
		return soap->error;
	return soap_out__tan__ModifyAnalyticsModulesResponse(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tan__ModifyAnalyticsModulesResponse ** SOAP_FMAC4 soap_in_PointerTo_tan__ModifyAnalyticsModulesResponse(struct soap *soap, const char *tag, struct _tan__ModifyAnalyticsModulesResponse **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tan__ModifyAnalyticsModulesResponse **)soap_malloc(soap, sizeof(struct _tan__ModifyAnalyticsModulesResponse *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tan__ModifyAnalyticsModulesResponse(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tan__ModifyAnalyticsModulesResponse **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tan__ModifyAnalyticsModulesResponse, sizeof(struct _tan__ModifyAnalyticsModulesResponse), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__ModifyAnalyticsModulesResponse(struct soap *soap, struct _tan__ModifyAnalyticsModulesResponse *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tan__ModifyAnalyticsModulesResponse);
	if (soap_out_PointerTo_tan__ModifyAnalyticsModulesResponse(soap, tag?tag:"tan:ModifyAnalyticsModulesResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__ModifyAnalyticsModulesResponse ** SOAP_FMAC4 soap_get_PointerTo_tan__ModifyAnalyticsModulesResponse(struct soap *soap, struct _tan__ModifyAnalyticsModulesResponse **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tan__ModifyAnalyticsModulesResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__ModifyAnalyticsModules(struct soap *soap, struct _tan__ModifyAnalyticsModules *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tan__ModifyAnalyticsModules))
		soap_serialize__tan__ModifyAnalyticsModules(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__ModifyAnalyticsModules(struct soap *soap, const char *tag, int id, struct _tan__ModifyAnalyticsModules *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tan__ModifyAnalyticsModules);
	if (id < 0)
		return soap->error;
	return soap_out__tan__ModifyAnalyticsModules(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tan__ModifyAnalyticsModules ** SOAP_FMAC4 soap_in_PointerTo_tan__ModifyAnalyticsModules(struct soap *soap, const char *tag, struct _tan__ModifyAnalyticsModules **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tan__ModifyAnalyticsModules **)soap_malloc(soap, sizeof(struct _tan__ModifyAnalyticsModules *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tan__ModifyAnalyticsModules(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tan__ModifyAnalyticsModules **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tan__ModifyAnalyticsModules, sizeof(struct _tan__ModifyAnalyticsModules), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__ModifyAnalyticsModules(struct soap *soap, struct _tan__ModifyAnalyticsModules *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tan__ModifyAnalyticsModules);
	if (soap_out_PointerTo_tan__ModifyAnalyticsModules(soap, tag?tag:"tan:ModifyAnalyticsModules", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__ModifyAnalyticsModules ** SOAP_FMAC4 soap_get_PointerTo_tan__ModifyAnalyticsModules(struct soap *soap, struct _tan__ModifyAnalyticsModules **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tan__ModifyAnalyticsModules(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__GetAnalyticsModulesResponse(struct soap *soap, struct _tan__GetAnalyticsModulesResponse *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tan__GetAnalyticsModulesResponse))
		soap_serialize__tan__GetAnalyticsModulesResponse(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__GetAnalyticsModulesResponse(struct soap *soap, const char *tag, int id, struct _tan__GetAnalyticsModulesResponse *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tan__GetAnalyticsModulesResponse);
	if (id < 0)
		return soap->error;
	return soap_out__tan__GetAnalyticsModulesResponse(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tan__GetAnalyticsModulesResponse ** SOAP_FMAC4 soap_in_PointerTo_tan__GetAnalyticsModulesResponse(struct soap *soap, const char *tag, struct _tan__GetAnalyticsModulesResponse **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tan__GetAnalyticsModulesResponse **)soap_malloc(soap, sizeof(struct _tan__GetAnalyticsModulesResponse *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tan__GetAnalyticsModulesResponse(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tan__GetAnalyticsModulesResponse **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tan__GetAnalyticsModulesResponse, sizeof(struct _tan__GetAnalyticsModulesResponse), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__GetAnalyticsModulesResponse(struct soap *soap, struct _tan__GetAnalyticsModulesResponse *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tan__GetAnalyticsModulesResponse);
	if (soap_out_PointerTo_tan__GetAnalyticsModulesResponse(soap, tag?tag:"tan:GetAnalyticsModulesResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__GetAnalyticsModulesResponse ** SOAP_FMAC4 soap_get_PointerTo_tan__GetAnalyticsModulesResponse(struct soap *soap, struct _tan__GetAnalyticsModulesResponse **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tan__GetAnalyticsModulesResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__GetAnalyticsModules(struct soap *soap, struct _tan__GetAnalyticsModules *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tan__GetAnalyticsModules))
		soap_serialize__tan__GetAnalyticsModules(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__GetAnalyticsModules(struct soap *soap, const char *tag, int id, struct _tan__GetAnalyticsModules *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tan__GetAnalyticsModules);
	if (id < 0)
		return soap->error;
	return soap_out__tan__GetAnalyticsModules(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tan__GetAnalyticsModules ** SOAP_FMAC4 soap_in_PointerTo_tan__GetAnalyticsModules(struct soap *soap, const char *tag, struct _tan__GetAnalyticsModules **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tan__GetAnalyticsModules **)soap_malloc(soap, sizeof(struct _tan__GetAnalyticsModules *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tan__GetAnalyticsModules(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tan__GetAnalyticsModules **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tan__GetAnalyticsModules, sizeof(struct _tan__GetAnalyticsModules), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__GetAnalyticsModules(struct soap *soap, struct _tan__GetAnalyticsModules *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tan__GetAnalyticsModules);
	if (soap_out_PointerTo_tan__GetAnalyticsModules(soap, tag?tag:"tan:GetAnalyticsModules", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__GetAnalyticsModules ** SOAP_FMAC4 soap_get_PointerTo_tan__GetAnalyticsModules(struct soap *soap, struct _tan__GetAnalyticsModules **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tan__GetAnalyticsModules(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__DeleteAnalyticsModulesResponse(struct soap *soap, struct _tan__DeleteAnalyticsModulesResponse *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tan__DeleteAnalyticsModulesResponse))
		soap_serialize__tan__DeleteAnalyticsModulesResponse(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__DeleteAnalyticsModulesResponse(struct soap *soap, const char *tag, int id, struct _tan__DeleteAnalyticsModulesResponse *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tan__DeleteAnalyticsModulesResponse);
	if (id < 0)
		return soap->error;
	return soap_out__tan__DeleteAnalyticsModulesResponse(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tan__DeleteAnalyticsModulesResponse ** SOAP_FMAC4 soap_in_PointerTo_tan__DeleteAnalyticsModulesResponse(struct soap *soap, const char *tag, struct _tan__DeleteAnalyticsModulesResponse **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tan__DeleteAnalyticsModulesResponse **)soap_malloc(soap, sizeof(struct _tan__DeleteAnalyticsModulesResponse *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tan__DeleteAnalyticsModulesResponse(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tan__DeleteAnalyticsModulesResponse **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tan__DeleteAnalyticsModulesResponse, sizeof(struct _tan__DeleteAnalyticsModulesResponse), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__DeleteAnalyticsModulesResponse(struct soap *soap, struct _tan__DeleteAnalyticsModulesResponse *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tan__DeleteAnalyticsModulesResponse);
	if (soap_out_PointerTo_tan__DeleteAnalyticsModulesResponse(soap, tag?tag:"tan:DeleteAnalyticsModulesResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__DeleteAnalyticsModulesResponse ** SOAP_FMAC4 soap_get_PointerTo_tan__DeleteAnalyticsModulesResponse(struct soap *soap, struct _tan__DeleteAnalyticsModulesResponse **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tan__DeleteAnalyticsModulesResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__DeleteAnalyticsModules(struct soap *soap, struct _tan__DeleteAnalyticsModules *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tan__DeleteAnalyticsModules))
		soap_serialize__tan__DeleteAnalyticsModules(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__DeleteAnalyticsModules(struct soap *soap, const char *tag, int id, struct _tan__DeleteAnalyticsModules *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tan__DeleteAnalyticsModules);
	if (id < 0)
		return soap->error;
	return soap_out__tan__DeleteAnalyticsModules(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tan__DeleteAnalyticsModules ** SOAP_FMAC4 soap_in_PointerTo_tan__DeleteAnalyticsModules(struct soap *soap, const char *tag, struct _tan__DeleteAnalyticsModules **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tan__DeleteAnalyticsModules **)soap_malloc(soap, sizeof(struct _tan__DeleteAnalyticsModules *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tan__DeleteAnalyticsModules(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tan__DeleteAnalyticsModules **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tan__DeleteAnalyticsModules, sizeof(struct _tan__DeleteAnalyticsModules), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__DeleteAnalyticsModules(struct soap *soap, struct _tan__DeleteAnalyticsModules *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tan__DeleteAnalyticsModules);
	if (soap_out_PointerTo_tan__DeleteAnalyticsModules(soap, tag?tag:"tan:DeleteAnalyticsModules", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__DeleteAnalyticsModules ** SOAP_FMAC4 soap_get_PointerTo_tan__DeleteAnalyticsModules(struct soap *soap, struct _tan__DeleteAnalyticsModules **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tan__DeleteAnalyticsModules(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__CreateAnalyticsModulesResponse(struct soap *soap, struct _tan__CreateAnalyticsModulesResponse *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tan__CreateAnalyticsModulesResponse))
		soap_serialize__tan__CreateAnalyticsModulesResponse(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__CreateAnalyticsModulesResponse(struct soap *soap, const char *tag, int id, struct _tan__CreateAnalyticsModulesResponse *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tan__CreateAnalyticsModulesResponse);
	if (id < 0)
		return soap->error;
	return soap_out__tan__CreateAnalyticsModulesResponse(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tan__CreateAnalyticsModulesResponse ** SOAP_FMAC4 soap_in_PointerTo_tan__CreateAnalyticsModulesResponse(struct soap *soap, const char *tag, struct _tan__CreateAnalyticsModulesResponse **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tan__CreateAnalyticsModulesResponse **)soap_malloc(soap, sizeof(struct _tan__CreateAnalyticsModulesResponse *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tan__CreateAnalyticsModulesResponse(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tan__CreateAnalyticsModulesResponse **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tan__CreateAnalyticsModulesResponse, sizeof(struct _tan__CreateAnalyticsModulesResponse), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__CreateAnalyticsModulesResponse(struct soap *soap, struct _tan__CreateAnalyticsModulesResponse *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tan__CreateAnalyticsModulesResponse);
	if (soap_out_PointerTo_tan__CreateAnalyticsModulesResponse(soap, tag?tag:"tan:CreateAnalyticsModulesResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__CreateAnalyticsModulesResponse ** SOAP_FMAC4 soap_get_PointerTo_tan__CreateAnalyticsModulesResponse(struct soap *soap, struct _tan__CreateAnalyticsModulesResponse **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tan__CreateAnalyticsModulesResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__CreateAnalyticsModules(struct soap *soap, struct _tan__CreateAnalyticsModules *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tan__CreateAnalyticsModules))
		soap_serialize__tan__CreateAnalyticsModules(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__CreateAnalyticsModules(struct soap *soap, const char *tag, int id, struct _tan__CreateAnalyticsModules *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tan__CreateAnalyticsModules);
	if (id < 0)
		return soap->error;
	return soap_out__tan__CreateAnalyticsModules(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tan__CreateAnalyticsModules ** SOAP_FMAC4 soap_in_PointerTo_tan__CreateAnalyticsModules(struct soap *soap, const char *tag, struct _tan__CreateAnalyticsModules **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tan__CreateAnalyticsModules **)soap_malloc(soap, sizeof(struct _tan__CreateAnalyticsModules *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tan__CreateAnalyticsModules(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tan__CreateAnalyticsModules **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tan__CreateAnalyticsModules, sizeof(struct _tan__CreateAnalyticsModules), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__CreateAnalyticsModules(struct soap *soap, struct _tan__CreateAnalyticsModules *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tan__CreateAnalyticsModules);
	if (soap_out_PointerTo_tan__CreateAnalyticsModules(soap, tag?tag:"tan:CreateAnalyticsModules", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__CreateAnalyticsModules ** SOAP_FMAC4 soap_get_PointerTo_tan__CreateAnalyticsModules(struct soap *soap, struct _tan__CreateAnalyticsModules **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tan__CreateAnalyticsModules(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////




SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__GetAnalyticsModulesResponse(struct soap *soap, struct _tan__GetAnalyticsModulesResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__sizeAnalyticsModule = 0;
	a->AnalyticsModule = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__GetAnalyticsModulesResponse(struct soap *soap, const struct _tan__GetAnalyticsModulesResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	if (a->AnalyticsModule)
	{	int i;
	for (i = 0; i < a->__sizeAnalyticsModule; i++)
	{
		soap_embedded(soap, a->AnalyticsModule + i, SOAP_TYPE_tt__Config);
		soap_serialize_tt__Config(soap, a->AnalyticsModule + i);
	}
	}
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__GetAnalyticsModulesResponse(struct soap *soap, const char *tag, int id, const struct _tan__GetAnalyticsModulesResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tan__GetAnalyticsModulesResponse), type))
		return soap->error;
	soap_element_result(soap, "-sizeAnalyticsModule");
	if (a->AnalyticsModule)
	{	int i;
	for (i = 0; i < a->__sizeAnalyticsModule; i++)
		if (soap_out_tt__Config(soap, "tan:AnalyticsModule", -1, a->AnalyticsModule + i, ""))
			return soap->error;
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tan__GetAnalyticsModulesResponse * SOAP_FMAC4 soap_in__tan__GetAnalyticsModulesResponse(struct soap *soap, const char *tag, struct _tan__GetAnalyticsModulesResponse *a, const char *type)
{
	struct soap_blist *soap_blist_AnalyticsModule = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tan__GetAnalyticsModulesResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tan__GetAnalyticsModulesResponse, sizeof(struct _tan__GetAnalyticsModulesResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tan__GetAnalyticsModulesResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tan:AnalyticsModule", 1, NULL))
		{	if (a->AnalyticsModule == NULL)
		{	if (soap_blist_AnalyticsModule == NULL)
		soap_blist_AnalyticsModule = soap_new_block(soap);
		a->AnalyticsModule = (struct tt__Config *)soap_push_block(soap, soap_blist_AnalyticsModule, sizeof(struct tt__Config));
		if (a->AnalyticsModule == NULL)
			return NULL;
		soap_default_tt__Config(soap, a->AnalyticsModule);
		}
		soap_revert(soap);
		if (soap_in_tt__Config(soap, "tan:AnalyticsModule", a->AnalyticsModule, "tt:Config"))
		{	a->__sizeAnalyticsModule++;
		a->AnalyticsModule = NULL;
		continue;
		}
		}
		soap_check_result(soap, "-sizeAnalyticsModule");
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (a->AnalyticsModule)
			soap_pop_block(soap, soap_blist_AnalyticsModule);
		if (a->__sizeAnalyticsModule)
			a->AnalyticsModule = (struct tt__Config *)soap_save_block(soap, soap_blist_AnalyticsModule, NULL, 1);
		else
		{	a->AnalyticsModule = NULL;
		if (soap_blist_AnalyticsModule)
			soap_end_block(soap, soap_blist_AnalyticsModule);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _tan__GetAnalyticsModulesResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tan__GetAnalyticsModulesResponse, 0, sizeof(struct _tan__GetAnalyticsModulesResponse), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__GetAnalyticsModulesResponse(struct soap *soap, const struct _tan__GetAnalyticsModulesResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tan__GetAnalyticsModulesResponse);
	if (soap_out__tan__GetAnalyticsModulesResponse(soap, tag?tag:"tan:GetAnalyticsModulesResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__GetAnalyticsModulesResponse * SOAP_FMAC4 soap_get__tan__GetAnalyticsModulesResponse(struct soap *soap, struct _tan__GetAnalyticsModulesResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tan__GetAnalyticsModulesResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__GetAnalyticsModules(struct soap *soap, struct _tan__GetAnalyticsModules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->ConfigurationToken);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__GetAnalyticsModules(struct soap *soap, const struct _tan__GetAnalyticsModules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->ConfigurationToken);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__GetAnalyticsModules(struct soap *soap, const char *tag, int id, const struct _tan__GetAnalyticsModules *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tan__GetAnalyticsModules), type))
		return soap->error;
	if (a->ConfigurationToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tan:ConfigurationToken", -1, &a->ConfigurationToken, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tan:ConfigurationToken"))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tan__GetAnalyticsModules * SOAP_FMAC4 soap_in__tan__GetAnalyticsModules(struct soap *soap, const char *tag, struct _tan__GetAnalyticsModules *a, const char *type)
{
	size_t soap_flag_ConfigurationToken = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tan__GetAnalyticsModules *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tan__GetAnalyticsModules, sizeof(struct _tan__GetAnalyticsModules), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tan__GetAnalyticsModules(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_ConfigurationToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_tt__ReferenceToken(soap, "tan:ConfigurationToken", &a->ConfigurationToken, "tt:ReferenceToken"))
			{	soap_flag_ConfigurationToken--;
		continue;
		}
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _tan__GetAnalyticsModules *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tan__GetAnalyticsModules, 0, sizeof(struct _tan__GetAnalyticsModules), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_ConfigurationToken > 0))
	{	soap->error = SOAP_OCCURS;
	return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__GetAnalyticsModules(struct soap *soap, const struct _tan__GetAnalyticsModules *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tan__GetAnalyticsModules);
	if (soap_out__tan__GetAnalyticsModules(soap, tag?tag:"tan:GetAnalyticsModules", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__GetAnalyticsModules * SOAP_FMAC4 soap_get__tan__GetAnalyticsModules(struct soap *soap, struct _tan__GetAnalyticsModules *p, const char *tag, const char *type)
{
	if ((p = soap_in__tan__GetAnalyticsModules(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__ModifyAnalyticsModulesResponse(struct soap *soap, struct _tan__ModifyAnalyticsModulesResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__ModifyAnalyticsModulesResponse(struct soap *soap, const struct _tan__ModifyAnalyticsModulesResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__ModifyAnalyticsModulesResponse(struct soap *soap, const char *tag, int id, const struct _tan__ModifyAnalyticsModulesResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tan__ModifyAnalyticsModulesResponse), type))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tan__ModifyAnalyticsModulesResponse * SOAP_FMAC4 soap_in__tan__ModifyAnalyticsModulesResponse(struct soap *soap, const char *tag, struct _tan__ModifyAnalyticsModulesResponse *a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tan__ModifyAnalyticsModulesResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tan__ModifyAnalyticsModulesResponse, sizeof(struct _tan__ModifyAnalyticsModulesResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tan__ModifyAnalyticsModulesResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _tan__ModifyAnalyticsModulesResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tan__ModifyAnalyticsModulesResponse, 0, sizeof(struct _tan__ModifyAnalyticsModulesResponse), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__ModifyAnalyticsModulesResponse(struct soap *soap, const struct _tan__ModifyAnalyticsModulesResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tan__ModifyAnalyticsModulesResponse);
	if (soap_out__tan__ModifyAnalyticsModulesResponse(soap, tag?tag:"tan:ModifyAnalyticsModulesResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__ModifyAnalyticsModulesResponse * SOAP_FMAC4 soap_get__tan__ModifyAnalyticsModulesResponse(struct soap *soap, struct _tan__ModifyAnalyticsModulesResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tan__ModifyAnalyticsModulesResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__ModifyAnalyticsModules(struct soap *soap, struct _tan__ModifyAnalyticsModules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->ConfigurationToken);
	a->__sizeAnalyticsModule = 0;
	a->AnalyticsModule = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__ModifyAnalyticsModules(struct soap *soap, const struct _tan__ModifyAnalyticsModules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->ConfigurationToken);
	if (a->AnalyticsModule)
	{	int i;
	for (i = 0; i < a->__sizeAnalyticsModule; i++)
	{
		soap_embedded(soap, a->AnalyticsModule + i, SOAP_TYPE_tt__Config);
		soap_serialize_tt__Config(soap, a->AnalyticsModule + i);
	}
	}
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__ModifyAnalyticsModules(struct soap *soap, const char *tag, int id, const struct _tan__ModifyAnalyticsModules *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tan__ModifyAnalyticsModules), type))
		return soap->error;
	if (a->ConfigurationToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tan:ConfigurationToken", -1, &a->ConfigurationToken, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tan:ConfigurationToken"))
		return soap->error;
	if (a->AnalyticsModule)
	{	int i;
	for (i = 0; i < a->__sizeAnalyticsModule; i++)
		if (soap_out_tt__Config(soap, "tan:AnalyticsModule", -1, a->AnalyticsModule + i, ""))
			return soap->error;
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tan__ModifyAnalyticsModules * SOAP_FMAC4 soap_in__tan__ModifyAnalyticsModules(struct soap *soap, const char *tag, struct _tan__ModifyAnalyticsModules *a, const char *type)
{
	size_t soap_flag_ConfigurationToken = 1;
	struct soap_blist *soap_blist_AnalyticsModule = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tan__ModifyAnalyticsModules *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tan__ModifyAnalyticsModules, sizeof(struct _tan__ModifyAnalyticsModules), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tan__ModifyAnalyticsModules(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_ConfigurationToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_tt__ReferenceToken(soap, "tan:ConfigurationToken", &a->ConfigurationToken, "tt:ReferenceToken"))
			{	soap_flag_ConfigurationToken--;
		continue;
		}
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tan:AnalyticsModule", 1, NULL))
		{	if (a->AnalyticsModule == NULL)
		{	if (soap_blist_AnalyticsModule == NULL)
		soap_blist_AnalyticsModule = soap_new_block(soap);
		a->AnalyticsModule = (struct tt__Config *)soap_push_block(soap, soap_blist_AnalyticsModule, sizeof(struct tt__Config));
		if (a->AnalyticsModule == NULL)
			return NULL;
		soap_default_tt__Config(soap, a->AnalyticsModule);
		}
		soap_revert(soap);
		if (soap_in_tt__Config(soap, "tan:AnalyticsModule", a->AnalyticsModule, "tt:Config"))
		{	a->__sizeAnalyticsModule++;
		a->AnalyticsModule = NULL;
		continue;
		}
		}
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (a->AnalyticsModule)
			soap_pop_block(soap, soap_blist_AnalyticsModule);
		if (a->__sizeAnalyticsModule)
			a->AnalyticsModule = (struct tt__Config *)soap_save_block(soap, soap_blist_AnalyticsModule, NULL, 1);
		else
		{	a->AnalyticsModule = NULL;
		if (soap_blist_AnalyticsModule)
			soap_end_block(soap, soap_blist_AnalyticsModule);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _tan__ModifyAnalyticsModules *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tan__ModifyAnalyticsModules, 0, sizeof(struct _tan__ModifyAnalyticsModules), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_ConfigurationToken > 0 || a->__sizeAnalyticsModule < 1))
	{	soap->error = SOAP_OCCURS;
	return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__ModifyAnalyticsModules(struct soap *soap, const struct _tan__ModifyAnalyticsModules *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tan__ModifyAnalyticsModules);
	if (soap_out__tan__ModifyAnalyticsModules(soap, tag?tag:"tan:ModifyAnalyticsModules", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__ModifyAnalyticsModules * SOAP_FMAC4 soap_get__tan__ModifyAnalyticsModules(struct soap *soap, struct _tan__ModifyAnalyticsModules *p, const char *tag, const char *type)
{
	if ((p = soap_in__tan__ModifyAnalyticsModules(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__DeleteAnalyticsModulesResponse(struct soap *soap, struct _tan__DeleteAnalyticsModulesResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__DeleteAnalyticsModulesResponse(struct soap *soap, const struct _tan__DeleteAnalyticsModulesResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__DeleteAnalyticsModulesResponse(struct soap *soap, const char *tag, int id, const struct _tan__DeleteAnalyticsModulesResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tan__DeleteAnalyticsModulesResponse), type))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tan__DeleteAnalyticsModulesResponse * SOAP_FMAC4 soap_in__tan__DeleteAnalyticsModulesResponse(struct soap *soap, const char *tag, struct _tan__DeleteAnalyticsModulesResponse *a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tan__DeleteAnalyticsModulesResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tan__DeleteAnalyticsModulesResponse, sizeof(struct _tan__DeleteAnalyticsModulesResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tan__DeleteAnalyticsModulesResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _tan__DeleteAnalyticsModulesResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tan__DeleteAnalyticsModulesResponse, 0, sizeof(struct _tan__DeleteAnalyticsModulesResponse), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__DeleteAnalyticsModulesResponse(struct soap *soap, const struct _tan__DeleteAnalyticsModulesResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tan__DeleteAnalyticsModulesResponse);
	if (soap_out__tan__DeleteAnalyticsModulesResponse(soap, tag?tag:"tan:DeleteAnalyticsModulesResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__DeleteAnalyticsModulesResponse * SOAP_FMAC4 soap_get__tan__DeleteAnalyticsModulesResponse(struct soap *soap, struct _tan__DeleteAnalyticsModulesResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tan__DeleteAnalyticsModulesResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__DeleteAnalyticsModules(struct soap *soap, struct _tan__DeleteAnalyticsModules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->ConfigurationToken);
	a->__sizeAnalyticsModuleName = 0;
	a->AnalyticsModuleName = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__DeleteAnalyticsModules(struct soap *soap, const struct _tan__DeleteAnalyticsModules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->ConfigurationToken);
	if (a->AnalyticsModuleName)
	{	int i;
	for (i = 0; i < a->__sizeAnalyticsModuleName; i++)
	{
		soap_serialize_string(soap, a->AnalyticsModuleName + i);
	}
	}
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__DeleteAnalyticsModules(struct soap *soap, const char *tag, int id, const struct _tan__DeleteAnalyticsModules *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tan__DeleteAnalyticsModules), type))
		return soap->error;
	if (a->ConfigurationToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tan:ConfigurationToken", -1, &a->ConfigurationToken, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tan:ConfigurationToken"))
		return soap->error;
	if (a->AnalyticsModuleName)
	{	int i;
	for (i = 0; i < a->__sizeAnalyticsModuleName; i++)
		if (soap_out_string(soap, "tan:AnalyticsModuleName", -1, a->AnalyticsModuleName + i, ""))
			return soap->error;
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tan__DeleteAnalyticsModules * SOAP_FMAC4 soap_in__tan__DeleteAnalyticsModules(struct soap *soap, const char *tag, struct _tan__DeleteAnalyticsModules *a, const char *type)
{
	size_t soap_flag_ConfigurationToken = 1;
	struct soap_blist *soap_blist_AnalyticsModuleName = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tan__DeleteAnalyticsModules *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tan__DeleteAnalyticsModules, sizeof(struct _tan__DeleteAnalyticsModules), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tan__DeleteAnalyticsModules(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_ConfigurationToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_tt__ReferenceToken(soap, "tan:ConfigurationToken", &a->ConfigurationToken, "tt:ReferenceToken"))
			{	soap_flag_ConfigurationToken--;
		continue;
		}
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tan:AnalyticsModuleName", 1, NULL))
		{	if (a->AnalyticsModuleName == NULL)
		{	if (soap_blist_AnalyticsModuleName == NULL)
		soap_blist_AnalyticsModuleName = soap_new_block(soap);
		a->AnalyticsModuleName = (char **)soap_push_block(soap, soap_blist_AnalyticsModuleName, sizeof(char *));
		if (a->AnalyticsModuleName == NULL)
			return NULL;
		*a->AnalyticsModuleName = NULL;
		}
		soap_revert(soap);
		if (soap_in_string(soap, "tan:AnalyticsModuleName", a->AnalyticsModuleName, "xsd:string"))
		{	a->__sizeAnalyticsModuleName++;
		a->AnalyticsModuleName = NULL;
		continue;
		}
		}
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (a->AnalyticsModuleName)
			soap_pop_block(soap, soap_blist_AnalyticsModuleName);
		if (a->__sizeAnalyticsModuleName)
			a->AnalyticsModuleName = (char **)soap_save_block(soap, soap_blist_AnalyticsModuleName, NULL, 1);
		else
		{	a->AnalyticsModuleName = NULL;
		if (soap_blist_AnalyticsModuleName)
			soap_end_block(soap, soap_blist_AnalyticsModuleName);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _tan__DeleteAnalyticsModules *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tan__DeleteAnalyticsModules, 0, sizeof(struct _tan__DeleteAnalyticsModules), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_ConfigurationToken > 0 || a->__sizeAnalyticsModuleName < 1))
	{	soap->error = SOAP_OCCURS;
	return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__DeleteAnalyticsModules(struct soap *soap, const struct _tan__DeleteAnalyticsModules *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tan__DeleteAnalyticsModules);
	if (soap_out__tan__DeleteAnalyticsModules(soap, tag?tag:"tan:DeleteAnalyticsModules", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__DeleteAnalyticsModules * SOAP_FMAC4 soap_get__tan__DeleteAnalyticsModules(struct soap *soap, struct _tan__DeleteAnalyticsModules *p, const char *tag, const char *type)
{
	if ((p = soap_in__tan__DeleteAnalyticsModules(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__CreateAnalyticsModulesResponse(struct soap *soap, struct _tan__CreateAnalyticsModulesResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__CreateAnalyticsModulesResponse(struct soap *soap, const struct _tan__CreateAnalyticsModulesResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__CreateAnalyticsModulesResponse(struct soap *soap, const char *tag, int id, const struct _tan__CreateAnalyticsModulesResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tan__CreateAnalyticsModulesResponse), type))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tan__CreateAnalyticsModulesResponse * SOAP_FMAC4 soap_in__tan__CreateAnalyticsModulesResponse(struct soap *soap, const char *tag, struct _tan__CreateAnalyticsModulesResponse *a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tan__CreateAnalyticsModulesResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tan__CreateAnalyticsModulesResponse, sizeof(struct _tan__CreateAnalyticsModulesResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tan__CreateAnalyticsModulesResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _tan__CreateAnalyticsModulesResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tan__CreateAnalyticsModulesResponse, 0, sizeof(struct _tan__CreateAnalyticsModulesResponse), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__CreateAnalyticsModulesResponse(struct soap *soap, const struct _tan__CreateAnalyticsModulesResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tan__CreateAnalyticsModulesResponse);
	if (soap_out__tan__CreateAnalyticsModulesResponse(soap, tag?tag:"tan:CreateAnalyticsModulesResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__CreateAnalyticsModulesResponse * SOAP_FMAC4 soap_get__tan__CreateAnalyticsModulesResponse(struct soap *soap, struct _tan__CreateAnalyticsModulesResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tan__CreateAnalyticsModulesResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__CreateAnalyticsModules(struct soap *soap, struct _tan__CreateAnalyticsModules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->ConfigurationToken);
	a->__sizeAnalyticsModule = 0;
	a->AnalyticsModule = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__CreateAnalyticsModules(struct soap *soap, const struct _tan__CreateAnalyticsModules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->ConfigurationToken);
	if (a->AnalyticsModule)
	{	int i;
	for (i = 0; i < a->__sizeAnalyticsModule; i++)
	{
		soap_embedded(soap, a->AnalyticsModule + i, SOAP_TYPE_tt__Config);
		soap_serialize_tt__Config(soap, a->AnalyticsModule + i);
	}
	}
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__CreateAnalyticsModules(struct soap *soap, const char *tag, int id, const struct _tan__CreateAnalyticsModules *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tan__CreateAnalyticsModules), type))
		return soap->error;
	if (a->ConfigurationToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tan:ConfigurationToken", -1, &a->ConfigurationToken, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tan:ConfigurationToken"))
		return soap->error;
	if (a->AnalyticsModule)
	{	int i;
	for (i = 0; i < a->__sizeAnalyticsModule; i++)
		if (soap_out_tt__Config(soap, "tan:AnalyticsModule", -1, a->AnalyticsModule + i, ""))
			return soap->error;
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tan__CreateAnalyticsModules * SOAP_FMAC4 soap_in__tan__CreateAnalyticsModules(struct soap *soap, const char *tag, struct _tan__CreateAnalyticsModules *a, const char *type)
{
	size_t soap_flag_ConfigurationToken = 1;
	struct soap_blist *soap_blist_AnalyticsModule = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tan__CreateAnalyticsModules *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tan__CreateAnalyticsModules, sizeof(struct _tan__CreateAnalyticsModules), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tan__CreateAnalyticsModules(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_ConfigurationToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_tt__ReferenceToken(soap, "tan:ConfigurationToken", &a->ConfigurationToken, "tt:ReferenceToken"))
			{	soap_flag_ConfigurationToken--;
		continue;
		}
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tan:AnalyticsModule", 1, NULL))
		{	if (a->AnalyticsModule == NULL)
		{	if (soap_blist_AnalyticsModule == NULL)
		soap_blist_AnalyticsModule = soap_new_block(soap);
		a->AnalyticsModule = (struct tt__Config *)soap_push_block(soap, soap_blist_AnalyticsModule, sizeof(struct tt__Config));
		if (a->AnalyticsModule == NULL)
			return NULL;
		soap_default_tt__Config(soap, a->AnalyticsModule);
		}
		soap_revert(soap);
		if (soap_in_tt__Config(soap, "tan:AnalyticsModule", a->AnalyticsModule, "tt:Config"))
		{	a->__sizeAnalyticsModule++;
		a->AnalyticsModule = NULL;
		continue;
		}
		}
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (a->AnalyticsModule)
			soap_pop_block(soap, soap_blist_AnalyticsModule);
		if (a->__sizeAnalyticsModule)
			a->AnalyticsModule = (struct tt__Config *)soap_save_block(soap, soap_blist_AnalyticsModule, NULL, 1);
		else
		{	a->AnalyticsModule = NULL;
		if (soap_blist_AnalyticsModule)
			soap_end_block(soap, soap_blist_AnalyticsModule);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _tan__CreateAnalyticsModules *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tan__CreateAnalyticsModules, 0, sizeof(struct _tan__CreateAnalyticsModules), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_ConfigurationToken > 0 || a->__sizeAnalyticsModule < 1))
	{	soap->error = SOAP_OCCURS;
	return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__CreateAnalyticsModules(struct soap *soap, const struct _tan__CreateAnalyticsModules *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tan__CreateAnalyticsModules);
	if (soap_out__tan__CreateAnalyticsModules(soap, tag?tag:"tan:CreateAnalyticsModules", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__CreateAnalyticsModules * SOAP_FMAC4 soap_get__tan__CreateAnalyticsModules(struct soap *soap, struct _tan__CreateAnalyticsModules *p, const char *tag, const char *type)
{
	if ((p = soap_in__tan__CreateAnalyticsModules(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}


SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__GetRulesResponse(struct soap *soap, struct _tan__GetRulesResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__sizeRule = 0;
	a->Rule = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__GetRulesResponse(struct soap *soap, const struct _tan__GetRulesResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	if (a->Rule)
	{	int i;
	for (i = 0; i < a->__sizeRule; i++)
	{
		soap_embedded(soap, a->Rule + i, SOAP_TYPE_tt__Config);
		soap_serialize_tt__Config(soap, a->Rule + i);
	}
	}
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__GetRulesResponse(struct soap *soap, const char *tag, int id, const struct _tan__GetRulesResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tan__GetRulesResponse), type))
		return soap->error;
	soap_element_result(soap, "-sizeRule");
	if (a->Rule)
	{	int i;
	for (i = 0; i < a->__sizeRule; i++)
		if (soap_out_tt__Config(soap, "tan:Rule", -1, a->Rule + i, ""))
			return soap->error;
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tan__GetRulesResponse * SOAP_FMAC4 soap_in__tan__GetRulesResponse(struct soap *soap, const char *tag, struct _tan__GetRulesResponse *a, const char *type)
{
	struct soap_blist *soap_blist_Rule = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tan__GetRulesResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tan__GetRulesResponse, sizeof(struct _tan__GetRulesResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tan__GetRulesResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tan:Rule", 1, NULL))
		{	if (a->Rule == NULL)
		{	if (soap_blist_Rule == NULL)
		soap_blist_Rule = soap_new_block(soap);
		a->Rule = (struct tt__Config *)soap_push_block(soap, soap_blist_Rule, sizeof(struct tt__Config));
		if (a->Rule == NULL)
			return NULL;
		soap_default_tt__Config(soap, a->Rule);
		}
		soap_revert(soap);
		if (soap_in_tt__Config(soap, "tan:Rule", a->Rule, "tt:Config"))
		{	a->__sizeRule++;
		a->Rule = NULL;
		continue;
		}
		}
		soap_check_result(soap, "-sizeRule");
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (a->Rule)
			soap_pop_block(soap, soap_blist_Rule);
		if (a->__sizeRule)
			a->Rule = (struct tt__Config *)soap_save_block(soap, soap_blist_Rule, NULL, 1);
		else
		{	a->Rule = NULL;
		if (soap_blist_Rule)
			soap_end_block(soap, soap_blist_Rule);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _tan__GetRulesResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tan__GetRulesResponse, 0, sizeof(struct _tan__GetRulesResponse), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__GetRulesResponse(struct soap *soap, const struct _tan__GetRulesResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tan__GetRulesResponse);
	if (soap_out__tan__GetRulesResponse(soap, tag?tag:"tan:GetRulesResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__GetRulesResponse * SOAP_FMAC4 soap_get__tan__GetRulesResponse(struct soap *soap, struct _tan__GetRulesResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tan__GetRulesResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__GetRules(struct soap *soap, struct _tan__GetRules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->ConfigurationToken);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__GetRules(struct soap *soap, const struct _tan__GetRules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->ConfigurationToken);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__GetRules(struct soap *soap, const char *tag, int id, const struct _tan__GetRules *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tan__GetRules), type))
		return soap->error;
	if (a->ConfigurationToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tan:ConfigurationToken", -1, &a->ConfigurationToken, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tan:ConfigurationToken"))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tan__GetRules * SOAP_FMAC4 soap_in__tan__GetRules(struct soap *soap, const char *tag, struct _tan__GetRules *a, const char *type)
{
	size_t soap_flag_ConfigurationToken = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tan__GetRules *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tan__GetRules, sizeof(struct _tan__GetRules), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tan__GetRules(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_ConfigurationToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_tt__ReferenceToken(soap, "tan:ConfigurationToken", &a->ConfigurationToken, "tt:ReferenceToken"))
			{	soap_flag_ConfigurationToken--;
		continue;
		}
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _tan__GetRules *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tan__GetRules, 0, sizeof(struct _tan__GetRules), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_ConfigurationToken > 0))
	{	soap->error = SOAP_OCCURS;
	return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__GetRules(struct soap *soap, const struct _tan__GetRules *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tan__GetRules);
	if (soap_out__tan__GetRules(soap, tag?tag:"tan:GetRules", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__GetRules * SOAP_FMAC4 soap_get__tan__GetRules(struct soap *soap, struct _tan__GetRules *p, const char *tag, const char *type)
{
	if ((p = soap_in__tan__GetRules(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__ModifyRulesResponse(struct soap *soap, struct _tan__ModifyRulesResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__ModifyRulesResponse(struct soap *soap, const struct _tan__ModifyRulesResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__ModifyRulesResponse(struct soap *soap, const char *tag, int id, const struct _tan__ModifyRulesResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tan__ModifyRulesResponse), type))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tan__ModifyRulesResponse * SOAP_FMAC4 soap_in__tan__ModifyRulesResponse(struct soap *soap, const char *tag, struct _tan__ModifyRulesResponse *a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tan__ModifyRulesResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tan__ModifyRulesResponse, sizeof(struct _tan__ModifyRulesResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tan__ModifyRulesResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _tan__ModifyRulesResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tan__ModifyRulesResponse, 0, sizeof(struct _tan__ModifyRulesResponse), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__ModifyRulesResponse(struct soap *soap, const struct _tan__ModifyRulesResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tan__ModifyRulesResponse);
	if (soap_out__tan__ModifyRulesResponse(soap, tag?tag:"tan:ModifyRulesResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__ModifyRulesResponse * SOAP_FMAC4 soap_get__tan__ModifyRulesResponse(struct soap *soap, struct _tan__ModifyRulesResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tan__ModifyRulesResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__ModifyRules(struct soap *soap, struct _tan__ModifyRules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->ConfigurationToken);
	a->__sizeRule = 0;
	a->Rule = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__ModifyRules(struct soap *soap, const struct _tan__ModifyRules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->ConfigurationToken);
	if (a->Rule)
	{	int i;
	for (i = 0; i < a->__sizeRule; i++)
	{
		soap_embedded(soap, a->Rule + i, SOAP_TYPE_tt__Config);
		soap_serialize_tt__Config(soap, a->Rule + i);
	}
	}
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__ModifyRules(struct soap *soap, const char *tag, int id, const struct _tan__ModifyRules *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tan__ModifyRules), type))
		return soap->error;
	if (a->ConfigurationToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tan:ConfigurationToken", -1, &a->ConfigurationToken, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tan:ConfigurationToken"))
		return soap->error;
	if (a->Rule)
	{	int i;
	for (i = 0; i < a->__sizeRule; i++)
		if (soap_out_tt__Config(soap, "tan:Rule", -1, a->Rule + i, ""))
			return soap->error;
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tan__ModifyRules * SOAP_FMAC4 soap_in__tan__ModifyRules(struct soap *soap, const char *tag, struct _tan__ModifyRules *a, const char *type)
{
	size_t soap_flag_ConfigurationToken = 1;
	struct soap_blist *soap_blist_Rule = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tan__ModifyRules *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tan__ModifyRules, sizeof(struct _tan__ModifyRules), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tan__ModifyRules(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_ConfigurationToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_tt__ReferenceToken(soap, "tan:ConfigurationToken", &a->ConfigurationToken, "tt:ReferenceToken"))
			{	soap_flag_ConfigurationToken--;
		continue;
		}
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tan:Rule", 1, NULL))
		{	if (a->Rule == NULL)
		{	if (soap_blist_Rule == NULL)
		soap_blist_Rule = soap_new_block(soap);
		a->Rule = (struct tt__Config *)soap_push_block(soap, soap_blist_Rule, sizeof(struct tt__Config));
		if (a->Rule == NULL)
			return NULL;
		soap_default_tt__Config(soap, a->Rule);
		}
		soap_revert(soap);
		if (soap_in_tt__Config(soap, "tan:Rule", a->Rule, "tt:Config"))
		{	a->__sizeRule++;
		a->Rule = NULL;
		continue;
		}
		}
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (a->Rule)
			soap_pop_block(soap, soap_blist_Rule);
		if (a->__sizeRule)
			a->Rule = (struct tt__Config *)soap_save_block(soap, soap_blist_Rule, NULL, 1);
		else
		{	a->Rule = NULL;
		if (soap_blist_Rule)
			soap_end_block(soap, soap_blist_Rule);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _tan__ModifyRules *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tan__ModifyRules, 0, sizeof(struct _tan__ModifyRules), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_ConfigurationToken > 0 || a->__sizeRule < 1))
	{	soap->error = SOAP_OCCURS;
	return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__ModifyRules(struct soap *soap, const struct _tan__ModifyRules *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tan__ModifyRules);
	if (soap_out__tan__ModifyRules(soap, tag?tag:"tan:ModifyRules", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__ModifyRules * SOAP_FMAC4 soap_get__tan__ModifyRules(struct soap *soap, struct _tan__ModifyRules *p, const char *tag, const char *type)
{
	if ((p = soap_in__tan__ModifyRules(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__DeleteRulesResponse(struct soap *soap, struct _tan__DeleteRulesResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__DeleteRulesResponse(struct soap *soap, const struct _tan__DeleteRulesResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__DeleteRulesResponse(struct soap *soap, const char *tag, int id, const struct _tan__DeleteRulesResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tan__DeleteRulesResponse), type))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tan__DeleteRulesResponse * SOAP_FMAC4 soap_in__tan__DeleteRulesResponse(struct soap *soap, const char *tag, struct _tan__DeleteRulesResponse *a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tan__DeleteRulesResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tan__DeleteRulesResponse, sizeof(struct _tan__DeleteRulesResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tan__DeleteRulesResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _tan__DeleteRulesResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tan__DeleteRulesResponse, 0, sizeof(struct _tan__DeleteRulesResponse), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__DeleteRulesResponse(struct soap *soap, const struct _tan__DeleteRulesResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tan__DeleteRulesResponse);
	if (soap_out__tan__DeleteRulesResponse(soap, tag?tag:"tan:DeleteRulesResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__DeleteRulesResponse * SOAP_FMAC4 soap_get__tan__DeleteRulesResponse(struct soap *soap, struct _tan__DeleteRulesResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tan__DeleteRulesResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__DeleteRules(struct soap *soap, struct _tan__DeleteRules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->ConfigurationToken);
	a->__sizeRuleName = 0;
	a->RuleName = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__DeleteRules(struct soap *soap, const struct _tan__DeleteRules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->ConfigurationToken);
	if (a->RuleName)
	{	int i;
	for (i = 0; i < a->__sizeRuleName; i++)
	{
		soap_serialize_string(soap, a->RuleName + i);
	}
	}
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__DeleteRules(struct soap *soap, const char *tag, int id, const struct _tan__DeleteRules *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tan__DeleteRules), type))
		return soap->error;
	if (a->ConfigurationToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tan:ConfigurationToken", -1, &a->ConfigurationToken, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tan:ConfigurationToken"))
		return soap->error;
	if (a->RuleName)
	{	int i;
	for (i = 0; i < a->__sizeRuleName; i++)
		if (soap_out_string(soap, "tan:RuleName", -1, a->RuleName + i, ""))
			return soap->error;
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tan__DeleteRules * SOAP_FMAC4 soap_in__tan__DeleteRules(struct soap *soap, const char *tag, struct _tan__DeleteRules *a, const char *type)
{
	size_t soap_flag_ConfigurationToken = 1;
	struct soap_blist *soap_blist_RuleName = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tan__DeleteRules *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tan__DeleteRules, sizeof(struct _tan__DeleteRules), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tan__DeleteRules(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_ConfigurationToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_tt__ReferenceToken(soap, "tan:ConfigurationToken", &a->ConfigurationToken, "tt:ReferenceToken"))
			{	soap_flag_ConfigurationToken--;
		continue;
		}
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tan:RuleName", 1, NULL))
		{	if (a->RuleName == NULL)
		{	if (soap_blist_RuleName == NULL)
		soap_blist_RuleName = soap_new_block(soap);
		a->RuleName = (char **)soap_push_block(soap, soap_blist_RuleName, sizeof(char *));
		if (a->RuleName == NULL)
			return NULL;
		*a->RuleName = NULL;
		}
		soap_revert(soap);
		if (soap_in_string(soap, "tan:RuleName", a->RuleName, "xsd:string"))
		{	a->__sizeRuleName++;
		a->RuleName = NULL;
		continue;
		}
		}
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (a->RuleName)
			soap_pop_block(soap, soap_blist_RuleName);
		if (a->__sizeRuleName)
			a->RuleName = (char **)soap_save_block(soap, soap_blist_RuleName, NULL, 1);
		else
		{	a->RuleName = NULL;
		if (soap_blist_RuleName)
			soap_end_block(soap, soap_blist_RuleName);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _tan__DeleteRules *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tan__DeleteRules, 0, sizeof(struct _tan__DeleteRules), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_ConfigurationToken > 0 || a->__sizeRuleName < 1))
	{	soap->error = SOAP_OCCURS;
	return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__DeleteRules(struct soap *soap, const struct _tan__DeleteRules *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tan__DeleteRules);
	if (soap_out__tan__DeleteRules(soap, tag?tag:"tan:DeleteRules", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__DeleteRules * SOAP_FMAC4 soap_get__tan__DeleteRules(struct soap *soap, struct _tan__DeleteRules *p, const char *tag, const char *type)
{
	if ((p = soap_in__tan__DeleteRules(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__CreateRulesResponse(struct soap *soap, struct _tan__CreateRulesResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__CreateRulesResponse(struct soap *soap, const struct _tan__CreateRulesResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__CreateRulesResponse(struct soap *soap, const char *tag, int id, const struct _tan__CreateRulesResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tan__CreateRulesResponse), type))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tan__CreateRulesResponse * SOAP_FMAC4 soap_in__tan__CreateRulesResponse(struct soap *soap, const char *tag, struct _tan__CreateRulesResponse *a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tan__CreateRulesResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tan__CreateRulesResponse, sizeof(struct _tan__CreateRulesResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tan__CreateRulesResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _tan__CreateRulesResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tan__CreateRulesResponse, 0, sizeof(struct _tan__CreateRulesResponse), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__CreateRulesResponse(struct soap *soap, const struct _tan__CreateRulesResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tan__CreateRulesResponse);
	if (soap_out__tan__CreateRulesResponse(soap, tag?tag:"tan:CreateRulesResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__CreateRulesResponse * SOAP_FMAC4 soap_get__tan__CreateRulesResponse(struct soap *soap, struct _tan__CreateRulesResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tan__CreateRulesResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__Config(struct soap *soap, struct tt__Config *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->Parameters = NULL;
	soap_default_string(soap, &a->Name);
	soap_default__QName(soap, &a->Type);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__Config(struct soap *soap, const struct tt__Config *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTott__ItemList(soap, &a->Parameters);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__Config(struct soap *soap, const char *tag, int id, const struct tt__Config *a, const char *type)
{
	if (a->Name)
		soap_set_attr(soap, "Name", a->Name, 1);
	if (a->Type)
		soap_set_attr(soap, "Type", soap_QName2s(soap, a->Type), 1);
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__Config), type))
		return soap->error;
	if (a->Parameters)
	{	if (soap_out_PointerTott__ItemList(soap, "tt:Parameters", -1, &a->Parameters, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tt:Parameters"))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__Config * SOAP_FMAC4 soap_in_tt__Config(struct soap *soap, const char *tag, struct tt__Config *a, const char *type)
{
	size_t soap_flag_Parameters = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__Config *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__Config, sizeof(struct tt__Config), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__Config(soap, a);
	if (soap_s2string(soap, soap_attr_value(soap, "Name", 1), &a->Name, 0, -1))
		return NULL;
	if (soap_s2QName(soap, soap_attr_value(soap, "Type", 1), &a->Type, 0, -1))
		return NULL;
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_Parameters && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_PointerTott__ItemList(soap, "tt:Parameters", &a->Parameters, "tt:ItemList"))
			{	soap_flag_Parameters--;
		continue;
		}
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct tt__Config *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__Config, 0, sizeof(struct tt__Config), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_Parameters > 0))
	{	soap->error = SOAP_OCCURS;
	return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__Config(struct soap *soap, const struct tt__Config *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__Config);
	if (soap_out_tt__Config(soap, tag?tag:"tt:Config", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__Config * SOAP_FMAC4 soap_get_tt__Config(struct soap *soap, struct tt__Config *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__Config(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__CreateRules(struct soap *soap, struct _tan__CreateRules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->ConfigurationToken);
	a->__sizeRule = 0;
	a->Rule = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__CreateRules(struct soap *soap, const struct _tan__CreateRules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->ConfigurationToken);
	if (a->Rule)
	{	int i;
	for (i = 0; i < a->__sizeRule; i++)
	{
		soap_embedded(soap, a->Rule + i, SOAP_TYPE_tt__Config);
		soap_serialize_tt__Config(soap, a->Rule + i);
	}
	}
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__CreateRules(struct soap *soap, const char *tag, int id, const struct _tan__CreateRules *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tan__CreateRules), type))
		return soap->error;
	if (a->ConfigurationToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tan:ConfigurationToken", -1, &a->ConfigurationToken, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tan:ConfigurationToken"))
		return soap->error;
	if (a->Rule)
	{	int i;
	for (i = 0; i < a->__sizeRule; i++)
		if (soap_out_tt__Config(soap, "tan:Rule", -1, a->Rule + i, ""))
			return soap->error;
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tan__CreateRules * SOAP_FMAC4 soap_in__tan__CreateRules(struct soap *soap, const char *tag, struct _tan__CreateRules *a, const char *type)
{
	size_t soap_flag_ConfigurationToken = 1;
	struct soap_blist *soap_blist_Rule = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tan__CreateRules *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tan__CreateRules, sizeof(struct _tan__CreateRules), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tan__CreateRules(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_ConfigurationToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_tt__ReferenceToken(soap, "tan:ConfigurationToken", &a->ConfigurationToken, "tt:ReferenceToken"))
			{	soap_flag_ConfigurationToken--;
		continue;
		}
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tan:Rule", 1, NULL))
		{	if (a->Rule == NULL)
		{	if (soap_blist_Rule == NULL)
		soap_blist_Rule = soap_new_block(soap);
		a->Rule = (struct tt__Config *)soap_push_block(soap, soap_blist_Rule, sizeof(struct tt__Config));
		if (a->Rule == NULL)
			return NULL;
		soap_default_tt__Config(soap, a->Rule);
		}
		soap_revert(soap);
		if (soap_in_tt__Config(soap, "tan:Rule", a->Rule, "tt:Config"))
		{	a->__sizeRule++;
		a->Rule = NULL;
		continue;
		}
		}
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (a->Rule)
			soap_pop_block(soap, soap_blist_Rule);
		if (a->__sizeRule)
			a->Rule = (struct tt__Config *)soap_save_block(soap, soap_blist_Rule, NULL, 1);
		else
		{	a->Rule = NULL;
		if (soap_blist_Rule)
			soap_end_block(soap, soap_blist_Rule);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _tan__CreateRules *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tan__CreateRules, 0, sizeof(struct _tan__CreateRules), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_ConfigurationToken > 0 || a->__sizeRule < 1))
	{	soap->error = SOAP_OCCURS;
	return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__CreateRules(struct soap *soap, const struct _tan__CreateRules *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tan__CreateRules);
	if (soap_out__tan__CreateRules(soap, tag?tag:"tan:CreateRules", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tan__CreateRules * SOAP_FMAC4 soap_get__tan__CreateRules(struct soap *soap, struct _tan__CreateRules *p, const char *tag, const char *type)
{
	if ((p = soap_in__tan__CreateRules(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}



///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////








SOAP_FMAC3 void SOAP_FMAC4 soap_default___dare__ModifyRules(struct soap *soap, struct __dare__ModifyRules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->tan__ModifyRules = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___dare__ModifyRules(struct soap *soap, const struct __dare__ModifyRules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_tan__ModifyRules(soap, &a->tan__ModifyRules);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___dare__ModifyRules(struct soap *soap, const char *tag, int id, const struct __dare__ModifyRules *a, const char *type)
{
	if (soap_out_PointerTo_tan__ModifyRules(soap, "tan:ModifyRules", -1, &a->tan__ModifyRules, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __dare__ModifyRules * SOAP_FMAC4 soap_in___dare__ModifyRules(struct soap *soap, const char *tag, struct __dare__ModifyRules *a, const char *type)
{
	size_t soap_flag_tan__ModifyRules = 1;
	short soap_flag;
	a = (struct __dare__ModifyRules *)soap_id_enter(soap, "", a, SOAP_TYPE___dare__ModifyRules, sizeof(struct __dare__ModifyRules), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___dare__ModifyRules(soap, a);
	for (soap_flag = 0;; soap_flag = 1)
	{	soap->error = SOAP_TAG_MISMATCH;
	if (soap_flag_tan__ModifyRules && soap->error == SOAP_TAG_MISMATCH)
		if (soap_in_PointerTo_tan__ModifyRules(soap, "tan:ModifyRules", &a->tan__ModifyRules, ""))
		{	soap_flag_tan__ModifyRules--;
	continue;
	}
	if (soap->error == SOAP_TAG_MISMATCH)
		if (soap_flag)
		{	soap->error = SOAP_OK;
	break;
	}
	if (soap_flag && soap->error == SOAP_NO_TAG)
		break;
	if (soap->error)
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put___dare__ModifyRules(struct soap *soap, const struct __dare__ModifyRules *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___dare__ModifyRules(soap, tag?tag:"-dare:ModifyRules", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __dare__ModifyRules * SOAP_FMAC4 soap_get___dare__ModifyRules(struct soap *soap, struct __dare__ModifyRules *p, const char *tag, const char *type)
{
	if ((p = soap_in___dare__ModifyRules(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___dare__GetRules(struct soap *soap, struct __dare__GetRules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->tan__GetRules = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___dare__GetRules(struct soap *soap, const struct __dare__GetRules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_tan__GetRules(soap, &a->tan__GetRules);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___dare__GetRules(struct soap *soap, const char *tag, int id, const struct __dare__GetRules *a, const char *type)
{
	if (soap_out_PointerTo_tan__GetRules(soap, "tan:GetRules", -1, &a->tan__GetRules, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __dare__GetRules * SOAP_FMAC4 soap_in___dare__GetRules(struct soap *soap, const char *tag, struct __dare__GetRules *a, const char *type)
{
	size_t soap_flag_tan__GetRules = 1;
	short soap_flag;
	a = (struct __dare__GetRules *)soap_id_enter(soap, "", a, SOAP_TYPE___dare__GetRules, sizeof(struct __dare__GetRules), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___dare__GetRules(soap, a);
	for (soap_flag = 0;; soap_flag = 1)
	{	soap->error = SOAP_TAG_MISMATCH;
	if (soap_flag_tan__GetRules && soap->error == SOAP_TAG_MISMATCH)
		if (soap_in_PointerTo_tan__GetRules(soap, "tan:GetRules", &a->tan__GetRules, ""))
		{	soap_flag_tan__GetRules--;
	continue;
	}
	if (soap->error == SOAP_TAG_MISMATCH)
		if (soap_flag)
		{	soap->error = SOAP_OK;
	break;
	}
	if (soap_flag && soap->error == SOAP_NO_TAG)
		break;
	if (soap->error)
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put___dare__GetRules(struct soap *soap, const struct __dare__GetRules *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___dare__GetRules(soap, tag?tag:"-dare:GetRules", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __dare__GetRules * SOAP_FMAC4 soap_get___dare__GetRules(struct soap *soap, struct __dare__GetRules *p, const char *tag, const char *type)
{
	if ((p = soap_in___dare__GetRules(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___dare__DeleteRules(struct soap *soap, struct __dare__DeleteRules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->tan__DeleteRules = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___dare__DeleteRules(struct soap *soap, const struct __dare__DeleteRules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_tan__DeleteRules(soap, &a->tan__DeleteRules);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___dare__DeleteRules(struct soap *soap, const char *tag, int id, const struct __dare__DeleteRules *a, const char *type)
{
	if (soap_out_PointerTo_tan__DeleteRules(soap, "tan:DeleteRules", -1, &a->tan__DeleteRules, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __dare__DeleteRules * SOAP_FMAC4 soap_in___dare__DeleteRules(struct soap *soap, const char *tag, struct __dare__DeleteRules *a, const char *type)
{
	size_t soap_flag_tan__DeleteRules = 1;
	short soap_flag;
	a = (struct __dare__DeleteRules *)soap_id_enter(soap, "", a, SOAP_TYPE___dare__DeleteRules, sizeof(struct __dare__DeleteRules), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___dare__DeleteRules(soap, a);
	for (soap_flag = 0;; soap_flag = 1)
	{	soap->error = SOAP_TAG_MISMATCH;
	if (soap_flag_tan__DeleteRules && soap->error == SOAP_TAG_MISMATCH)
		if (soap_in_PointerTo_tan__DeleteRules(soap, "tan:DeleteRules", &a->tan__DeleteRules, ""))
		{	soap_flag_tan__DeleteRules--;
	continue;
	}
	if (soap->error == SOAP_TAG_MISMATCH)
		if (soap_flag)
		{	soap->error = SOAP_OK;
	break;
	}
	if (soap_flag && soap->error == SOAP_NO_TAG)
		break;
	if (soap->error)
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put___dare__DeleteRules(struct soap *soap, const struct __dare__DeleteRules *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___dare__DeleteRules(soap, tag?tag:"-dare:DeleteRules", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __dare__DeleteRules * SOAP_FMAC4 soap_get___dare__DeleteRules(struct soap *soap, struct __dare__DeleteRules *p, const char *tag, const char *type)
{
	if ((p = soap_in___dare__DeleteRules(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___dare__CreateRules(struct soap *soap, struct __dare__CreateRules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->tan__CreateRules = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___dare__CreateRules(struct soap *soap, const struct __dare__CreateRules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_tan__CreateRules(soap, &a->tan__CreateRules);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___dare__CreateRules(struct soap *soap, const char *tag, int id, const struct __dare__CreateRules *a, const char *type)
{
	if (soap_out_PointerTo_tan__CreateRules(soap, "tan:CreateRules", -1, &a->tan__CreateRules, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __dare__CreateRules * SOAP_FMAC4 soap_in___dare__CreateRules(struct soap *soap, const char *tag, struct __dare__CreateRules *a, const char *type)
{
	size_t soap_flag_tan__CreateRules = 1;
	short soap_flag;
	a = (struct __dare__CreateRules *)soap_id_enter(soap, "", a, SOAP_TYPE___dare__CreateRules, sizeof(struct __dare__CreateRules), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___dare__CreateRules(soap, a);
	for (soap_flag = 0;; soap_flag = 1)
	{	soap->error = SOAP_TAG_MISMATCH;
	if (soap_flag_tan__CreateRules && soap->error == SOAP_TAG_MISMATCH)
		if (soap_in_PointerTo_tan__CreateRules(soap, "tan:CreateRules", &a->tan__CreateRules, ""))
		{	soap_flag_tan__CreateRules--;
	continue;
	}
	if (soap->error == SOAP_TAG_MISMATCH)
		if (soap_flag)
		{	soap->error = SOAP_OK;
	break;
	}
	if (soap_flag && soap->error == SOAP_NO_TAG)
		break;
	if (soap->error)
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put___dare__CreateRules(struct soap *soap, const struct __dare__CreateRules *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___dare__CreateRules(soap, tag?tag:"-dare:CreateRules", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __dare__CreateRules * SOAP_FMAC4 soap_get___dare__CreateRules(struct soap *soap, struct __dare__CreateRules *p, const char *tag, const char *type)
{
	if ((p = soap_in___dare__CreateRules(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___daae__ModifyAnalyticsModules(struct soap *soap, struct __daae__ModifyAnalyticsModules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->tan__ModifyAnalyticsModules = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___daae__ModifyAnalyticsModules(struct soap *soap, const struct __daae__ModifyAnalyticsModules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_tan__ModifyAnalyticsModules(soap, &a->tan__ModifyAnalyticsModules);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___daae__ModifyAnalyticsModules(struct soap *soap, const char *tag, int id, const struct __daae__ModifyAnalyticsModules *a, const char *type)
{
	if (soap_out_PointerTo_tan__ModifyAnalyticsModules(soap, "tan:ModifyAnalyticsModules", -1, &a->tan__ModifyAnalyticsModules, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __daae__ModifyAnalyticsModules * SOAP_FMAC4 soap_in___daae__ModifyAnalyticsModules(struct soap *soap, const char *tag, struct __daae__ModifyAnalyticsModules *a, const char *type)
{
	size_t soap_flag_tan__ModifyAnalyticsModules = 1;
	short soap_flag;
	a = (struct __daae__ModifyAnalyticsModules *)soap_id_enter(soap, "", a, SOAP_TYPE___daae__ModifyAnalyticsModules, sizeof(struct __daae__ModifyAnalyticsModules), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___daae__ModifyAnalyticsModules(soap, a);
	for (soap_flag = 0;; soap_flag = 1)
	{	soap->error = SOAP_TAG_MISMATCH;
	if (soap_flag_tan__ModifyAnalyticsModules && soap->error == SOAP_TAG_MISMATCH)
		if (soap_in_PointerTo_tan__ModifyAnalyticsModules(soap, "tan:ModifyAnalyticsModules", &a->tan__ModifyAnalyticsModules, ""))
		{	soap_flag_tan__ModifyAnalyticsModules--;
	continue;
	}
	if (soap->error == SOAP_TAG_MISMATCH)
		if (soap_flag)
		{	soap->error = SOAP_OK;
	break;
	}
	if (soap_flag && soap->error == SOAP_NO_TAG)
		break;
	if (soap->error)
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put___daae__ModifyAnalyticsModules(struct soap *soap, const struct __daae__ModifyAnalyticsModules *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___daae__ModifyAnalyticsModules(soap, tag?tag:"-daae:ModifyAnalyticsModules", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __daae__ModifyAnalyticsModules * SOAP_FMAC4 soap_get___daae__ModifyAnalyticsModules(struct soap *soap, struct __daae__ModifyAnalyticsModules *p, const char *tag, const char *type)
{
	if ((p = soap_in___daae__ModifyAnalyticsModules(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___daae__GetAnalyticsModules(struct soap *soap, struct __daae__GetAnalyticsModules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->tan__GetAnalyticsModules = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___daae__GetAnalyticsModules(struct soap *soap, const struct __daae__GetAnalyticsModules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_tan__GetAnalyticsModules(soap, &a->tan__GetAnalyticsModules);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___daae__GetAnalyticsModules(struct soap *soap, const char *tag, int id, const struct __daae__GetAnalyticsModules *a, const char *type)
{
	if (soap_out_PointerTo_tan__GetAnalyticsModules(soap, "tan:GetAnalyticsModules", -1, &a->tan__GetAnalyticsModules, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __daae__GetAnalyticsModules * SOAP_FMAC4 soap_in___daae__GetAnalyticsModules(struct soap *soap, const char *tag, struct __daae__GetAnalyticsModules *a, const char *type)
{
	size_t soap_flag_tan__GetAnalyticsModules = 1;
	short soap_flag;
	a = (struct __daae__GetAnalyticsModules *)soap_id_enter(soap, "", a, SOAP_TYPE___daae__GetAnalyticsModules, sizeof(struct __daae__GetAnalyticsModules), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___daae__GetAnalyticsModules(soap, a);
	for (soap_flag = 0;; soap_flag = 1)
	{	soap->error = SOAP_TAG_MISMATCH;
	if (soap_flag_tan__GetAnalyticsModules && soap->error == SOAP_TAG_MISMATCH)
		if (soap_in_PointerTo_tan__GetAnalyticsModules(soap, "tan:GetAnalyticsModules", &a->tan__GetAnalyticsModules, ""))
		{	soap_flag_tan__GetAnalyticsModules--;
	continue;
	}
	if (soap->error == SOAP_TAG_MISMATCH)
		if (soap_flag)
		{	soap->error = SOAP_OK;
	break;
	}
	if (soap_flag && soap->error == SOAP_NO_TAG)
		break;
	if (soap->error)
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put___daae__GetAnalyticsModules(struct soap *soap, const struct __daae__GetAnalyticsModules *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___daae__GetAnalyticsModules(soap, tag?tag:"-daae:GetAnalyticsModules", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __daae__GetAnalyticsModules * SOAP_FMAC4 soap_get___daae__GetAnalyticsModules(struct soap *soap, struct __daae__GetAnalyticsModules *p, const char *tag, const char *type)
{
	if ((p = soap_in___daae__GetAnalyticsModules(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___daae__DeleteAnalyticsModules(struct soap *soap, struct __daae__DeleteAnalyticsModules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->tan__DeleteAnalyticsModules = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___daae__DeleteAnalyticsModules(struct soap *soap, const struct __daae__DeleteAnalyticsModules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_tan__DeleteAnalyticsModules(soap, &a->tan__DeleteAnalyticsModules);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___daae__DeleteAnalyticsModules(struct soap *soap, const char *tag, int id, const struct __daae__DeleteAnalyticsModules *a, const char *type)
{
	if (soap_out_PointerTo_tan__DeleteAnalyticsModules(soap, "tan:DeleteAnalyticsModules", -1, &a->tan__DeleteAnalyticsModules, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __daae__DeleteAnalyticsModules * SOAP_FMAC4 soap_in___daae__DeleteAnalyticsModules(struct soap *soap, const char *tag, struct __daae__DeleteAnalyticsModules *a, const char *type)
{
	size_t soap_flag_tan__DeleteAnalyticsModules = 1;
	short soap_flag;
	a = (struct __daae__DeleteAnalyticsModules *)soap_id_enter(soap, "", a, SOAP_TYPE___daae__DeleteAnalyticsModules, sizeof(struct __daae__DeleteAnalyticsModules), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___daae__DeleteAnalyticsModules(soap, a);
	for (soap_flag = 0;; soap_flag = 1)
	{	soap->error = SOAP_TAG_MISMATCH;
	if (soap_flag_tan__DeleteAnalyticsModules && soap->error == SOAP_TAG_MISMATCH)
		if (soap_in_PointerTo_tan__DeleteAnalyticsModules(soap, "tan:DeleteAnalyticsModules", &a->tan__DeleteAnalyticsModules, ""))
		{	soap_flag_tan__DeleteAnalyticsModules--;
	continue;
	}
	if (soap->error == SOAP_TAG_MISMATCH)
		if (soap_flag)
		{	soap->error = SOAP_OK;
	break;
	}
	if (soap_flag && soap->error == SOAP_NO_TAG)
		break;
	if (soap->error)
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put___daae__DeleteAnalyticsModules(struct soap *soap, const struct __daae__DeleteAnalyticsModules *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___daae__DeleteAnalyticsModules(soap, tag?tag:"-daae:DeleteAnalyticsModules", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __daae__DeleteAnalyticsModules * SOAP_FMAC4 soap_get___daae__DeleteAnalyticsModules(struct soap *soap, struct __daae__DeleteAnalyticsModules *p, const char *tag, const char *type)
{
	if ((p = soap_in___daae__DeleteAnalyticsModules(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___daae__CreateAnalyticsModules(struct soap *soap, struct __daae__CreateAnalyticsModules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->tan__CreateAnalyticsModules = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___daae__CreateAnalyticsModules(struct soap *soap, const struct __daae__CreateAnalyticsModules *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_tan__CreateAnalyticsModules(soap, &a->tan__CreateAnalyticsModules);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___daae__CreateAnalyticsModules(struct soap *soap, const char *tag, int id, const struct __daae__CreateAnalyticsModules *a, const char *type)
{
	if (soap_out_PointerTo_tan__CreateAnalyticsModules(soap, "tan:CreateAnalyticsModules", -1, &a->tan__CreateAnalyticsModules, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __daae__CreateAnalyticsModules * SOAP_FMAC4 soap_in___daae__CreateAnalyticsModules(struct soap *soap, const char *tag, struct __daae__CreateAnalyticsModules *a, const char *type)
{
	size_t soap_flag_tan__CreateAnalyticsModules = 1;
	short soap_flag;
	a = (struct __daae__CreateAnalyticsModules *)soap_id_enter(soap, "", a, SOAP_TYPE___daae__CreateAnalyticsModules, sizeof(struct __daae__CreateAnalyticsModules), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___daae__CreateAnalyticsModules(soap, a);
	for (soap_flag = 0;; soap_flag = 1)
	{	soap->error = SOAP_TAG_MISMATCH;
	if (soap_flag_tan__CreateAnalyticsModules && soap->error == SOAP_TAG_MISMATCH)
		if (soap_in_PointerTo_tan__CreateAnalyticsModules(soap, "tan:CreateAnalyticsModules", &a->tan__CreateAnalyticsModules, ""))
		{	soap_flag_tan__CreateAnalyticsModules--;
	continue;
	}
	if (soap->error == SOAP_TAG_MISMATCH)
		if (soap_flag)
		{	soap->error = SOAP_OK;
	break;
	}
	if (soap_flag && soap->error == SOAP_NO_TAG)
		break;
	if (soap->error)
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put___daae__CreateAnalyticsModules(struct soap *soap, const struct __daae__CreateAnalyticsModules *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___daae__CreateAnalyticsModules(soap, tag?tag:"-daae:CreateAnalyticsModules", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __daae__CreateAnalyticsModules * SOAP_FMAC4 soap_get___daae__CreateAnalyticsModules(struct soap *soap, struct __daae__CreateAnalyticsModules *p, const char *tag, const char *type)
{
	if ((p = soap_in___daae__CreateAnalyticsModules(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////


SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__SupportedAnalyticsModulesExtension(struct soap *soap, struct tt__SupportedAnalyticsModulesExtension *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__SupportedAnalyticsModulesExtension))
		soap_serialize_tt__SupportedAnalyticsModulesExtension(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__SupportedAnalyticsModulesExtension(struct soap *soap, const char *tag, int id, struct tt__SupportedAnalyticsModulesExtension *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__SupportedAnalyticsModulesExtension);
	if (id < 0)
		return soap->error;
	return soap_out_tt__SupportedAnalyticsModulesExtension(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__SupportedAnalyticsModulesExtension ** SOAP_FMAC4 soap_in_PointerTott__SupportedAnalyticsModulesExtension(struct soap *soap, const char *tag, struct tt__SupportedAnalyticsModulesExtension **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__SupportedAnalyticsModulesExtension **)soap_malloc(soap, sizeof(struct tt__SupportedAnalyticsModulesExtension *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in_tt__SupportedAnalyticsModulesExtension(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct tt__SupportedAnalyticsModulesExtension **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__SupportedAnalyticsModulesExtension, sizeof(struct tt__SupportedAnalyticsModulesExtension), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__SupportedAnalyticsModulesExtension(struct soap *soap, struct tt__SupportedAnalyticsModulesExtension *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__SupportedAnalyticsModulesExtension);
	if (soap_out_PointerTott__SupportedAnalyticsModulesExtension(soap, tag?tag:"tt:SupportedAnalyticsModulesExtension", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__SupportedAnalyticsModulesExtension ** SOAP_FMAC4 soap_get_PointerTott__SupportedAnalyticsModulesExtension(struct soap *soap, struct tt__SupportedAnalyticsModulesExtension **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__SupportedAnalyticsModulesExtension(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__SupportedRulesExtension(struct soap *soap, struct tt__SupportedRulesExtension *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__SupportedRulesExtension))
		soap_serialize_tt__SupportedRulesExtension(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__SupportedRulesExtension(struct soap *soap, const char *tag, int id, struct tt__SupportedRulesExtension *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__SupportedRulesExtension);
	if (id < 0)
		return soap->error;
	return soap_out_tt__SupportedRulesExtension(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__SupportedRulesExtension ** SOAP_FMAC4 soap_in_PointerTott__SupportedRulesExtension(struct soap *soap, const char *tag, struct tt__SupportedRulesExtension **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__SupportedRulesExtension **)soap_malloc(soap, sizeof(struct tt__SupportedRulesExtension *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in_tt__SupportedRulesExtension(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct tt__SupportedRulesExtension **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__SupportedRulesExtension, sizeof(struct tt__SupportedRulesExtension), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__SupportedRulesExtension(struct soap *soap, struct tt__SupportedRulesExtension *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__SupportedRulesExtension);
	if (soap_out_PointerTott__SupportedRulesExtension(soap, tag?tag:"tt:SupportedRulesExtension", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__SupportedRulesExtension ** SOAP_FMAC4 soap_get_PointerTott__SupportedRulesExtension(struct soap *soap, struct tt__SupportedRulesExtension **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__SupportedRulesExtension(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}






SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__SupportedAnalyticsModulesExtension(struct soap *soap, struct tt__SupportedAnalyticsModulesExtension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__size = 0;
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__SupportedAnalyticsModulesExtension(struct soap *soap, const struct tt__SupportedAnalyticsModulesExtension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__SupportedAnalyticsModulesExtension(struct soap *soap, const char *tag, int id, const struct tt__SupportedAnalyticsModulesExtension *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__SupportedAnalyticsModulesExtension), type))
		return soap->error;
	if (a->__any)
	{	int i;
	for (i = 0; i < a->__size; i++)
		soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__SupportedAnalyticsModulesExtension * SOAP_FMAC4 soap_in_tt__SupportedAnalyticsModulesExtension(struct soap *soap, const char *tag, struct tt__SupportedAnalyticsModulesExtension *a, const char *type)
{
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__SupportedAnalyticsModulesExtension *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__SupportedAnalyticsModulesExtension, sizeof(struct tt__SupportedAnalyticsModulesExtension), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__SupportedAnalyticsModulesExtension(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap->error == SOAP_TAG_MISMATCH && !soap_peek_element(soap))
		{	if (a->__any == NULL)
		{	if (soap_blist___any == NULL)
		soap_blist___any = soap_new_block(soap);
		a->__any = (char **)soap_push_block(soap, soap_blist___any, sizeof(char *));
		if (a->__any == NULL)
			return NULL;
		*a->__any = NULL;
		}
		if (soap_inliteral(soap, "-any", a->__any))
		{	a->__size++;
		a->__any = NULL;
		continue;
		}
		}
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (a->__any)
			soap_pop_block(soap, soap_blist___any);
		if (a->__size)
			a->__any = (char **)soap_save_block(soap, soap_blist___any, NULL, 1);
		else
		{	a->__any = NULL;
		if (soap_blist___any)
			soap_end_block(soap, soap_blist___any);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct tt__SupportedAnalyticsModulesExtension *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__SupportedAnalyticsModulesExtension, 0, sizeof(struct tt__SupportedAnalyticsModulesExtension), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__SupportedAnalyticsModulesExtension(struct soap *soap, const struct tt__SupportedAnalyticsModulesExtension *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__SupportedAnalyticsModulesExtension);
	if (soap_out_tt__SupportedAnalyticsModulesExtension(soap, tag?tag:"tt:SupportedAnalyticsModulesExtension", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__SupportedAnalyticsModulesExtension * SOAP_FMAC4 soap_get_tt__SupportedAnalyticsModulesExtension(struct soap *soap, struct tt__SupportedAnalyticsModulesExtension *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__SupportedAnalyticsModulesExtension(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__SupportedRulesExtension(struct soap *soap, struct tt__SupportedRulesExtension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__size = 0;
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__SupportedRulesExtension(struct soap *soap, const struct tt__SupportedRulesExtension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__SupportedRulesExtension(struct soap *soap, const char *tag, int id, const struct tt__SupportedRulesExtension *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__SupportedRulesExtension), type))
		return soap->error;
	if (a->__any)
	{	int i;
	for (i = 0; i < a->__size; i++)
		soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__SupportedRulesExtension * SOAP_FMAC4 soap_in_tt__SupportedRulesExtension(struct soap *soap, const char *tag, struct tt__SupportedRulesExtension *a, const char *type)
{
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__SupportedRulesExtension *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__SupportedRulesExtension, sizeof(struct tt__SupportedRulesExtension), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__SupportedRulesExtension(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap->error == SOAP_TAG_MISMATCH && !soap_peek_element(soap))
		{	if (a->__any == NULL)
		{	if (soap_blist___any == NULL)
		soap_blist___any = soap_new_block(soap);
		a->__any = (char **)soap_push_block(soap, soap_blist___any, sizeof(char *));
		if (a->__any == NULL)
			return NULL;
		*a->__any = NULL;
		}
		if (soap_inliteral(soap, "-any", a->__any))
		{	a->__size++;
		a->__any = NULL;
		continue;
		}
		}
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (a->__any)
			soap_pop_block(soap, soap_blist___any);
		if (a->__size)
			a->__any = (char **)soap_save_block(soap, soap_blist___any, NULL, 1);
		else
		{	a->__any = NULL;
		if (soap_blist___any)
			soap_end_block(soap, soap_blist___any);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct tt__SupportedRulesExtension *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__SupportedRulesExtension, 0, sizeof(struct tt__SupportedRulesExtension), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__SupportedRulesExtension(struct soap *soap, const struct tt__SupportedRulesExtension *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__SupportedRulesExtension);
	if (soap_out_tt__SupportedRulesExtension(soap, tag?tag:"tt:SupportedRulesExtension", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__SupportedRulesExtension * SOAP_FMAC4 soap_get_tt__SupportedRulesExtension(struct soap *soap, struct tt__SupportedRulesExtension *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__SupportedRulesExtension(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}


#ifdef __cplusplus
}
#endif
