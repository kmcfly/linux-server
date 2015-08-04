#if defined(__BORLANDC__)
#pragma option push -w-8060
#pragma option push -w-8004
#endif

#include "OnvifSoapH.h"

#ifdef __cplusplus
extern "C"
{
#endif

//device management
SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetCapabilities(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetCapabilities *tds__GetCapabilities, struct _tds__GetCapabilitiesResponse *tds__GetCapabilitiesResponse)
{	
	struct __tds__GetCapabilities soap_tmp___tds__GetCapabilities;
	if (soap_action == NULL)
		soap_action = "http://www.onvif.org/ver10/device/wsdl/GetCapabilities";
	soap->encodingStyle = NULL;
	soap_tmp___tds__GetCapabilities.tds__GetCapabilities = tds__GetCapabilities;
	soap_begin(soap);

	soap_serializeheader(soap);

	soap_serialize___tds__GetCapabilities(soap, &soap_tmp___tds__GetCapabilities);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__GetCapabilities(soap, &soap_tmp___tds__GetCapabilities, "-tds:GetCapabilities", NULL)
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
		|| soap_put___tds__GetCapabilities(soap, &soap_tmp___tds__GetCapabilities, "-tds:GetCapabilities", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tds__GetCapabilitiesResponse)
		return soap_closesock(soap);
	
	soap_default__tds__GetCapabilitiesResponse(soap, tds__GetCapabilitiesResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tds__GetCapabilitiesResponse(soap, tds__GetCapabilitiesResponse, "tds:GetCapabilitiesResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetDeviceInformation(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetDeviceInformation *tds__GetDeviceInformation, struct _tds__GetDeviceInformationResponse *tds__GetDeviceInformationResponse)
{	
	struct __tds__GetDeviceInformation soap_tmp___tds__GetDeviceInformation;
	if (!soap_action)
		soap_action = "http://www.onvif.org/ver10/device/wsdl/GetDeviceInformation";
	soap->encodingStyle = NULL;
	soap_tmp___tds__GetDeviceInformation.tds__GetDeviceInformation = tds__GetDeviceInformation;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__GetDeviceInformation(soap, &soap_tmp___tds__GetDeviceInformation);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__GetDeviceInformation(soap, &soap_tmp___tds__GetDeviceInformation, "-tds:GetDeviceInformation", NULL)
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
		|| soap_put___tds__GetDeviceInformation(soap, &soap_tmp___tds__GetDeviceInformation, "-tds:GetDeviceInformation", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tds__GetDeviceInformationResponse)
		return soap_closesock(soap);
	soap_default__tds__GetDeviceInformationResponse(soap, tds__GetDeviceInformationResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tds__GetDeviceInformationResponse(soap, tds__GetDeviceInformationResponse, "tds:GetDeviceInformationResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetSystemDateAndTime(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetSystemDateAndTime *tds__GetSystemDateAndTime, struct _tds__GetSystemDateAndTimeResponse *tds__GetSystemDateAndTimeResponse)
{	
	struct __tds__GetSystemDateAndTime soap_tmp___tds__GetSystemDateAndTime;
	if (!soap_action)
		soap_action = "http://www.onvif.org/ver10/device/wsdl/GetSystemDateAndTime";
	soap->encodingStyle = NULL;
	soap_tmp___tds__GetSystemDateAndTime.tds__GetSystemDateAndTime = tds__GetSystemDateAndTime;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__GetSystemDateAndTime(soap, &soap_tmp___tds__GetSystemDateAndTime);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__GetSystemDateAndTime(soap, &soap_tmp___tds__GetSystemDateAndTime, "-tds:GetSystemDateAndTime", NULL)
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
		|| soap_put___tds__GetSystemDateAndTime(soap, &soap_tmp___tds__GetSystemDateAndTime, "-tds:GetSystemDateAndTime", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tds__GetSystemDateAndTimeResponse)
		return soap_closesock(soap);
	soap_default__tds__GetSystemDateAndTimeResponse(soap, tds__GetSystemDateAndTimeResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tds__GetSystemDateAndTimeResponse(soap, tds__GetSystemDateAndTimeResponse, "tds:GetSystemDateAndTimeResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetSystemDateAndTime(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetSystemDateAndTime *tds__SetSystemDateAndTime, struct _tds__SetSystemDateAndTimeResponse *tds__SetSystemDateAndTimeResponse)
{	
	struct __tds__SetSystemDateAndTime soap_tmp___tds__SetSystemDateAndTime;
	if (soap_action == NULL)
		soap_action = "http://www.onvif.org/ver10/device/wsdl/SetSystemDateAndTime";
	soap->encodingStyle = NULL;
	soap_tmp___tds__SetSystemDateAndTime.tds__SetSystemDateAndTime = tds__SetSystemDateAndTime;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__SetSystemDateAndTime(soap, &soap_tmp___tds__SetSystemDateAndTime);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__SetSystemDateAndTime(soap, &soap_tmp___tds__SetSystemDateAndTime, "-tds:SetSystemDateAndTime", NULL)
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
		|| soap_put___tds__SetSystemDateAndTime(soap, &soap_tmp___tds__SetSystemDateAndTime, "-tds:SetSystemDateAndTime", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tds__SetSystemDateAndTimeResponse)
		return soap_closesock(soap);
	soap_default__tds__SetSystemDateAndTimeResponse(soap, tds__SetSystemDateAndTimeResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tds__SetSystemDateAndTimeResponse(soap, tds__SetSystemDateAndTimeResponse, "tds:SetSystemDateAndTimeResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SystemReboot(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SystemReboot *tds__SystemReboot, struct _tds__SystemRebootResponse *tds__SystemRebootResponse)
{	
	struct __tds__SystemReboot soap_tmp___tds__SystemReboot;
	if (!soap_action)
		soap_action = "http://www.onvif.org/ver10/device/wsdl/SystemReboot";
	soap->encodingStyle = NULL;
	soap_tmp___tds__SystemReboot.tds__SystemReboot = tds__SystemReboot;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__SystemReboot(soap, &soap_tmp___tds__SystemReboot);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__SystemReboot(soap, &soap_tmp___tds__SystemReboot, "-tds:SystemReboot", NULL)
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
		|| soap_put___tds__SystemReboot(soap, &soap_tmp___tds__SystemReboot, "-tds:SystemReboot", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tds__SystemRebootResponse)
		return soap_closesock(soap);
	soap_default__tds__SystemRebootResponse(soap, tds__SystemRebootResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tds__SystemRebootResponse(soap, tds__SystemRebootResponse, "tds:SystemRebootResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetSystemFactoryDefault(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetSystemFactoryDefault *tds__SetSystemFactoryDefault, struct _tds__SetSystemFactoryDefaultResponse *tds__SetSystemFactoryDefaultResponse)
{	
	struct __tds__SetSystemFactoryDefault soap_tmp___tds__SetSystemFactoryDefault;
	if (!soap_action)
		soap_action = "http://www.onvif.org/ver10/device/wsdl/SetSystemFactoryDefault";
	soap->encodingStyle = NULL;
	soap_tmp___tds__SetSystemFactoryDefault.tds__SetSystemFactoryDefault = tds__SetSystemFactoryDefault;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__SetSystemFactoryDefault(soap, &soap_tmp___tds__SetSystemFactoryDefault);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__SetSystemFactoryDefault(soap, &soap_tmp___tds__SetSystemFactoryDefault, "-tds:SetSystemFactoryDefault", NULL)
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
		|| soap_put___tds__SetSystemFactoryDefault(soap, &soap_tmp___tds__SetSystemFactoryDefault, "-tds:SetSystemFactoryDefault", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tds__SetSystemFactoryDefaultResponse)
		return soap_closesock(soap);
	soap_default__tds__SetSystemFactoryDefaultResponse(soap, tds__SetSystemFactoryDefaultResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tds__SetSystemFactoryDefaultResponse(soap, tds__SetSystemFactoryDefaultResponse, "tds:SetSystemFactoryDefaultResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetUsers(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetUsers *tds__GetUsers, struct _tds__GetUsersResponse *tds__GetUsersResponse)
{	
	struct __tds__GetUsers soap_tmp___tds__GetUsers;
	if (!soap_action)
		soap_action = "http://www.onvif.org/ver10/device/wsdl/GetUsers";
	soap->encodingStyle = NULL;
	soap_tmp___tds__GetUsers.tds__GetUsers = tds__GetUsers;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__GetUsers(soap, &soap_tmp___tds__GetUsers);
	if (soap_begin_count(soap))
	return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__GetUsers(soap, &soap_tmp___tds__GetUsers, "-tds:GetUsers", NULL)
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
		|| soap_put___tds__GetUsers(soap, &soap_tmp___tds__GetUsers, "-tds:GetUsers", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tds__GetUsersResponse)
		return soap_closesock(soap);
	soap_default__tds__GetUsersResponse(soap, tds__GetUsersResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tds__GetUsersResponse(soap, tds__GetUsersResponse, "tds:GetUsersResponse", "");
	if (soap->error)
	return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__CreateUsers(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__CreateUsers *tds__CreateUsers, struct _tds__CreateUsersResponse *tds__CreateUsersResponse)
{	
	struct __tds__CreateUsers soap_tmp___tds__CreateUsers;
	if (soap_action == NULL)
		soap_action = "http://www.onvif.org/ver10/device/wsdl/CreateUsers";
	soap->encodingStyle = NULL;
	soap_tmp___tds__CreateUsers.tds__CreateUsers = tds__CreateUsers;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__CreateUsers(soap, &soap_tmp___tds__CreateUsers);
	if (soap_begin_count(soap))
	return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__CreateUsers(soap, &soap_tmp___tds__CreateUsers, "-tds:CreateUsers", NULL)
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
		|| soap_put___tds__CreateUsers(soap, &soap_tmp___tds__CreateUsers, "-tds:CreateUsers", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tds__CreateUsersResponse)
		return soap_closesock(soap);
	soap_default__tds__CreateUsersResponse(soap, tds__CreateUsersResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tds__CreateUsersResponse(soap, tds__CreateUsersResponse, "tds:CreateUsersResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__DeleteUsers(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__DeleteUsers *tds__DeleteUsers, struct _tds__DeleteUsersResponse *tds__DeleteUsersResponse)
{	
	struct __tds__DeleteUsers soap_tmp___tds__DeleteUsers;
	if (soap_action == NULL)
		soap_action = "http://www.onvif.org/ver10/device/wsdl/DeleteUsers";
	soap->encodingStyle = NULL;
	soap_tmp___tds__DeleteUsers.tds__DeleteUsers = tds__DeleteUsers;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__DeleteUsers(soap, &soap_tmp___tds__DeleteUsers);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__DeleteUsers(soap, &soap_tmp___tds__DeleteUsers, "-tds:DeleteUsers", NULL)
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
		|| soap_put___tds__DeleteUsers(soap, &soap_tmp___tds__DeleteUsers, "-tds:DeleteUsers", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tds__DeleteUsersResponse)
	return soap_closesock(soap);
	soap_default__tds__DeleteUsersResponse(soap, tds__DeleteUsersResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tds__DeleteUsersResponse(soap, tds__DeleteUsersResponse, "tds:DeleteUsersResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetUser(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetUser *tds__SetUser, struct _tds__SetUserResponse *tds__SetUserResponse)
{	
	struct __tds__SetUser soap_tmp___tds__SetUser;
	if (soap_action == NULL)
		soap_action = "http://www.onvif.org/ver10/device/wsdl/SetUser";
	soap->encodingStyle = NULL;
	soap_tmp___tds__SetUser.tds__SetUser = tds__SetUser;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__SetUser(soap, &soap_tmp___tds__SetUser);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__SetUser(soap, &soap_tmp___tds__SetUser, "-tds:SetUser", NULL)
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
		|| soap_put___tds__SetUser(soap, &soap_tmp___tds__SetUser, "-tds:SetUser", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tds__SetUserResponse)
		return soap_closesock(soap);
	soap_default__tds__SetUserResponse(soap, tds__SetUserResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tds__SetUserResponse(soap, tds__SetUserResponse, "tds:SetUserResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

//device network
SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetNetworkInterfaces(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetNetworkInterfaces *tds__GetNetworkInterfaces, struct _tds__GetNetworkInterfacesResponse *tds__GetNetworkInterfacesResponse)
{	
	struct __tds__GetNetworkInterfaces soap_tmp___tds__GetNetworkInterfaces;
	if (soap_action == NULL)
		soap_action = "http://www.onvif.org/ver10/device/wsdl/GetNetworkInterfaces";
	soap->encodingStyle = NULL;
	soap_tmp___tds__GetNetworkInterfaces.tds__GetNetworkInterfaces = tds__GetNetworkInterfaces;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__GetNetworkInterfaces(soap, &soap_tmp___tds__GetNetworkInterfaces);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__GetNetworkInterfaces(soap, &soap_tmp___tds__GetNetworkInterfaces, "-tds:GetNetworkInterfaces", NULL)
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
		|| soap_put___tds__GetNetworkInterfaces(soap, &soap_tmp___tds__GetNetworkInterfaces, "-tds:GetNetworkInterfaces", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tds__GetNetworkInterfacesResponse)
		return soap_closesock(soap);
	soap_default__tds__GetNetworkInterfacesResponse(soap, tds__GetNetworkInterfacesResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tds__GetNetworkInterfacesResponse(soap, tds__GetNetworkInterfacesResponse, "tds:GetNetworkInterfacesResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}


SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetNetworkInterfaces(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetNetworkInterfaces *tds__SetNetworkInterfaces, struct _tds__SetNetworkInterfacesResponse *tds__SetNetworkInterfacesResponse)
{	
	struct __tds__SetNetworkInterfaces soap_tmp___tds__SetNetworkInterfaces;
	if (soap_action == NULL) 
		soap_action = "http://www.onvif.org/ver10/device/wsdl/SetNetworkInterfaces";
	soap->encodingStyle = NULL;
	soap_tmp___tds__SetNetworkInterfaces.tds__SetNetworkInterfaces = tds__SetNetworkInterfaces;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__SetNetworkInterfaces(soap, &soap_tmp___tds__SetNetworkInterfaces);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__SetNetworkInterfaces(soap, &soap_tmp___tds__SetNetworkInterfaces, "-tds:SetNetworkInterfaces", NULL)
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
		|| soap_put___tds__SetNetworkInterfaces(soap, &soap_tmp___tds__SetNetworkInterfaces, "-tds:SetNetworkInterfaces", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tds__SetNetworkInterfacesResponse)
		return soap_closesock(soap);
	soap_default__tds__SetNetworkInterfacesResponse(soap, tds__SetNetworkInterfacesResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tds__SetNetworkInterfacesResponse(soap, tds__SetNetworkInterfacesResponse, "tds:SetNetworkInterfacesResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetNetworkProtocols(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetNetworkProtocols *tds__GetNetworkProtocols, struct _tds__GetNetworkProtocolsResponse *tds__GetNetworkProtocolsResponse)
{	
	struct __tds__GetNetworkProtocols soap_tmp___tds__GetNetworkProtocols;
	if (soap_action == NULL)
		soap_action = "http://www.onvif.org/ver10/device/wsdl/GetNetworkProtocols";
	soap->encodingStyle = NULL;
	soap_tmp___tds__GetNetworkProtocols.tds__GetNetworkProtocols = tds__GetNetworkProtocols;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__GetNetworkProtocols(soap, &soap_tmp___tds__GetNetworkProtocols);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__GetNetworkProtocols(soap, &soap_tmp___tds__GetNetworkProtocols, "-tds:GetNetworkProtocols", NULL)
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
		|| soap_put___tds__GetNetworkProtocols(soap, &soap_tmp___tds__GetNetworkProtocols, "-tds:GetNetworkProtocols", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tds__GetNetworkProtocolsResponse)
		return soap_closesock(soap);
	soap_default__tds__GetNetworkProtocolsResponse(soap, tds__GetNetworkProtocolsResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tds__GetNetworkProtocolsResponse(soap, tds__GetNetworkProtocolsResponse, "tds:GetNetworkProtocolsResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetNetworkProtocols(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetNetworkProtocols *tds__SetNetworkProtocols, struct _tds__SetNetworkProtocolsResponse *tds__SetNetworkProtocolsResponse)
{	struct __tds__SetNetworkProtocols soap_tmp___tds__SetNetworkProtocols;
	if (soap_action == NULL)
		soap_action = "http://www.onvif.org/ver10/device/wsdl/SetNetworkProtocols";
	soap->encodingStyle = NULL;
	soap_tmp___tds__SetNetworkProtocols.tds__SetNetworkProtocols = tds__SetNetworkProtocols;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__SetNetworkProtocols(soap, &soap_tmp___tds__SetNetworkProtocols);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__SetNetworkProtocols(soap, &soap_tmp___tds__SetNetworkProtocols, "-tds:SetNetworkProtocols", NULL)
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
		|| soap_put___tds__SetNetworkProtocols(soap, &soap_tmp___tds__SetNetworkProtocols, "-tds:SetNetworkProtocols", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tds__SetNetworkProtocolsResponse)
	return soap_closesock(soap);
	soap_default__tds__SetNetworkProtocolsResponse(soap, tds__SetNetworkProtocolsResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tds__SetNetworkProtocolsResponse(soap, tds__SetNetworkProtocolsResponse, "tds:SetNetworkProtocolsResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetDNS(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetDNS *tds__GetDNS, struct _tds__GetDNSResponse *tds__GetDNSResponse)
{	
	struct __tds__GetDNS soap_tmp___tds__GetDNS;
	if (soap_action == NULL)
		soap_action = "http://www.onvif.org/ver10/device/wsdl/GetDNS";
	soap->encodingStyle = NULL;
	soap_tmp___tds__GetDNS.tds__GetDNS = tds__GetDNS;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__GetDNS(soap, &soap_tmp___tds__GetDNS);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__GetDNS(soap, &soap_tmp___tds__GetDNS, "-tds:GetDNS", NULL)
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
		|| soap_put___tds__GetDNS(soap, &soap_tmp___tds__GetDNS, "-tds:GetDNS", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tds__GetDNSResponse)
	return soap_closesock(soap);
	soap_default__tds__GetDNSResponse(soap, tds__GetDNSResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tds__GetDNSResponse(soap, tds__GetDNSResponse, "tds:GetDNSResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetDNS(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetDNS *tds__SetDNS, struct _tds__SetDNSResponse *tds__SetDNSResponse)
{	
	struct __tds__SetDNS soap_tmp___tds__SetDNS;
	if (soap_action == NULL)
		soap_action = "http://www.onvif.org/ver10/device/wsdl/SetDNS";
	soap->encodingStyle = NULL;
	soap_tmp___tds__SetDNS.tds__SetDNS = tds__SetDNS;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__SetDNS(soap, &soap_tmp___tds__SetDNS);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__SetDNS(soap, &soap_tmp___tds__SetDNS, "-tds:SetDNS", NULL)
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
		|| soap_put___tds__SetDNS(soap, &soap_tmp___tds__SetDNS, "-tds:SetDNS", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tds__SetDNSResponse)
		return soap_closesock(soap);
	soap_default__tds__SetDNSResponse(soap, tds__SetDNSResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tds__SetDNSResponse(soap, tds__SetDNSResponse, "tds:SetDNSResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetNetworkDefaultGateway(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetNetworkDefaultGateway *tds__GetNetworkDefaultGateway, struct _tds__GetNetworkDefaultGatewayResponse *tds__GetNetworkDefaultGatewayResponse)
{	
	struct __tds__GetNetworkDefaultGateway soap_tmp___tds__GetNetworkDefaultGateway;
	if (soap_action == NULL)
		soap_action = "http://www.onvif.org/ver10/device/wsdl/GetNetworkDefaultGateway";
	soap->encodingStyle = NULL;
	soap_tmp___tds__GetNetworkDefaultGateway.tds__GetNetworkDefaultGateway = tds__GetNetworkDefaultGateway;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__GetNetworkDefaultGateway(soap, &soap_tmp___tds__GetNetworkDefaultGateway);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__GetNetworkDefaultGateway(soap, &soap_tmp___tds__GetNetworkDefaultGateway, "-tds:GetNetworkDefaultGateway", NULL)
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
		|| soap_put___tds__GetNetworkDefaultGateway(soap, &soap_tmp___tds__GetNetworkDefaultGateway, "-tds:GetNetworkDefaultGateway", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tds__GetNetworkDefaultGatewayResponse)
		return soap_closesock(soap);
	soap_default__tds__GetNetworkDefaultGatewayResponse(soap, tds__GetNetworkDefaultGatewayResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tds__GetNetworkDefaultGatewayResponse(soap, tds__GetNetworkDefaultGatewayResponse, "tds:GetNetworkDefaultGatewayResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetNetworkDefaultGateway(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetNetworkDefaultGateway *tds__SetNetworkDefaultGateway, struct _tds__SetNetworkDefaultGatewayResponse *tds__SetNetworkDefaultGatewayResponse)
{	
	struct __tds__SetNetworkDefaultGateway soap_tmp___tds__SetNetworkDefaultGateway;
	if (soap_action == NULL)
		soap_action = "http://www.onvif.org/ver10/device/wsdl/SetNetworkDefaultGateway";
	soap->encodingStyle = NULL;
	soap_tmp___tds__SetNetworkDefaultGateway.tds__SetNetworkDefaultGateway = tds__SetNetworkDefaultGateway;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__SetNetworkDefaultGateway(soap, &soap_tmp___tds__SetNetworkDefaultGateway);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__SetNetworkDefaultGateway(soap, &soap_tmp___tds__SetNetworkDefaultGateway, "-tds:SetNetworkDefaultGateway", NULL)
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
		|| soap_put___tds__SetNetworkDefaultGateway(soap, &soap_tmp___tds__SetNetworkDefaultGateway, "-tds:SetNetworkDefaultGateway", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tds__SetNetworkDefaultGatewayResponse)
		return soap_closesock(soap);
	soap_default__tds__SetNetworkDefaultGatewayResponse(soap, tds__SetNetworkDefaultGatewayResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tds__SetNetworkDefaultGatewayResponse(soap, tds__SetNetworkDefaultGatewayResponse, "tds:SetNetworkDefaultGatewayResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetNTP(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetNTP *tds__GetNTP, struct _tds__GetNTPResponse *tds__GetNTPResponse)
{	
	struct __tds__GetNTP soap_tmp___tds__GetNTP;
	if (soap_action == NULL)
		soap_action = "http://www.onvif.org/ver10/device/wsdl/GetNTP";
	soap->encodingStyle = NULL;
	soap_tmp___tds__GetNTP.tds__GetNTP = tds__GetNTP;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__GetNTP(soap, &soap_tmp___tds__GetNTP);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__GetNTP(soap, &soap_tmp___tds__GetNTP, "-tds:GetNTP", NULL)
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
		|| soap_put___tds__GetNTP(soap, &soap_tmp___tds__GetNTP, "-tds:GetNTP", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tds__GetNTPResponse)
		return soap_closesock(soap);
	soap_default__tds__GetNTPResponse(soap, tds__GetNTPResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tds__GetNTPResponse(soap, tds__GetNTPResponse, "tds:GetNTPResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetNTP(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetNTP *tds__SetNTP, struct _tds__SetNTPResponse *tds__SetNTPResponse)
{	
	struct __tds__SetNTP soap_tmp___tds__SetNTP;
	if (soap_action == NULL)
		soap_action = "http://www.onvif.org/ver10/device/wsdl/SetNTP";
	soap->encodingStyle = NULL;
	soap_tmp___tds__SetNTP.tds__SetNTP = tds__SetNTP;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__SetNTP(soap, &soap_tmp___tds__SetNTP);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__SetNTP(soap, &soap_tmp___tds__SetNTP, "-tds:SetNTP", NULL)
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
		|| soap_put___tds__SetNTP(soap, &soap_tmp___tds__SetNTP, "-tds:SetNTP", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tds__SetNTPResponse)
		return soap_closesock(soap);
	soap_default__tds__SetNTPResponse(soap, tds__SetNTPResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tds__SetNTPResponse(soap, tds__SetNTPResponse, "tds:SetNTPResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetDynamicDNS(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetDynamicDNS *tds__GetDynamicDNS, struct _tds__GetDynamicDNSResponse *tds__GetDynamicDNSResponse)
{	
	struct __tds__GetDynamicDNS soap_tmp___tds__GetDynamicDNS;
	if (soap_action == NULL)
		soap_action = "http://www.onvif.org/ver10/device/wsdl/GetDynamicDNS";
	soap->encodingStyle = NULL;
	soap_tmp___tds__GetDynamicDNS.tds__GetDynamicDNS = tds__GetDynamicDNS;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__GetDynamicDNS(soap, &soap_tmp___tds__GetDynamicDNS);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__GetDynamicDNS(soap, &soap_tmp___tds__GetDynamicDNS, "-tds:GetDynamicDNS", NULL)
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
		|| soap_put___tds__GetDynamicDNS(soap, &soap_tmp___tds__GetDynamicDNS, "-tds:GetDynamicDNS", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tds__GetDynamicDNSResponse)
		return soap_closesock(soap);
	soap_default__tds__GetDynamicDNSResponse(soap, tds__GetDynamicDNSResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tds__GetDynamicDNSResponse(soap, tds__GetDynamicDNSResponse, "tds:GetDynamicDNSResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetDynamicDNS(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetDynamicDNS *tds__SetDynamicDNS, struct _tds__SetDynamicDNSResponse *tds__SetDynamicDNSResponse)
{	struct __tds__SetDynamicDNS soap_tmp___tds__SetDynamicDNS;
	if (soap_action == NULL) 
		soap_action = "http://www.onvif.org/ver10/device/wsdl/SetDynamicDNS";
	soap->encodingStyle = NULL;
	soap_tmp___tds__SetDynamicDNS.tds__SetDynamicDNS = tds__SetDynamicDNS;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__SetDynamicDNS(soap, &soap_tmp___tds__SetDynamicDNS);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__SetDynamicDNS(soap, &soap_tmp___tds__SetDynamicDNS, "-tds:SetDynamicDNS", NULL)
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
		|| soap_put___tds__SetDynamicDNS(soap, &soap_tmp___tds__SetDynamicDNS, "-tds:SetDynamicDNS", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tds__SetDynamicDNSResponse)
		return soap_closesock(soap);
	soap_default__tds__SetDynamicDNSResponse(soap, tds__SetDynamicDNSResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tds__SetDynamicDNSResponse(soap, tds__SetDynamicDNSResponse, "tds:SetDynamicDNSResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetHostname(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetHostname *tds__GetHostname, struct _tds__GetHostnameResponse *tds__GetHostnameResponse)
{	
	struct __tds__GetHostname soap_tmp___tds__GetHostname;
	if (soap_action == NULL)
		soap_action = "http://www.onvif.org/ver10/device/wsdl/GetHostname";
	soap->encodingStyle = NULL;
	soap_tmp___tds__GetHostname.tds__GetHostname = tds__GetHostname;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__GetHostname(soap, &soap_tmp___tds__GetHostname);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__GetHostname(soap, &soap_tmp___tds__GetHostname, "-tds:GetHostname", NULL)
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
		|| soap_put___tds__GetHostname(soap, &soap_tmp___tds__GetHostname, "-tds:GetHostname", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tds__GetHostnameResponse)
		return soap_closesock(soap);
	soap_default__tds__GetHostnameResponse(soap, tds__GetHostnameResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tds__GetHostnameResponse(soap, tds__GetHostnameResponse, "tds:GetHostnameResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetHostname(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetHostname *tds__SetHostname, struct _tds__SetHostnameResponse *tds__SetHostnameResponse)
{	
	struct __tds__SetHostname soap_tmp___tds__SetHostname;
	if (!soap_action)
		soap_action = "http://www.onvif.org/ver10/device/wsdl/SetHostname";
	soap->encodingStyle = NULL;
	soap_tmp___tds__SetHostname.tds__SetHostname = tds__SetHostname;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__SetHostname(soap, &soap_tmp___tds__SetHostname);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__SetHostname(soap, &soap_tmp___tds__SetHostname, "-tds:SetHostname", NULL)
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
		|| soap_put___tds__SetHostname(soap, &soap_tmp___tds__SetHostname, "-tds:SetHostname", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tds__SetHostnameResponse)
		return soap_closesock(soap);
	soap_default__tds__SetHostnameResponse(soap, tds__SetHostnameResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tds__SetHostnameResponse(soap, tds__SetHostnameResponse, "tds:SetHostnameResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetHostnameFromDHCP(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetHostnameFromDHCP *tds__SetHostnameFromDHCP, struct _tds__SetHostnameFromDHCPResponse *tds__SetHostnameFromDHCPResponse)
{	
	struct __tds__SetHostnameFromDHCP soap_tmp___tds__SetHostnameFromDHCP;
	if (soap_action == NULL)
		soap_action = "http://www.onvif.org/ver10/device/wsdl/SetHostnameFromDHCP";
	soap->encodingStyle = NULL;
	soap_tmp___tds__SetHostnameFromDHCP.tds__SetHostnameFromDHCP = tds__SetHostnameFromDHCP;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__SetHostnameFromDHCP(soap, &soap_tmp___tds__SetHostnameFromDHCP);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__SetHostnameFromDHCP(soap, &soap_tmp___tds__SetHostnameFromDHCP, "-tds:SetHostnameFromDHCP", NULL)
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
		|| soap_put___tds__SetHostnameFromDHCP(soap, &soap_tmp___tds__SetHostnameFromDHCP, "-tds:SetHostnameFromDHCP", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tds__SetHostnameFromDHCPResponse)
		return soap_closesock(soap);
	soap_default__tds__SetHostnameFromDHCPResponse(soap, tds__SetHostnameFromDHCPResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tds__SetHostnameFromDHCPResponse(soap, tds__SetHostnameFromDHCPResponse, "tds:SetHostnameFromDHCPResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetIPAddressFilter(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetIPAddressFilter *tds__GetIPAddressFilter, struct _tds__GetIPAddressFilterResponse *tds__GetIPAddressFilterResponse)
{	
	struct __tds__GetIPAddressFilter soap_tmp___tds__GetIPAddressFilter;
	if (soap_action == NULL)
		soap_action = "http://www.onvif.org/ver10/device/wsdl/GetIPAddressFilter";
	soap->encodingStyle = NULL;
	soap_tmp___tds__GetIPAddressFilter.tds__GetIPAddressFilter = tds__GetIPAddressFilter;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__GetIPAddressFilter(soap, &soap_tmp___tds__GetIPAddressFilter);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__GetIPAddressFilter(soap, &soap_tmp___tds__GetIPAddressFilter, "-tds:GetIPAddressFilter", NULL)
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
		|| soap_put___tds__GetIPAddressFilter(soap, &soap_tmp___tds__GetIPAddressFilter, "-tds:GetIPAddressFilter", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tds__GetIPAddressFilterResponse)
		return soap_closesock(soap);
	soap_default__tds__GetIPAddressFilterResponse(soap, tds__GetIPAddressFilterResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tds__GetIPAddressFilterResponse(soap, tds__GetIPAddressFilterResponse, "tds:GetIPAddressFilterResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetIPAddressFilter(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetIPAddressFilter *tds__SetIPAddressFilter, struct _tds__SetIPAddressFilterResponse *tds__SetIPAddressFilterResponse)
{	
	struct __tds__SetIPAddressFilter soap_tmp___tds__SetIPAddressFilter;
	if (soap_action == NULL)
		soap_action = "http://www.onvif.org/ver10/device/wsdl/SetIPAddressFilter";
	soap->encodingStyle = NULL;
	soap_tmp___tds__SetIPAddressFilter.tds__SetIPAddressFilter = tds__SetIPAddressFilter;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__SetIPAddressFilter(soap, &soap_tmp___tds__SetIPAddressFilter);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__SetIPAddressFilter(soap, &soap_tmp___tds__SetIPAddressFilter, "-tds:SetIPAddressFilter", NULL)
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
		|| soap_put___tds__SetIPAddressFilter(soap, &soap_tmp___tds__SetIPAddressFilter, "-tds:SetIPAddressFilter", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tds__SetIPAddressFilterResponse)
		return soap_closesock(soap);
	soap_default__tds__SetIPAddressFilterResponse(soap, tds__SetIPAddressFilterResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tds__SetIPAddressFilterResponse(soap, tds__SetIPAddressFilterResponse, "tds:SetIPAddressFilterResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__AddIPAddressFilter(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__AddIPAddressFilter *tds__AddIPAddressFilter, struct _tds__AddIPAddressFilterResponse *tds__AddIPAddressFilterResponse)
{	
	struct __tds__AddIPAddressFilter soap_tmp___tds__AddIPAddressFilter;
	if (soap_action == NULL)
		soap_action = "http://www.onvif.org/ver10/device/wsdl/AddIPAddressFilter";
	soap->encodingStyle = NULL;
	soap_tmp___tds__AddIPAddressFilter.tds__AddIPAddressFilter = tds__AddIPAddressFilter;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__AddIPAddressFilter(soap, &soap_tmp___tds__AddIPAddressFilter);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__AddIPAddressFilter(soap, &soap_tmp___tds__AddIPAddressFilter, "-tds:AddIPAddressFilter", NULL)
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
		|| soap_put___tds__AddIPAddressFilter(soap, &soap_tmp___tds__AddIPAddressFilter, "-tds:AddIPAddressFilter", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tds__AddIPAddressFilterResponse)
		return soap_closesock(soap);
	soap_default__tds__AddIPAddressFilterResponse(soap, tds__AddIPAddressFilterResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tds__AddIPAddressFilterResponse(soap, tds__AddIPAddressFilterResponse, "tds:AddIPAddressFilterResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__RemoveIPAddressFilter(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__RemoveIPAddressFilter *tds__RemoveIPAddressFilter, struct _tds__RemoveIPAddressFilterResponse *tds__RemoveIPAddressFilterResponse)
{	
	struct __tds__RemoveIPAddressFilter soap_tmp___tds__RemoveIPAddressFilter;
	if (soap_action == NULL)
		soap_action = "http://www.onvif.org/ver10/device/wsdl/RemoveIPAddressFilter";
	soap->encodingStyle = NULL;
	soap_tmp___tds__RemoveIPAddressFilter.tds__RemoveIPAddressFilter = tds__RemoveIPAddressFilter;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__RemoveIPAddressFilter(soap, &soap_tmp___tds__RemoveIPAddressFilter);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__RemoveIPAddressFilter(soap, &soap_tmp___tds__RemoveIPAddressFilter, "-tds:RemoveIPAddressFilter", NULL)
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
		|| soap_put___tds__RemoveIPAddressFilter(soap, &soap_tmp___tds__RemoveIPAddressFilter, "-tds:RemoveIPAddressFilter", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);
	if (!tds__RemoveIPAddressFilterResponse)
		return soap_closesock(soap);
	soap_default__tds__RemoveIPAddressFilterResponse(soap, tds__RemoveIPAddressFilterResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__tds__RemoveIPAddressFilterResponse(soap, tds__RemoveIPAddressFilterResponse, "tds:RemoveIPAddressFilterResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

//device configuration
SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetProfiles(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetProfiles *trt__GetProfiles, struct _trt__GetProfilesResponse *trt__GetProfilesResponse)
{	
	struct __trt__GetProfiles soap_tmp___trt__GetProfiles;
	if (soap_action == NULL)
		soap_action = "http://www.onvif.org/ver10/media/wsdl/GetProfiles";
	soap->encodingStyle = NULL;
	soap_tmp___trt__GetProfiles.trt__GetProfiles = trt__GetProfiles;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___trt__GetProfiles(soap, &soap_tmp___trt__GetProfiles);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___trt__GetProfiles(soap, &soap_tmp___trt__GetProfiles, "-trt:GetProfiles", NULL)
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
		|| soap_put___trt__GetProfiles(soap, &soap_tmp___trt__GetProfiles, "-trt:GetProfiles", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
		return soap_closesock(soap);

	if (!trt__GetProfilesResponse)
		return soap_closesock(soap);

	soap_default__trt__GetProfilesResponse(soap, trt__GetProfilesResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
		return soap_closesock(soap);
	soap_get__trt__GetProfilesResponse(soap, trt__GetProfilesResponse, "trt:GetProfilesResponse", "");
	if (soap->error)
		return soap_recv_fault(soap, 0);
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
		return soap_closesock(soap);
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetVideoEncoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetVideoEncoderConfiguration *trt__GetVideoEncoderConfiguration, struct _trt__GetVideoEncoderConfigurationResponse *trt__GetVideoEncoderConfigurationResponse)
{	
	struct __trt__GetVideoEncoderConfiguration soap_tmp___trt__GetVideoEncoderConfiguration;
	if (soap_action == NULL)
		soap_action = "http://www.onvif.org/ver10/media/wsdl/GetVideoEncoderConfiguration";
	soap->encodingStyle = NULL;
	soap_tmp___trt__GetVideoEncoderConfiguration.trt__GetVideoEncoderConfiguration = trt__GetVideoEncoderConfiguration;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___trt__GetVideoEncoderConfiguration(soap, &soap_tmp___trt__GetVideoEncoderConfiguration);
	if (soap_begin_count(soap))
	{
		return soap->error;
	}
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___trt__GetVideoEncoderConfiguration(soap, &soap_tmp___trt__GetVideoEncoderConfiguration, "-trt:GetVideoEncoderConfiguration", NULL)
			|| soap_body_end_out(soap)
			|| soap_envelope_end_out(soap))
		{
			return soap->error;
		}
	}
	if (soap_end_count(soap))
	{
		return soap->error;
	}
	if (soap_connect(soap, soap_endpoint, soap_action)
		|| soap_envelope_begin_out(soap)
		|| soap_putheader(soap)
		|| soap_body_begin_out(soap)
		|| soap_put___trt__GetVideoEncoderConfiguration(soap, &soap_tmp___trt__GetVideoEncoderConfiguration, "-trt:GetVideoEncoderConfiguration", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
	{
		return soap_closesock(soap);
	}
	if (trt__GetVideoEncoderConfigurationResponse == NULL)
	{
		return soap_closesock(soap);
	}
	soap_default__trt__GetVideoEncoderConfigurationResponse(soap, trt__GetVideoEncoderConfigurationResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
	{
		return soap_closesock(soap);
	}
	soap_get__trt__GetVideoEncoderConfigurationResponse(soap, trt__GetVideoEncoderConfigurationResponse, "trt:GetVideoEncoderConfigurationResponse", "");
	if (soap->error)
	{
		return soap_recv_fault(soap, 0);
	}
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
	{
		return soap_closesock(soap);
	}
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__SetVideoEncoderConfiguration(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__SetVideoEncoderConfiguration *trt__SetVideoEncoderConfiguration, struct _trt__SetVideoEncoderConfigurationResponse *trt__SetVideoEncoderConfigurationResponse)
{	
	struct __trt__SetVideoEncoderConfiguration soap_tmp___trt__SetVideoEncoderConfiguration;
	if (soap_action == NULL)
	{
		soap_action = "http://www.onvif.org/ver10/media/wsdl/SetVideoEncoderConfiguration";	
	}
	soap->encodingStyle = NULL;
	soap_tmp___trt__SetVideoEncoderConfiguration.trt__SetVideoEncoderConfiguration = trt__SetVideoEncoderConfiguration;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___trt__SetVideoEncoderConfiguration(soap, &soap_tmp___trt__SetVideoEncoderConfiguration);
	if (soap_begin_count(soap))
	{
		return soap->error;
	}
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___trt__SetVideoEncoderConfiguration(soap, &soap_tmp___trt__SetVideoEncoderConfiguration, "-trt:SetVideoEncoderConfiguration", NULL)
			|| soap_body_end_out(soap)
			|| soap_envelope_end_out(soap))
		{
			return soap->error;
		}
	}
	if (soap_end_count(soap))
	{
		return soap->error;
	}
	if (soap_connect(soap, soap_endpoint, soap_action)
		|| soap_envelope_begin_out(soap)
		|| soap_putheader(soap)
		|| soap_body_begin_out(soap)
		|| soap_put___trt__SetVideoEncoderConfiguration(soap, &soap_tmp___trt__SetVideoEncoderConfiguration, "-trt:SetVideoEncoderConfiguration", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
	{
		return soap_closesock(soap);
	}
	if (trt__SetVideoEncoderConfigurationResponse == NULL)
	{
		return soap_closesock(soap);
	}
	soap_default__trt__SetVideoEncoderConfigurationResponse(soap, trt__SetVideoEncoderConfigurationResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
	{
		return soap_closesock(soap);
	}
	soap_get__trt__SetVideoEncoderConfigurationResponse(soap, trt__SetVideoEncoderConfigurationResponse, "trt:SetVideoEncoderConfigurationResponse", "");
	if (soap->error)
	{
		return soap_recv_fault(soap, 0);
	}
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
	{
		return soap_closesock(soap);
	}
	return soap_closesock(soap);
}


SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetVideoEncoderConfigurationOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetVideoEncoderConfigurationOptions *trt__GetVideoEncoderConfigurationOptions, struct _trt__GetVideoEncoderConfigurationOptionsResponse *trt__GetVideoEncoderConfigurationOptionsResponse)
{	
	struct __trt__GetVideoEncoderConfigurationOptions soap_tmp___trt__GetVideoEncoderConfigurationOptions;
	if (soap_action == NULL)
	{
		soap_action = "http://www.onvif.org/ver10/media/wsdl/GetVideoEncoderConfigurationOptions";
	}
	soap->encodingStyle = NULL;
	soap_tmp___trt__GetVideoEncoderConfigurationOptions.trt__GetVideoEncoderConfigurationOptions = trt__GetVideoEncoderConfigurationOptions;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___trt__GetVideoEncoderConfigurationOptions(soap, &soap_tmp___trt__GetVideoEncoderConfigurationOptions);
	if (soap_begin_count(soap))
	{
		return soap->error;
	}
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___trt__GetVideoEncoderConfigurationOptions(soap, &soap_tmp___trt__GetVideoEncoderConfigurationOptions, "-trt:GetVideoEncoderConfigurationOptions", NULL)
			|| soap_body_end_out(soap)
			|| soap_envelope_end_out(soap))
		{
			return soap->error;
		}
	}
	if (soap_end_count(soap))
	{
		return soap->error;
	}
	if (soap_connect(soap, soap_endpoint, soap_action)
		|| soap_envelope_begin_out(soap)
		|| soap_putheader(soap)
		|| soap_body_begin_out(soap)
		|| soap_put___trt__GetVideoEncoderConfigurationOptions(soap, &soap_tmp___trt__GetVideoEncoderConfigurationOptions, "-trt:GetVideoEncoderConfigurationOptions", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
	{
		return soap_closesock(soap);
	}
	if (trt__GetVideoEncoderConfigurationOptionsResponse == NULL)
	{
		return soap_closesock(soap);
	}
	soap_default__trt__GetVideoEncoderConfigurationOptionsResponse(soap, trt__GetVideoEncoderConfigurationOptionsResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
	{
		return soap_closesock(soap);
	}
	soap_get__trt__GetVideoEncoderConfigurationOptionsResponse(soap, trt__GetVideoEncoderConfigurationOptionsResponse, "trt:GetVideoEncoderConfigurationOptionsResponse", "");
	if (soap->error)
	{
		return soap_recv_fault(soap, 0);
	}
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
	{
		return soap_closesock(soap);
	}
	return soap_closesock(soap);
}

//device video
SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetStreamUri(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetStreamUri *trt__GetStreamUri, struct _trt__GetStreamUriResponse *trt__GetStreamUriResponse)
{	
	struct __trt__GetStreamUri soap_tmp___trt__GetStreamUri;
	if (soap_action == NULL)
	{
		soap_action = "http://www.onvif.org/ver10/media/wsdl/GetStreamUri";
	}
	soap->encodingStyle = NULL;
	soap_tmp___trt__GetStreamUri.trt__GetStreamUri = trt__GetStreamUri;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___trt__GetStreamUri(soap, &soap_tmp___trt__GetStreamUri);
	if (soap_begin_count(soap))
	{
		return soap->error;
	}
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___trt__GetStreamUri(soap, &soap_tmp___trt__GetStreamUri, "-trt:GetStreamUri", NULL)
			|| soap_body_end_out(soap)
			|| soap_envelope_end_out(soap))
		{
			return soap->error;
		}
	}
	if (soap_end_count(soap))
	{
		return soap->error;
	}
	if (soap_connect(soap, soap_endpoint, soap_action)
		|| soap_envelope_begin_out(soap)
		|| soap_putheader(soap)
		|| soap_body_begin_out(soap)
		|| soap_put___trt__GetStreamUri(soap, &soap_tmp___trt__GetStreamUri, "-trt:GetStreamUri", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
	{
		return soap_closesock(soap);
	}
	if (trt__GetStreamUriResponse == NULL)
	{
		return soap_closesock(soap);
	}
	soap_default__trt__GetStreamUriResponse(soap, trt__GetStreamUriResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
	{
		return soap_closesock(soap);
	}
	soap_get__trt__GetStreamUriResponse(soap, trt__GetStreamUriResponse, "trt:GetStreamUriResponse", "");
	if (soap->error)
	{
		return soap_recv_fault(soap, 0);
	}
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
	{
		return soap_closesock(soap);
	}
	return soap_closesock(soap);
}


SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__StartMulticastStreaming(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__StartMulticastStreaming *trt__StartMulticastStreaming, struct _trt__StartMulticastStreamingResponse *trt__StartMulticastStreamingResponse)
{	
	struct __trt__StartMulticastStreaming soap_tmp___trt__StartMulticastStreaming;
	if (soap_action == NULL)
	{
		soap_action = "http://www.onvif.org/ver10/media/wsdl/StartMulticastStreaming";
	}
	soap->encodingStyle = NULL;
	soap_tmp___trt__StartMulticastStreaming.trt__StartMulticastStreaming = trt__StartMulticastStreaming;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___trt__StartMulticastStreaming(soap, &soap_tmp___trt__StartMulticastStreaming);
	if (soap_begin_count(soap))
	{
		return soap->error;
	}
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___trt__StartMulticastStreaming(soap, &soap_tmp___trt__StartMulticastStreaming, "-trt:StartMulticastStreaming", NULL)
			|| soap_body_end_out(soap)
			|| soap_envelope_end_out(soap))
		{
			return soap->error;
		}
	}
	if (soap_end_count(soap))
	{
		return soap->error;
	}
	if (soap_connect(soap, soap_endpoint, soap_action)
		|| soap_envelope_begin_out(soap)
		|| soap_putheader(soap)
		|| soap_body_begin_out(soap)
		|| soap_put___trt__StartMulticastStreaming(soap, &soap_tmp___trt__StartMulticastStreaming, "-trt:StartMulticastStreaming", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
	{
		return soap_closesock(soap);
	}
	if (trt__StartMulticastStreamingResponse == NULL)
	{
		return soap_closesock(soap);
	}
	soap_default__trt__StartMulticastStreamingResponse(soap, trt__StartMulticastStreamingResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
	{
		return soap_closesock(soap);
	}
	soap_get__trt__StartMulticastStreamingResponse(soap, trt__StartMulticastStreamingResponse, "trt:StartMulticastStreamingResponse", "");
	if (soap->error)
	{
		return soap_recv_fault(soap, 0);
	}
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
	{
		return soap_closesock(soap);
	}
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__StopMulticastStreaming(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__StopMulticastStreaming *trt__StopMulticastStreaming, struct _trt__StopMulticastStreamingResponse *trt__StopMulticastStreamingResponse)
{	
	struct __trt__StopMulticastStreaming soap_tmp___trt__StopMulticastStreaming;
	if (soap_action == NULL)
	{
		soap_action = "http://www.onvif.org/ver10/media/wsdl/StopMulticastStreaming";
	}
	soap->encodingStyle = NULL;
	soap_tmp___trt__StopMulticastStreaming.trt__StopMulticastStreaming = trt__StopMulticastStreaming;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___trt__StopMulticastStreaming(soap, &soap_tmp___trt__StopMulticastStreaming);
	if (soap_begin_count(soap))
	{
		return soap->error;
	}
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___trt__StopMulticastStreaming(soap, &soap_tmp___trt__StopMulticastStreaming, "-trt:StopMulticastStreaming", NULL)
			|| soap_body_end_out(soap)
			|| soap_envelope_end_out(soap))
		{
			return soap->error;
		}
	}
	if (soap_end_count(soap))
	{
		return soap->error;
	}
	if (soap_connect(soap, soap_endpoint, soap_action)
		|| soap_envelope_begin_out(soap)
		|| soap_putheader(soap)
		|| soap_body_begin_out(soap)
		|| soap_put___trt__StopMulticastStreaming(soap, &soap_tmp___trt__StopMulticastStreaming, "-trt:StopMulticastStreaming", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
	{
		return soap_closesock(soap);
	}
	if (trt__StopMulticastStreamingResponse == NULL)
	{
		return soap_closesock(soap);
	}
	soap_default__trt__StopMulticastStreamingResponse(soap, trt__StopMulticastStreamingResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
	{
		return soap_closesock(soap);
	}
	soap_get__trt__StopMulticastStreamingResponse(soap, trt__StopMulticastStreamingResponse, "trt:StopMulticastStreamingResponse", "");
	if (soap->error)
	{
		return soap_recv_fault(soap, 0);
	}
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
	{
		return soap_closesock(soap);
	}
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__SetSynchronizationPoint(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__SetSynchronizationPoint *trt__SetSynchronizationPoint, struct _trt__SetSynchronizationPointResponse *trt__SetSynchronizationPointResponse)
{	
	struct __trt__SetSynchronizationPoint soap_tmp___trt__SetSynchronizationPoint;
	if (soap_action == NULL)
	{
		soap_action = "http://www.onvif.org/ver10/media/wsdl/SetSynchronizationPoint";
	}
	soap->encodingStyle = NULL;
	soap_tmp___trt__SetSynchronizationPoint.trt__SetSynchronizationPoint = trt__SetSynchronizationPoint;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___trt__SetSynchronizationPoint(soap, &soap_tmp___trt__SetSynchronizationPoint);
	if (soap_begin_count(soap))
	{
		return soap->error;
	}
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___trt__SetSynchronizationPoint(soap, &soap_tmp___trt__SetSynchronizationPoint, "-trt:SetSynchronizationPoint", NULL)
			|| soap_body_end_out(soap)
			|| soap_envelope_end_out(soap))
		{
			return soap->error;
		}
	}
	if (soap_end_count(soap))
	{
		return soap->error;
	}
	if (soap_connect(soap, soap_endpoint, soap_action)
		|| soap_envelope_begin_out(soap)
		|| soap_putheader(soap)
		|| soap_body_begin_out(soap)
		|| soap_put___trt__SetSynchronizationPoint(soap, &soap_tmp___trt__SetSynchronizationPoint, "-trt:SetSynchronizationPoint", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
	{
		return soap_closesock(soap);
	}
	if (trt__SetSynchronizationPointResponse == NULL)
	{
		return soap_closesock(soap);
	}
	soap_default__trt__SetSynchronizationPointResponse(soap, trt__SetSynchronizationPointResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
	{
		return soap_closesock(soap);
	}
	soap_get__trt__SetSynchronizationPointResponse(soap, trt__SetSynchronizationPointResponse, "trt:SetSynchronizationPointResponse", "");
	if (soap->error)
	{
		return soap_recv_fault(soap, 0);
	}
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
	{
		return soap_closesock(soap);
	}
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___trt__GetSnapshotUri(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _trt__GetSnapshotUri *trt__GetSnapshotUri, struct _trt__GetSnapshotUriResponse *trt__GetSnapshotUriResponse)
{	
	struct __trt__GetSnapshotUri soap_tmp___trt__GetSnapshotUri;
	if (soap_action == NULL)
	{
		soap_action = "http://www.onvif.org/ver10/media/wsdl/GetSnapshotUri";
	}
	soap->encodingStyle = NULL;
	soap_tmp___trt__GetSnapshotUri.trt__GetSnapshotUri = trt__GetSnapshotUri;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___trt__GetSnapshotUri(soap, &soap_tmp___trt__GetSnapshotUri);
	if (soap_begin_count(soap))
	{
		return soap->error;
	}
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___trt__GetSnapshotUri(soap, &soap_tmp___trt__GetSnapshotUri, "-trt:GetSnapshotUri", NULL)
			|| soap_body_end_out(soap)
			|| soap_envelope_end_out(soap))
		{
			return soap->error;
		}
	}
	if (soap_end_count(soap))
	{
		return soap->error;
	}
	if (soap_connect(soap, soap_endpoint, soap_action)
		|| soap_envelope_begin_out(soap)
		|| soap_putheader(soap)
		|| soap_body_begin_out(soap)
		|| soap_put___trt__GetSnapshotUri(soap, &soap_tmp___trt__GetSnapshotUri, "-trt:GetSnapshotUri", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
	{
		return soap_closesock(soap);
	}
	if (trt__GetSnapshotUriResponse == NULL)
	{
		return soap_closesock(soap);
	}
	soap_default__trt__GetSnapshotUriResponse(soap, trt__GetSnapshotUriResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
	{
		return soap_closesock(soap);
	}
	soap_get__trt__GetSnapshotUriResponse(soap, trt__GetSnapshotUriResponse, "trt:GetSnapshotUriResponse", "");
	if (soap->error)
	{
		return soap_recv_fault(soap, 0);
	}
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
	{
		return soap_closesock(soap);
	}
	return soap_closesock(soap);
}
//device imaging control
SOAP_FMAC5 int SOAP_FMAC6 soap_call___timg__GetImagingSettings(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__GetImagingSettings *timg__GetImagingSettings, struct _timg__GetImagingSettingsResponse *timg__GetImagingSettingsResponse)
{	
	struct __timg__GetImagingSettings soap_tmp___timg__GetImagingSettings;
	if (soap_action == NULL)
	{
		soap_action = "http://www.onvif.org/ver20/imaging/wsdl/GetImagingSettings";	
	}
	soap->encodingStyle = NULL;
	soap_tmp___timg__GetImagingSettings.timg__GetImagingSettings = timg__GetImagingSettings;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___timg__GetImagingSettings(soap, &soap_tmp___timg__GetImagingSettings);
	if (soap_begin_count(soap))
	{
		return soap->error;
	}
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___timg__GetImagingSettings(soap, &soap_tmp___timg__GetImagingSettings, "-timg:GetImagingSettings", NULL)
			|| soap_body_end_out(soap)
			|| soap_envelope_end_out(soap))
		{
			return soap->error;
		}
	}
	if (soap_end_count(soap))
	{
		return soap->error;
	}
	if (soap_connect(soap, soap_endpoint, soap_action)
		|| soap_envelope_begin_out(soap)
		|| soap_putheader(soap)
		|| soap_body_begin_out(soap)
		|| soap_put___timg__GetImagingSettings(soap, &soap_tmp___timg__GetImagingSettings, "-timg:GetImagingSettings", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
	{
		return soap_closesock(soap);
	}
	if (timg__GetImagingSettingsResponse == NULL)
	{
		return soap_closesock(soap);
	}
	soap_default__timg__GetImagingSettingsResponse(soap, timg__GetImagingSettingsResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
	{
		return soap_closesock(soap);
	}
	soap_get__timg__GetImagingSettingsResponse(soap, timg__GetImagingSettingsResponse, "timg:GetImagingSettingsResponse", "");
	if (soap->error)
	{
		return soap_recv_fault(soap, 0);
	}
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
	{
		return soap_closesock(soap);
	}
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___timg__SetImagingSettings(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__SetImagingSettings *timg__SetImagingSettings, struct _timg__SetImagingSettingsResponse *timg__SetImagingSettingsResponse)
{	
	struct __timg__SetImagingSettings soap_tmp___timg__SetImagingSettings;
	if (soap_action == NULL)
	{
		soap_action = "http://www.onvif.org/ver20/imaging/wsdl/SetImagingSettings";
	}
	soap->encodingStyle = NULL;
	soap_tmp___timg__SetImagingSettings.timg__SetImagingSettings = timg__SetImagingSettings;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___timg__SetImagingSettings(soap, &soap_tmp___timg__SetImagingSettings);
	if (soap_begin_count(soap))
	{
		return soap->error;
	}
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___timg__SetImagingSettings(soap, &soap_tmp___timg__SetImagingSettings, "-timg:SetImagingSettings", NULL)
			|| soap_body_end_out(soap)
			|| soap_envelope_end_out(soap))
		{
			return soap->error;
		}
	}
	if (soap_end_count(soap))
	{
		return soap->error;
	}
	if (soap_connect(soap, soap_endpoint, soap_action)
		|| soap_envelope_begin_out(soap)
		|| soap_putheader(soap)
		|| soap_body_begin_out(soap)
		|| soap_put___timg__SetImagingSettings(soap, &soap_tmp___timg__SetImagingSettings, "-timg:SetImagingSettings", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
	{
		return soap_closesock(soap);
	}
	if (timg__SetImagingSettingsResponse == NULL)
	{
		return soap_closesock(soap);
	}
	soap_default__timg__SetImagingSettingsResponse(soap, timg__SetImagingSettingsResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
	{
		return soap_closesock(soap);
	}
	soap_get__timg__SetImagingSettingsResponse(soap, timg__SetImagingSettingsResponse, "timg:SetImagingSettingsResponse", "");
	if (soap->error)
	{
		return soap_recv_fault(soap, 0);
	}
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
	{
		return soap_closesock(soap);
	}
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___timg__GetOptions(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _timg__GetOptions *timg__GetOptions, struct _timg__GetOptionsResponse *timg__GetOptionsResponse)
{	
	struct __timg__GetOptions soap_tmp___timg__GetOptions;
	if (NULL==soap_action)
	{
		soap_action = "http://www.onvif.org/ver20/imaging/wsdl/GetOptions";
	}
	soap->encodingStyle = NULL;
	soap_tmp___timg__GetOptions.timg__GetOptions = timg__GetOptions;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___timg__GetOptions(soap, &soap_tmp___timg__GetOptions);
	if (soap_begin_count(soap))
	{
		return soap->error;
	}
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___timg__GetOptions(soap, &soap_tmp___timg__GetOptions, "-timg:GetOptions", NULL)
			|| soap_body_end_out(soap)
			|| soap_envelope_end_out(soap))
		{
			return soap->error;
		}
	}
	if (soap_end_count(soap))
	{
		return soap->error;
	}
	if (soap_connect(soap, soap_endpoint, soap_action)
		|| soap_envelope_begin_out(soap)
		|| soap_putheader(soap)
		|| soap_body_begin_out(soap)
		|| soap_put___timg__GetOptions(soap, &soap_tmp___timg__GetOptions, "-timg:GetOptions", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
	{
		return soap_closesock(soap);
	}
	if (NULL==timg__GetOptionsResponse)
	{
		return soap_closesock(soap);
	}
	soap_default__timg__GetOptionsResponse(soap, timg__GetOptionsResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
	{
		return soap_closesock(soap);
	}
	soap_get__timg__GetOptionsResponse(soap, timg__GetOptionsResponse, "timg:GetOptionsResponse", "");
	if (soap->error)
	{
		return soap_recv_fault(soap, 0);
	}
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
	{
		return soap_closesock(soap);
	}
	return soap_closesock(soap);
}

//device IO Control
SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__GetRelayOutputs(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__GetRelayOutputs *tds__GetRelayOutputs, struct _tds__GetRelayOutputsResponse *tds__GetRelayOutputsResponse)
{	
	struct __tds__GetRelayOutputs soap_tmp___tds__GetRelayOutputs;
	if (soap_action == NULL)
	{
		soap_action = "http://www.onvif.org/ver10/device/wsdl/GetRelayOutputs";
	}
	soap->encodingStyle = NULL;
	soap_tmp___tds__GetRelayOutputs.tds__GetRelayOutputs = tds__GetRelayOutputs;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__GetRelayOutputs(soap, &soap_tmp___tds__GetRelayOutputs);
	if (soap_begin_count(soap))
	{
		return soap->error;
	}
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__GetRelayOutputs(soap, &soap_tmp___tds__GetRelayOutputs, "-tds:GetRelayOutputs", NULL)
			|| soap_body_end_out(soap)
			|| soap_envelope_end_out(soap))
		{
			return soap->error;
		}
	}
	if (soap_end_count(soap))
	{
		return soap->error;
	}
	if (soap_connect(soap, soap_endpoint, soap_action)
		|| soap_envelope_begin_out(soap)
		|| soap_putheader(soap)
		|| soap_body_begin_out(soap)
		|| soap_put___tds__GetRelayOutputs(soap, &soap_tmp___tds__GetRelayOutputs, "-tds:GetRelayOutputs", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
	{
		return soap_closesock(soap);
	}
	if (tds__GetRelayOutputsResponse == NULL) 
	{
		return soap_closesock(soap);
	}
	soap_default__tds__GetRelayOutputsResponse(soap, tds__GetRelayOutputsResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
	{
		return soap_closesock(soap);
	}
	soap_get__tds__GetRelayOutputsResponse(soap, tds__GetRelayOutputsResponse, "tds:GetRelayOutputsResponse", "");
	if (soap->error)
	{
		return soap_recv_fault(soap, 0);
	}
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
	{
		return soap_closesock(soap);
	}
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetRelayOutputSettings(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetRelayOutputSettings *tds__SetRelayOutputSettings, struct _tds__SetRelayOutputSettingsResponse *tds__SetRelayOutputSettingsResponse)
{	
	struct __tds__SetRelayOutputSettings soap_tmp___tds__SetRelayOutputSettings;
	if (soap_action == NULL)
	{
		soap_action = "http://www.onvif.org/ver10/device/wsdl/SetRelayOutputSettings";
	}
	soap->encodingStyle = NULL;
	soap_tmp___tds__SetRelayOutputSettings.tds__SetRelayOutputSettings = tds__SetRelayOutputSettings;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__SetRelayOutputSettings(soap, &soap_tmp___tds__SetRelayOutputSettings);
	if (soap_begin_count(soap))
	{
		return soap->error;
	}
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__SetRelayOutputSettings(soap, &soap_tmp___tds__SetRelayOutputSettings, "-tds:SetRelayOutputSettings", NULL)
			|| soap_body_end_out(soap)
			|| soap_envelope_end_out(soap))
		{
			return soap->error;
		}
	}
	if (soap_end_count(soap))
	{
		return soap->error;
	}
	if (soap_connect(soap, soap_endpoint, soap_action)
		|| soap_envelope_begin_out(soap)
		|| soap_putheader(soap)
		|| soap_body_begin_out(soap)
		|| soap_put___tds__SetRelayOutputSettings(soap, &soap_tmp___tds__SetRelayOutputSettings, "-tds:SetRelayOutputSettings", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
	{
		return soap_closesock(soap);
	}
	if (NULL==tds__SetRelayOutputSettingsResponse)
	{
		return soap_closesock(soap);
	}
	soap_default__tds__SetRelayOutputSettingsResponse(soap, tds__SetRelayOutputSettingsResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
	{
		return soap_closesock(soap);
	}
	soap_get__tds__SetRelayOutputSettingsResponse(soap, tds__SetRelayOutputSettingsResponse, "tds:SetRelayOutputSettingsResponse", "");
	if (soap->error)
	{
		return soap_recv_fault(soap, 0);
	}
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
	{
		return soap_closesock(soap);
	}
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SetRelayOutputState(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SetRelayOutputState *tds__SetRelayOutputState, struct _tds__SetRelayOutputStateResponse *tds__SetRelayOutputStateResponse)
{	
	struct __tds__SetRelayOutputState soap_tmp___tds__SetRelayOutputState;
	if (NULL==soap_action)
	{
		soap_action = "http://www.onvif.org/ver10/device/wsdl/SetRelayOutputState";
	}
	soap->encodingStyle = NULL;
	soap_tmp___tds__SetRelayOutputState.tds__SetRelayOutputState = tds__SetRelayOutputState;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__SetRelayOutputState(soap, &soap_tmp___tds__SetRelayOutputState);
	if (soap_begin_count(soap))
	{
		return soap->error;
	}
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__SetRelayOutputState(soap, &soap_tmp___tds__SetRelayOutputState, "-tds:SetRelayOutputState", NULL)
			|| soap_body_end_out(soap)
			|| soap_envelope_end_out(soap))
		{
			return soap->error;
		}
	}
	if (soap_end_count(soap))
	{
		return soap->error;
	}
	if (soap_connect(soap, soap_endpoint, soap_action)
		|| soap_envelope_begin_out(soap)
		|| soap_putheader(soap)
		|| soap_body_begin_out(soap)
		|| soap_put___tds__SetRelayOutputState(soap, &soap_tmp___tds__SetRelayOutputState, "-tds:SetRelayOutputState", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
	{
		return soap_closesock(soap);
	}
	if (NULL==tds__SetRelayOutputStateResponse)
	{
		return soap_closesock(soap);
	}
	soap_default__tds__SetRelayOutputStateResponse(soap, tds__SetRelayOutputStateResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
	{
		return soap_closesock(soap);
	}
	soap_get__tds__SetRelayOutputStateResponse(soap, tds__SetRelayOutputStateResponse, "tds:SetRelayOutputStateResponse", "");
	if (soap->error)
	{
		return soap_recv_fault(soap, 0);
	}
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
	{
		return soap_closesock(soap);
	}
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_call___tds__SendAuxiliaryCommand(struct soap *soap, const char *soap_endpoint, const char *soap_action, struct _tds__SendAuxiliaryCommand *tds__SendAuxiliaryCommand, struct _tds__SendAuxiliaryCommandResponse *tds__SendAuxiliaryCommandResponse)
{	
	struct __tds__SendAuxiliaryCommand soap_tmp___tds__SendAuxiliaryCommand;
	if (NULL==soap_action)
	{
		soap_action = "http://www.onvif.org/ver10/device/wsdl/SendAuxiliaryCommand";
	}
	soap->encodingStyle = NULL;
	soap_tmp___tds__SendAuxiliaryCommand.tds__SendAuxiliaryCommand = tds__SendAuxiliaryCommand;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___tds__SendAuxiliaryCommand(soap, &soap_tmp___tds__SendAuxiliaryCommand);
	if (soap_begin_count(soap))
	{
		return soap->error;
	}
	if (soap->mode & SOAP_IO_LENGTH)
	{	
		if (soap_envelope_begin_out(soap)
			|| soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___tds__SendAuxiliaryCommand(soap, &soap_tmp___tds__SendAuxiliaryCommand, "-tds:SendAuxiliaryCommand", NULL)
			|| soap_body_end_out(soap)
			|| soap_envelope_end_out(soap))
		{
			return soap->error;
		}
	}
	if (soap_end_count(soap))
	{
		return soap->error;
	}
	if (soap_connect(soap, soap_endpoint, soap_action)
		|| soap_envelope_begin_out(soap)
		|| soap_putheader(soap)
		|| soap_body_begin_out(soap)
		|| soap_put___tds__SendAuxiliaryCommand(soap, &soap_tmp___tds__SendAuxiliaryCommand, "-tds:SendAuxiliaryCommand", NULL)
		|| soap_body_end_out(soap)
		|| soap_envelope_end_out(soap)
		|| soap_end_send(soap))
	{
		return soap_closesock(soap);
	}
	if (NULL==tds__SendAuxiliaryCommandResponse)
	{
		return soap_closesock(soap);
	}
	soap_default__tds__SendAuxiliaryCommandResponse(soap, tds__SendAuxiliaryCommandResponse);
	if (soap_begin_recv(soap)
		|| soap_envelope_begin_in(soap)
		|| soap_recv_header(soap)
		|| soap_body_begin_in(soap))
	{
		return soap_closesock(soap);
	}
	soap_get__tds__SendAuxiliaryCommandResponse(soap, tds__SendAuxiliaryCommandResponse, "tds:SendAuxiliaryCommandResponse", "");
	if (soap->error)
	{
		return soap_recv_fault(soap, 0);
	}
	if (soap_body_end_in(soap)
		|| soap_envelope_end_in(soap)
		|| soap_end_recv(soap))
	{
		return soap_closesock(soap);
	}
	return soap_closesock(soap);
}

//device alarm


#ifdef __cplusplus
}
#endif

#if defined(__BORLANDC__)
#pragma option pop
#pragma option pop
#endif