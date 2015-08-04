
#if defined(__BORLANDC__)
#pragma option push -w-8060
#pragma option push -w-8004
#endif
#include "OnvifPtzH.h"
#ifdef __cplusplus
extern "C" {
#endif

SOAP_SOURCE_STAMP("@(#) soapClient.c ver 2.8.3 2011-09-15 07:42:52 GMT")

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__GetPresets(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__GetPresets *tptz__GetPresets, struct _tptz__GetPresetsResponse *tptz__GetPresetsResponse)
{	
	struct __tptz__GetPresets soap_tmp___tptz__GetPresets;
	if (!soap_action)
		soap_action = "http://www.onvif.org/ver20/ptz/wsdl/GetPresets";
	soap->encodingStyle = NULL;
	soap_tmp___tptz__GetPresets.tptz__GetPresets = tptz__GetPresets;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tptz__GetPresets(soap, &soap_tmp___tptz__GetPresets);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___tptz__GetPresets(soap, &soap_tmp___tptz__GetPresets, "-tptz:GetPresets", NULL)
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
	 || soap_put___tptz__GetPresets(soap, &soap_tmp___tptz__GetPresets, "-tptz:GetPresets", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!tptz__GetPresetsResponse)
		return soap_closesock(soap);
	soap_default__tptz__GetPresetsResponse(soap, tptz__GetPresetsResponse);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tptz__GetPresetsResponse(soap, tptz__GetPresetsResponse, "tptz:GetPresetsResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__SetPreset(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__SetPreset *tptz__SetPreset, struct _tptz__SetPresetResponse *tptz__SetPresetResponse)
{	
	struct __tptz__SetPreset soap_tmp___tptz__SetPreset;
	if (NULL==soap_action)
	{
		soap_action = "http://www.onvif.org/ver20/ptz/wsdl/SetPreset";
	}
	soap->encodingStyle = NULL;
	soap_tmp___tptz__SetPreset.tptz__SetPreset = tptz__SetPreset;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tptz__SetPreset(soap, &soap_tmp___tptz__SetPreset);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___tptz__SetPreset(soap, &soap_tmp___tptz__SetPreset, "-tptz:SetPreset", NULL)
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
	 || soap_put___tptz__SetPreset(soap, &soap_tmp___tptz__SetPreset, "-tptz:SetPreset", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!tptz__SetPresetResponse)
		return soap_closesock(soap);
	soap_default__tptz__SetPresetResponse(soap, tptz__SetPresetResponse);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tptz__SetPresetResponse(soap, tptz__SetPresetResponse, "tptz:SetPresetResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__RemovePreset(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__RemovePreset *tptz__RemovePreset, struct _tptz__RemovePresetResponse *tptz__RemovePresetResponse)
{	struct __tptz__RemovePreset soap_tmp___tptz__RemovePreset;
	if (!soap_action)
		soap_action = "http://www.onvif.org/ver20/ptz/wsdl/RemovePreset";
	soap->encodingStyle = NULL;
	soap_tmp___tptz__RemovePreset.tptz__RemovePreset = tptz__RemovePreset;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tptz__RemovePreset(soap, &soap_tmp___tptz__RemovePreset);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___tptz__RemovePreset(soap, &soap_tmp___tptz__RemovePreset, "-tptz:RemovePreset", NULL)
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
	 || soap_put___tptz__RemovePreset(soap, &soap_tmp___tptz__RemovePreset, "-tptz:RemovePreset", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!tptz__RemovePresetResponse)
		return soap_closesock(soap);
	soap_default__tptz__RemovePresetResponse(soap, tptz__RemovePresetResponse);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tptz__RemovePresetResponse(soap, tptz__RemovePresetResponse, "tptz:RemovePresetResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__GotoPreset(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__GotoPreset *tptz__GotoPreset, struct _tptz__GotoPresetResponse *tptz__GotoPresetResponse)
{	struct __tptz__GotoPreset soap_tmp___tptz__GotoPreset;
	if (!soap_action)
		soap_action = "http://www.onvif.org/ver20/ptz/wsdl/GotoPreset";
	soap->encodingStyle = NULL;
	soap_tmp___tptz__GotoPreset.tptz__GotoPreset = tptz__GotoPreset;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tptz__GotoPreset(soap, &soap_tmp___tptz__GotoPreset);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___tptz__GotoPreset(soap, &soap_tmp___tptz__GotoPreset, "-tptz:GotoPreset", NULL)
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
	 || soap_put___tptz__GotoPreset(soap, &soap_tmp___tptz__GotoPreset, "-tptz:GotoPreset", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!tptz__GotoPresetResponse)
		return soap_closesock(soap);
	soap_default__tptz__GotoPresetResponse(soap, tptz__GotoPresetResponse);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tptz__GotoPresetResponse(soap, tptz__GotoPresetResponse, "tptz:GotoPresetResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__GetNode(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__GetNode *tptz__GetNode, struct _tptz__GetNodeResponse *tptz__GetNodeResponse)
{	struct __tptz__GetNode soap_tmp___tptz__GetNode;
	if (!soap_action)
		soap_action = "http://www.onvif.org/ver20/ptz/wsdl/GetNode";
	soap->encodingStyle = NULL;
	soap_tmp___tptz__GetNode.tptz__GetNode = tptz__GetNode;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tptz__GetNode(soap, &soap_tmp___tptz__GetNode);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___tptz__GetNode(soap, &soap_tmp___tptz__GetNode, "-tptz:GetNode", NULL)
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
	 || soap_put___tptz__GetNode(soap, &soap_tmp___tptz__GetNode, "-tptz:GetNode", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!tptz__GetNodeResponse)
		return soap_closesock(soap);
	soap_default__tptz__GetNodeResponse(soap, tptz__GetNodeResponse);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tptz__GetNodeResponse(soap, tptz__GetNodeResponse, "tptz:GetNodeResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}


SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__ContinuousMove(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__ContinuousMove *tptz__ContinuousMove, struct _tptz__ContinuousMoveResponse *tptz__ContinuousMoveResponse)
{	struct __tptz__ContinuousMove soap_tmp___tptz__ContinuousMove;
	if (!soap_action)
		soap_action = "http://www.onvif.org/ver20/ptz/wsdl/ContinuousMove";
	soap->encodingStyle = NULL;
	soap_tmp___tptz__ContinuousMove.tptz__ContinuousMove = tptz__ContinuousMove;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tptz__ContinuousMove(soap, &soap_tmp___tptz__ContinuousMove);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___tptz__ContinuousMove(soap, &soap_tmp___tptz__ContinuousMove, "-tptz:ContinuousMove", NULL)
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
	 || soap_put___tptz__ContinuousMove(soap, &soap_tmp___tptz__ContinuousMove, "-tptz:ContinuousMove", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!tptz__ContinuousMoveResponse)
		return soap_closesock(soap);
	soap_default__tptz__ContinuousMoveResponse(soap, tptz__ContinuousMoveResponse);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tptz__ContinuousMoveResponse(soap, tptz__ContinuousMoveResponse, "tptz:ContinuousMoveResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__RelativeMove(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__RelativeMove *tptz__RelativeMove, struct _tptz__RelativeMoveResponse *tptz__RelativeMoveResponse)
{	struct __tptz__RelativeMove soap_tmp___tptz__RelativeMove;
	if (!soap_action)
		soap_action = "http://www.onvif.org/ver20/ptz/wsdl/RelativeMove";
	soap->encodingStyle = NULL;
	soap_tmp___tptz__RelativeMove.tptz__RelativeMove = tptz__RelativeMove;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tptz__RelativeMove(soap, &soap_tmp___tptz__RelativeMove);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___tptz__RelativeMove(soap, &soap_tmp___tptz__RelativeMove, "-tptz:RelativeMove", NULL)
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
	 || soap_put___tptz__RelativeMove(soap, &soap_tmp___tptz__RelativeMove, "-tptz:RelativeMove", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!tptz__RelativeMoveResponse)
		return soap_closesock(soap);
	soap_default__tptz__RelativeMoveResponse(soap, tptz__RelativeMoveResponse);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tptz__RelativeMoveResponse(soap, tptz__RelativeMoveResponse, "tptz:RelativeMoveResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__SendAuxiliaryCommand(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__SendAuxiliaryCommand *tptz__SendAuxiliaryCommand, struct _tptz__SendAuxiliaryCommandResponse *tptz__SendAuxiliaryCommandResponse)
{	struct __tptz__SendAuxiliaryCommand soap_tmp___tptz__SendAuxiliaryCommand;
	if (!soap_action)
		soap_action = "http://www.onvif.org/ver20/ptz/wsdl/SendAuxiliaryCommand";
	soap->encodingStyle = NULL;
	soap_tmp___tptz__SendAuxiliaryCommand.tptz__SendAuxiliaryCommand = tptz__SendAuxiliaryCommand;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tptz__SendAuxiliaryCommand(soap, &soap_tmp___tptz__SendAuxiliaryCommand);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___tptz__SendAuxiliaryCommand(soap, &soap_tmp___tptz__SendAuxiliaryCommand, "-tptz:SendAuxiliaryCommand", NULL)
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
	 || soap_put___tptz__SendAuxiliaryCommand(soap, &soap_tmp___tptz__SendAuxiliaryCommand, "-tptz:SendAuxiliaryCommand", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!tptz__SendAuxiliaryCommandResponse)
		return soap_closesock(soap);
	soap_default__tptz__SendAuxiliaryCommandResponse(soap, tptz__SendAuxiliaryCommandResponse);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tptz__SendAuxiliaryCommandResponse(soap, tptz__SendAuxiliaryCommandResponse, "tptz:SendAuxiliaryCommandResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__AbsoluteMove(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__AbsoluteMove *tptz__AbsoluteMove, struct _tptz__AbsoluteMoveResponse *tptz__AbsoluteMoveResponse)
{	struct __tptz__AbsoluteMove soap_tmp___tptz__AbsoluteMove;
	if (!soap_action)
		soap_action = "http://www.onvif.org/ver20/ptz/wsdl/AbsoluteMove";
	soap->encodingStyle = NULL;
	soap_tmp___tptz__AbsoluteMove.tptz__AbsoluteMove = tptz__AbsoluteMove;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tptz__AbsoluteMove(soap, &soap_tmp___tptz__AbsoluteMove);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___tptz__AbsoluteMove(soap, &soap_tmp___tptz__AbsoluteMove, "-tptz:AbsoluteMove", NULL)
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
	 || soap_put___tptz__AbsoluteMove(soap, &soap_tmp___tptz__AbsoluteMove, "-tptz:AbsoluteMove", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!tptz__AbsoluteMoveResponse)
		return soap_closesock(soap);
	soap_default__tptz__AbsoluteMoveResponse(soap, tptz__AbsoluteMoveResponse);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tptz__AbsoluteMoveResponse(soap, tptz__AbsoluteMoveResponse, "tptz:AbsoluteMoveResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__Stop(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__Stop *tptz__Stop, struct _tptz__StopResponse *tptz__StopResponse)
{	struct __tptz__Stop soap_tmp___tptz__Stop;
	if (!soap_action)
		soap_action = "http://www.onvif.org/ver20/ptz/wsdl/Stop";
	soap->encodingStyle = NULL;
	soap_tmp___tptz__Stop.tptz__Stop = tptz__Stop;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tptz__Stop(soap, &soap_tmp___tptz__Stop);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___tptz__Stop(soap, &soap_tmp___tptz__Stop, "-tptz:Stop", NULL)
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
	 || soap_put___tptz__Stop(soap, &soap_tmp___tptz__Stop, "-tptz:Stop", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!tptz__StopResponse)
		return soap_closesock(soap);
	soap_default__tptz__StopResponse(soap, tptz__StopResponse);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tptz__StopResponse(soap, tptz__StopResponse, "tptz:StopResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__GetPresetTours(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__GetPresetTours *tptz__GetPresetTours, struct _tptz__GetPresetToursResponse *tptz__GetPresetToursResponse)
{	struct __tptz__GetPresetTours soap_tmp___tptz__GetPresetTours;
	if (!soap_action)
		soap_action = "http://www.onvif.org/ver20/ptz/wsdl/GetPresetTours";
	soap->encodingStyle = NULL;
	soap_tmp___tptz__GetPresetTours.tptz__GetPresetTours = tptz__GetPresetTours;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tptz__GetPresetTours(soap, &soap_tmp___tptz__GetPresetTours);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___tptz__GetPresetTours(soap, &soap_tmp___tptz__GetPresetTours, "-tptz:GetPresetTours", NULL)
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
	 || soap_put___tptz__GetPresetTours(soap, &soap_tmp___tptz__GetPresetTours, "-tptz:GetPresetTours", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!tptz__GetPresetToursResponse)
		return soap_closesock(soap);
	soap_default__tptz__GetPresetToursResponse(soap, tptz__GetPresetToursResponse);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tptz__GetPresetToursResponse(soap, tptz__GetPresetToursResponse, "tptz:GetPresetToursResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__GetPresetTour(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__GetPresetTour *tptz__GetPresetTour, struct _tptz__GetPresetTourResponse *tptz__GetPresetTourResponse)
{	struct __tptz__GetPresetTour soap_tmp___tptz__GetPresetTour;
	if (!soap_action)
		soap_action = "http://www.onvif.org/ver20/ptz/wsdl/GetPresetTour";
	soap->encodingStyle = NULL;
	soap_tmp___tptz__GetPresetTour.tptz__GetPresetTour = tptz__GetPresetTour;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tptz__GetPresetTour(soap, &soap_tmp___tptz__GetPresetTour);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___tptz__GetPresetTour(soap, &soap_tmp___tptz__GetPresetTour, "-tptz:GetPresetTour", NULL)
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
	 || soap_put___tptz__GetPresetTour(soap, &soap_tmp___tptz__GetPresetTour, "-tptz:GetPresetTour", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!tptz__GetPresetTourResponse)
		return soap_closesock(soap);
	soap_default__tptz__GetPresetTourResponse(soap, tptz__GetPresetTourResponse);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tptz__GetPresetTourResponse(soap, tptz__GetPresetTourResponse, "tptz:GetPresetTourResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

//SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__GetPresetTourOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__GetPresetTourOptions *tptz__GetPresetTourOptions, struct _tptz__GetPresetTourOptionsResponse *tptz__GetPresetTourOptionsResponse)
//{	struct __tptz__GetPresetTourOptions soap_tmp___tptz__GetPresetTourOptions;
//	if (!soap_action)
//		soap_action = "http://www.onvif.org/ver20/ptz/wsdl/GetPresetTourOptions";
//	soap->encodingStyle = NULL;
//	soap_tmp___tptz__GetPresetTourOptions.tptz__GetPresetTourOptions = tptz__GetPresetTourOptions;
//	soap_begin(soap);
//	soap_serializeheader(soap);
//	soap_serialize___tptz__GetPresetTourOptions(soap, &soap_tmp___tptz__GetPresetTourOptions);
//	if (soap_begin_count(soap))
//		return soap->error;
//	if (soap->mode & SOAP_IO_LENGTH)
//	{	if (soap_envelope_begin_out(soap)
//		 || soap_putheader(soap)
//		 || soap_body_begin_out(soap)
//		 || soap_put___tptz__GetPresetTourOptions(soap, &soap_tmp___tptz__GetPresetTourOptions, "-tptz:GetPresetTourOptions", NULL)
//		 || soap_body_end_out(soap)
//		 || soap_envelope_end_out(soap))
//			 return soap->error;
//	}
//	if (soap_end_count(soap))
//		return soap->error;
//	if (soap_connect(soap, soap_endpoint, soap_action)
//	 || soap_envelope_begin_out(soap)
//	 || soap_putheader(soap)
//	 || soap_body_begin_out(soap)
//	 || soap_put___tptz__GetPresetTourOptions(soap, &soap_tmp___tptz__GetPresetTourOptions, "-tptz:GetPresetTourOptions", NULL)
//	 || soap_body_end_out(soap)
//	 || soap_envelope_end_out(soap)
//	 || soap_end_send(soap))
//		return soap_closesock(soap);
//	if (!tptz__GetPresetTourOptionsResponse)
//		return soap_closesock(soap);
//	soap_default__tptz__GetPresetTourOptionsResponse(soap, tptz__GetPresetTourOptionsResponse);
//	if (soap_begin_recv(soap)
//	 || soap_envelope_begin_in(soap)
//	 || soap_recv_header(soap)
//	 || soap_body_begin_in(soap))
//		return soap_closesock(soap);
//	soap_get__tptz__GetPresetTourOptionsResponse(soap, tptz__GetPresetTourOptionsResponse, "tptz:GetPresetTourOptionsResponse", "");
//	if (soap->error)
//		return soap_recv_fault(soap, 0);
//	if (soap_body_end_in(soap)
//	 || soap_envelope_end_in(soap)
//	 || soap_end_recv(soap))
//		return soap_closesock(soap);
//	return soap_closesock(soap);
//}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__CreatePresetTour(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__CreatePresetTour *tptz__CreatePresetTour, struct _tptz__CreatePresetTourResponse *tptz__CreatePresetTourResponse)
{	struct __tptz__CreatePresetTour soap_tmp___tptz__CreatePresetTour;
	if (!soap_action)
		soap_action = "http://www.onvif.org/ver20/ptz/wsdl/CreatePresetTour";
	soap->encodingStyle = NULL;
	soap_tmp___tptz__CreatePresetTour.tptz__CreatePresetTour = tptz__CreatePresetTour;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tptz__CreatePresetTour(soap, &soap_tmp___tptz__CreatePresetTour);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___tptz__CreatePresetTour(soap, &soap_tmp___tptz__CreatePresetTour, "-tptz:CreatePresetTour", NULL)
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
	 || soap_put___tptz__CreatePresetTour(soap, &soap_tmp___tptz__CreatePresetTour, "-tptz:CreatePresetTour", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!tptz__CreatePresetTourResponse)
		return soap_closesock(soap);
	soap_default__tptz__CreatePresetTourResponse(soap, tptz__CreatePresetTourResponse);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tptz__CreatePresetTourResponse(soap, tptz__CreatePresetTourResponse, "tptz:CreatePresetTourResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__ModifyPresetTour(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__ModifyPresetTour *tptz__ModifyPresetTour, struct _tptz__ModifyPresetTourResponse *tptz__ModifyPresetTourResponse)
{	struct __tptz__ModifyPresetTour soap_tmp___tptz__ModifyPresetTour;
	if (!soap_action)
		soap_action = "http://www.onvif.org/ver20/ptz/wsdl/ModifyPresetTour";
	soap->encodingStyle = NULL;
	soap_tmp___tptz__ModifyPresetTour.tptz__ModifyPresetTour = tptz__ModifyPresetTour;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tptz__ModifyPresetTour(soap, &soap_tmp___tptz__ModifyPresetTour);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___tptz__ModifyPresetTour(soap, &soap_tmp___tptz__ModifyPresetTour, "-tptz:ModifyPresetTour", NULL)
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
	 || soap_put___tptz__ModifyPresetTour(soap, &soap_tmp___tptz__ModifyPresetTour, "-tptz:ModifyPresetTour", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!tptz__ModifyPresetTourResponse)
		return soap_closesock(soap);
	soap_default__tptz__ModifyPresetTourResponse(soap, tptz__ModifyPresetTourResponse);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tptz__ModifyPresetTourResponse(soap, tptz__ModifyPresetTourResponse, "tptz:ModifyPresetTourResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);

	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__OperatePresetTour(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__OperatePresetTour *tptz__OperatePresetTour, struct _tptz__OperatePresetTourResponse *tptz__OperatePresetTourResponse)
{	struct __tptz__OperatePresetTour soap_tmp___tptz__OperatePresetTour;
	if (!soap_action)
		soap_action = "http://www.onvif.org/ver20/ptz/wsdl/OperatePresetTour";
	soap->encodingStyle = NULL;
	soap_tmp___tptz__OperatePresetTour.tptz__OperatePresetTour = tptz__OperatePresetTour;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tptz__OperatePresetTour(soap, &soap_tmp___tptz__OperatePresetTour);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___tptz__OperatePresetTour(soap, &soap_tmp___tptz__OperatePresetTour, "-tptz:OperatePresetTour", NULL)
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
	 || soap_put___tptz__OperatePresetTour(soap, &soap_tmp___tptz__OperatePresetTour, "-tptz:OperatePresetTour", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!tptz__OperatePresetTourResponse)
		return soap_closesock(soap);
	soap_default__tptz__OperatePresetTourResponse(soap, tptz__OperatePresetTourResponse);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tptz__OperatePresetTourResponse(soap, tptz__OperatePresetTourResponse, "tptz:OperatePresetTourResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tptz__RemovePresetTour(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tptz__RemovePresetTour *tptz__RemovePresetTour, struct _tptz__RemovePresetTourResponse *tptz__RemovePresetTourResponse)
{	struct __tptz__RemovePresetTour soap_tmp___tptz__RemovePresetTour;
	if (!soap_action)
		soap_action = "http://www.onvif.org/ver20/ptz/wsdl/RemovePresetTour";
	soap->encodingStyle = NULL;
	soap_tmp___tptz__RemovePresetTour.tptz__RemovePresetTour = tptz__RemovePresetTour;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tptz__RemovePresetTour(soap, &soap_tmp___tptz__RemovePresetTour);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___tptz__RemovePresetTour(soap, &soap_tmp___tptz__RemovePresetTour, "-tptz:RemovePresetTour", NULL)
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
	 || soap_put___tptz__RemovePresetTour(soap, &soap_tmp___tptz__RemovePresetTour, "-tptz:RemovePresetTour", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!tptz__RemovePresetTourResponse)
		return soap_closesock(soap);
	soap_default__tptz__RemovePresetTourResponse(soap, tptz__RemovePresetTourResponse);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tptz__RemovePresetTourResponse(soap, tptz__RemovePresetTourResponse, "tptz:RemovePresetTourResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___timg__Move(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__Move *timg__Move, struct _timg__MoveResponse *timg__MoveResponse)
{	struct __timg__Move soap_tmp___timg__Move;
	if (!soap_action)
		soap_action = "http://www.onvif.org/ver20/imaging/wsdl/Move";
	soap->encodingStyle = NULL;
	soap_tmp___timg__Move.timg__Move = timg__Move;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___timg__Move(soap, &soap_tmp___timg__Move);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___timg__Move(soap, &soap_tmp___timg__Move, "-timg:Move", NULL)
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
	 || soap_put___timg__Move(soap, &soap_tmp___timg__Move, "-timg:Move", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!timg__MoveResponse)
		return soap_closesock(soap);
	soap_default__timg__MoveResponse(soap, timg__MoveResponse);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__timg__MoveResponse(soap, timg__MoveResponse, "timg:MoveResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___timg__Stop(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__Stop *timg__Stop, struct _timg__StopResponse *timg__StopResponse)
{	struct __timg__Stop soap_tmp___timg__Stop;
	if (!soap_action)
		soap_action = "http://www.onvif.org/ver20/imaging/wsdl/FocusStop";
	soap->encodingStyle = NULL;
	soap_tmp___timg__Stop.timg__Stop = timg__Stop;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___timg__Stop(soap, &soap_tmp___timg__Stop);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___timg__Stop(soap, &soap_tmp___timg__Stop, "-timg:Stop", NULL)
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
	 || soap_put___timg__Stop(soap, &soap_tmp___timg__Stop, "-timg:Stop", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!timg__StopResponse)
		return soap_closesock(soap);
	soap_default__timg__StopResponse(soap, timg__StopResponse);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__timg__StopResponse(soap, timg__StopResponse, "timg:StopResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___timg__GetMoveOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__GetMoveOptions *timg__GetMoveOptions, struct _timg__GetMoveOptionsResponse *timg__GetMoveOptionsResponse)
{	struct __timg__GetMoveOptions soap_tmp___timg__GetMoveOptions;
	if (!soap_action)
		soap_action = "http://www.onvif.org/ver20/imaging/wsdl/GetMoveOptions";
	soap->encodingStyle = NULL;
	soap_tmp___timg__GetMoveOptions.timg__GetMoveOptions = timg__GetMoveOptions;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___timg__GetMoveOptions(soap, &soap_tmp___timg__GetMoveOptions);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put___timg__GetMoveOptions(soap, &soap_tmp___timg__GetMoveOptions, "-timg:GetMoveOptions", NULL)
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
	 || soap_put___timg__GetMoveOptions(soap, &soap_tmp___timg__GetMoveOptions, "-timg:GetMoveOptions", NULL)
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap_closesock(soap);
	if (!timg__GetMoveOptionsResponse)
		return soap_closesock(soap);
	soap_default__timg__GetMoveOptionsResponse(soap, timg__GetMoveOptionsResponse);
	if (soap_begin_recv(soap)
	 || soap_envelope_begin_in(soap)
	 || soap_recv_header(soap)
	 || soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__timg__GetMoveOptionsResponse(soap, timg__GetMoveOptionsResponse, "timg:GetMoveOptionsResponse", "");
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

/* End of soapClient.c */
