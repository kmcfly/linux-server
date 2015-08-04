#if defined(__BORLANDC__)
#pragma option push -w-8060
#pragma option push -w-8004
#endif

#include "OnvifPtzH.h"
//#include "soapH.h"

#ifdef __cplusplus
extern "C" 
{
#endif


SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__GetPresets(struct soap *soap, struct _tptz__GetPresets *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->ProfileToken);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__GetPresets(struct soap *soap, const struct _tptz__GetPresets *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->ProfileToken);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__GetPresets(struct soap *soap, const char *tag, int id, const struct _tptz__GetPresets *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__GetPresets), type))
		return soap->error;
	if (a->ProfileToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tptz:ProfileToken", -1, &a->ProfileToken, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:ProfileToken"))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__GetPresets * SOAP_FMAC4 soap_in__tptz__GetPresets(struct soap *soap, const char *tag, struct _tptz__GetPresets *a, const char *type)
{
	size_t soap_flag_ProfileToken = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__GetPresets *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__GetPresets, sizeof(struct _tptz__GetPresets), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__GetPresets(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_ProfileToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_tt__ReferenceToken(soap, "tptz:ProfileToken", &a->ProfileToken, "tt:ReferenceToken"))
			{	soap_flag_ProfileToken--;
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
	{	a = (struct _tptz__GetPresets *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__GetPresets, 0, sizeof(struct _tptz__GetPresets), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_ProfileToken > 0))
	{	soap->error = SOAP_OCCURS;
	return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__GetPresets(struct soap *soap, const struct _tptz__GetPresets *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__GetPresets);
	if (soap_out__tptz__GetPresets(soap, tag?tag:"tptz:GetPresets", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__GetPresets * SOAP_FMAC4 soap_get__tptz__GetPresets(struct soap *soap, struct _tptz__GetPresets *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__GetPresets(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tptz__GetPresets(struct soap *soap, struct _tptz__GetPresets *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tptz__GetPresets))
		soap_serialize__tptz__GetPresets(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tptz__GetPresets(struct soap *soap, const char *tag, int id, struct _tptz__GetPresets *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tptz__GetPresets);
	if (id < 0)
		return soap->error;
	return soap_out__tptz__GetPresets(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tptz__GetPresets ** SOAP_FMAC4 soap_in_PointerTo_tptz__GetPresets(struct soap *soap, const char *tag, struct _tptz__GetPresets **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tptz__GetPresets **)soap_malloc(soap, sizeof(struct _tptz__GetPresets *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tptz__GetPresets(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tptz__GetPresets **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tptz__GetPresets, sizeof(struct _tptz__GetPresets), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tptz__GetPresets(struct soap *soap, struct _tptz__GetPresets *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tptz__GetPresets);
	if (soap_out_PointerTo_tptz__GetPresets(soap, tag?tag:"tptz:GetPresets", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__GetPresets ** SOAP_FMAC4 soap_get_PointerTo_tptz__GetPresets(struct soap *soap, struct _tptz__GetPresets **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tptz__GetPresets(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___tptz__GetPresets(struct soap *soap, struct __tptz__GetPresets *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->tptz__GetPresets = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___tptz__GetPresets(struct soap *soap, const struct __tptz__GetPresets *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_tptz__GetPresets(soap, &a->tptz__GetPresets);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___tptz__GetPresets(struct soap *soap, const char *tag, int id, const struct __tptz__GetPresets *a, const char *type)
{
	if (soap_out_PointerTo_tptz__GetPresets(soap, "tptz:GetPresets", -1, &a->tptz__GetPresets, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__GetPresets * SOAP_FMAC4 soap_in___tptz__GetPresets(struct soap *soap, const char *tag, struct __tptz__GetPresets *a, const char *type)
{
	size_t soap_flag_tptz__GetPresets = 1;
	short soap_flag;
	a = (struct __tptz__GetPresets *)soap_id_enter(soap, "", a, SOAP_TYPE___tptz__GetPresets, sizeof(struct __tptz__GetPresets), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___tptz__GetPresets(soap, a);
	for (soap_flag = 0;; soap_flag = 1)
	{	soap->error = SOAP_TAG_MISMATCH;
	if (soap_flag_tptz__GetPresets && soap->error == SOAP_TAG_MISMATCH)
		if (soap_in_PointerTo_tptz__GetPresets(soap, "tptz:GetPresets", &a->tptz__GetPresets, ""))
		{	soap_flag_tptz__GetPresets--;
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

SOAP_FMAC3 int SOAP_FMAC4 soap_put___tptz__GetPresets(struct soap *soap, const struct __tptz__GetPresets *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___tptz__GetPresets(soap, tag?tag:"-tptz:GetPresets", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__GetPresets * SOAP_FMAC4 soap_get___tptz__GetPresets(struct soap *soap, struct __tptz__GetPresets *p, const char *tag, const char *type)
{
	if ((p = soap_in___tptz__GetPresets(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}




SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__PTZPreset(struct soap *soap, struct tt__PTZPreset *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__Name(soap, &a->Name);
	a->PTZPosition = NULL;
	soap_default_tt__ReferenceToken(soap, &a->token);
	a->__anyAttribute = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__PTZPreset(struct soap *soap, const struct tt__PTZPreset *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__Name(soap, &a->Name);
	soap_serialize_PointerTott__PTZVector(soap, &a->PTZPosition);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__PTZPreset(struct soap *soap, const char *tag, int id, const struct tt__PTZPreset *a, const char *type)
{
	if (a->token)
		soap_set_attr(soap, "token", a->token, 1);
	if (a->__anyAttribute)
		soap_set_attr(soap, "-anyAttribute", a->__anyAttribute, 1);
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__PTZPreset), type))
		return soap->error;
	if (soap_out_tt__Name(soap, "tt:Name", -1, &a->Name, ""))
		return soap->error;
	if (soap_out_PointerTott__PTZVector(soap, "tt:PTZPosition", -1, &a->PTZPosition, ""))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__PTZPreset * SOAP_FMAC4 soap_in_tt__PTZPreset(struct soap *soap, const char *tag, struct tt__PTZPreset *a, const char *type)
{
	size_t soap_flag_Name = 1;
	size_t soap_flag_PTZPosition = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__PTZPreset *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__PTZPreset, sizeof(struct tt__PTZPreset), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__PTZPreset(soap, a);
	if (soap_s2string(soap, soap_attr_value(soap, "token", 0), &a->token, 0, 64))
		return NULL;
	if (soap_s2string(soap, soap_attr_value(soap, "-anyAttribute", 0), &a->__anyAttribute, 0, -1))
		return NULL;
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_Name && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_tt__Name(soap, "tt:Name", &a->Name, "tt:Name"))
			{	soap_flag_Name--;
		continue;
		}
		if (soap_flag_PTZPosition && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_PointerTott__PTZVector(soap, "tt:PTZPosition", &a->PTZPosition, "tt:PTZVector"))
			{	soap_flag_PTZPosition--;
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
	{	a = (struct tt__PTZPreset *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__PTZPreset, 0, sizeof(struct tt__PTZPreset), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__PTZPreset(struct soap *soap, const struct tt__PTZPreset *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__PTZPreset);
	if (soap_out_tt__PTZPreset(soap, tag?tag:"tt:PTZPreset", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPreset * SOAP_FMAC4 soap_get_tt__PTZPreset(struct soap *soap, struct tt__PTZPreset *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__PTZPreset(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__GetPresetsResponse(struct soap *soap, struct _tptz__GetPresetsResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__sizePreset = 0;
	a->Preset = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__GetPresetsResponse(struct soap *soap, const struct _tptz__GetPresetsResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	if (a->Preset)
	{	
		int i;
		for (i = 0; i < a->__sizePreset; i++)
		{
			soap_embedded(soap, a->Preset + i, SOAP_TYPE_tt__PTZPreset);
			soap_serialize_tt__PTZPreset(soap, a->Preset + i);
		}
	}
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__GetPresetsResponse(struct soap *soap, const char *tag, int id, const struct _tptz__GetPresetsResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__GetPresetsResponse), type))
		return soap->error;
	soap_element_result(soap, "-sizePreset");
	if (a->Preset)
	{	int i;
	for (i = 0; i < a->__sizePreset; i++)
		if (soap_out_tt__PTZPreset(soap, "tptz:Preset", -1, a->Preset + i, ""))
			return soap->error;
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__GetPresetsResponse * SOAP_FMAC4 soap_in__tptz__GetPresetsResponse(struct soap *soap, const char *tag, struct _tptz__GetPresetsResponse *a, const char *type)
{
	struct soap_blist *soap_blist_Preset = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__GetPresetsResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__GetPresetsResponse, sizeof(struct _tptz__GetPresetsResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__GetPresetsResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tptz:Preset", 1, NULL))
		{	if (a->Preset == NULL)
		{	if (soap_blist_Preset == NULL)
		soap_blist_Preset = soap_new_block(soap);
		a->Preset = (struct tt__PTZPreset *)soap_push_block(soap, soap_blist_Preset, sizeof(struct tt__PTZPreset));
		if (a->Preset == NULL)
			return NULL;
		soap_default_tt__PTZPreset(soap, a->Preset);
		}
		soap_revert(soap);
		if (soap_in_tt__PTZPreset(soap, "tptz:Preset", a->Preset, "tt:PTZPreset"))
		{	a->__sizePreset++;
		a->Preset = NULL;
		continue;
		}
		}
		soap_check_result(soap, "-sizePreset");
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (a->Preset)
			soap_pop_block(soap, soap_blist_Preset);
		if (a->__sizePreset)
			a->Preset = (struct tt__PTZPreset *)soap_save_block(soap, soap_blist_Preset, NULL, 1);
		else
		{	a->Preset = NULL;
		if (soap_blist_Preset)
			soap_end_block(soap, soap_blist_Preset);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _tptz__GetPresetsResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__GetPresetsResponse, 0, sizeof(struct _tptz__GetPresetsResponse), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__GetPresetsResponse(struct soap *soap, const struct _tptz__GetPresetsResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__GetPresetsResponse);
	if (soap_out__tptz__GetPresetsResponse(soap, tag?tag:"tptz:GetPresetsResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__GetPresetsResponse * SOAP_FMAC4 soap_get__tptz__GetPresetsResponse(struct soap *soap, struct _tptz__GetPresetsResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__GetPresetsResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__SetPreset(struct soap *soap, struct _tptz__SetPreset *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->ProfileToken);
	soap_default_string(soap, &a->PresetName);
	soap_default_tt__ReferenceToken(soap, &a->PresetToken);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__SetPreset(struct soap *soap, const struct _tptz__SetPreset *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->ProfileToken);
	soap_serialize_string(soap, &a->PresetName);
	soap_serialize_tt__ReferenceToken(soap, &a->PresetToken);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__SetPreset(struct soap *soap, const char *tag, int id, const struct _tptz__SetPreset *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__SetPreset), type))
		return soap->error;
	if (a->ProfileToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tptz:ProfileToken", -1, &a->ProfileToken, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:ProfileToken"))
		return soap->error;
	if (soap_out_string(soap, "tptz:PresetName", -1, &a->PresetName, ""))
		return soap->error;
	if (soap_out_tt__ReferenceToken(soap, "tptz:PresetToken", -1, &a->PresetToken, ""))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__SetPreset * SOAP_FMAC4 soap_in__tptz__SetPreset(struct soap *soap, const char *tag, struct _tptz__SetPreset *a, const char *type)
{
	size_t soap_flag_ProfileToken = 1;
	size_t soap_flag_PresetName = 1;
	size_t soap_flag_PresetToken = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__SetPreset *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__SetPreset, sizeof(struct _tptz__SetPreset), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__SetPreset(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_ProfileToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_tt__ReferenceToken(soap, "tptz:ProfileToken", &a->ProfileToken, "tt:ReferenceToken"))
			{	soap_flag_ProfileToken--;
		continue;
		}
		if (soap_flag_PresetName && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_string(soap, "tptz:PresetName", &a->PresetName, "xsd:string"))
			{	soap_flag_PresetName--;
		continue;
		}
		if (soap_flag_PresetToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_tt__ReferenceToken(soap, "tptz:PresetToken", &a->PresetToken, "tt:ReferenceToken"))
			{	soap_flag_PresetToken--;
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
	{	a = (struct _tptz__SetPreset *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__SetPreset, 0, sizeof(struct _tptz__SetPreset), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_ProfileToken > 0))
	{	soap->error = SOAP_OCCURS;
	return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__SetPreset(struct soap *soap, const struct _tptz__SetPreset *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__SetPreset);
	if (soap_out__tptz__SetPreset(soap, tag?tag:"tptz:SetPreset", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__SetPreset * SOAP_FMAC4 soap_get__tptz__SetPreset(struct soap *soap, struct _tptz__SetPreset *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__SetPreset(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tptz__SetPreset(struct soap *soap, struct _tptz__SetPreset *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tptz__SetPreset))
		soap_serialize__tptz__SetPreset(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tptz__SetPreset(struct soap *soap, const char *tag, int id, struct _tptz__SetPreset *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tptz__SetPreset);
	if (id < 0)
		return soap->error;
	return soap_out__tptz__SetPreset(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tptz__SetPreset ** SOAP_FMAC4 soap_in_PointerTo_tptz__SetPreset(struct soap *soap, const char *tag, struct _tptz__SetPreset **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tptz__SetPreset **)soap_malloc(soap, sizeof(struct _tptz__SetPreset *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tptz__SetPreset(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tptz__SetPreset **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tptz__SetPreset, sizeof(struct _tptz__SetPreset), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tptz__SetPreset(struct soap *soap, struct _tptz__SetPreset *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tptz__SetPreset);
	if (soap_out_PointerTo_tptz__SetPreset(soap, tag?tag:"tptz:SetPreset", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__SetPreset ** SOAP_FMAC4 soap_get_PointerTo_tptz__SetPreset(struct soap *soap, struct _tptz__SetPreset **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tptz__SetPreset(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___tptz__SetPreset(struct soap *soap, struct __tptz__SetPreset *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->tptz__SetPreset = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___tptz__SetPreset(struct soap *soap, const struct __tptz__SetPreset *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_tptz__SetPreset(soap, &a->tptz__SetPreset);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___tptz__SetPreset(struct soap *soap, const char *tag, int id, const struct __tptz__SetPreset *a, const char *type)
{
	if (soap_out_PointerTo_tptz__SetPreset(soap, "tptz:SetPreset", -1, &a->tptz__SetPreset, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__SetPreset * SOAP_FMAC4 soap_in___tptz__SetPreset(struct soap *soap, const char *tag, struct __tptz__SetPreset *a, const char *type)
{
	size_t soap_flag_tptz__SetPreset = 1;
	short soap_flag;
	a = (struct __tptz__SetPreset *)soap_id_enter(soap, "", a, SOAP_TYPE___tptz__SetPreset, sizeof(struct __tptz__SetPreset), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___tptz__SetPreset(soap, a);
	for (soap_flag = 0;; soap_flag = 1)
	{	soap->error = SOAP_TAG_MISMATCH;
	if (soap_flag_tptz__SetPreset && soap->error == SOAP_TAG_MISMATCH)
		if (soap_in_PointerTo_tptz__SetPreset(soap, "tptz:SetPreset", &a->tptz__SetPreset, ""))
		{	soap_flag_tptz__SetPreset--;
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

SOAP_FMAC3 int SOAP_FMAC4 soap_put___tptz__SetPreset(struct soap *soap, const struct __tptz__SetPreset *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___tptz__SetPreset(soap, tag?tag:"-tptz:SetPreset", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__SetPreset * SOAP_FMAC4 soap_get___tptz__SetPreset(struct soap *soap, struct __tptz__SetPreset *p, const char *tag, const char *type)
{
	if ((p = soap_in___tptz__SetPreset(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__SetPresetResponse(struct soap *soap, struct _tptz__SetPresetResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->PresetToken);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__SetPresetResponse(struct soap *soap, const struct _tptz__SetPresetResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->PresetToken);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__SetPresetResponse(struct soap *soap, const char *tag, int id, const struct _tptz__SetPresetResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__SetPresetResponse), type))
		return soap->error;
	if (a->PresetToken)
		soap_element_result(soap, "tptz:PresetToken");
	if (a->PresetToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tptz:PresetToken", -1, &a->PresetToken, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:PresetToken"))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__SetPresetResponse * SOAP_FMAC4 soap_in__tptz__SetPresetResponse(struct soap *soap, const char *tag, struct _tptz__SetPresetResponse *a, const char *type)
{
	size_t soap_flag_PresetToken = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__SetPresetResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__SetPresetResponse, sizeof(struct _tptz__SetPresetResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__SetPresetResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_PresetToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_tt__ReferenceToken(soap, "tptz:PresetToken", &a->PresetToken, "tt:ReferenceToken"))
			{	soap_flag_PresetToken--;
		continue;
		}
		soap_check_result(soap, "tptz:PresetToken");
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
	{	a = (struct _tptz__SetPresetResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__SetPresetResponse, 0, sizeof(struct _tptz__SetPresetResponse), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_PresetToken > 0))
	{	soap->error = SOAP_OCCURS;
	return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__SetPresetResponse(struct soap *soap, const struct _tptz__SetPresetResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__SetPresetResponse);
	if (soap_out__tptz__SetPresetResponse(soap, tag?tag:"tptz:SetPresetResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__SetPresetResponse * SOAP_FMAC4 soap_get__tptz__SetPresetResponse(struct soap *soap, struct _tptz__SetPresetResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__SetPresetResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__RemovePreset(struct soap *soap, struct _tptz__RemovePreset *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->ProfileToken);
	soap_default_tt__ReferenceToken(soap, &a->PresetToken);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__RemovePreset(struct soap *soap, const struct _tptz__RemovePreset *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->ProfileToken);
	soap_serialize_tt__ReferenceToken(soap, &a->PresetToken);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__RemovePreset(struct soap *soap, const char *tag, int id, const struct _tptz__RemovePreset *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__RemovePreset), type))
		return soap->error;
	if (a->ProfileToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tptz:ProfileToken", -1, &a->ProfileToken, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:ProfileToken"))
		return soap->error;
	if (a->PresetToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tptz:PresetToken", -1, &a->PresetToken, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:PresetToken"))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__RemovePreset * SOAP_FMAC4 soap_in__tptz__RemovePreset(struct soap *soap, const char *tag, struct _tptz__RemovePreset *a, const char *type)
{
	size_t soap_flag_ProfileToken = 1;
	size_t soap_flag_PresetToken = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__RemovePreset *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__RemovePreset, sizeof(struct _tptz__RemovePreset), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__RemovePreset(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_ProfileToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_tt__ReferenceToken(soap, "tptz:ProfileToken", &a->ProfileToken, "tt:ReferenceToken"))
			{	soap_flag_ProfileToken--;
		continue;
		}
		if (soap_flag_PresetToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_tt__ReferenceToken(soap, "tptz:PresetToken", &a->PresetToken, "tt:ReferenceToken"))
			{	soap_flag_PresetToken--;
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
	{	a = (struct _tptz__RemovePreset *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__RemovePreset, 0, sizeof(struct _tptz__RemovePreset), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_ProfileToken > 0 || soap_flag_PresetToken > 0))
	{	soap->error = SOAP_OCCURS;
	return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__RemovePreset(struct soap *soap, const struct _tptz__RemovePreset *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__RemovePreset);
	if (soap_out__tptz__RemovePreset(soap, tag?tag:"tptz:RemovePreset", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__RemovePreset * SOAP_FMAC4 soap_get__tptz__RemovePreset(struct soap *soap, struct _tptz__RemovePreset *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__RemovePreset(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tptz__RemovePreset(struct soap *soap, struct _tptz__RemovePreset *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tptz__RemovePreset))
		soap_serialize__tptz__RemovePreset(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tptz__RemovePreset(struct soap *soap, const char *tag, int id, struct _tptz__RemovePreset *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tptz__RemovePreset);
	if (id < 0)
		return soap->error;
	return soap_out__tptz__RemovePreset(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tptz__RemovePreset ** SOAP_FMAC4 soap_in_PointerTo_tptz__RemovePreset(struct soap *soap, const char *tag, struct _tptz__RemovePreset **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tptz__RemovePreset **)soap_malloc(soap, sizeof(struct _tptz__RemovePreset *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tptz__RemovePreset(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tptz__RemovePreset **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tptz__RemovePreset, sizeof(struct _tptz__RemovePreset), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tptz__RemovePreset(struct soap *soap, struct _tptz__RemovePreset *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tptz__RemovePreset);
	if (soap_out_PointerTo_tptz__RemovePreset(soap, tag?tag:"tptz:RemovePreset", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__RemovePreset ** SOAP_FMAC4 soap_get_PointerTo_tptz__RemovePreset(struct soap *soap, struct _tptz__RemovePreset **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tptz__RemovePreset(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___tptz__RemovePreset(struct soap *soap, struct __tptz__RemovePreset *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->tptz__RemovePreset = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___tptz__RemovePreset(struct soap *soap, const struct __tptz__RemovePreset *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_tptz__RemovePreset(soap, &a->tptz__RemovePreset);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___tptz__RemovePreset(struct soap *soap, const char *tag, int id, const struct __tptz__RemovePreset *a, const char *type)
{
	if (soap_out_PointerTo_tptz__RemovePreset(soap, "tptz:RemovePreset", -1, &a->tptz__RemovePreset, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__RemovePreset * SOAP_FMAC4 soap_in___tptz__RemovePreset(struct soap *soap, const char *tag, struct __tptz__RemovePreset *a, const char *type)
{
	size_t soap_flag_tptz__RemovePreset = 1;
	short soap_flag;
	a = (struct __tptz__RemovePreset *)soap_id_enter(soap, "", a, SOAP_TYPE___tptz__RemovePreset, sizeof(struct __tptz__RemovePreset), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___tptz__RemovePreset(soap, a);
	for (soap_flag = 0;; soap_flag = 1)
	{	soap->error = SOAP_TAG_MISMATCH;
	if (soap_flag_tptz__RemovePreset && soap->error == SOAP_TAG_MISMATCH)
		if (soap_in_PointerTo_tptz__RemovePreset(soap, "tptz:RemovePreset", &a->tptz__RemovePreset, ""))
		{	soap_flag_tptz__RemovePreset--;
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

SOAP_FMAC3 int SOAP_FMAC4 soap_put___tptz__RemovePreset(struct soap *soap, const struct __tptz__RemovePreset *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___tptz__RemovePreset(soap, tag?tag:"-tptz:RemovePreset", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__RemovePreset * SOAP_FMAC4 soap_get___tptz__RemovePreset(struct soap *soap, struct __tptz__RemovePreset *p, const char *tag, const char *type)
{
	if ((p = soap_in___tptz__RemovePreset(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}


SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__RemovePresetResponse(struct soap *soap, struct _tptz__RemovePresetResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__RemovePresetResponse(struct soap *soap, const struct _tptz__RemovePresetResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__RemovePresetResponse(struct soap *soap, const char *tag, int id, const struct _tptz__RemovePresetResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__RemovePresetResponse), type))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__RemovePresetResponse * SOAP_FMAC4 soap_in__tptz__RemovePresetResponse(struct soap *soap, const char *tag, struct _tptz__RemovePresetResponse *a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__RemovePresetResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__RemovePresetResponse, sizeof(struct _tptz__RemovePresetResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__RemovePresetResponse(soap, a);
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
	{	a = (struct _tptz__RemovePresetResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__RemovePresetResponse, 0, sizeof(struct _tptz__RemovePresetResponse), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__RemovePresetResponse(struct soap *soap, const struct _tptz__RemovePresetResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__RemovePresetResponse);
	if (soap_out__tptz__RemovePresetResponse(soap, tag?tag:"tptz:RemovePresetResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__RemovePresetResponse * SOAP_FMAC4 soap_get__tptz__RemovePresetResponse(struct soap *soap, struct _tptz__RemovePresetResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__RemovePresetResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__GotoPreset(struct soap *soap, struct _tptz__GotoPreset *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->ProfileToken);
	soap_default_tt__ReferenceToken(soap, &a->PresetToken);
	a->Speed = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__GotoPreset(struct soap *soap, const struct _tptz__GotoPreset *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->ProfileToken);
	soap_serialize_tt__ReferenceToken(soap, &a->PresetToken);
	soap_serialize_PointerTott__PTZSpeed(soap, &a->Speed);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__GotoPreset(struct soap *soap, const char *tag, int id, const struct _tptz__GotoPreset *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__GotoPreset), type))
		return soap->error;
	if (a->ProfileToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tptz:ProfileToken", -1, &a->ProfileToken, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:ProfileToken"))
		return soap->error;
	if (a->PresetToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tptz:PresetToken", -1, &a->PresetToken, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:PresetToken"))
		return soap->error;
	if (soap_out_PointerTott__PTZSpeed(soap, "tptz:Speed", -1, &a->Speed, ""))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__GotoPreset * SOAP_FMAC4 soap_in__tptz__GotoPreset(struct soap *soap, const char *tag, struct _tptz__GotoPreset *a, const char *type)
{
	size_t soap_flag_ProfileToken = 1;
	size_t soap_flag_PresetToken = 1;
	size_t soap_flag_Speed = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__GotoPreset *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__GotoPreset, sizeof(struct _tptz__GotoPreset), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__GotoPreset(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_ProfileToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_tt__ReferenceToken(soap, "tptz:ProfileToken", &a->ProfileToken, "tt:ReferenceToken"))
			{	soap_flag_ProfileToken--;
		continue;
		}
		if (soap_flag_PresetToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_tt__ReferenceToken(soap, "tptz:PresetToken", &a->PresetToken, "tt:ReferenceToken"))
			{	soap_flag_PresetToken--;
		continue;
		}
		if (soap_flag_Speed && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_PointerTott__PTZSpeed(soap, "tptz:Speed", &a->Speed, "tt:PTZSpeed"))
			{	soap_flag_Speed--;
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
	{	a = (struct _tptz__GotoPreset *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__GotoPreset, 0, sizeof(struct _tptz__GotoPreset), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_ProfileToken > 0 || soap_flag_PresetToken > 0))
	{	soap->error = SOAP_OCCURS;
	return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__GotoPreset(struct soap *soap, const struct _tptz__GotoPreset *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__GotoPreset);
	if (soap_out__tptz__GotoPreset(soap, tag?tag:"tptz:GotoPreset", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__GotoPreset * SOAP_FMAC4 soap_get__tptz__GotoPreset(struct soap *soap, struct _tptz__GotoPreset *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__GotoPreset(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tptz__GotoPreset(struct soap *soap, struct _tptz__GotoPreset *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tptz__GotoPreset))
		soap_serialize__tptz__GotoPreset(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tptz__GotoPreset(struct soap *soap, const char *tag, int id, struct _tptz__GotoPreset *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tptz__GotoPreset);
	if (id < 0)
		return soap->error;
	return soap_out__tptz__GotoPreset(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tptz__GotoPreset ** SOAP_FMAC4 soap_in_PointerTo_tptz__GotoPreset(struct soap *soap, const char *tag, struct _tptz__GotoPreset **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tptz__GotoPreset **)soap_malloc(soap, sizeof(struct _tptz__GotoPreset *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tptz__GotoPreset(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tptz__GotoPreset **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tptz__GotoPreset, sizeof(struct _tptz__GotoPreset), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tptz__GotoPreset(struct soap *soap, struct _tptz__GotoPreset *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tptz__GotoPreset);
	if (soap_out_PointerTo_tptz__GotoPreset(soap, tag?tag:"tptz:GotoPreset", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__GotoPreset ** SOAP_FMAC4 soap_get_PointerTo_tptz__GotoPreset(struct soap *soap, struct _tptz__GotoPreset **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tptz__GotoPreset(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___tptz__GotoPreset(struct soap *soap, struct __tptz__GotoPreset *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->tptz__GotoPreset = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___tptz__GotoPreset(struct soap *soap, const struct __tptz__GotoPreset *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_tptz__GotoPreset(soap, &a->tptz__GotoPreset);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___tptz__GotoPreset(struct soap *soap, const char *tag, int id, const struct __tptz__GotoPreset *a, const char *type)
{
	if (soap_out_PointerTo_tptz__GotoPreset(soap, "tptz:GotoPreset", -1, &a->tptz__GotoPreset, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__GotoPreset * SOAP_FMAC4 soap_in___tptz__GotoPreset(struct soap *soap, const char *tag, struct __tptz__GotoPreset *a, const char *type)
{
	size_t soap_flag_tptz__GotoPreset = 1;
	short soap_flag;
	a = (struct __tptz__GotoPreset *)soap_id_enter(soap, "", a, SOAP_TYPE___tptz__GotoPreset, sizeof(struct __tptz__GotoPreset), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___tptz__GotoPreset(soap, a);
	for (soap_flag = 0;; soap_flag = 1)
	{	soap->error = SOAP_TAG_MISMATCH;
	if (soap_flag_tptz__GotoPreset && soap->error == SOAP_TAG_MISMATCH)
		if (soap_in_PointerTo_tptz__GotoPreset(soap, "tptz:GotoPreset", &a->tptz__GotoPreset, ""))
		{	soap_flag_tptz__GotoPreset--;
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

SOAP_FMAC3 int SOAP_FMAC4 soap_put___tptz__GotoPreset(struct soap *soap, const struct __tptz__GotoPreset *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___tptz__GotoPreset(soap, tag?tag:"-tptz:GotoPreset", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__GotoPreset * SOAP_FMAC4 soap_get___tptz__GotoPreset(struct soap *soap, struct __tptz__GotoPreset *p, const char *tag, const char *type)
{
	if ((p = soap_in___tptz__GotoPreset(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__GotoPresetResponse(struct soap *soap, struct _tptz__GotoPresetResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__GotoPresetResponse(struct soap *soap, const struct _tptz__GotoPresetResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__GotoPresetResponse(struct soap *soap, const char *tag, int id, const struct _tptz__GotoPresetResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__GotoPresetResponse), type))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__GotoPresetResponse * SOAP_FMAC4 soap_in__tptz__GotoPresetResponse(struct soap *soap, const char *tag, struct _tptz__GotoPresetResponse *a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__GotoPresetResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__GotoPresetResponse, sizeof(struct _tptz__GotoPresetResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__GotoPresetResponse(soap, a);
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
	{	a = (struct _tptz__GotoPresetResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__GotoPresetResponse, 0, sizeof(struct _tptz__GotoPresetResponse), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__GotoPresetResponse(struct soap *soap, const struct _tptz__GotoPresetResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__GotoPresetResponse);
	if (soap_out__tptz__GotoPresetResponse(soap, tag?tag:"tptz:GotoPresetResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__GotoPresetResponse * SOAP_FMAC4 soap_get__tptz__GotoPresetResponse(struct soap *soap, struct _tptz__GotoPresetResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__GotoPresetResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__MoveStatus(struct soap *soap, enum tt__MoveStatus *a)
{
	(void)soap; /* appease -Wall -Werror */
#ifdef SOAP_DEFAULT_tt__MoveStatus
	*a = SOAP_DEFAULT_tt__MoveStatus;
#else
	*a = (enum tt__MoveStatus)0;
#endif
}

static const struct soap_code_map soap_codes_tt__MoveStatus[] =
{	{ (long)tt__MoveStatus__IDLE, "IDLE" },
{ (long)tt__MoveStatus__MOVING, "MOVING" },
{ (long)tt__MoveStatus__UNKNOWN, "UNKNOWN" },
{ 0, NULL }
};

SOAP_FMAC3S const char* SOAP_FMAC4S soap_tt__MoveStatus2s(struct soap *soap, enum tt__MoveStatus n)
{	const char *s = soap_code_str(soap_codes_tt__MoveStatus, (long)n);
if (s)
return s;
return soap_long2s(soap, (long)n);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__MoveStatus(struct soap *soap, const char *tag, int id, const enum tt__MoveStatus *a, const char *type)
{	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__MoveStatus), type) || soap_send(soap, soap_tt__MoveStatus2s(soap, *a)))
return soap->error;
return soap_element_end_out(soap, tag);
}

SOAP_FMAC3S int SOAP_FMAC4S soap_s2tt__MoveStatus(struct soap *soap, const char *s, enum tt__MoveStatus *a)
{
	const struct soap_code_map *map;
	if (!s)
		return soap->error;
	map = soap_code(soap_codes_tt__MoveStatus, s);
	if (map)
		*a = (enum tt__MoveStatus)map->code;
	else
	{	long n;
	if (soap_s2long(soap, s, &n) || ((soap->mode & SOAP_XML_STRICT) && (n < 0 || n > 2)))
		return soap->error = SOAP_TYPE;
	*a = (enum tt__MoveStatus)n;
	}
	return SOAP_OK;
}

SOAP_FMAC3 enum tt__MoveStatus * SOAP_FMAC4 soap_in_tt__MoveStatus(struct soap *soap, const char *tag, enum tt__MoveStatus *a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 0, type))
	return NULL;
	a = (enum tt__MoveStatus *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__MoveStatus, sizeof(enum tt__MoveStatus), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	if (soap->body && !*soap->href)
	{	if (!a || soap_s2tt__MoveStatus(soap, soap_value(soap), a) || soap_element_end_in(soap, tag))
	return NULL;
	}
	else
	{	a = (enum tt__MoveStatus *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__MoveStatus, 0, sizeof(enum tt__MoveStatus), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__MoveStatus(struct soap *soap, const enum tt__MoveStatus *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__MoveStatus);
	if (soap_out_tt__MoveStatus(soap, tag?tag:"tt:MoveStatus", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 enum tt__MoveStatus * SOAP_FMAC4 soap_get_tt__MoveStatus(struct soap *soap, enum tt__MoveStatus *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__MoveStatus(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__MoveStatus(struct soap *soap, enum tt__MoveStatus *const*a)
{
	soap_reference(soap, *a, SOAP_TYPE_tt__MoveStatus);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__MoveStatus(struct soap *soap, const char *tag, int id, enum tt__MoveStatus *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__MoveStatus);
	if (id < 0)
		return soap->error;
	return soap_out_tt__MoveStatus(soap, tag, id, *a, type);
}

SOAP_FMAC3 enum tt__MoveStatus ** SOAP_FMAC4 soap_in_PointerTott__MoveStatus(struct soap *soap, const char *tag, enum tt__MoveStatus **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
	return NULL;
	if (!a)
		if (!(a = (enum tt__MoveStatus **)soap_malloc(soap, sizeof(enum tt__MoveStatus *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in_tt__MoveStatus(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (enum tt__MoveStatus **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__MoveStatus, sizeof(enum tt__MoveStatus), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__MoveStatus(struct soap *soap, enum tt__MoveStatus *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__MoveStatus);
	if (soap_out_PointerTott__MoveStatus(soap, tag?tag:"tt:MoveStatus", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 enum tt__MoveStatus ** SOAP_FMAC4 soap_get_PointerTott__MoveStatus(struct soap *soap, enum tt__MoveStatus **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__MoveStatus(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__PTZMoveStatus(struct soap *soap, struct tt__PTZMoveStatus *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->PanTilt = NULL;
	a->Zoom = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__PTZMoveStatus(struct soap *soap, const struct tt__PTZMoveStatus *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTott__MoveStatus(soap, &a->PanTilt);
	soap_serialize_PointerTott__MoveStatus(soap, &a->Zoom);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__PTZMoveStatus(struct soap *soap, const char *tag, int id, const struct tt__PTZMoveStatus *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__PTZMoveStatus), type))
		return soap->error;
	if (soap_out_PointerTott__MoveStatus(soap, "tt:PanTilt", -1, &a->PanTilt, ""))
		return soap->error;
	if (soap_out_PointerTott__MoveStatus(soap, "tt:Zoom", -1, &a->Zoom, ""))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__PTZMoveStatus * SOAP_FMAC4 soap_in_tt__PTZMoveStatus(struct soap *soap, const char *tag, struct tt__PTZMoveStatus *a, const char *type)
{
	size_t soap_flag_PanTilt = 1;
	size_t soap_flag_Zoom = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__PTZMoveStatus *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__PTZMoveStatus, sizeof(struct tt__PTZMoveStatus), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__PTZMoveStatus(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_PanTilt && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_PointerTott__MoveStatus(soap, "tt:PanTilt", &a->PanTilt, "tt:MoveStatus"))
			{	soap_flag_PanTilt--;
		continue;
		}
		if (soap_flag_Zoom && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_PointerTott__MoveStatus(soap, "tt:Zoom", &a->Zoom, "tt:MoveStatus"))
			{	soap_flag_Zoom--;
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
	{	a = (struct tt__PTZMoveStatus *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__PTZMoveStatus, 0, sizeof(struct tt__PTZMoveStatus), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__PTZMoveStatus(struct soap *soap, const struct tt__PTZMoveStatus *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__PTZMoveStatus);
	if (soap_out_tt__PTZMoveStatus(soap, tag?tag:"tt:PTZMoveStatus", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZMoveStatus * SOAP_FMAC4 soap_get_tt__PTZMoveStatus(struct soap *soap, struct tt__PTZMoveStatus *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__PTZMoveStatus(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__PTZMoveStatus(struct soap *soap, struct tt__PTZMoveStatus *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__PTZMoveStatus))
		soap_serialize_tt__PTZMoveStatus(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__PTZMoveStatus(struct soap *soap, const char *tag, int id, struct tt__PTZMoveStatus *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__PTZMoveStatus);
	if (id < 0)
		return soap->error;
	return soap_out_tt__PTZMoveStatus(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__PTZMoveStatus ** SOAP_FMAC4 soap_in_PointerTott__PTZMoveStatus(struct soap *soap, const char *tag, struct tt__PTZMoveStatus **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__PTZMoveStatus **)soap_malloc(soap, sizeof(struct tt__PTZMoveStatus *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in_tt__PTZMoveStatus(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct tt__PTZMoveStatus **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__PTZMoveStatus, sizeof(struct tt__PTZMoveStatus), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__PTZMoveStatus(struct soap *soap, struct tt__PTZMoveStatus *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__PTZMoveStatus);
	if (soap_out_PointerTott__PTZMoveStatus(soap, tag?tag:"tt:PTZMoveStatus", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZMoveStatus ** SOAP_FMAC4 soap_get_PointerTott__PTZMoveStatus(struct soap *soap, struct tt__PTZMoveStatus **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__PTZMoveStatus(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__PTZStatus(struct soap *soap, struct tt__PTZStatus *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->Position = NULL;
	a->MoveStatus = NULL;
	soap_default_string(soap, &a->Error);
	soap_default_time(soap, &a->UtcTime);
	a->__size = 0;
	a->__any = NULL;
	a->__anyAttribute = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__PTZStatus(struct soap *soap, const struct tt__PTZStatus *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTott__PTZVector(soap, &a->Position);
	soap_serialize_PointerTott__PTZMoveStatus(soap, &a->MoveStatus);
	soap_serialize_string(soap, &a->Error);
	soap_embedded(soap, &a->UtcTime, SOAP_TYPE_time);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__PTZStatus(struct soap *soap, const char *tag, int id, const struct tt__PTZStatus *a, const char *type)
{
	if (a->__anyAttribute)
		soap_set_attr(soap, "-anyAttribute", a->__anyAttribute, 1);
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__PTZStatus), type))
		return soap->error;
	if (soap_out_PointerTott__PTZVector(soap, "tt:Position", -1, &a->Position, ""))
		return soap->error;
	if (soap_out_PointerTott__PTZMoveStatus(soap, "tt:MoveStatus", -1, &a->MoveStatus, ""))
		return soap->error;
	if (soap_out_string(soap, "tt:Error", -1, &a->Error, ""))
		return soap->error;
	if (soap_out_time(soap, "tt:UtcTime", -1, &a->UtcTime, ""))
		return soap->error;
	if (a->__any)
	{	int i;
	for (i = 0; i < a->__size; i++)
		soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__PTZStatus * SOAP_FMAC4 soap_in_tt__PTZStatus(struct soap *soap, const char *tag, struct tt__PTZStatus *a, const char *type)
{
	size_t soap_flag_Position = 1;
	size_t soap_flag_MoveStatus = 1;
	size_t soap_flag_Error = 1;
	size_t soap_flag_UtcTime = 1;
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__PTZStatus *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__PTZStatus, sizeof(struct tt__PTZStatus), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__PTZStatus(soap, a);
	if (soap_s2string(soap, soap_attr_value(soap, "-anyAttribute", 0), &a->__anyAttribute, 0, -1))
		return NULL;
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_Position && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_PointerTott__PTZVector(soap, "tt:Position", &a->Position, "tt:PTZVector"))
			{	soap_flag_Position--;
		continue;
		}
		if (soap_flag_MoveStatus && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_PointerTott__PTZMoveStatus(soap, "tt:MoveStatus", &a->MoveStatus, "tt:PTZMoveStatus"))
			{	soap_flag_MoveStatus--;
		continue;
		}
		if (soap_flag_Error && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_string(soap, "tt:Error", &a->Error, "xsd:string"))
			{	soap_flag_Error--;
		continue;
		}
		if (soap_flag_UtcTime && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_time(soap, "tt:UtcTime", &a->UtcTime, "xsd:dateTime"))
			{	soap_flag_UtcTime--;
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
	{	a = (struct tt__PTZStatus *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__PTZStatus, 0, sizeof(struct tt__PTZStatus), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_UtcTime > 0))
	{	soap->error = SOAP_OCCURS;
	return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__PTZStatus(struct soap *soap, const struct tt__PTZStatus *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__PTZStatus);
	if (soap_out_tt__PTZStatus(soap, tag?tag:"tt:PTZStatus", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZStatus * SOAP_FMAC4 soap_get_tt__PTZStatus(struct soap *soap, struct tt__PTZStatus *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__PTZStatus(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__PTZStatus(struct soap *soap, struct tt__PTZStatus *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__PTZStatus))
		soap_serialize_tt__PTZStatus(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__PTZStatus(struct soap *soap, const char *tag, int id, struct tt__PTZStatus *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__PTZStatus);
	if (id < 0)
		return soap->error;
	return soap_out_tt__PTZStatus(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__PTZStatus ** SOAP_FMAC4 soap_in_PointerTott__PTZStatus(struct soap *soap, const char *tag, struct tt__PTZStatus **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__PTZStatus **)soap_malloc(soap, sizeof(struct tt__PTZStatus *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in_tt__PTZStatus(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct tt__PTZStatus **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__PTZStatus, sizeof(struct tt__PTZStatus), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__PTZStatus(struct soap *soap, struct tt__PTZStatus *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__PTZStatus);
	if (soap_out_PointerTott__PTZStatus(soap, tag?tag:"tt:PTZStatus", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZStatus ** SOAP_FMAC4 soap_get_PointerTott__PTZStatus(struct soap *soap, struct tt__PTZStatus **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__PTZStatus(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__PTZSpacesExtension(struct soap *soap, struct tt__PTZSpacesExtension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__size = 0;
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__PTZSpacesExtension(struct soap *soap, const struct tt__PTZSpacesExtension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__PTZSpacesExtension(struct soap *soap, const char *tag, int id, const struct tt__PTZSpacesExtension *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__PTZSpacesExtension), type))
		return soap->error;
	if (a->__any)
	{	int i;
	for (i = 0; i < a->__size; i++)
		soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__PTZSpacesExtension * SOAP_FMAC4 soap_in_tt__PTZSpacesExtension(struct soap *soap, const char *tag, struct tt__PTZSpacesExtension *a, const char *type)
{
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__PTZSpacesExtension *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__PTZSpacesExtension, sizeof(struct tt__PTZSpacesExtension), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__PTZSpacesExtension(soap, a);
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
	{	a = (struct tt__PTZSpacesExtension *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__PTZSpacesExtension, 0, sizeof(struct tt__PTZSpacesExtension), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__PTZSpacesExtension(struct soap *soap, const struct tt__PTZSpacesExtension *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__PTZSpacesExtension);
	if (soap_out_tt__PTZSpacesExtension(soap, tag?tag:"tt:PTZSpacesExtension", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZSpacesExtension * SOAP_FMAC4 soap_get_tt__PTZSpacesExtension(struct soap *soap, struct tt__PTZSpacesExtension *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__PTZSpacesExtension(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__PTZSpacesExtension(struct soap *soap, struct tt__PTZSpacesExtension *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__PTZSpacesExtension))
		soap_serialize_tt__PTZSpacesExtension(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__PTZSpacesExtension(struct soap *soap, const char *tag, int id, struct tt__PTZSpacesExtension *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__PTZSpacesExtension);
	if (id < 0)
		return soap->error;
	return soap_out_tt__PTZSpacesExtension(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__PTZSpacesExtension ** SOAP_FMAC4 soap_in_PointerTott__PTZSpacesExtension(struct soap *soap, const char *tag, struct tt__PTZSpacesExtension **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__PTZSpacesExtension **)soap_malloc(soap, sizeof(struct tt__PTZSpacesExtension *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in_tt__PTZSpacesExtension(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct tt__PTZSpacesExtension **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__PTZSpacesExtension, sizeof(struct tt__PTZSpacesExtension), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__PTZSpacesExtension(struct soap *soap, struct tt__PTZSpacesExtension *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__PTZSpacesExtension);
	if (soap_out_PointerTott__PTZSpacesExtension(soap, tag?tag:"tt:PTZSpacesExtension", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZSpacesExtension ** SOAP_FMAC4 soap_get_PointerTott__PTZSpacesExtension(struct soap *soap, struct tt__PTZSpacesExtension **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__PTZSpacesExtension(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__PTZSpaces(struct soap *soap, struct tt__PTZSpaces *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__sizeAbsolutePanTiltPositionSpace = 0;
	a->AbsolutePanTiltPositionSpace = NULL;
	a->__sizeAbsoluteZoomPositionSpace = 0;
	a->AbsoluteZoomPositionSpace = NULL;
	a->__sizeRelativePanTiltTranslationSpace = 0;
	a->RelativePanTiltTranslationSpace = NULL;
	a->__sizeRelativeZoomTranslationSpace = 0;
	a->RelativeZoomTranslationSpace = NULL;
	a->__sizeContinuousPanTiltVelocitySpace = 0;
	a->ContinuousPanTiltVelocitySpace = NULL;
	a->__sizeContinuousZoomVelocitySpace = 0;
	a->ContinuousZoomVelocitySpace = NULL;
	a->__sizePanTiltSpeedSpace = 0;
	a->PanTiltSpeedSpace = NULL;
	a->__sizeZoomSpeedSpace = 0;
	a->ZoomSpeedSpace = NULL;
	a->Extension = NULL;
	a->__anyAttribute = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__PTZSpaces(struct soap *soap, const struct tt__PTZSpaces *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	if (a->AbsolutePanTiltPositionSpace)
	{	int i;
	for (i = 0; i < a->__sizeAbsolutePanTiltPositionSpace; i++)
	{
		soap_embedded(soap, a->AbsolutePanTiltPositionSpace + i, SOAP_TYPE_tt__Space2DDescription);
		soap_serialize_tt__Space2DDescription(soap, a->AbsolutePanTiltPositionSpace + i);
	}
	}
	if (a->AbsoluteZoomPositionSpace)
	{	int i;
	for (i = 0; i < a->__sizeAbsoluteZoomPositionSpace; i++)
	{
		soap_embedded(soap, a->AbsoluteZoomPositionSpace + i, SOAP_TYPE_tt__Space1DDescription);
		soap_serialize_tt__Space1DDescription(soap, a->AbsoluteZoomPositionSpace + i);
	}
	}
	if (a->RelativePanTiltTranslationSpace)
	{	int i;
	for (i = 0; i < a->__sizeRelativePanTiltTranslationSpace; i++)
	{
		soap_embedded(soap, a->RelativePanTiltTranslationSpace + i, SOAP_TYPE_tt__Space2DDescription);
		soap_serialize_tt__Space2DDescription(soap, a->RelativePanTiltTranslationSpace + i);
	}
	}
	if (a->RelativeZoomTranslationSpace)
	{	int i;
	for (i = 0; i < a->__sizeRelativeZoomTranslationSpace; i++)
	{
		soap_embedded(soap, a->RelativeZoomTranslationSpace + i, SOAP_TYPE_tt__Space1DDescription);
		soap_serialize_tt__Space1DDescription(soap, a->RelativeZoomTranslationSpace + i);
	}
	}
	if (a->ContinuousPanTiltVelocitySpace)
	{	int i;
	for (i = 0; i < a->__sizeContinuousPanTiltVelocitySpace; i++)
	{
		soap_embedded(soap, a->ContinuousPanTiltVelocitySpace + i, SOAP_TYPE_tt__Space2DDescription);
		soap_serialize_tt__Space2DDescription(soap, a->ContinuousPanTiltVelocitySpace + i);
	}
	}
	if (a->ContinuousZoomVelocitySpace)
	{	int i;
	for (i = 0; i < a->__sizeContinuousZoomVelocitySpace; i++)
	{
		soap_embedded(soap, a->ContinuousZoomVelocitySpace + i, SOAP_TYPE_tt__Space1DDescription);
		soap_serialize_tt__Space1DDescription(soap, a->ContinuousZoomVelocitySpace + i);
	}
	}
	if (a->PanTiltSpeedSpace)
	{	int i;
	for (i = 0; i < a->__sizePanTiltSpeedSpace; i++)
	{
		soap_embedded(soap, a->PanTiltSpeedSpace + i, SOAP_TYPE_tt__Space1DDescription);
		soap_serialize_tt__Space1DDescription(soap, a->PanTiltSpeedSpace + i);
	}
	}
	if (a->ZoomSpeedSpace)
	{	int i;
	for (i = 0; i < a->__sizeZoomSpeedSpace; i++)
	{
		soap_embedded(soap, a->ZoomSpeedSpace + i, SOAP_TYPE_tt__Space1DDescription);
		soap_serialize_tt__Space1DDescription(soap, a->ZoomSpeedSpace + i);
	}
	}
	soap_serialize_PointerTott__PTZSpacesExtension(soap, &a->Extension);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__PTZSpaces(struct soap *soap, const char *tag, int id, const struct tt__PTZSpaces *a, const char *type)
{
	if (a->__anyAttribute)
		soap_set_attr(soap, "-anyAttribute", a->__anyAttribute, 1);
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__PTZSpaces), type))
		return soap->error;
	if (a->AbsolutePanTiltPositionSpace)
	{	int i;
	for (i = 0; i < a->__sizeAbsolutePanTiltPositionSpace; i++)
		if (soap_out_tt__Space2DDescription(soap, "tt:AbsolutePanTiltPositionSpace", -1, a->AbsolutePanTiltPositionSpace + i, ""))
			return soap->error;
	}
	if (a->AbsoluteZoomPositionSpace)
	{	int i;
	for (i = 0; i < a->__sizeAbsoluteZoomPositionSpace; i++)
		if (soap_out_tt__Space1DDescription(soap, "tt:AbsoluteZoomPositionSpace", -1, a->AbsoluteZoomPositionSpace + i, ""))
			return soap->error;
	}
	if (a->RelativePanTiltTranslationSpace)
	{	int i;
	for (i = 0; i < a->__sizeRelativePanTiltTranslationSpace; i++)
		if (soap_out_tt__Space2DDescription(soap, "tt:RelativePanTiltTranslationSpace", -1, a->RelativePanTiltTranslationSpace + i, ""))
			return soap->error;
	}
	if (a->RelativeZoomTranslationSpace)
	{	int i;
	for (i = 0; i < a->__sizeRelativeZoomTranslationSpace; i++)
		if (soap_out_tt__Space1DDescription(soap, "tt:RelativeZoomTranslationSpace", -1, a->RelativeZoomTranslationSpace + i, ""))
			return soap->error;
	}
	if (a->ContinuousPanTiltVelocitySpace)
	{	int i;
	for (i = 0; i < a->__sizeContinuousPanTiltVelocitySpace; i++)
		if (soap_out_tt__Space2DDescription(soap, "tt:ContinuousPanTiltVelocitySpace", -1, a->ContinuousPanTiltVelocitySpace + i, ""))
			return soap->error;
	}
	if (a->ContinuousZoomVelocitySpace)
	{	int i;
	for (i = 0; i < a->__sizeContinuousZoomVelocitySpace; i++)
		if (soap_out_tt__Space1DDescription(soap, "tt:ContinuousZoomVelocitySpace", -1, a->ContinuousZoomVelocitySpace + i, ""))
			return soap->error;
	}
	if (a->PanTiltSpeedSpace)
	{	int i;
	for (i = 0; i < a->__sizePanTiltSpeedSpace; i++)
		if (soap_out_tt__Space1DDescription(soap, "tt:PanTiltSpeedSpace", -1, a->PanTiltSpeedSpace + i, ""))
			return soap->error;
	}
	if (a->ZoomSpeedSpace)
	{	int i;
	for (i = 0; i < a->__sizeZoomSpeedSpace; i++)
		if (soap_out_tt__Space1DDescription(soap, "tt:ZoomSpeedSpace", -1, a->ZoomSpeedSpace + i, ""))
			return soap->error;
	}
	if (soap_out_PointerTott__PTZSpacesExtension(soap, "tt:Extension", -1, &a->Extension, ""))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__PTZSpaces * SOAP_FMAC4 soap_in_tt__PTZSpaces(struct soap *soap, const char *tag, struct tt__PTZSpaces *a, const char *type)
{
	struct soap_blist *soap_blist_AbsolutePanTiltPositionSpace = NULL;
	struct soap_blist *soap_blist_AbsoluteZoomPositionSpace = NULL;
	struct soap_blist *soap_blist_RelativePanTiltTranslationSpace = NULL;
	struct soap_blist *soap_blist_RelativeZoomTranslationSpace = NULL;
	struct soap_blist *soap_blist_ContinuousPanTiltVelocitySpace = NULL;
	struct soap_blist *soap_blist_ContinuousZoomVelocitySpace = NULL;
	struct soap_blist *soap_blist_PanTiltSpeedSpace = NULL;
	struct soap_blist *soap_blist_ZoomSpeedSpace = NULL;
	size_t soap_flag_Extension = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__PTZSpaces *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__PTZSpaces, sizeof(struct tt__PTZSpaces), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__PTZSpaces(soap, a);
	if (soap_s2string(soap, soap_attr_value(soap, "-anyAttribute", 0), &a->__anyAttribute, 0, -1))
		return NULL;
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tt:AbsolutePanTiltPositionSpace", 1, NULL))
		{	if (a->AbsolutePanTiltPositionSpace == NULL)
		{	if (soap_blist_AbsolutePanTiltPositionSpace == NULL)
		soap_blist_AbsolutePanTiltPositionSpace = soap_new_block(soap);
		a->AbsolutePanTiltPositionSpace = (struct tt__Space2DDescription *)soap_push_block(soap, soap_blist_AbsolutePanTiltPositionSpace, sizeof(struct tt__Space2DDescription));
		if (a->AbsolutePanTiltPositionSpace == NULL)
			return NULL;
		soap_default_tt__Space2DDescription(soap, a->AbsolutePanTiltPositionSpace);
		}
		soap_revert(soap);
		if (soap_in_tt__Space2DDescription(soap, "tt:AbsolutePanTiltPositionSpace", a->AbsolutePanTiltPositionSpace, "tt:Space2DDescription"))
		{	a->__sizeAbsolutePanTiltPositionSpace++;
		a->AbsolutePanTiltPositionSpace = NULL;
		continue;
		}
		}
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tt:AbsoluteZoomPositionSpace", 1, NULL))
		{	if (a->AbsoluteZoomPositionSpace == NULL)
		{	if (soap_blist_AbsoluteZoomPositionSpace == NULL)
		soap_blist_AbsoluteZoomPositionSpace = soap_new_block(soap);
		a->AbsoluteZoomPositionSpace = (struct tt__Space1DDescription *)soap_push_block(soap, soap_blist_AbsoluteZoomPositionSpace, sizeof(struct tt__Space1DDescription));
		if (a->AbsoluteZoomPositionSpace == NULL)
			return NULL;
		soap_default_tt__Space1DDescription(soap, a->AbsoluteZoomPositionSpace);
		}
		soap_revert(soap);
		if (soap_in_tt__Space1DDescription(soap, "tt:AbsoluteZoomPositionSpace", a->AbsoluteZoomPositionSpace, "tt:Space1DDescription"))
		{	a->__sizeAbsoluteZoomPositionSpace++;
		a->AbsoluteZoomPositionSpace = NULL;
		continue;
		}
		}
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tt:RelativePanTiltTranslationSpace", 1, NULL))
		{	if (a->RelativePanTiltTranslationSpace == NULL)
		{	if (soap_blist_RelativePanTiltTranslationSpace == NULL)
		soap_blist_RelativePanTiltTranslationSpace = soap_new_block(soap);
		a->RelativePanTiltTranslationSpace = (struct tt__Space2DDescription *)soap_push_block(soap, soap_blist_RelativePanTiltTranslationSpace, sizeof(struct tt__Space2DDescription));
		if (a->RelativePanTiltTranslationSpace == NULL)
			return NULL;
		soap_default_tt__Space2DDescription(soap, a->RelativePanTiltTranslationSpace);
		}
		soap_revert(soap);
		if (soap_in_tt__Space2DDescription(soap, "tt:RelativePanTiltTranslationSpace", a->RelativePanTiltTranslationSpace, "tt:Space2DDescription"))
		{	a->__sizeRelativePanTiltTranslationSpace++;
		a->RelativePanTiltTranslationSpace = NULL;
		continue;
		}
		}
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tt:RelativeZoomTranslationSpace", 1, NULL))
		{	if (a->RelativeZoomTranslationSpace == NULL)
		{	if (soap_blist_RelativeZoomTranslationSpace == NULL)
		soap_blist_RelativeZoomTranslationSpace = soap_new_block(soap);
		a->RelativeZoomTranslationSpace = (struct tt__Space1DDescription *)soap_push_block(soap, soap_blist_RelativeZoomTranslationSpace, sizeof(struct tt__Space1DDescription));
		if (a->RelativeZoomTranslationSpace == NULL)
			return NULL;
		soap_default_tt__Space1DDescription(soap, a->RelativeZoomTranslationSpace);
		}
		soap_revert(soap);
		if (soap_in_tt__Space1DDescription(soap, "tt:RelativeZoomTranslationSpace", a->RelativeZoomTranslationSpace, "tt:Space1DDescription"))
		{	a->__sizeRelativeZoomTranslationSpace++;
		a->RelativeZoomTranslationSpace = NULL;
		continue;
		}
		}
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tt:ContinuousPanTiltVelocitySpace", 1, NULL))
		{	if (a->ContinuousPanTiltVelocitySpace == NULL)
		{	if (soap_blist_ContinuousPanTiltVelocitySpace == NULL)
		soap_blist_ContinuousPanTiltVelocitySpace = soap_new_block(soap);
		a->ContinuousPanTiltVelocitySpace = (struct tt__Space2DDescription *)soap_push_block(soap, soap_blist_ContinuousPanTiltVelocitySpace, sizeof(struct tt__Space2DDescription));
		if (a->ContinuousPanTiltVelocitySpace == NULL)
			return NULL;
		soap_default_tt__Space2DDescription(soap, a->ContinuousPanTiltVelocitySpace);
		}
		soap_revert(soap);
		if (soap_in_tt__Space2DDescription(soap, "tt:ContinuousPanTiltVelocitySpace", a->ContinuousPanTiltVelocitySpace, "tt:Space2DDescription"))
		{	a->__sizeContinuousPanTiltVelocitySpace++;
		a->ContinuousPanTiltVelocitySpace = NULL;
		continue;
		}
		}
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tt:ContinuousZoomVelocitySpace", 1, NULL))
		{	if (a->ContinuousZoomVelocitySpace == NULL)
		{	if (soap_blist_ContinuousZoomVelocitySpace == NULL)
		soap_blist_ContinuousZoomVelocitySpace = soap_new_block(soap);
		a->ContinuousZoomVelocitySpace = (struct tt__Space1DDescription *)soap_push_block(soap, soap_blist_ContinuousZoomVelocitySpace, sizeof(struct tt__Space1DDescription));
		if (a->ContinuousZoomVelocitySpace == NULL)
			return NULL;
		soap_default_tt__Space1DDescription(soap, a->ContinuousZoomVelocitySpace);
		}
		soap_revert(soap);
		if (soap_in_tt__Space1DDescription(soap, "tt:ContinuousZoomVelocitySpace", a->ContinuousZoomVelocitySpace, "tt:Space1DDescription"))
		{	a->__sizeContinuousZoomVelocitySpace++;
		a->ContinuousZoomVelocitySpace = NULL;
		continue;
		}
		}
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tt:PanTiltSpeedSpace", 1, NULL))
		{	if (a->PanTiltSpeedSpace == NULL)
		{	if (soap_blist_PanTiltSpeedSpace == NULL)
		soap_blist_PanTiltSpeedSpace = soap_new_block(soap);
		a->PanTiltSpeedSpace = (struct tt__Space1DDescription *)soap_push_block(soap, soap_blist_PanTiltSpeedSpace, sizeof(struct tt__Space1DDescription));
		if (a->PanTiltSpeedSpace == NULL)
			return NULL;
		soap_default_tt__Space1DDescription(soap, a->PanTiltSpeedSpace);
		}
		soap_revert(soap);
		if (soap_in_tt__Space1DDescription(soap, "tt:PanTiltSpeedSpace", a->PanTiltSpeedSpace, "tt:Space1DDescription"))
		{	a->__sizePanTiltSpeedSpace++;
		a->PanTiltSpeedSpace = NULL;
		continue;
		}
		}
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tt:ZoomSpeedSpace", 1, NULL))
		{	if (a->ZoomSpeedSpace == NULL)
		{	if (soap_blist_ZoomSpeedSpace == NULL)
		soap_blist_ZoomSpeedSpace = soap_new_block(soap);
		a->ZoomSpeedSpace = (struct tt__Space1DDescription *)soap_push_block(soap, soap_blist_ZoomSpeedSpace, sizeof(struct tt__Space1DDescription));
		if (a->ZoomSpeedSpace == NULL)
			return NULL;
		soap_default_tt__Space1DDescription(soap, a->ZoomSpeedSpace);
		}
		soap_revert(soap);
		if (soap_in_tt__Space1DDescription(soap, "tt:ZoomSpeedSpace", a->ZoomSpeedSpace, "tt:Space1DDescription"))
		{	a->__sizeZoomSpeedSpace++;
		a->ZoomSpeedSpace = NULL;
		continue;
		}
		}
		if (soap_flag_Extension && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_PointerTott__PTZSpacesExtension(soap, "tt:Extension", &a->Extension, "tt:PTZSpacesExtension"))
			{	soap_flag_Extension--;
		continue;
		}
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (a->AbsolutePanTiltPositionSpace)
			soap_pop_block(soap, soap_blist_AbsolutePanTiltPositionSpace);
		if (a->__sizeAbsolutePanTiltPositionSpace)
			a->AbsolutePanTiltPositionSpace = (struct tt__Space2DDescription *)soap_save_block(soap, soap_blist_AbsolutePanTiltPositionSpace, NULL, 1);
		else
		{	a->AbsolutePanTiltPositionSpace = NULL;
		if (soap_blist_AbsolutePanTiltPositionSpace)
			soap_end_block(soap, soap_blist_AbsolutePanTiltPositionSpace);
		}
		if (a->AbsoluteZoomPositionSpace)
			soap_pop_block(soap, soap_blist_AbsoluteZoomPositionSpace);
		if (a->__sizeAbsoluteZoomPositionSpace)
			a->AbsoluteZoomPositionSpace = (struct tt__Space1DDescription *)soap_save_block(soap, soap_blist_AbsoluteZoomPositionSpace, NULL, 1);
		else
		{	a->AbsoluteZoomPositionSpace = NULL;
		if (soap_blist_AbsoluteZoomPositionSpace)
			soap_end_block(soap, soap_blist_AbsoluteZoomPositionSpace);
		}
		if (a->RelativePanTiltTranslationSpace)
			soap_pop_block(soap, soap_blist_RelativePanTiltTranslationSpace);
		if (a->__sizeRelativePanTiltTranslationSpace)
			a->RelativePanTiltTranslationSpace = (struct tt__Space2DDescription *)soap_save_block(soap, soap_blist_RelativePanTiltTranslationSpace, NULL, 1);
		else
		{	a->RelativePanTiltTranslationSpace = NULL;
		if (soap_blist_RelativePanTiltTranslationSpace)
			soap_end_block(soap, soap_blist_RelativePanTiltTranslationSpace);
		}
		if (a->RelativeZoomTranslationSpace)
			soap_pop_block(soap, soap_blist_RelativeZoomTranslationSpace);
		if (a->__sizeRelativeZoomTranslationSpace)
			a->RelativeZoomTranslationSpace = (struct tt__Space1DDescription *)soap_save_block(soap, soap_blist_RelativeZoomTranslationSpace, NULL, 1);
		else
		{	a->RelativeZoomTranslationSpace = NULL;
		if (soap_blist_RelativeZoomTranslationSpace)
			soap_end_block(soap, soap_blist_RelativeZoomTranslationSpace);
		}
		if (a->ContinuousPanTiltVelocitySpace)
			soap_pop_block(soap, soap_blist_ContinuousPanTiltVelocitySpace);
		if (a->__sizeContinuousPanTiltVelocitySpace)
			a->ContinuousPanTiltVelocitySpace = (struct tt__Space2DDescription *)soap_save_block(soap, soap_blist_ContinuousPanTiltVelocitySpace, NULL, 1);
		else
		{	a->ContinuousPanTiltVelocitySpace = NULL;
		if (soap_blist_ContinuousPanTiltVelocitySpace)
			soap_end_block(soap, soap_blist_ContinuousPanTiltVelocitySpace);
		}
		if (a->ContinuousZoomVelocitySpace)
			soap_pop_block(soap, soap_blist_ContinuousZoomVelocitySpace);
		if (a->__sizeContinuousZoomVelocitySpace)
			a->ContinuousZoomVelocitySpace = (struct tt__Space1DDescription *)soap_save_block(soap, soap_blist_ContinuousZoomVelocitySpace, NULL, 1);
		else
		{	a->ContinuousZoomVelocitySpace = NULL;
		if (soap_blist_ContinuousZoomVelocitySpace)
			soap_end_block(soap, soap_blist_ContinuousZoomVelocitySpace);
		}
		if (a->PanTiltSpeedSpace)
			soap_pop_block(soap, soap_blist_PanTiltSpeedSpace);
		if (a->__sizePanTiltSpeedSpace)
			a->PanTiltSpeedSpace = (struct tt__Space1DDescription *)soap_save_block(soap, soap_blist_PanTiltSpeedSpace, NULL, 1);
		else
		{	a->PanTiltSpeedSpace = NULL;
		if (soap_blist_PanTiltSpeedSpace)
			soap_end_block(soap, soap_blist_PanTiltSpeedSpace);
		}
		if (a->ZoomSpeedSpace)
			soap_pop_block(soap, soap_blist_ZoomSpeedSpace);
		if (a->__sizeZoomSpeedSpace)
			a->ZoomSpeedSpace = (struct tt__Space1DDescription *)soap_save_block(soap, soap_blist_ZoomSpeedSpace, NULL, 1);
		else
		{	a->ZoomSpeedSpace = NULL;
		if (soap_blist_ZoomSpeedSpace)
			soap_end_block(soap, soap_blist_ZoomSpeedSpace);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct tt__PTZSpaces *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__PTZSpaces, 0, sizeof(struct tt__PTZSpaces), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__PTZSpaces(struct soap *soap, const struct tt__PTZSpaces *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__PTZSpaces);
	if (soap_out_tt__PTZSpaces(soap, tag?tag:"tt:PTZSpaces", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZSpaces * SOAP_FMAC4 soap_get_tt__PTZSpaces(struct soap *soap, struct tt__PTZSpaces *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__PTZSpaces(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__PTZSpaces(struct soap *soap, struct tt__PTZSpaces *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__PTZSpaces))
		soap_serialize_tt__PTZSpaces(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__PTZSpaces(struct soap *soap, const char *tag, int id, struct tt__PTZSpaces *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__PTZSpaces);
	if (id < 0)
		return soap->error;
	return soap_out_tt__PTZSpaces(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__PTZSpaces ** SOAP_FMAC4 soap_in_PointerTott__PTZSpaces(struct soap *soap, const char *tag, struct tt__PTZSpaces **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__PTZSpaces **)soap_malloc(soap, sizeof(struct tt__PTZSpaces *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in_tt__PTZSpaces(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct tt__PTZSpaces **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__PTZSpaces, sizeof(struct tt__PTZSpaces), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__PTZSpaces(struct soap *soap, struct tt__PTZSpaces *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__PTZSpaces);
	if (soap_out_PointerTott__PTZSpaces(soap, tag?tag:"tt:PTZSpaces", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZSpaces ** SOAP_FMAC4 soap_get_PointerTott__PTZSpaces(struct soap *soap, struct tt__PTZSpaces **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__PTZSpaces(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}


SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__PTZNodeExtension(struct soap *soap, struct tt__PTZNodeExtension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__size = 0;
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__PTZNodeExtension(struct soap *soap, const struct tt__PTZNodeExtension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__PTZNodeExtension(struct soap *soap, const char *tag, int id, const struct tt__PTZNodeExtension *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__PTZNodeExtension), type))
		return soap->error;
	if (a->__any)
	{	int i;
	for (i = 0; i < a->__size; i++)
		soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__PTZNodeExtension * SOAP_FMAC4 soap_in_tt__PTZNodeExtension(struct soap *soap, const char *tag, struct tt__PTZNodeExtension *a, const char *type)
{
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__PTZNodeExtension *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__PTZNodeExtension, sizeof(struct tt__PTZNodeExtension), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__PTZNodeExtension(soap, a);
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
	{	a = (struct tt__PTZNodeExtension *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__PTZNodeExtension, 0, sizeof(struct tt__PTZNodeExtension), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__PTZNodeExtension(struct soap *soap, const struct tt__PTZNodeExtension *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__PTZNodeExtension);
	if (soap_out_tt__PTZNodeExtension(soap, tag?tag:"tt:PTZNodeExtension", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZNodeExtension * SOAP_FMAC4 soap_get_tt__PTZNodeExtension(struct soap *soap, struct tt__PTZNodeExtension *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__PTZNodeExtension(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__PTZNodeExtension(struct soap *soap, struct tt__PTZNodeExtension *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__PTZNodeExtension))
		soap_serialize_tt__PTZNodeExtension(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__PTZNodeExtension(struct soap *soap, const char *tag, int id, struct tt__PTZNodeExtension *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__PTZNodeExtension);
	if (id < 0)
		return soap->error;
	return soap_out_tt__PTZNodeExtension(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__PTZNodeExtension ** SOAP_FMAC4 soap_in_PointerTott__PTZNodeExtension(struct soap *soap, const char *tag, struct tt__PTZNodeExtension **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__PTZNodeExtension **)soap_malloc(soap, sizeof(struct tt__PTZNodeExtension *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in_tt__PTZNodeExtension(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct tt__PTZNodeExtension **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__PTZNodeExtension, sizeof(struct tt__PTZNodeExtension), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__PTZNodeExtension(struct soap *soap, struct tt__PTZNodeExtension *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__PTZNodeExtension);
	if (soap_out_PointerTott__PTZNodeExtension(soap, tag?tag:"tt:PTZNodeExtension", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZNodeExtension ** SOAP_FMAC4 soap_get_PointerTott__PTZNodeExtension(struct soap *soap, struct tt__PTZNodeExtension **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__PTZNodeExtension(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__PTZNode(struct soap *soap, struct tt__PTZNode *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->token);
	soap_default_tt__Name(soap, &a->Name);
	a->SupportedPTZSpaces = NULL;
	soap_default_int(soap, &a->MaximumNumberOfPresets);
	soap_default_xsd__boolean_(soap, &a->HomeSupported);
	a->__sizeAuxiliaryCommands = 0;
	a->AuxiliaryCommands = NULL;
	a->Extension = NULL;
	a->__anyAttribute = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__PTZNode(struct soap *soap, const struct tt__PTZNode *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__Name(soap, &a->Name);
	soap_serialize_PointerTott__PTZSpaces(soap, &a->SupportedPTZSpaces);
	soap_embedded(soap, &a->MaximumNumberOfPresets, SOAP_TYPE_int);
	soap_embedded(soap, &a->HomeSupported, SOAP_TYPE_xsd__boolean_);
	if (a->AuxiliaryCommands)
	{	
		int i;
		for (i = 0; i < a->__sizeAuxiliaryCommands; i++)
		{
			soap_serialize_tt__AuxiliaryData(soap, a->AuxiliaryCommands + i);
		}
	}
	soap_serialize_PointerTott__PTZNodeExtension(soap, &a->Extension);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__PTZNode(struct soap *soap, const char *tag, int id, const struct tt__PTZNode *a, const char *type)
{
	if (a->token)
		soap_set_attr(soap, "token", a->token, 1);
	if (a->__anyAttribute)
		soap_set_attr(soap, "-anyAttribute", a->__anyAttribute, 1);
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__PTZNode), type))
		return soap->error;
	if (soap_out_tt__Name(soap, "tt:Name", -1, &a->Name, ""))
		return soap->error;
	if (a->SupportedPTZSpaces)
	{	if (soap_out_PointerTott__PTZSpaces(soap, "tt:SupportedPTZSpaces", -1, &a->SupportedPTZSpaces, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tt:SupportedPTZSpaces"))
		return soap->error;
	if (soap_out_int(soap, "tt:MaximumNumberOfPresets", -1, &a->MaximumNumberOfPresets, ""))
		return soap->error;
	if (soap_out_xsd__boolean_(soap, "tt:HomeSupported", -1, &a->HomeSupported, ""))
		return soap->error;
	if (a->AuxiliaryCommands)
	{	int i;
	for (i = 0; i < a->__sizeAuxiliaryCommands; i++)
		if (soap_out_tt__AuxiliaryData(soap, "tt:AuxiliaryCommands", -1, a->AuxiliaryCommands + i, ""))
			return soap->error;
	}
	if (soap_out_PointerTott__PTZNodeExtension(soap, "tt:Extension", -1, &a->Extension, ""))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__PTZNode * SOAP_FMAC4 soap_in_tt__PTZNode(struct soap *soap, const char *tag, struct tt__PTZNode *a, const char *type)
{
	size_t soap_flag_Name = 1;
	size_t soap_flag_SupportedPTZSpaces = 1;
	size_t soap_flag_MaximumNumberOfPresets = 1;
	size_t soap_flag_HomeSupported = 1;
	struct soap_blist *soap_blist_AuxiliaryCommands = NULL;
	size_t soap_flag_Extension = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__PTZNode *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__PTZNode, sizeof(struct tt__PTZNode), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__PTZNode(soap, a);
	if (soap_s2string(soap, soap_attr_value(soap, "token", 1), &a->token, 0, 64))
		return NULL;
	if (soap_s2string(soap, soap_attr_value(soap, "-anyAttribute", 0), &a->__anyAttribute, 0, -1))
		return NULL;
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_Name && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_tt__Name(soap, "tt:Name", &a->Name, "tt:Name"))
			{	soap_flag_Name--;
		continue;
		}
		if (soap_flag_SupportedPTZSpaces && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_PointerTott__PTZSpaces(soap, "tt:SupportedPTZSpaces", &a->SupportedPTZSpaces, "tt:PTZSpaces"))
			{	soap_flag_SupportedPTZSpaces--;
		continue;
		}
		if (soap_flag_MaximumNumberOfPresets && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_int(soap, "tt:MaximumNumberOfPresets", &a->MaximumNumberOfPresets, "xsd:int"))
			{	soap_flag_MaximumNumberOfPresets--;
		continue;
		}
		if (soap_flag_HomeSupported && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_xsd__boolean_(soap, "tt:HomeSupported", &a->HomeSupported, "xsd:boolean"))
			{	soap_flag_HomeSupported--;
		continue;
		}
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tt:AuxiliaryCommands", 1, NULL))
		{	if (a->AuxiliaryCommands == NULL)
		{	if (soap_blist_AuxiliaryCommands == NULL)
		soap_blist_AuxiliaryCommands = soap_new_block(soap);
		a->AuxiliaryCommands = (char **)soap_push_block(soap, soap_blist_AuxiliaryCommands, sizeof(char *));
		if (a->AuxiliaryCommands == NULL)
			return NULL;
		*a->AuxiliaryCommands = NULL;
		}
		soap_revert(soap);
		if (soap_in_tt__AuxiliaryData(soap, "tt:AuxiliaryCommands", a->AuxiliaryCommands, "tt:AuxiliaryData"))
		{	a->__sizeAuxiliaryCommands++;
		a->AuxiliaryCommands = NULL;
		continue;
		}
		}
		if (soap_flag_Extension && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_PointerTott__PTZNodeExtension(soap, "tt:Extension", &a->Extension, "tt:PTZNodeExtension"))
			{	soap_flag_Extension--;
		continue;
		}
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (a->AuxiliaryCommands)
			soap_pop_block(soap, soap_blist_AuxiliaryCommands);
		if (a->__sizeAuxiliaryCommands)
			a->AuxiliaryCommands = (char **)soap_save_block(soap, soap_blist_AuxiliaryCommands, NULL, 1);
		else
		{	a->AuxiliaryCommands = NULL;
		if (soap_blist_AuxiliaryCommands)
			soap_end_block(soap, soap_blist_AuxiliaryCommands);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct tt__PTZNode *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__PTZNode, 0, sizeof(struct tt__PTZNode), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_SupportedPTZSpaces > 0 || soap_flag_MaximumNumberOfPresets > 0 || soap_flag_HomeSupported > 0))
	{	soap->error = SOAP_OCCURS;
	return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__PTZNode(struct soap *soap, const struct tt__PTZNode *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__PTZNode);
	if (soap_out_tt__PTZNode(soap, tag?tag:"tt:PTZNode", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZNode * SOAP_FMAC4 soap_get_tt__PTZNode(struct soap *soap, struct tt__PTZNode *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__PTZNode(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__GetNodesResponse(struct soap *soap, struct _tptz__GetNodesResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__sizePTZNode = 0;
	a->PTZNode = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__GetNodesResponse(struct soap *soap, const struct _tptz__GetNodesResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	if (a->PTZNode)
	{	
		int i;
		for (i = 0; i < a->__sizePTZNode; i++)
		{
			soap_embedded(soap, a->PTZNode + i, SOAP_TYPE_tt__PTZNode);
			soap_serialize_tt__PTZNode(soap, a->PTZNode + i);
		}
	}
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__GetNodesResponse(struct soap *soap, const char *tag, int id, const struct _tptz__GetNodesResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__GetNodesResponse), type))
		return soap->error;
	soap_element_result(soap, "-sizePTZNode");
	if (a->PTZNode)
	{	int i;
	for (i = 0; i < a->__sizePTZNode; i++)
		if (soap_out_tt__PTZNode(soap, "tptz:PTZNode", -1, a->PTZNode + i, ""))
			return soap->error;
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__GetNodesResponse * SOAP_FMAC4 soap_in__tptz__GetNodesResponse(struct soap *soap, const char *tag, struct _tptz__GetNodesResponse *a, const char *type)
{
	struct soap_blist *soap_blist_PTZNode = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__GetNodesResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__GetNodesResponse, sizeof(struct _tptz__GetNodesResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__GetNodesResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tptz:PTZNode", 1, NULL))
		{	if (a->PTZNode == NULL)
		{	if (soap_blist_PTZNode == NULL)
		soap_blist_PTZNode = soap_new_block(soap);
		a->PTZNode = (struct tt__PTZNode *)soap_push_block(soap, soap_blist_PTZNode, sizeof(struct tt__PTZNode));
		if (a->PTZNode == NULL)
			return NULL;
		soap_default_tt__PTZNode(soap, a->PTZNode);
		}
		soap_revert(soap);
		if (soap_in_tt__PTZNode(soap, "tptz:PTZNode", a->PTZNode, "tt:PTZNode"))
		{	a->__sizePTZNode++;
		a->PTZNode = NULL;
		continue;
		}
		}
		soap_check_result(soap, "-sizePTZNode");
		if (soap->error == SOAP_TAG_MISMATCH)
			soap->error = soap_ignore_element(soap);
		if (soap->error == SOAP_NO_TAG)
			break;
		if (soap->error)
			return NULL;
		}
		if (a->PTZNode)
			soap_pop_block(soap, soap_blist_PTZNode);
		if (a->__sizePTZNode)
			a->PTZNode = (struct tt__PTZNode *)soap_save_block(soap, soap_blist_PTZNode, NULL, 1);
		else
		{	a->PTZNode = NULL;
		if (soap_blist_PTZNode)
			soap_end_block(soap, soap_blist_PTZNode);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _tptz__GetNodesResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__GetNodesResponse, 0, sizeof(struct _tptz__GetNodesResponse), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__GetNodesResponse(struct soap *soap, const struct _tptz__GetNodesResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__GetNodesResponse);
	if (soap_out__tptz__GetNodesResponse(soap, tag?tag:"tptz:GetNodesResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__GetNodesResponse * SOAP_FMAC4 soap_get__tptz__GetNodesResponse(struct soap *soap, struct _tptz__GetNodesResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__GetNodesResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__GetNode(struct soap *soap, struct _tptz__GetNode *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->NodeToken);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__GetNode(struct soap *soap, const struct _tptz__GetNode *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->NodeToken);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__GetNode(struct soap *soap, const char *tag, int id, const struct _tptz__GetNode *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__GetNode), type))
		return soap->error;
	if (a->NodeToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tptz:NodeToken", -1, &a->NodeToken, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:NodeToken"))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__GetNode * SOAP_FMAC4 soap_in__tptz__GetNode(struct soap *soap, const char *tag, struct _tptz__GetNode *a, const char *type)
{
	size_t soap_flag_NodeToken = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__GetNode *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__GetNode, sizeof(struct _tptz__GetNode), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__GetNode(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_NodeToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_tt__ReferenceToken(soap, "tptz:NodeToken", &a->NodeToken, "tt:ReferenceToken"))
			{	soap_flag_NodeToken--;
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
	{	a = (struct _tptz__GetNode *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__GetNode, 0, sizeof(struct _tptz__GetNode), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_NodeToken > 0))
	{	soap->error = SOAP_OCCURS;
	return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__GetNode(struct soap *soap, const struct _tptz__GetNode *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__GetNode);
	if (soap_out__tptz__GetNode(soap, tag?tag:"tptz:GetNode", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__GetNode * SOAP_FMAC4 soap_get__tptz__GetNode(struct soap *soap, struct _tptz__GetNode *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__GetNode(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tptz__GetNode(struct soap *soap, struct _tptz__GetNode *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tptz__GetNode))
		soap_serialize__tptz__GetNode(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tptz__GetNode(struct soap *soap, const char *tag, int id, struct _tptz__GetNode *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tptz__GetNode);
	if (id < 0)
		return soap->error;
	return soap_out__tptz__GetNode(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tptz__GetNode ** SOAP_FMAC4 soap_in_PointerTo_tptz__GetNode(struct soap *soap, const char *tag, struct _tptz__GetNode **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tptz__GetNode **)soap_malloc(soap, sizeof(struct _tptz__GetNode *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tptz__GetNode(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tptz__GetNode **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tptz__GetNode, sizeof(struct _tptz__GetNode), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tptz__GetNode(struct soap *soap, struct _tptz__GetNode *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tptz__GetNode);
	if (soap_out_PointerTo_tptz__GetNode(soap, tag?tag:"tptz:GetNode", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__GetNode ** SOAP_FMAC4 soap_get_PointerTo_tptz__GetNode(struct soap *soap, struct _tptz__GetNode **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tptz__GetNode(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___tptz__GetNode(struct soap *soap, struct __tptz__GetNode *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->tptz__GetNode = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___tptz__GetNode(struct soap *soap, const struct __tptz__GetNode *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_tptz__GetNode(soap, &a->tptz__GetNode);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___tptz__GetNode(struct soap *soap, const char *tag, int id, const struct __tptz__GetNode *a, const char *type)
{
	if (soap_out_PointerTo_tptz__GetNode(soap, "tptz:GetNode", -1, &a->tptz__GetNode, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__GetNode * SOAP_FMAC4 soap_in___tptz__GetNode(struct soap *soap, const char *tag, struct __tptz__GetNode *a, const char *type)
{
	size_t soap_flag_tptz__GetNode = 1;
	short soap_flag;
	a = (struct __tptz__GetNode *)soap_id_enter(soap, "", a, SOAP_TYPE___tptz__GetNode, sizeof(struct __tptz__GetNode), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___tptz__GetNode(soap, a);
	for (soap_flag = 0;; soap_flag = 1)
	{	soap->error = SOAP_TAG_MISMATCH;
	if (soap_flag_tptz__GetNode && soap->error == SOAP_TAG_MISMATCH)
		if (soap_in_PointerTo_tptz__GetNode(soap, "tptz:GetNode", &a->tptz__GetNode, ""))
		{	soap_flag_tptz__GetNode--;
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

SOAP_FMAC3 int SOAP_FMAC4 soap_put___tptz__GetNode(struct soap *soap, const struct __tptz__GetNode *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___tptz__GetNode(soap, tag?tag:"-tptz:GetNode", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__GetNode * SOAP_FMAC4 soap_get___tptz__GetNode(struct soap *soap, struct __tptz__GetNode *p, const char *tag, const char *type)
{
	if ((p = soap_in___tptz__GetNode(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__PTZNode(struct soap *soap, struct tt__PTZNode *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__PTZNode))
		soap_serialize_tt__PTZNode(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__PTZNode(struct soap *soap, const char *tag, int id, struct tt__PTZNode *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__PTZNode);
	if (id < 0)
		return soap->error;
	return soap_out_tt__PTZNode(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__PTZNode ** SOAP_FMAC4 soap_in_PointerTott__PTZNode(struct soap *soap, const char *tag, struct tt__PTZNode **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__PTZNode **)soap_malloc(soap, sizeof(struct tt__PTZNode *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in_tt__PTZNode(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct tt__PTZNode **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__PTZNode, sizeof(struct tt__PTZNode), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__PTZNode(struct soap *soap, struct tt__PTZNode *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__PTZNode);
	if (soap_out_PointerTott__PTZNode(soap, tag?tag:"tt:PTZNode", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZNode ** SOAP_FMAC4 soap_get_PointerTott__PTZNode(struct soap *soap, struct tt__PTZNode **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__PTZNode(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__GetNodeResponse(struct soap *soap, struct _tptz__GetNodeResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->PTZNode = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__GetNodeResponse(struct soap *soap, const struct _tptz__GetNodeResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTott__PTZNode(soap, &a->PTZNode);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__GetNodeResponse(struct soap *soap, const char *tag, int id, const struct _tptz__GetNodeResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__GetNodeResponse), type))
		return soap->error;
	if (a->PTZNode)
		soap_element_result(soap, "tptz:PTZNode");
	if (a->PTZNode)
	{	if (soap_out_PointerTott__PTZNode(soap, "tptz:PTZNode", -1, &a->PTZNode, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:PTZNode"))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__GetNodeResponse * SOAP_FMAC4 soap_in__tptz__GetNodeResponse(struct soap *soap, const char *tag, struct _tptz__GetNodeResponse *a, const char *type)
{
	size_t soap_flag_PTZNode = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__GetNodeResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__GetNodeResponse, sizeof(struct _tptz__GetNodeResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__GetNodeResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_PTZNode && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_PointerTott__PTZNode(soap, "tptz:PTZNode", &a->PTZNode, "tt:PTZNode"))
			{	soap_flag_PTZNode--;
		continue;
		}
		soap_check_result(soap, "tptz:PTZNode");
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
	{	a = (struct _tptz__GetNodeResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__GetNodeResponse, 0, sizeof(struct _tptz__GetNodeResponse), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_PTZNode > 0))
	{	soap->error = SOAP_OCCURS;
	return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__GetNodeResponse(struct soap *soap, const struct _tptz__GetNodeResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__GetNodeResponse);
	if (soap_out__tptz__GetNodeResponse(soap, tag?tag:"tptz:GetNodeResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__GetNodeResponse * SOAP_FMAC4 soap_get__tptz__GetNodeResponse(struct soap *soap, struct _tptz__GetNodeResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__GetNodeResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}


SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__DurationRange(struct soap *soap, struct tt__DurationRange *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_string(soap, &a->Min);
	soap_default_string(soap, &a->Max);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__DurationRange(struct soap *soap, const struct tt__DurationRange *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_string(soap, &a->Min);
	soap_serialize_string(soap, &a->Max);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__DurationRange(struct soap *soap, const char *tag, int id, const struct tt__DurationRange *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__DurationRange), type))
		return soap->error;
	if (a->Min)
	{	if (soap_out_string(soap, "tt:Min", -1, &a->Min, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tt:Min"))
		return soap->error;
	if (a->Max)
	{	if (soap_out_string(soap, "tt:Max", -1, &a->Max, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tt:Max"))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__DurationRange * SOAP_FMAC4 soap_in_tt__DurationRange(struct soap *soap, const char *tag, struct tt__DurationRange *a, const char *type)
{
	size_t soap_flag_Min = 1;
	size_t soap_flag_Max = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__DurationRange *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__DurationRange, sizeof(struct tt__DurationRange), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__DurationRange(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_Min && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_string(soap, "tt:Min", &a->Min, "xsd:string"))
			{	soap_flag_Min--;
		continue;
		}
		if (soap_flag_Max && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_string(soap, "tt:Max", &a->Max, "xsd:string"))
			{	soap_flag_Max--;
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
	{	a = (struct tt__DurationRange *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__DurationRange, 0, sizeof(struct tt__DurationRange), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_Min > 0 || soap_flag_Max > 0))
	{	soap->error = SOAP_OCCURS;
	return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__DurationRange(struct soap *soap, const struct tt__DurationRange *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__DurationRange);
	if (soap_out_tt__DurationRange(soap, tag?tag:"tt:DurationRange", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__DurationRange * SOAP_FMAC4 soap_get_tt__DurationRange(struct soap *soap, struct tt__DurationRange *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__DurationRange(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__DurationRange(struct soap *soap, struct tt__DurationRange *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__DurationRange))
		soap_serialize_tt__DurationRange(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__DurationRange(struct soap *soap, const char *tag, int id, struct tt__DurationRange *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__DurationRange);
	if (id < 0)
		return soap->error;
	return soap_out_tt__DurationRange(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__DurationRange ** SOAP_FMAC4 soap_in_PointerTott__DurationRange(struct soap *soap, const char *tag, struct tt__DurationRange **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__DurationRange **)soap_malloc(soap, sizeof(struct tt__DurationRange *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in_tt__DurationRange(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct tt__DurationRange **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__DurationRange, sizeof(struct tt__DurationRange), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__DurationRange(struct soap *soap, struct tt__DurationRange *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__DurationRange);
	if (soap_out_PointerTott__DurationRange(soap, tag?tag:"tt:DurationRange", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__DurationRange ** SOAP_FMAC4 soap_get_PointerTott__DurationRange(struct soap *soap, struct tt__DurationRange **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__DurationRange(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}


SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__ContinuousMove(struct soap *soap, struct _tptz__ContinuousMove *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->ProfileToken);
	a->Velocity = NULL;
	soap_default_string(soap, &a->Timeout);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__ContinuousMove(struct soap *soap, const struct _tptz__ContinuousMove *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->ProfileToken);
	soap_serialize_PointerTott__PTZSpeed(soap, &a->Velocity);
	soap_serialize_string(soap, &a->Timeout);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__ContinuousMove(struct soap *soap, const char *tag, int id, const struct _tptz__ContinuousMove *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__ContinuousMove), type))
		return soap->error;
	if (a->ProfileToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tptz:ProfileToken", -1, &a->ProfileToken, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:ProfileToken"))
		return soap->error;
	if (a->Velocity)
	{	if (soap_out_PointerTott__PTZSpeed(soap, "tptz:Velocity", -1, &a->Velocity, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:Velocity"))
		return soap->error;
	if (soap_out_string(soap, "tptz:Timeout", -1, &a->Timeout, ""))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__ContinuousMove * SOAP_FMAC4 soap_in__tptz__ContinuousMove(struct soap *soap, const char *tag, struct _tptz__ContinuousMove *a, const char *type)
{
	size_t soap_flag_ProfileToken = 1;
	size_t soap_flag_Velocity = 1;
	size_t soap_flag_Timeout = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__ContinuousMove *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__ContinuousMove, sizeof(struct _tptz__ContinuousMove), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__ContinuousMove(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_ProfileToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_tt__ReferenceToken(soap, "tptz:ProfileToken", &a->ProfileToken, "tt:ReferenceToken"))
			{	soap_flag_ProfileToken--;
		continue;
		}
		if (soap_flag_Velocity && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_PointerTott__PTZSpeed(soap, "tptz:Velocity", &a->Velocity, "tt:PTZSpeed"))
			{	soap_flag_Velocity--;
		continue;
		}
		if (soap_flag_Timeout && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_string(soap, "tptz:Timeout", &a->Timeout, "xsd:string"))
			{	soap_flag_Timeout--;
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
	{	a = (struct _tptz__ContinuousMove *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__ContinuousMove, 0, sizeof(struct _tptz__ContinuousMove), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_ProfileToken > 0 || soap_flag_Velocity > 0))
	{	soap->error = SOAP_OCCURS;
	return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__ContinuousMove(struct soap *soap, const struct _tptz__ContinuousMove *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__ContinuousMove);
	if (soap_out__tptz__ContinuousMove(soap, tag?tag:"tptz:ContinuousMove", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__ContinuousMove * SOAP_FMAC4 soap_get__tptz__ContinuousMove(struct soap *soap, struct _tptz__ContinuousMove *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__ContinuousMove(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tptz__ContinuousMove(struct soap *soap, struct _tptz__ContinuousMove *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tptz__ContinuousMove))
		soap_serialize__tptz__ContinuousMove(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tptz__ContinuousMove(struct soap *soap, const char *tag, int id, struct _tptz__ContinuousMove *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tptz__ContinuousMove);
	if (id < 0)
		return soap->error;
	return soap_out__tptz__ContinuousMove(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tptz__ContinuousMove ** SOAP_FMAC4 soap_in_PointerTo_tptz__ContinuousMove(struct soap *soap, const char *tag, struct _tptz__ContinuousMove **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tptz__ContinuousMove **)soap_malloc(soap, sizeof(struct _tptz__ContinuousMove *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tptz__ContinuousMove(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tptz__ContinuousMove **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tptz__ContinuousMove, sizeof(struct _tptz__ContinuousMove), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tptz__ContinuousMove(struct soap *soap, struct _tptz__ContinuousMove *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tptz__ContinuousMove);
	if (soap_out_PointerTo_tptz__ContinuousMove(soap, tag?tag:"tptz:ContinuousMove", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__ContinuousMove ** SOAP_FMAC4 soap_get_PointerTo_tptz__ContinuousMove(struct soap *soap, struct _tptz__ContinuousMove **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tptz__ContinuousMove(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___tptz__ContinuousMove(struct soap *soap, struct __tptz__ContinuousMove *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->tptz__ContinuousMove = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___tptz__ContinuousMove(struct soap *soap, const struct __tptz__ContinuousMove *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_tptz__ContinuousMove(soap, &a->tptz__ContinuousMove);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___tptz__ContinuousMove(struct soap *soap, const char *tag, int id, const struct __tptz__ContinuousMove *a, const char *type)
{
	if (soap_out_PointerTo_tptz__ContinuousMove(soap, "tptz:ContinuousMove", -1, &a->tptz__ContinuousMove, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__ContinuousMove * SOAP_FMAC4 soap_in___tptz__ContinuousMove(struct soap *soap, const char *tag, struct __tptz__ContinuousMove *a, const char *type)
{
	size_t soap_flag_tptz__ContinuousMove = 1;
	short soap_flag;
	a = (struct __tptz__ContinuousMove *)soap_id_enter(soap, "", a, SOAP_TYPE___tptz__ContinuousMove, sizeof(struct __tptz__ContinuousMove), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___tptz__ContinuousMove(soap, a);
	for (soap_flag = 0;; soap_flag = 1)
	{	soap->error = SOAP_TAG_MISMATCH;
	if (soap_flag_tptz__ContinuousMove && soap->error == SOAP_TAG_MISMATCH)
		if (soap_in_PointerTo_tptz__ContinuousMove(soap, "tptz:ContinuousMove", &a->tptz__ContinuousMove, ""))
		{	soap_flag_tptz__ContinuousMove--;
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

SOAP_FMAC3 int SOAP_FMAC4 soap_put___tptz__ContinuousMove(struct soap *soap, const struct __tptz__ContinuousMove *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___tptz__ContinuousMove(soap, tag?tag:"-tptz:ContinuousMove", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__ContinuousMove * SOAP_FMAC4 soap_get___tptz__ContinuousMove(struct soap *soap, struct __tptz__ContinuousMove *p, const char *tag, const char *type)
{
	if ((p = soap_in___tptz__ContinuousMove(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__ContinuousMoveResponse(struct soap *soap, struct _tptz__ContinuousMoveResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__ContinuousMoveResponse(struct soap *soap, const struct _tptz__ContinuousMoveResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__ContinuousMoveResponse(struct soap *soap, const char *tag, int id, const struct _tptz__ContinuousMoveResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__ContinuousMoveResponse), type))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__ContinuousMoveResponse * SOAP_FMAC4 soap_in__tptz__ContinuousMoveResponse(struct soap *soap, const char *tag, struct _tptz__ContinuousMoveResponse *a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__ContinuousMoveResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__ContinuousMoveResponse, sizeof(struct _tptz__ContinuousMoveResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__ContinuousMoveResponse(soap, a);
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
	{	a = (struct _tptz__ContinuousMoveResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__ContinuousMoveResponse, 0, sizeof(struct _tptz__ContinuousMoveResponse), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__ContinuousMoveResponse(struct soap *soap, const struct _tptz__ContinuousMoveResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__ContinuousMoveResponse);
	if (soap_out__tptz__ContinuousMoveResponse(soap, tag?tag:"tptz:ContinuousMoveResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__ContinuousMoveResponse * SOAP_FMAC4 soap_get__tptz__ContinuousMoveResponse(struct soap *soap, struct _tptz__ContinuousMoveResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__ContinuousMoveResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__PTZVector(struct soap *soap, struct tt__PTZVector *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->PanTilt = NULL;
	a->Zoom = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__PTZVector(struct soap *soap, const struct tt__PTZVector *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTott__Vector2D(soap, &a->PanTilt);
	soap_serialize_PointerTott__Vector1D(soap, &a->Zoom);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__PTZVector(struct soap *soap, const char *tag, int id, const struct tt__PTZVector *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__PTZVector), type))
		return soap->error;
	if (soap_out_PointerTott__Vector2D(soap, "tt:PanTilt", -1, &a->PanTilt, ""))
		return soap->error;
	if (soap_out_PointerTott__Vector1D(soap, "tt:Zoom", -1, &a->Zoom, ""))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__PTZVector * SOAP_FMAC4 soap_in_tt__PTZVector(struct soap *soap, const char *tag, struct tt__PTZVector *a, const char *type)
{
	size_t soap_flag_PanTilt = 1;
	size_t soap_flag_Zoom = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__PTZVector *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__PTZVector, sizeof(struct tt__PTZVector), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__PTZVector(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_PanTilt && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_PointerTott__Vector2D(soap, "tt:PanTilt", &a->PanTilt, "tt:Vector2D"))
			{	soap_flag_PanTilt--;
		continue;
		}
		if (soap_flag_Zoom && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_PointerTott__Vector1D(soap, "tt:Zoom", &a->Zoom, "tt:Vector1D"))
			{	soap_flag_Zoom--;
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
	{	a = (struct tt__PTZVector *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__PTZVector, 0, sizeof(struct tt__PTZVector), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__PTZVector(struct soap *soap, const struct tt__PTZVector *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__PTZVector);
	if (soap_out_tt__PTZVector(soap, tag?tag:"tt:PTZVector", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZVector * SOAP_FMAC4 soap_get_tt__PTZVector(struct soap *soap, struct tt__PTZVector *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__PTZVector(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__PTZVector(struct soap *soap, struct tt__PTZVector *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__PTZVector))
		soap_serialize_tt__PTZVector(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__PTZVector(struct soap *soap, const char *tag, int id, struct tt__PTZVector *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__PTZVector);
	if (id < 0)
		return soap->error;
	return soap_out_tt__PTZVector(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__PTZVector ** SOAP_FMAC4 soap_in_PointerTott__PTZVector(struct soap *soap, const char *tag, struct tt__PTZVector **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__PTZVector **)soap_malloc(soap, sizeof(struct tt__PTZVector *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in_tt__PTZVector(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct tt__PTZVector **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__PTZVector, sizeof(struct tt__PTZVector), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__PTZVector(struct soap *soap, struct tt__PTZVector *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__PTZVector);
	if (soap_out_PointerTott__PTZVector(soap, tag?tag:"tt:PTZVector", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZVector ** SOAP_FMAC4 soap_get_PointerTott__PTZVector(struct soap *soap, struct tt__PTZVector **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__PTZVector(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__RelativeMove(struct soap *soap, struct _tptz__RelativeMove *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->ProfileToken);
	a->Translation = NULL;
	a->Speed = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__RelativeMove(struct soap *soap, const struct _tptz__RelativeMove *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->ProfileToken);
	soap_serialize_PointerTott__PTZVector(soap, &a->Translation);
	soap_serialize_PointerTott__PTZSpeed(soap, &a->Speed);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__RelativeMove(struct soap *soap, const char *tag, int id, const struct _tptz__RelativeMove *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__RelativeMove), type))
		return soap->error;
	if (a->ProfileToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tptz:ProfileToken", -1, &a->ProfileToken, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:ProfileToken"))
		return soap->error;
	if (a->Translation)
	{	if (soap_out_PointerTott__PTZVector(soap, "tptz:Translation", -1, &a->Translation, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:Translation"))
		return soap->error;
	if (soap_out_PointerTott__PTZSpeed(soap, "tptz:Speed", -1, &a->Speed, ""))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__RelativeMove * SOAP_FMAC4 soap_in__tptz__RelativeMove(struct soap *soap, const char *tag, struct _tptz__RelativeMove *a, const char *type)
{
	size_t soap_flag_ProfileToken = 1;
	size_t soap_flag_Translation = 1;
	size_t soap_flag_Speed = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__RelativeMove *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__RelativeMove, sizeof(struct _tptz__RelativeMove), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__RelativeMove(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_ProfileToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_tt__ReferenceToken(soap, "tptz:ProfileToken", &a->ProfileToken, "tt:ReferenceToken"))
			{	soap_flag_ProfileToken--;
		continue;
		}
		if (soap_flag_Translation && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_PointerTott__PTZVector(soap, "tptz:Translation", &a->Translation, "tt:PTZVector"))
			{	soap_flag_Translation--;
		continue;
		}
		if (soap_flag_Speed && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_PointerTott__PTZSpeed(soap, "tptz:Speed", &a->Speed, "tt:PTZSpeed"))
			{	soap_flag_Speed--;
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
	{	a = (struct _tptz__RelativeMove *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__RelativeMove, 0, sizeof(struct _tptz__RelativeMove), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_ProfileToken > 0 || soap_flag_Translation > 0))
	{	soap->error = SOAP_OCCURS;
	return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__RelativeMove(struct soap *soap, const struct _tptz__RelativeMove *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__RelativeMove);
	if (soap_out__tptz__RelativeMove(soap, tag?tag:"tptz:RelativeMove", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__RelativeMove * SOAP_FMAC4 soap_get__tptz__RelativeMove(struct soap *soap, struct _tptz__RelativeMove *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__RelativeMove(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tptz__RelativeMove(struct soap *soap, struct _tptz__RelativeMove *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tptz__RelativeMove))
		soap_serialize__tptz__RelativeMove(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tptz__RelativeMove(struct soap *soap, const char *tag, int id, struct _tptz__RelativeMove *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tptz__RelativeMove);
	if (id < 0)
		return soap->error;
	return soap_out__tptz__RelativeMove(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tptz__RelativeMove ** SOAP_FMAC4 soap_in_PointerTo_tptz__RelativeMove(struct soap *soap, const char *tag, struct _tptz__RelativeMove **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tptz__RelativeMove **)soap_malloc(soap, sizeof(struct _tptz__RelativeMove *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tptz__RelativeMove(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tptz__RelativeMove **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tptz__RelativeMove, sizeof(struct _tptz__RelativeMove), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tptz__RelativeMove(struct soap *soap, struct _tptz__RelativeMove *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tptz__RelativeMove);
	if (soap_out_PointerTo_tptz__RelativeMove(soap, tag?tag:"tptz:RelativeMove", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__RelativeMove ** SOAP_FMAC4 soap_get_PointerTo_tptz__RelativeMove(struct soap *soap, struct _tptz__RelativeMove **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tptz__RelativeMove(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___tptz__RelativeMove(struct soap *soap, struct __tptz__RelativeMove *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->tptz__RelativeMove = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___tptz__RelativeMove(struct soap *soap, const struct __tptz__RelativeMove *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_tptz__RelativeMove(soap, &a->tptz__RelativeMove);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___tptz__RelativeMove(struct soap *soap, const char *tag, int id, const struct __tptz__RelativeMove *a, const char *type)
{
	if (soap_out_PointerTo_tptz__RelativeMove(soap, "tptz:RelativeMove", -1, &a->tptz__RelativeMove, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__RelativeMove * SOAP_FMAC4 soap_in___tptz__RelativeMove(struct soap *soap, const char *tag, struct __tptz__RelativeMove *a, const char *type)
{
	size_t soap_flag_tptz__RelativeMove = 1;
	short soap_flag;
	a = (struct __tptz__RelativeMove *)soap_id_enter(soap, "", a, SOAP_TYPE___tptz__RelativeMove, sizeof(struct __tptz__RelativeMove), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___tptz__RelativeMove(soap, a);
	for (soap_flag = 0;; soap_flag = 1)
	{	soap->error = SOAP_TAG_MISMATCH;
	if (soap_flag_tptz__RelativeMove && soap->error == SOAP_TAG_MISMATCH)
		if (soap_in_PointerTo_tptz__RelativeMove(soap, "tptz:RelativeMove", &a->tptz__RelativeMove, ""))
		{	soap_flag_tptz__RelativeMove--;
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

SOAP_FMAC3 int SOAP_FMAC4 soap_put___tptz__RelativeMove(struct soap *soap, const struct __tptz__RelativeMove *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___tptz__RelativeMove(soap, tag?tag:"-tptz:RelativeMove", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__RelativeMove * SOAP_FMAC4 soap_get___tptz__RelativeMove(struct soap *soap, struct __tptz__RelativeMove *p, const char *tag, const char *type)
{
	if ((p = soap_in___tptz__RelativeMove(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__RelativeMoveResponse(struct soap *soap, struct _tptz__RelativeMoveResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__RelativeMoveResponse(struct soap *soap, const struct _tptz__RelativeMoveResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__RelativeMoveResponse(struct soap *soap, const char *tag, int id, const struct _tptz__RelativeMoveResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__RelativeMoveResponse), type))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__RelativeMoveResponse * SOAP_FMAC4 soap_in__tptz__RelativeMoveResponse(struct soap *soap, const char *tag, struct _tptz__RelativeMoveResponse *a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__RelativeMoveResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__RelativeMoveResponse, sizeof(struct _tptz__RelativeMoveResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__RelativeMoveResponse(soap, a);
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
	{	a = (struct _tptz__RelativeMoveResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__RelativeMoveResponse, 0, sizeof(struct _tptz__RelativeMoveResponse), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__RelativeMoveResponse(struct soap *soap, const struct _tptz__RelativeMoveResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__RelativeMoveResponse);
	if (soap_out__tptz__RelativeMoveResponse(soap, tag?tag:"tptz:RelativeMoveResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__RelativeMoveResponse * SOAP_FMAC4 soap_get__tptz__RelativeMoveResponse(struct soap *soap, struct _tptz__RelativeMoveResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__RelativeMoveResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__SendAuxiliaryCommand(struct soap *soap, struct _tptz__SendAuxiliaryCommand *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->ProfileToken);
	soap_default_tt__AuxiliaryData(soap, &a->AuxiliaryData);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__SendAuxiliaryCommand(struct soap *soap, const struct _tptz__SendAuxiliaryCommand *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->ProfileToken);
	soap_serialize_tt__AuxiliaryData(soap, &a->AuxiliaryData);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__SendAuxiliaryCommand(struct soap *soap, const char *tag, int id, const struct _tptz__SendAuxiliaryCommand *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__SendAuxiliaryCommand), type))
		return soap->error;
	if (a->ProfileToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tptz:ProfileToken", -1, &a->ProfileToken, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:ProfileToken"))
		return soap->error;
	if (a->AuxiliaryData)
	{	if (soap_out_tt__AuxiliaryData(soap, "tptz:AuxiliaryData", -1, &a->AuxiliaryData, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:AuxiliaryData"))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__SendAuxiliaryCommand * SOAP_FMAC4 soap_in__tptz__SendAuxiliaryCommand(struct soap *soap, const char *tag, struct _tptz__SendAuxiliaryCommand *a, const char *type)
{
	size_t soap_flag_ProfileToken = 1;
	size_t soap_flag_AuxiliaryData = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__SendAuxiliaryCommand *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__SendAuxiliaryCommand, sizeof(struct _tptz__SendAuxiliaryCommand), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__SendAuxiliaryCommand(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_ProfileToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_tt__ReferenceToken(soap, "tptz:ProfileToken", &a->ProfileToken, "tt:ReferenceToken"))
			{	soap_flag_ProfileToken--;
		continue;
		}
		if (soap_flag_AuxiliaryData && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_tt__AuxiliaryData(soap, "tptz:AuxiliaryData", &a->AuxiliaryData, "tt:AuxiliaryData"))
			{	soap_flag_AuxiliaryData--;
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
	{	a = (struct _tptz__SendAuxiliaryCommand *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__SendAuxiliaryCommand, 0, sizeof(struct _tptz__SendAuxiliaryCommand), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_ProfileToken > 0 || soap_flag_AuxiliaryData > 0))
	{	soap->error = SOAP_OCCURS;
	return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__SendAuxiliaryCommand(struct soap *soap, const struct _tptz__SendAuxiliaryCommand *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__SendAuxiliaryCommand);
	if (soap_out__tptz__SendAuxiliaryCommand(soap, tag?tag:"tptz:SendAuxiliaryCommand", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__SendAuxiliaryCommand * SOAP_FMAC4 soap_get__tptz__SendAuxiliaryCommand(struct soap *soap, struct _tptz__SendAuxiliaryCommand *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__SendAuxiliaryCommand(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tptz__SendAuxiliaryCommand(struct soap *soap, struct _tptz__SendAuxiliaryCommand *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tptz__SendAuxiliaryCommand))
		soap_serialize__tptz__SendAuxiliaryCommand(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tptz__SendAuxiliaryCommand(struct soap *soap, const char *tag, int id, struct _tptz__SendAuxiliaryCommand *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tptz__SendAuxiliaryCommand);
	if (id < 0)
		return soap->error;
	return soap_out__tptz__SendAuxiliaryCommand(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tptz__SendAuxiliaryCommand ** SOAP_FMAC4 soap_in_PointerTo_tptz__SendAuxiliaryCommand(struct soap *soap, const char *tag, struct _tptz__SendAuxiliaryCommand **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tptz__SendAuxiliaryCommand **)soap_malloc(soap, sizeof(struct _tptz__SendAuxiliaryCommand *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tptz__SendAuxiliaryCommand(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tptz__SendAuxiliaryCommand **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tptz__SendAuxiliaryCommand, sizeof(struct _tptz__SendAuxiliaryCommand), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tptz__SendAuxiliaryCommand(struct soap *soap, struct _tptz__SendAuxiliaryCommand *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tptz__SendAuxiliaryCommand);
	if (soap_out_PointerTo_tptz__SendAuxiliaryCommand(soap, tag?tag:"tptz:SendAuxiliaryCommand", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__SendAuxiliaryCommand ** SOAP_FMAC4 soap_get_PointerTo_tptz__SendAuxiliaryCommand(struct soap *soap, struct _tptz__SendAuxiliaryCommand **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tptz__SendAuxiliaryCommand(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___tptz__SendAuxiliaryCommand(struct soap *soap, struct __tptz__SendAuxiliaryCommand *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->tptz__SendAuxiliaryCommand = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___tptz__SendAuxiliaryCommand(struct soap *soap, const struct __tptz__SendAuxiliaryCommand *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_tptz__SendAuxiliaryCommand(soap, &a->tptz__SendAuxiliaryCommand);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___tptz__SendAuxiliaryCommand(struct soap *soap, const char *tag, int id, const struct __tptz__SendAuxiliaryCommand *a, const char *type)
{
	if (soap_out_PointerTo_tptz__SendAuxiliaryCommand(soap, "tptz:SendAuxiliaryCommand", -1, &a->tptz__SendAuxiliaryCommand, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__SendAuxiliaryCommand * SOAP_FMAC4 soap_in___tptz__SendAuxiliaryCommand(struct soap *soap, const char *tag, struct __tptz__SendAuxiliaryCommand *a, const char *type)
{
	size_t soap_flag_tptz__SendAuxiliaryCommand = 1;
	short soap_flag;
	a = (struct __tptz__SendAuxiliaryCommand *)soap_id_enter(soap, "", a, SOAP_TYPE___tptz__SendAuxiliaryCommand, sizeof(struct __tptz__SendAuxiliaryCommand), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___tptz__SendAuxiliaryCommand(soap, a);
	for (soap_flag = 0;; soap_flag = 1)
	{	soap->error = SOAP_TAG_MISMATCH;
	if (soap_flag_tptz__SendAuxiliaryCommand && soap->error == SOAP_TAG_MISMATCH)
		if (soap_in_PointerTo_tptz__SendAuxiliaryCommand(soap, "tptz:SendAuxiliaryCommand", &a->tptz__SendAuxiliaryCommand, ""))
		{	soap_flag_tptz__SendAuxiliaryCommand--;
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

SOAP_FMAC3 int SOAP_FMAC4 soap_put___tptz__SendAuxiliaryCommand(struct soap *soap, const struct __tptz__SendAuxiliaryCommand *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___tptz__SendAuxiliaryCommand(soap, tag?tag:"-tptz:SendAuxiliaryCommand", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__SendAuxiliaryCommand * SOAP_FMAC4 soap_get___tptz__SendAuxiliaryCommand(struct soap *soap, struct __tptz__SendAuxiliaryCommand *p, const char *tag, const char *type)
{
	if ((p = soap_in___tptz__SendAuxiliaryCommand(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__SendAuxiliaryCommandResponse(struct soap *soap, struct _tptz__SendAuxiliaryCommandResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__AuxiliaryData(soap, &a->AuxiliaryResponse);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__SendAuxiliaryCommandResponse(struct soap *soap, const struct _tptz__SendAuxiliaryCommandResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__AuxiliaryData(soap, &a->AuxiliaryResponse);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__SendAuxiliaryCommandResponse(struct soap *soap, const char *tag, int id, const struct _tptz__SendAuxiliaryCommandResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__SendAuxiliaryCommandResponse), type))
		return soap->error;
	if (a->AuxiliaryResponse)
		soap_element_result(soap, "tptz:AuxiliaryResponse");
	if (a->AuxiliaryResponse)
	{	if (soap_out_tt__AuxiliaryData(soap, "tptz:AuxiliaryResponse", -1, &a->AuxiliaryResponse, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:AuxiliaryResponse"))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__SendAuxiliaryCommandResponse * SOAP_FMAC4 soap_in__tptz__SendAuxiliaryCommandResponse(struct soap *soap, const char *tag, struct _tptz__SendAuxiliaryCommandResponse *a, const char *type)
{
	size_t soap_flag_AuxiliaryResponse = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__SendAuxiliaryCommandResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__SendAuxiliaryCommandResponse, sizeof(struct _tptz__SendAuxiliaryCommandResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__SendAuxiliaryCommandResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_AuxiliaryResponse && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_tt__AuxiliaryData(soap, "tptz:AuxiliaryResponse", &a->AuxiliaryResponse, "tt:AuxiliaryData"))
			{	soap_flag_AuxiliaryResponse--;
		continue;
		}
		soap_check_result(soap, "tptz:AuxiliaryResponse");
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
	{	a = (struct _tptz__SendAuxiliaryCommandResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__SendAuxiliaryCommandResponse, 0, sizeof(struct _tptz__SendAuxiliaryCommandResponse), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_AuxiliaryResponse > 0))
	{	soap->error = SOAP_OCCURS;
	return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__SendAuxiliaryCommandResponse(struct soap *soap, const struct _tptz__SendAuxiliaryCommandResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__SendAuxiliaryCommandResponse);
	if (soap_out__tptz__SendAuxiliaryCommandResponse(soap, tag?tag:"tptz:SendAuxiliaryCommandResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__SendAuxiliaryCommandResponse * SOAP_FMAC4 soap_get__tptz__SendAuxiliaryCommandResponse(struct soap *soap, struct _tptz__SendAuxiliaryCommandResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__SendAuxiliaryCommandResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__AbsoluteMove(struct soap *soap, struct _tptz__AbsoluteMove *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->ProfileToken);
	a->Position = NULL;
	a->Speed = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__AbsoluteMove(struct soap *soap, const struct _tptz__AbsoluteMove *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->ProfileToken);
	soap_serialize_PointerTott__PTZVector(soap, &a->Position);
	soap_serialize_PointerTott__PTZSpeed(soap, &a->Speed);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__AbsoluteMove(struct soap *soap, const char *tag, int id, const struct _tptz__AbsoluteMove *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__AbsoluteMove), type))
		return soap->error;
	if (a->ProfileToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tptz:ProfileToken", -1, &a->ProfileToken, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:ProfileToken"))
		return soap->error;
	if (a->Position)
	{	if (soap_out_PointerTott__PTZVector(soap, "tptz:Position", -1, &a->Position, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:Position"))
		return soap->error;
	if (soap_out_PointerTott__PTZSpeed(soap, "tptz:Speed", -1, &a->Speed, ""))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__AbsoluteMove * SOAP_FMAC4 soap_in__tptz__AbsoluteMove(struct soap *soap, const char *tag, struct _tptz__AbsoluteMove *a, const char *type)
{
	size_t soap_flag_ProfileToken = 1;
	size_t soap_flag_Position = 1;
	size_t soap_flag_Speed = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__AbsoluteMove *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__AbsoluteMove, sizeof(struct _tptz__AbsoluteMove), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__AbsoluteMove(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_ProfileToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_tt__ReferenceToken(soap, "tptz:ProfileToken", &a->ProfileToken, "tt:ReferenceToken"))
			{	soap_flag_ProfileToken--;
		continue;
		}
		if (soap_flag_Position && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_PointerTott__PTZVector(soap, "tptz:Position", &a->Position, "tt:PTZVector"))
			{	soap_flag_Position--;
		continue;
		}
		if (soap_flag_Speed && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_PointerTott__PTZSpeed(soap, "tptz:Speed", &a->Speed, "tt:PTZSpeed"))
			{	soap_flag_Speed--;
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
	{	a = (struct _tptz__AbsoluteMove *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__AbsoluteMove, 0, sizeof(struct _tptz__AbsoluteMove), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_ProfileToken > 0 || soap_flag_Position > 0))
	{	soap->error = SOAP_OCCURS;
	return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__AbsoluteMove(struct soap *soap, const struct _tptz__AbsoluteMove *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__AbsoluteMove);
	if (soap_out__tptz__AbsoluteMove(soap, tag?tag:"tptz:AbsoluteMove", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__AbsoluteMove * SOAP_FMAC4 soap_get__tptz__AbsoluteMove(struct soap *soap, struct _tptz__AbsoluteMove *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__AbsoluteMove(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tptz__AbsoluteMove(struct soap *soap, struct _tptz__AbsoluteMove *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tptz__AbsoluteMove))
		soap_serialize__tptz__AbsoluteMove(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tptz__AbsoluteMove(struct soap *soap, const char *tag, int id, struct _tptz__AbsoluteMove *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tptz__AbsoluteMove);
	if (id < 0)
		return soap->error;
	return soap_out__tptz__AbsoluteMove(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tptz__AbsoluteMove ** SOAP_FMAC4 soap_in_PointerTo_tptz__AbsoluteMove(struct soap *soap, const char *tag, struct _tptz__AbsoluteMove **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tptz__AbsoluteMove **)soap_malloc(soap, sizeof(struct _tptz__AbsoluteMove *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tptz__AbsoluteMove(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tptz__AbsoluteMove **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tptz__AbsoluteMove, sizeof(struct _tptz__AbsoluteMove), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tptz__AbsoluteMove(struct soap *soap, struct _tptz__AbsoluteMove *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tptz__AbsoluteMove);
	if (soap_out_PointerTo_tptz__AbsoluteMove(soap, tag?tag:"tptz:AbsoluteMove", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__AbsoluteMove ** SOAP_FMAC4 soap_get_PointerTo_tptz__AbsoluteMove(struct soap *soap, struct _tptz__AbsoluteMove **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tptz__AbsoluteMove(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___tptz__AbsoluteMove(struct soap *soap, struct __tptz__AbsoluteMove *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->tptz__AbsoluteMove = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___tptz__AbsoluteMove(struct soap *soap, const struct __tptz__AbsoluteMove *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_tptz__AbsoluteMove(soap, &a->tptz__AbsoluteMove);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___tptz__AbsoluteMove(struct soap *soap, const char *tag, int id, const struct __tptz__AbsoluteMove *a, const char *type)
{
	if (soap_out_PointerTo_tptz__AbsoluteMove(soap, "tptz:AbsoluteMove", -1, &a->tptz__AbsoluteMove, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__AbsoluteMove * SOAP_FMAC4 soap_in___tptz__AbsoluteMove(struct soap *soap, const char *tag, struct __tptz__AbsoluteMove *a, const char *type)
{
	size_t soap_flag_tptz__AbsoluteMove = 1;
	short soap_flag;
	a = (struct __tptz__AbsoluteMove *)soap_id_enter(soap, "", a, SOAP_TYPE___tptz__AbsoluteMove, sizeof(struct __tptz__AbsoluteMove), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___tptz__AbsoluteMove(soap, a);
	for (soap_flag = 0;; soap_flag = 1)
	{	soap->error = SOAP_TAG_MISMATCH;
	if (soap_flag_tptz__AbsoluteMove && soap->error == SOAP_TAG_MISMATCH)
		if (soap_in_PointerTo_tptz__AbsoluteMove(soap, "tptz:AbsoluteMove", &a->tptz__AbsoluteMove, ""))
		{	soap_flag_tptz__AbsoluteMove--;
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

SOAP_FMAC3 int SOAP_FMAC4 soap_put___tptz__AbsoluteMove(struct soap *soap, const struct __tptz__AbsoluteMove *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___tptz__AbsoluteMove(soap, tag?tag:"-tptz:AbsoluteMove", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__AbsoluteMove * SOAP_FMAC4 soap_get___tptz__AbsoluteMove(struct soap *soap, struct __tptz__AbsoluteMove *p, const char *tag, const char *type)
{
	if ((p = soap_in___tptz__AbsoluteMove(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__AbsoluteMoveResponse(struct soap *soap, struct _tptz__AbsoluteMoveResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__AbsoluteMoveResponse(struct soap *soap, const struct _tptz__AbsoluteMoveResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__AbsoluteMoveResponse(struct soap *soap, const char *tag, int id, const struct _tptz__AbsoluteMoveResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__AbsoluteMoveResponse), type))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__AbsoluteMoveResponse * SOAP_FMAC4 soap_in__tptz__AbsoluteMoveResponse(struct soap *soap, const char *tag, struct _tptz__AbsoluteMoveResponse *a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__AbsoluteMoveResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__AbsoluteMoveResponse, sizeof(struct _tptz__AbsoluteMoveResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__AbsoluteMoveResponse(soap, a);
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
	{	a = (struct _tptz__AbsoluteMoveResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__AbsoluteMoveResponse, 0, sizeof(struct _tptz__AbsoluteMoveResponse), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__AbsoluteMoveResponse(struct soap *soap, const struct _tptz__AbsoluteMoveResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__AbsoluteMoveResponse);
	if (soap_out__tptz__AbsoluteMoveResponse(soap, tag?tag:"tptz:AbsoluteMoveResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__AbsoluteMoveResponse * SOAP_FMAC4 soap_get__tptz__AbsoluteMoveResponse(struct soap *soap, struct _tptz__AbsoluteMoveResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__AbsoluteMoveResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}


SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__Stop(struct soap *soap, struct _tptz__Stop *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->ProfileToken);
	a->PanTilt = NULL;
	a->Zoom = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__Stop(struct soap *soap, const struct _tptz__Stop *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->ProfileToken);
	soap_serialize_PointerToxsd__boolean_(soap, &a->PanTilt);
	soap_serialize_PointerToxsd__boolean_(soap, &a->Zoom);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__Stop(struct soap *soap, const char *tag, int id, const struct _tptz__Stop *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__Stop), type))
		return soap->error;
	if (a->ProfileToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tptz:ProfileToken", -1, &a->ProfileToken, ""))
	return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:ProfileToken"))
		return soap->error;
	if (soap_out_PointerToxsd__boolean_(soap, "tptz:PanTilt", -1, &a->PanTilt, ""))
		return soap->error;
	if (soap_out_PointerToxsd__boolean_(soap, "tptz:Zoom", -1, &a->Zoom, ""))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__Stop * SOAP_FMAC4 soap_in__tptz__Stop(struct soap *soap, const char *tag, struct _tptz__Stop *a, const char *type)
{
	size_t soap_flag_ProfileToken = 1;
	size_t soap_flag_PanTilt = 1;
	size_t soap_flag_Zoom = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__Stop *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__Stop, sizeof(struct _tptz__Stop), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__Stop(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_ProfileToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
			if (soap_in_tt__ReferenceToken(soap, "tptz:ProfileToken", &a->ProfileToken, "tt:ReferenceToken"))
			{	soap_flag_ProfileToken--;
		continue;
		}
		if (soap_flag_PanTilt && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_PointerToxsd__boolean_(soap, "tptz:PanTilt", &a->PanTilt, "xsd:boolean"))
			{	soap_flag_PanTilt--;
		continue;
		}
		if (soap_flag_Zoom && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_PointerToxsd__boolean_(soap, "tptz:Zoom", &a->Zoom, "xsd:boolean"))
			{	soap_flag_Zoom--;
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
	{	a = (struct _tptz__Stop *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__Stop, 0, sizeof(struct _tptz__Stop), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_ProfileToken > 0))
	{	soap->error = SOAP_OCCURS;
	return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__Stop(struct soap *soap, const struct _tptz__Stop *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__Stop);
	if (soap_out__tptz__Stop(soap, tag?tag:"tptz:Stop", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__Stop * SOAP_FMAC4 soap_get__tptz__Stop(struct soap *soap, struct _tptz__Stop *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__Stop(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tptz__Stop(struct soap *soap, struct _tptz__Stop *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tptz__Stop))
		soap_serialize__tptz__Stop(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tptz__Stop(struct soap *soap, const char *tag, int id, struct _tptz__Stop *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tptz__Stop);
	if (id < 0)
		return soap->error;
	return soap_out__tptz__Stop(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tptz__Stop ** SOAP_FMAC4 soap_in_PointerTo_tptz__Stop(struct soap *soap, const char *tag, struct _tptz__Stop **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tptz__Stop **)soap_malloc(soap, sizeof(struct _tptz__Stop *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
	if (!(*a = soap_in__tptz__Stop(soap, tag, *a, type)))
		return NULL;
	}
	else
	{	a = (struct _tptz__Stop **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tptz__Stop, sizeof(struct _tptz__Stop), 0);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tptz__Stop(struct soap *soap, struct _tptz__Stop *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tptz__Stop);
	if (soap_out_PointerTo_tptz__Stop(soap, tag?tag:"tptz:Stop", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__Stop ** SOAP_FMAC4 soap_get_PointerTo_tptz__Stop(struct soap *soap, struct _tptz__Stop **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tptz__Stop(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___tptz__Stop(struct soap *soap, struct __tptz__Stop *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->tptz__Stop = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___tptz__Stop(struct soap *soap, const struct __tptz__Stop *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_tptz__Stop(soap, &a->tptz__Stop);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___tptz__Stop(struct soap *soap, const char *tag, int id, const struct __tptz__Stop *a, const char *type)
{
	if (soap_out_PointerTo_tptz__Stop(soap, "tptz:Stop", -1, &a->tptz__Stop, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__Stop * SOAP_FMAC4 soap_in___tptz__Stop(struct soap *soap, const char *tag, struct __tptz__Stop *a, const char *type)
{
	size_t soap_flag_tptz__Stop = 1;
	short soap_flag;
	a = (struct __tptz__Stop *)soap_id_enter(soap, "", a, SOAP_TYPE___tptz__Stop, sizeof(struct __tptz__Stop), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___tptz__Stop(soap, a);
	for (soap_flag = 0;; soap_flag = 1)
	{	soap->error = SOAP_TAG_MISMATCH;
	if (soap_flag_tptz__Stop && soap->error == SOAP_TAG_MISMATCH)
		if (soap_in_PointerTo_tptz__Stop(soap, "tptz:Stop", &a->tptz__Stop, ""))
		{	soap_flag_tptz__Stop--;
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

SOAP_FMAC3 int SOAP_FMAC4 soap_put___tptz__Stop(struct soap *soap, const struct __tptz__Stop *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___tptz__Stop(soap, tag?tag:"-tptz:Stop", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__Stop * SOAP_FMAC4 soap_get___tptz__Stop(struct soap *soap, struct __tptz__Stop *p, const char *tag, const char *type)
{
	if ((p = soap_in___tptz__Stop(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__StopResponse(struct soap *soap, struct _tptz__StopResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__StopResponse(struct soap *soap, const struct _tptz__StopResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__StopResponse(struct soap *soap, const char *tag, int id, const struct _tptz__StopResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__StopResponse), type))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__StopResponse * SOAP_FMAC4 soap_in__tptz__StopResponse(struct soap *soap, const char *tag, struct _tptz__StopResponse *a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__StopResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__StopResponse, sizeof(struct _tptz__StopResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__StopResponse(soap, a);
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
	{	a = (struct _tptz__StopResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__StopResponse, 0, sizeof(struct _tptz__StopResponse), 0, NULL);
	if (soap->body && soap_element_end_in(soap, tag))
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__StopResponse(struct soap *soap, const struct _tptz__StopResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__StopResponse);
	if (soap_out__tptz__StopResponse(soap, tag?tag:"tptz:StopResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__StopResponse * SOAP_FMAC4 soap_get__tptz__StopResponse(struct soap *soap, struct _tptz__StopResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__StopResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__PTZPresetTourOperation(struct soap *soap, enum tt__PTZPresetTourOperation *a)
{
	(void)soap; /* appease -Wall -Werror */
#ifdef SOAP_DEFAULT_tt__PTZPresetTourOperation
	*a = SOAP_DEFAULT_tt__PTZPresetTourOperation;
#else
	*a = (enum tt__PTZPresetTourOperation)0;
#endif
}

static const struct soap_code_map soap_codes_tt__PTZPresetTourOperation[] =
{	{ (long)tt__PTZPresetTourOperation__Start, "Start" },
	{ (long)tt__PTZPresetTourOperation__Stop, "Stop" },
	{ (long)tt__PTZPresetTourOperation__Pause, "Pause" },
	{ (long)tt__PTZPresetTourOperation__Extended, "Extended" },
	{ 0, NULL }
};

SOAP_FMAC3S const char* SOAP_FMAC4S soap_tt__PTZPresetTourOperation2s(struct soap *soap, enum tt__PTZPresetTourOperation n)
{	const char *s = soap_code_str(soap_codes_tt__PTZPresetTourOperation, (long)n);
	if (s)
		return s;
	return soap_long2s(soap, (long)n);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__PTZPresetTourOperation(struct soap *soap, const char *tag, int id, const enum tt__PTZPresetTourOperation *a, const char *type)
{	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__PTZPresetTourOperation), type) || soap_send(soap, soap_tt__PTZPresetTourOperation2s(soap, *a)))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3S int SOAP_FMAC4S soap_s2tt__PTZPresetTourOperation(struct soap *soap, const char *s, enum tt__PTZPresetTourOperation *a)
{
	const struct soap_code_map *map;
	if (!s)
		return soap->error;
	map = soap_code(soap_codes_tt__PTZPresetTourOperation, s);
	if (map)
		*a = (enum tt__PTZPresetTourOperation)map->code;
	else
	{	long n;
		if (soap_s2long(soap, s, &n) || ((soap->mode & SOAP_XML_STRICT) && (n < 0 || n > 3)))
			return soap->error = SOAP_TYPE;
		*a = (enum tt__PTZPresetTourOperation)n;
	}
	return SOAP_OK;
}

SOAP_FMAC3 enum tt__PTZPresetTourOperation * SOAP_FMAC4 soap_in_tt__PTZPresetTourOperation(struct soap *soap, const char *tag, enum tt__PTZPresetTourOperation *a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (enum tt__PTZPresetTourOperation *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__PTZPresetTourOperation, sizeof(enum tt__PTZPresetTourOperation), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	if (soap->body && !*soap->href)
	{	if (!a || soap_s2tt__PTZPresetTourOperation(soap, soap_value(soap), a) || soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (enum tt__PTZPresetTourOperation *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__PTZPresetTourOperation, 0, sizeof(enum tt__PTZPresetTourOperation), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__PTZPresetTourOperation(struct soap *soap, const enum tt__PTZPresetTourOperation *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__PTZPresetTourOperation);
	if (soap_out_tt__PTZPresetTourOperation(soap, tag?tag:"tt:PTZPresetTourOperation", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 enum tt__PTZPresetTourOperation * SOAP_FMAC4 soap_get_tt__PTZPresetTourOperation(struct soap *soap, enum tt__PTZPresetTourOperation *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__PTZPresetTourOperation(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__PTZPresetTourDirection(struct soap *soap, enum tt__PTZPresetTourDirection *a)
{
	(void)soap; /* appease -Wall -Werror */
#ifdef SOAP_DEFAULT_tt__PTZPresetTourDirection
	*a = SOAP_DEFAULT_tt__PTZPresetTourDirection;
#else
	*a = (enum tt__PTZPresetTourDirection)0;
#endif
}

static const struct soap_code_map soap_codes_tt__PTZPresetTourDirection[] =
{	{ (long)tt__PTZPresetTourDirection__Forward, "Forward" },
	{ (long)tt__PTZPresetTourDirection__Backward, "Backward" },
	{ (long)tt__PTZPresetTourDirection__Extended, "Extended" },
	{ 0, NULL }
};

SOAP_FMAC3S const char* SOAP_FMAC4S soap_tt__PTZPresetTourDirection2s(struct soap *soap, enum tt__PTZPresetTourDirection n)
{	const char *s = soap_code_str(soap_codes_tt__PTZPresetTourDirection, (long)n);
	if (s)
		return s;
	return soap_long2s(soap, (long)n);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__PTZPresetTourDirection(struct soap *soap, const char *tag, int id, const enum tt__PTZPresetTourDirection *a, const char *type)
{	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__PTZPresetTourDirection), type) || soap_send(soap, soap_tt__PTZPresetTourDirection2s(soap, *a)))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3S int SOAP_FMAC4S soap_s2tt__PTZPresetTourDirection(struct soap *soap, const char *s, enum tt__PTZPresetTourDirection *a)
{
	const struct soap_code_map *map;
	if (!s)
		return soap->error;
	map = soap_code(soap_codes_tt__PTZPresetTourDirection, s);
	if (map)
		*a = (enum tt__PTZPresetTourDirection)map->code;
	else
	{	long n;
		if (soap_s2long(soap, s, &n) || ((soap->mode & SOAP_XML_STRICT) && (n < 0 || n > 2)))
			return soap->error = SOAP_TYPE;
		*a = (enum tt__PTZPresetTourDirection)n;
	}
	return SOAP_OK;
}

SOAP_FMAC3 enum tt__PTZPresetTourDirection * SOAP_FMAC4 soap_in_tt__PTZPresetTourDirection(struct soap *soap, const char *tag, enum tt__PTZPresetTourDirection *a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (enum tt__PTZPresetTourDirection *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__PTZPresetTourDirection, sizeof(enum tt__PTZPresetTourDirection), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	if (soap->body && !*soap->href)
	{	if (!a || soap_s2tt__PTZPresetTourDirection(soap, soap_value(soap), a) || soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (enum tt__PTZPresetTourDirection *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__PTZPresetTourDirection, 0, sizeof(enum tt__PTZPresetTourDirection), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__PTZPresetTourDirection(struct soap *soap, const enum tt__PTZPresetTourDirection *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__PTZPresetTourDirection);
	if (soap_out_tt__PTZPresetTourDirection(soap, tag?tag:"tt:PTZPresetTourDirection", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 enum tt__PTZPresetTourDirection * SOAP_FMAC4 soap_get_tt__PTZPresetTourDirection(struct soap *soap, enum tt__PTZPresetTourDirection *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__PTZPresetTourDirection(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__PTZPresetTourState(struct soap *soap, enum tt__PTZPresetTourState *a)
{
	(void)soap; /* appease -Wall -Werror */
#ifdef SOAP_DEFAULT_tt__PTZPresetTourState
	*a = SOAP_DEFAULT_tt__PTZPresetTourState;
#else
	*a = (enum tt__PTZPresetTourState)0;
#endif
}

static const struct soap_code_map soap_codes_tt__PTZPresetTourState[] =
{	{ (long)tt__PTZPresetTourState__Idle, "Idle" },
	{ (long)tt__PTZPresetTourState__Touring, "Touring" },
	{ (long)tt__PTZPresetTourState__Paused, "Paused" },
	{ (long)tt__PTZPresetTourState__Extended, "Extended" },
	{ 0, NULL }
};

SOAP_FMAC3S const char* SOAP_FMAC4S soap_tt__PTZPresetTourState2s(struct soap *soap, enum tt__PTZPresetTourState n)
{	const char *s = soap_code_str(soap_codes_tt__PTZPresetTourState, (long)n);
	if (s)
		return s;
	return soap_long2s(soap, (long)n);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__PTZPresetTourState(struct soap *soap, const char *tag, int id, const enum tt__PTZPresetTourState *a, const char *type)
{	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__PTZPresetTourState), type) || soap_send(soap, soap_tt__PTZPresetTourState2s(soap, *a)))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3S int SOAP_FMAC4S soap_s2tt__PTZPresetTourState(struct soap *soap, const char *s, enum tt__PTZPresetTourState *a)
{
	const struct soap_code_map *map;
	if (!s)
		return soap->error;
	map = soap_code(soap_codes_tt__PTZPresetTourState, s);
	if (map)
		*a = (enum tt__PTZPresetTourState)map->code;
	else
	{	long n;
		if (soap_s2long(soap, s, &n) || ((soap->mode & SOAP_XML_STRICT) && (n < 0 || n > 3)))
			return soap->error = SOAP_TYPE;
		*a = (enum tt__PTZPresetTourState)n;
	}
	return SOAP_OK;
}

SOAP_FMAC3 enum tt__PTZPresetTourState * SOAP_FMAC4 soap_in_tt__PTZPresetTourState(struct soap *soap, const char *tag, enum tt__PTZPresetTourState *a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (enum tt__PTZPresetTourState *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__PTZPresetTourState, sizeof(enum tt__PTZPresetTourState), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	if (soap->body && !*soap->href)
	{	if (!a || soap_s2tt__PTZPresetTourState(soap, soap_value(soap), a) || soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (enum tt__PTZPresetTourState *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__PTZPresetTourState, 0, sizeof(enum tt__PTZPresetTourState), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__PTZPresetTourState(struct soap *soap, const enum tt__PTZPresetTourState *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__PTZPresetTourState);
	if (soap_out_tt__PTZPresetTourState(soap, tag?tag:"tt:PTZPresetTourState", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 enum tt__PTZPresetTourState * SOAP_FMAC4 soap_get_tt__PTZPresetTourState(struct soap *soap, enum tt__PTZPresetTourState *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__PTZPresetTourState(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__OperatePresetTour(struct soap *soap, struct _tptz__OperatePresetTour *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->ProfileToken);
	soap_default_tt__ReferenceToken(soap, &a->PresetTourToken);
	soap_default_tt__PTZPresetTourOperation(soap, &a->Operation);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__OperatePresetTour(struct soap *soap, const struct _tptz__OperatePresetTour *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->ProfileToken);
	soap_serialize_tt__ReferenceToken(soap, &a->PresetTourToken);
	soap_embedded(soap, &a->Operation, SOAP_TYPE_tt__PTZPresetTourOperation);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__OperatePresetTour(struct soap *soap, const char *tag, int id, const struct _tptz__OperatePresetTour *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__OperatePresetTour), type))
		return soap->error;
	if (a->ProfileToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tptz:ProfileToken", -1, &a->ProfileToken, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:ProfileToken"))
		return soap->error;
	if (a->PresetTourToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tptz:PresetTourToken", -1, &a->PresetTourToken, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:PresetTourToken"))
		return soap->error;
	if (soap_out_tt__PTZPresetTourOperation(soap, "tptz:Operation", -1, &a->Operation, ""))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__OperatePresetTour * SOAP_FMAC4 soap_in__tptz__OperatePresetTour(struct soap *soap, const char *tag, struct _tptz__OperatePresetTour *a, const char *type)
{
	size_t soap_flag_ProfileToken = 1;
	size_t soap_flag_PresetTourToken = 1;
	size_t soap_flag_Operation = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__OperatePresetTour *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__OperatePresetTour, sizeof(struct _tptz__OperatePresetTour), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__OperatePresetTour(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_ProfileToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_tt__ReferenceToken(soap, "tptz:ProfileToken", &a->ProfileToken, "tt:ReferenceToken"))
				{	soap_flag_ProfileToken--;
					continue;
				}
			if (soap_flag_PresetTourToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_tt__ReferenceToken(soap, "tptz:PresetTourToken", &a->PresetTourToken, "tt:ReferenceToken"))
				{	soap_flag_PresetTourToken--;
					continue;
				}
			if (soap_flag_Operation && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_tt__PTZPresetTourOperation(soap, "tptz:Operation", &a->Operation, "tt:PTZPresetTourOperation"))
				{	soap_flag_Operation--;
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
	{	a = (struct _tptz__OperatePresetTour *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__OperatePresetTour, 0, sizeof(struct _tptz__OperatePresetTour), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_ProfileToken > 0 || soap_flag_PresetTourToken > 0 || soap_flag_Operation > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__OperatePresetTour(struct soap *soap, const struct _tptz__OperatePresetTour *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__OperatePresetTour);
	if (soap_out__tptz__OperatePresetTour(soap, tag?tag:"tptz:OperatePresetTour", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__OperatePresetTour * SOAP_FMAC4 soap_get__tptz__OperatePresetTour(struct soap *soap, struct _tptz__OperatePresetTour *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__OperatePresetTour(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__ModifyPresetTourResponse(struct soap *soap, struct _tptz__ModifyPresetTourResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__ModifyPresetTourResponse(struct soap *soap, const struct _tptz__ModifyPresetTourResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__ModifyPresetTourResponse(struct soap *soap, const char *tag, int id, const struct _tptz__ModifyPresetTourResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__ModifyPresetTourResponse), type))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__ModifyPresetTourResponse * SOAP_FMAC4 soap_in__tptz__ModifyPresetTourResponse(struct soap *soap, const char *tag, struct _tptz__ModifyPresetTourResponse *a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__ModifyPresetTourResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__ModifyPresetTourResponse, sizeof(struct _tptz__ModifyPresetTourResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__ModifyPresetTourResponse(soap, a);
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
	{	a = (struct _tptz__ModifyPresetTourResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__ModifyPresetTourResponse, 0, sizeof(struct _tptz__ModifyPresetTourResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__ModifyPresetTourResponse(struct soap *soap, const struct _tptz__ModifyPresetTourResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__ModifyPresetTourResponse);
	if (soap_out__tptz__ModifyPresetTourResponse(soap, tag?tag:"tptz:ModifyPresetTourResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__ModifyPresetTourResponse * SOAP_FMAC4 soap_get__tptz__ModifyPresetTourResponse(struct soap *soap, struct _tptz__ModifyPresetTourResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__ModifyPresetTourResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__ModifyPresetTour(struct soap *soap, struct _tptz__ModifyPresetTour *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->ProfileToken);
	a->PresetTour = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__ModifyPresetTour(struct soap *soap, const struct _tptz__ModifyPresetTour *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->ProfileToken);
	soap_serialize_PointerTott__PresetTour(soap, &a->PresetTour);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__ModifyPresetTour(struct soap *soap, const char *tag, int id, const struct _tptz__ModifyPresetTour *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__ModifyPresetTour), type))
		return soap->error;
	if (a->ProfileToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tptz:ProfileToken", -1, &a->ProfileToken, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:ProfileToken"))
		return soap->error;
	if (a->PresetTour)
	{	if (soap_out_PointerTott__PresetTour(soap, "tptz:PresetTour", -1, &a->PresetTour, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:PresetTour"))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__ModifyPresetTour * SOAP_FMAC4 soap_in__tptz__ModifyPresetTour(struct soap *soap, const char *tag, struct _tptz__ModifyPresetTour *a, const char *type)
{
	size_t soap_flag_ProfileToken = 1;
	size_t soap_flag_PresetTour = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__ModifyPresetTour *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__ModifyPresetTour, sizeof(struct _tptz__ModifyPresetTour), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__ModifyPresetTour(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_ProfileToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_tt__ReferenceToken(soap, "tptz:ProfileToken", &a->ProfileToken, "tt:ReferenceToken"))
				{	soap_flag_ProfileToken--;
					continue;
				}
			if (soap_flag_PresetTour && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__PresetTour(soap, "tptz:PresetTour", &a->PresetTour, "tt:PresetTour"))
				{	soap_flag_PresetTour--;
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
	{	a = (struct _tptz__ModifyPresetTour *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__ModifyPresetTour, 0, sizeof(struct _tptz__ModifyPresetTour), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_ProfileToken > 0 || soap_flag_PresetTour > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__ModifyPresetTour(struct soap *soap, const struct _tptz__ModifyPresetTour *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__ModifyPresetTour);
	if (soap_out__tptz__ModifyPresetTour(soap, tag?tag:"tptz:ModifyPresetTour", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__ModifyPresetTour * SOAP_FMAC4 soap_get__tptz__ModifyPresetTour(struct soap *soap, struct _tptz__ModifyPresetTour *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__ModifyPresetTour(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__CreatePresetTourResponse(struct soap *soap, struct _tptz__CreatePresetTourResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->PresetTourToken);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__CreatePresetTourResponse(struct soap *soap, const struct _tptz__CreatePresetTourResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->PresetTourToken);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__CreatePresetTourResponse(struct soap *soap, const char *tag, int id, const struct _tptz__CreatePresetTourResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__CreatePresetTourResponse), type))
		return soap->error;
	if (a->PresetTourToken)
		soap_element_result(soap, "tptz:PresetTourToken");
	if (a->PresetTourToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tptz:PresetTourToken", -1, &a->PresetTourToken, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:PresetTourToken"))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__CreatePresetTourResponse * SOAP_FMAC4 soap_in__tptz__CreatePresetTourResponse(struct soap *soap, const char *tag, struct _tptz__CreatePresetTourResponse *a, const char *type)
{
	size_t soap_flag_PresetTourToken = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__CreatePresetTourResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__CreatePresetTourResponse, sizeof(struct _tptz__CreatePresetTourResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__CreatePresetTourResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_PresetTourToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_tt__ReferenceToken(soap, "tptz:PresetTourToken", &a->PresetTourToken, "tt:ReferenceToken"))
				{	soap_flag_PresetTourToken--;
					continue;
				}
			soap_check_result(soap, "tptz:PresetTourToken");
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
	{	a = (struct _tptz__CreatePresetTourResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__CreatePresetTourResponse, 0, sizeof(struct _tptz__CreatePresetTourResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_PresetTourToken > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__CreatePresetTourResponse(struct soap *soap, const struct _tptz__CreatePresetTourResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__CreatePresetTourResponse);
	if (soap_out__tptz__CreatePresetTourResponse(soap, tag?tag:"tptz:CreatePresetTourResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__CreatePresetTourResponse * SOAP_FMAC4 soap_get__tptz__CreatePresetTourResponse(struct soap *soap, struct _tptz__CreatePresetTourResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__CreatePresetTourResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__CreatePresetTour(struct soap *soap, struct _tptz__CreatePresetTour *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->ProfileToken);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__CreatePresetTour(struct soap *soap, const struct _tptz__CreatePresetTour *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->ProfileToken);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__CreatePresetTour(struct soap *soap, const char *tag, int id, const struct _tptz__CreatePresetTour *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__CreatePresetTour), type))
		return soap->error;
	if (a->ProfileToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tptz:ProfileToken", -1, &a->ProfileToken, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:ProfileToken"))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__CreatePresetTour * SOAP_FMAC4 soap_in__tptz__CreatePresetTour(struct soap *soap, const char *tag, struct _tptz__CreatePresetTour *a, const char *type)
{
	size_t soap_flag_ProfileToken = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__CreatePresetTour *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__CreatePresetTour, sizeof(struct _tptz__CreatePresetTour), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__CreatePresetTour(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_ProfileToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_tt__ReferenceToken(soap, "tptz:ProfileToken", &a->ProfileToken, "tt:ReferenceToken"))
				{	soap_flag_ProfileToken--;
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
	{	a = (struct _tptz__CreatePresetTour *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__CreatePresetTour, 0, sizeof(struct _tptz__CreatePresetTour), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_ProfileToken > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__CreatePresetTour(struct soap *soap, const struct _tptz__CreatePresetTour *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__CreatePresetTour);
	if (soap_out__tptz__CreatePresetTour(soap, tag?tag:"tptz:CreatePresetTour", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__CreatePresetTour * SOAP_FMAC4 soap_get__tptz__CreatePresetTour(struct soap *soap, struct _tptz__CreatePresetTour *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__CreatePresetTour(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__GetPresetTourOptionsResponse(struct soap *soap, struct _tptz__GetPresetTourOptionsResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->Options = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__GetPresetTourOptionsResponse(struct soap *soap, const struct _tptz__GetPresetTourOptionsResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTott__PTZPresetTourOptions(soap, &a->Options);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__GetPresetTourOptionsResponse(struct soap *soap, const char *tag, int id, const struct _tptz__GetPresetTourOptionsResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__GetPresetTourOptionsResponse), type))
		return soap->error;
	if (a->Options)
		soap_element_result(soap, "tptz:Options");
	if (a->Options)
	{	if (soap_out_PointerTott__PTZPresetTourOptions(soap, "tptz:Options", -1, &a->Options, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:Options"))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__GetPresetTourOptionsResponse * SOAP_FMAC4 soap_in__tptz__GetPresetTourOptionsResponse(struct soap *soap, const char *tag, struct _tptz__GetPresetTourOptionsResponse *a, const char *type)
{
	size_t soap_flag_Options = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__GetPresetTourOptionsResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__GetPresetTourOptionsResponse, sizeof(struct _tptz__GetPresetTourOptionsResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__GetPresetTourOptionsResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_Options && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__PTZPresetTourOptions(soap, "tptz:Options", &a->Options, "tt:PTZPresetTourOptions"))
				{	soap_flag_Options--;
					continue;
				}
			soap_check_result(soap, "tptz:Options");
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
	{	a = (struct _tptz__GetPresetTourOptionsResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__GetPresetTourOptionsResponse, 0, sizeof(struct _tptz__GetPresetTourOptionsResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_Options > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__GetPresetTourOptionsResponse(struct soap *soap, const struct _tptz__GetPresetTourOptionsResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__GetPresetTourOptionsResponse);
	if (soap_out__tptz__GetPresetTourOptionsResponse(soap, tag?tag:"tptz:GetPresetTourOptionsResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__GetPresetTourOptionsResponse * SOAP_FMAC4 soap_get__tptz__GetPresetTourOptionsResponse(struct soap *soap, struct _tptz__GetPresetTourOptionsResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__GetPresetTourOptionsResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__GetPresetTourOptions(struct soap *soap, struct _tptz__GetPresetTourOptions *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->ProfileToken);
	soap_default_tt__ReferenceToken(soap, &a->PresetTourToken);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__GetPresetTourOptions(struct soap *soap, const struct _tptz__GetPresetTourOptions *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->ProfileToken);
	soap_serialize_tt__ReferenceToken(soap, &a->PresetTourToken);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__GetPresetTourOptions(struct soap *soap, const char *tag, int id, const struct _tptz__GetPresetTourOptions *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__GetPresetTourOptions), type))
		return soap->error;
	if (a->ProfileToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tptz:ProfileToken", -1, &a->ProfileToken, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:ProfileToken"))
		return soap->error;
	if (soap_out_tt__ReferenceToken(soap, "tptz:PresetTourToken", -1, &a->PresetTourToken, ""))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__GetPresetTourOptions * SOAP_FMAC4 soap_in__tptz__GetPresetTourOptions(struct soap *soap, const char *tag, struct _tptz__GetPresetTourOptions *a, const char *type)
{
	size_t soap_flag_ProfileToken = 1;
	size_t soap_flag_PresetTourToken = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__GetPresetTourOptions *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__GetPresetTourOptions, sizeof(struct _tptz__GetPresetTourOptions), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__GetPresetTourOptions(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_ProfileToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_tt__ReferenceToken(soap, "tptz:ProfileToken", &a->ProfileToken, "tt:ReferenceToken"))
				{	soap_flag_ProfileToken--;
					continue;
				}
			if (soap_flag_PresetTourToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_tt__ReferenceToken(soap, "tptz:PresetTourToken", &a->PresetTourToken, "tt:ReferenceToken"))
				{	soap_flag_PresetTourToken--;
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
	{	a = (struct _tptz__GetPresetTourOptions *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__GetPresetTourOptions, 0, sizeof(struct _tptz__GetPresetTourOptions), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_ProfileToken > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__GetPresetTourOptions(struct soap *soap, const struct _tptz__GetPresetTourOptions *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__GetPresetTourOptions);
	if (soap_out__tptz__GetPresetTourOptions(soap, tag?tag:"tptz:GetPresetTourOptions", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__GetPresetTourOptions * SOAP_FMAC4 soap_get__tptz__GetPresetTourOptions(struct soap *soap, struct _tptz__GetPresetTourOptions *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__GetPresetTourOptions(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__GetPresetTourResponse(struct soap *soap, struct _tptz__GetPresetTourResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->PresetTour = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__GetPresetTourResponse(struct soap *soap, const struct _tptz__GetPresetTourResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTott__PresetTour(soap, &a->PresetTour);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__GetPresetTourResponse(struct soap *soap, const char *tag, int id, const struct _tptz__GetPresetTourResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__GetPresetTourResponse), type))
		return soap->error;
	if (a->PresetTour)
		soap_element_result(soap, "tptz:PresetTour");
	if (a->PresetTour)
	{	if (soap_out_PointerTott__PresetTour(soap, "tptz:PresetTour", -1, &a->PresetTour, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:PresetTour"))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__GetPresetTourResponse * SOAP_FMAC4 soap_in__tptz__GetPresetTourResponse(struct soap *soap, const char *tag, struct _tptz__GetPresetTourResponse *a, const char *type)
{
	size_t soap_flag_PresetTour = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__GetPresetTourResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__GetPresetTourResponse, sizeof(struct _tptz__GetPresetTourResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__GetPresetTourResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_PresetTour && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__PresetTour(soap, "tptz:PresetTour", &a->PresetTour, "tt:PresetTour"))
				{	soap_flag_PresetTour--;
					continue;
				}
			soap_check_result(soap, "tptz:PresetTour");
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
	{	a = (struct _tptz__GetPresetTourResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__GetPresetTourResponse, 0, sizeof(struct _tptz__GetPresetTourResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_PresetTour > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__GetPresetTourResponse(struct soap *soap, const struct _tptz__GetPresetTourResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__GetPresetTourResponse);
	if (soap_out__tptz__GetPresetTourResponse(soap, tag?tag:"tptz:GetPresetTourResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__GetPresetTourResponse * SOAP_FMAC4 soap_get__tptz__GetPresetTourResponse(struct soap *soap, struct _tptz__GetPresetTourResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__GetPresetTourResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__GetPresetTour(struct soap *soap, struct _tptz__GetPresetTour *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->ProfileToken);
	soap_default_tt__ReferenceToken(soap, &a->PresetTourToken);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__GetPresetTour(struct soap *soap, const struct _tptz__GetPresetTour *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->ProfileToken);
	soap_serialize_tt__ReferenceToken(soap, &a->PresetTourToken);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__GetPresetTour(struct soap *soap, const char *tag, int id, const struct _tptz__GetPresetTour *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__GetPresetTour), type))
		return soap->error;
	if (a->ProfileToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tptz:ProfileToken", -1, &a->ProfileToken, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:ProfileToken"))
		return soap->error;
	if (a->PresetTourToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tptz:PresetTourToken", -1, &a->PresetTourToken, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:PresetTourToken"))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__GetPresetTour * SOAP_FMAC4 soap_in__tptz__GetPresetTour(struct soap *soap, const char *tag, struct _tptz__GetPresetTour *a, const char *type)
{
	size_t soap_flag_ProfileToken = 1;
	size_t soap_flag_PresetTourToken = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__GetPresetTour *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__GetPresetTour, sizeof(struct _tptz__GetPresetTour), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__GetPresetTour(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_ProfileToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_tt__ReferenceToken(soap, "tptz:ProfileToken", &a->ProfileToken, "tt:ReferenceToken"))
				{	soap_flag_ProfileToken--;
					continue;
				}
			if (soap_flag_PresetTourToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_tt__ReferenceToken(soap, "tptz:PresetTourToken", &a->PresetTourToken, "tt:ReferenceToken"))
				{	soap_flag_PresetTourToken--;
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
	{	a = (struct _tptz__GetPresetTour *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__GetPresetTour, 0, sizeof(struct _tptz__GetPresetTour), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_ProfileToken > 0 || soap_flag_PresetTourToken > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__GetPresetTour(struct soap *soap, const struct _tptz__GetPresetTour *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__GetPresetTour);
	if (soap_out__tptz__GetPresetTour(soap, tag?tag:"tptz:GetPresetTour", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__GetPresetTour * SOAP_FMAC4 soap_get__tptz__GetPresetTour(struct soap *soap, struct _tptz__GetPresetTour *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__GetPresetTour(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__GetPresetToursResponse(struct soap *soap, struct _tptz__GetPresetToursResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__sizePresetTour = 0;
	a->PresetTour = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__GetPresetToursResponse(struct soap *soap, const struct _tptz__GetPresetToursResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	if (a->PresetTour)
	{	int i;
		for (i = 0; i < a->__sizePresetTour; i++)
		{
			soap_embedded(soap, a->PresetTour + i, SOAP_TYPE_tt__PresetTour);
			soap_serialize_tt__PresetTour(soap, a->PresetTour + i);
		}
	}
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__GetPresetToursResponse(struct soap *soap, const char *tag, int id, const struct _tptz__GetPresetToursResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__GetPresetToursResponse), type))
		return soap->error;
	soap_element_result(soap, "-sizePresetTour");
	if (a->PresetTour)
	{	int i;
		for (i = 0; i < a->__sizePresetTour; i++)
			if (soap_out_tt__PresetTour(soap, "tptz:PresetTour", -1, a->PresetTour + i, ""))
				return soap->error;
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__GetPresetToursResponse * SOAP_FMAC4 soap_in__tptz__GetPresetToursResponse(struct soap *soap, const char *tag, struct _tptz__GetPresetToursResponse *a, const char *type)
{
	struct soap_blist *soap_blist_PresetTour = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__GetPresetToursResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__GetPresetToursResponse, sizeof(struct _tptz__GetPresetToursResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__GetPresetToursResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	
			//int i = -1;
			//if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tptz:PresetTour", 1, NULL))
			soap->error = SOAP_TAG_MISMATCH;
			//i = soap_element_begin_in(soap, "tptz:PresetTour", 1, NULL);
            //if (1 && i)
			if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tptz:PresetTour", 1, NULL))
			{	if (a->PresetTour == NULL)
				{	if (soap_blist_PresetTour == NULL)
						soap_blist_PresetTour = soap_new_block(soap);
					a->PresetTour = (struct tt__PresetTour *)soap_push_block(soap, soap_blist_PresetTour, sizeof(struct tt__PresetTour));
					if (a->PresetTour == NULL)
						return NULL;
					soap_default_tt__PresetTour(soap, a->PresetTour);
				}
				soap_revert(soap);
				if (soap_in_tt__PresetTour(soap, "tptz:PresetTour", a->PresetTour, "tt:PresetTour"))
				{	a->__sizePresetTour++;
					a->PresetTour = NULL;
					continue;
				}
			}
			soap_check_result(soap, "-sizePresetTour");
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if (a->PresetTour)
			soap_pop_block(soap, soap_blist_PresetTour);
		if (a->__sizePresetTour)
			a->PresetTour = (struct tt__PresetTour *)soap_save_block(soap, soap_blist_PresetTour, NULL, 1);
		else
		{	a->PresetTour = NULL;
			if (soap_blist_PresetTour)
				soap_end_block(soap, soap_blist_PresetTour);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct _tptz__GetPresetToursResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__GetPresetToursResponse, 0, sizeof(struct _tptz__GetPresetToursResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__GetPresetToursResponse(struct soap *soap, const struct _tptz__GetPresetToursResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__GetPresetToursResponse);
	if (soap_out__tptz__GetPresetToursResponse(soap, tag?tag:"tptz:GetPresetToursResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__GetPresetToursResponse * SOAP_FMAC4 soap_get__tptz__GetPresetToursResponse(struct soap *soap, struct _tptz__GetPresetToursResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__GetPresetToursResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__GetPresetTours(struct soap *soap, struct _tptz__GetPresetTours *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->ProfileToken);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__GetPresetTours(struct soap *soap, const struct _tptz__GetPresetTours *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->ProfileToken);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__GetPresetTours(struct soap *soap, const char *tag, int id, const struct _tptz__GetPresetTours *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__GetPresetTours), type))
		return soap->error;
	if (a->ProfileToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tptz:ProfileToken", -1, &a->ProfileToken, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:ProfileToken"))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__GetPresetTours * SOAP_FMAC4 soap_in__tptz__GetPresetTours(struct soap *soap, const char *tag, struct _tptz__GetPresetTours *a, const char *type)
{
	size_t soap_flag_ProfileToken = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__GetPresetTours *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__GetPresetTours, sizeof(struct _tptz__GetPresetTours), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__GetPresetTours(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_ProfileToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_tt__ReferenceToken(soap, "tptz:ProfileToken", &a->ProfileToken, "tt:ReferenceToken"))
				{	soap_flag_ProfileToken--;
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
	{	a = (struct _tptz__GetPresetTours *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__GetPresetTours, 0, sizeof(struct _tptz__GetPresetTours), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_ProfileToken > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__GetPresetTours(struct soap *soap, const struct _tptz__GetPresetTours *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__GetPresetTours);
	if (soap_out__tptz__GetPresetTours(soap, tag?tag:"tptz:GetPresetTours", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__GetPresetTours * SOAP_FMAC4 soap_get__tptz__GetPresetTours(struct soap *soap, struct _tptz__GetPresetTours *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__GetPresetTours(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__PTZPresetTourStartingConditionOptionsExtension(struct soap *soap, struct tt__PTZPresetTourStartingConditionOptionsExtension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__size = 0;
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__PTZPresetTourStartingConditionOptionsExtension(struct soap *soap, const struct tt__PTZPresetTourStartingConditionOptionsExtension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__PTZPresetTourStartingConditionOptionsExtension(struct soap *soap, const char *tag, int id, const struct tt__PTZPresetTourStartingConditionOptionsExtension *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__PTZPresetTourStartingConditionOptionsExtension), type))
		return soap->error;
	if (a->__any)
	{	int i;
		for (i = 0; i < a->__size; i++)
			soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__PTZPresetTourStartingConditionOptionsExtension * SOAP_FMAC4 soap_in_tt__PTZPresetTourStartingConditionOptionsExtension(struct soap *soap, const char *tag, struct tt__PTZPresetTourStartingConditionOptionsExtension *a, const char *type)
{
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__PTZPresetTourStartingConditionOptionsExtension *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__PTZPresetTourStartingConditionOptionsExtension, sizeof(struct tt__PTZPresetTourStartingConditionOptionsExtension), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__PTZPresetTourStartingConditionOptionsExtension(soap, a);
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
	{	a = (struct tt__PTZPresetTourStartingConditionOptionsExtension *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__PTZPresetTourStartingConditionOptionsExtension, 0, sizeof(struct tt__PTZPresetTourStartingConditionOptionsExtension), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__PTZPresetTourStartingConditionOptionsExtension(struct soap *soap, const struct tt__PTZPresetTourStartingConditionOptionsExtension *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__PTZPresetTourStartingConditionOptionsExtension);
	if (soap_out_tt__PTZPresetTourStartingConditionOptionsExtension(soap, tag?tag:"tt:PTZPresetTourStartingConditionOptionsExtension", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourStartingConditionOptionsExtension * SOAP_FMAC4 soap_get_tt__PTZPresetTourStartingConditionOptionsExtension(struct soap *soap, struct tt__PTZPresetTourStartingConditionOptionsExtension *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__PTZPresetTourStartingConditionOptionsExtension(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__PTZPresetTourPresetDetailOptionsExtension(struct soap *soap, struct tt__PTZPresetTourPresetDetailOptionsExtension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__size = 0;
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__PTZPresetTourPresetDetailOptionsExtension(struct soap *soap, const struct tt__PTZPresetTourPresetDetailOptionsExtension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__PTZPresetTourPresetDetailOptionsExtension(struct soap *soap, const char *tag, int id, const struct tt__PTZPresetTourPresetDetailOptionsExtension *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__PTZPresetTourPresetDetailOptionsExtension), type))
		return soap->error;
	if (a->__any)
	{	int i;
		for (i = 0; i < a->__size; i++)
			soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__PTZPresetTourPresetDetailOptionsExtension * SOAP_FMAC4 soap_in_tt__PTZPresetTourPresetDetailOptionsExtension(struct soap *soap, const char *tag, struct tt__PTZPresetTourPresetDetailOptionsExtension *a, const char *type)
{
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__PTZPresetTourPresetDetailOptionsExtension *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__PTZPresetTourPresetDetailOptionsExtension, sizeof(struct tt__PTZPresetTourPresetDetailOptionsExtension), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__PTZPresetTourPresetDetailOptionsExtension(soap, a);
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
	{	a = (struct tt__PTZPresetTourPresetDetailOptionsExtension *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__PTZPresetTourPresetDetailOptionsExtension, 0, sizeof(struct tt__PTZPresetTourPresetDetailOptionsExtension), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__PTZPresetTourPresetDetailOptionsExtension(struct soap *soap, const struct tt__PTZPresetTourPresetDetailOptionsExtension *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__PTZPresetTourPresetDetailOptionsExtension);
	if (soap_out_tt__PTZPresetTourPresetDetailOptionsExtension(soap, tag?tag:"tt:PTZPresetTourPresetDetailOptionsExtension", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourPresetDetailOptionsExtension * SOAP_FMAC4 soap_get_tt__PTZPresetTourPresetDetailOptionsExtension(struct soap *soap, struct tt__PTZPresetTourPresetDetailOptionsExtension *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__PTZPresetTourPresetDetailOptionsExtension(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__PTZPresetTourPresetDetailOptions(struct soap *soap, struct tt__PTZPresetTourPresetDetailOptions *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__sizePresetToken = 0;
	a->PresetToken = NULL;
	a->Home = NULL;
	a->PanTiltPositionSpace = NULL;
	a->ZoomPositionSpace = NULL;
	a->Extension = NULL;
	a->__anyAttribute = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__PTZPresetTourPresetDetailOptions(struct soap *soap, const struct tt__PTZPresetTourPresetDetailOptions *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	if (a->PresetToken)
	{	int i;
		for (i = 0; i < a->__sizePresetToken; i++)
		{
			soap_serialize_tt__ReferenceToken(soap, a->PresetToken + i);
		}
	}
	soap_serialize_PointerToxsd__boolean_(soap, &a->Home);
	soap_serialize_PointerTott__Space2DDescription(soap, &a->PanTiltPositionSpace);
	soap_serialize_PointerTott__Space1DDescription(soap, &a->ZoomPositionSpace);
	soap_serialize_PointerTott__PTZPresetTourPresetDetailOptionsExtension(soap, &a->Extension);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__PTZPresetTourPresetDetailOptions(struct soap *soap, const char *tag, int id, const struct tt__PTZPresetTourPresetDetailOptions *a, const char *type)
{
	if (a->__anyAttribute)
		soap_set_attr(soap, "-anyAttribute", a->__anyAttribute, 1);
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__PTZPresetTourPresetDetailOptions), type))
		return soap->error;
	if (a->PresetToken)
	{	int i;
		for (i = 0; i < a->__sizePresetToken; i++)
			if (soap_out_tt__ReferenceToken(soap, "tt:PresetToken", -1, a->PresetToken + i, ""))
				return soap->error;
	}
	if (soap_out_PointerToxsd__boolean_(soap, "tt:Home", -1, &a->Home, ""))
		return soap->error;
	if (soap_out_PointerTott__Space2DDescription(soap, "tt:PanTiltPositionSpace", -1, &a->PanTiltPositionSpace, ""))
		return soap->error;
	if (soap_out_PointerTott__Space1DDescription(soap, "tt:ZoomPositionSpace", -1, &a->ZoomPositionSpace, ""))
		return soap->error;
	if (soap_out_PointerTott__PTZPresetTourPresetDetailOptionsExtension(soap, "tt:Extension", -1, &a->Extension, ""))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__PTZPresetTourPresetDetailOptions * SOAP_FMAC4 soap_in_tt__PTZPresetTourPresetDetailOptions(struct soap *soap, const char *tag, struct tt__PTZPresetTourPresetDetailOptions *a, const char *type)
{
	struct soap_blist *soap_blist_PresetToken = NULL;
	size_t soap_flag_Home = 1;
	size_t soap_flag_PanTiltPositionSpace = 1;
	size_t soap_flag_ZoomPositionSpace = 1;
	size_t soap_flag_Extension = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__PTZPresetTourPresetDetailOptions *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__PTZPresetTourPresetDetailOptions, sizeof(struct tt__PTZPresetTourPresetDetailOptions), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__PTZPresetTourPresetDetailOptions(soap, a);
	if (soap_s2string(soap, soap_attr_value(soap, "-anyAttribute", 0), &a->__anyAttribute, 0, -1))
		return NULL;
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tt:PresetToken", 1, NULL))
			{	if (a->PresetToken == NULL)
				{	if (soap_blist_PresetToken == NULL)
						soap_blist_PresetToken = soap_new_block(soap);
					a->PresetToken = (char **)soap_push_block(soap, soap_blist_PresetToken, sizeof(char *));
					if (a->PresetToken == NULL)
						return NULL;
					*a->PresetToken = NULL;
				}
				soap_revert(soap);
				if (soap_in_tt__ReferenceToken(soap, "tt:PresetToken", a->PresetToken, "tt:ReferenceToken"))
				{	a->__sizePresetToken++;
					a->PresetToken = NULL;
					continue;
				}
			}
			if (soap_flag_Home && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerToxsd__boolean_(soap, "tt:Home", &a->Home, "xsd:boolean"))
				{	soap_flag_Home--;
					continue;
				}
			if (soap_flag_PanTiltPositionSpace && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__Space2DDescription(soap, "tt:PanTiltPositionSpace", &a->PanTiltPositionSpace, "tt:Space2DDescription"))
				{	soap_flag_PanTiltPositionSpace--;
					continue;
				}
			if (soap_flag_ZoomPositionSpace && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__Space1DDescription(soap, "tt:ZoomPositionSpace", &a->ZoomPositionSpace, "tt:Space1DDescription"))
				{	soap_flag_ZoomPositionSpace--;
					continue;
				}
			if (soap_flag_Extension && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__PTZPresetTourPresetDetailOptionsExtension(soap, "tt:Extension", &a->Extension, "tt:PTZPresetTourPresetDetailOptionsExtension"))
				{	soap_flag_Extension--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if (a->PresetToken)
			soap_pop_block(soap, soap_blist_PresetToken);
		if (a->__sizePresetToken)
			a->PresetToken = (char **)soap_save_block(soap, soap_blist_PresetToken, NULL, 1);
		else
		{	a->PresetToken = NULL;
			if (soap_blist_PresetToken)
				soap_end_block(soap, soap_blist_PresetToken);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct tt__PTZPresetTourPresetDetailOptions *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__PTZPresetTourPresetDetailOptions, 0, sizeof(struct tt__PTZPresetTourPresetDetailOptions), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__PTZPresetTourPresetDetailOptions(struct soap *soap, const struct tt__PTZPresetTourPresetDetailOptions *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__PTZPresetTourPresetDetailOptions);
	if (soap_out_tt__PTZPresetTourPresetDetailOptions(soap, tag?tag:"tt:PTZPresetTourPresetDetailOptions", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourPresetDetailOptions * SOAP_FMAC4 soap_get_tt__PTZPresetTourPresetDetailOptions(struct soap *soap, struct tt__PTZPresetTourPresetDetailOptions *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__PTZPresetTourPresetDetailOptions(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__PTZPresetTourSpotOptions(struct soap *soap, struct tt__PTZPresetTourSpotOptions *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->PresetDetail = NULL;
	a->StayTime = NULL;
	a->__size = 0;
	a->__any = NULL;
	a->__anyAttribute = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__PTZPresetTourSpotOptions(struct soap *soap, const struct tt__PTZPresetTourSpotOptions *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTott__PTZPresetTourPresetDetailOptions(soap, &a->PresetDetail);
	soap_serialize_PointerTott__DurationRange(soap, &a->StayTime);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__PTZPresetTourSpotOptions(struct soap *soap, const char *tag, int id, const struct tt__PTZPresetTourSpotOptions *a, const char *type)
{
	if (a->__anyAttribute)
		soap_set_attr(soap, "-anyAttribute", a->__anyAttribute, 1);
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__PTZPresetTourSpotOptions), type))
		return soap->error;
	if (a->PresetDetail)
	{	if (soap_out_PointerTott__PTZPresetTourPresetDetailOptions(soap, "tt:PresetDetail", -1, &a->PresetDetail, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "tt:PresetDetail"))
		return soap->error;
	if (a->StayTime)
	{	if (soap_out_PointerTott__DurationRange(soap, "tt:StayTime", -1, &a->StayTime, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "tt:StayTime"))
		return soap->error;
	if (a->__any)
	{	int i;
		for (i = 0; i < a->__size; i++)
			soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__PTZPresetTourSpotOptions * SOAP_FMAC4 soap_in_tt__PTZPresetTourSpotOptions(struct soap *soap, const char *tag, struct tt__PTZPresetTourSpotOptions *a, const char *type)
{
	size_t soap_flag_PresetDetail = 1;
	size_t soap_flag_StayTime = 1;
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__PTZPresetTourSpotOptions *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__PTZPresetTourSpotOptions, sizeof(struct tt__PTZPresetTourSpotOptions), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__PTZPresetTourSpotOptions(soap, a);
	if (soap_s2string(soap, soap_attr_value(soap, "-anyAttribute", 0), &a->__anyAttribute, 0, -1))
		return NULL;
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_PresetDetail && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__PTZPresetTourPresetDetailOptions(soap, "tt:PresetDetail", &a->PresetDetail, "tt:PTZPresetTourPresetDetailOptions"))
				{	soap_flag_PresetDetail--;
					continue;
				}
			if (soap_flag_StayTime && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__DurationRange(soap, "tt:StayTime", &a->StayTime, "tt:DurationRange"))
				{	soap_flag_StayTime--;
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
	{	a = (struct tt__PTZPresetTourSpotOptions *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__PTZPresetTourSpotOptions, 0, sizeof(struct tt__PTZPresetTourSpotOptions), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_PresetDetail > 0 || soap_flag_StayTime > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__PTZPresetTourSpotOptions(struct soap *soap, const struct tt__PTZPresetTourSpotOptions *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__PTZPresetTourSpotOptions);
	if (soap_out_tt__PTZPresetTourSpotOptions(soap, tag?tag:"tt:PTZPresetTourSpotOptions", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourSpotOptions * SOAP_FMAC4 soap_get_tt__PTZPresetTourSpotOptions(struct soap *soap, struct tt__PTZPresetTourSpotOptions *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__PTZPresetTourSpotOptions(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__PTZPresetTourStartingConditionOptions(struct soap *soap, struct tt__PTZPresetTourStartingConditionOptions *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->RecurringTime = NULL;
	a->RecurringDuration = NULL;
	a->__sizeDirection = 0;
	a->Direction = NULL;
	a->Extension = NULL;
	a->__anyAttribute = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__PTZPresetTourStartingConditionOptions(struct soap *soap, const struct tt__PTZPresetTourStartingConditionOptions *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTott__IntRange(soap, &a->RecurringTime);
	soap_serialize_PointerTott__DurationRange(soap, &a->RecurringDuration);
	if (a->Direction)
	{	int i;
		for (i = 0; i < a->__sizeDirection; i++)
		{
			soap_embedded(soap, a->Direction + i, SOAP_TYPE_tt__PTZPresetTourDirection);
		}
	}
	soap_serialize_PointerTott__PTZPresetTourStartingConditionOptionsExtension(soap, &a->Extension);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__PTZPresetTourStartingConditionOptions(struct soap *soap, const char *tag, int id, const struct tt__PTZPresetTourStartingConditionOptions *a, const char *type)
{
	if (a->__anyAttribute)
		soap_set_attr(soap, "-anyAttribute", a->__anyAttribute, 1);
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__PTZPresetTourStartingConditionOptions), type))
		return soap->error;
	if (soap_out_PointerTott__IntRange(soap, "tt:RecurringTime", -1, &a->RecurringTime, ""))
		return soap->error;
	if (soap_out_PointerTott__DurationRange(soap, "tt:RecurringDuration", -1, &a->RecurringDuration, ""))
		return soap->error;
	if (a->Direction)
	{	int i;
		for (i = 0; i < a->__sizeDirection; i++)
			if (soap_out_tt__PTZPresetTourDirection(soap, "tt:Direction", -1, a->Direction + i, ""))
				return soap->error;
	}
	if (soap_out_PointerTott__PTZPresetTourStartingConditionOptionsExtension(soap, "tt:Extension", -1, &a->Extension, ""))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__PTZPresetTourStartingConditionOptions * SOAP_FMAC4 soap_in_tt__PTZPresetTourStartingConditionOptions(struct soap *soap, const char *tag, struct tt__PTZPresetTourStartingConditionOptions *a, const char *type)
{
	size_t soap_flag_RecurringTime = 1;
	size_t soap_flag_RecurringDuration = 1;
	struct soap_blist *soap_blist_Direction = NULL;
	size_t soap_flag_Extension = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__PTZPresetTourStartingConditionOptions *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__PTZPresetTourStartingConditionOptions, sizeof(struct tt__PTZPresetTourStartingConditionOptions), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__PTZPresetTourStartingConditionOptions(soap, a);
	if (soap_s2string(soap, soap_attr_value(soap, "-anyAttribute", 0), &a->__anyAttribute, 0, -1))
		return NULL;
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_RecurringTime && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__IntRange(soap, "tt:RecurringTime", &a->RecurringTime, "tt:IntRange"))
				{	soap_flag_RecurringTime--;
					continue;
				}
			if (soap_flag_RecurringDuration && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__DurationRange(soap, "tt:RecurringDuration", &a->RecurringDuration, "tt:DurationRange"))
				{	soap_flag_RecurringDuration--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tt:Direction", 1, NULL))
			{	if (a->Direction == NULL)
				{	if (soap_blist_Direction == NULL)
						soap_blist_Direction = soap_new_block(soap);
					a->Direction = (enum tt__PTZPresetTourDirection *)soap_push_block(soap, soap_blist_Direction, sizeof(enum tt__PTZPresetTourDirection));
					if (a->Direction == NULL)
						return NULL;
					soap_default_tt__PTZPresetTourDirection(soap, a->Direction);
				}
				soap_revert(soap);
				if (soap_in_tt__PTZPresetTourDirection(soap, "tt:Direction", a->Direction, "tt:PTZPresetTourDirection"))
				{	a->__sizeDirection++;
					a->Direction = NULL;
					continue;
				}
			}
			if (soap_flag_Extension && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__PTZPresetTourStartingConditionOptionsExtension(soap, "tt:Extension", &a->Extension, "tt:PTZPresetTourStartingConditionOptionsExtension"))
				{	soap_flag_Extension--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if (a->Direction)
			soap_pop_block(soap, soap_blist_Direction);
		if (a->__sizeDirection)
			a->Direction = (enum tt__PTZPresetTourDirection *)soap_save_block(soap, soap_blist_Direction, NULL, 1);
		else
		{	a->Direction = NULL;
			if (soap_blist_Direction)
				soap_end_block(soap, soap_blist_Direction);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct tt__PTZPresetTourStartingConditionOptions *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__PTZPresetTourStartingConditionOptions, 0, sizeof(struct tt__PTZPresetTourStartingConditionOptions), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__PTZPresetTourStartingConditionOptions(struct soap *soap, const struct tt__PTZPresetTourStartingConditionOptions *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__PTZPresetTourStartingConditionOptions);
	if (soap_out_tt__PTZPresetTourStartingConditionOptions(soap, tag?tag:"tt:PTZPresetTourStartingConditionOptions", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourStartingConditionOptions * SOAP_FMAC4 soap_get_tt__PTZPresetTourStartingConditionOptions(struct soap *soap, struct tt__PTZPresetTourStartingConditionOptions *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__PTZPresetTourStartingConditionOptions(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__PTZPresetTourOptions(struct soap *soap, struct tt__PTZPresetTourOptions *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_xsd__boolean_(soap, &a->AutoStart);
	a->StartingCondition = NULL;
	a->TourSpot = NULL;
	a->__size = 0;
	a->__any = NULL;
	a->__anyAttribute = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__PTZPresetTourOptions(struct soap *soap, const struct tt__PTZPresetTourOptions *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_embedded(soap, &a->AutoStart, SOAP_TYPE_xsd__boolean_);
	soap_serialize_PointerTott__PTZPresetTourStartingConditionOptions(soap, &a->StartingCondition);
	soap_serialize_PointerTott__PTZPresetTourSpotOptions(soap, &a->TourSpot);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__PTZPresetTourOptions(struct soap *soap, const char *tag, int id, const struct tt__PTZPresetTourOptions *a, const char *type)
{
	if (a->__anyAttribute)
		soap_set_attr(soap, "-anyAttribute", a->__anyAttribute, 1);
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__PTZPresetTourOptions), type))
		return soap->error;
	if (soap_out_xsd__boolean_(soap, "tt:AutoStart", -1, &a->AutoStart, ""))
		return soap->error;
	if (a->StartingCondition)
	{	if (soap_out_PointerTott__PTZPresetTourStartingConditionOptions(soap, "tt:StartingCondition", -1, &a->StartingCondition, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "tt:StartingCondition"))
		return soap->error;
	if (a->TourSpot)
	{	if (soap_out_PointerTott__PTZPresetTourSpotOptions(soap, "tt:TourSpot", -1, &a->TourSpot, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "tt:TourSpot"))
		return soap->error;
	if (a->__any)
	{	int i;
		for (i = 0; i < a->__size; i++)
			soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__PTZPresetTourOptions * SOAP_FMAC4 soap_in_tt__PTZPresetTourOptions(struct soap *soap, const char *tag, struct tt__PTZPresetTourOptions *a, const char *type)
{
	size_t soap_flag_AutoStart = 1;
	size_t soap_flag_StartingCondition = 1;
	size_t soap_flag_TourSpot = 1;
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__PTZPresetTourOptions *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__PTZPresetTourOptions, sizeof(struct tt__PTZPresetTourOptions), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__PTZPresetTourOptions(soap, a);
	if (soap_s2string(soap, soap_attr_value(soap, "-anyAttribute", 0), &a->__anyAttribute, 0, -1))
		return NULL;
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_AutoStart && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_xsd__boolean_(soap, "tt:AutoStart", &a->AutoStart, "xsd:boolean"))
				{	soap_flag_AutoStart--;
					continue;
				}
			if (soap_flag_StartingCondition && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__PTZPresetTourStartingConditionOptions(soap, "tt:StartingCondition", &a->StartingCondition, "tt:PTZPresetTourStartingConditionOptions"))
				{	soap_flag_StartingCondition--;
					continue;
				}
			if (soap_flag_TourSpot && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__PTZPresetTourSpotOptions(soap, "tt:TourSpot", &a->TourSpot, "tt:PTZPresetTourSpotOptions"))
				{	soap_flag_TourSpot--;
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
	{	a = (struct tt__PTZPresetTourOptions *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__PTZPresetTourOptions, 0, sizeof(struct tt__PTZPresetTourOptions), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_AutoStart > 0 || soap_flag_StartingCondition > 0 || soap_flag_TourSpot > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__PTZPresetTourOptions(struct soap *soap, const struct tt__PTZPresetTourOptions *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__PTZPresetTourOptions);
	if (soap_out_tt__PTZPresetTourOptions(soap, tag?tag:"tt:PTZPresetTourOptions", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourOptions * SOAP_FMAC4 soap_get_tt__PTZPresetTourOptions(struct soap *soap, struct tt__PTZPresetTourOptions *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__PTZPresetTourOptions(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__PTZPresetTourStartingConditionExtension(struct soap *soap, struct tt__PTZPresetTourStartingConditionExtension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__size = 0;
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__PTZPresetTourStartingConditionExtension(struct soap *soap, const struct tt__PTZPresetTourStartingConditionExtension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__PTZPresetTourStartingConditionExtension(struct soap *soap, const char *tag, int id, const struct tt__PTZPresetTourStartingConditionExtension *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__PTZPresetTourStartingConditionExtension), type))
		return soap->error;
	if (a->__any)
	{	int i;
		for (i = 0; i < a->__size; i++)
			soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__PTZPresetTourStartingConditionExtension * SOAP_FMAC4 soap_in_tt__PTZPresetTourStartingConditionExtension(struct soap *soap, const char *tag, struct tt__PTZPresetTourStartingConditionExtension *a, const char *type)
{
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__PTZPresetTourStartingConditionExtension *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__PTZPresetTourStartingConditionExtension, sizeof(struct tt__PTZPresetTourStartingConditionExtension), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__PTZPresetTourStartingConditionExtension(soap, a);
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
	{	a = (struct tt__PTZPresetTourStartingConditionExtension *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__PTZPresetTourStartingConditionExtension, 0, sizeof(struct tt__PTZPresetTourStartingConditionExtension), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__PTZPresetTourStartingConditionExtension(struct soap *soap, const struct tt__PTZPresetTourStartingConditionExtension *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__PTZPresetTourStartingConditionExtension);
	if (soap_out_tt__PTZPresetTourStartingConditionExtension(soap, tag?tag:"tt:PTZPresetTourStartingConditionExtension", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourStartingConditionExtension * SOAP_FMAC4 soap_get_tt__PTZPresetTourStartingConditionExtension(struct soap *soap, struct tt__PTZPresetTourStartingConditionExtension *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__PTZPresetTourStartingConditionExtension(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__PTZPresetTourStatusExtension(struct soap *soap, struct tt__PTZPresetTourStatusExtension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__size = 0;
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__PTZPresetTourStatusExtension(struct soap *soap, const struct tt__PTZPresetTourStatusExtension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__PTZPresetTourStatusExtension(struct soap *soap, const char *tag, int id, const struct tt__PTZPresetTourStatusExtension *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__PTZPresetTourStatusExtension), type))
		return soap->error;
	if (a->__any)
	{	int i;
		for (i = 0; i < a->__size; i++)
			soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__PTZPresetTourStatusExtension * SOAP_FMAC4 soap_in_tt__PTZPresetTourStatusExtension(struct soap *soap, const char *tag, struct tt__PTZPresetTourStatusExtension *a, const char *type)
{
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__PTZPresetTourStatusExtension *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__PTZPresetTourStatusExtension, sizeof(struct tt__PTZPresetTourStatusExtension), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__PTZPresetTourStatusExtension(soap, a);
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
	{	a = (struct tt__PTZPresetTourStatusExtension *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__PTZPresetTourStatusExtension, 0, sizeof(struct tt__PTZPresetTourStatusExtension), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__PTZPresetTourStatusExtension(struct soap *soap, const struct tt__PTZPresetTourStatusExtension *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__PTZPresetTourStatusExtension);
	if (soap_out_tt__PTZPresetTourStatusExtension(soap, tag?tag:"tt:PTZPresetTourStatusExtension", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourStatusExtension * SOAP_FMAC4 soap_get_tt__PTZPresetTourStatusExtension(struct soap *soap, struct tt__PTZPresetTourStatusExtension *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__PTZPresetTourStatusExtension(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__PTZPresetTourTypeExtension(struct soap *soap, struct tt__PTZPresetTourTypeExtension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__size = 0;
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__PTZPresetTourTypeExtension(struct soap *soap, const struct tt__PTZPresetTourTypeExtension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__PTZPresetTourTypeExtension(struct soap *soap, const char *tag, int id, const struct tt__PTZPresetTourTypeExtension *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__PTZPresetTourTypeExtension), type))
		return soap->error;
	if (a->__any)
	{	int i;
		for (i = 0; i < a->__size; i++)
			soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__PTZPresetTourTypeExtension * SOAP_FMAC4 soap_in_tt__PTZPresetTourTypeExtension(struct soap *soap, const char *tag, struct tt__PTZPresetTourTypeExtension *a, const char *type)
{
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__PTZPresetTourTypeExtension *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__PTZPresetTourTypeExtension, sizeof(struct tt__PTZPresetTourTypeExtension), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__PTZPresetTourTypeExtension(soap, a);
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
	{	a = (struct tt__PTZPresetTourTypeExtension *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__PTZPresetTourTypeExtension, 0, sizeof(struct tt__PTZPresetTourTypeExtension), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__PTZPresetTourTypeExtension(struct soap *soap, const struct tt__PTZPresetTourTypeExtension *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__PTZPresetTourTypeExtension);
	if (soap_out_tt__PTZPresetTourTypeExtension(soap, tag?tag:"tt:PTZPresetTourTypeExtension", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourTypeExtension * SOAP_FMAC4 soap_get_tt__PTZPresetTourTypeExtension(struct soap *soap, struct tt__PTZPresetTourTypeExtension *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__PTZPresetTourTypeExtension(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__PTZPresetTourSpotExtension(struct soap *soap, struct tt__PTZPresetTourSpotExtension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__size = 0;
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__PTZPresetTourSpotExtension(struct soap *soap, const struct tt__PTZPresetTourSpotExtension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__PTZPresetTourSpotExtension(struct soap *soap, const char *tag, int id, const struct tt__PTZPresetTourSpotExtension *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__PTZPresetTourSpotExtension), type))
		return soap->error;
	if (a->__any)
	{	int i;
		for (i = 0; i < a->__size; i++)
			soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__PTZPresetTourSpotExtension * SOAP_FMAC4 soap_in_tt__PTZPresetTourSpotExtension(struct soap *soap, const char *tag, struct tt__PTZPresetTourSpotExtension *a, const char *type)
{
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__PTZPresetTourSpotExtension *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__PTZPresetTourSpotExtension, sizeof(struct tt__PTZPresetTourSpotExtension), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__PTZPresetTourSpotExtension(soap, a);
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
	{	a = (struct tt__PTZPresetTourSpotExtension *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__PTZPresetTourSpotExtension, 0, sizeof(struct tt__PTZPresetTourSpotExtension), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__PTZPresetTourSpotExtension(struct soap *soap, const struct tt__PTZPresetTourSpotExtension *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__PTZPresetTourSpotExtension);
	if (soap_out_tt__PTZPresetTourSpotExtension(soap, tag?tag:"tt:PTZPresetTourSpotExtension", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourSpotExtension * SOAP_FMAC4 soap_get_tt__PTZPresetTourSpotExtension(struct soap *soap, struct tt__PTZPresetTourSpotExtension *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__PTZPresetTourSpotExtension(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__PTZPresetTourPresetDetail(struct soap *soap, struct tt__PTZPresetTourPresetDetail *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__union_PTZPresetTourPresetDetail = 0;
	a->__size = 0;
	a->__any = NULL;
	a->__anyAttribute = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__PTZPresetTourPresetDetail(struct soap *soap, const struct tt__PTZPresetTourPresetDetail *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize__tt__union_PTZPresetTourPresetDetail(soap, a->__union_PTZPresetTourPresetDetail, &a->union_PTZPresetTourPresetDetail);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__PTZPresetTourPresetDetail(struct soap *soap, const char *tag, int id, const struct tt__PTZPresetTourPresetDetail *a, const char *type)
{
	if (a->__anyAttribute)
		soap_set_attr(soap, "-anyAttribute", a->__anyAttribute, 1);
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__PTZPresetTourPresetDetail), type))
		return soap->error;
	if (soap_out__tt__union_PTZPresetTourPresetDetail(soap, a->__union_PTZPresetTourPresetDetail, &a->union_PTZPresetTourPresetDetail))
		return soap->error;
	if (a->__any)
	{	int i;
		for (i = 0; i < a->__size; i++)
			soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__PTZPresetTourPresetDetail * SOAP_FMAC4 soap_in_tt__PTZPresetTourPresetDetail(struct soap *soap, const char *tag, struct tt__PTZPresetTourPresetDetail *a, const char *type)
{
	size_t soap_flag_union_PTZPresetTourPresetDetail = 1;
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__PTZPresetTourPresetDetail *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__PTZPresetTourPresetDetail, sizeof(struct tt__PTZPresetTourPresetDetail), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__PTZPresetTourPresetDetail(soap, a);
	if (soap_s2string(soap, soap_attr_value(soap, "-anyAttribute", 0), &a->__anyAttribute, 0, -1))
		return NULL;
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_union_PTZPresetTourPresetDetail && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in__tt__union_PTZPresetTourPresetDetail(soap, &a->__union_PTZPresetTourPresetDetail, &a->union_PTZPresetTourPresetDetail))
				{	soap_flag_union_PTZPresetTourPresetDetail = 0;
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
	{	a = (struct tt__PTZPresetTourPresetDetail *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__PTZPresetTourPresetDetail, 0, sizeof(struct tt__PTZPresetTourPresetDetail), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_union_PTZPresetTourPresetDetail))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__PTZPresetTourPresetDetail(struct soap *soap, const struct tt__PTZPresetTourPresetDetail *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__PTZPresetTourPresetDetail);
	if (soap_out_tt__PTZPresetTourPresetDetail(soap, tag?tag:"tt:PTZPresetTourPresetDetail", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourPresetDetail * SOAP_FMAC4 soap_get_tt__PTZPresetTourPresetDetail(struct soap *soap, struct tt__PTZPresetTourPresetDetail *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__PTZPresetTourPresetDetail(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__PTZPresetTourExtension(struct soap *soap, struct tt__PTZPresetTourExtension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__size = 0;
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__PTZPresetTourExtension(struct soap *soap, const struct tt__PTZPresetTourExtension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__PTZPresetTourExtension(struct soap *soap, const char *tag, int id, const struct tt__PTZPresetTourExtension *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__PTZPresetTourExtension), type))
		return soap->error;
	if (a->__any)
	{	int i;
		for (i = 0; i < a->__size; i++)
			soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__PTZPresetTourExtension * SOAP_FMAC4 soap_in_tt__PTZPresetTourExtension(struct soap *soap, const char *tag, struct tt__PTZPresetTourExtension *a, const char *type)
{
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__PTZPresetTourExtension *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__PTZPresetTourExtension, sizeof(struct tt__PTZPresetTourExtension), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__PTZPresetTourExtension(soap, a);
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
	{	a = (struct tt__PTZPresetTourExtension *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__PTZPresetTourExtension, 0, sizeof(struct tt__PTZPresetTourExtension), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__PTZPresetTourExtension(struct soap *soap, const struct tt__PTZPresetTourExtension *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__PTZPresetTourExtension);
	if (soap_out_tt__PTZPresetTourExtension(soap, tag?tag:"tt:PTZPresetTourExtension", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourExtension * SOAP_FMAC4 soap_get_tt__PTZPresetTourExtension(struct soap *soap, struct tt__PTZPresetTourExtension *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__PTZPresetTourExtension(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__PTZPresetTourSpot(struct soap *soap, struct tt__PTZPresetTourSpot *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->PresetDetail = NULL;
	a->Speed = NULL;
	soap_default_string(soap, &a->StayTime);
	a->Extension = NULL;
	a->__anyAttribute = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__PTZPresetTourSpot(struct soap *soap, const struct tt__PTZPresetTourSpot *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTott__PTZPresetTourPresetDetail(soap, &a->PresetDetail);
	soap_serialize_PointerTott__PTZSpeed(soap, &a->Speed);
	soap_serialize_string(soap, &a->StayTime);
	soap_serialize_PointerTott__PTZPresetTourSpotExtension(soap, &a->Extension);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__PTZPresetTourSpot(struct soap *soap, const char *tag, int id, const struct tt__PTZPresetTourSpot *a, const char *type)
{
	if (a->__anyAttribute)
		soap_set_attr(soap, "-anyAttribute", a->__anyAttribute, 1);
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__PTZPresetTourSpot), type))
		return soap->error;
	if (a->PresetDetail)
	{	if (soap_out_PointerTott__PTZPresetTourPresetDetail(soap, "tt:PresetDetail", -1, &a->PresetDetail, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "tt:PresetDetail"))
		return soap->error;
	if (soap_out_PointerTott__PTZSpeed(soap, "tt:Speed", -1, &a->Speed, ""))
		return soap->error;
	if (soap_out_string(soap, "tt:StayTime", -1, &a->StayTime, ""))
		return soap->error;
	if (soap_out_PointerTott__PTZPresetTourSpotExtension(soap, "tt:Extension", -1, &a->Extension, ""))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__PTZPresetTourSpot * SOAP_FMAC4 soap_in_tt__PTZPresetTourSpot(struct soap *soap, const char *tag, struct tt__PTZPresetTourSpot *a, const char *type)
{
	size_t soap_flag_PresetDetail = 1;
	size_t soap_flag_Speed = 1;
	size_t soap_flag_StayTime = 1;
	size_t soap_flag_Extension = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__PTZPresetTourSpot *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__PTZPresetTourSpot, sizeof(struct tt__PTZPresetTourSpot), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__PTZPresetTourSpot(soap, a);
	if (soap_s2string(soap, soap_attr_value(soap, "-anyAttribute", 0), &a->__anyAttribute, 0, -1))
		return NULL;
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_PresetDetail && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__PTZPresetTourPresetDetail(soap, "tt:PresetDetail", &a->PresetDetail, "tt:PTZPresetTourPresetDetail"))
				{	soap_flag_PresetDetail--;
					continue;
				}
			if (soap_flag_Speed && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__PTZSpeed(soap, "tt:Speed", &a->Speed, "tt:PTZSpeed"))
				{	soap_flag_Speed--;
					continue;
				}
			if (soap_flag_StayTime && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_string(soap, "tt:StayTime", &a->StayTime, "xsd:string"))
				{	soap_flag_StayTime--;
					continue;
				}
			if (soap_flag_Extension && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__PTZPresetTourSpotExtension(soap, "tt:Extension", &a->Extension, "tt:PTZPresetTourSpotExtension"))
				{	soap_flag_Extension--;
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
	{	a = (struct tt__PTZPresetTourSpot *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__PTZPresetTourSpot, 0, sizeof(struct tt__PTZPresetTourSpot), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_PresetDetail > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__PTZPresetTourSpot(struct soap *soap, const struct tt__PTZPresetTourSpot *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__PTZPresetTourSpot);
	if (soap_out_tt__PTZPresetTourSpot(soap, tag?tag:"tt:PTZPresetTourSpot", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourSpot * SOAP_FMAC4 soap_get_tt__PTZPresetTourSpot(struct soap *soap, struct tt__PTZPresetTourSpot *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__PTZPresetTourSpot(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__PTZPresetTourStartingCondition(struct soap *soap, struct tt__PTZPresetTourStartingCondition *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->RecurringTime = NULL;
	soap_default_string(soap, &a->RecurringDuration);
	a->Direction = NULL;
	a->Extension = NULL;
	a->__anyAttribute = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__PTZPresetTourStartingCondition(struct soap *soap, const struct tt__PTZPresetTourStartingCondition *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerToint(soap, &a->RecurringTime);
	soap_serialize_string(soap, &a->RecurringDuration);
	soap_serialize_PointerTott__PTZPresetTourDirection(soap, &a->Direction);
	soap_serialize_PointerTott__PTZPresetTourStartingConditionExtension(soap, &a->Extension);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__PTZPresetTourStartingCondition(struct soap *soap, const char *tag, int id, const struct tt__PTZPresetTourStartingCondition *a, const char *type)
{
	if (a->__anyAttribute)
		soap_set_attr(soap, "-anyAttribute", a->__anyAttribute, 1);
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__PTZPresetTourStartingCondition), type))
		return soap->error;
	if (soap_out_PointerToint(soap, "tt:RecurringTime", -1, &a->RecurringTime, ""))
		return soap->error;
	if (soap_out_string(soap, "tt:RecurringDuration", -1, &a->RecurringDuration, ""))
		return soap->error;
	if (soap_out_PointerTott__PTZPresetTourDirection(soap, "tt:Direction", -1, &a->Direction, ""))
		return soap->error;
	if (soap_out_PointerTott__PTZPresetTourStartingConditionExtension(soap, "tt:Extension", -1, &a->Extension, ""))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__PTZPresetTourStartingCondition * SOAP_FMAC4 soap_in_tt__PTZPresetTourStartingCondition(struct soap *soap, const char *tag, struct tt__PTZPresetTourStartingCondition *a, const char *type)
{
	size_t soap_flag_RecurringTime = 1;
	size_t soap_flag_RecurringDuration = 1;
	size_t soap_flag_Direction = 1;
	size_t soap_flag_Extension = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__PTZPresetTourStartingCondition *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__PTZPresetTourStartingCondition, sizeof(struct tt__PTZPresetTourStartingCondition), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__PTZPresetTourStartingCondition(soap, a);
	if (soap_s2string(soap, soap_attr_value(soap, "-anyAttribute", 0), &a->__anyAttribute, 0, -1))
		return NULL;
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_RecurringTime && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerToint(soap, "tt:RecurringTime", &a->RecurringTime, "xsd:int"))
				{	soap_flag_RecurringTime--;
					continue;
				}
			if (soap_flag_RecurringDuration && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_string(soap, "tt:RecurringDuration", &a->RecurringDuration, "xsd:string"))
				{	soap_flag_RecurringDuration--;
					continue;
				}
			if (soap_flag_Direction && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__PTZPresetTourDirection(soap, "tt:Direction", &a->Direction, "tt:PTZPresetTourDirection"))
				{	soap_flag_Direction--;
					continue;
				}
			if (soap_flag_Extension && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__PTZPresetTourStartingConditionExtension(soap, "tt:Extension", &a->Extension, "tt:PTZPresetTourStartingConditionExtension"))
				{	soap_flag_Extension--;
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
	{	a = (struct tt__PTZPresetTourStartingCondition *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__PTZPresetTourStartingCondition, 0, sizeof(struct tt__PTZPresetTourStartingCondition), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__PTZPresetTourStartingCondition(struct soap *soap, const struct tt__PTZPresetTourStartingCondition *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__PTZPresetTourStartingCondition);
	if (soap_out_tt__PTZPresetTourStartingCondition(soap, tag?tag:"tt:PTZPresetTourStartingCondition", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourStartingCondition * SOAP_FMAC4 soap_get_tt__PTZPresetTourStartingCondition(struct soap *soap, struct tt__PTZPresetTourStartingCondition *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__PTZPresetTourStartingCondition(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__PTZPresetTourStatus(struct soap *soap, struct tt__PTZPresetTourStatus *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__PTZPresetTourState(soap, &a->State);
	a->CurrentTourSpot = NULL;
	a->Extension = NULL;
	a->__anyAttribute = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__PTZPresetTourStatus(struct soap *soap, const struct tt__PTZPresetTourStatus *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTott__PTZPresetTourSpot(soap, &a->CurrentTourSpot);
	soap_serialize_PointerTott__PTZPresetTourStatusExtension(soap, &a->Extension);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__PTZPresetTourStatus(struct soap *soap, const char *tag, int id, const struct tt__PTZPresetTourStatus *a, const char *type)
{
	if (a->__anyAttribute)
		soap_set_attr(soap, "-anyAttribute", a->__anyAttribute, 1);
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__PTZPresetTourStatus), type))
		return soap->error;
	if (soap_out_tt__PTZPresetTourState(soap, "tt:State", -1, &a->State, ""))
		return soap->error;
	if (soap_out_PointerTott__PTZPresetTourSpot(soap, "tt:CurrentTourSpot", -1, &a->CurrentTourSpot, ""))
		return soap->error;
	if (soap_out_PointerTott__PTZPresetTourStatusExtension(soap, "tt:Extension", -1, &a->Extension, ""))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__PTZPresetTourStatus * SOAP_FMAC4 soap_in_tt__PTZPresetTourStatus(struct soap *soap, const char *tag, struct tt__PTZPresetTourStatus *a, const char *type)
{
	size_t soap_flag_State = 1;
	size_t soap_flag_CurrentTourSpot = 1;
	size_t soap_flag_Extension = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__PTZPresetTourStatus *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__PTZPresetTourStatus, sizeof(struct tt__PTZPresetTourStatus), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__PTZPresetTourStatus(soap, a);
	if (soap_s2string(soap, soap_attr_value(soap, "-anyAttribute", 0), &a->__anyAttribute, 0, -1))
		return NULL;
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_State && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_tt__PTZPresetTourState(soap, "tt:State", &a->State, "tt:PTZPresetTourState"))
				{	soap_flag_State--;
					continue;
				}
			if (soap_flag_CurrentTourSpot && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__PTZPresetTourSpot(soap, "tt:CurrentTourSpot", &a->CurrentTourSpot, "tt:PTZPresetTourSpot"))
				{	soap_flag_CurrentTourSpot--;
					continue;
				}
			if (soap_flag_Extension && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__PTZPresetTourStatusExtension(soap, "tt:Extension", &a->Extension, "tt:PTZPresetTourStatusExtension"))
				{	soap_flag_Extension--;
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
	{	a = (struct tt__PTZPresetTourStatus *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__PTZPresetTourStatus, 0, sizeof(struct tt__PTZPresetTourStatus), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_State > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__PTZPresetTourStatus(struct soap *soap, const struct tt__PTZPresetTourStatus *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__PTZPresetTourStatus);
	if (soap_out_tt__PTZPresetTourStatus(soap, tag?tag:"tt:PTZPresetTourStatus", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourStatus * SOAP_FMAC4 soap_get_tt__PTZPresetTourStatus(struct soap *soap, struct tt__PTZPresetTourStatus *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__PTZPresetTourStatus(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__PresetTour(struct soap *soap, struct tt__PresetTour *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__Name(soap, &a->Name);
	a->Status = NULL;
	soap_default_xsd__boolean_(soap, &a->AutoStart);
	a->StartingCondition = NULL;
	a->__sizeTourSpot = 0;
	a->TourSpot = NULL;
	a->Extension = NULL;
	soap_default_tt__ReferenceToken(soap, &a->token);
	a->__anyAttribute = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__PresetTour(struct soap *soap, const struct tt__PresetTour *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__Name(soap, &a->Name);
	soap_serialize_PointerTott__PTZPresetTourStatus(soap, &a->Status);
	soap_embedded(soap, &a->AutoStart, SOAP_TYPE_xsd__boolean_);
	soap_serialize_PointerTott__PTZPresetTourStartingCondition(soap, &a->StartingCondition);
	if (a->TourSpot)
	{	int i;
		for (i = 0; i < a->__sizeTourSpot; i++)
		{
			soap_embedded(soap, a->TourSpot + i, SOAP_TYPE_tt__PTZPresetTourSpot);
			soap_serialize_tt__PTZPresetTourSpot(soap, a->TourSpot + i);
		}
	}
	soap_serialize_PointerTott__PTZPresetTourExtension(soap, &a->Extension);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__PresetTour(struct soap *soap, const char *tag, int id, const struct tt__PresetTour *a, const char *type)
{
	if (a->token)
		soap_set_attr(soap, "token", a->token, 1);
	if (a->__anyAttribute)
		soap_set_attr(soap, "-anyAttribute", a->__anyAttribute, 1);
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__PresetTour), type))
		return soap->error;
	if (soap_out_tt__Name(soap, "tt:Name", -1, &a->Name, ""))
		return soap->error;
	if (a->Status)
	{	if (soap_out_PointerTott__PTZPresetTourStatus(soap, "tt:Status", -1, &a->Status, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "tt:Status"))
		return soap->error;
	if (soap_out_xsd__boolean_(soap, "tt:AutoStart", -1, &a->AutoStart, ""))
		return soap->error;
	if (a->StartingCondition)
	{	if (soap_out_PointerTott__PTZPresetTourStartingCondition(soap, "tt:StartingCondition", -1, &a->StartingCondition, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "tt:StartingCondition"))
		return soap->error;
	if (a->TourSpot)
	{	int i;
		for (i = 0; i < a->__sizeTourSpot; i++)
			if (soap_out_tt__PTZPresetTourSpot(soap, "tt:TourSpot", -1, a->TourSpot + i, ""))
				return soap->error;
	}
	if (soap_out_PointerTott__PTZPresetTourExtension(soap, "tt:Extension", -1, &a->Extension, ""))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__PresetTour * SOAP_FMAC4 soap_in_tt__PresetTour(struct soap *soap, const char *tag, struct tt__PresetTour *a, const char *type)
{
	size_t soap_flag_Name = 1;
	size_t soap_flag_Status = 1;
	size_t soap_flag_AutoStart = 1;
	size_t soap_flag_StartingCondition = 1;
	struct soap_blist *soap_blist_TourSpot = NULL;
	size_t soap_flag_Extension = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__PresetTour *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__PresetTour, sizeof(struct tt__PresetTour), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__PresetTour(soap, a);
	if (soap_s2string(soap, soap_attr_value(soap, "token", 0), &a->token, 0, 64))
		return NULL;
	if (soap_s2string(soap, soap_attr_value(soap, "-anyAttribute", 0), &a->__anyAttribute, 0, -1))
		return NULL;
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_Name && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_tt__Name(soap, "tt:Name", &a->Name, "tt:Name"))
				{	soap_flag_Name--;
					continue;
				}
			if (soap_flag_Status && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__PTZPresetTourStatus(soap, "tt:Status", &a->Status, "tt:PTZPresetTourStatus"))
				{	soap_flag_Status--;
					continue;
				}
			if (soap_flag_AutoStart && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_xsd__boolean_(soap, "tt:AutoStart", &a->AutoStart, "xsd:boolean"))
				{	soap_flag_AutoStart--;
					continue;
				}
			if (soap_flag_StartingCondition && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__PTZPresetTourStartingCondition(soap, "tt:StartingCondition", &a->StartingCondition, "tt:PTZPresetTourStartingCondition"))
				{	soap_flag_StartingCondition--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tt:TourSpot", 1, NULL))
			{	if (a->TourSpot == NULL)
				{	if (soap_blist_TourSpot == NULL)
						soap_blist_TourSpot = soap_new_block(soap);
					a->TourSpot = (struct tt__PTZPresetTourSpot *)soap_push_block(soap, soap_blist_TourSpot, sizeof(struct tt__PTZPresetTourSpot));
					if (a->TourSpot == NULL)
						return NULL;
					soap_default_tt__PTZPresetTourSpot(soap, a->TourSpot);
				}
				soap_revert(soap);
				if (soap_in_tt__PTZPresetTourSpot(soap, "tt:TourSpot", a->TourSpot, "tt:PTZPresetTourSpot"))
				{	a->__sizeTourSpot++;
					a->TourSpot = NULL;
					continue;
				}
			}
			if (soap_flag_Extension && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__PTZPresetTourExtension(soap, "tt:Extension", &a->Extension, "tt:PTZPresetTourExtension"))
				{	soap_flag_Extension--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if (a->TourSpot)
			soap_pop_block(soap, soap_blist_TourSpot);
		if (a->__sizeTourSpot)
			a->TourSpot = (struct tt__PTZPresetTourSpot *)soap_save_block(soap, soap_blist_TourSpot, NULL, 1);
		else
		{	a->TourSpot = NULL;
			if (soap_blist_TourSpot)
				soap_end_block(soap, soap_blist_TourSpot);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct tt__PresetTour *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__PresetTour, 0, sizeof(struct tt__PresetTour), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_Status > 0 || soap_flag_AutoStart > 0 || soap_flag_StartingCondition > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__PresetTour(struct soap *soap, const struct tt__PresetTour *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__PresetTour);
	if (soap_out_tt__PresetTour(soap, tag?tag:"tt:PresetTour", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PresetTour * SOAP_FMAC4 soap_get_tt__PresetTour(struct soap *soap, struct tt__PresetTour *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__PresetTour(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__PTZPresetTourSupportedExtension(struct soap *soap, struct tt__PTZPresetTourSupportedExtension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__size = 0;
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__PTZPresetTourSupportedExtension(struct soap *soap, const struct tt__PTZPresetTourSupportedExtension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__PTZPresetTourSupportedExtension(struct soap *soap, const char *tag, int id, const struct tt__PTZPresetTourSupportedExtension *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__PTZPresetTourSupportedExtension), type))
		return soap->error;
	if (a->__any)
	{	int i;
		for (i = 0; i < a->__size; i++)
			soap_outliteral(soap, "-any", a->__any + i, NULL);
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__PTZPresetTourSupportedExtension * SOAP_FMAC4 soap_in_tt__PTZPresetTourSupportedExtension(struct soap *soap, const char *tag, struct tt__PTZPresetTourSupportedExtension *a, const char *type)
{
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__PTZPresetTourSupportedExtension *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__PTZPresetTourSupportedExtension, sizeof(struct tt__PTZPresetTourSupportedExtension), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__PTZPresetTourSupportedExtension(soap, a);
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
	{	a = (struct tt__PTZPresetTourSupportedExtension *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__PTZPresetTourSupportedExtension, 0, sizeof(struct tt__PTZPresetTourSupportedExtension), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__PTZPresetTourSupportedExtension(struct soap *soap, const struct tt__PTZPresetTourSupportedExtension *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__PTZPresetTourSupportedExtension);
	if (soap_out_tt__PTZPresetTourSupportedExtension(soap, tag?tag:"tt:PTZPresetTourSupportedExtension", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourSupportedExtension * SOAP_FMAC4 soap_get_tt__PTZPresetTourSupportedExtension(struct soap *soap, struct tt__PTZPresetTourSupportedExtension *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__PTZPresetTourSupportedExtension(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__PTZPresetTourSupported(struct soap *soap, struct tt__PTZPresetTourSupported *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_int(soap, &a->MaximumNumberOfPresetTours);
	a->__sizePTZPresetTourOperation = 0;
	a->PTZPresetTourOperation = NULL;
	a->Extension = NULL;
	a->__anyAttribute = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__PTZPresetTourSupported(struct soap *soap, const struct tt__PTZPresetTourSupported *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_embedded(soap, &a->MaximumNumberOfPresetTours, SOAP_TYPE_int);
	if (a->PTZPresetTourOperation)
	{	int i;
		for (i = 0; i < a->__sizePTZPresetTourOperation; i++)
		{
			soap_embedded(soap, a->PTZPresetTourOperation + i, SOAP_TYPE_tt__PTZPresetTourOperation);
		}
	}
	soap_serialize_PointerTott__PTZPresetTourSupportedExtension(soap, &a->Extension);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__PTZPresetTourSupported(struct soap *soap, const char *tag, int id, const struct tt__PTZPresetTourSupported *a, const char *type)
{
	if (a->__anyAttribute)
		soap_set_attr(soap, "-anyAttribute", a->__anyAttribute, 1);
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__PTZPresetTourSupported), type))
		return soap->error;
	if (soap_out_int(soap, "tt:MaximumNumberOfPresetTours", -1, &a->MaximumNumberOfPresetTours, ""))
		return soap->error;
	if (a->PTZPresetTourOperation)
	{	int i;
		for (i = 0; i < a->__sizePTZPresetTourOperation; i++)
			if (soap_out_tt__PTZPresetTourOperation(soap, "tt:PTZPresetTourOperation", -1, a->PTZPresetTourOperation + i, ""))
				return soap->error;
	}
	if (soap_out_PointerTott__PTZPresetTourSupportedExtension(soap, "tt:Extension", -1, &a->Extension, ""))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__PTZPresetTourSupported * SOAP_FMAC4 soap_in_tt__PTZPresetTourSupported(struct soap *soap, const char *tag, struct tt__PTZPresetTourSupported *a, const char *type)
{
	size_t soap_flag_MaximumNumberOfPresetTours = 1;
	struct soap_blist *soap_blist_PTZPresetTourOperation = NULL;
	size_t soap_flag_Extension = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__PTZPresetTourSupported *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__PTZPresetTourSupported, sizeof(struct tt__PTZPresetTourSupported), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__PTZPresetTourSupported(soap, a);
	if (soap_s2string(soap, soap_attr_value(soap, "-anyAttribute", 0), &a->__anyAttribute, 0, -1))
		return NULL;
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_MaximumNumberOfPresetTours && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_int(soap, "tt:MaximumNumberOfPresetTours", &a->MaximumNumberOfPresetTours, "xsd:int"))
				{	soap_flag_MaximumNumberOfPresetTours--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tt:PTZPresetTourOperation", 1, NULL))
			{	if (a->PTZPresetTourOperation == NULL)
				{	if (soap_blist_PTZPresetTourOperation == NULL)
						soap_blist_PTZPresetTourOperation = soap_new_block(soap);
					a->PTZPresetTourOperation = (enum tt__PTZPresetTourOperation *)soap_push_block(soap, soap_blist_PTZPresetTourOperation, sizeof(enum tt__PTZPresetTourOperation));
					if (a->PTZPresetTourOperation == NULL)
						return NULL;
					soap_default_tt__PTZPresetTourOperation(soap, a->PTZPresetTourOperation);
				}
				soap_revert(soap);
				if (soap_in_tt__PTZPresetTourOperation(soap, "tt:PTZPresetTourOperation", a->PTZPresetTourOperation, "tt:PTZPresetTourOperation"))
				{	a->__sizePTZPresetTourOperation++;
					a->PTZPresetTourOperation = NULL;
					continue;
				}
			}
			if (soap_flag_Extension && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__PTZPresetTourSupportedExtension(soap, "tt:Extension", &a->Extension, "tt:PTZPresetTourSupportedExtension"))
				{	soap_flag_Extension--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
				soap->error = soap_ignore_element(soap);
			if (soap->error == SOAP_NO_TAG)
				break;
			if (soap->error)
				return NULL;
		}
		if (a->PTZPresetTourOperation)
			soap_pop_block(soap, soap_blist_PTZPresetTourOperation);
		if (a->__sizePTZPresetTourOperation)
			a->PTZPresetTourOperation = (enum tt__PTZPresetTourOperation *)soap_save_block(soap, soap_blist_PTZPresetTourOperation, NULL, 1);
		else
		{	a->PTZPresetTourOperation = NULL;
			if (soap_blist_PTZPresetTourOperation)
				soap_end_block(soap, soap_blist_PTZPresetTourOperation);
		}
		if (soap_element_end_in(soap, tag))
			return NULL;
	}
	else
	{	a = (struct tt__PTZPresetTourSupported *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__PTZPresetTourSupported, 0, sizeof(struct tt__PTZPresetTourSupported), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_MaximumNumberOfPresetTours > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__PTZPresetTourSupported(struct soap *soap, const struct tt__PTZPresetTourSupported *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__PTZPresetTourSupported);
	if (soap_out_tt__PTZPresetTourSupported(soap, tag?tag:"tt:PTZPresetTourSupported", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourSupported * SOAP_FMAC4 soap_get_tt__PTZPresetTourSupported(struct soap *soap, struct tt__PTZPresetTourSupported *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__PTZPresetTourSupported(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tt__union_PTZPresetTourPresetDetail(struct soap *soap, int choice, const union _tt__union_PTZPresetTourPresetDetail *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	switch (choice)
	{
	case SOAP_UNION__tt__union_PTZPresetTourPresetDetail_PresetToken:
		soap_serialize_tt__ReferenceToken(soap, &a->PresetToken);
		break;
	case SOAP_UNION__tt__union_PTZPresetTourPresetDetail_Home:
		soap_embedded(soap, &a->Home, SOAP_TYPE_xsd__boolean_);
		break;
	case SOAP_UNION__tt__union_PTZPresetTourPresetDetail_PTZPosition:
		soap_serialize_PointerTott__PTZVector(soap, &a->PTZPosition);
		break;
	case SOAP_UNION__tt__union_PTZPresetTourPresetDetail_TypeExtension:
		soap_serialize_PointerTott__PTZPresetTourTypeExtension(soap, &a->TypeExtension);
		break;
	default:
		break;
	}
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tt__union_PTZPresetTourPresetDetail(struct soap *soap, int choice, const union _tt__union_PTZPresetTourPresetDetail *a)
{
	switch (choice)
	{
	case SOAP_UNION__tt__union_PTZPresetTourPresetDetail_PresetToken:
		return soap_out_tt__ReferenceToken(soap, "tt:PresetToken", -1, &a->PresetToken, "");
	case SOAP_UNION__tt__union_PTZPresetTourPresetDetail_Home:
		return soap_out_xsd__boolean_(soap, "tt:Home", -1, &a->Home, "");
	case SOAP_UNION__tt__union_PTZPresetTourPresetDetail_PTZPosition:
		return soap_out_PointerTott__PTZVector(soap, "tt:PTZPosition", -1, &a->PTZPosition, "");
	case SOAP_UNION__tt__union_PTZPresetTourPresetDetail_TypeExtension:
		return soap_out_PointerTott__PTZPresetTourTypeExtension(soap, "tt:TypeExtension", -1, &a->TypeExtension, "");
	default:
		break;
	}
	return SOAP_OK;
}

SOAP_FMAC3 union _tt__union_PTZPresetTourPresetDetail * SOAP_FMAC4 soap_in__tt__union_PTZPresetTourPresetDetail(struct soap *soap, int *choice, union _tt__union_PTZPresetTourPresetDetail *a)
{	soap->error = SOAP_TAG_MISMATCH;
	a->PresetToken = NULL;
	if (soap->error == SOAP_TAG_MISMATCH && soap_in_tt__ReferenceToken(soap, "tt:PresetToken", &a->PresetToken, "tt:ReferenceToken"))
	{	*choice = SOAP_UNION__tt__union_PTZPresetTourPresetDetail_PresetToken;
		return a;
	}
	if (soap->error == SOAP_TAG_MISMATCH && soap_in_xsd__boolean_(soap, "tt:Home", &a->Home, "xsd:boolean"))
	{	*choice = SOAP_UNION__tt__union_PTZPresetTourPresetDetail_Home;
		return a;
	}
	a->PTZPosition = NULL;
	if (soap->error == SOAP_TAG_MISMATCH && soap_in_PointerTott__PTZVector(soap, "tt:PTZPosition", &a->PTZPosition, "tt:PTZVector"))
	{	*choice = SOAP_UNION__tt__union_PTZPresetTourPresetDetail_PTZPosition;
		return a;
	}
	a->TypeExtension = NULL;
	if (soap->error == SOAP_TAG_MISMATCH && soap_in_PointerTott__PTZPresetTourTypeExtension(soap, "tt:TypeExtension", &a->TypeExtension, "tt:PTZPresetTourTypeExtension"))
	{	*choice = SOAP_UNION__tt__union_PTZPresetTourPresetDetail_TypeExtension;
		return a;
	}
	*choice = 0;
	if (!soap->error)
		soap->error = SOAP_TAG_MISMATCH;
	return NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tptz__RemovePresetTourResponse(struct soap *soap, struct _tptz__RemovePresetTourResponse *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tptz__RemovePresetTourResponse))
		soap_serialize__tptz__RemovePresetTourResponse(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tptz__RemovePresetTourResponse(struct soap *soap, const char *tag, int id, struct _tptz__RemovePresetTourResponse *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tptz__RemovePresetTourResponse);
	if (id < 0)
		return soap->error;
	return soap_out__tptz__RemovePresetTourResponse(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tptz__RemovePresetTourResponse ** SOAP_FMAC4 soap_in_PointerTo_tptz__RemovePresetTourResponse(struct soap *soap, const char *tag, struct _tptz__RemovePresetTourResponse **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tptz__RemovePresetTourResponse **)soap_malloc(soap, sizeof(struct _tptz__RemovePresetTourResponse *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in__tptz__RemovePresetTourResponse(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct _tptz__RemovePresetTourResponse **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tptz__RemovePresetTourResponse, sizeof(struct _tptz__RemovePresetTourResponse), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tptz__RemovePresetTourResponse(struct soap *soap, struct _tptz__RemovePresetTourResponse *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tptz__RemovePresetTourResponse);
	if (soap_out_PointerTo_tptz__RemovePresetTourResponse(soap, tag?tag:"tptz:RemovePresetTourResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__RemovePresetTourResponse ** SOAP_FMAC4 soap_get_PointerTo_tptz__RemovePresetTourResponse(struct soap *soap, struct _tptz__RemovePresetTourResponse **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tptz__RemovePresetTourResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tptz__RemovePresetTour(struct soap *soap, struct _tptz__RemovePresetTour *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tptz__RemovePresetTour))
		soap_serialize__tptz__RemovePresetTour(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tptz__RemovePresetTour(struct soap *soap, const char *tag, int id, struct _tptz__RemovePresetTour *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tptz__RemovePresetTour);
	if (id < 0)
		return soap->error;
	return soap_out__tptz__RemovePresetTour(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tptz__RemovePresetTour ** SOAP_FMAC4 soap_in_PointerTo_tptz__RemovePresetTour(struct soap *soap, const char *tag, struct _tptz__RemovePresetTour **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tptz__RemovePresetTour **)soap_malloc(soap, sizeof(struct _tptz__RemovePresetTour *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in__tptz__RemovePresetTour(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct _tptz__RemovePresetTour **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tptz__RemovePresetTour, sizeof(struct _tptz__RemovePresetTour), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tptz__RemovePresetTour(struct soap *soap, struct _tptz__RemovePresetTour *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tptz__RemovePresetTour);
	if (soap_out_PointerTo_tptz__RemovePresetTour(soap, tag?tag:"tptz:RemovePresetTour", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__RemovePresetTour ** SOAP_FMAC4 soap_get_PointerTo_tptz__RemovePresetTour(struct soap *soap, struct _tptz__RemovePresetTour **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tptz__RemovePresetTour(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tptz__OperatePresetTourResponse(struct soap *soap, struct _tptz__OperatePresetTourResponse *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tptz__OperatePresetTourResponse))
		soap_serialize__tptz__OperatePresetTourResponse(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tptz__OperatePresetTourResponse(struct soap *soap, const char *tag, int id, struct _tptz__OperatePresetTourResponse *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tptz__OperatePresetTourResponse);
	if (id < 0)
		return soap->error;
	return soap_out__tptz__OperatePresetTourResponse(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tptz__OperatePresetTourResponse ** SOAP_FMAC4 soap_in_PointerTo_tptz__OperatePresetTourResponse(struct soap *soap, const char *tag, struct _tptz__OperatePresetTourResponse **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tptz__OperatePresetTourResponse **)soap_malloc(soap, sizeof(struct _tptz__OperatePresetTourResponse *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in__tptz__OperatePresetTourResponse(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct _tptz__OperatePresetTourResponse **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tptz__OperatePresetTourResponse, sizeof(struct _tptz__OperatePresetTourResponse), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tptz__OperatePresetTourResponse(struct soap *soap, struct _tptz__OperatePresetTourResponse *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tptz__OperatePresetTourResponse);
	if (soap_out_PointerTo_tptz__OperatePresetTourResponse(soap, tag?tag:"tptz:OperatePresetTourResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__OperatePresetTourResponse ** SOAP_FMAC4 soap_get_PointerTo_tptz__OperatePresetTourResponse(struct soap *soap, struct _tptz__OperatePresetTourResponse **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tptz__OperatePresetTourResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tptz__OperatePresetTour(struct soap *soap, struct _tptz__OperatePresetTour *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tptz__OperatePresetTour))
		soap_serialize__tptz__OperatePresetTour(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tptz__OperatePresetTour(struct soap *soap, const char *tag, int id, struct _tptz__OperatePresetTour *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tptz__OperatePresetTour);
	if (id < 0)
		return soap->error;
	return soap_out__tptz__OperatePresetTour(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tptz__OperatePresetTour ** SOAP_FMAC4 soap_in_PointerTo_tptz__OperatePresetTour(struct soap *soap, const char *tag, struct _tptz__OperatePresetTour **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tptz__OperatePresetTour **)soap_malloc(soap, sizeof(struct _tptz__OperatePresetTour *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in__tptz__OperatePresetTour(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct _tptz__OperatePresetTour **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tptz__OperatePresetTour, sizeof(struct _tptz__OperatePresetTour), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tptz__OperatePresetTour(struct soap *soap, struct _tptz__OperatePresetTour *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tptz__OperatePresetTour);
	if (soap_out_PointerTo_tptz__OperatePresetTour(soap, tag?tag:"tptz:OperatePresetTour", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__OperatePresetTour ** SOAP_FMAC4 soap_get_PointerTo_tptz__OperatePresetTour(struct soap *soap, struct _tptz__OperatePresetTour **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tptz__OperatePresetTour(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tptz__ModifyPresetTourResponse(struct soap *soap, struct _tptz__ModifyPresetTourResponse *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tptz__ModifyPresetTourResponse))
		soap_serialize__tptz__ModifyPresetTourResponse(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tptz__ModifyPresetTourResponse(struct soap *soap, const char *tag, int id, struct _tptz__ModifyPresetTourResponse *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tptz__ModifyPresetTourResponse);
	if (id < 0)
		return soap->error;
	return soap_out__tptz__ModifyPresetTourResponse(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tptz__ModifyPresetTourResponse ** SOAP_FMAC4 soap_in_PointerTo_tptz__ModifyPresetTourResponse(struct soap *soap, const char *tag, struct _tptz__ModifyPresetTourResponse **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tptz__ModifyPresetTourResponse **)soap_malloc(soap, sizeof(struct _tptz__ModifyPresetTourResponse *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in__tptz__ModifyPresetTourResponse(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct _tptz__ModifyPresetTourResponse **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tptz__ModifyPresetTourResponse, sizeof(struct _tptz__ModifyPresetTourResponse), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tptz__ModifyPresetTourResponse(struct soap *soap, struct _tptz__ModifyPresetTourResponse *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tptz__ModifyPresetTourResponse);
	if (soap_out_PointerTo_tptz__ModifyPresetTourResponse(soap, tag?tag:"tptz:ModifyPresetTourResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__ModifyPresetTourResponse ** SOAP_FMAC4 soap_get_PointerTo_tptz__ModifyPresetTourResponse(struct soap *soap, struct _tptz__ModifyPresetTourResponse **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tptz__ModifyPresetTourResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tptz__ModifyPresetTour(struct soap *soap, struct _tptz__ModifyPresetTour *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tptz__ModifyPresetTour))
		soap_serialize__tptz__ModifyPresetTour(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tptz__ModifyPresetTour(struct soap *soap, const char *tag, int id, struct _tptz__ModifyPresetTour *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tptz__ModifyPresetTour);
	if (id < 0)
		return soap->error;
	return soap_out__tptz__ModifyPresetTour(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tptz__ModifyPresetTour ** SOAP_FMAC4 soap_in_PointerTo_tptz__ModifyPresetTour(struct soap *soap, const char *tag, struct _tptz__ModifyPresetTour **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tptz__ModifyPresetTour **)soap_malloc(soap, sizeof(struct _tptz__ModifyPresetTour *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in__tptz__ModifyPresetTour(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct _tptz__ModifyPresetTour **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tptz__ModifyPresetTour, sizeof(struct _tptz__ModifyPresetTour), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tptz__ModifyPresetTour(struct soap *soap, struct _tptz__ModifyPresetTour *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tptz__ModifyPresetTour);
	if (soap_out_PointerTo_tptz__ModifyPresetTour(soap, tag?tag:"tptz:ModifyPresetTour", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__ModifyPresetTour ** SOAP_FMAC4 soap_get_PointerTo_tptz__ModifyPresetTour(struct soap *soap, struct _tptz__ModifyPresetTour **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tptz__ModifyPresetTour(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tptz__CreatePresetTourResponse(struct soap *soap, struct _tptz__CreatePresetTourResponse *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tptz__CreatePresetTourResponse))
		soap_serialize__tptz__CreatePresetTourResponse(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tptz__CreatePresetTourResponse(struct soap *soap, const char *tag, int id, struct _tptz__CreatePresetTourResponse *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tptz__CreatePresetTourResponse);
	if (id < 0)
		return soap->error;
	return soap_out__tptz__CreatePresetTourResponse(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tptz__CreatePresetTourResponse ** SOAP_FMAC4 soap_in_PointerTo_tptz__CreatePresetTourResponse(struct soap *soap, const char *tag, struct _tptz__CreatePresetTourResponse **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tptz__CreatePresetTourResponse **)soap_malloc(soap, sizeof(struct _tptz__CreatePresetTourResponse *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in__tptz__CreatePresetTourResponse(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct _tptz__CreatePresetTourResponse **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tptz__CreatePresetTourResponse, sizeof(struct _tptz__CreatePresetTourResponse), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tptz__CreatePresetTourResponse(struct soap *soap, struct _tptz__CreatePresetTourResponse *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tptz__CreatePresetTourResponse);
	if (soap_out_PointerTo_tptz__CreatePresetTourResponse(soap, tag?tag:"tptz:CreatePresetTourResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__CreatePresetTourResponse ** SOAP_FMAC4 soap_get_PointerTo_tptz__CreatePresetTourResponse(struct soap *soap, struct _tptz__CreatePresetTourResponse **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tptz__CreatePresetTourResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tptz__CreatePresetTour(struct soap *soap, struct _tptz__CreatePresetTour *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tptz__CreatePresetTour))
		soap_serialize__tptz__CreatePresetTour(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tptz__CreatePresetTour(struct soap *soap, const char *tag, int id, struct _tptz__CreatePresetTour *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tptz__CreatePresetTour);
	if (id < 0)
		return soap->error;
	return soap_out__tptz__CreatePresetTour(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tptz__CreatePresetTour ** SOAP_FMAC4 soap_in_PointerTo_tptz__CreatePresetTour(struct soap *soap, const char *tag, struct _tptz__CreatePresetTour **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tptz__CreatePresetTour **)soap_malloc(soap, sizeof(struct _tptz__CreatePresetTour *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in__tptz__CreatePresetTour(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct _tptz__CreatePresetTour **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tptz__CreatePresetTour, sizeof(struct _tptz__CreatePresetTour), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tptz__CreatePresetTour(struct soap *soap, struct _tptz__CreatePresetTour *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tptz__CreatePresetTour);
	if (soap_out_PointerTo_tptz__CreatePresetTour(soap, tag?tag:"tptz:CreatePresetTour", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__CreatePresetTour ** SOAP_FMAC4 soap_get_PointerTo_tptz__CreatePresetTour(struct soap *soap, struct _tptz__CreatePresetTour **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tptz__CreatePresetTour(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tptz__GetPresetTourOptionsResponse(struct soap *soap, struct _tptz__GetPresetTourOptionsResponse *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tptz__GetPresetTourOptionsResponse))
		soap_serialize__tptz__GetPresetTourOptionsResponse(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tptz__GetPresetTourOptionsResponse(struct soap *soap, const char *tag, int id, struct _tptz__GetPresetTourOptionsResponse *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tptz__GetPresetTourOptionsResponse);
	if (id < 0)
		return soap->error;
	return soap_out__tptz__GetPresetTourOptionsResponse(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tptz__GetPresetTourOptionsResponse ** SOAP_FMAC4 soap_in_PointerTo_tptz__GetPresetTourOptionsResponse(struct soap *soap, const char *tag, struct _tptz__GetPresetTourOptionsResponse **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tptz__GetPresetTourOptionsResponse **)soap_malloc(soap, sizeof(struct _tptz__GetPresetTourOptionsResponse *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in__tptz__GetPresetTourOptionsResponse(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct _tptz__GetPresetTourOptionsResponse **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tptz__GetPresetTourOptionsResponse, sizeof(struct _tptz__GetPresetTourOptionsResponse), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tptz__GetPresetTourOptionsResponse(struct soap *soap, struct _tptz__GetPresetTourOptionsResponse *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tptz__GetPresetTourOptionsResponse);
	if (soap_out_PointerTo_tptz__GetPresetTourOptionsResponse(soap, tag?tag:"tptz:GetPresetTourOptionsResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__GetPresetTourOptionsResponse ** SOAP_FMAC4 soap_get_PointerTo_tptz__GetPresetTourOptionsResponse(struct soap *soap, struct _tptz__GetPresetTourOptionsResponse **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tptz__GetPresetTourOptionsResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tptz__GetPresetTourOptions(struct soap *soap, struct _tptz__GetPresetTourOptions *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tptz__GetPresetTourOptions))
		soap_serialize__tptz__GetPresetTourOptions(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tptz__GetPresetTourOptions(struct soap *soap, const char *tag, int id, struct _tptz__GetPresetTourOptions *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tptz__GetPresetTourOptions);
	if (id < 0)
		return soap->error;
	return soap_out__tptz__GetPresetTourOptions(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tptz__GetPresetTourOptions ** SOAP_FMAC4 soap_in_PointerTo_tptz__GetPresetTourOptions(struct soap *soap, const char *tag, struct _tptz__GetPresetTourOptions **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tptz__GetPresetTourOptions **)soap_malloc(soap, sizeof(struct _tptz__GetPresetTourOptions *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in__tptz__GetPresetTourOptions(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct _tptz__GetPresetTourOptions **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tptz__GetPresetTourOptions, sizeof(struct _tptz__GetPresetTourOptions), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tptz__GetPresetTourOptions(struct soap *soap, struct _tptz__GetPresetTourOptions *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tptz__GetPresetTourOptions);
	if (soap_out_PointerTo_tptz__GetPresetTourOptions(soap, tag?tag:"tptz:GetPresetTourOptions", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__GetPresetTourOptions ** SOAP_FMAC4 soap_get_PointerTo_tptz__GetPresetTourOptions(struct soap *soap, struct _tptz__GetPresetTourOptions **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tptz__GetPresetTourOptions(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tptz__GetPresetTourResponse(struct soap *soap, struct _tptz__GetPresetTourResponse *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tptz__GetPresetTourResponse))
		soap_serialize__tptz__GetPresetTourResponse(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tptz__GetPresetTourResponse(struct soap *soap, const char *tag, int id, struct _tptz__GetPresetTourResponse *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tptz__GetPresetTourResponse);
	if (id < 0)
		return soap->error;
	return soap_out__tptz__GetPresetTourResponse(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tptz__GetPresetTourResponse ** SOAP_FMAC4 soap_in_PointerTo_tptz__GetPresetTourResponse(struct soap *soap, const char *tag, struct _tptz__GetPresetTourResponse **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tptz__GetPresetTourResponse **)soap_malloc(soap, sizeof(struct _tptz__GetPresetTourResponse *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in__tptz__GetPresetTourResponse(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct _tptz__GetPresetTourResponse **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tptz__GetPresetTourResponse, sizeof(struct _tptz__GetPresetTourResponse), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tptz__GetPresetTourResponse(struct soap *soap, struct _tptz__GetPresetTourResponse *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tptz__GetPresetTourResponse);
	if (soap_out_PointerTo_tptz__GetPresetTourResponse(soap, tag?tag:"tptz:GetPresetTourResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__GetPresetTourResponse ** SOAP_FMAC4 soap_get_PointerTo_tptz__GetPresetTourResponse(struct soap *soap, struct _tptz__GetPresetTourResponse **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tptz__GetPresetTourResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tptz__GetPresetTour(struct soap *soap, struct _tptz__GetPresetTour *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tptz__GetPresetTour))
		soap_serialize__tptz__GetPresetTour(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tptz__GetPresetTour(struct soap *soap, const char *tag, int id, struct _tptz__GetPresetTour *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tptz__GetPresetTour);
	if (id < 0)
		return soap->error;
	return soap_out__tptz__GetPresetTour(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tptz__GetPresetTour ** SOAP_FMAC4 soap_in_PointerTo_tptz__GetPresetTour(struct soap *soap, const char *tag, struct _tptz__GetPresetTour **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tptz__GetPresetTour **)soap_malloc(soap, sizeof(struct _tptz__GetPresetTour *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in__tptz__GetPresetTour(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct _tptz__GetPresetTour **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tptz__GetPresetTour, sizeof(struct _tptz__GetPresetTour), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tptz__GetPresetTour(struct soap *soap, struct _tptz__GetPresetTour *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tptz__GetPresetTour);
	if (soap_out_PointerTo_tptz__GetPresetTour(soap, tag?tag:"tptz:GetPresetTour", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__GetPresetTour ** SOAP_FMAC4 soap_get_PointerTo_tptz__GetPresetTour(struct soap *soap, struct _tptz__GetPresetTour **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tptz__GetPresetTour(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tptz__GetPresetToursResponse(struct soap *soap, struct _tptz__GetPresetToursResponse *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tptz__GetPresetToursResponse))
		soap_serialize__tptz__GetPresetToursResponse(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tptz__GetPresetToursResponse(struct soap *soap, const char *tag, int id, struct _tptz__GetPresetToursResponse *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tptz__GetPresetToursResponse);
	if (id < 0)
		return soap->error;
	return soap_out__tptz__GetPresetToursResponse(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tptz__GetPresetToursResponse ** SOAP_FMAC4 soap_in_PointerTo_tptz__GetPresetToursResponse(struct soap *soap, const char *tag, struct _tptz__GetPresetToursResponse **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tptz__GetPresetToursResponse **)soap_malloc(soap, sizeof(struct _tptz__GetPresetToursResponse *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in__tptz__GetPresetToursResponse(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct _tptz__GetPresetToursResponse **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tptz__GetPresetToursResponse, sizeof(struct _tptz__GetPresetToursResponse), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tptz__GetPresetToursResponse(struct soap *soap, struct _tptz__GetPresetToursResponse *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tptz__GetPresetToursResponse);
	if (soap_out_PointerTo_tptz__GetPresetToursResponse(soap, tag?tag:"tptz:GetPresetToursResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__GetPresetToursResponse ** SOAP_FMAC4 soap_get_PointerTo_tptz__GetPresetToursResponse(struct soap *soap, struct _tptz__GetPresetToursResponse **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tptz__GetPresetToursResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tptz__GetPresetTours(struct soap *soap, struct _tptz__GetPresetTours *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__tptz__GetPresetTours))
		soap_serialize__tptz__GetPresetTours(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tptz__GetPresetTours(struct soap *soap, const char *tag, int id, struct _tptz__GetPresetTours *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tptz__GetPresetTours);
	if (id < 0)
		return soap->error;
	return soap_out__tptz__GetPresetTours(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _tptz__GetPresetTours ** SOAP_FMAC4 soap_in_PointerTo_tptz__GetPresetTours(struct soap *soap, const char *tag, struct _tptz__GetPresetTours **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _tptz__GetPresetTours **)soap_malloc(soap, sizeof(struct _tptz__GetPresetTours *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in__tptz__GetPresetTours(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct _tptz__GetPresetTours **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tptz__GetPresetTours, sizeof(struct _tptz__GetPresetTours), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tptz__GetPresetTours(struct soap *soap, struct _tptz__GetPresetTours *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tptz__GetPresetTours);
	if (soap_out_PointerTo_tptz__GetPresetTours(soap, tag?tag:"tptz:GetPresetTours", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__GetPresetTours ** SOAP_FMAC4 soap_get_PointerTo_tptz__GetPresetTours(struct soap *soap, struct _tptz__GetPresetTours **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_tptz__GetPresetTours(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__PresetTour(struct soap *soap, struct tt__PresetTour *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__PresetTour))
		soap_serialize_tt__PresetTour(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__PresetTour(struct soap *soap, const char *tag, int id, struct tt__PresetTour *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__PresetTour);
	if (id < 0)
		return soap->error;
	return soap_out_tt__PresetTour(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__PresetTour ** SOAP_FMAC4 soap_in_PointerTott__PresetTour(struct soap *soap, const char *tag, struct tt__PresetTour **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__PresetTour **)soap_malloc(soap, sizeof(struct tt__PresetTour *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_tt__PresetTour(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct tt__PresetTour **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__PresetTour, sizeof(struct tt__PresetTour), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__PresetTour(struct soap *soap, struct tt__PresetTour *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__PresetTour);
	if (soap_out_PointerTott__PresetTour(soap, tag?tag:"tt:PresetTour", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PresetTour ** SOAP_FMAC4 soap_get_PointerTott__PresetTour(struct soap *soap, struct tt__PresetTour **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__PresetTour(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

//SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tptz__GetPresetTours(struct soap *soap, struct _tptz__GetPresetTours *const*a)
//{
//	if (!soap_reference(soap, *a, SOAP_TYPE__tptz__GetPresetTours))
//		soap_serialize__tptz__GetPresetTours(soap, *a);
//}

//SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tptz__GetPresetTours(struct soap *soap, const char *tag, int id, struct _tptz__GetPresetTours *const*a, const char *type)
//{
//	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__tptz__GetPresetTours);
//	if (id < 0)
//		return soap->error;
//	return soap_out__tptz__GetPresetTours(soap, tag, id, *a, type);
//}

//SOAP_FMAC3 struct _tptz__GetPresetTours ** SOAP_FMAC4 soap_in_PointerTo_tptz__GetPresetTours(struct soap *soap, const char *tag, struct _tptz__GetPresetTours **a, const char *type)
//{
//	if (soap_element_begin_in(soap, tag, 1, NULL))
//		return NULL;
//	if (!a)
//		if (!(a = (struct _tptz__GetPresetTours **)soap_malloc(soap, sizeof(struct _tptz__GetPresetTours *))))
//			return NULL;
//	*a = NULL;
//	if (!soap->null && *soap->href != '#')
//	{	soap_revert(soap);
//		if (!(*a = soap_in__tptz__GetPresetTours(soap, tag, *a, type)))
//			return NULL;
//	}
//	else
//	{	a = (struct _tptz__GetPresetTours **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__tptz__GetPresetTours, sizeof(struct _tptz__GetPresetTours), 0);
//		if (soap->body && soap_element_end_in(soap, tag))
//			return NULL;
//	}
//	return a;
//}

//SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tptz__GetPresetTours(struct soap *soap, struct _tptz__GetPresetTours *const*a, const char *tag, const char *type)
//{
//	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_tptz__GetPresetTours);
//	if (soap_out_PointerTo_tptz__GetPresetTours(soap, tag?tag:"tptz:GetPresetTours", id, a, type))
//		return soap->error;
//	return soap_putindependent(soap);
//}

//SOAP_FMAC3 struct _tptz__GetPresetTours ** SOAP_FMAC4 soap_get_PointerTo_tptz__GetPresetTours(struct soap *soap, struct _tptz__GetPresetTours **p, const char *tag, const char *type)
//{
//	if ((p = soap_in_PointerTo_tptz__GetPresetTours(soap, tag, p, type)))
//		if (soap_getindependent(soap))
//			return NULL;
//	return p;
//}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__PTZPresetTourPresetDetail(struct soap *soap, struct tt__PTZPresetTourPresetDetail *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__PTZPresetTourPresetDetail))
		soap_serialize_tt__PTZPresetTourPresetDetail(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__PTZPresetTourPresetDetail(struct soap *soap, const char *tag, int id, struct tt__PTZPresetTourPresetDetail *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__PTZPresetTourPresetDetail);
	if (id < 0)
		return soap->error;
	return soap_out_tt__PTZPresetTourPresetDetail(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__PTZPresetTourPresetDetail ** SOAP_FMAC4 soap_in_PointerTott__PTZPresetTourPresetDetail(struct soap *soap, const char *tag, struct tt__PTZPresetTourPresetDetail **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__PTZPresetTourPresetDetail **)soap_malloc(soap, sizeof(struct tt__PTZPresetTourPresetDetail *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_tt__PTZPresetTourPresetDetail(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct tt__PTZPresetTourPresetDetail **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__PTZPresetTourPresetDetail, sizeof(struct tt__PTZPresetTourPresetDetail), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__PTZPresetTourPresetDetail(struct soap *soap, struct tt__PTZPresetTourPresetDetail *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__PTZPresetTourPresetDetail);
	if (soap_out_PointerTott__PTZPresetTourPresetDetail(soap, tag?tag:"tt:PTZPresetTourPresetDetail", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourPresetDetail ** SOAP_FMAC4 soap_get_PointerTott__PTZPresetTourPresetDetail(struct soap *soap, struct tt__PTZPresetTourPresetDetail **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__PTZPresetTourPresetDetail(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__PTZPresetTourExtension(struct soap *soap, struct tt__PTZPresetTourExtension *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__PTZPresetTourExtension))
		soap_serialize_tt__PTZPresetTourExtension(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__PTZPresetTourExtension(struct soap *soap, const char *tag, int id, struct tt__PTZPresetTourExtension *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__PTZPresetTourExtension);
	if (id < 0)
		return soap->error;
	return soap_out_tt__PTZPresetTourExtension(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__PTZPresetTourExtension ** SOAP_FMAC4 soap_in_PointerTott__PTZPresetTourExtension(struct soap *soap, const char *tag, struct tt__PTZPresetTourExtension **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__PTZPresetTourExtension **)soap_malloc(soap, sizeof(struct tt__PTZPresetTourExtension *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_tt__PTZPresetTourExtension(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct tt__PTZPresetTourExtension **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__PTZPresetTourExtension, sizeof(struct tt__PTZPresetTourExtension), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__PTZPresetTourExtension(struct soap *soap, struct tt__PTZPresetTourExtension *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__PTZPresetTourExtension);
	if (soap_out_PointerTott__PTZPresetTourExtension(soap, tag?tag:"tt:PTZPresetTourExtension", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourExtension ** SOAP_FMAC4 soap_get_PointerTott__PTZPresetTourExtension(struct soap *soap, struct tt__PTZPresetTourExtension **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__PTZPresetTourExtension(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__PTZPresetTourSpot(struct soap *soap, struct tt__PTZPresetTourSpot *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__PTZPresetTourSpot))
		soap_serialize_tt__PTZPresetTourSpot(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__PTZPresetTourSpot(struct soap *soap, const char *tag, int id, struct tt__PTZPresetTourSpot *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__PTZPresetTourSpot);
	if (id < 0)
		return soap->error;
	return soap_out_tt__PTZPresetTourSpot(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__PTZPresetTourSpot ** SOAP_FMAC4 soap_in_PointerTott__PTZPresetTourSpot(struct soap *soap, const char *tag, struct tt__PTZPresetTourSpot **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__PTZPresetTourSpot **)soap_malloc(soap, sizeof(struct tt__PTZPresetTourSpot *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_tt__PTZPresetTourSpot(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct tt__PTZPresetTourSpot **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__PTZPresetTourSpot, sizeof(struct tt__PTZPresetTourSpot), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__PTZPresetTourSpot(struct soap *soap, struct tt__PTZPresetTourSpot *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__PTZPresetTourSpot);
	if (soap_out_PointerTott__PTZPresetTourSpot(soap, tag?tag:"tt:PTZPresetTourSpot", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourSpot ** SOAP_FMAC4 soap_get_PointerTott__PTZPresetTourSpot(struct soap *soap, struct tt__PTZPresetTourSpot **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__PTZPresetTourSpot(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__PTZPresetTourStartingCondition(struct soap *soap, struct tt__PTZPresetTourStartingCondition *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__PTZPresetTourStartingCondition))
		soap_serialize_tt__PTZPresetTourStartingCondition(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__PTZPresetTourStartingCondition(struct soap *soap, const char *tag, int id, struct tt__PTZPresetTourStartingCondition *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__PTZPresetTourStartingCondition);
	if (id < 0)
		return soap->error;
	return soap_out_tt__PTZPresetTourStartingCondition(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__PTZPresetTourStartingCondition ** SOAP_FMAC4 soap_in_PointerTott__PTZPresetTourStartingCondition(struct soap *soap, const char *tag, struct tt__PTZPresetTourStartingCondition **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__PTZPresetTourStartingCondition **)soap_malloc(soap, sizeof(struct tt__PTZPresetTourStartingCondition *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_tt__PTZPresetTourStartingCondition(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct tt__PTZPresetTourStartingCondition **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__PTZPresetTourStartingCondition, sizeof(struct tt__PTZPresetTourStartingCondition), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__PTZPresetTourStartingCondition(struct soap *soap, struct tt__PTZPresetTourStartingCondition *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__PTZPresetTourStartingCondition);
	if (soap_out_PointerTott__PTZPresetTourStartingCondition(soap, tag?tag:"tt:PTZPresetTourStartingCondition", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourStartingCondition ** SOAP_FMAC4 soap_get_PointerTott__PTZPresetTourStartingCondition(struct soap *soap, struct tt__PTZPresetTourStartingCondition **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__PTZPresetTourStartingCondition(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__PTZPresetTourStatus(struct soap *soap, struct tt__PTZPresetTourStatus *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__PTZPresetTourStatus))
		soap_serialize_tt__PTZPresetTourStatus(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__PTZPresetTourStatus(struct soap *soap, const char *tag, int id, struct tt__PTZPresetTourStatus *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__PTZPresetTourStatus);
	if (id < 0)
		return soap->error;
	return soap_out_tt__PTZPresetTourStatus(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__PTZPresetTourStatus ** SOAP_FMAC4 soap_in_PointerTott__PTZPresetTourStatus(struct soap *soap, const char *tag, struct tt__PTZPresetTourStatus **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__PTZPresetTourStatus **)soap_malloc(soap, sizeof(struct tt__PTZPresetTourStatus *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_tt__PTZPresetTourStatus(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct tt__PTZPresetTourStatus **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__PTZPresetTourStatus, sizeof(struct tt__PTZPresetTourStatus), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__PTZPresetTourStatus(struct soap *soap, struct tt__PTZPresetTourStatus *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__PTZPresetTourStatus);
	if (soap_out_PointerTott__PTZPresetTourStatus(soap, tag?tag:"tt:PTZPresetTourStatus", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourStatus ** SOAP_FMAC4 soap_get_PointerTott__PTZPresetTourStatus(struct soap *soap, struct tt__PTZPresetTourStatus **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__PTZPresetTourStatus(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__PTZPresetTourOptions(struct soap *soap, struct tt__PTZPresetTourOptions *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__PTZPresetTourOptions))
		soap_serialize_tt__PTZPresetTourOptions(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__PTZPresetTourOptions(struct soap *soap, const char *tag, int id, struct tt__PTZPresetTourOptions *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__PTZPresetTourOptions);
	if (id < 0)
		return soap->error;
	return soap_out_tt__PTZPresetTourOptions(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__PTZPresetTourOptions ** SOAP_FMAC4 soap_in_PointerTott__PTZPresetTourOptions(struct soap *soap, const char *tag, struct tt__PTZPresetTourOptions **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__PTZPresetTourOptions **)soap_malloc(soap, sizeof(struct tt__PTZPresetTourOptions *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_tt__PTZPresetTourOptions(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct tt__PTZPresetTourOptions **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__PTZPresetTourOptions, sizeof(struct tt__PTZPresetTourOptions), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__PTZPresetTourOptions(struct soap *soap, struct tt__PTZPresetTourOptions *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__PTZPresetTourOptions);
	if (soap_out_PointerTott__PTZPresetTourOptions(soap, tag?tag:"tt:PTZPresetTourOptions", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourOptions ** SOAP_FMAC4 soap_get_PointerTott__PTZPresetTourOptions(struct soap *soap, struct tt__PTZPresetTourOptions **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__PTZPresetTourOptions(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

//SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__PresetTour(struct soap *soap, struct tt__PresetTour *const*a)
//{
//	if (!soap_reference(soap, *a, SOAP_TYPE_tt__PresetTour))
//		soap_serialize_tt__PresetTour(soap, *a);
//}

//SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__PresetTour(struct soap *soap, const char *tag, int id, struct tt__PresetTour *const*a, const char *type)
//{
//	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__PresetTour);
//	if (id < 0)
//		return soap->error;
//	return soap_out_tt__PresetTour(soap, tag, id, *a, type);
//}

//SOAP_FMAC3 struct tt__PresetTour ** SOAP_FMAC4 soap_in_PointerTott__PresetTour(struct soap *soap, const char *tag, struct tt__PresetTour **a, const char *type)
//{
//	if (soap_element_begin_in(soap, tag, 1, NULL))
//		return NULL;
//	if (!a)
//		if (!(a = (struct tt__PresetTour **)soap_malloc(soap, sizeof(struct tt__PresetTour *))))
//			return NULL;
//	*a = NULL;
//	if (!soap->null && *soap->href != '#')
//	{	soap_revert(soap);
//		if (!(*a = soap_in_tt__PresetTour(soap, tag, *a, type)))
//			return NULL;
//	}
//	else
//	{	a = (struct tt__PresetTour **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__PresetTour, sizeof(struct tt__PresetTour), 0);
//		if (soap->body && soap_element_end_in(soap, tag))
//			return NULL;
//	}
//	return a;
//}

//SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__PresetTour(struct soap *soap, struct tt__PresetTour *const*a, const char *tag, const char *type)
//{
//	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__PresetTour);
//	if (soap_out_PointerTott__PresetTour(soap, tag?tag:"tt:PresetTour", id, a, type))
//		return soap->error;
//	return soap_putindependent(soap);
//}

//SOAP_FMAC3 struct tt__PresetTour ** SOAP_FMAC4 soap_get_PointerTott__PresetTour(struct soap *soap, struct tt__PresetTour **p, const char *tag, const char *type)
//{
//	if ((p = soap_in_PointerTott__PresetTour(soap, tag, p, type)))
//		if (soap_getindependent(soap))
//			return NULL;
//	return p;
//}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__PTZPresetTourPresetDetailOptionsExtension(struct soap *soap, struct tt__PTZPresetTourPresetDetailOptionsExtension *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__PTZPresetTourPresetDetailOptionsExtension))
		soap_serialize_tt__PTZPresetTourPresetDetailOptionsExtension(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__PTZPresetTourPresetDetailOptionsExtension(struct soap *soap, const char *tag, int id, struct tt__PTZPresetTourPresetDetailOptionsExtension *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__PTZPresetTourPresetDetailOptionsExtension);
	if (id < 0)
		return soap->error;
	return soap_out_tt__PTZPresetTourPresetDetailOptionsExtension(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__PTZPresetTourPresetDetailOptionsExtension ** SOAP_FMAC4 soap_in_PointerTott__PTZPresetTourPresetDetailOptionsExtension(struct soap *soap, const char *tag, struct tt__PTZPresetTourPresetDetailOptionsExtension **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__PTZPresetTourPresetDetailOptionsExtension **)soap_malloc(soap, sizeof(struct tt__PTZPresetTourPresetDetailOptionsExtension *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_tt__PTZPresetTourPresetDetailOptionsExtension(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct tt__PTZPresetTourPresetDetailOptionsExtension **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__PTZPresetTourPresetDetailOptionsExtension, sizeof(struct tt__PTZPresetTourPresetDetailOptionsExtension), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__PTZPresetTourPresetDetailOptions(struct soap *soap, struct tt__PTZPresetTourPresetDetailOptions *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__PTZPresetTourPresetDetailOptions))
		soap_serialize_tt__PTZPresetTourPresetDetailOptions(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__PTZPresetTourPresetDetailOptions(struct soap *soap, const char *tag, int id, struct tt__PTZPresetTourPresetDetailOptions *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__PTZPresetTourPresetDetailOptions);
	if (id < 0)
		return soap->error;
	return soap_out_tt__PTZPresetTourPresetDetailOptions(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__PTZPresetTourPresetDetailOptions ** SOAP_FMAC4 soap_in_PointerTott__PTZPresetTourPresetDetailOptions(struct soap *soap, const char *tag, struct tt__PTZPresetTourPresetDetailOptions **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__PTZPresetTourPresetDetailOptions **)soap_malloc(soap, sizeof(struct tt__PTZPresetTourPresetDetailOptions *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_tt__PTZPresetTourPresetDetailOptions(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct tt__PTZPresetTourPresetDetailOptions **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__PTZPresetTourPresetDetailOptions, sizeof(struct tt__PTZPresetTourPresetDetailOptions), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__PTZPresetTourStartingConditionOptionsExtension(struct soap *soap, struct tt__PTZPresetTourStartingConditionOptionsExtension *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__PTZPresetTourStartingConditionOptionsExtension))
		soap_serialize_tt__PTZPresetTourStartingConditionOptionsExtension(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__PTZPresetTourStartingConditionOptionsExtension(struct soap *soap, const char *tag, int id, struct tt__PTZPresetTourStartingConditionOptionsExtension *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__PTZPresetTourStartingConditionOptionsExtension);
	if (id < 0)
		return soap->error;
	return soap_out_tt__PTZPresetTourStartingConditionOptionsExtension(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__PTZPresetTourStartingConditionOptionsExtension ** SOAP_FMAC4 soap_in_PointerTott__PTZPresetTourStartingConditionOptionsExtension(struct soap *soap, const char *tag, struct tt__PTZPresetTourStartingConditionOptionsExtension **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__PTZPresetTourStartingConditionOptionsExtension **)soap_malloc(soap, sizeof(struct tt__PTZPresetTourStartingConditionOptionsExtension *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_tt__PTZPresetTourStartingConditionOptionsExtension(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct tt__PTZPresetTourStartingConditionOptionsExtension **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__PTZPresetTourStartingConditionOptionsExtension, sizeof(struct tt__PTZPresetTourStartingConditionOptionsExtension), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__PTZPresetTourStartingConditionOptionsExtension(struct soap *soap, struct tt__PTZPresetTourStartingConditionOptionsExtension *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__PTZPresetTourStartingConditionOptionsExtension);
	if (soap_out_PointerTott__PTZPresetTourStartingConditionOptionsExtension(soap, tag?tag:"tt:PTZPresetTourStartingConditionOptionsExtension", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourStartingConditionOptionsExtension ** SOAP_FMAC4 soap_get_PointerTott__PTZPresetTourStartingConditionOptionsExtension(struct soap *soap, struct tt__PTZPresetTourStartingConditionOptionsExtension **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__PTZPresetTourStartingConditionOptionsExtension(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__PTZPresetTourSpotOptions(struct soap *soap, struct tt__PTZPresetTourSpotOptions *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__PTZPresetTourSpotOptions))
		soap_serialize_tt__PTZPresetTourSpotOptions(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__PTZPresetTourSpotOptions(struct soap *soap, const char *tag, int id, struct tt__PTZPresetTourSpotOptions *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__PTZPresetTourSpotOptions);
	if (id < 0)
		return soap->error;
	return soap_out_tt__PTZPresetTourSpotOptions(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__PTZPresetTourSpotOptions ** SOAP_FMAC4 soap_in_PointerTott__PTZPresetTourSpotOptions(struct soap *soap, const char *tag, struct tt__PTZPresetTourSpotOptions **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__PTZPresetTourSpotOptions **)soap_malloc(soap, sizeof(struct tt__PTZPresetTourSpotOptions *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_tt__PTZPresetTourSpotOptions(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct tt__PTZPresetTourSpotOptions **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__PTZPresetTourSpotOptions, sizeof(struct tt__PTZPresetTourSpotOptions), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__PTZPresetTourSpotOptions(struct soap *soap, struct tt__PTZPresetTourSpotOptions *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__PTZPresetTourSpotOptions);
	if (soap_out_PointerTott__PTZPresetTourSpotOptions(soap, tag?tag:"tt:PTZPresetTourSpotOptions", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourSpotOptions ** SOAP_FMAC4 soap_get_PointerTott__PTZPresetTourSpotOptions(struct soap *soap, struct tt__PTZPresetTourSpotOptions **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__PTZPresetTourSpotOptions(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__PTZPresetTourStartingConditionOptions(struct soap *soap, struct tt__PTZPresetTourStartingConditionOptions *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__PTZPresetTourStartingConditionOptions))
		soap_serialize_tt__PTZPresetTourStartingConditionOptions(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__PTZPresetTourStartingConditionOptions(struct soap *soap, const char *tag, int id, struct tt__PTZPresetTourStartingConditionOptions *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__PTZPresetTourStartingConditionOptions);
	if (id < 0)
		return soap->error;
	return soap_out_tt__PTZPresetTourStartingConditionOptions(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__PTZPresetTourStartingConditionOptions ** SOAP_FMAC4 soap_in_PointerTott__PTZPresetTourStartingConditionOptions(struct soap *soap, const char *tag, struct tt__PTZPresetTourStartingConditionOptions **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__PTZPresetTourStartingConditionOptions **)soap_malloc(soap, sizeof(struct tt__PTZPresetTourStartingConditionOptions *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_tt__PTZPresetTourStartingConditionOptions(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct tt__PTZPresetTourStartingConditionOptions **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__PTZPresetTourStartingConditionOptions, sizeof(struct tt__PTZPresetTourStartingConditionOptions), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__PTZPresetTourStartingConditionOptions(struct soap *soap, struct tt__PTZPresetTourStartingConditionOptions *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__PTZPresetTourStartingConditionOptions);
	if (soap_out_PointerTott__PTZPresetTourStartingConditionOptions(soap, tag?tag:"tt:PTZPresetTourStartingConditionOptions", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourStartingConditionOptions ** SOAP_FMAC4 soap_get_PointerTott__PTZPresetTourStartingConditionOptions(struct soap *soap, struct tt__PTZPresetTourStartingConditionOptions **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__PTZPresetTourStartingConditionOptions(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__PTZPresetTourSpotExtension(struct soap *soap, struct tt__PTZPresetTourSpotExtension *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__PTZPresetTourSpotExtension))
		soap_serialize_tt__PTZPresetTourSpotExtension(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__PTZPresetTourSpotExtension(struct soap *soap, const char *tag, int id, struct tt__PTZPresetTourSpotExtension *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__PTZPresetTourSpotExtension);
	if (id < 0)
		return soap->error;
	return soap_out_tt__PTZPresetTourSpotExtension(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__PTZPresetTourSpotExtension ** SOAP_FMAC4 soap_in_PointerTott__PTZPresetTourSpotExtension(struct soap *soap, const char *tag, struct tt__PTZPresetTourSpotExtension **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__PTZPresetTourSpotExtension **)soap_malloc(soap, sizeof(struct tt__PTZPresetTourSpotExtension *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_tt__PTZPresetTourSpotExtension(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct tt__PTZPresetTourSpotExtension **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__PTZPresetTourSpotExtension, sizeof(struct tt__PTZPresetTourSpotExtension), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__PTZPresetTourSpotExtension(struct soap *soap, struct tt__PTZPresetTourSpotExtension *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__PTZPresetTourSpotExtension);
	if (soap_out_PointerTott__PTZPresetTourSpotExtension(soap, tag?tag:"tt:PTZPresetTourSpotExtension", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourSpotExtension ** SOAP_FMAC4 soap_get_PointerTott__PTZPresetTourSpotExtension(struct soap *soap, struct tt__PTZPresetTourSpotExtension **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__PTZPresetTourSpotExtension(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__PTZPresetTourStartingConditionExtension(struct soap *soap, struct tt__PTZPresetTourStartingConditionExtension *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__PTZPresetTourStartingConditionExtension))
		soap_serialize_tt__PTZPresetTourStartingConditionExtension(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__PTZPresetTourStartingConditionExtension(struct soap *soap, const char *tag, int id, struct tt__PTZPresetTourStartingConditionExtension *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__PTZPresetTourStartingConditionExtension);
	if (id < 0)
		return soap->error;
	return soap_out_tt__PTZPresetTourStartingConditionExtension(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__PTZPresetTourStartingConditionExtension ** SOAP_FMAC4 soap_in_PointerTott__PTZPresetTourStartingConditionExtension(struct soap *soap, const char *tag, struct tt__PTZPresetTourStartingConditionExtension **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__PTZPresetTourStartingConditionExtension **)soap_malloc(soap, sizeof(struct tt__PTZPresetTourStartingConditionExtension *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_tt__PTZPresetTourStartingConditionExtension(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct tt__PTZPresetTourStartingConditionExtension **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__PTZPresetTourStartingConditionExtension, sizeof(struct tt__PTZPresetTourStartingConditionExtension), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__PTZPresetTourStartingConditionExtension(struct soap *soap, struct tt__PTZPresetTourStartingConditionExtension *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__PTZPresetTourStartingConditionExtension);
	if (soap_out_PointerTott__PTZPresetTourStartingConditionExtension(soap, tag?tag:"tt:PTZPresetTourStartingConditionExtension", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourStartingConditionExtension ** SOAP_FMAC4 soap_get_PointerTott__PTZPresetTourStartingConditionExtension(struct soap *soap, struct tt__PTZPresetTourStartingConditionExtension **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__PTZPresetTourStartingConditionExtension(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__PTZPresetTourDirection(struct soap *soap, enum tt__PTZPresetTourDirection *const*a)
{
	soap_reference(soap, *a, SOAP_TYPE_tt__PTZPresetTourDirection);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__PTZPresetTourDirection(struct soap *soap, const char *tag, int id, enum tt__PTZPresetTourDirection *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__PTZPresetTourDirection);
	if (id < 0)
		return soap->error;
	return soap_out_tt__PTZPresetTourDirection(soap, tag, id, *a, type);
}

SOAP_FMAC3 enum tt__PTZPresetTourDirection ** SOAP_FMAC4 soap_in_PointerTott__PTZPresetTourDirection(struct soap *soap, const char *tag, enum tt__PTZPresetTourDirection **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (enum tt__PTZPresetTourDirection **)soap_malloc(soap, sizeof(enum tt__PTZPresetTourDirection *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_tt__PTZPresetTourDirection(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (enum tt__PTZPresetTourDirection **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__PTZPresetTourDirection, sizeof(enum tt__PTZPresetTourDirection), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__PTZPresetTourDirection(struct soap *soap, enum tt__PTZPresetTourDirection *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__PTZPresetTourDirection);
	if (soap_out_PointerTott__PTZPresetTourDirection(soap, tag?tag:"tt:PTZPresetTourDirection", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 enum tt__PTZPresetTourDirection ** SOAP_FMAC4 soap_get_PointerTott__PTZPresetTourDirection(struct soap *soap, enum tt__PTZPresetTourDirection **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__PTZPresetTourDirection(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__PTZPresetTourStatusExtension(struct soap *soap, struct tt__PTZPresetTourStatusExtension *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__PTZPresetTourStatusExtension))
		soap_serialize_tt__PTZPresetTourStatusExtension(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__PTZPresetTourStatusExtension(struct soap *soap, const char *tag, int id, struct tt__PTZPresetTourStatusExtension *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__PTZPresetTourStatusExtension);
	if (id < 0)
		return soap->error;
	return soap_out_tt__PTZPresetTourStatusExtension(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__PTZPresetTourStatusExtension ** SOAP_FMAC4 soap_in_PointerTott__PTZPresetTourStatusExtension(struct soap *soap, const char *tag, struct tt__PTZPresetTourStatusExtension **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__PTZPresetTourStatusExtension **)soap_malloc(soap, sizeof(struct tt__PTZPresetTourStatusExtension *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_tt__PTZPresetTourStatusExtension(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct tt__PTZPresetTourStatusExtension **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__PTZPresetTourStatusExtension, sizeof(struct tt__PTZPresetTourStatusExtension), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__PTZPresetTourStatusExtension(struct soap *soap, struct tt__PTZPresetTourStatusExtension *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__PTZPresetTourStatusExtension);
	if (soap_out_PointerTott__PTZPresetTourStatusExtension(soap, tag?tag:"tt:PTZPresetTourStatusExtension", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourStatusExtension ** SOAP_FMAC4 soap_get_PointerTott__PTZPresetTourStatusExtension(struct soap *soap, struct tt__PTZPresetTourStatusExtension **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__PTZPresetTourStatusExtension(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__PTZPresetTourTypeExtension(struct soap *soap, struct tt__PTZPresetTourTypeExtension *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__PTZPresetTourTypeExtension))
		soap_serialize_tt__PTZPresetTourTypeExtension(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__PTZPresetTourTypeExtension(struct soap *soap, const char *tag, int id, struct tt__PTZPresetTourTypeExtension *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__PTZPresetTourTypeExtension);
	if (id < 0)
		return soap->error;
	return soap_out_tt__PTZPresetTourTypeExtension(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__PTZPresetTourTypeExtension ** SOAP_FMAC4 soap_in_PointerTott__PTZPresetTourTypeExtension(struct soap *soap, const char *tag, struct tt__PTZPresetTourTypeExtension **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__PTZPresetTourTypeExtension **)soap_malloc(soap, sizeof(struct tt__PTZPresetTourTypeExtension *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_tt__PTZPresetTourTypeExtension(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct tt__PTZPresetTourTypeExtension **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__PTZPresetTourTypeExtension, sizeof(struct tt__PTZPresetTourTypeExtension), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__PTZPresetTourTypeExtension(struct soap *soap, struct tt__PTZPresetTourTypeExtension *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__PTZPresetTourTypeExtension);
	if (soap_out_PointerTott__PTZPresetTourTypeExtension(soap, tag?tag:"tt:PTZPresetTourTypeExtension", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourTypeExtension ** SOAP_FMAC4 soap_get_PointerTott__PTZPresetTourTypeExtension(struct soap *soap, struct tt__PTZPresetTourTypeExtension **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__PTZPresetTourTypeExtension(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__RemovePresetTourResponse(struct soap *soap, const struct _tptz__RemovePresetTourResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__RemovePresetTourResponse(struct soap *soap, const char *tag, int id, const struct _tptz__RemovePresetTourResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__RemovePresetTourResponse), type))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__RemovePresetTourResponse * SOAP_FMAC4 soap_in__tptz__RemovePresetTourResponse(struct soap *soap, const char *tag, struct _tptz__RemovePresetTourResponse *a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__RemovePresetTourResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__RemovePresetTourResponse, sizeof(struct _tptz__RemovePresetTourResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__RemovePresetTourResponse(soap, a);
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
	{	a = (struct _tptz__RemovePresetTourResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__RemovePresetTourResponse, 0, sizeof(struct _tptz__RemovePresetTourResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__RemovePresetTourResponse(struct soap *soap, const struct _tptz__RemovePresetTourResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__RemovePresetTourResponse);
	if (soap_out__tptz__RemovePresetTourResponse(soap, tag?tag:"tptz:RemovePresetTourResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__RemovePresetTourResponse * SOAP_FMAC4 soap_get__tptz__RemovePresetTourResponse(struct soap *soap, struct _tptz__RemovePresetTourResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__RemovePresetTourResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__RemovePresetTour(struct soap *soap, struct _tptz__RemovePresetTour *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->ProfileToken);
	soap_default_tt__ReferenceToken(soap, &a->PresetTourToken);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__RemovePresetTour(struct soap *soap, const struct _tptz__RemovePresetTour *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->ProfileToken);
	soap_serialize_tt__ReferenceToken(soap, &a->PresetTourToken);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__RemovePresetTour(struct soap *soap, const char *tag, int id, const struct _tptz__RemovePresetTour *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__RemovePresetTour), type))
		return soap->error;
	if (a->ProfileToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tptz:ProfileToken", -1, &a->ProfileToken, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:ProfileToken"))
		return soap->error;
	if (a->PresetTourToken)
	{	if (soap_out_tt__ReferenceToken(soap, "tptz:PresetTourToken", -1, &a->PresetTourToken, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "tptz:PresetTourToken"))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__RemovePresetTour * SOAP_FMAC4 soap_in__tptz__RemovePresetTour(struct soap *soap, const char *tag, struct _tptz__RemovePresetTour *a, const char *type)
{
	size_t soap_flag_ProfileToken = 1;
	size_t soap_flag_PresetTourToken = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__RemovePresetTour *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__RemovePresetTour, sizeof(struct _tptz__RemovePresetTour), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__RemovePresetTour(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_ProfileToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_tt__ReferenceToken(soap, "tptz:ProfileToken", &a->ProfileToken, "tt:ReferenceToken"))
				{	soap_flag_ProfileToken--;
					continue;
				}
			if (soap_flag_PresetTourToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_tt__ReferenceToken(soap, "tptz:PresetTourToken", &a->PresetTourToken, "tt:ReferenceToken"))
				{	soap_flag_PresetTourToken--;
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
	{	a = (struct _tptz__RemovePresetTour *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__RemovePresetTour, 0, sizeof(struct _tptz__RemovePresetTour), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_ProfileToken > 0 || soap_flag_PresetTourToken > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__RemovePresetTour(struct soap *soap, const struct _tptz__RemovePresetTour *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__RemovePresetTour);
	if (soap_out__tptz__RemovePresetTour(soap, tag?tag:"tptz:RemovePresetTour", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__RemovePresetTour * SOAP_FMAC4 soap_get__tptz__RemovePresetTour(struct soap *soap, struct _tptz__RemovePresetTour *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__RemovePresetTour(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__OperatePresetTourResponse(struct soap *soap, struct _tptz__OperatePresetTourResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__OperatePresetTourResponse(struct soap *soap, const struct _tptz__OperatePresetTourResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__OperatePresetTourResponse(struct soap *soap, const char *tag, int id, const struct _tptz__OperatePresetTourResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__OperatePresetTourResponse), type))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _tptz__OperatePresetTourResponse * SOAP_FMAC4 soap_in__tptz__OperatePresetTourResponse(struct soap *soap, const char *tag, struct _tptz__OperatePresetTourResponse *a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _tptz__OperatePresetTourResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__OperatePresetTourResponse, sizeof(struct _tptz__OperatePresetTourResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__tptz__OperatePresetTourResponse(soap, a);
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
	{	a = (struct _tptz__OperatePresetTourResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__OperatePresetTourResponse, 0, sizeof(struct _tptz__OperatePresetTourResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__OperatePresetTourResponse(struct soap *soap, const struct _tptz__OperatePresetTourResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__OperatePresetTourResponse);
	if (soap_out__tptz__OperatePresetTourResponse(soap, tag?tag:"tptz:OperatePresetTourResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _tptz__OperatePresetTourResponse * SOAP_FMAC4 soap_get__tptz__OperatePresetTourResponse(struct soap *soap, struct _tptz__OperatePresetTourResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__tptz__OperatePresetTourResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

//SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__OperatePresetTour(struct soap *soap, struct _tptz__OperatePresetTour *a)
//{
//	(void)soap; (void)a; /* appease -Wall -Werror */
//	soap_default_tt__ReferenceToken(soap, &a->ProfileToken);
//	soap_default_tt__ReferenceToken(soap, &a->PresetTourToken);
//	soap_default_tt__PTZPresetTourOperation(soap, &a->Operation);
//}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__PTZPresetTourSupportedExtension(struct soap *soap, struct tt__PTZPresetTourSupportedExtension *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__PTZPresetTourSupportedExtension))
		soap_serialize_tt__PTZPresetTourSupportedExtension(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__PTZPresetTourSupportedExtension(struct soap *soap, const char *tag, int id, struct tt__PTZPresetTourSupportedExtension *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__PTZPresetTourSupportedExtension);
	if (id < 0)
		return soap->error;
	return soap_out_tt__PTZPresetTourSupportedExtension(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__PTZPresetTourSupportedExtension ** SOAP_FMAC4 soap_in_PointerTott__PTZPresetTourSupportedExtension(struct soap *soap, const char *tag, struct tt__PTZPresetTourSupportedExtension **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__PTZPresetTourSupportedExtension **)soap_malloc(soap, sizeof(struct tt__PTZPresetTourSupportedExtension *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_tt__PTZPresetTourSupportedExtension(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct tt__PTZPresetTourSupportedExtension **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__PTZPresetTourSupportedExtension, sizeof(struct tt__PTZPresetTourSupportedExtension), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__PTZPresetTourSupportedExtension(struct soap *soap, struct tt__PTZPresetTourSupportedExtension *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__PTZPresetTourSupportedExtension);
	if (soap_out_PointerTott__PTZPresetTourSupportedExtension(soap, tag?tag:"tt:PTZPresetTourSupportedExtension", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__PTZPresetTourSupportedExtension ** SOAP_FMAC4 soap_get_PointerTott__PTZPresetTourSupportedExtension(struct soap *soap, struct tt__PTZPresetTourSupportedExtension **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__PTZPresetTourSupportedExtension(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__RemovePresetTourResponse(struct soap *soap, struct _tptz__RemovePresetTourResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

//SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__RemovePresetTourResponse(struct soap *soap, const struct _tptz__RemovePresetTourResponse *a)
//{
//	(void)soap; (void)a; /* appease -Wall -Werror */
//}
//
//SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__RemovePresetTourResponse(struct soap *soap, const char *tag, int id, const struct _tptz__RemovePresetTourResponse *a, const char *type)
//{
//	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__RemovePresetTourResponse), type))
//		return soap->error;
//	return soap_element_end_out(soap, tag);
//}
//
//SOAP_FMAC3 struct _tptz__RemovePresetTourResponse * SOAP_FMAC4 soap_in__tptz__RemovePresetTourResponse(struct soap *soap, const char *tag, struct _tptz__RemovePresetTourResponse *a, const char *type)
//{
//	if (soap_element_begin_in(soap, tag, 0, type))
//		return NULL;
//	a = (struct _tptz__RemovePresetTourResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__RemovePresetTourResponse, sizeof(struct _tptz__RemovePresetTourResponse), 0, NULL, NULL, NULL);
//	if (!a)
//		return NULL;
//	soap_default__tptz__RemovePresetTourResponse(soap, a);
//	if (soap->body && !*soap->href)
//	{
//		for (;;)
//		{	soap->error = SOAP_TAG_MISMATCH;
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
//	{	a = (struct _tptz__RemovePresetTourResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__RemovePresetTourResponse, 0, sizeof(struct _tptz__RemovePresetTourResponse), 0, NULL);
//		if (soap->body && soap_element_end_in(soap, tag))
//			return NULL;
//	}
//	return a;
//}
//
//SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__RemovePresetTourResponse(struct soap *soap, const struct _tptz__RemovePresetTourResponse *a, const char *tag, const char *type)
//{
//	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__RemovePresetTourResponse);
//	if (soap_out__tptz__RemovePresetTourResponse(soap, tag?tag:"tptz:RemovePresetTourResponse", id, a, type))
//		return soap->error;
//	return soap_putindependent(soap);
//}
//
//SOAP_FMAC3 struct _tptz__RemovePresetTourResponse * SOAP_FMAC4 soap_get__tptz__RemovePresetTourResponse(struct soap *soap, struct _tptz__RemovePresetTourResponse *p, const char *tag, const char *type)
//{
//	if ((p = soap_in__tptz__RemovePresetTourResponse(soap, tag, p, type)))
//		if (soap_getindependent(soap))
//			return NULL;
//	return p;
//}
//
//SOAP_FMAC3 void SOAP_FMAC4 soap_default__tptz__OperatePresetTourResponse(struct soap *soap, struct _tptz__OperatePresetTourResponse *a)
//{
//	(void)soap; (void)a; /* appease -Wall -Werror */
//}
//
//SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tptz__OperatePresetTourResponse(struct soap *soap, const struct _tptz__OperatePresetTourResponse *a)
//{
//	(void)soap; (void)a; /* appease -Wall -Werror */
//}
//
//SOAP_FMAC3 int SOAP_FMAC4 soap_out__tptz__OperatePresetTourResponse(struct soap *soap, const char *tag, int id, const struct _tptz__OperatePresetTourResponse *a, const char *type)
//{
//	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__tptz__OperatePresetTourResponse), type))
//		return soap->error;
//	return soap_element_end_out(soap, tag);
//}
//
//SOAP_FMAC3 struct _tptz__OperatePresetTourResponse * SOAP_FMAC4 soap_in__tptz__OperatePresetTourResponse(struct soap *soap, const char *tag, struct _tptz__OperatePresetTourResponse *a, const char *type)
//{
//	if (soap_element_begin_in(soap, tag, 0, type))
//		return NULL;
//	a = (struct _tptz__OperatePresetTourResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__tptz__OperatePresetTourResponse, sizeof(struct _tptz__OperatePresetTourResponse), 0, NULL, NULL, NULL);
//	if (!a)
//		return NULL;
//	soap_default__tptz__OperatePresetTourResponse(soap, a);
//	if (soap->body && !*soap->href)
//	{
//		for (;;)
//		{	soap->error = SOAP_TAG_MISMATCH;
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
//	{	a = (struct _tptz__OperatePresetTourResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__tptz__OperatePresetTourResponse, 0, sizeof(struct _tptz__OperatePresetTourResponse), 0, NULL);
//		if (soap->body && soap_element_end_in(soap, tag))
//			return NULL;
//	}
//	return a;
//}
//
//SOAP_FMAC3 int SOAP_FMAC4 soap_put__tptz__OperatePresetTourResponse(struct soap *soap, const struct _tptz__OperatePresetTourResponse *a, const char *tag, const char *type)
//{
//	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__tptz__OperatePresetTourResponse);
//	if (soap_out__tptz__OperatePresetTourResponse(soap, tag?tag:"tptz:OperatePresetTourResponse", id, a, type))
//		return soap->error;
//	return soap_putindependent(soap);
//}
//
//SOAP_FMAC3 struct _tptz__OperatePresetTourResponse * SOAP_FMAC4 soap_get__tptz__OperatePresetTourResponse(struct soap *soap, struct _tptz__OperatePresetTourResponse *p, const char *tag, const char *type)
//{
//	if ((p = soap_in__tptz__OperatePresetTourResponse(soap, tag, p, type)))
//		if (soap_getindependent(soap))
//			return NULL;
//	return p;
//}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___tptz__GetPresetTours(struct soap *soap, const struct __tptz__GetPresetTours *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_tptz__GetPresetTours(soap, &a->tptz__GetPresetTours);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___tptz__GetPresetTours(struct soap *soap, const char *tag, int id, const struct __tptz__GetPresetTours *a, const char *type)
{
	if (soap_out_PointerTo_tptz__GetPresetTours(soap, "tptz:GetPresetTours", -1, &a->tptz__GetPresetTours, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__GetPresetTours * SOAP_FMAC4 soap_in___tptz__GetPresetTours(struct soap *soap, const char *tag, struct __tptz__GetPresetTours *a, const char *type)
{
	size_t soap_flag_tptz__GetPresetTours = 1;
	short soap_flag;
	a = (struct __tptz__GetPresetTours *)soap_id_enter(soap, "", a, SOAP_TYPE___tptz__GetPresetTours, sizeof(struct __tptz__GetPresetTours), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___tptz__GetPresetTours(soap, a);
		for (soap_flag = 0;; soap_flag = 1)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_tptz__GetPresetTours && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTo_tptz__GetPresetTours(soap, "tptz:GetPresetTours", &a->tptz__GetPresetTours, ""))
				{	soap_flag_tptz__GetPresetTours--;
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

SOAP_FMAC3 int SOAP_FMAC4 soap_put___tptz__GetPresetTours(struct soap *soap, const struct __tptz__GetPresetTours *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___tptz__GetPresetTours(soap, tag?tag:"-tptz:GetPresetTours", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__GetPresetTours * SOAP_FMAC4 soap_get___tptz__GetPresetTours(struct soap *soap, struct __tptz__GetPresetTours *p, const char *tag, const char *type)
{
	if ((p = soap_in___tptz__GetPresetTours(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___tptz__GetPresetTour(struct soap *soap, struct __tptz__GetPresetTour *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->tptz__GetPresetTour = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___tptz__GetPresetTour(struct soap *soap, const struct __tptz__GetPresetTour *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_tptz__GetPresetTour(soap, &a->tptz__GetPresetTour);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___tptz__GetPresetTour(struct soap *soap, const char *tag, int id, const struct __tptz__GetPresetTour *a, const char *type)
{
	if (soap_out_PointerTo_tptz__GetPresetTour(soap, "tptz:GetPresetTour", -1, &a->tptz__GetPresetTour, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__GetPresetTour * SOAP_FMAC4 soap_in___tptz__GetPresetTour(struct soap *soap, const char *tag, struct __tptz__GetPresetTour *a, const char *type)
{
	size_t soap_flag_tptz__GetPresetTour = 1;
	short soap_flag;
	a = (struct __tptz__GetPresetTour *)soap_id_enter(soap, "", a, SOAP_TYPE___tptz__GetPresetTour, sizeof(struct __tptz__GetPresetTour), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___tptz__GetPresetTour(soap, a);
		for (soap_flag = 0;; soap_flag = 1)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_tptz__GetPresetTour && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTo_tptz__GetPresetTour(soap, "tptz:GetPresetTour", &a->tptz__GetPresetTour, ""))
				{	soap_flag_tptz__GetPresetTour--;
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

SOAP_FMAC3 int SOAP_FMAC4 soap_put___tptz__GetPresetTour(struct soap *soap, const struct __tptz__GetPresetTour *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___tptz__GetPresetTour(soap, tag?tag:"-tptz:GetPresetTour", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__GetPresetTour * SOAP_FMAC4 soap_get___tptz__GetPresetTour(struct soap *soap, struct __tptz__GetPresetTour *p, const char *tag, const char *type)
{
	if ((p = soap_in___tptz__GetPresetTour(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

//SOAP_FMAC3 void SOAP_FMAC4 soap_default___tptz__GetPresetTourOptions(struct soap *soap, struct __tptz__GetPresetTourOptions *a)
//{
//	(void)soap; (void)a; /* appease -Wall -Werror */
//	a->tptz__GetPresetTourOptions = NULL;
//}
//
//SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___tptz__GetPresetTourOptions(struct soap *soap, const struct __tptz__GetPresetTourOptions *a)
//{
//	(void)soap; (void)a; /* appease -Wall -Werror */
//	soap_serialize_PointerTo_tptz__GetPresetTourOptions(soap, &a->tptz__GetPresetTourOptions);
//}
//
//SOAP_FMAC3 int SOAP_FMAC4 soap_out___tptz__GetPresetTourOptions(struct soap *soap, const char *tag, int id, const struct __tptz__GetPresetTourOptions *a, const char *type)
//{
//	if (soap_out_PointerTo_tptz__GetPresetTourOptions(soap, "tptz:GetPresetTourOptions", -1, &a->tptz__GetPresetTourOptions, ""))
//		return soap->error;
//	return SOAP_OK;
//}
//
//SOAP_FMAC3 struct __tptz__GetPresetTourOptions * SOAP_FMAC4 soap_in___tptz__GetPresetTourOptions(struct soap *soap, const char *tag, struct __tptz__GetPresetTourOptions *a, const char *type)
//{
//	size_t soap_flag_tptz__GetPresetTourOptions = 1;
//	short soap_flag;
//	a = (struct __tptz__GetPresetTourOptions *)soap_id_enter(soap, "", a, SOAP_TYPE___tptz__GetPresetTourOptions, sizeof(struct __tptz__GetPresetTourOptions), 0, NULL, NULL, NULL);
//	if (!a)
//		return NULL;
//	soap_default___tptz__GetPresetTourOptions(soap, a);
//		for (soap_flag = 0;; soap_flag = 1)
//		{	soap->error = SOAP_TAG_MISMATCH;
//			if (soap_flag_tptz__GetPresetTourOptions && soap->error == SOAP_TAG_MISMATCH)
//				if (soap_in_PointerTo_tptz__GetPresetTourOptions(soap, "tptz:GetPresetTourOptions", &a->tptz__GetPresetTourOptions, ""))
//				{	soap_flag_tptz__GetPresetTourOptions--;
//					continue;
//				}
//			if (soap->error == SOAP_TAG_MISMATCH)
//				if (soap_flag)
//				{	soap->error = SOAP_OK;
//					break;
//				}
//			if (soap_flag && soap->error == SOAP_NO_TAG)
//				break;
//			if (soap->error)
//				return NULL;
//		}
//	return a;
//}
//
//SOAP_FMAC3 int SOAP_FMAC4 soap_put___tptz__GetPresetTourOptions(struct soap *soap, const struct __tptz__GetPresetTourOptions *a, const char *tag, const char *type)
//{
//	register int id = 0;
//	if (soap_out___tptz__GetPresetTourOptions(soap, tag?tag:"-tptz:GetPresetTourOptions", id, a, type))
//		return soap->error;
//	return SOAP_OK;
//}
//
//SOAP_FMAC3 struct __tptz__GetPresetTourOptions * SOAP_FMAC4 soap_get___tptz__GetPresetTourOptions(struct soap *soap, struct __tptz__GetPresetTourOptions *p, const char *tag, const char *type)
//{
//	if ((p = soap_in___tptz__GetPresetTourOptions(soap, tag, p, type)))
//		if (soap_getindependent(soap))
//			return NULL;
//	return p;
//}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___tptz__CreatePresetTour(struct soap *soap, const struct __tptz__CreatePresetTour *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_tptz__CreatePresetTour(soap, &a->tptz__CreatePresetTour);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___tptz__CreatePresetTour(struct soap *soap, const char *tag, int id, const struct __tptz__CreatePresetTour *a, const char *type)
{
	if (soap_out_PointerTo_tptz__CreatePresetTour(soap, "tptz:CreatePresetTour", -1, &a->tptz__CreatePresetTour, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__CreatePresetTour * SOAP_FMAC4 soap_in___tptz__CreatePresetTour(struct soap *soap, const char *tag, struct __tptz__CreatePresetTour *a, const char *type)
{
	size_t soap_flag_tptz__CreatePresetTour = 1;
	short soap_flag;
	a = (struct __tptz__CreatePresetTour *)soap_id_enter(soap, "", a, SOAP_TYPE___tptz__CreatePresetTour, sizeof(struct __tptz__CreatePresetTour), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___tptz__CreatePresetTour(soap, a);
		for (soap_flag = 0;; soap_flag = 1)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_tptz__CreatePresetTour && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTo_tptz__CreatePresetTour(soap, "tptz:CreatePresetTour", &a->tptz__CreatePresetTour, ""))
				{	soap_flag_tptz__CreatePresetTour--;
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

SOAP_FMAC3 int SOAP_FMAC4 soap_put___tptz__CreatePresetTour(struct soap *soap, const struct __tptz__CreatePresetTour *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___tptz__CreatePresetTour(soap, tag?tag:"-tptz:CreatePresetTour", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__CreatePresetTour * SOAP_FMAC4 soap_get___tptz__CreatePresetTour(struct soap *soap, struct __tptz__CreatePresetTour *p, const char *tag, const char *type)
{
	if ((p = soap_in___tptz__CreatePresetTour(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___tptz__ModifyPresetTour(struct soap *soap, const struct __tptz__ModifyPresetTour *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_tptz__ModifyPresetTour(soap, &a->tptz__ModifyPresetTour);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___tptz__ModifyPresetTour(struct soap *soap, const char *tag, int id, const struct __tptz__ModifyPresetTour *a, const char *type)
{
	if (soap_out_PointerTo_tptz__ModifyPresetTour(soap, "tptz:ModifyPresetTour", -1, &a->tptz__ModifyPresetTour, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__ModifyPresetTour * SOAP_FMAC4 soap_in___tptz__ModifyPresetTour(struct soap *soap, const char *tag, struct __tptz__ModifyPresetTour *a, const char *type)
{
	size_t soap_flag_tptz__ModifyPresetTour = 1;
	short soap_flag;
	a = (struct __tptz__ModifyPresetTour *)soap_id_enter(soap, "", a, SOAP_TYPE___tptz__ModifyPresetTour, sizeof(struct __tptz__ModifyPresetTour), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___tptz__ModifyPresetTour(soap, a);
		for (soap_flag = 0;; soap_flag = 1)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_tptz__ModifyPresetTour && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTo_tptz__ModifyPresetTour(soap, "tptz:ModifyPresetTour", &a->tptz__ModifyPresetTour, ""))
				{	soap_flag_tptz__ModifyPresetTour--;
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

SOAP_FMAC3 int SOAP_FMAC4 soap_put___tptz__ModifyPresetTour(struct soap *soap, const struct __tptz__ModifyPresetTour *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___tptz__ModifyPresetTour(soap, tag?tag:"-tptz:ModifyPresetTour", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__ModifyPresetTour * SOAP_FMAC4 soap_get___tptz__ModifyPresetTour(struct soap *soap, struct __tptz__ModifyPresetTour *p, const char *tag, const char *type)
{
	if ((p = soap_in___tptz__ModifyPresetTour(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___tptz__OperatePresetTour(struct soap *soap, const struct __tptz__OperatePresetTour *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_tptz__OperatePresetTour(soap, &a->tptz__OperatePresetTour);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___tptz__OperatePresetTour(struct soap *soap, const char *tag, int id, const struct __tptz__OperatePresetTour *a, const char *type)
{
	if (soap_out_PointerTo_tptz__OperatePresetTour(soap, "tptz:OperatePresetTour", -1, &a->tptz__OperatePresetTour, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__OperatePresetTour * SOAP_FMAC4 soap_in___tptz__OperatePresetTour(struct soap *soap, const char *tag, struct __tptz__OperatePresetTour *a, const char *type)
{
	size_t soap_flag_tptz__OperatePresetTour = 1;
	short soap_flag;
	a = (struct __tptz__OperatePresetTour *)soap_id_enter(soap, "", a, SOAP_TYPE___tptz__OperatePresetTour, sizeof(struct __tptz__OperatePresetTour), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___tptz__OperatePresetTour(soap, a);
		for (soap_flag = 0;; soap_flag = 1)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_tptz__OperatePresetTour && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTo_tptz__OperatePresetTour(soap, "tptz:OperatePresetTour", &a->tptz__OperatePresetTour, ""))
				{	soap_flag_tptz__OperatePresetTour--;
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

SOAP_FMAC3 int SOAP_FMAC4 soap_put___tptz__OperatePresetTour(struct soap *soap, const struct __tptz__OperatePresetTour *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___tptz__OperatePresetTour(soap, tag?tag:"-tptz:OperatePresetTour", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__OperatePresetTour * SOAP_FMAC4 soap_get___tptz__OperatePresetTour(struct soap *soap, struct __tptz__OperatePresetTour *p, const char *tag, const char *type)
{
	if ((p = soap_in___tptz__OperatePresetTour(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___tptz__RemovePresetTour(struct soap *soap, const struct __tptz__RemovePresetTour *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_tptz__RemovePresetTour(soap, &a->tptz__RemovePresetTour);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___tptz__RemovePresetTour(struct soap *soap, const char *tag, int id, const struct __tptz__RemovePresetTour *a, const char *type)
{
	if (soap_out_PointerTo_tptz__RemovePresetTour(soap, "tptz:RemovePresetTour", -1, &a->tptz__RemovePresetTour, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__RemovePresetTour * SOAP_FMAC4 soap_in___tptz__RemovePresetTour(struct soap *soap, const char *tag, struct __tptz__RemovePresetTour *a, const char *type)
{
	size_t soap_flag_tptz__RemovePresetTour = 1;
	short soap_flag;
	a = (struct __tptz__RemovePresetTour *)soap_id_enter(soap, "", a, SOAP_TYPE___tptz__RemovePresetTour, sizeof(struct __tptz__RemovePresetTour), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___tptz__RemovePresetTour(soap, a);
		for (soap_flag = 0;; soap_flag = 1)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_tptz__RemovePresetTour && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTo_tptz__RemovePresetTour(soap, "tptz:RemovePresetTour", &a->tptz__RemovePresetTour, ""))
				{	soap_flag_tptz__RemovePresetTour--;
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

SOAP_FMAC3 int SOAP_FMAC4 soap_put___tptz__RemovePresetTour(struct soap *soap, const struct __tptz__RemovePresetTour *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___tptz__RemovePresetTour(soap, tag?tag:"-tptz:RemovePresetTour", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __tptz__RemovePresetTour * SOAP_FMAC4 soap_get___tptz__RemovePresetTour(struct soap *soap, struct __tptz__RemovePresetTour *p, const char *tag, const char *type)
{
	if ((p = soap_in___tptz__RemovePresetTour(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___tptz__GetPresetTours(struct soap *soap, struct __tptz__GetPresetTours *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->tptz__GetPresetTours = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___tptz__CreatePresetTour(struct soap *soap, struct __tptz__CreatePresetTour *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->tptz__CreatePresetTour = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___tptz__ModifyPresetTour(struct soap *soap, struct __tptz__ModifyPresetTour *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->tptz__ModifyPresetTour = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___tptz__OperatePresetTour(struct soap *soap, struct __tptz__OperatePresetTour *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->tptz__OperatePresetTour = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___tptz__RemovePresetTour(struct soap *soap, struct __tptz__RemovePresetTour *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->tptz__RemovePresetTour = NULL;
}

//SOAP_FMAC3 struct _tptz__GetPresetTourResponse * SOAP_FMAC4 soap_get__tptz__GetPresetTourResponse(struct soap *soap, struct _tptz__GetPresetTourResponse *p, const char *tag, const char *type)
//{
//	if ((p = soap_in__tptz__GetPresetTourResponse(soap, tag, p, type)))
//		if (soap_getindependent(soap))
//			return NULL;
//	return p;
//}
//***************************************&&&&&&&&&&&&&&&&&&&&&&&***************

SOAP_FMAC3 void SOAP_FMAC4 soap_default___timg__Move(struct soap *soap, struct __timg__Move *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->timg__Move = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___timg__Move(struct soap *soap, const struct __timg__Move *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_timg__Move(soap, &a->timg__Move);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___timg__Move(struct soap *soap, const char *tag, int id, const struct __timg__Move *a, const char *type)
{
	if (soap_out_PointerTo_timg__Move(soap, "timg:Move", -1, &a->timg__Move, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __timg__Move * SOAP_FMAC4 soap_in___timg__Move(struct soap *soap, const char *tag, struct __timg__Move *a, const char *type)
{
	size_t soap_flag_timg__Move = 1;
	short soap_flag;
	a = (struct __timg__Move *)soap_id_enter(soap, "", a, SOAP_TYPE___timg__Move, sizeof(struct __timg__Move), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___timg__Move(soap, a);
		for (soap_flag = 0;; soap_flag = 1)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_timg__Move && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTo_timg__Move(soap, "timg:Move", &a->timg__Move, ""))
				{	soap_flag_timg__Move--;
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

SOAP_FMAC3 int SOAP_FMAC4 soap_put___timg__Move(struct soap *soap, const struct __timg__Move *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___timg__Move(soap, tag?tag:"-timg:Move", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __timg__Move * SOAP_FMAC4 soap_get___timg__Move(struct soap *soap, struct __timg__Move *p, const char *tag, const char *type)
{
	if ((p = soap_in___timg__Move(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__timg__MoveResponse(struct soap *soap, struct _timg__MoveResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__timg__MoveResponse(struct soap *soap, const struct _timg__MoveResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__timg__MoveResponse(struct soap *soap, const char *tag, int id, const struct _timg__MoveResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__timg__MoveResponse), type))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _timg__MoveResponse * SOAP_FMAC4 soap_in__timg__MoveResponse(struct soap *soap, const char *tag, struct _timg__MoveResponse *a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _timg__MoveResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__timg__MoveResponse, sizeof(struct _timg__MoveResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__timg__MoveResponse(soap, a);
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
	{	a = (struct _timg__MoveResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__timg__MoveResponse, 0, sizeof(struct _timg__MoveResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__timg__MoveResponse(struct soap *soap, const struct _timg__MoveResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__timg__MoveResponse);
	if (soap_out__timg__MoveResponse(soap, tag?tag:"timg:MoveResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _timg__MoveResponse * SOAP_FMAC4 soap_get__timg__MoveResponse(struct soap *soap, struct _timg__MoveResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__timg__MoveResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_timg__MoveResponse(struct soap *soap, struct _timg__MoveResponse *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__timg__MoveResponse))
		soap_serialize__timg__MoveResponse(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_timg__MoveResponse(struct soap *soap, const char *tag, int id, struct _timg__MoveResponse *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__timg__MoveResponse);
	if (id < 0)
		return soap->error;
	return soap_out__timg__MoveResponse(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _timg__MoveResponse ** SOAP_FMAC4 soap_in_PointerTo_timg__MoveResponse(struct soap *soap, const char *tag, struct _timg__MoveResponse **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _timg__MoveResponse **)soap_malloc(soap, sizeof(struct _timg__MoveResponse *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in__timg__MoveResponse(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct _timg__MoveResponse **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__timg__MoveResponse, sizeof(struct _timg__MoveResponse), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_timg__MoveResponse(struct soap *soap, struct _timg__MoveResponse *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_timg__MoveResponse);
	if (soap_out_PointerTo_timg__MoveResponse(soap, tag?tag:"timg:MoveResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _timg__MoveResponse ** SOAP_FMAC4 soap_get_PointerTo_timg__MoveResponse(struct soap *soap, struct _timg__MoveResponse **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_timg__MoveResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_timg__Move(struct soap *soap, struct _timg__Move *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__timg__Move))
		soap_serialize__timg__Move(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_timg__Move(struct soap *soap, const char *tag, int id, struct _timg__Move *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__timg__Move);
	if (id < 0)
		return soap->error;
	return soap_out__timg__Move(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _timg__Move ** SOAP_FMAC4 soap_in_PointerTo_timg__Move(struct soap *soap, const char *tag, struct _timg__Move **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _timg__Move **)soap_malloc(soap, sizeof(struct _timg__Move *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in__timg__Move(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct _timg__Move **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__timg__Move, sizeof(struct _timg__Move), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_timg__Move(struct soap *soap, struct _timg__Move *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_timg__Move);
	if (soap_out_PointerTo_timg__Move(soap, tag?tag:"timg:Move", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _timg__Move ** SOAP_FMAC4 soap_get_PointerTo_timg__Move(struct soap *soap, struct _timg__Move **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_timg__Move(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___timg__Stop(struct soap *soap, struct __timg__Stop *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->timg__Stop = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___timg__Stop(struct soap *soap, const struct __timg__Stop *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_timg__Stop(soap, &a->timg__Stop);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___timg__Stop(struct soap *soap, const char *tag, int id, const struct __timg__Stop *a, const char *type)
{
	if (soap_out_PointerTo_timg__Stop(soap, "timg:Stop", -1, &a->timg__Stop, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __timg__Stop * SOAP_FMAC4 soap_in___timg__Stop(struct soap *soap, const char *tag, struct __timg__Stop *a, const char *type)
{
	size_t soap_flag_timg__Stop = 1;
	short soap_flag;
	a = (struct __timg__Stop *)soap_id_enter(soap, "", a, SOAP_TYPE___timg__Stop, sizeof(struct __timg__Stop), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___timg__Stop(soap, a);
		for (soap_flag = 0;; soap_flag = 1)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_timg__Stop && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTo_timg__Stop(soap, "timg:Stop", &a->timg__Stop, ""))
				{	soap_flag_timg__Stop--;
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

SOAP_FMAC3 int SOAP_FMAC4 soap_put___timg__Stop(struct soap *soap, const struct __timg__Stop *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___timg__Stop(soap, tag?tag:"-timg:Stop", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __timg__Stop * SOAP_FMAC4 soap_get___timg__Stop(struct soap *soap, struct __timg__Stop *p, const char *tag, const char *type)
{
	if ((p = soap_in___timg__Stop(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__timg__StopResponse(struct soap *soap, struct _timg__StopResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__timg__StopResponse(struct soap *soap, const struct _timg__StopResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__timg__StopResponse(struct soap *soap, const char *tag, int id, const struct _timg__StopResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__timg__StopResponse), type))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _timg__StopResponse * SOAP_FMAC4 soap_in__timg__StopResponse(struct soap *soap, const char *tag, struct _timg__StopResponse *a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _timg__StopResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__timg__StopResponse, sizeof(struct _timg__StopResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__timg__StopResponse(soap, a);
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
	{	a = (struct _timg__StopResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__timg__StopResponse, 0, sizeof(struct _timg__StopResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__timg__StopResponse(struct soap *soap, const struct _timg__StopResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__timg__StopResponse);
	if (soap_out__timg__StopResponse(soap, tag?tag:"timg:StopResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _timg__StopResponse * SOAP_FMAC4 soap_get__timg__StopResponse(struct soap *soap, struct _timg__StopResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__timg__StopResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__timg__Stop(struct soap *soap, struct _timg__Stop *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->VideoSourceToken);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__timg__Stop(struct soap *soap, const struct _timg__Stop *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->VideoSourceToken);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__timg__Stop(struct soap *soap, const char *tag, int id, const struct _timg__Stop *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__timg__Stop), type))
		return soap->error;
	if (a->VideoSourceToken)
	{	if (soap_out_tt__ReferenceToken(soap, "timg:VideoSourceToken", -1, &a->VideoSourceToken, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "timg:VideoSourceToken"))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _timg__Stop * SOAP_FMAC4 soap_in__timg__Stop(struct soap *soap, const char *tag, struct _timg__Stop *a, const char *type)
{
	size_t soap_flag_VideoSourceToken = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _timg__Stop *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__timg__Stop, sizeof(struct _timg__Stop), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__timg__Stop(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_VideoSourceToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_tt__ReferenceToken(soap, "timg:VideoSourceToken", &a->VideoSourceToken, "tt:ReferenceToken"))
				{	soap_flag_VideoSourceToken--;
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
	{	a = (struct _timg__Stop *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__timg__Stop, 0, sizeof(struct _timg__Stop), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_VideoSourceToken > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__timg__Stop(struct soap *soap, const struct _timg__Stop *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__timg__Stop);
	if (soap_out__timg__Stop(soap, tag?tag:"timg:Stop", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _timg__Stop * SOAP_FMAC4 soap_get__timg__Stop(struct soap *soap, struct _timg__Stop *p, const char *tag, const char *type)
{
	if ((p = soap_in__timg__Stop(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_timg__StopResponse(struct soap *soap, struct _timg__StopResponse *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__timg__StopResponse))
		soap_serialize__timg__StopResponse(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_timg__StopResponse(struct soap *soap, const char *tag, int id, struct _timg__StopResponse *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__timg__StopResponse);
	if (id < 0)
		return soap->error;
	return soap_out__timg__StopResponse(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _timg__StopResponse ** SOAP_FMAC4 soap_in_PointerTo_timg__StopResponse(struct soap *soap, const char *tag, struct _timg__StopResponse **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _timg__StopResponse **)soap_malloc(soap, sizeof(struct _timg__StopResponse *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in__timg__StopResponse(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct _timg__StopResponse **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__timg__StopResponse, sizeof(struct _timg__StopResponse), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_timg__StopResponse(struct soap *soap, struct _timg__StopResponse *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_timg__StopResponse);
	if (soap_out_PointerTo_timg__StopResponse(soap, tag?tag:"timg:StopResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _timg__StopResponse ** SOAP_FMAC4 soap_get_PointerTo_timg__StopResponse(struct soap *soap, struct _timg__StopResponse **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_timg__StopResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_timg__Stop(struct soap *soap, struct _timg__Stop *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__timg__Stop))
		soap_serialize__timg__Stop(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_timg__Stop(struct soap *soap, const char *tag, int id, struct _timg__Stop *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__timg__Stop);
	if (id < 0)
		return soap->error;
	return soap_out__timg__Stop(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _timg__Stop ** SOAP_FMAC4 soap_in_PointerTo_timg__Stop(struct soap *soap, const char *tag, struct _timg__Stop **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _timg__Stop **)soap_malloc(soap, sizeof(struct _timg__Stop *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in__timg__Stop(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct _timg__Stop **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__timg__Stop, sizeof(struct _timg__Stop), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_timg__Stop(struct soap *soap, struct _timg__Stop *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_timg__Stop);
	if (soap_out_PointerTo_timg__Stop(soap, tag?tag:"timg:Stop", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _timg__Stop ** SOAP_FMAC4 soap_get_PointerTo_timg__Stop(struct soap *soap, struct _timg__Stop **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_timg__Stop(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default___timg__GetMoveOptions(struct soap *soap, struct __timg__GetMoveOptions *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->timg__GetMoveOptions = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___timg__GetMoveOptions(struct soap *soap, const struct __timg__GetMoveOptions *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_timg__GetMoveOptions(soap, &a->timg__GetMoveOptions);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___timg__GetMoveOptions(struct soap *soap, const char *tag, int id, const struct __timg__GetMoveOptions *a, const char *type)
{
	if (soap_out_PointerTo_timg__GetMoveOptions(soap, "timg:GetMoveOptions", -1, &a->timg__GetMoveOptions, ""))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __timg__GetMoveOptions * SOAP_FMAC4 soap_in___timg__GetMoveOptions(struct soap *soap, const char *tag, struct __timg__GetMoveOptions *a, const char *type)
{
	size_t soap_flag_timg__GetMoveOptions = 1;
	short soap_flag;
	a = (struct __timg__GetMoveOptions *)soap_id_enter(soap, "", a, SOAP_TYPE___timg__GetMoveOptions, sizeof(struct __timg__GetMoveOptions), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default___timg__GetMoveOptions(soap, a);
		for (soap_flag = 0;; soap_flag = 1)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_timg__GetMoveOptions && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTo_timg__GetMoveOptions(soap, "timg:GetMoveOptions", &a->timg__GetMoveOptions, ""))
				{	soap_flag_timg__GetMoveOptions--;
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

SOAP_FMAC3 int SOAP_FMAC4 soap_put___timg__GetMoveOptions(struct soap *soap, const struct __timg__GetMoveOptions *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___timg__GetMoveOptions(soap, tag?tag:"-timg:GetMoveOptions", id, a, type))
		return soap->error;
	return SOAP_OK;
}

SOAP_FMAC3 struct __timg__GetMoveOptions * SOAP_FMAC4 soap_get___timg__GetMoveOptions(struct soap *soap, struct __timg__GetMoveOptions *p, const char *tag, const char *type)
{
	if ((p = soap_in___timg__GetMoveOptions(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_timg__GetMoveOptionsResponse(struct soap *soap, struct _timg__GetMoveOptionsResponse *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__timg__GetMoveOptionsResponse))
		soap_serialize__timg__GetMoveOptionsResponse(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_timg__GetMoveOptionsResponse(struct soap *soap, const char *tag, int id, struct _timg__GetMoveOptionsResponse *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__timg__GetMoveOptionsResponse);
	if (id < 0)
		return soap->error;
	return soap_out__timg__GetMoveOptionsResponse(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _timg__GetMoveOptionsResponse ** SOAP_FMAC4 soap_in_PointerTo_timg__GetMoveOptionsResponse(struct soap *soap, const char *tag, struct _timg__GetMoveOptionsResponse **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _timg__GetMoveOptionsResponse **)soap_malloc(soap, sizeof(struct _timg__GetMoveOptionsResponse *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in__timg__GetMoveOptionsResponse(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct _timg__GetMoveOptionsResponse **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__timg__GetMoveOptionsResponse, sizeof(struct _timg__GetMoveOptionsResponse), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_timg__GetMoveOptionsResponse(struct soap *soap, struct _timg__GetMoveOptionsResponse *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_timg__GetMoveOptionsResponse);
	if (soap_out_PointerTo_timg__GetMoveOptionsResponse(soap, tag?tag:"timg:GetMoveOptionsResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _timg__GetMoveOptionsResponse ** SOAP_FMAC4 soap_get_PointerTo_timg__GetMoveOptionsResponse(struct soap *soap, struct _timg__GetMoveOptionsResponse **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_timg__GetMoveOptionsResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_timg__GetMoveOptions(struct soap *soap, struct _timg__GetMoveOptions *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__timg__GetMoveOptions))
		soap_serialize__timg__GetMoveOptions(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_timg__GetMoveOptions(struct soap *soap, const char *tag, int id, struct _timg__GetMoveOptions *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__timg__GetMoveOptions);
	if (id < 0)
		return soap->error;
	return soap_out__timg__GetMoveOptions(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _timg__GetMoveOptions ** SOAP_FMAC4 soap_in_PointerTo_timg__GetMoveOptions(struct soap *soap, const char *tag, struct _timg__GetMoveOptions **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct _timg__GetMoveOptions **)soap_malloc(soap, sizeof(struct _timg__GetMoveOptions *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in__timg__GetMoveOptions(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct _timg__GetMoveOptions **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__timg__GetMoveOptions, sizeof(struct _timg__GetMoveOptions), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_timg__GetMoveOptions(struct soap *soap, struct _timg__GetMoveOptions *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_timg__GetMoveOptions);
	if (soap_out_PointerTo_timg__GetMoveOptions(soap, tag?tag:"timg:GetMoveOptions", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _timg__GetMoveOptions ** SOAP_FMAC4 soap_get_PointerTo_timg__GetMoveOptions(struct soap *soap, struct _timg__GetMoveOptions **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_timg__GetMoveOptions(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__FocusMove(struct soap *soap, struct tt__FocusMove *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__FocusMove))
		soap_serialize_tt__FocusMove(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__FocusMove(struct soap *soap, const char *tag, int id, struct tt__FocusMove *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__FocusMove);
	if (id < 0)
		return soap->error;
	return soap_out_tt__FocusMove(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__FocusMove ** SOAP_FMAC4 soap_in_PointerTott__FocusMove(struct soap *soap, const char *tag, struct tt__FocusMove **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__FocusMove **)soap_malloc(soap, sizeof(struct tt__FocusMove *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_tt__FocusMove(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct tt__FocusMove **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__FocusMove, sizeof(struct tt__FocusMove), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__FocusMove(struct soap *soap, struct tt__FocusMove *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__FocusMove);
	if (soap_out_PointerTott__FocusMove(soap, tag?tag:"tt:FocusMove", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__FocusMove ** SOAP_FMAC4 soap_get_PointerTott__FocusMove(struct soap *soap, struct tt__FocusMove **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__FocusMove(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__timg__Move(struct soap *soap, struct _timg__Move *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->VideoSourceToken);
	a->Focus = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__timg__Move(struct soap *soap, const struct _timg__Move *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->VideoSourceToken);
	soap_serialize_PointerTott__FocusMove(soap, &a->Focus);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__timg__Move(struct soap *soap, const char *tag, int id, const struct _timg__Move *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__timg__Move), type))
		return soap->error;
	if (a->VideoSourceToken)
	{	if (soap_out_tt__ReferenceToken(soap, "timg:VideoSourceToken", -1, &a->VideoSourceToken, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "timg:VideoSourceToken"))
		return soap->error;
	if (a->Focus)
	{	if (soap_out_PointerTott__FocusMove(soap, "timg:Focus", -1, &a->Focus, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "timg:Focus"))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _timg__Move * SOAP_FMAC4 soap_in__timg__Move(struct soap *soap, const char *tag, struct _timg__Move *a, const char *type)
{
	size_t soap_flag_VideoSourceToken = 1;
	size_t soap_flag_Focus = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _timg__Move *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__timg__Move, sizeof(struct _timg__Move), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__timg__Move(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_VideoSourceToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_tt__ReferenceToken(soap, "timg:VideoSourceToken", &a->VideoSourceToken, "tt:ReferenceToken"))
				{	soap_flag_VideoSourceToken--;
					continue;
				}
			if (soap_flag_Focus && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FocusMove(soap, "timg:Focus", &a->Focus, "tt:FocusMove"))
				{	soap_flag_Focus--;
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
	{	a = (struct _timg__Move *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__timg__Move, 0, sizeof(struct _timg__Move), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_VideoSourceToken > 0 || soap_flag_Focus > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__timg__Move(struct soap *soap, const struct _timg__Move *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__timg__Move);
	if (soap_out__timg__Move(soap, tag?tag:"timg:Move", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _timg__Move * SOAP_FMAC4 soap_get__timg__Move(struct soap *soap, struct _timg__Move *p, const char *tag, const char *type)
{
	if ((p = soap_in__timg__Move(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__timg__GetMoveOptionsResponse(struct soap *soap, struct _timg__GetMoveOptionsResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->MoveOptions = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__timg__GetMoveOptionsResponse(struct soap *soap, const struct _timg__GetMoveOptionsResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTott__MoveOptions20(soap, &a->MoveOptions);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__timg__GetMoveOptionsResponse(struct soap *soap, const char *tag, int id, const struct _timg__GetMoveOptionsResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__timg__GetMoveOptionsResponse), type))
		return soap->error;
	if (a->MoveOptions)
		soap_element_result(soap, "timg:MoveOptions");
	if (a->MoveOptions)
	{	if (soap_out_PointerTott__MoveOptions20(soap, "timg:MoveOptions", -1, &a->MoveOptions, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "timg:MoveOptions"))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _timg__GetMoveOptionsResponse * SOAP_FMAC4 soap_in__timg__GetMoveOptionsResponse(struct soap *soap, const char *tag, struct _timg__GetMoveOptionsResponse *a, const char *type)
{
	
	int iii=0;
	size_t soap_flag_MoveOptions = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _timg__GetMoveOptionsResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__timg__GetMoveOptionsResponse, sizeof(struct _timg__GetMoveOptionsResponse), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__timg__GetMoveOptionsResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_MoveOptions && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__MoveOptions20(soap, "timg:MoveOptions", &a->MoveOptions, "tt:MoveOptions20"))
				{	soap_flag_MoveOptions--;
					continue;
				}
			soap_check_result(soap, "timg:MoveOptions");
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
	{	a = (struct _timg__GetMoveOptionsResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__timg__GetMoveOptionsResponse, 0, sizeof(struct _timg__GetMoveOptionsResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_MoveOptions > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__timg__GetMoveOptionsResponse(struct soap *soap, const struct _timg__GetMoveOptionsResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__timg__GetMoveOptionsResponse);
	if (soap_out__timg__GetMoveOptionsResponse(soap, tag?tag:"timg:GetMoveOptionsResponse", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _timg__GetMoveOptionsResponse * SOAP_FMAC4 soap_get__timg__GetMoveOptionsResponse(struct soap *soap, struct _timg__GetMoveOptionsResponse *p, const char *tag, const char *type)
{
	
	int i= 0;
		int  j = 0;
	
	if ((p = soap_in__timg__GetMoveOptionsResponse(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__timg__GetMoveOptions(struct soap *soap, struct _timg__GetMoveOptions *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->VideoSourceToken);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__timg__GetMoveOptions(struct soap *soap, const struct _timg__GetMoveOptions *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->VideoSourceToken);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__timg__GetMoveOptions(struct soap *soap, const char *tag, int id, const struct _timg__GetMoveOptions *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__timg__GetMoveOptions), type))
		return soap->error;
	if (a->VideoSourceToken)
	{	if (soap_out_tt__ReferenceToken(soap, "timg:VideoSourceToken", -1, &a->VideoSourceToken, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "timg:VideoSourceToken"))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _timg__GetMoveOptions * SOAP_FMAC4 soap_in__timg__GetMoveOptions(struct soap *soap, const char *tag, struct _timg__GetMoveOptions *a, const char *type)
{
	size_t soap_flag_VideoSourceToken = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct _timg__GetMoveOptions *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__timg__GetMoveOptions, sizeof(struct _timg__GetMoveOptions), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default__timg__GetMoveOptions(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_VideoSourceToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_tt__ReferenceToken(soap, "timg:VideoSourceToken", &a->VideoSourceToken, "tt:ReferenceToken"))
				{	soap_flag_VideoSourceToken--;
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
	{	a = (struct _timg__GetMoveOptions *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__timg__GetMoveOptions, 0, sizeof(struct _timg__GetMoveOptions), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_VideoSourceToken > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__timg__GetMoveOptions(struct soap *soap, const struct _timg__GetMoveOptions *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__timg__GetMoveOptions);
	if (soap_out__timg__GetMoveOptions(soap, tag?tag:"timg:GetMoveOptions", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _timg__GetMoveOptions * SOAP_FMAC4 soap_get__timg__GetMoveOptions(struct soap *soap, struct _timg__GetMoveOptions *p, const char *tag, const char *type)
{
	if ((p = soap_in__timg__GetMoveOptions(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__FocusMove(struct soap *soap, struct tt__FocusMove *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->Absolute = NULL;
	a->Relative = NULL;
	a->Continuous = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__FocusMove(struct soap *soap, const struct tt__FocusMove *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTott__AbsoluteFocus(soap, &a->Absolute);
	soap_serialize_PointerTott__RelativeFocus(soap, &a->Relative);
	soap_serialize_PointerTott__ContinuousFocus(soap, &a->Continuous);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__FocusMove(struct soap *soap, const char *tag, int id, const struct tt__FocusMove *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__FocusMove), type))
		return soap->error;
	if (soap_out_PointerTott__AbsoluteFocus(soap, "tt:Absolute", -1, &a->Absolute, ""))
		return soap->error;
	if (soap_out_PointerTott__RelativeFocus(soap, "tt:Relative", -1, &a->Relative, ""))
		return soap->error;
	if (soap_out_PointerTott__ContinuousFocus(soap, "tt:Continuous", -1, &a->Continuous, ""))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__FocusMove * SOAP_FMAC4 soap_in_tt__FocusMove(struct soap *soap, const char *tag, struct tt__FocusMove *a, const char *type)
{
	size_t soap_flag_Absolute = 1;
	size_t soap_flag_Relative = 1;
	size_t soap_flag_Continuous = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__FocusMove *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__FocusMove, sizeof(struct tt__FocusMove), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__FocusMove(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_Absolute && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__AbsoluteFocus(soap, "tt:Absolute", &a->Absolute, "tt:AbsoluteFocus"))
				{	soap_flag_Absolute--;
					continue;
				}
			if (soap_flag_Relative && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__RelativeFocus(soap, "tt:Relative", &a->Relative, "tt:RelativeFocus"))
				{	soap_flag_Relative--;
					continue;
				}
			if (soap_flag_Continuous && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__ContinuousFocus(soap, "tt:Continuous", &a->Continuous, "tt:ContinuousFocus"))
				{	soap_flag_Continuous--;
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
	{	a = (struct tt__FocusMove *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__FocusMove, 0, sizeof(struct tt__FocusMove), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__FocusMove(struct soap *soap, const struct tt__FocusMove *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__FocusMove);
	if (soap_out_tt__FocusMove(soap, tag?tag:"tt:FocusMove", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__FocusMove * SOAP_FMAC4 soap_get_tt__FocusMove(struct soap *soap, struct tt__FocusMove *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__FocusMove(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__MoveOptions20(struct soap *soap, struct tt__MoveOptions20 *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__MoveOptions20))
		soap_serialize_tt__MoveOptions20(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__MoveOptions20(struct soap *soap, const char *tag, int id, struct tt__MoveOptions20 *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__MoveOptions20);
	if (id < 0)
		return soap->error;
	return soap_out_tt__MoveOptions20(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__MoveOptions20 ** SOAP_FMAC4 soap_in_PointerTott__MoveOptions20(struct soap *soap, const char *tag, struct tt__MoveOptions20 **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__MoveOptions20 **)soap_malloc(soap, sizeof(struct tt__MoveOptions20 *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_tt__MoveOptions20(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct tt__MoveOptions20 **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__MoveOptions20, sizeof(struct tt__MoveOptions20), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__MoveOptions20(struct soap *soap, struct tt__MoveOptions20 *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__MoveOptions20);
	if (soap_out_PointerTott__MoveOptions20(soap, tag?tag:"tt:MoveOptions20", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__MoveOptions20 ** SOAP_FMAC4 soap_get_PointerTott__MoveOptions20(struct soap *soap, struct tt__MoveOptions20 **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__MoveOptions20(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__ContinuousFocus(struct soap *soap, struct tt__ContinuousFocus *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__ContinuousFocus))
		soap_serialize_tt__ContinuousFocus(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__ContinuousFocus(struct soap *soap, const char *tag, int id, struct tt__ContinuousFocus *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__ContinuousFocus);
	if (id < 0)
		return soap->error;
	return soap_out_tt__ContinuousFocus(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__ContinuousFocus ** SOAP_FMAC4 soap_in_PointerTott__ContinuousFocus(struct soap *soap, const char *tag, struct tt__ContinuousFocus **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__ContinuousFocus **)soap_malloc(soap, sizeof(struct tt__ContinuousFocus *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_tt__ContinuousFocus(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct tt__ContinuousFocus **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__ContinuousFocus, sizeof(struct tt__ContinuousFocus), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__ContinuousFocus(struct soap *soap, struct tt__ContinuousFocus *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__ContinuousFocus);
	if (soap_out_PointerTott__ContinuousFocus(soap, tag?tag:"tt:ContinuousFocus", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__ContinuousFocus ** SOAP_FMAC4 soap_get_PointerTott__ContinuousFocus(struct soap *soap, struct tt__ContinuousFocus **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__ContinuousFocus(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__RelativeFocus(struct soap *soap, struct tt__RelativeFocus *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__RelativeFocus))
		soap_serialize_tt__RelativeFocus(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__RelativeFocus(struct soap *soap, const char *tag, int id, struct tt__RelativeFocus *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__RelativeFocus);
	if (id < 0)
		return soap->error;
	return soap_out_tt__RelativeFocus(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__RelativeFocus ** SOAP_FMAC4 soap_in_PointerTott__RelativeFocus(struct soap *soap, const char *tag, struct tt__RelativeFocus **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__RelativeFocus **)soap_malloc(soap, sizeof(struct tt__RelativeFocus *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_tt__RelativeFocus(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct tt__RelativeFocus **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__RelativeFocus, sizeof(struct tt__RelativeFocus), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__RelativeFocus(struct soap *soap, struct tt__RelativeFocus *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__RelativeFocus);
	if (soap_out_PointerTott__RelativeFocus(soap, tag?tag:"tt:RelativeFocus", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__RelativeFocus ** SOAP_FMAC4 soap_get_PointerTott__RelativeFocus(struct soap *soap, struct tt__RelativeFocus **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__RelativeFocus(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__AbsoluteFocus(struct soap *soap, struct tt__AbsoluteFocus *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__AbsoluteFocus))
		soap_serialize_tt__AbsoluteFocus(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__AbsoluteFocus(struct soap *soap, const char *tag, int id, struct tt__AbsoluteFocus *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__AbsoluteFocus);
	if (id < 0)
		return soap->error;
	return soap_out_tt__AbsoluteFocus(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__AbsoluteFocus ** SOAP_FMAC4 soap_in_PointerTott__AbsoluteFocus(struct soap *soap, const char *tag, struct tt__AbsoluteFocus **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__AbsoluteFocus **)soap_malloc(soap, sizeof(struct tt__AbsoluteFocus *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_tt__AbsoluteFocus(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct tt__AbsoluteFocus **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__AbsoluteFocus, sizeof(struct tt__AbsoluteFocus), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__AbsoluteFocus(struct soap *soap, struct tt__AbsoluteFocus *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__AbsoluteFocus);
	if (soap_out_PointerTott__AbsoluteFocus(soap, tag?tag:"tt:AbsoluteFocus", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__AbsoluteFocus ** SOAP_FMAC4 soap_get_PointerTott__AbsoluteFocus(struct soap *soap, struct tt__AbsoluteFocus **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__AbsoluteFocus(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__MoveOptions20(struct soap *soap, struct tt__MoveOptions20 *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->Absolute = NULL;
	a->Relative = NULL;
	a->Continuous = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__MoveOptions20(struct soap *soap, const struct tt__MoveOptions20 *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTott__AbsoluteFocusOptions(soap, &a->Absolute);
	soap_serialize_PointerTott__RelativeFocusOptions20(soap, &a->Relative);
	soap_serialize_PointerTott__ContinuousFocusOptions(soap, &a->Continuous);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__MoveOptions20(struct soap *soap, const char *tag, int id, const struct tt__MoveOptions20 *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__MoveOptions20), type))
		return soap->error;
	if (soap_out_PointerTott__AbsoluteFocusOptions(soap, "tt:Absolute", -1, &a->Absolute, ""))
		return soap->error;
	if (soap_out_PointerTott__RelativeFocusOptions20(soap, "tt:Relative", -1, &a->Relative, ""))
		return soap->error;
	if (soap_out_PointerTott__ContinuousFocusOptions(soap, "tt:Continuous", -1, &a->Continuous, ""))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__MoveOptions20 * SOAP_FMAC4 soap_in_tt__MoveOptions20(struct soap *soap, const char *tag, struct tt__MoveOptions20 *a, const char *type)
{
	size_t soap_flag_Absolute = 1;
	size_t soap_flag_Relative = 1;
	size_t soap_flag_Continuous = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__MoveOptions20 *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__MoveOptions20, sizeof(struct tt__MoveOptions20), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__MoveOptions20(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_Absolute && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__AbsoluteFocusOptions(soap, "tt:Absolute", &a->Absolute, "tt:AbsoluteFocusOptions"))
				{	soap_flag_Absolute--;
					continue;
				}
			if (soap_flag_Relative && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__RelativeFocusOptions20(soap, "tt:Relative", &a->Relative, "tt:RelativeFocusOptions20"))
				{	soap_flag_Relative--;
					continue;
				}
			if (soap_flag_Continuous && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__ContinuousFocusOptions(soap, "tt:Continuous", &a->Continuous, "tt:ContinuousFocusOptions"))
				{	soap_flag_Continuous--;
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
	{	a = (struct tt__MoveOptions20 *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__MoveOptions20, 0, sizeof(struct tt__MoveOptions20), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__MoveOptions20(struct soap *soap, const struct tt__MoveOptions20 *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__MoveOptions20);
	if (soap_out_tt__MoveOptions20(soap, tag?tag:"tt:MoveOptions20", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__MoveOptions20 * SOAP_FMAC4 soap_get_tt__MoveOptions20(struct soap *soap, struct tt__MoveOptions20 *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__MoveOptions20(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__ContinuousFocus(struct soap *soap, struct tt__ContinuousFocus *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_float(soap, &a->Speed);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__ContinuousFocus(struct soap *soap, const struct tt__ContinuousFocus *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_embedded(soap, &a->Speed, SOAP_TYPE_float);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__ContinuousFocus(struct soap *soap, const char *tag, int id, const struct tt__ContinuousFocus *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__ContinuousFocus), type))
		return soap->error;
	if (soap_out_float(soap, "tt:Speed", -1, &a->Speed, ""))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__ContinuousFocus * SOAP_FMAC4 soap_in_tt__ContinuousFocus(struct soap *soap, const char *tag, struct tt__ContinuousFocus *a, const char *type)
{
	size_t soap_flag_Speed = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__ContinuousFocus *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__ContinuousFocus, sizeof(struct tt__ContinuousFocus), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__ContinuousFocus(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_Speed && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_float(soap, "tt:Speed", &a->Speed, "xsd:float"))
				{	soap_flag_Speed--;
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
	{	a = (struct tt__ContinuousFocus *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__ContinuousFocus, 0, sizeof(struct tt__ContinuousFocus), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_Speed > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__ContinuousFocus(struct soap *soap, const struct tt__ContinuousFocus *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__ContinuousFocus);
	if (soap_out_tt__ContinuousFocus(soap, tag?tag:"tt:ContinuousFocus", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__ContinuousFocus * SOAP_FMAC4 soap_get_tt__ContinuousFocus(struct soap *soap, struct tt__ContinuousFocus *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__ContinuousFocus(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__RelativeFocus(struct soap *soap, struct tt__RelativeFocus *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_float(soap, &a->Distance);
	a->Speed = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__RelativeFocus(struct soap *soap, const struct tt__RelativeFocus *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_embedded(soap, &a->Distance, SOAP_TYPE_float);
	soap_serialize_PointerTofloat(soap, &a->Speed);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__RelativeFocus(struct soap *soap, const char *tag, int id, const struct tt__RelativeFocus *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__RelativeFocus), type))
		return soap->error;
	if (soap_out_float(soap, "tt:Distance", -1, &a->Distance, ""))
		return soap->error;
	if (soap_out_PointerTofloat(soap, "tt:Speed", -1, &a->Speed, ""))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__RelativeFocus * SOAP_FMAC4 soap_in_tt__RelativeFocus(struct soap *soap, const char *tag, struct tt__RelativeFocus *a, const char *type)
{
	size_t soap_flag_Distance = 1;
	size_t soap_flag_Speed = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__RelativeFocus *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__RelativeFocus, sizeof(struct tt__RelativeFocus), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__RelativeFocus(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_Distance && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_float(soap, "tt:Distance", &a->Distance, "xsd:float"))
				{	soap_flag_Distance--;
					continue;
				}
			if (soap_flag_Speed && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTofloat(soap, "tt:Speed", &a->Speed, "xsd:float"))
				{	soap_flag_Speed--;
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
	{	a = (struct tt__RelativeFocus *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__RelativeFocus, 0, sizeof(struct tt__RelativeFocus), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_Distance > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__RelativeFocus(struct soap *soap, const struct tt__RelativeFocus *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__RelativeFocus);
	if (soap_out_tt__RelativeFocus(soap, tag?tag:"tt:RelativeFocus", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__RelativeFocus * SOAP_FMAC4 soap_get_tt__RelativeFocus(struct soap *soap, struct tt__RelativeFocus *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__RelativeFocus(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__AbsoluteFocus(struct soap *soap, struct tt__AbsoluteFocus *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_float(soap, &a->Position);
	a->Speed = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__AbsoluteFocus(struct soap *soap, const struct tt__AbsoluteFocus *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_embedded(soap, &a->Position, SOAP_TYPE_float);
	soap_serialize_PointerTofloat(soap, &a->Speed);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__AbsoluteFocus(struct soap *soap, const char *tag, int id, const struct tt__AbsoluteFocus *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__AbsoluteFocus), type))
		return soap->error;
	if (soap_out_float(soap, "tt:Position", -1, &a->Position, ""))
		return soap->error;
	if (soap_out_PointerTofloat(soap, "tt:Speed", -1, &a->Speed, ""))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__AbsoluteFocus * SOAP_FMAC4 soap_in_tt__AbsoluteFocus(struct soap *soap, const char *tag, struct tt__AbsoluteFocus *a, const char *type)
{
	size_t soap_flag_Position = 1;
	size_t soap_flag_Speed = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__AbsoluteFocus *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__AbsoluteFocus, sizeof(struct tt__AbsoluteFocus), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__AbsoluteFocus(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_Position && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_float(soap, "tt:Position", &a->Position, "xsd:float"))
				{	soap_flag_Position--;
					continue;
				}
			if (soap_flag_Speed && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTofloat(soap, "tt:Speed", &a->Speed, "xsd:float"))
				{	soap_flag_Speed--;
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
	{	a = (struct tt__AbsoluteFocus *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__AbsoluteFocus, 0, sizeof(struct tt__AbsoluteFocus), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_Position > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__AbsoluteFocus(struct soap *soap, const struct tt__AbsoluteFocus *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__AbsoluteFocus);
	if (soap_out_tt__AbsoluteFocus(soap, tag?tag:"tt:AbsoluteFocus", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__AbsoluteFocus * SOAP_FMAC4 soap_get_tt__AbsoluteFocus(struct soap *soap, struct tt__AbsoluteFocus *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__AbsoluteFocus(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__ContinuousFocusOptions(struct soap *soap, struct tt__ContinuousFocusOptions *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__ContinuousFocusOptions))
		soap_serialize_tt__ContinuousFocusOptions(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__ContinuousFocusOptions(struct soap *soap, const char *tag, int id, struct tt__ContinuousFocusOptions *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__ContinuousFocusOptions);
	if (id < 0)
		return soap->error;
	return soap_out_tt__ContinuousFocusOptions(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__ContinuousFocusOptions ** SOAP_FMAC4 soap_in_PointerTott__ContinuousFocusOptions(struct soap *soap, const char *tag, struct tt__ContinuousFocusOptions **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__ContinuousFocusOptions **)soap_malloc(soap, sizeof(struct tt__ContinuousFocusOptions *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_tt__ContinuousFocusOptions(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct tt__ContinuousFocusOptions **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__ContinuousFocusOptions, sizeof(struct tt__ContinuousFocusOptions), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__ContinuousFocusOptions(struct soap *soap, struct tt__ContinuousFocusOptions *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__ContinuousFocusOptions);
	if (soap_out_PointerTott__ContinuousFocusOptions(soap, tag?tag:"tt:ContinuousFocusOptions", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__ContinuousFocusOptions ** SOAP_FMAC4 soap_get_PointerTott__ContinuousFocusOptions(struct soap *soap, struct tt__ContinuousFocusOptions **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__ContinuousFocusOptions(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__RelativeFocusOptions(struct soap *soap, struct tt__RelativeFocusOptions *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__RelativeFocusOptions))
		soap_serialize_tt__RelativeFocusOptions(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__RelativeFocusOptions(struct soap *soap, const char *tag, int id, struct tt__RelativeFocusOptions *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__RelativeFocusOptions);
	if (id < 0)
		return soap->error;
	return soap_out_tt__RelativeFocusOptions(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__RelativeFocusOptions ** SOAP_FMAC4 soap_in_PointerTott__RelativeFocusOptions(struct soap *soap, const char *tag, struct tt__RelativeFocusOptions **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__RelativeFocusOptions **)soap_malloc(soap, sizeof(struct tt__RelativeFocusOptions *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_tt__RelativeFocusOptions(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct tt__RelativeFocusOptions **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__RelativeFocusOptions, sizeof(struct tt__RelativeFocusOptions), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__RelativeFocusOptions(struct soap *soap, struct tt__RelativeFocusOptions *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__RelativeFocusOptions);
	if (soap_out_PointerTott__RelativeFocusOptions(soap, tag?tag:"tt:RelativeFocusOptions", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__RelativeFocusOptions ** SOAP_FMAC4 soap_get_PointerTott__RelativeFocusOptions(struct soap *soap, struct tt__RelativeFocusOptions **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__RelativeFocusOptions(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__AbsoluteFocusOptions(struct soap *soap, struct tt__AbsoluteFocusOptions *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__AbsoluteFocusOptions))
		soap_serialize_tt__AbsoluteFocusOptions(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__AbsoluteFocusOptions(struct soap *soap, const char *tag, int id, struct tt__AbsoluteFocusOptions *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__AbsoluteFocusOptions);
	if (id < 0)
		return soap->error;
	return soap_out_tt__AbsoluteFocusOptions(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__AbsoluteFocusOptions ** SOAP_FMAC4 soap_in_PointerTott__AbsoluteFocusOptions(struct soap *soap, const char *tag, struct tt__AbsoluteFocusOptions **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__AbsoluteFocusOptions **)soap_malloc(soap, sizeof(struct tt__AbsoluteFocusOptions *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_tt__AbsoluteFocusOptions(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct tt__AbsoluteFocusOptions **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__AbsoluteFocusOptions, sizeof(struct tt__AbsoluteFocusOptions), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__AbsoluteFocusOptions(struct soap *soap, struct tt__AbsoluteFocusOptions *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__AbsoluteFocusOptions);
	if (soap_out_PointerTott__AbsoluteFocusOptions(soap, tag?tag:"tt:AbsoluteFocusOptions", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__AbsoluteFocusOptions ** SOAP_FMAC4 soap_get_PointerTott__AbsoluteFocusOptions(struct soap *soap, struct tt__AbsoluteFocusOptions **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__AbsoluteFocusOptions(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__RelativeFocusOptions20(struct soap *soap, struct tt__RelativeFocusOptions20 *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__RelativeFocusOptions20))
		soap_serialize_tt__RelativeFocusOptions20(soap, *a);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__RelativeFocusOptions20(struct soap *soap, const char *tag, int id, struct tt__RelativeFocusOptions20 *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__RelativeFocusOptions20);
	if (id < 0)
		return soap->error;
	return soap_out_tt__RelativeFocusOptions20(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__RelativeFocusOptions20 ** SOAP_FMAC4 soap_in_PointerTott__RelativeFocusOptions20(struct soap *soap, const char *tag, struct tt__RelativeFocusOptions20 **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
		return NULL;
	if (!a)
		if (!(a = (struct tt__RelativeFocusOptions20 **)soap_malloc(soap, sizeof(struct tt__RelativeFocusOptions20 *))))
			return NULL;
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	soap_revert(soap);
		if (!(*a = soap_in_tt__RelativeFocusOptions20(soap, tag, *a, type)))
			return NULL;
	}
	else
	{	a = (struct tt__RelativeFocusOptions20 **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__RelativeFocusOptions20, sizeof(struct tt__RelativeFocusOptions20), 0);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__RelativeFocusOptions20(struct soap *soap, struct tt__RelativeFocusOptions20 *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__RelativeFocusOptions20);
	if (soap_out_PointerTott__RelativeFocusOptions20(soap, tag?tag:"tt:RelativeFocusOptions20", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__RelativeFocusOptions20 ** SOAP_FMAC4 soap_get_PointerTott__RelativeFocusOptions20(struct soap *soap, struct tt__RelativeFocusOptions20 **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__RelativeFocusOptions20(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__ContinuousFocusOptions(struct soap *soap, struct tt__ContinuousFocusOptions *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->Speed = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__ContinuousFocusOptions(struct soap *soap, const struct tt__ContinuousFocusOptions *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTott__FloatRange(soap, &a->Speed);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__ContinuousFocusOptions(struct soap *soap, const char *tag, int id, const struct tt__ContinuousFocusOptions *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__ContinuousFocusOptions), type))
		return soap->error;
	if (a->Speed)
	{	if (soap_out_PointerTott__FloatRange(soap, "tt:Speed", -1, &a->Speed, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "tt:Speed"))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__ContinuousFocusOptions * SOAP_FMAC4 soap_in_tt__ContinuousFocusOptions(struct soap *soap, const char *tag, struct tt__ContinuousFocusOptions *a, const char *type)
{
	size_t soap_flag_Speed = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__ContinuousFocusOptions *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__ContinuousFocusOptions, sizeof(struct tt__ContinuousFocusOptions), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__ContinuousFocusOptions(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_Speed && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FloatRange(soap, "tt:Speed", &a->Speed, "tt:FloatRange"))
				{	soap_flag_Speed--;
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
	{	a = (struct tt__ContinuousFocusOptions *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__ContinuousFocusOptions, 0, sizeof(struct tt__ContinuousFocusOptions), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_Speed > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__ContinuousFocusOptions(struct soap *soap, const struct tt__ContinuousFocusOptions *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__ContinuousFocusOptions);
	if (soap_out_tt__ContinuousFocusOptions(soap, tag?tag:"tt:ContinuousFocusOptions", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__ContinuousFocusOptions * SOAP_FMAC4 soap_get_tt__ContinuousFocusOptions(struct soap *soap, struct tt__ContinuousFocusOptions *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__ContinuousFocusOptions(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__RelativeFocusOptions(struct soap *soap, struct tt__RelativeFocusOptions *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->Distance = NULL;
	a->Speed = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__RelativeFocusOptions(struct soap *soap, const struct tt__RelativeFocusOptions *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTott__FloatRange(soap, &a->Distance);
	soap_serialize_PointerTott__FloatRange(soap, &a->Speed);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__RelativeFocusOptions(struct soap *soap, const char *tag, int id, const struct tt__RelativeFocusOptions *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__RelativeFocusOptions), type))
		return soap->error;
	if (a->Distance)
	{	if (soap_out_PointerTott__FloatRange(soap, "tt:Distance", -1, &a->Distance, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "tt:Distance"))
		return soap->error;
	if (a->Speed)
	{	if (soap_out_PointerTott__FloatRange(soap, "tt:Speed", -1, &a->Speed, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "tt:Speed"))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__RelativeFocusOptions * SOAP_FMAC4 soap_in_tt__RelativeFocusOptions(struct soap *soap, const char *tag, struct tt__RelativeFocusOptions *a, const char *type)
{
	size_t soap_flag_Distance = 1;
	size_t soap_flag_Speed = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__RelativeFocusOptions *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__RelativeFocusOptions, sizeof(struct tt__RelativeFocusOptions), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__RelativeFocusOptions(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_Distance && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FloatRange(soap, "tt:Distance", &a->Distance, "tt:FloatRange"))
				{	soap_flag_Distance--;
					continue;
				}
			if (soap_flag_Speed && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FloatRange(soap, "tt:Speed", &a->Speed, "tt:FloatRange"))
				{	soap_flag_Speed--;
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
	{	a = (struct tt__RelativeFocusOptions *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__RelativeFocusOptions, 0, sizeof(struct tt__RelativeFocusOptions), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_Distance > 0 || soap_flag_Speed > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__RelativeFocusOptions(struct soap *soap, const struct tt__RelativeFocusOptions *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__RelativeFocusOptions);
	if (soap_out_tt__RelativeFocusOptions(soap, tag?tag:"tt:RelativeFocusOptions", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__RelativeFocusOptions * SOAP_FMAC4 soap_get_tt__RelativeFocusOptions(struct soap *soap, struct tt__RelativeFocusOptions *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__RelativeFocusOptions(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__AbsoluteFocusOptions(struct soap *soap, struct tt__AbsoluteFocusOptions *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->Position = NULL;
	a->Speed = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__AbsoluteFocusOptions(struct soap *soap, const struct tt__AbsoluteFocusOptions *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTott__FloatRange(soap, &a->Position);
	soap_serialize_PointerTott__FloatRange(soap, &a->Speed);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__AbsoluteFocusOptions(struct soap *soap, const char *tag, int id, const struct tt__AbsoluteFocusOptions *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__AbsoluteFocusOptions), type))
		return soap->error;
	if (a->Position)
	{	if (soap_out_PointerTott__FloatRange(soap, "tt:Position", -1, &a->Position, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "tt:Position"))
		return soap->error;
	if (soap_out_PointerTott__FloatRange(soap, "tt:Speed", -1, &a->Speed, ""))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__AbsoluteFocusOptions * SOAP_FMAC4 soap_in_tt__AbsoluteFocusOptions(struct soap *soap, const char *tag, struct tt__AbsoluteFocusOptions *a, const char *type)
{
	size_t soap_flag_Position = 1;
	size_t soap_flag_Speed = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__AbsoluteFocusOptions *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__AbsoluteFocusOptions, sizeof(struct tt__AbsoluteFocusOptions), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__AbsoluteFocusOptions(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_Position && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FloatRange(soap, "tt:Position", &a->Position, "tt:FloatRange"))
				{	soap_flag_Position--;
					continue;
				}
			if (soap_flag_Speed && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FloatRange(soap, "tt:Speed", &a->Speed, "tt:FloatRange"))
				{	soap_flag_Speed--;
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
	{	a = (struct tt__AbsoluteFocusOptions *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__AbsoluteFocusOptions, 0, sizeof(struct tt__AbsoluteFocusOptions), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_Position > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__AbsoluteFocusOptions(struct soap *soap, const struct tt__AbsoluteFocusOptions *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__AbsoluteFocusOptions);
	if (soap_out_tt__AbsoluteFocusOptions(soap, tag?tag:"tt:AbsoluteFocusOptions", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__AbsoluteFocusOptions * SOAP_FMAC4 soap_get_tt__AbsoluteFocusOptions(struct soap *soap, struct tt__AbsoluteFocusOptions *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__AbsoluteFocusOptions(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__RelativeFocusOptions20(struct soap *soap, struct tt__RelativeFocusOptions20 *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->Distance = NULL;
	a->Speed = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__RelativeFocusOptions20(struct soap *soap, const struct tt__RelativeFocusOptions20 *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTott__FloatRange(soap, &a->Distance);
	soap_serialize_PointerTott__FloatRange(soap, &a->Speed);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__RelativeFocusOptions20(struct soap *soap, const char *tag, int id, const struct tt__RelativeFocusOptions20 *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__RelativeFocusOptions20), type))
		return soap->error;
	if (a->Distance)
	{	if (soap_out_PointerTott__FloatRange(soap, "tt:Distance", -1, &a->Distance, ""))
			return soap->error;
	}
	else if (soap_element_nil(soap, "tt:Distance"))
		return soap->error;
	if (soap_out_PointerTott__FloatRange(soap, "tt:Speed", -1, &a->Speed, ""))
		return soap->error;
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__RelativeFocusOptions20 * SOAP_FMAC4 soap_in_tt__RelativeFocusOptions20(struct soap *soap, const char *tag, struct tt__RelativeFocusOptions20 *a, const char *type)
{
	size_t soap_flag_Distance = 1;
	size_t soap_flag_Speed = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
		return NULL;
	a = (struct tt__RelativeFocusOptions20 *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__RelativeFocusOptions20, sizeof(struct tt__RelativeFocusOptions20), 0, NULL, NULL, NULL);
	if (!a)
		return NULL;
	soap_default_tt__RelativeFocusOptions20(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_Distance && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FloatRange(soap, "tt:Distance", &a->Distance, "tt:FloatRange"))
				{	soap_flag_Distance--;
					continue;
				}
			if (soap_flag_Speed && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FloatRange(soap, "tt:Speed", &a->Speed, "tt:FloatRange"))
				{	soap_flag_Speed--;
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
	{	a = (struct tt__RelativeFocusOptions20 *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__RelativeFocusOptions20, 0, sizeof(struct tt__RelativeFocusOptions20), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
			return NULL;
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_Distance > 0))
	{	soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__RelativeFocusOptions20(struct soap *soap, const struct tt__RelativeFocusOptions20 *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__RelativeFocusOptions20);
	if (soap_out_tt__RelativeFocusOptions20(soap, tag?tag:"tt:RelativeFocusOptions20", id, a, type))
		return soap->error;
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__RelativeFocusOptions20 * SOAP_FMAC4 soap_get_tt__RelativeFocusOptions20(struct soap *soap, struct tt__RelativeFocusOptions20 *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__RelativeFocusOptions20(soap, tag, p, type)))
		if (soap_getindependent(soap))
			return NULL;
	return p;
}




#ifdef __cplusplus
}
#endif

#if defined(__BORLANDC__)
#pragma option pop
#pragma option pop
#endif

/* End of soapC.c */
