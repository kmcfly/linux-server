#if defined(__BORLANDC__)
#pragma option push -w-8060
#pragma option push -w-8004
#endif

#include "OnvifSoapH.h"

#ifdef __cplusplus
extern "C"
{
#endif

SOAP_FMAC3 void SOAP_FMAC4 soap_default___timg__GetOptions(struct soap *soap, struct __timg__GetOptions *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->timg__GetOptions = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___timg__GetOptions(struct soap *soap, const struct __timg__GetOptions *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTo_timg__GetOptions(soap, &a->timg__GetOptions);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out___timg__GetOptions(struct soap *soap, const char *tag, int id, const struct __timg__GetOptions *a, const char *type)
{
	if (soap_out_PointerTo_timg__GetOptions(soap, "timg:GetOptions", -1, &a->timg__GetOptions, ""))
	{
		return soap->error;
	}
	return SOAP_OK;
}

SOAP_FMAC3 struct __timg__GetOptions * SOAP_FMAC4 soap_in___timg__GetOptions(struct soap *soap, const char *tag, struct __timg__GetOptions *a, const char *type)
{
	size_t soap_flag_timg__GetOptions = 1;
	short soap_flag;
	a = (struct __timg__GetOptions *)soap_id_enter(soap, "", a, SOAP_TYPE___timg__GetOptions, sizeof(struct __timg__GetOptions), 0, NULL, NULL, NULL);
	if (NULL==a)
	{
		return NULL;
	}
	soap_default___timg__GetOptions(soap, a);
	for (soap_flag = 0;; soap_flag = 1)
	{	
		soap->error = SOAP_TAG_MISMATCH;
		if (soap_flag_timg__GetOptions && soap->error == SOAP_TAG_MISMATCH)
			if (soap_in_PointerTo_timg__GetOptions(soap, "timg:GetOptions", &a->timg__GetOptions, ""))
			{	
				soap_flag_timg__GetOptions--;
				continue;
			}
		if (soap->error == SOAP_TAG_MISMATCH)
			if (soap_flag)
			{
				soap->error = SOAP_OK;
				break;
			}
		if (soap_flag && soap->error == SOAP_NO_TAG)
		{
			break;
		}
		if (soap->error)
		{
			return NULL;
		}
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put___timg__GetOptions(struct soap *soap, const struct __timg__GetOptions *a, const char *tag, const char *type)
{
	register int id = 0;
	if (soap_out___timg__GetOptions(soap, tag?tag:"-timg:GetOptions", id, a, type))
	{
		return soap->error;
	}
	return SOAP_OK;
}

SOAP_FMAC3 struct __timg__GetOptions * SOAP_FMAC4 soap_get___timg__GetOptions(struct soap *soap, struct __timg__GetOptions *p, const char *tag, const char *type)
{
	if ((p = soap_in___timg__GetOptions(soap, tag, p, type)) != NULL)
	{
		if (soap_getindependent(soap))
		{
			return NULL;
		}
	}
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_timg__GetOptions(struct soap *soap, struct _timg__GetOptions *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE__timg__GetOptions))
	{
		soap_serialize__timg__GetOptions(soap, *a);
	}
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_timg__GetOptions(struct soap *soap, const char *tag, int id, struct _timg__GetOptions *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE__timg__GetOptions);
	if (id < 0)
	{
		return soap->error;
	}
	return soap_out__timg__GetOptions(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct _timg__GetOptions ** SOAP_FMAC4 soap_in_PointerTo_timg__GetOptions(struct soap *soap, const char *tag, struct _timg__GetOptions **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
	{
		return NULL;
	}
	if (NULL==a)
	{
		if (NULL==(a = (struct _timg__GetOptions **)soap_malloc(soap, sizeof(struct _timg__GetOptions *))))
		{
			return NULL;
		}
	}
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	
		soap_revert(soap);
		if (NULL==(*a = soap_in__timg__GetOptions(soap, tag, *a, type)))
		{
			return NULL;
		}
	}
	else
	{	
		a = (struct _timg__GetOptions **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE__timg__GetOptions, sizeof(struct _timg__GetOptions), 0);
		if (soap->body && soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_timg__GetOptions(struct soap *soap, struct _timg__GetOptions *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTo_timg__GetOptions);
	if (soap_out_PointerTo_timg__GetOptions(soap, tag?tag:"timg:GetOptions", id, a, type))
	{
		return soap->error;
	}
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _timg__GetOptions ** SOAP_FMAC4 soap_get_PointerTo_timg__GetOptions(struct soap *soap, struct _timg__GetOptions **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTo_timg__GetOptions(soap, tag, p, type)) != NULL)
	{
		if (soap_getindependent(soap))
		{
			return NULL;
		}
	}
	return p;
}
SOAP_FMAC3 void SOAP_FMAC4 soap_default__timg__GetOptions(struct soap *soap, struct _timg__GetOptions *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_default_tt__ReferenceToken(soap, &a->VideoSourceToken);
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__timg__GetOptions(struct soap *soap, const struct _timg__GetOptions *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_tt__ReferenceToken(soap, &a->VideoSourceToken);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__timg__GetOptions(struct soap *soap, const char *tag, int id, const struct _timg__GetOptions *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__timg__GetOptions), type))
	{
		return soap->error;
	}
	if (a->VideoSourceToken != NULL)
	{	
		if (soap_out_tt__ReferenceToken(soap, "timg:VideoSourceToken", -1, &a->VideoSourceToken, ""))
		{
			return soap->error;
		}
	}
	else if (soap_element_nil(soap, "timg:VideoSourceToken"))
	{
		return soap->error;
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _timg__GetOptions * SOAP_FMAC4 soap_in__timg__GetOptions(struct soap *soap, const char *tag, struct _timg__GetOptions *a, const char *type)
{
	size_t soap_flag_VideoSourceToken = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
	{
		return NULL;
	}
	a = (struct _timg__GetOptions *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__timg__GetOptions, sizeof(struct _timg__GetOptions), 0, NULL, NULL, NULL);
	if (NULL==a)
	{
		return NULL;
	}
	soap_default__timg__GetOptions(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	
			soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_VideoSourceToken && (soap->error == SOAP_TAG_MISMATCH || soap->error == SOAP_NO_TAG))
				if (soap_in_tt__ReferenceToken(soap, "timg:VideoSourceToken", &a->VideoSourceToken, "tt:ReferenceToken"))
				{	
					soap_flag_VideoSourceToken--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
			{
				soap->error = soap_ignore_element(soap);
			}
			if (soap->error == SOAP_NO_TAG)
			{
				break;
			}
			if (soap->error)
			{
				return NULL;
			}
		}
		if (soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	else
	{	
		a = (struct _timg__GetOptions *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__timg__GetOptions, 0, sizeof(struct _timg__GetOptions), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_VideoSourceToken > 0))
	{	
		soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__timg__GetOptions(struct soap *soap, const struct _timg__GetOptions *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__timg__GetOptions);
	if (soap_out__timg__GetOptions(soap, tag?tag:"timg:GetOptions", id, a, type))
	{
		return soap->error;
	}
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _timg__GetOptions * SOAP_FMAC4 soap_get__timg__GetOptions(struct soap *soap, struct _timg__GetOptions *p, const char *tag, const char *type)
{
	if ((p = soap_in__timg__GetOptions(soap, tag, p, type)) != NULL)
	{
		if (soap_getindependent(soap))
		{
			return NULL;
		}
	}
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default__timg__GetOptionsResponse(struct soap *soap, struct _timg__GetOptionsResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->ImagingOptions = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__timg__GetOptionsResponse(struct soap *soap, const struct _timg__GetOptionsResponse *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTott__ImagingOptions20(soap, &a->ImagingOptions);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out__timg__GetOptionsResponse(struct soap *soap, const char *tag, int id, const struct _timg__GetOptionsResponse *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE__timg__GetOptionsResponse), type))
	{
		return soap->error;
	}
	if (a->ImagingOptions != NULL)
	{
		soap_element_result(soap, "timg:ImagingOptions");
	}
	if (a->ImagingOptions != NULL)
	{	
		if (soap_out_PointerTott__ImagingOptions20(soap, "timg:ImagingOptions", -1, &a->ImagingOptions, ""))
		{
			return soap->error;
		}
	}
	else if (soap_element_nil(soap, "timg:ImagingOptions"))
	{
		return soap->error;
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct _timg__GetOptionsResponse * SOAP_FMAC4 soap_in__timg__GetOptionsResponse(struct soap *soap, const char *tag, struct _timg__GetOptionsResponse *a, const char *type)
{
	size_t soap_flag_ImagingOptions = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
	{
		return NULL;
	}
	a = (struct _timg__GetOptionsResponse *)soap_id_enter(soap, soap->id, a, SOAP_TYPE__timg__GetOptionsResponse, sizeof(struct _timg__GetOptionsResponse), 0, NULL, NULL, NULL);
	if (NULL==a)
	{
		return NULL;
	}
	soap_default__timg__GetOptionsResponse(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	
			soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_ImagingOptions && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__ImagingOptions20(soap, "timg:ImagingOptions", &a->ImagingOptions, "tt:ImagingOptions20"))
				{	
					soap_flag_ImagingOptions--;
					continue;
				}
			soap_check_result(soap, "timg:ImagingOptions");
			if (soap->error == SOAP_TAG_MISMATCH)
			{
				soap->error = soap_ignore_element(soap);
			}
			if (soap->error == SOAP_NO_TAG)
			{
				break;
			}
			if (soap->error)
			{
				return NULL;
			}
		}
		if (soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	else
	{	
		a = (struct _timg__GetOptionsResponse *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE__timg__GetOptionsResponse, 0, sizeof(struct _timg__GetOptionsResponse), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	if ((soap->mode & SOAP_XML_STRICT) && (soap_flag_ImagingOptions > 0))
	{	
		soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put__timg__GetOptionsResponse(struct soap *soap, const struct _timg__GetOptionsResponse *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE__timg__GetOptionsResponse);
	if (soap_out__timg__GetOptionsResponse(soap, tag?tag:"timg:GetOptionsResponse", id, a, type))
	{
		return soap->error;
	}
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct _timg__GetOptionsResponse * SOAP_FMAC4 soap_get__timg__GetOptionsResponse(struct soap *soap, struct _timg__GetOptionsResponse *p, const char *tag, const char *type)
{
	if ((p = soap_in__timg__GetOptionsResponse(soap, tag, p, type)) != NULL)
	{
		if (soap_getindependent(soap))
		{
			return NULL;
		}
	}
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__ImagingOptions20(struct soap *soap, struct tt__ImagingOptions20 *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__ImagingOptions20))
	{
		soap_serialize_tt__ImagingOptions20(soap, *a);
	}
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__ImagingOptions20(struct soap *soap, const char *tag, int id, struct tt__ImagingOptions20 *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__ImagingOptions20);
	if (id < 0)
	{
		return soap->error;
	}
	return soap_out_tt__ImagingOptions20(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__ImagingOptions20 ** SOAP_FMAC4 soap_in_PointerTott__ImagingOptions20(struct soap *soap, const char *tag, struct tt__ImagingOptions20 **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
	{
		return NULL;
	}
	if (NULL==a)
	{
		if (NULL==(a = (struct tt__ImagingOptions20 **)soap_malloc(soap, sizeof(struct tt__ImagingOptions20 *))))
		{
			return NULL;
		}
	}
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	
		soap_revert(soap);
		if (NULL==(*a = soap_in_tt__ImagingOptions20(soap, tag, *a, type)))
		{
			return NULL;
		}
	}
	else
	{	
		a = (struct tt__ImagingOptions20 **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__ImagingOptions20, sizeof(struct tt__ImagingOptions20), 0);
		if (soap->body && soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__ImagingOptions20(struct soap *soap, struct tt__ImagingOptions20 *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__ImagingOptions20);
	if (soap_out_PointerTott__ImagingOptions20(soap, tag?tag:"tt:ImagingOptions20", id, a, type))
	{
		return soap->error;
	}
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__ImagingOptions20 ** SOAP_FMAC4 soap_get_PointerTott__ImagingOptions20(struct soap *soap, struct tt__ImagingOptions20 **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__ImagingOptions20(soap, tag, p, type)) != NULL)
	{
		if (soap_getindependent(soap))
		{
			return NULL;
		}
	}
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__ImagingOptions20(struct soap *soap, struct tt__ImagingOptions20 *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->BacklightCompensation = NULL;
	a->Brightness = NULL;
	a->ColorSaturation = NULL;
	a->Contrast = NULL;
	a->Exposure = NULL;
	a->Focus = NULL;
	a->__sizeIrCutFilterModes = 0;
	a->IrCutFilterModes = NULL;
	a->Sharpness = NULL;
	a->WideDynamicRange = NULL;
	a->WhiteBalance = NULL;
	a->Extension = NULL;
	a->__anyAttribute = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__ImagingOptions20(struct soap *soap, const struct tt__ImagingOptions20 *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	soap_serialize_PointerTott__BacklightCompensationOptions20(soap, &a->BacklightCompensation);
	soap_serialize_PointerTott__FloatRange(soap, &a->Brightness);
	soap_serialize_PointerTott__FloatRange(soap, &a->ColorSaturation);
	soap_serialize_PointerTott__FloatRange(soap, &a->Contrast);
	soap_serialize_PointerTott__ExposureOptions20(soap, &a->Exposure);
	soap_serialize_PointerTott__FocusOptions20(soap, &a->Focus);
	if (a->IrCutFilterModes != NULL)
	{	
		int i;
		for (i = 0; i < a->__sizeIrCutFilterModes; i++)
		{
			soap_embedded(soap, a->IrCutFilterModes + i, SOAP_TYPE_tt__IrCutFilterMode);
		}
	}
	soap_serialize_PointerTott__FloatRange(soap, &a->Sharpness);
	soap_serialize_PointerTott__WideDynamicRangeOptions20(soap, &a->WideDynamicRange);
	soap_serialize_PointerTott__WhiteBalanceOptions20(soap, &a->WhiteBalance);
	soap_serialize_PointerTott__ImagingOptions20Extension(soap, &a->Extension);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__ImagingOptions20(struct soap *soap, const char *tag, int id, const struct tt__ImagingOptions20 *a, const char *type)
{
	if (a->__anyAttribute != NULL)
	{
		soap_set_attr(soap, "-anyAttribute", a->__anyAttribute, 1);
	}
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__ImagingOptions20), type))
	{
		return soap->error;
	}
	if (soap_out_PointerTott__BacklightCompensationOptions20(soap, "tt:BacklightCompensation", -1, &a->BacklightCompensation, ""))
	{
		return soap->error;
	}
	if (soap_out_PointerTott__FloatRange(soap, "tt:Brightness", -1, &a->Brightness, ""))
	{
		return soap->error;
	}
	if (soap_out_PointerTott__FloatRange(soap, "tt:ColorSaturation", -1, &a->ColorSaturation, ""))
	{
		return soap->error;
	}
	if (soap_out_PointerTott__FloatRange(soap, "tt:Contrast", -1, &a->Contrast, ""))
	{
		return soap->error;
	}
	if (soap_out_PointerTott__ExposureOptions20(soap, "tt:Exposure", -1, &a->Exposure, ""))
	{
		return soap->error;
	}
	if (soap_out_PointerTott__FocusOptions20(soap, "tt:Focus", -1, &a->Focus, ""))
	{
		return soap->error;
	}
	if (a->IrCutFilterModes != NULL)
	{	
		int i;
		for (i = 0; i < a->__sizeIrCutFilterModes; i++)
		{
			if (soap_out_tt__IrCutFilterMode(soap, "tt:IrCutFilterModes", -1, a->IrCutFilterModes + i, ""))
			{
				return soap->error;
			}
		}
	}
	if (soap_out_PointerTott__FloatRange(soap, "tt:Sharpness", -1, &a->Sharpness, ""))
	{
		return soap->error;
	}
	if (soap_out_PointerTott__WideDynamicRangeOptions20(soap, "tt:WideDynamicRange", -1, &a->WideDynamicRange, ""))
	{
		return soap->error;
	}
	if (soap_out_PointerTott__WhiteBalanceOptions20(soap, "tt:WhiteBalance", -1, &a->WhiteBalance, ""))
	{
		return soap->error;
	}
	if (soap_out_PointerTott__ImagingOptions20Extension(soap, "tt:Extension", -1, &a->Extension, ""))
	{
		return soap->error;
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__ImagingOptions20 * SOAP_FMAC4 soap_in_tt__ImagingOptions20(struct soap *soap, const char *tag, struct tt__ImagingOptions20 *a, const char *type)
{
	size_t soap_flag_BacklightCompensation = 1;
	size_t soap_flag_Brightness = 1;
	size_t soap_flag_ColorSaturation = 1;
	size_t soap_flag_Contrast = 1;
	size_t soap_flag_Exposure = 1;
	size_t soap_flag_Focus = 1;
	struct soap_blist *soap_blist_IrCutFilterModes = NULL;
	size_t soap_flag_Sharpness = 1;
	size_t soap_flag_WideDynamicRange = 1;
	size_t soap_flag_WhiteBalance = 1;
	size_t soap_flag_Extension = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
	{
		return NULL;
	}
	a = (struct tt__ImagingOptions20 *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__ImagingOptions20, sizeof(struct tt__ImagingOptions20), 0, NULL, NULL, NULL);
	if (NULL==a)
	{
		return NULL;
	}
	soap_default_tt__ImagingOptions20(soap, a);
	if (soap_s2string(soap, soap_attr_value(soap, "-anyAttribute", 0), &a->__anyAttribute, 0, -1))
	{
		return NULL;
	}
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	
			soap->error = SOAP_TAG_MISMATCH;
			if (soap_flag_BacklightCompensation && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__BacklightCompensationOptions20(soap, "tt:BacklightCompensation", &a->BacklightCompensation, "tt:BacklightCompensationOptions20"))
				{	
					soap_flag_BacklightCompensation--;
					continue;
				}
			if (soap_flag_Brightness && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FloatRange(soap, "tt:Brightness", &a->Brightness, "tt:FloatRange"))
				{	
					soap_flag_Brightness--;
					continue;
				}
			if (soap_flag_ColorSaturation && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FloatRange(soap, "tt:ColorSaturation", &a->ColorSaturation, "tt:FloatRange"))
				{	
					soap_flag_ColorSaturation--;
					continue;
				}
			if (soap_flag_Contrast && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FloatRange(soap, "tt:Contrast", &a->Contrast, "tt:FloatRange"))
				{	
					soap_flag_Contrast--;
					continue;
				}
			if (soap_flag_Exposure && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__ExposureOptions20(soap, "tt:Exposure", &a->Exposure, "tt:ExposureOptions20"))
				{	
					soap_flag_Exposure--;
					continue;
				}
			if (soap_flag_Focus && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FocusOptions20(soap, "tt:Focus", &a->Focus, "tt:FocusOptions20"))
				{
					soap_flag_Focus--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tt:IrCutFilterModes", 1, NULL))
			{	
				if (a->IrCutFilterModes == NULL)
				{	
					if (soap_blist_IrCutFilterModes == NULL)
					{
						soap_blist_IrCutFilterModes = soap_new_block(soap);
					}
					a->IrCutFilterModes = (enum tt__IrCutFilterMode *)soap_push_block(soap, soap_blist_IrCutFilterModes, sizeof(enum tt__IrCutFilterMode));
					if (a->IrCutFilterModes == NULL)
					{
						return NULL;
					}
					soap_default_tt__IrCutFilterMode(soap, a->IrCutFilterModes);
				}
				soap_revert(soap);
				if (soap_in_tt__IrCutFilterMode(soap, "tt:IrCutFilterModes", a->IrCutFilterModes, "tt:IrCutFilterMode"))
				{
					a->__sizeIrCutFilterModes++;
					a->IrCutFilterModes = NULL;
					continue;
				}
			}
			if (soap_flag_Sharpness && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FloatRange(soap, "tt:Sharpness", &a->Sharpness, "tt:FloatRange"))
				{	
					soap_flag_Sharpness--;
					continue;
				}
			if (soap_flag_WideDynamicRange && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__WideDynamicRangeOptions20(soap, "tt:WideDynamicRange", &a->WideDynamicRange, "tt:WideDynamicRangeOptions20"))
				{	
					soap_flag_WideDynamicRange--;
					continue;
				}
			if (soap_flag_WhiteBalance && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__WhiteBalanceOptions20(soap, "tt:WhiteBalance", &a->WhiteBalance, "tt:WhiteBalanceOptions20"))
				{
					soap_flag_WhiteBalance--;
				continue;
				}
			if (soap_flag_Extension && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__ImagingOptions20Extension(soap, "tt:Extension", &a->Extension, "tt:ImagingOptions20Extension"))
				{	
					soap_flag_Extension--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
			{
				soap->error = soap_ignore_element(soap);
			}
			if (soap->error == SOAP_NO_TAG)
			{
				break;
			}
			if (soap->error)
			{
				return NULL;
			}
		}
		if (a->IrCutFilterModes != NULL)
		{
			soap_pop_block(soap, soap_blist_IrCutFilterModes);
		}
		if (a->__sizeIrCutFilterModes)
		{
			a->IrCutFilterModes = (enum tt__IrCutFilterMode *)soap_save_block(soap, soap_blist_IrCutFilterModes, NULL, 1);
		}
		else
		{	
			a->IrCutFilterModes = NULL;
			if (soap_blist_IrCutFilterModes)
			{
				soap_end_block(soap, soap_blist_IrCutFilterModes);
			}
		}
		if (soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	else
	{	
		a = (struct tt__ImagingOptions20 *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__ImagingOptions20, 0, sizeof(struct tt__ImagingOptions20), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__ImagingOptions20(struct soap *soap, const struct tt__ImagingOptions20 *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__ImagingOptions20);
	if (soap_out_tt__ImagingOptions20(soap, tag?tag:"tt:ImagingOptions20", id, a, type))
	{
		return soap->error;
	}
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__ImagingOptions20 * SOAP_FMAC4 soap_get_tt__ImagingOptions20(struct soap *soap, struct tt__ImagingOptions20 *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__ImagingOptions20(soap, tag, p, type)) != NULL)
	{
		if (soap_getindependent(soap))
		{
			return NULL;
		}
	}
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__BacklightCompensationOptions20(struct soap *soap, struct tt__BacklightCompensationOptions20 *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__BacklightCompensationOptions20))
	{
		soap_serialize_tt__BacklightCompensationOptions20(soap, *a);
	}
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__BacklightCompensationOptions20(struct soap *soap, const char *tag, int id, struct tt__BacklightCompensationOptions20 *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__BacklightCompensationOptions20);
	if (id < 0)
	{
		return soap->error;
	}
	return soap_out_tt__BacklightCompensationOptions20(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__BacklightCompensationOptions20 ** SOAP_FMAC4 soap_in_PointerTott__BacklightCompensationOptions20(struct soap *soap, const char *tag, struct tt__BacklightCompensationOptions20 **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
	{
		return NULL;
	}
	if (NULL==a)
	{
		if (NULL==(a = (struct tt__BacklightCompensationOptions20 **)soap_malloc(soap, sizeof(struct tt__BacklightCompensationOptions20 *))))
		{
			return NULL;
		}
	}
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	
		soap_revert(soap);
		if (NULL==(*a = soap_in_tt__BacklightCompensationOptions20(soap, tag, *a, type)))
		{
			return NULL;
		}
	}
	else
	{	
		a = (struct tt__BacklightCompensationOptions20 **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__BacklightCompensationOptions20, sizeof(struct tt__BacklightCompensationOptions20), 0);
		if (soap->body && soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__BacklightCompensationOptions20(struct soap *soap, struct tt__BacklightCompensationOptions20 *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__BacklightCompensationOptions20);
	if (soap_out_PointerTott__BacklightCompensationOptions20(soap, tag?tag:"tt:BacklightCompensationOptions20", id, a, type))
	{
		return soap->error;
	}
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__BacklightCompensationOptions20 ** SOAP_FMAC4 soap_get_PointerTott__BacklightCompensationOptions20(struct soap *soap, struct tt__BacklightCompensationOptions20 **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__BacklightCompensationOptions20(soap, tag, p, type))!= NULL)
	{
		if (soap_getindependent(soap))
		{
			return NULL;
		}
	}
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__BacklightCompensationOptions20(struct soap *soap, struct tt__BacklightCompensationOptions20 *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__sizeMode = 0;
	a->Mode = NULL;
	a->Level = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__BacklightCompensationOptions20(struct soap *soap, const struct tt__BacklightCompensationOptions20 *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	if (a->Mode != NULL)
	{	
		int i;
		for (i = 0; i < a->__sizeMode; i++)
		{
			soap_embedded(soap, a->Mode + i, SOAP_TYPE_tt__BacklightCompensationMode);
		}
	}
	soap_serialize_PointerTott__FloatRange(soap, &a->Level);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__BacklightCompensationOptions20(struct soap *soap, const char *tag, int id, const struct tt__BacklightCompensationOptions20 *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__BacklightCompensationOptions20), type))
	{
		return soap->error;
	}
	if (a->Mode != NULL)
	{	
		int i;
		for (i = 0; i < a->__sizeMode; i++)
		{
			if (soap_out_tt__BacklightCompensationMode(soap, "tt:Mode", -1, a->Mode + i, ""))
			{
				return soap->error;
			}
		}
	}
	if (soap_out_PointerTott__FloatRange(soap, "tt:Level", -1, &a->Level, ""))
	{
		return soap->error;
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__BacklightCompensationOptions20 * SOAP_FMAC4 soap_in_tt__BacklightCompensationOptions20(struct soap *soap, const char *tag, struct tt__BacklightCompensationOptions20 *a, const char *type)
{
	struct soap_blist *soap_blist_Mode = NULL;
	size_t soap_flag_Level = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
	{
		return NULL;
	}
	a = (struct tt__BacklightCompensationOptions20 *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__BacklightCompensationOptions20, sizeof(struct tt__BacklightCompensationOptions20), 0, NULL, NULL, NULL);
	if (NULL==a)
	{
		return NULL;
	}
	soap_default_tt__BacklightCompensationOptions20(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	
			soap->error = SOAP_TAG_MISMATCH;
			if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tt:Mode", 1, NULL))
			{	
				if (a->Mode == NULL)
				{	
					if (soap_blist_Mode == NULL)
					{
						soap_blist_Mode = soap_new_block(soap);
					}
					a->Mode = (enum tt__BacklightCompensationMode *)soap_push_block(soap, soap_blist_Mode, sizeof(enum tt__BacklightCompensationMode));
					if (a->Mode == NULL)
					{
						return NULL;
					}
					soap_default_tt__BacklightCompensationMode(soap, a->Mode);
				}
				soap_revert(soap);
				if (soap_in_tt__BacklightCompensationMode(soap, "tt:Mode", a->Mode, "tt:BacklightCompensationMode"))
				{	
					a->__sizeMode++;
					a->Mode = NULL;
					continue;
				}
			}
			if (soap_flag_Level && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FloatRange(soap, "tt:Level", &a->Level, "tt:FloatRange"))
				{	
					soap_flag_Level--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
			{
				soap->error = soap_ignore_element(soap);
			}
			if (soap->error == SOAP_NO_TAG)
			{
				break;
			}
			if (soap->error)
			{
				return NULL;
			}
		}
		if (a->Mode != NULL)
		{
			soap_pop_block(soap, soap_blist_Mode);
		}
		if (a->__sizeMode)
		{
			a->Mode = (enum tt__BacklightCompensationMode *)soap_save_block(soap, soap_blist_Mode, NULL, 1);
		}
		else
		{	
			a->Mode = NULL;
			if (soap_blist_Mode)
			{
				soap_end_block(soap, soap_blist_Mode);
			}
		}
		if (soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	else
	{	
		a = (struct tt__BacklightCompensationOptions20 *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__BacklightCompensationOptions20, 0, sizeof(struct tt__BacklightCompensationOptions20), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	if ((soap->mode & SOAP_XML_STRICT) && (a->__sizeMode < 1))
	{	
		soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__BacklightCompensationOptions20(struct soap *soap, const struct tt__BacklightCompensationOptions20 *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__BacklightCompensationOptions20);
	if (soap_out_tt__BacklightCompensationOptions20(soap, tag?tag:"tt:BacklightCompensationOptions20", id, a, type))
	{
		return soap->error;
	}
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__BacklightCompensationOptions20 * SOAP_FMAC4 soap_get_tt__BacklightCompensationOptions20(struct soap *soap, struct tt__BacklightCompensationOptions20 *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__BacklightCompensationOptions20(soap, tag, p, type)) != NULL)
	{
		if (soap_getindependent(soap))
		{
			return NULL;
		}
	}
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__ExposureOptions20(struct soap *soap, struct tt__ExposureOptions20 *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__ExposureOptions20))
	{
		soap_serialize_tt__ExposureOptions20(soap, *a);
	}
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__ExposureOptions20(struct soap *soap, const char *tag, int id, struct tt__ExposureOptions20 *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__ExposureOptions20);
	if (id < 0)
	{
		return soap->error;
	}
	return soap_out_tt__ExposureOptions20(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__ExposureOptions20 ** SOAP_FMAC4 soap_in_PointerTott__ExposureOptions20(struct soap *soap, const char *tag, struct tt__ExposureOptions20 **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
	{
		return NULL;
	}
	if (NULL==a)
	{
		if (NULL==(a = (struct tt__ExposureOptions20 **)soap_malloc(soap, sizeof(struct tt__ExposureOptions20 *))))
		{
			return NULL;
		}
	}
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	
		soap_revert(soap);
		if (NULL==(*a = soap_in_tt__ExposureOptions20(soap, tag, *a, type)))
		{
			return NULL;
		}
	}
	else
	{	
		a = (struct tt__ExposureOptions20 **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__ExposureOptions20, sizeof(struct tt__ExposureOptions20), 0);
		if (soap->body && soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__ExposureOptions20(struct soap *soap, struct tt__ExposureOptions20 *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__ExposureOptions20);
	if (soap_out_PointerTott__ExposureOptions20(soap, tag?tag:"tt:ExposureOptions20", id, a, type))
	{
		return soap->error;
	}
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__ExposureOptions20 ** SOAP_FMAC4 soap_get_PointerTott__ExposureOptions20(struct soap *soap, struct tt__ExposureOptions20 **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__ExposureOptions20(soap, tag, p, type)) != NULL)
	{
		if (soap_getindependent(soap))
		{
			return NULL;
		}
	}
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__ExposureOptions20(struct soap *soap, struct tt__ExposureOptions20 *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__sizeMode = 0;
	a->Mode = NULL;
	a->__sizePriority = 0;
	a->Priority = NULL;
	a->MinExposureTime = NULL;
	a->MaxExposureTime = NULL;
	a->MinGain = NULL;
	a->MaxGain = NULL;
	a->MinIris = NULL;
	a->MaxIris = NULL;
	a->ExposureTime = NULL;
	a->Gain = NULL;
	a->Iris = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__ExposureOptions20(struct soap *soap, const struct tt__ExposureOptions20 *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	if (a->Mode != NULL)
	{	
		int i;
		for (i = 0; i < a->__sizeMode; i++)
		{
			soap_embedded(soap, a->Mode + i, SOAP_TYPE_tt__ExposureMode);
		}
	}
	if (a->Priority != NULL)
	{	
		int i;
		for (i = 0; i < a->__sizePriority; i++)
		{
			soap_embedded(soap, a->Priority + i, SOAP_TYPE_tt__ExposurePriority);
		}
	}
	soap_serialize_PointerTott__FloatRange(soap, &a->MinExposureTime);
	soap_serialize_PointerTott__FloatRange(soap, &a->MaxExposureTime);
	soap_serialize_PointerTott__FloatRange(soap, &a->MinGain);
	soap_serialize_PointerTott__FloatRange(soap, &a->MaxGain);
	soap_serialize_PointerTott__FloatRange(soap, &a->MinIris);
	soap_serialize_PointerTott__FloatRange(soap, &a->MaxIris);
	soap_serialize_PointerTott__FloatRange(soap, &a->ExposureTime);
	soap_serialize_PointerTott__FloatRange(soap, &a->Gain);
	soap_serialize_PointerTott__FloatRange(soap, &a->Iris);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__ExposureOptions20(struct soap *soap, const char *tag, int id, const struct tt__ExposureOptions20 *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__ExposureOptions20), type))
		return soap->error;
	if (a->Mode != NULL)
	{	
		int i;
		for (i = 0; i < a->__sizeMode; i++)
		{
			if (soap_out_tt__ExposureMode(soap, "tt:Mode", -1, a->Mode + i, ""))
			{
				return soap->error;
			}
		}
	}
	if (a->Priority != NULL)
	{	
		int i;
		for (i = 0; i < a->__sizePriority; i++)
		{
			if (soap_out_tt__ExposurePriority(soap, "tt:Priority", -1, a->Priority + i, ""))
			{
				return soap->error;
			}
		}
	}
	if (soap_out_PointerTott__FloatRange(soap, "tt:MinExposureTime", -1, &a->MinExposureTime, ""))
	{
		return soap->error;
	}
	if (soap_out_PointerTott__FloatRange(soap, "tt:MaxExposureTime", -1, &a->MaxExposureTime, ""))
	{
		return soap->error;
	}
	if (soap_out_PointerTott__FloatRange(soap, "tt:MinGain", -1, &a->MinGain, ""))
	{
		return soap->error;
	}
	if (soap_out_PointerTott__FloatRange(soap, "tt:MaxGain", -1, &a->MaxGain, ""))
	{
		return soap->error;
	}
	if (soap_out_PointerTott__FloatRange(soap, "tt:MinIris", -1, &a->MinIris, ""))
	{
		return soap->error;
	}
	if (soap_out_PointerTott__FloatRange(soap, "tt:MaxIris", -1, &a->MaxIris, ""))
	{
		return soap->error;
	}
	if (soap_out_PointerTott__FloatRange(soap, "tt:ExposureTime", -1, &a->ExposureTime, ""))
	{
		return soap->error;
	}
	if (soap_out_PointerTott__FloatRange(soap, "tt:Gain", -1, &a->Gain, ""))
	{
		return soap->error;
	}
	if (soap_out_PointerTott__FloatRange(soap, "tt:Iris", -1, &a->Iris, ""))
	{
		return soap->error;
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__ExposureOptions20 * SOAP_FMAC4 soap_in_tt__ExposureOptions20(struct soap *soap, const char *tag, struct tt__ExposureOptions20 *a, const char *type)
{
	struct soap_blist *soap_blist_Mode = NULL;
	struct soap_blist *soap_blist_Priority = NULL;
	size_t soap_flag_MinExposureTime = 1;
	size_t soap_flag_MaxExposureTime = 1;
	size_t soap_flag_MinGain = 1;
	size_t soap_flag_MaxGain = 1;
	size_t soap_flag_MinIris = 1;
	size_t soap_flag_MaxIris = 1;
	size_t soap_flag_ExposureTime = 1;
	size_t soap_flag_Gain = 1;
	size_t soap_flag_Iris = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
	{
		return NULL;
	}
	a = (struct tt__ExposureOptions20 *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__ExposureOptions20, sizeof(struct tt__ExposureOptions20), 0, NULL, NULL, NULL);
	if (NULL==a)
	{
		return NULL;
	}
	soap_default_tt__ExposureOptions20(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	
			soap->error = SOAP_TAG_MISMATCH;
			if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tt:Mode", 1, NULL))
			{	
				if (a->Mode == NULL)
				{	
					if (soap_blist_Mode == NULL)
					{
						soap_blist_Mode = soap_new_block(soap);
					}
					a->Mode = (enum tt__ExposureMode *)soap_push_block(soap, soap_blist_Mode, sizeof(enum tt__ExposureMode));
					if (a->Mode == NULL)
					{	
						return NULL;
					}
					soap_default_tt__ExposureMode(soap, a->Mode);
				}
				soap_revert(soap);
				if (soap_in_tt__ExposureMode(soap, "tt:Mode", a->Mode, "tt:ExposureMode"))
				{	
					a->__sizeMode++;
					a->Mode = NULL;
					continue;
				}
			}
			if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tt:Priority", 1, NULL))
			{	
				if (a->Priority == NULL)
				{	
					if (soap_blist_Priority == NULL)
					{
						soap_blist_Priority = soap_new_block(soap);
					}
					a->Priority = (enum tt__ExposurePriority *)soap_push_block(soap, soap_blist_Priority, sizeof(enum tt__ExposurePriority));
					if (a->Priority == NULL)
						return NULL;
					soap_default_tt__ExposurePriority(soap, a->Priority);
				}
				soap_revert(soap);
				if (soap_in_tt__ExposurePriority(soap, "tt:Priority", a->Priority, "tt:ExposurePriority"))
				{	
					a->__sizePriority++;
					a->Priority = NULL;
					continue;
				}
			}
			if (soap_flag_MinExposureTime && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FloatRange(soap, "tt:MinExposureTime", &a->MinExposureTime, "tt:FloatRange"))
				{	
					soap_flag_MinExposureTime--;
					continue;
				}
			if (soap_flag_MaxExposureTime && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FloatRange(soap, "tt:MaxExposureTime", &a->MaxExposureTime, "tt:FloatRange"))
				{	
					soap_flag_MaxExposureTime--;
					continue;
				}
			if (soap_flag_MinGain && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FloatRange(soap, "tt:MinGain", &a->MinGain, "tt:FloatRange"))
				{
					soap_flag_MinGain--;
					continue;
				}
			if (soap_flag_MaxGain && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FloatRange(soap, "tt:MaxGain", &a->MaxGain, "tt:FloatRange"))
				{	
					soap_flag_MaxGain--;
					continue;
				}
			if (soap_flag_MinIris && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FloatRange(soap, "tt:MinIris", &a->MinIris, "tt:FloatRange"))
				{
					soap_flag_MinIris--;
					continue;
				}
			if (soap_flag_MaxIris && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FloatRange(soap, "tt:MaxIris", &a->MaxIris, "tt:FloatRange"))
				{	
					soap_flag_MaxIris--;
					continue;
				}
			if (soap_flag_ExposureTime && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FloatRange(soap, "tt:ExposureTime", &a->ExposureTime, "tt:FloatRange"))
				{	
					soap_flag_ExposureTime--;
					continue;
				}
			if (soap_flag_Gain && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FloatRange(soap, "tt:Gain", &a->Gain, "tt:FloatRange"))
				{	
					soap_flag_Gain--;
					continue;
				}
			if (soap_flag_Iris && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FloatRange(soap, "tt:Iris", &a->Iris, "tt:FloatRange"))
				{	
					soap_flag_Iris--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
			{
				soap->error = soap_ignore_element(soap);
			}
			if (soap->error == SOAP_NO_TAG)
			{
				break;
			}
			if (soap->error)
			{
				return NULL;
			}
		}
		if (a->Mode != NULL)
		{
			soap_pop_block(soap, soap_blist_Mode);
		}
		if (a->__sizeMode)
		{
			a->Mode = (enum tt__ExposureMode *)soap_save_block(soap, soap_blist_Mode, NULL, 1);
		}
		else
		{	
			a->Mode = NULL;
			if (soap_blist_Mode)
			{
				soap_end_block(soap, soap_blist_Mode);
			}
		}
		if (a->Priority != NULL)
		{
			soap_pop_block(soap, soap_blist_Priority);
		}
		if (a->__sizePriority)
		{
			a->Priority = (enum tt__ExposurePriority *)soap_save_block(soap, soap_blist_Priority, NULL, 1);
		}
		else
		{	
			a->Priority = NULL;
			if (soap_blist_Priority)
			{
				soap_end_block(soap, soap_blist_Priority);
			}
		}
		if (soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	else
	{	
		a = (struct tt__ExposureOptions20 *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__ExposureOptions20, 0, sizeof(struct tt__ExposureOptions20), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	if ((soap->mode & SOAP_XML_STRICT) && (a->__sizeMode < 1))
	{	
		soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__ExposureOptions20(struct soap *soap, const struct tt__ExposureOptions20 *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__ExposureOptions20);
	if (soap_out_tt__ExposureOptions20(soap, tag?tag:"tt:ExposureOptions20", id, a, type))
	{
		return soap->error;
	}
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__ExposureOptions20 * SOAP_FMAC4 soap_get_tt__ExposureOptions20(struct soap *soap, struct tt__ExposureOptions20 *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__ExposureOptions20(soap, tag, p, type))!= NULL)
	{
		if (soap_getindependent(soap))
		{
			return NULL;
		}
	}
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__FocusOptions20(struct soap *soap, struct tt__FocusOptions20 *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__FocusOptions20))
	{
		soap_serialize_tt__FocusOptions20(soap, *a);
	}
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__FocusOptions20(struct soap *soap, const char *tag, int id, struct tt__FocusOptions20 *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__FocusOptions20);
	if (id < 0)
	{
		return soap->error;
	}
	return soap_out_tt__FocusOptions20(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__FocusOptions20 ** SOAP_FMAC4 soap_in_PointerTott__FocusOptions20(struct soap *soap, const char *tag, struct tt__FocusOptions20 **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
	{
		return NULL;
	}
	if (a == NULL)
	{
		if (NULL==(a = (struct tt__FocusOptions20 **)soap_malloc(soap, sizeof(struct tt__FocusOptions20 *))))
		{
			return NULL;
		}
	}
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	
		soap_revert(soap);
		if (NULL==(*a = soap_in_tt__FocusOptions20(soap, tag, *a, type)))
		{
			return NULL;
		}
	}
	else
	{	
		a = (struct tt__FocusOptions20 **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__FocusOptions20, sizeof(struct tt__FocusOptions20), 0);
		if (soap->body && soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__FocusOptions20(struct soap *soap, struct tt__FocusOptions20 *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__FocusOptions20);
	if (soap_out_PointerTott__FocusOptions20(soap, tag?tag:"tt:FocusOptions20", id, a, type))
	{
		return soap->error;
	}
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__FocusOptions20 ** SOAP_FMAC4 soap_get_PointerTott__FocusOptions20(struct soap *soap, struct tt__FocusOptions20 **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__FocusOptions20(soap, tag, p, type)) != NULL)
	{
		if (soap_getindependent(soap))
		{
			return NULL;
		}
	}
	return p;
}


SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__FocusOptions20(struct soap *soap, struct tt__FocusOptions20 *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__sizeAutoFocusModes = 0;
	a->AutoFocusModes = NULL;
	a->DefaultSpeed = NULL;
	a->NearLimit = NULL;
	a->FarLimit = NULL;
	a->Extension = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__FocusOptions20(struct soap *soap, const struct tt__FocusOptions20 *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	if (a->AutoFocusModes != NULL)
	{	
		int i;
		for (i = 0; i < a->__sizeAutoFocusModes; i++)
		{
			soap_embedded(soap, a->AutoFocusModes + i, SOAP_TYPE_tt__AutoFocusMode);
		}
	}
	soap_serialize_PointerTott__FloatRange(soap, &a->DefaultSpeed);
	soap_serialize_PointerTott__FloatRange(soap, &a->NearLimit);
	soap_serialize_PointerTott__FloatRange(soap, &a->FarLimit);
	soap_serialize_PointerTott__FocusOptions20Extension(soap, &a->Extension);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__FocusOptions20(struct soap *soap, const char *tag, int id, const struct tt__FocusOptions20 *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__FocusOptions20), type))
	{
		return soap->error;
	}

	if (a->AutoFocusModes != NULL)
	{	
		int i;
		for (i = 0; i < a->__sizeAutoFocusModes; i++)
		{
			if (soap_out_tt__AutoFocusMode(soap, "tt:AutoFocusModes", -1, a->AutoFocusModes + i, ""))
			{
				return soap->error;
			}
		}
	}
	if (soap_out_PointerTott__FloatRange(soap, "tt:DefaultSpeed", -1, &a->DefaultSpeed, ""))
	{
		return soap->error;
	}
	if (soap_out_PointerTott__FloatRange(soap, "tt:NearLimit", -1, &a->NearLimit, ""))
	{
		return soap->error;
	}
	if (soap_out_PointerTott__FloatRange(soap, "tt:FarLimit", -1, &a->FarLimit, ""))
	{
		return soap->error;
	}
	if (soap_out_PointerTott__FocusOptions20Extension(soap, "tt:Extension", -1, &a->Extension, ""))
	{
		return soap->error;
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__FocusOptions20 * SOAP_FMAC4 soap_in_tt__FocusOptions20(struct soap *soap, const char *tag, struct tt__FocusOptions20 *a, const char *type)
{
	struct soap_blist *soap_blist_AutoFocusModes = NULL;
	size_t soap_flag_DefaultSpeed = 1;
	size_t soap_flag_NearLimit = 1;
	size_t soap_flag_FarLimit = 1;
	size_t soap_flag_Extension = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
	{
		return NULL;
	}
	a = (struct tt__FocusOptions20 *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__FocusOptions20, sizeof(struct tt__FocusOptions20), 0, NULL, NULL, NULL);
	if (NULL==a)
	{
		return NULL;
	}
	soap_default_tt__FocusOptions20(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	
			soap->error = SOAP_TAG_MISMATCH;
			if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tt:AutoFocusModes", 1, NULL))
			{	
				if (a->AutoFocusModes == NULL)
				{	
					if (soap_blist_AutoFocusModes == NULL)
					{
						soap_blist_AutoFocusModes = soap_new_block(soap);
					}
					a->AutoFocusModes = (enum tt__AutoFocusMode *)soap_push_block(soap, soap_blist_AutoFocusModes, sizeof(enum tt__AutoFocusMode));
					if (a->AutoFocusModes == NULL)
					{
						return NULL;
					}
					soap_default_tt__AutoFocusMode(soap, a->AutoFocusModes);
				}
				soap_revert(soap);
				if (soap_in_tt__AutoFocusMode(soap, "tt:AutoFocusModes", a->AutoFocusModes, "tt:AutoFocusMode"))
				{	
					a->__sizeAutoFocusModes++;
					a->AutoFocusModes = NULL;
					continue;
				}
			}
			if (soap_flag_DefaultSpeed && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FloatRange(soap, "tt:DefaultSpeed", &a->DefaultSpeed, "tt:FloatRange"))
				{	
					soap_flag_DefaultSpeed--;
					continue;
				}
			if (soap_flag_NearLimit && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FloatRange(soap, "tt:NearLimit", &a->NearLimit, "tt:FloatRange"))
				{	
					soap_flag_NearLimit--;
					continue;
				}
			if (soap_flag_FarLimit && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FloatRange(soap, "tt:FarLimit", &a->FarLimit, "tt:FloatRange"))
				{	
					soap_flag_FarLimit--;
					continue;
				}
			if (soap_flag_Extension && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FocusOptions20Extension(soap, "tt:Extension", &a->Extension, "tt:FocusOptions20Extension"))
				{	
					soap_flag_Extension--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
			{
				soap->error = soap_ignore_element(soap);
			}
			if (soap->error == SOAP_NO_TAG)
			{
				break;
			}
			if (soap->error)
			{
				return NULL;
			}
		}
		if (a->AutoFocusModes != NULL)
		{
			soap_pop_block(soap, soap_blist_AutoFocusModes);
		}
		if (a->__sizeAutoFocusModes)
		{
			a->AutoFocusModes = (enum tt__AutoFocusMode *)soap_save_block(soap, soap_blist_AutoFocusModes, NULL, 1);
		}
		else
		{	
			a->AutoFocusModes = NULL;
			if (soap_blist_AutoFocusModes)
			{
				soap_end_block(soap, soap_blist_AutoFocusModes);
			}
		}
		if (soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	else
	{	
		a = (struct tt__FocusOptions20 *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__FocusOptions20, 0, sizeof(struct tt__FocusOptions20), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__FocusOptions20(struct soap *soap, const struct tt__FocusOptions20 *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__FocusOptions20);
	if (soap_out_tt__FocusOptions20(soap, tag?tag:"tt:FocusOptions20", id, a, type))
	{
		return soap->error;
	}
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__FocusOptions20 * SOAP_FMAC4 soap_get_tt__FocusOptions20(struct soap *soap, struct tt__FocusOptions20 *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__FocusOptions20(soap, tag, p, type)) != NULL)
	{
		if (soap_getindependent(soap))
		{
			return NULL;
		}
	}
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__FocusOptions20Extension(struct soap *soap, struct tt__FocusOptions20Extension *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__FocusOptions20Extension))
	{
		soap_serialize_tt__FocusOptions20Extension(soap, *a);
	}
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__FocusOptions20Extension(struct soap *soap, const char *tag, int id, struct tt__FocusOptions20Extension *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__FocusOptions20Extension);
	if (id < 0)
	{
		return soap->error;
	}
	return soap_out_tt__FocusOptions20Extension(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__FocusOptions20Extension ** SOAP_FMAC4 soap_in_PointerTott__FocusOptions20Extension(struct soap *soap, const char *tag, struct tt__FocusOptions20Extension **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
	{
		return NULL;
	}
	if (NULL==a)
	{
		if (NULL==(a = (struct tt__FocusOptions20Extension **)soap_malloc(soap, sizeof(struct tt__FocusOptions20Extension *))))
		{
			return NULL;
		}
	}
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	
		soap_revert(soap);
		if (NULL==(*a = soap_in_tt__FocusOptions20Extension(soap, tag, *a, type)))
		{
			return NULL;
		}
	}
	else
	{	
		a = (struct tt__FocusOptions20Extension **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__FocusOptions20Extension, sizeof(struct tt__FocusOptions20Extension), 0);
		if (soap->body && soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__FocusOptions20Extension(struct soap *soap, struct tt__FocusOptions20Extension *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__FocusOptions20Extension);
	if (soap_out_PointerTott__FocusOptions20Extension(soap, tag?tag:"tt:FocusOptions20Extension", id, a, type))
	{
		return soap->error;
	}
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__FocusOptions20Extension ** SOAP_FMAC4 soap_get_PointerTott__FocusOptions20Extension(struct soap *soap, struct tt__FocusOptions20Extension **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__FocusOptions20Extension(soap, tag, p, type)) != NULL)
	{
		if (soap_getindependent(soap))
		{
			return NULL;
		}
	}
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__FocusOptions20Extension(struct soap *soap, struct tt__FocusOptions20Extension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__size = 0;
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__FocusOptions20Extension(struct soap *soap, const struct tt__FocusOptions20Extension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__FocusOptions20Extension(struct soap *soap, const char *tag, int id, const struct tt__FocusOptions20Extension *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__FocusOptions20Extension), type))
	{
		return soap->error;
	}
	if (a->__any != NULL)
	{	
		int i;
		for (i = 0; i < a->__size; i++)
		{
			soap_outliteral(soap, "-any", a->__any + i, NULL);
		}
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__FocusOptions20Extension * SOAP_FMAC4 soap_in_tt__FocusOptions20Extension(struct soap *soap, const char *tag, struct tt__FocusOptions20Extension *a, const char *type)
{
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
	{
		return NULL;
	}
	a = (struct tt__FocusOptions20Extension *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__FocusOptions20Extension, sizeof(struct tt__FocusOptions20Extension), 0, NULL, NULL, NULL);
	if (a == NULL)
	{
		return NULL;
	}
	soap_default_tt__FocusOptions20Extension(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	
			soap->error = SOAP_TAG_MISMATCH;
			if (soap->error == SOAP_TAG_MISMATCH && !soap_peek_element(soap))
			{	
				if (a->__any == NULL)
				{	
					if (soap_blist___any == NULL)
					{
						soap_blist___any = soap_new_block(soap);
					}
					a->__any = (char **)soap_push_block(soap, soap_blist___any, sizeof(char *));
					if (a->__any == NULL)
					{
						return NULL;
					}
					*a->__any = NULL;
				}
				if (soap_inliteral(soap, "-any", a->__any))
				{	
					a->__size++;
					a->__any = NULL;
					continue;
				}
			}
			if (soap->error == SOAP_TAG_MISMATCH)
			{
				soap->error = soap_ignore_element(soap);
			}
			if (soap->error == SOAP_NO_TAG)
			{
				break;
			}
			if (soap->error)
			{
				return NULL;
			}
		}
		if (a->__any != NULL)
		{
			soap_pop_block(soap, soap_blist___any);
		}
		if (a->__size)
		{
			a->__any = (char **)soap_save_block(soap, soap_blist___any, NULL, 1);
		}
		else
		{	
			a->__any = NULL;
			if (soap_blist___any)
			{
				soap_end_block(soap, soap_blist___any);
			}
		}
		if (soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	else
	{	
		a = (struct tt__FocusOptions20Extension *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__FocusOptions20Extension, 0, sizeof(struct tt__FocusOptions20Extension), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__FocusOptions20Extension(struct soap *soap, const struct tt__FocusOptions20Extension *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__FocusOptions20Extension);
	if (soap_out_tt__FocusOptions20Extension(soap, tag?tag:"tt:FocusOptions20Extension", id, a, type))
	{
		return soap->error;
	}
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__FocusOptions20Extension * SOAP_FMAC4 soap_get_tt__FocusOptions20Extension(struct soap *soap, struct tt__FocusOptions20Extension *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__FocusOptions20Extension(soap, tag, p, type)) != NULL)
	{
		if (soap_getindependent(soap))
		{
			return NULL;
		}
	}
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__WideDynamicRangeOptions20(struct soap *soap, struct tt__WideDynamicRangeOptions20 *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__WideDynamicRangeOptions20))
	{
		soap_serialize_tt__WideDynamicRangeOptions20(soap, *a);
	}
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__WideDynamicRangeOptions20(struct soap *soap, const char *tag, int id, struct tt__WideDynamicRangeOptions20 *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__WideDynamicRangeOptions20);
	if (id < 0)
	{
		return soap->error;
	}
	return soap_out_tt__WideDynamicRangeOptions20(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__WideDynamicRangeOptions20 ** SOAP_FMAC4 soap_in_PointerTott__WideDynamicRangeOptions20(struct soap *soap, const char *tag, struct tt__WideDynamicRangeOptions20 **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
	{
		return NULL;
	}
	if (NULL==a)
	{
		if (NULL==(a = (struct tt__WideDynamicRangeOptions20 **)soap_malloc(soap, sizeof(struct tt__WideDynamicRangeOptions20 *))))
		{
			return NULL;
		}
	}
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	
		soap_revert(soap);
		if (NULL==(*a = soap_in_tt__WideDynamicRangeOptions20(soap, tag, *a, type)))
		{
			return NULL;
		}
	}
	else
	{	
		a = (struct tt__WideDynamicRangeOptions20 **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__WideDynamicRangeOptions20, sizeof(struct tt__WideDynamicRangeOptions20), 0);
		if (soap->body && soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__WideDynamicRangeOptions20(struct soap *soap, struct tt__WideDynamicRangeOptions20 *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__WideDynamicRangeOptions20);
	if (soap_out_PointerTott__WideDynamicRangeOptions20(soap, tag?tag:"tt:WideDynamicRangeOptions20", id, a, type))
	{
		return soap->error;
	}
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__WideDynamicRangeOptions20 ** SOAP_FMAC4 soap_get_PointerTott__WideDynamicRangeOptions20(struct soap *soap, struct tt__WideDynamicRangeOptions20 **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__WideDynamicRangeOptions20(soap, tag, p, type)) != NULL)
	{
		if (soap_getindependent(soap))
		{
			return NULL;
		}
	}
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__WideDynamicRangeOptions20(struct soap *soap, struct tt__WideDynamicRangeOptions20 *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__sizeMode = 0;
	a->Mode = NULL;
	a->Level = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__WideDynamicRangeOptions20(struct soap *soap, const struct tt__WideDynamicRangeOptions20 *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	if (a->Mode != NULL)
	{	
		int i;
		for (i = 0; i < a->__sizeMode; i++)
		{
			soap_embedded(soap, a->Mode + i, SOAP_TYPE_tt__WideDynamicMode);
		}
	}
	soap_serialize_PointerTott__FloatRange(soap, &a->Level);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__WideDynamicRangeOptions20(struct soap *soap, const char *tag, int id, const struct tt__WideDynamicRangeOptions20 *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__WideDynamicRangeOptions20), type))
	{
		return soap->error;
	}
	if (a->Mode != NULL)
	{	
		int i;
		for (i = 0; i < a->__sizeMode; i++)
		{
			if (soap_out_tt__WideDynamicMode(soap, "tt:Mode", -1, a->Mode + i, ""))
			{
				return soap->error;
			}
		}
	}
	if (soap_out_PointerTott__FloatRange(soap, "tt:Level", -1, &a->Level, ""))
	{
		return soap->error;
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__WideDynamicRangeOptions20 * SOAP_FMAC4 soap_in_tt__WideDynamicRangeOptions20(struct soap *soap, const char *tag, struct tt__WideDynamicRangeOptions20 *a, const char *type)
{
	struct soap_blist *soap_blist_Mode = NULL;
	size_t soap_flag_Level = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
	{
		return NULL;
	}
	a = (struct tt__WideDynamicRangeOptions20 *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__WideDynamicRangeOptions20, sizeof(struct tt__WideDynamicRangeOptions20), 0, NULL, NULL, NULL);
	if (NULL==a)
	{
		return NULL;
	}
	soap_default_tt__WideDynamicRangeOptions20(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	
			soap->error = SOAP_TAG_MISMATCH;
			if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tt:Mode", 1, NULL))
			{	
				if (a->Mode == NULL)
				{	
					if (soap_blist_Mode == NULL)
					{
						soap_blist_Mode = soap_new_block(soap);
					}
					a->Mode = (enum tt__WideDynamicMode *)soap_push_block(soap, soap_blist_Mode, sizeof(enum tt__WideDynamicMode));
					if (a->Mode == NULL)
					{
						return NULL;
					}
					soap_default_tt__WideDynamicMode(soap, a->Mode);
				}
				soap_revert(soap);
				if (soap_in_tt__WideDynamicMode(soap, "tt:Mode", a->Mode, "tt:WideDynamicMode"))
				{	
					a->__sizeMode++;
					a->Mode = NULL;
					continue;
				}
			}
			if (soap_flag_Level && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FloatRange(soap, "tt:Level", &a->Level, "tt:FloatRange"))
				{	
					soap_flag_Level--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
			{
				soap->error = soap_ignore_element(soap);
			}
			if (soap->error == SOAP_NO_TAG)
			{
				break;
			}
			if (soap->error)
			{
				return NULL;
			}
		}
		if (a->Mode != NULL)
		{
			soap_pop_block(soap, soap_blist_Mode);
		}
		if (a->__sizeMode)
		{
			a->Mode = (enum tt__WideDynamicMode *)soap_save_block(soap, soap_blist_Mode, NULL, 1);
		}
		else
		{	
			a->Mode = NULL;
			if (soap_blist_Mode)
			{
				soap_end_block(soap, soap_blist_Mode);
			}
		}
		if (soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	else
	{	
		a = (struct tt__WideDynamicRangeOptions20 *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__WideDynamicRangeOptions20, 0, sizeof(struct tt__WideDynamicRangeOptions20), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	if ((soap->mode & SOAP_XML_STRICT) && (a->__sizeMode < 1))
	{
		soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__WideDynamicRangeOptions20(struct soap *soap, const struct tt__WideDynamicRangeOptions20 *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__WideDynamicRangeOptions20);
	if (soap_out_tt__WideDynamicRangeOptions20(soap, tag?tag:"tt:WideDynamicRangeOptions20", id, a, type))
	{
		return soap->error;
	}
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__WideDynamicRangeOptions20 * SOAP_FMAC4 soap_get_tt__WideDynamicRangeOptions20(struct soap *soap, struct tt__WideDynamicRangeOptions20 *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__WideDynamicRangeOptions20(soap, tag, p, type)) != NULL)
	{
		if (soap_getindependent(soap))
		{
			return NULL;
		}
	}
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__WhiteBalanceOptions20(struct soap *soap, struct tt__WhiteBalanceOptions20 *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__WhiteBalanceOptions20))
	{
		soap_serialize_tt__WhiteBalanceOptions20(soap, *a);
	}
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__WhiteBalanceOptions20(struct soap *soap, const char *tag, int id, struct tt__WhiteBalanceOptions20 *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__WhiteBalanceOptions20);
	if (id < 0)
	{
		return soap->error;
	}
	return soap_out_tt__WhiteBalanceOptions20(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__WhiteBalanceOptions20 ** SOAP_FMAC4 soap_in_PointerTott__WhiteBalanceOptions20(struct soap *soap, const char *tag, struct tt__WhiteBalanceOptions20 **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
	{
		return NULL;
	}
	if (NULL==a)
	{
		if (NULL==(a = (struct tt__WhiteBalanceOptions20 **)soap_malloc(soap, sizeof(struct tt__WhiteBalanceOptions20 *))))
		{
			return NULL;
		}
	}
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	
		soap_revert(soap);
		if (NULL==(*a = soap_in_tt__WhiteBalanceOptions20(soap, tag, *a, type)))
		{
			return NULL;
		}
	}
	else
	{	
		a = (struct tt__WhiteBalanceOptions20 **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__WhiteBalanceOptions20, sizeof(struct tt__WhiteBalanceOptions20), 0);
		if (soap->body && soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__WhiteBalanceOptions20(struct soap *soap, struct tt__WhiteBalanceOptions20 *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__WhiteBalanceOptions20);
	if (soap_out_PointerTott__WhiteBalanceOptions20(soap, tag?tag:"tt:WhiteBalanceOptions20", id, a, type))
	{
		return soap->error;
	}
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__WhiteBalanceOptions20 ** SOAP_FMAC4 soap_get_PointerTott__WhiteBalanceOptions20(struct soap *soap, struct tt__WhiteBalanceOptions20 **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__WhiteBalanceOptions20(soap, tag, p, type)) != NULL)
	{
		if (soap_getindependent(soap))
		{
			return NULL;
		}
	}
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__WhiteBalanceOptions20(struct soap *soap, struct tt__WhiteBalanceOptions20 *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__sizeMode = 0;
	a->Mode = NULL;
	a->YrGain = NULL;
	a->YbGain = NULL;
	a->Extension = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__WhiteBalanceOptions20(struct soap *soap, const struct tt__WhiteBalanceOptions20 *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	if (a->Mode != NULL)
	{	
		int i;
		for (i = 0; i < a->__sizeMode; i++)
		{
			soap_embedded(soap, a->Mode + i, SOAP_TYPE_tt__WhiteBalanceMode);
		}
	}
	soap_serialize_PointerTott__FloatRange(soap, &a->YrGain);
	soap_serialize_PointerTott__FloatRange(soap, &a->YbGain);
	soap_serialize_PointerTott__WhiteBalanceOptions20Extension(soap, &a->Extension);
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__WhiteBalanceOptions20(struct soap *soap, const char *tag, int id, const struct tt__WhiteBalanceOptions20 *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__WhiteBalanceOptions20), type))
	{
		return soap->error;
	}
	if (a->Mode != NULL)
	{	
		int i;
		for (i = 0; i < a->__sizeMode; i++)
		{
			if (soap_out_tt__WhiteBalanceMode(soap, "tt:Mode", -1, a->Mode + i, ""))
			{
				return soap->error;
			}
		}
	}
	if (soap_out_PointerTott__FloatRange(soap, "tt:YrGain", -1, &a->YrGain, ""))
	{
		return soap->error;
	}
	if (soap_out_PointerTott__FloatRange(soap, "tt:YbGain", -1, &a->YbGain, ""))
	{
		return soap->error;
	}
	if (soap_out_PointerTott__WhiteBalanceOptions20Extension(soap, "tt:Extension", -1, &a->Extension, ""))
	{
		return soap->error;
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__WhiteBalanceOptions20 * SOAP_FMAC4 soap_in_tt__WhiteBalanceOptions20(struct soap *soap, const char *tag, struct tt__WhiteBalanceOptions20 *a, const char *type)
{
	struct soap_blist *soap_blist_Mode = NULL;
	size_t soap_flag_YrGain = 1;
	size_t soap_flag_YbGain = 1;
	size_t soap_flag_Extension = 1;
	if (soap_element_begin_in(soap, tag, 0, type))
	{
		return NULL;
	}
	a = (struct tt__WhiteBalanceOptions20 *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__WhiteBalanceOptions20, sizeof(struct tt__WhiteBalanceOptions20), 0, NULL, NULL, NULL);
	if (NULL==a)
	{
		return NULL;
	}
	soap_default_tt__WhiteBalanceOptions20(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	
			soap->error = SOAP_TAG_MISMATCH;
			if (soap->error == SOAP_TAG_MISMATCH && !soap_element_begin_in(soap, "tt:Mode", 1, NULL))
			{	
				if (a->Mode == NULL)
				{	
					if (soap_blist_Mode == NULL)
					{
						soap_blist_Mode = soap_new_block(soap);
					}
					a->Mode = (enum tt__WhiteBalanceMode *)soap_push_block(soap, soap_blist_Mode, sizeof(enum tt__WhiteBalanceMode));
					if (a->Mode == NULL)
					{
						return NULL;
					}
					soap_default_tt__WhiteBalanceMode(soap, a->Mode);
				}
				soap_revert(soap);
				if (soap_in_tt__WhiteBalanceMode(soap, "tt:Mode", a->Mode, "tt:WhiteBalanceMode"))
				{	
					a->__sizeMode++;
					a->Mode = NULL;
					continue;
				}
			}
			if (soap_flag_YrGain && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FloatRange(soap, "tt:YrGain", &a->YrGain, "tt:FloatRange"))
				{	
					soap_flag_YrGain--;
					continue;
				}
			if (soap_flag_YbGain && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__FloatRange(soap, "tt:YbGain", &a->YbGain, "tt:FloatRange"))
				{	
					soap_flag_YbGain--;
					continue;
				}
			if (soap_flag_Extension && soap->error == SOAP_TAG_MISMATCH)
				if (soap_in_PointerTott__WhiteBalanceOptions20Extension(soap, "tt:Extension", &a->Extension, "tt:WhiteBalanceOptions20Extension"))
				{	
					soap_flag_Extension--;
					continue;
				}
			if (soap->error == SOAP_TAG_MISMATCH)
			{
				soap->error = soap_ignore_element(soap);
			}
			if (soap->error == SOAP_NO_TAG)
			{
				break;
			}
			if (soap->error)
			{
				return NULL;
			}
		}
		if (a->Mode != NULL)
		{
			soap_pop_block(soap, soap_blist_Mode);
		}
		if (a->__sizeMode)
		{
			a->Mode = (enum tt__WhiteBalanceMode *)soap_save_block(soap, soap_blist_Mode, NULL, 1);
		}
		else
		{	
			a->Mode = NULL;
			if (soap_blist_Mode)
			{
				soap_end_block(soap, soap_blist_Mode);
			}
		}
		if (soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	else
	{	
		a = (struct tt__WhiteBalanceOptions20 *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__WhiteBalanceOptions20, 0, sizeof(struct tt__WhiteBalanceOptions20), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	if ((soap->mode & SOAP_XML_STRICT) && (a->__sizeMode < 1))
	{	
		soap->error = SOAP_OCCURS;
		return NULL;
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__WhiteBalanceOptions20(struct soap *soap, const struct tt__WhiteBalanceOptions20 *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__WhiteBalanceOptions20);
	if (soap_out_tt__WhiteBalanceOptions20(soap, tag?tag:"tt:WhiteBalanceOptions20", id, a, type))
	{
		return soap->error;
	}
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__WhiteBalanceOptions20 * SOAP_FMAC4 soap_get_tt__WhiteBalanceOptions20(struct soap *soap, struct tt__WhiteBalanceOptions20 *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__WhiteBalanceOptions20(soap, tag, p, type)) != NULL)
	{
		if (soap_getindependent(soap))
		{
			return NULL;
		}
	}
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__WhiteBalanceOptions20Extension(struct soap *soap, struct tt__WhiteBalanceOptions20Extension *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__WhiteBalanceOptions20Extension))
	{
		soap_serialize_tt__WhiteBalanceOptions20Extension(soap, *a);
	}
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__WhiteBalanceOptions20Extension(struct soap *soap, const char *tag, int id, struct tt__WhiteBalanceOptions20Extension *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__WhiteBalanceOptions20Extension);
	if (id < 0)
	{
		return soap->error;
	}
	return soap_out_tt__WhiteBalanceOptions20Extension(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__WhiteBalanceOptions20Extension ** SOAP_FMAC4 soap_in_PointerTott__WhiteBalanceOptions20Extension(struct soap *soap, const char *tag, struct tt__WhiteBalanceOptions20Extension **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
	{
		return NULL;
	}
	if (NULL==a)
	{
		if (NULL==(a = (struct tt__WhiteBalanceOptions20Extension **)soap_malloc(soap, sizeof(struct tt__WhiteBalanceOptions20Extension *))))
		{
			return NULL;
		}
	}
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	
		soap_revert(soap);
		if (NULL==(*a = soap_in_tt__WhiteBalanceOptions20Extension(soap, tag, *a, type)))
		{
			return NULL;
		}
	}
	else
	{	
		a = (struct tt__WhiteBalanceOptions20Extension **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__WhiteBalanceOptions20Extension, sizeof(struct tt__WhiteBalanceOptions20Extension), 0);
		if (soap->body && soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__WhiteBalanceOptions20Extension(struct soap *soap, struct tt__WhiteBalanceOptions20Extension *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__WhiteBalanceOptions20Extension);
	if (soap_out_PointerTott__WhiteBalanceOptions20Extension(soap, tag?tag:"tt:WhiteBalanceOptions20Extension", id, a, type))
	{
		return soap->error;
	}
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__WhiteBalanceOptions20Extension ** SOAP_FMAC4 soap_get_PointerTott__WhiteBalanceOptions20Extension(struct soap *soap, struct tt__WhiteBalanceOptions20Extension **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__WhiteBalanceOptions20Extension(soap, tag, p, type)) != NULL)
	{
		if (soap_getindependent(soap))
		{
			return NULL;
		}
	}
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__WhiteBalanceOptions20Extension(struct soap *soap, struct tt__WhiteBalanceOptions20Extension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__size = 0;
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__WhiteBalanceOptions20Extension(struct soap *soap, const struct tt__WhiteBalanceOptions20Extension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__WhiteBalanceOptions20Extension(struct soap *soap, const char *tag, int id, const struct tt__WhiteBalanceOptions20Extension *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__WhiteBalanceOptions20Extension), type))
	{
		return soap->error;
	}
	if (a->__any)
	{	
		int i;
		for (i = 0; i < a->__size; i++)
		{
			soap_outliteral(soap, "-any", a->__any + i, NULL);
		}
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__WhiteBalanceOptions20Extension * SOAP_FMAC4 soap_in_tt__WhiteBalanceOptions20Extension(struct soap *soap, const char *tag, struct tt__WhiteBalanceOptions20Extension *a, const char *type)
{
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
	{
		return NULL;
	}
	a = (struct tt__WhiteBalanceOptions20Extension *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__WhiteBalanceOptions20Extension, sizeof(struct tt__WhiteBalanceOptions20Extension), 0, NULL, NULL, NULL);
	if (NULL==a)
	{
		return NULL;
	}
	soap_default_tt__WhiteBalanceOptions20Extension(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{	
			soap->error = SOAP_TAG_MISMATCH;
			if (soap->error == SOAP_TAG_MISMATCH && !soap_peek_element(soap))
			{
				if (a->__any == NULL)
				{	
					if (soap_blist___any == NULL)
					{
						soap_blist___any = soap_new_block(soap);
					}
					a->__any = (char **)soap_push_block(soap, soap_blist___any, sizeof(char *));
					if (a->__any == NULL)
					{
						return NULL;
					}
					*a->__any = NULL;
				}
				if (soap_inliteral(soap, "-any", a->__any))
				{	
					a->__size++;
					a->__any = NULL;
					continue;
				}
			}
			if (soap->error == SOAP_TAG_MISMATCH)
			{
				soap->error = soap_ignore_element(soap);
			}
			if (soap->error == SOAP_NO_TAG)
			{
				break;
			}
			if (soap->error)
			{
				return NULL;
			}
		}
		if (a->__any != NULL)
		{
			soap_pop_block(soap, soap_blist___any);
		}
		if (a->__size)
		{
			a->__any = (char **)soap_save_block(soap, soap_blist___any, NULL, 1);
		}
		else
		{	
			a->__any = NULL;
			if (soap_blist___any)
			{
				soap_end_block(soap, soap_blist___any);
			}
		}
		if (soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	else
	{	
		a = (struct tt__WhiteBalanceOptions20Extension *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__WhiteBalanceOptions20Extension, 0, sizeof(struct tt__WhiteBalanceOptions20Extension), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__WhiteBalanceOptions20Extension(struct soap *soap, const struct tt__WhiteBalanceOptions20Extension *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__WhiteBalanceOptions20Extension);
	if (soap_out_tt__WhiteBalanceOptions20Extension(soap, tag?tag:"tt:WhiteBalanceOptions20Extension", id, a, type))
	{
		return soap->error;
	}
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__WhiteBalanceOptions20Extension * SOAP_FMAC4 soap_get_tt__WhiteBalanceOptions20Extension(struct soap *soap, struct tt__WhiteBalanceOptions20Extension *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__WhiteBalanceOptions20Extension(soap, tag, p, type)) !=NULL)
	{
		if (soap_getindependent(soap))
		{
			return NULL;
		}
	}
	return p;
}


SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__ImagingOptions20Extension(struct soap *soap, struct tt__ImagingOptions20Extension *const*a)
{
	if (!soap_reference(soap, *a, SOAP_TYPE_tt__ImagingOptions20Extension))
	{
		soap_serialize_tt__ImagingOptions20Extension(soap, *a);
	}
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__ImagingOptions20Extension(struct soap *soap, const char *tag, int id, struct tt__ImagingOptions20Extension *const*a, const char *type)
{
	id = soap_element_id(soap, tag, id, *a, NULL, 0, type, SOAP_TYPE_tt__ImagingOptions20Extension);
	if (id < 0)
	{
		return soap->error;
	}
	return soap_out_tt__ImagingOptions20Extension(soap, tag, id, *a, type);
}

SOAP_FMAC3 struct tt__ImagingOptions20Extension ** SOAP_FMAC4 soap_in_PointerTott__ImagingOptions20Extension(struct soap *soap, const char *tag, struct tt__ImagingOptions20Extension **a, const char *type)
{
	if (soap_element_begin_in(soap, tag, 1, NULL))
	{
		return NULL;
	}
	if (NULL==a)
	{
		if (NULL==(a = (struct tt__ImagingOptions20Extension **)soap_malloc(soap, sizeof(struct tt__ImagingOptions20Extension *))))
		{
			return NULL;
		}
	}
	*a = NULL;
	if (!soap->null && *soap->href != '#')
	{	
		soap_revert(soap);
		if (NULL==(*a = soap_in_tt__ImagingOptions20Extension(soap, tag, *a, type)))
		{
			return NULL;
		}
	}
	else
	{	
		a = (struct tt__ImagingOptions20Extension **)soap_id_lookup(soap, soap->href, (void**)a, SOAP_TYPE_tt__ImagingOptions20Extension, sizeof(struct tt__ImagingOptions20Extension), 0);
		if (soap->body && soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__ImagingOptions20Extension(struct soap *soap, struct tt__ImagingOptions20Extension *const*a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_PointerTott__ImagingOptions20Extension);
	if (soap_out_PointerTott__ImagingOptions20Extension(soap, tag?tag:"tt:ImagingOptions20Extension", id, a, type))
	{
		return soap->error;
	}
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__ImagingOptions20Extension ** SOAP_FMAC4 soap_get_PointerTott__ImagingOptions20Extension(struct soap *soap, struct tt__ImagingOptions20Extension **p, const char *tag, const char *type)
{
	if ((p = soap_in_PointerTott__ImagingOptions20Extension(soap, tag, p, type)) != NULL)
	{
		if (soap_getindependent(soap))
		{
			return NULL;
		}
	}
	return p;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__ImagingOptions20Extension(struct soap *soap, struct tt__ImagingOptions20Extension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
	a->__size = 0;
	a->__any = NULL;
}

SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__ImagingOptions20Extension(struct soap *soap, const struct tt__ImagingOptions20Extension *a)
{
	(void)soap; (void)a; /* appease -Wall -Werror */
}

SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__ImagingOptions20Extension(struct soap *soap, const char *tag, int id, const struct tt__ImagingOptions20Extension *a, const char *type)
{
	if (soap_element_begin_out(soap, tag, soap_embedded_id(soap, id, a, SOAP_TYPE_tt__ImagingOptions20Extension), type))
	{
		return soap->error;
	}
	if (a->__any != NULL)
	{	
		int i;
		for (i = 0; i < a->__size; i++)
		{
			soap_outliteral(soap, "-any", a->__any + i, NULL);
		}
	}
	return soap_element_end_out(soap, tag);
}

SOAP_FMAC3 struct tt__ImagingOptions20Extension * SOAP_FMAC4 soap_in_tt__ImagingOptions20Extension(struct soap *soap, const char *tag, struct tt__ImagingOptions20Extension *a, const char *type)
{
	struct soap_blist *soap_blist___any = NULL;
	if (soap_element_begin_in(soap, tag, 0, type))
	{
		return NULL;
	}
	a = (struct tt__ImagingOptions20Extension *)soap_id_enter(soap, soap->id, a, SOAP_TYPE_tt__ImagingOptions20Extension, sizeof(struct tt__ImagingOptions20Extension), 0, NULL, NULL, NULL);
	if (NULL==a)
	{
		return NULL;
	}
	soap_default_tt__ImagingOptions20Extension(soap, a);
	if (soap->body && !*soap->href)
	{
		for (;;)
		{
			soap->error = SOAP_TAG_MISMATCH;
			if (soap->error == SOAP_TAG_MISMATCH && !soap_peek_element(soap))
			{	
				if (a->__any == NULL)
				{	
					if (soap_blist___any == NULL)
					{
						soap_blist___any = soap_new_block(soap);
					}
					a->__any = (char **)soap_push_block(soap, soap_blist___any, sizeof(char *));
					if (a->__any == NULL)
					{
						return NULL;
					}
					*a->__any = NULL;
				}
				if (soap_inliteral(soap, "-any", a->__any))
				{	
					a->__size++;
					a->__any = NULL;
					continue;
				}
			}
			if (soap->error == SOAP_TAG_MISMATCH)
			{
				soap->error = soap_ignore_element(soap);
			}
			if (soap->error == SOAP_NO_TAG)
			{
				break;
			}
			if (soap->error)
			{
				return NULL;
			}
		}
		if (a->__any != NULL)
		{
			soap_pop_block(soap, soap_blist___any);
		}
		if (a->__size)
		{
			a->__any = (char **)soap_save_block(soap, soap_blist___any, NULL, 1);
		}
		else
		{	
			a->__any = NULL;
			if (soap_blist___any)
			{
				soap_end_block(soap, soap_blist___any);
			}

		}
		if (soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	else
	{	
		a = (struct tt__ImagingOptions20Extension *)soap_id_forward(soap, soap->href, (void*)a, 0, SOAP_TYPE_tt__ImagingOptions20Extension, 0, sizeof(struct tt__ImagingOptions20Extension), 0, NULL);
		if (soap->body && soap_element_end_in(soap, tag))
		{
			return NULL;
		}
	}
	return a;
}

SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__ImagingOptions20Extension(struct soap *soap, const struct tt__ImagingOptions20Extension *a, const char *tag, const char *type)
{
	register int id = soap_embed(soap, (void*)a, NULL, 0, tag, SOAP_TYPE_tt__ImagingOptions20Extension);
	if (soap_out_tt__ImagingOptions20Extension(soap, tag?tag:"tt:ImagingOptions20Extension", id, a, type))
	{
		return soap->error;
	}
	return soap_putindependent(soap);
}

SOAP_FMAC3 struct tt__ImagingOptions20Extension * SOAP_FMAC4 soap_get_tt__ImagingOptions20Extension(struct soap *soap, struct tt__ImagingOptions20Extension *p, const char *tag, const char *type)
{
	if ((p = soap_in_tt__ImagingOptions20Extension(soap, tag, p, type)) != NULL)
	{
		if (soap_getindependent(soap))
		{
			return NULL;
		}
	}
	return p;
}
#ifdef __cplusplus
}
#endif

#if defined(__BORLANDC__)
#pragma option pop
#pragma option pop
#endif