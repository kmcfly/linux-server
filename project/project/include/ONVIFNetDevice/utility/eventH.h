#ifndef eventH_H
#define eventH_H

#include "eventStub.h"
#include "OnvifSoapStub.h"


#ifdef __cplusplus
extern "C" {
#endif

#ifndef SOAP_TYPE___denc__Notify
#define SOAP_TYPE___denc__Notify (1764)
#endif

SOAP_FMAC3 void SOAP_FMAC4 soap_default___denc__Notify(struct soap*, struct __denc__Notify *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___denc__Notify(struct soap*, const struct __denc__Notify *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out___denc__Notify(struct soap*, const char*, int, const struct __denc__Notify *, const char*);
SOAP_FMAC3 struct __denc__Notify * SOAP_FMAC4 soap_in___denc__Notify(struct soap*, const char*, struct __denc__Notify *, const char*);

#ifndef soap_write___denc__Notify
#define soap_write___denc__Notify(soap, data) ( soap_begin_send(soap) || (soap_serialize___denc__Notify(soap, data), 0) || soap_put___denc__Notify(soap, data, "-denc:Notify", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put___denc__Notify(struct soap*, const struct __denc__Notify *, const char*, const char*);

#ifndef soap_read___denc__Notify
#define soap_read___denc__Notify(soap, data) ( soap_begin_recv(soap) || !soap_get___denc__Notify(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct __denc__Notify * SOAP_FMAC4 soap_get___denc__Notify(struct soap*, struct __denc__Notify *, const char*, const char*);

//#ifndef SOAP_TYPE_time
//#define SOAP_TYPE_time (395)
//#endif
//SOAP_FMAC3 void SOAP_FMAC4 soap_default_time(struct soap*, time_t *);
//SOAP_FMAC3 int SOAP_FMAC4 soap_out_time(struct soap*, const char*, int, const time_t *, const char*);
//SOAP_FMAC3 time_t * SOAP_FMAC4 soap_in_time(struct soap*, const char*, time_t *, const char*);
//
//#ifndef soap_write_time
//#define soap_write_time(soap, data) ( soap_begin_send(soap) || soap_put_time(soap, data, "dateTime", NULL) || soap_end_send(soap) )
//#endif
//
//
//SOAP_FMAC3 int SOAP_FMAC4 soap_put_time(struct soap*, const time_t *, const char*, const char*);
//
//#ifndef soap_read_time
//#define soap_read_time(soap, data) ( soap_begin_recv(soap) || !soap_get_time(soap, data, NULL, NULL) || soap_end_recv(soap) )
//#endif
//
//SOAP_FMAC3 time_t * SOAP_FMAC4 soap_get_time(struct soap*, time_t *, const char*, const char*);

#ifndef SOAP_TYPE__wsnt__SubscribeResponse
#define SOAP_TYPE__wsnt__SubscribeResponse (1242)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__wsnt__SubscribeResponse(struct soap*, struct _wsnt__SubscribeResponse *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__wsnt__SubscribeResponse(struct soap*, const struct _wsnt__SubscribeResponse *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__wsnt__SubscribeResponse(struct soap*, const char*, int, const struct _wsnt__SubscribeResponse *, const char*);
SOAP_FMAC3 struct _wsnt__SubscribeResponse * SOAP_FMAC4 soap_in__wsnt__SubscribeResponse(struct soap*, const char*, struct _wsnt__SubscribeResponse *, const char*);

#ifndef soap_write__wsnt__SubscribeResponse
#define soap_write__wsnt__SubscribeResponse(soap, data) ( soap_begin_send(soap) || (soap_serialize__wsnt__SubscribeResponse(soap, data), 0) || soap_put__wsnt__SubscribeResponse(soap, data, "wsnt:SubscribeResponse", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__wsnt__SubscribeResponse(struct soap*, const struct _wsnt__SubscribeResponse *, const char*, const char*);

#ifndef soap_read__wsnt__SubscribeResponse
#define soap_read__wsnt__SubscribeResponse(soap, data) ( soap_begin_recv(soap) || !soap_get__wsnt__SubscribeResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _wsnt__SubscribeResponse * SOAP_FMAC4 soap_get__wsnt__SubscribeResponse(struct soap*, struct _wsnt__SubscribeResponse *, const char*, const char*);

#ifndef SOAP_TYPE_PointerTo_wsnt__Notify
#define SOAP_TYPE_PointerTo_wsnt__Notify (1762)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_wsnt__Notify(struct soap*, struct _wsnt__Notify *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_wsnt__Notify(struct soap*, const char *, int, struct _wsnt__Notify *const*, const char *);
SOAP_FMAC3 struct _wsnt__Notify ** SOAP_FMAC4 soap_in_PointerTo_wsnt__Notify(struct soap*, const char*, struct _wsnt__Notify **, const char*);

#ifndef soap_write_PointerTo_wsnt__Notify
#define soap_write_PointerTo_wsnt__Notify(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_wsnt__Notify(soap, data), 0) || soap_put_PointerTo_wsnt__Notify(soap, data, "wsnt:Notify", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_wsnt__Notify(struct soap*, struct _wsnt__Notify *const*, const char*, const char*);

#ifndef soap_read_PointerTo_wsnt__Notify
#define soap_read_PointerTo_wsnt__Notify(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_wsnt__Notify(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _wsnt__Notify ** SOAP_FMAC4 soap_get_PointerTo_wsnt__Notify(struct soap*, struct _wsnt__Notify **, const char*, const char*);

#ifndef SOAP_TYPE__wsnt__Notify
#define SOAP_TYPE__wsnt__Notify (857)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__wsnt__Notify(struct soap*, struct _wsnt__Notify *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__wsnt__Notify(struct soap*, const struct _wsnt__Notify *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__wsnt__Notify(struct soap*, const char*, int, const struct _wsnt__Notify *, const char*);
SOAP_FMAC3 struct _wsnt__Notify * SOAP_FMAC4 soap_in__wsnt__Notify(struct soap*, const char*, struct _wsnt__Notify *, const char*);

#ifndef soap_write__wsnt__Notify
#define soap_write__wsnt__Notify(soap, data) ( soap_begin_send(soap) || (soap_serialize__wsnt__Notify(soap, data), 0) || soap_put__wsnt__Notify(soap, data, "wsnt:Notify", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__wsnt__Notify(struct soap*, const struct _wsnt__Notify *, const char*, const char*);

#ifndef soap_read__wsnt__Notify
#define soap_read__wsnt__Notify(soap, data) ( soap_begin_recv(soap) || !soap_get__wsnt__Notify(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _wsnt__Notify * SOAP_FMAC4 soap_get__wsnt__Notify(struct soap*, struct _wsnt__Notify *, const char*, const char*);

#ifndef SOAP_TYPE_PointerTo_wsnt__Subscribe
#define SOAP_TYPE_PointerTo_wsnt__Subscribe (1765)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_wsnt__Subscribe(struct soap*, struct _wsnt__Subscribe *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_wsnt__Subscribe(struct soap*, const char *, int, struct _wsnt__Subscribe *const*, const char *);
SOAP_FMAC3 struct _wsnt__Subscribe ** SOAP_FMAC4 soap_in_PointerTo_wsnt__Subscribe(struct soap*, const char*, struct _wsnt__Subscribe **, const char*);

#ifndef soap_write_PointerTo_wsnt__Subscribe
#define soap_write_PointerTo_wsnt__Subscribe(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_wsnt__Subscribe(soap, data), 0) || soap_put_PointerTo_wsnt__Subscribe(soap, data, "wsnt:Subscribe", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_wsnt__Subscribe(struct soap*, struct _wsnt__Subscribe *const*, const char*, const char*);

#ifndef soap_read_PointerTo_wsnt__Subscribe
#define soap_read_PointerTo_wsnt__Subscribe(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_wsnt__Subscribe(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _wsnt__Subscribe ** SOAP_FMAC4 soap_get_PointerTo_wsnt__Subscribe(struct soap*, struct _wsnt__Subscribe **, const char*, const char*);

#ifndef SOAP_TYPE_PointerTo_wsnt__Renew
#define SOAP_TYPE_PointerTo_wsnt__Renew (1791)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_wsnt__Renew(struct soap*, struct _wsnt__Renew *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_wsnt__Renew(struct soap*, const char *, int, struct _wsnt__Renew *const*, const char *);
SOAP_FMAC3 struct _wsnt__Renew ** SOAP_FMAC4 soap_in_PointerTo_wsnt__Renew(struct soap*, const char*, struct _wsnt__Renew **, const char*);

#ifndef soap_write_PointerTo_wsnt__Renew
#define soap_write_PointerTo_wsnt__Renew(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_wsnt__Renew(soap, data), 0) || soap_put_PointerTo_wsnt__Renew(soap, data, "wsnt:Renew", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_wsnt__Renew(struct soap*, struct _wsnt__Renew *const*, const char*, const char*);

#ifndef soap_read_PointerTo_wsnt__Renew
#define soap_read_PointerTo_wsnt__Renew(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_wsnt__Renew(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _wsnt__Renew ** SOAP_FMAC4 soap_get_PointerTo_wsnt__Renew(struct soap*, struct _wsnt__Renew **, const char*, const char*);

#ifndef SOAP_TYPE_wsnt__NotificationMessageHolderType
#define SOAP_TYPE_wsnt__NotificationMessageHolderType (730)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_wsnt__NotificationMessageHolderType(struct soap*, struct wsnt__NotificationMessageHolderType *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_wsnt__NotificationMessageHolderType(struct soap*, const struct wsnt__NotificationMessageHolderType *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_wsnt__NotificationMessageHolderType(struct soap*, const char*, int, const struct wsnt__NotificationMessageHolderType *, const char*);
SOAP_FMAC3 struct wsnt__NotificationMessageHolderType * SOAP_FMAC4 soap_in_wsnt__NotificationMessageHolderType(struct soap*, const char*, struct wsnt__NotificationMessageHolderType *, const char*);

#ifndef soap_write_wsnt__NotificationMessageHolderType
#define soap_write_wsnt__NotificationMessageHolderType(soap, data) ( soap_begin_send(soap) || (soap_serialize_wsnt__NotificationMessageHolderType(soap, data), 0) || soap_put_wsnt__NotificationMessageHolderType(soap, data, "wsnt:NotificationMessageHolderType", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put_wsnt__NotificationMessageHolderType(struct soap*, const struct wsnt__NotificationMessageHolderType *, const char*, const char*);

#ifndef soap_read_wsnt__NotificationMessageHolderType
#define soap_read_wsnt__NotificationMessageHolderType(soap, data) ( soap_begin_recv(soap) || !soap_get_wsnt__NotificationMessageHolderType(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct wsnt__NotificationMessageHolderType * SOAP_FMAC4 soap_get_wsnt__NotificationMessageHolderType(struct soap*, struct wsnt__NotificationMessageHolderType *, const char*, const char*);

//#ifndef SOAP_TYPE__wsnt__NotificationMessageHolderType_Message
//#define SOAP_TYPE__wsnt__NotificationMessageHolderType_Message (1231)
//#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__wsnt__NotificationMessageHolderType_Message(struct soap*, struct _wsnt__NotificationMessageHolderType_Message *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__wsnt__NotificationMessageHolderType_Message(struct soap*, const struct _wsnt__NotificationMessageHolderType_Message *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__wsnt__NotificationMessageHolderType_Message(struct soap*, const char*, int, const struct _wsnt__NotificationMessageHolderType_Message *, const char*);
SOAP_FMAC3 struct _wsnt__NotificationMessageHolderType_Message * SOAP_FMAC4 soap_in__wsnt__NotificationMessageHolderType_Message(struct soap*, const char*, struct _wsnt__NotificationMessageHolderType_Message *, const char*);

#ifndef soap_write__wsnt__NotificationMessageHolderType_Message
#define soap_write__wsnt__NotificationMessageHolderType_Message(soap, data) ( soap_begin_send(soap) || (soap_serialize__wsnt__NotificationMessageHolderType_Message(soap, data), 0) || soap_put__wsnt__NotificationMessageHolderType_Message(soap, data, "wsnt:NotificationMessageHolderType-Message", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__wsnt__NotificationMessageHolderType_Message(struct soap*, const struct _wsnt__NotificationMessageHolderType_Message *, const char*, const char*);

#ifndef soap_read__wsnt__NotificationMessageHolderType_Message
#define soap_read__wsnt__NotificationMessageHolderType_Message(soap, data) ( soap_begin_recv(soap) || !soap_get__wsnt__NotificationMessageHolderType_Message(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _wsnt__NotificationMessageHolderType_Message * SOAP_FMAC4 soap_get__wsnt__NotificationMessageHolderType_Message(struct soap*, struct _wsnt__NotificationMessageHolderType_Message *, const char*, const char*);

#ifndef SOAP_TYPE__wsnt__Subscribe
#define SOAP_TYPE__wsnt__Subscribe (1239)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__wsnt__Subscribe(struct soap*, struct _wsnt__Subscribe *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__wsnt__Subscribe(struct soap*, const struct _wsnt__Subscribe *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__wsnt__Subscribe(struct soap*, const char*, int, const struct _wsnt__Subscribe *, const char*);
SOAP_FMAC3 struct _wsnt__Subscribe * SOAP_FMAC4 soap_in__wsnt__Subscribe(struct soap*, const char*, struct _wsnt__Subscribe *, const char*);

#ifndef soap_write__wsnt__Subscribe
#define soap_write__wsnt__Subscribe(soap, data) ( soap_begin_send(soap) || (soap_serialize__wsnt__Subscribe(soap, data), 0) || soap_put__wsnt__Subscribe(soap, data, "wsnt:Subscribe", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__wsnt__Subscribe(struct soap*, const struct _wsnt__Subscribe *, const char*, const char*);

#ifndef soap_read__wsnt__Subscribe
#define soap_read__wsnt__Subscribe(soap, data) ( soap_begin_recv(soap) || !soap_get__wsnt__Subscribe(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _wsnt__Subscribe * SOAP_FMAC4 soap_get__wsnt__Subscribe(struct soap*, struct _wsnt__Subscribe *, const char*, const char*);

#ifndef SOAP_TYPE__wsnt__Renew
#define SOAP_TYPE__wsnt__Renew (1252)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__wsnt__Renew(struct soap*, struct _wsnt__Renew *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__wsnt__Renew(struct soap*, const struct _wsnt__Renew *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__wsnt__Renew(struct soap*, const char*, int, const struct _wsnt__Renew *, const char*);
SOAP_FMAC3 struct _wsnt__Renew * SOAP_FMAC4 soap_in__wsnt__Renew(struct soap*, const char*, struct _wsnt__Renew *, const char*);

#ifndef soap_write__wsnt__Renew
#define soap_write__wsnt__Renew(soap, data) ( soap_begin_send(soap) || (soap_serialize__wsnt__Renew(soap, data), 0) || soap_put__wsnt__Renew(soap, data, "wsnt:Renew", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__wsnt__Renew(struct soap*, const struct _wsnt__Renew *, const char*, const char*);

#ifndef soap_read__wsnt__Renew
#define soap_read__wsnt__Renew(soap, data) ( soap_begin_recv(soap) || !soap_get__wsnt__Renew(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _wsnt__Renew * SOAP_FMAC4 soap_get__wsnt__Renew(struct soap*, struct _wsnt__Renew *, const char*, const char*);

#ifndef SOAP_TYPE_PointerTo_wsnt__Subscribe_SubscriptionPolicy
#define SOAP_TYPE_PointerTo_wsnt__Subscribe_SubscriptionPolicy (1241)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_wsnt__Subscribe_SubscriptionPolicy(struct soap*, struct _wsnt__Subscribe_SubscriptionPolicy *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_wsnt__Subscribe_SubscriptionPolicy(struct soap*, const char *, int, struct _wsnt__Subscribe_SubscriptionPolicy *const*, const char *);
SOAP_FMAC3 struct _wsnt__Subscribe_SubscriptionPolicy ** SOAP_FMAC4 soap_in_PointerTo_wsnt__Subscribe_SubscriptionPolicy(struct soap*, const char*, struct _wsnt__Subscribe_SubscriptionPolicy **, const char*);

#ifndef soap_write_PointerTo_wsnt__Subscribe_SubscriptionPolicy
#define soap_write_PointerTo_wsnt__Subscribe_SubscriptionPolicy(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_wsnt__Subscribe_SubscriptionPolicy(soap, data), 0) || soap_put_PointerTo_wsnt__Subscribe_SubscriptionPolicy(soap, data, "wsnt:Subscribe-SubscriptionPolicy", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_wsnt__Subscribe_SubscriptionPolicy(struct soap*, struct _wsnt__Subscribe_SubscriptionPolicy *const*, const char*, const char*);

#ifndef soap_read_PointerTo_wsnt__Subscribe_SubscriptionPolicy
#define soap_read_PointerTo_wsnt__Subscribe_SubscriptionPolicy(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_wsnt__Subscribe_SubscriptionPolicy(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _wsnt__Subscribe_SubscriptionPolicy ** SOAP_FMAC4 soap_get_PointerTo_wsnt__Subscribe_SubscriptionPolicy(struct soap*, struct _wsnt__Subscribe_SubscriptionPolicy **, const char*, const char*);

#ifndef SOAP_TYPE__wsnt__Subscribe_SubscriptionPolicy
#define SOAP_TYPE__wsnt__Subscribe_SubscriptionPolicy (1240)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__wsnt__Subscribe_SubscriptionPolicy(struct soap*, struct _wsnt__Subscribe_SubscriptionPolicy *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__wsnt__Subscribe_SubscriptionPolicy(struct soap*, const struct _wsnt__Subscribe_SubscriptionPolicy *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__wsnt__Subscribe_SubscriptionPolicy(struct soap*, const char*, int, const struct _wsnt__Subscribe_SubscriptionPolicy *, const char*);
SOAP_FMAC3 struct _wsnt__Subscribe_SubscriptionPolicy * SOAP_FMAC4 soap_in__wsnt__Subscribe_SubscriptionPolicy(struct soap*, const char*, struct _wsnt__Subscribe_SubscriptionPolicy *, const char*);

#ifndef soap_write__wsnt__Subscribe_SubscriptionPolicy
#define soap_write__wsnt__Subscribe_SubscriptionPolicy(soap, data) ( soap_begin_send(soap) || (soap_serialize__wsnt__Subscribe_SubscriptionPolicy(soap, data), 0) || soap_put__wsnt__Subscribe_SubscriptionPolicy(soap, data, "wsnt:Subscribe-SubscriptionPolicy", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__wsnt__Subscribe_SubscriptionPolicy(struct soap*, const struct _wsnt__Subscribe_SubscriptionPolicy *, const char*, const char*);

#ifndef soap_read__wsnt__Subscribe_SubscriptionPolicy
#define soap_read__wsnt__Subscribe_SubscriptionPolicy(soap, data) ( soap_begin_recv(soap) || !soap_get__wsnt__Subscribe_SubscriptionPolicy(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _wsnt__Subscribe_SubscriptionPolicy * SOAP_FMAC4 soap_get__wsnt__Subscribe_SubscriptionPolicy(struct soap*, struct _wsnt__Subscribe_SubscriptionPolicy *, const char*, const char*);

#ifndef SOAP_TYPE_wsnt__AbsoluteOrRelativeTimeType
#define SOAP_TYPE_wsnt__AbsoluteOrRelativeTimeType (135)
#endif

#define soap_default_wsnt__AbsoluteOrRelativeTimeType(soap, a) soap_default_string(soap, a)


#define soap_serialize_wsnt__AbsoluteOrRelativeTimeType(soap, a) soap_serialize_string(soap, a)

SOAP_FMAC3 int SOAP_FMAC4 soap_out_wsnt__AbsoluteOrRelativeTimeType(struct soap*, const char*, int, char*const*, const char*);
SOAP_FMAC3 char * * SOAP_FMAC4 soap_in_wsnt__AbsoluteOrRelativeTimeType(struct soap*, const char*, char **, const char*);

#ifndef soap_write_wsnt__AbsoluteOrRelativeTimeType
#define soap_write_wsnt__AbsoluteOrRelativeTimeType(soap, data) ( soap_begin_send(soap) || (soap_serialize_wsnt__AbsoluteOrRelativeTimeType(soap, data), 0) || soap_put_wsnt__AbsoluteOrRelativeTimeType(soap, data, "byte", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_wsnt__AbsoluteOrRelativeTimeType(struct soap*, char *const*, const char*, const char*);

#ifndef soap_read_wsnt__AbsoluteOrRelativeTimeType
#define soap_read_wsnt__AbsoluteOrRelativeTimeType(soap, data) ( soap_begin_recv(soap) || !soap_get_wsnt__AbsoluteOrRelativeTimeType(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 char ** SOAP_FMAC4 soap_get_wsnt__AbsoluteOrRelativeTimeType(struct soap*, char **, const char*, const char*);

#ifndef SOAP_TYPE_PointerTowsnt__FilterType
#define SOAP_TYPE_PointerTowsnt__FilterType (571)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTowsnt__FilterType(struct soap*, struct wsnt__FilterType *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTowsnt__FilterType(struct soap*, const char *, int, struct wsnt__FilterType *const*, const char *);
SOAP_FMAC3 struct wsnt__FilterType ** SOAP_FMAC4 soap_in_PointerTowsnt__FilterType(struct soap*, const char*, struct wsnt__FilterType **, const char*);

#ifndef soap_write_PointerTowsnt__FilterType
#define soap_write_PointerTowsnt__FilterType(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTowsnt__FilterType(soap, data), 0) || soap_put_PointerTowsnt__FilterType(soap, data, "wsnt:FilterType", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTowsnt__FilterType(struct soap*, struct wsnt__FilterType *const*, const char*, const char*);

#ifndef soap_read_PointerTowsnt__FilterType
#define soap_read_PointerTowsnt__FilterType(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTowsnt__FilterType(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct wsnt__FilterType ** SOAP_FMAC4 soap_get_PointerTowsnt__FilterType(struct soap*, struct wsnt__FilterType **, const char*, const char*);

#ifndef SOAP_TYPE_PointerTowsnt__TopicExpressionType
#define SOAP_TYPE_PointerTowsnt__TopicExpressionType (1230)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTowsnt__TopicExpressionType(struct soap*, struct wsnt__TopicExpressionType *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTowsnt__TopicExpressionType(struct soap*, const char *, int, struct wsnt__TopicExpressionType *const*, const char *);
SOAP_FMAC3 struct wsnt__TopicExpressionType ** SOAP_FMAC4 soap_in_PointerTowsnt__TopicExpressionType(struct soap*, const char*, struct wsnt__TopicExpressionType **, const char*);

#ifndef soap_write_PointerTowsnt__TopicExpressionType
#define soap_write_PointerTowsnt__TopicExpressionType(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTowsnt__TopicExpressionType(soap, data), 0) || soap_put_PointerTowsnt__TopicExpressionType(soap, data, "wsnt:TopicExpressionType", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTowsnt__TopicExpressionType(struct soap*, struct wsnt__TopicExpressionType *const*, const char*, const char*);

#ifndef soap_read_PointerTowsnt__TopicExpressionType
#define soap_read_PointerTowsnt__TopicExpressionType(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTowsnt__TopicExpressionType(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct wsnt__TopicExpressionType ** SOAP_FMAC4 soap_get_PointerTowsnt__TopicExpressionType(struct soap*, struct wsnt__TopicExpressionType **, const char*, const char*);

#ifndef SOAP_TYPE_wsnt__TopicExpressionType
#define SOAP_TYPE_wsnt__TopicExpressionType (1227)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_wsnt__TopicExpressionType(struct soap*, struct wsnt__TopicExpressionType *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_wsnt__TopicExpressionType(struct soap*, const struct wsnt__TopicExpressionType *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_wsnt__TopicExpressionType(struct soap*, const char*, int, const struct wsnt__TopicExpressionType *, const char*);
SOAP_FMAC3 struct wsnt__TopicExpressionType * SOAP_FMAC4 soap_in_wsnt__TopicExpressionType(struct soap*, const char*, struct wsnt__TopicExpressionType *, const char*);

#ifndef soap_write_wsnt__TopicExpressionType
#define soap_write_wsnt__TopicExpressionType(soap, data) ( soap_begin_send(soap) || (soap_serialize_wsnt__TopicExpressionType(soap, data), 0) || soap_put_wsnt__TopicExpressionType(soap, data, "wsnt:TopicExpressionType", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put_wsnt__TopicExpressionType(struct soap*, const struct wsnt__TopicExpressionType *, const char*, const char*);

#ifndef soap_read_wsnt__TopicExpressionType
#define soap_read_wsnt__TopicExpressionType(soap, data) ( soap_begin_recv(soap) || !soap_get_wsnt__TopicExpressionType(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct wsnt__TopicExpressionType * SOAP_FMAC4 soap_get_wsnt__TopicExpressionType(struct soap*, struct wsnt__TopicExpressionType *, const char*, const char*);

#ifndef SOAP_TYPE_PointerTowsa5__EndpointReferenceType
#define SOAP_TYPE_PointerTowsa5__EndpointReferenceType (1229)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTowsa5__EndpointReferenceType(struct soap*, struct wsa5__EndpointReferenceType *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTowsa5__EndpointReferenceType(struct soap*, const char *, int, struct wsa5__EndpointReferenceType *const*, const char *);
SOAP_FMAC3 struct wsa5__EndpointReferenceType ** SOAP_FMAC4 soap_in_PointerTowsa5__EndpointReferenceType(struct soap*, const char*, struct wsa5__EndpointReferenceType **, const char*);

#ifndef soap_write_PointerTowsa5__EndpointReferenceType
#define soap_write_PointerTowsa5__EndpointReferenceType(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTowsa5__EndpointReferenceType(soap, data), 0) || soap_put_PointerTowsa5__EndpointReferenceType(soap, data, "wsa5:EndpointReferenceType", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTowsa5__EndpointReferenceType(struct soap*, struct wsa5__EndpointReferenceType *const*, const char*, const char*);

#ifndef soap_read_PointerTowsa5__EndpointReferenceType
#define soap_read_PointerTowsa5__EndpointReferenceType(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTowsa5__EndpointReferenceType(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct wsa5__EndpointReferenceType ** SOAP_FMAC4 soap_get_PointerTowsa5__EndpointReferenceType(struct soap*, struct wsa5__EndpointReferenceType **, const char*, const char*);

#ifndef SOAP_TYPE_wsnt__FilterType
#define SOAP_TYPE_wsnt__FilterType (570)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_wsnt__FilterType(struct soap*, struct wsnt__FilterType *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_wsnt__FilterType(struct soap*, const struct wsnt__FilterType *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_wsnt__FilterType(struct soap*, const char*, int, const struct wsnt__FilterType *, const char*);
SOAP_FMAC3 struct wsnt__FilterType * SOAP_FMAC4 soap_in_wsnt__FilterType(struct soap*, const char*, struct wsnt__FilterType *, const char*);

#ifndef soap_write_wsnt__FilterType
#define soap_write_wsnt__FilterType(soap, data) ( soap_begin_send(soap) || (soap_serialize_wsnt__FilterType(soap, data), 0) || soap_put_wsnt__FilterType(soap, data, "wsnt:FilterType", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put_wsnt__FilterType(struct soap*, const struct wsnt__FilterType *, const char*, const char*);

#ifndef soap_read_wsnt__FilterType
#define soap_read_wsnt__FilterType(soap, data) ( soap_begin_recv(soap) || !soap_get_wsnt__FilterType(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct wsnt__FilterType * SOAP_FMAC4 soap_get_wsnt__FilterType(struct soap*, struct wsnt__FilterType *, const char*, const char*);

#ifndef SOAP_TYPE___desm__Unsubscribe
#define SOAP_TYPE___desm__Unsubscribe (1810)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default___desm__Unsubscribe(struct soap*, struct __desm__Unsubscribe *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___desm__Unsubscribe(struct soap*, const struct __desm__Unsubscribe *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out___desm__Unsubscribe(struct soap*, const char*, int, const struct __desm__Unsubscribe *, const char*);
SOAP_FMAC3 struct __desm__Unsubscribe * SOAP_FMAC4 soap_in___desm__Unsubscribe(struct soap*, const char*, struct __desm__Unsubscribe *, const char*);

#ifndef soap_write___desm__Unsubscribe
#define soap_write___desm__Unsubscribe(soap, data) ( soap_begin_send(soap) || (soap_serialize___desm__Unsubscribe(soap, data), 0) || soap_put___desm__Unsubscribe(soap, data, "-desm:Unsubscribe", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put___desm__Unsubscribe(struct soap*, const struct __desm__Unsubscribe *, const char*, const char*);

#ifndef soap_read___desm__Unsubscribe
#define soap_read___desm__Unsubscribe(soap, data) ( soap_begin_recv(soap) || !soap_get___desm__Unsubscribe(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct __desm__Unsubscribe * SOAP_FMAC4 soap_get___desm__Unsubscribe(struct soap*, struct __desm__Unsubscribe *, const char*, const char*);


#ifndef SOAP_TYPE_PointerTo_wsnt__UnsubscribeResponse
#define SOAP_TYPE_PointerTo_wsnt__UnsubscribeResponse (1796)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_wsnt__UnsubscribeResponse(struct soap*, struct _wsnt__UnsubscribeResponse *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_wsnt__UnsubscribeResponse(struct soap*, const char *, int, struct _wsnt__UnsubscribeResponse *const*, const char *);
SOAP_FMAC3 struct _wsnt__UnsubscribeResponse ** SOAP_FMAC4 soap_in_PointerTo_wsnt__UnsubscribeResponse(struct soap*, const char*, struct _wsnt__UnsubscribeResponse **, const char*);

#ifndef soap_write_PointerTo_wsnt__UnsubscribeResponse
#define soap_write_PointerTo_wsnt__UnsubscribeResponse(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_wsnt__UnsubscribeResponse(soap, data), 0) || soap_put_PointerTo_wsnt__UnsubscribeResponse(soap, data, "wsnt:UnsubscribeResponse", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_wsnt__UnsubscribeResponse(struct soap*, struct _wsnt__UnsubscribeResponse *const*, const char*, const char*);

#ifndef soap_read_PointerTo_wsnt__UnsubscribeResponse
#define soap_read_PointerTo_wsnt__UnsubscribeResponse(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_wsnt__UnsubscribeResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _wsnt__UnsubscribeResponse ** SOAP_FMAC4 soap_get_PointerTo_wsnt__UnsubscribeResponse(struct soap*, struct _wsnt__UnsubscribeResponse **, const char*, const char*);

#ifndef SOAP_TYPE__wsnt__UnsubscribeResponse
#define SOAP_TYPE__wsnt__UnsubscribeResponse (1255)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__wsnt__UnsubscribeResponse(struct soap*, struct _wsnt__UnsubscribeResponse *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__wsnt__UnsubscribeResponse(struct soap*, const struct _wsnt__UnsubscribeResponse *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__wsnt__UnsubscribeResponse(struct soap*, const char*, int, const struct _wsnt__UnsubscribeResponse *, const char*);
SOAP_FMAC3 struct _wsnt__UnsubscribeResponse * SOAP_FMAC4 soap_in__wsnt__UnsubscribeResponse(struct soap*, const char*, struct _wsnt__UnsubscribeResponse *, const char*);

#ifndef soap_write__wsnt__UnsubscribeResponse
#define soap_write__wsnt__UnsubscribeResponse(soap, data) ( soap_begin_send(soap) || (soap_serialize__wsnt__UnsubscribeResponse(soap, data), 0) || soap_put__wsnt__UnsubscribeResponse(soap, data, "wsnt:UnsubscribeResponse", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__wsnt__UnsubscribeResponse(struct soap*, const struct _wsnt__UnsubscribeResponse *, const char*, const char*);

#ifndef soap_read__wsnt__UnsubscribeResponse
#define soap_read__wsnt__UnsubscribeResponse(soap, data) ( soap_begin_recv(soap) || !soap_get__wsnt__UnsubscribeResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _wsnt__UnsubscribeResponse * SOAP_FMAC4 soap_get__wsnt__UnsubscribeResponse(struct soap*, struct _wsnt__UnsubscribeResponse *, const char*, const char*);

#ifndef SOAP_TYPE_PointerTo_wsnt__Unsubscribe
#define SOAP_TYPE_PointerTo_wsnt__Unsubscribe (1795)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_wsnt__Unsubscribe(struct soap*, struct _wsnt__Unsubscribe *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_wsnt__Unsubscribe(struct soap*, const char *, int, struct _wsnt__Unsubscribe *const*, const char *);
SOAP_FMAC3 struct _wsnt__Unsubscribe ** SOAP_FMAC4 soap_in_PointerTo_wsnt__Unsubscribe(struct soap*, const char*, struct _wsnt__Unsubscribe **, const char*);

#ifndef soap_write_PointerTo_wsnt__Unsubscribe
#define soap_write_PointerTo_wsnt__Unsubscribe(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_wsnt__Unsubscribe(soap, data), 0) || soap_put_PointerTo_wsnt__Unsubscribe(soap, data, "wsnt:Unsubscribe", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_wsnt__Unsubscribe(struct soap*, struct _wsnt__Unsubscribe *const*, const char*, const char*);

#ifndef soap_read_PointerTo_wsnt__Unsubscribe
#define soap_read_PointerTo_wsnt__Unsubscribe(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_wsnt__Unsubscribe(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _wsnt__Unsubscribe ** SOAP_FMAC4 soap_get_PointerTo_wsnt__Unsubscribe(struct soap*, struct _wsnt__Unsubscribe **, const char*, const char*);

//#ifndef SOAP_TYPE_PointerTo_wsnt__UnsubscribeResponse
//#define SOAP_TYPE_PointerTo_wsnt__UnsubscribeResponse (1796)
//#endif
//SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_wsnt__UnsubscribeResponse(struct soap*, struct _wsnt__UnsubscribeResponse *const*);
//SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_wsnt__UnsubscribeResponse(struct soap*, const char *, int, struct _wsnt__UnsubscribeResponse *const*, const char *);
//SOAP_FMAC3 struct _wsnt__UnsubscribeResponse ** SOAP_FMAC4 soap_in_PointerTo_wsnt__UnsubscribeResponse(struct soap*, const char*, struct _wsnt__UnsubscribeResponse **, const char*);
//
//#ifndef soap_write_PointerTo_wsnt__UnsubscribeResponse
//#define soap_write_PointerTo_wsnt__UnsubscribeResponse(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_wsnt__UnsubscribeResponse(soap, data), 0) || soap_put_PointerTo_wsnt__UnsubscribeResponse(soap, data, "wsnt:UnsubscribeResponse", NULL) || soap_end_send(soap) )
//#endif
//
//SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_wsnt__UnsubscribeResponse(struct soap*, struct _wsnt__UnsubscribeResponse *const*, const char*, const char*);
//
//#ifndef soap_read_PointerTo_wsnt__UnsubscribeResponse
//#define soap_read_PointerTo_wsnt__UnsubscribeResponse(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_wsnt__UnsubscribeResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
//#endif
//
//SOAP_FMAC3 struct _wsnt__UnsubscribeResponse ** SOAP_FMAC4 soap_get_PointerTo_wsnt__UnsubscribeResponse(struct soap*, struct _wsnt__UnsubscribeResponse **, const char*, const char*);

#ifndef SOAP_TYPE__wsnt__Unsubscribe
#define SOAP_TYPE__wsnt__Unsubscribe (1254)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__wsnt__Unsubscribe(struct soap*, struct _wsnt__Unsubscribe *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__wsnt__Unsubscribe(struct soap*, const struct _wsnt__Unsubscribe *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__wsnt__Unsubscribe(struct soap*, const char*, int, const struct _wsnt__Unsubscribe *, const char*);
SOAP_FMAC3 struct _wsnt__Unsubscribe * SOAP_FMAC4 soap_in__wsnt__Unsubscribe(struct soap*, const char*, struct _wsnt__Unsubscribe *, const char*);

#ifndef soap_write__wsnt__Unsubscribe
#define soap_write__wsnt__Unsubscribe(soap, data) ( soap_begin_send(soap) || (soap_serialize__wsnt__Unsubscribe(soap, data), 0) || soap_put__wsnt__Unsubscribe(soap, data, "wsnt:Unsubscribe", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__wsnt__Unsubscribe(struct soap*, const struct _wsnt__Unsubscribe *, const char*, const char*);

#ifndef soap_read__wsnt__Unsubscribe
#define soap_read__wsnt__Unsubscribe(soap, data) ( soap_begin_recv(soap) || !soap_get__wsnt__Unsubscribe(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _wsnt__Unsubscribe * SOAP_FMAC4 soap_get__wsnt__Unsubscribe(struct soap*, struct _wsnt__Unsubscribe *, const char*, const char*);

#ifndef SOAP_TYPE__wsnt__UnsubscribeResponse
#define SOAP_TYPE__wsnt__UnsubscribeResponse (1255)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__wsnt__UnsubscribeResponse(struct soap*, struct _wsnt__UnsubscribeResponse *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__wsnt__UnsubscribeResponse(struct soap*, const struct _wsnt__UnsubscribeResponse *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__wsnt__UnsubscribeResponse(struct soap*, const char*, int, const struct _wsnt__UnsubscribeResponse *, const char*);
SOAP_FMAC3 struct _wsnt__UnsubscribeResponse * SOAP_FMAC4 soap_in__wsnt__UnsubscribeResponse(struct soap*, const char*, struct _wsnt__UnsubscribeResponse *, const char*);

#ifndef soap_write__wsnt__UnsubscribeResponse
#define soap_write__wsnt__UnsubscribeResponse(soap, data) ( soap_begin_send(soap) || (soap_serialize__wsnt__UnsubscribeResponse(soap, data), 0) || soap_put__wsnt__UnsubscribeResponse(soap, data, "wsnt:UnsubscribeResponse", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__wsnt__UnsubscribeResponse(struct soap*, const struct _wsnt__UnsubscribeResponse *, const char*, const char*);

#ifndef soap_read__wsnt__UnsubscribeResponse
#define soap_read__wsnt__UnsubscribeResponse(soap, data) ( soap_begin_recv(soap) || !soap_get__wsnt__UnsubscribeResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _wsnt__UnsubscribeResponse * SOAP_FMAC4 soap_get__wsnt__UnsubscribeResponse(struct soap*, struct _wsnt__UnsubscribeResponse *, const char*, const char*);

#ifndef SOAP_TYPE___denf__Subscribe
#define SOAP_TYPE___denf__Subscribe (1768)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default___denf__Subscribe(struct soap*, struct __denf__Subscribe *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___denf__Subscribe(struct soap*, const struct __denf__Subscribe *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out___denf__Subscribe(struct soap*, const char*, int, const struct __denf__Subscribe *, const char*);
SOAP_FMAC3 struct __denf__Subscribe * SOAP_FMAC4 soap_in___denf__Subscribe(struct soap*, const char*, struct __denf__Subscribe *, const char*);

#ifndef soap_write___denf__Subscribe
#define soap_write___denf__Subscribe(soap, data) ( soap_begin_send(soap) || (soap_serialize___denf__Subscribe(soap, data), 0) || soap_put___denf__Subscribe(soap, data, "-denf:Subscribe", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put___denf__Subscribe(struct soap*, const struct __denf__Subscribe *, const char*, const char*);

#ifndef soap_read___denf__Subscribe
#define soap_read___denf__Subscribe(soap, data) ( soap_begin_recv(soap) || !soap_get___denf__Subscribe(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct __denf__Subscribe * SOAP_FMAC4 soap_get___denf__Subscribe(struct soap*, struct __denf__Subscribe *, const char*, const char*);

#ifndef SOAP_TYPE___desm__Renew
#define SOAP_TYPE___desm__Renew (1808)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default___desm__Renew(struct soap*, struct __desm__Renew *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___desm__Renew(struct soap*, const struct __desm__Renew *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out___desm__Renew(struct soap*, const char*, int, const struct __desm__Renew *, const char*);
SOAP_FMAC3 struct __desm__Renew * SOAP_FMAC4 soap_in___desm__Renew(struct soap*, const char*, struct __desm__Renew *, const char*);

#ifndef soap_write___desm__Renew
#define soap_write___desm__Renew(soap, data) ( soap_begin_send(soap) || (soap_serialize___desm__Renew(soap, data), 0) || soap_put___desm__Renew(soap, data, "-desm:Renew", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put___desm__Renew(struct soap*, const struct __desm__Renew *, const char*, const char*);

#ifndef soap_read___desm__Renew
#define soap_read___desm__Renew(soap, data) ( soap_begin_recv(soap) || !soap_get___desm__Renew(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct __desm__Renew * SOAP_FMAC4 soap_get___desm__Renew(struct soap*, struct __desm__Renew *, const char*, const char*);

#ifndef SOAP_TYPE__wsnt__RenewResponse
#define SOAP_TYPE__wsnt__RenewResponse (1253)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__wsnt__RenewResponse(struct soap*, struct _wsnt__RenewResponse *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__wsnt__RenewResponse(struct soap*, const struct _wsnt__RenewResponse *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__wsnt__RenewResponse(struct soap*, const char*, int, const struct _wsnt__RenewResponse *, const char*);
SOAP_FMAC3 struct _wsnt__RenewResponse * SOAP_FMAC4 soap_in__wsnt__RenewResponse(struct soap*, const char*, struct _wsnt__RenewResponse *, const char*);

#ifndef soap_write__wsnt__RenewResponse
#define soap_write__wsnt__RenewResponse(soap, data) ( soap_begin_send(soap) || (soap_serialize__wsnt__RenewResponse(soap, data), 0) || soap_put__wsnt__RenewResponse(soap, data, "wsnt:RenewResponse", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__wsnt__RenewResponse(struct soap*, const struct _wsnt__RenewResponse *, const char*, const char*);

#ifndef soap_read__wsnt__RenewResponse
#define soap_read__wsnt__RenewResponse(soap, data) ( soap_begin_recv(soap) || !soap_get__wsnt__RenewResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _wsnt__RenewResponse * SOAP_FMAC4 soap_get__wsnt__RenewResponse(struct soap*, struct _wsnt__RenewResponse *, const char*, const char*);


#ifndef SOAP_TYPE___dee__GetEventProperties
#define SOAP_TYPE___dee__GetEventProperties (2132)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default___dee__GetEventProperties(struct soap*, struct __dee__GetEventProperties *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___dee__GetEventProperties(struct soap*, const struct __dee__GetEventProperties *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out___dee__GetEventProperties(struct soap*, const char*, int, const struct __dee__GetEventProperties *, const char*);
SOAP_FMAC3 struct __dee__GetEventProperties * SOAP_FMAC4 soap_in___dee__GetEventProperties(struct soap*, const char*, struct __dee__GetEventProperties *, const char*);

#ifndef soap_write___dee__GetEventProperties
#define soap_write___dee__GetEventProperties(soap, data) ( soap_begin_send(soap) || (soap_serialize___dee__GetEventProperties(soap, data), 0) || soap_put___dee__GetEventProperties(soap, data, "-dee:GetEventProperties", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put___dee__GetEventProperties(struct soap*, const struct __dee__GetEventProperties *, const char*, const char*);

#ifndef soap_read___dee__GetEventProperties
#define soap_read___dee__GetEventProperties(soap, data) ( soap_begin_recv(soap) || !soap_get___dee__GetEventProperties(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct __dee__GetEventProperties * SOAP_FMAC4 soap_get___dee__GetEventProperties(struct soap*, struct __dee__GetEventProperties *, const char*, const char*);

#ifndef SOAP_TYPE_PointerTo_tev__GetEventPropertiesResponse
#define SOAP_TYPE_PointerTo_tev__GetEventPropertiesResponse (2130)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tev__GetEventPropertiesResponse(struct soap*, struct _tev__GetEventPropertiesResponse *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tev__GetEventPropertiesResponse(struct soap*, const char *, int, struct _tev__GetEventPropertiesResponse *const*, const char *);
SOAP_FMAC3 struct _tev__GetEventPropertiesResponse ** SOAP_FMAC4 soap_in_PointerTo_tev__GetEventPropertiesResponse(struct soap*, const char*, struct _tev__GetEventPropertiesResponse **, const char*);

#ifndef soap_write_PointerTo_tev__GetEventPropertiesResponse
#define soap_write_PointerTo_tev__GetEventPropertiesResponse(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_tev__GetEventPropertiesResponse(soap, data), 0) || soap_put_PointerTo_tev__GetEventPropertiesResponse(soap, data, "tev:GetEventPropertiesResponse", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tev__GetEventPropertiesResponse(struct soap*, struct _tev__GetEventPropertiesResponse *const*, const char*, const char*);

#ifndef soap_read_PointerTo_tev__GetEventPropertiesResponse
#define soap_read_PointerTo_tev__GetEventPropertiesResponse(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_tev__GetEventPropertiesResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tev__GetEventPropertiesResponse ** SOAP_FMAC4 soap_get_PointerTo_tev__GetEventPropertiesResponse(struct soap*, struct _tev__GetEventPropertiesResponse **, const char*, const char*);

#ifndef SOAP_TYPE_PointerTo_tev__GetEventProperties
#define SOAP_TYPE_PointerTo_tev__GetEventProperties (2129)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tev__GetEventProperties(struct soap*, struct _tev__GetEventProperties *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tev__GetEventProperties(struct soap*, const char *, int, struct _tev__GetEventProperties *const*, const char *);
SOAP_FMAC3 struct _tev__GetEventProperties ** SOAP_FMAC4 soap_in_PointerTo_tev__GetEventProperties(struct soap*, const char*, struct _tev__GetEventProperties **, const char*);

#ifndef soap_write_PointerTo_tev__GetEventProperties
#define soap_write_PointerTo_tev__GetEventProperties(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_tev__GetEventProperties(soap, data), 0) || soap_put_PointerTo_tev__GetEventProperties(soap, data, "tev:GetEventProperties", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tev__GetEventProperties(struct soap*, struct _tev__GetEventProperties *const*, const char*, const char*);

#ifndef soap_read_PointerTo_tev__GetEventProperties
#define soap_read_PointerTo_tev__GetEventProperties(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_tev__GetEventProperties(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tev__GetEventProperties ** SOAP_FMAC4 soap_get_PointerTo_tev__GetEventProperties(struct soap*, struct _tev__GetEventProperties **, const char*, const char*);

#ifndef SOAP_TYPE__tev__GetEventPropertiesResponse
#define SOAP_TYPE__tev__GetEventPropertiesResponse (1576)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__tev__GetEventPropertiesResponse(struct soap*, struct _tev__GetEventPropertiesResponse *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tev__GetEventPropertiesResponse(struct soap*, const struct _tev__GetEventPropertiesResponse *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__tev__GetEventPropertiesResponse(struct soap*, const char*, int, const struct _tev__GetEventPropertiesResponse *, const char*);
SOAP_FMAC3 struct _tev__GetEventPropertiesResponse * SOAP_FMAC4 soap_in__tev__GetEventPropertiesResponse(struct soap*, const char*, struct _tev__GetEventPropertiesResponse *, const char*);

#ifndef soap_write__tev__GetEventPropertiesResponse
#define soap_write__tev__GetEventPropertiesResponse(soap, data) ( soap_begin_send(soap) || (soap_serialize__tev__GetEventPropertiesResponse(soap, data), 0) || soap_put__tev__GetEventPropertiesResponse(soap, data, "tev:GetEventPropertiesResponse", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__tev__GetEventPropertiesResponse(struct soap*, const struct _tev__GetEventPropertiesResponse *, const char*, const char*);

#ifndef soap_read__tev__GetEventPropertiesResponse
#define soap_read__tev__GetEventPropertiesResponse(soap, data) ( soap_begin_recv(soap) || !soap_get__tev__GetEventPropertiesResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tev__GetEventPropertiesResponse * SOAP_FMAC4 soap_get__tev__GetEventPropertiesResponse(struct soap*, struct _tev__GetEventPropertiesResponse *, const char*, const char*);

#ifndef SOAP_TYPE__tev__GetEventProperties
#define SOAP_TYPE__tev__GetEventProperties (1575)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__tev__GetEventProperties(struct soap*, struct _tev__GetEventProperties *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tev__GetEventProperties(struct soap*, const struct _tev__GetEventProperties *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__tev__GetEventProperties(struct soap*, const char*, int, const struct _tev__GetEventProperties *, const char*);
SOAP_FMAC3 struct _tev__GetEventProperties * SOAP_FMAC4 soap_in__tev__GetEventProperties(struct soap*, const char*, struct _tev__GetEventProperties *, const char*);

#ifndef soap_write__tev__GetEventProperties
#define soap_write__tev__GetEventProperties(soap, data) ( soap_begin_send(soap) || (soap_serialize__tev__GetEventProperties(soap, data), 0) || soap_put__tev__GetEventProperties(soap, data, "tev:GetEventProperties", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__tev__GetEventProperties(struct soap*, const struct _tev__GetEventProperties *, const char*, const char*);

#ifndef soap_read__tev__GetEventProperties
#define soap_read__tev__GetEventProperties(soap, data) ( soap_begin_recv(soap) || !soap_get__tev__GetEventProperties(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tev__GetEventProperties * SOAP_FMAC4 soap_get__tev__GetEventProperties(struct soap*, struct _tev__GetEventProperties *, const char*, const char*);

#ifndef SOAP_TYPE_PointerTowstop__Documentation
#define SOAP_TYPE_PointerTowstop__Documentation (1119)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTowstop__Documentation(struct soap*, struct wstop__Documentation *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTowstop__Documentation(struct soap*, const char *, int, struct wstop__Documentation *const*, const char *);
SOAP_FMAC3 struct wstop__Documentation ** SOAP_FMAC4 soap_in_PointerTowstop__Documentation(struct soap*, const char*, struct wstop__Documentation **, const char*);

#ifndef soap_write_PointerTowstop__Documentation
#define soap_write_PointerTowstop__Documentation(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTowstop__Documentation(soap, data), 0) || soap_put_PointerTowstop__Documentation(soap, data, "wstop:Documentation", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTowstop__Documentation(struct soap*, struct wstop__Documentation *const*, const char*, const char*);

#ifndef soap_read_PointerTowstop__Documentation
#define soap_read_PointerTowstop__Documentation(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTowstop__Documentation(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct wstop__Documentation ** SOAP_FMAC4 soap_get_PointerTowstop__Documentation(struct soap*, struct wstop__Documentation **, const char*, const char*);

#ifndef SOAP_TYPE_wstop__Documentation
#define SOAP_TYPE_wstop__Documentation (1117)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_wstop__Documentation(struct soap*, struct wstop__Documentation *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_wstop__Documentation(struct soap*, const struct wstop__Documentation *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_wstop__Documentation(struct soap*, const char*, int, const struct wstop__Documentation *, const char*);
SOAP_FMAC3 struct wstop__Documentation * SOAP_FMAC4 soap_in_wstop__Documentation(struct soap*, const char*, struct wstop__Documentation *, const char*);

#ifndef soap_write_wstop__Documentation
#define soap_write_wstop__Documentation(soap, data) ( soap_begin_send(soap) || (soap_serialize_wstop__Documentation(soap, data), 0) || soap_put_wstop__Documentation(soap, data, "wstop:Documentation", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put_wstop__Documentation(struct soap*, const struct wstop__Documentation *, const char*, const char*);

#ifndef soap_read_wstop__Documentation
#define soap_read_wstop__Documentation(soap, data) ( soap_begin_recv(soap) || !soap_get_wstop__Documentation(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct wstop__Documentation * SOAP_FMAC4 soap_get_wstop__Documentation(struct soap*, struct wstop__Documentation *, const char*, const char*);

#ifndef SOAP_TYPE_PointerTowstop__TopicSetType
#define SOAP_TYPE_PointerTowstop__TopicSetType (1084)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTowstop__TopicSetType(struct soap*, struct wstop__TopicSetType *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTowstop__TopicSetType(struct soap*, const char *, int, struct wstop__TopicSetType *const*, const char *);
SOAP_FMAC3 struct wstop__TopicSetType ** SOAP_FMAC4 soap_in_PointerTowstop__TopicSetType(struct soap*, const char*, struct wstop__TopicSetType **, const char*);

#ifndef soap_write_PointerTowstop__TopicSetType
#define soap_write_PointerTowstop__TopicSetType(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTowstop__TopicSetType(soap, data), 0) || soap_put_PointerTowstop__TopicSetType(soap, data, "wstop:TopicSetType", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTowstop__TopicSetType(struct soap*, struct wstop__TopicSetType *const*, const char*, const char*);

#ifndef soap_read_PointerTowstop__TopicSetType
#define soap_read_PointerTowstop__TopicSetType(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTowstop__TopicSetType(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct wstop__TopicSetType ** SOAP_FMAC4 soap_get_PointerTowstop__TopicSetType(struct soap*, struct wstop__TopicSetType **, const char*, const char*);

#ifndef SOAP_TYPE___dee__CreatePullPointSubscription
#define SOAP_TYPE___dee__CreatePullPointSubscription (2128)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default___dee__CreatePullPointSubscription(struct soap*, struct __dee__CreatePullPointSubscription *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___dee__CreatePullPointSubscription(struct soap*, const struct __dee__CreatePullPointSubscription *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out___dee__CreatePullPointSubscription(struct soap*, const char*, int, const struct __dee__CreatePullPointSubscription *, const char*);
SOAP_FMAC3 struct __dee__CreatePullPointSubscription * SOAP_FMAC4 soap_in___dee__CreatePullPointSubscription(struct soap*, const char*, struct __dee__CreatePullPointSubscription *, const char*);

#ifndef soap_write___dee__CreatePullPointSubscription
#define soap_write___dee__CreatePullPointSubscription(soap, data) ( soap_begin_send(soap) || (soap_serialize___dee__CreatePullPointSubscription(soap, data), 0) || soap_put___dee__CreatePullPointSubscription(soap, data, "-dee:CreatePullPointSubscription", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put___dee__CreatePullPointSubscription(struct soap*, const struct __dee__CreatePullPointSubscription *, const char*, const char*);

#ifndef soap_read___dee__CreatePullPointSubscription
#define soap_read___dee__CreatePullPointSubscription(soap, data) ( soap_begin_recv(soap) || !soap_get___dee__CreatePullPointSubscription(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct __dee__CreatePullPointSubscription * SOAP_FMAC4 soap_get___dee__CreatePullPointSubscription(struct soap*, struct __dee__CreatePullPointSubscription *, const char*, const char*);

#ifndef SOAP_TYPE_PointerTo_tev__CreatePullPointSubscriptionResponse
#define SOAP_TYPE_PointerTo_tev__CreatePullPointSubscriptionResponse (2126)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tev__CreatePullPointSubscriptionResponse(struct soap*, struct _tev__CreatePullPointSubscriptionResponse *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tev__CreatePullPointSubscriptionResponse(struct soap*, const char *, int, struct _tev__CreatePullPointSubscriptionResponse *const*, const char *);
SOAP_FMAC3 struct _tev__CreatePullPointSubscriptionResponse ** SOAP_FMAC4 soap_in_PointerTo_tev__CreatePullPointSubscriptionResponse(struct soap*, const char*, struct _tev__CreatePullPointSubscriptionResponse **, const char*);

#ifndef soap_write_PointerTo_tev__CreatePullPointSubscriptionResponse
#define soap_write_PointerTo_tev__CreatePullPointSubscriptionResponse(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_tev__CreatePullPointSubscriptionResponse(soap, data), 0) || soap_put_PointerTo_tev__CreatePullPointSubscriptionResponse(soap, data, "tev:CreatePullPointSubscriptionResponse", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tev__CreatePullPointSubscriptionResponse(struct soap*, struct _tev__CreatePullPointSubscriptionResponse *const*, const char*, const char*);

#ifndef soap_read_PointerTo_tev__CreatePullPointSubscriptionResponse
#define soap_read_PointerTo_tev__CreatePullPointSubscriptionResponse(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_tev__CreatePullPointSubscriptionResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tev__CreatePullPointSubscriptionResponse ** SOAP_FMAC4 soap_get_PointerTo_tev__CreatePullPointSubscriptionResponse(struct soap*, struct _tev__CreatePullPointSubscriptionResponse **, const char*, const char*);

#ifndef SOAP_TYPE_PointerTo_tev__CreatePullPointSubscription
#define SOAP_TYPE_PointerTo_tev__CreatePullPointSubscription (2125)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tev__CreatePullPointSubscription(struct soap*, struct _tev__CreatePullPointSubscription *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tev__CreatePullPointSubscription(struct soap*, const char *, int, struct _tev__CreatePullPointSubscription *const*, const char *);
SOAP_FMAC3 struct _tev__CreatePullPointSubscription ** SOAP_FMAC4 soap_in_PointerTo_tev__CreatePullPointSubscription(struct soap*, const char*, struct _tev__CreatePullPointSubscription **, const char*);

#ifndef soap_write_PointerTo_tev__CreatePullPointSubscription
#define soap_write_PointerTo_tev__CreatePullPointSubscription(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_tev__CreatePullPointSubscription(soap, data), 0) || soap_put_PointerTo_tev__CreatePullPointSubscription(soap, data, "tev:CreatePullPointSubscription", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tev__CreatePullPointSubscription(struct soap*, struct _tev__CreatePullPointSubscription *const*, const char*, const char*);

#ifndef soap_read_PointerTo_tev__CreatePullPointSubscription
#define soap_read_PointerTo_tev__CreatePullPointSubscription(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_tev__CreatePullPointSubscription(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tev__CreatePullPointSubscription ** SOAP_FMAC4 soap_get_PointerTo_tev__CreatePullPointSubscription(struct soap*, struct _tev__CreatePullPointSubscription **, const char*, const char*);

#ifndef SOAP_TYPE__tev__CreatePullPointSubscriptionResponse
#define SOAP_TYPE__tev__CreatePullPointSubscriptionResponse (1569)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__tev__CreatePullPointSubscriptionResponse(struct soap*, struct _tev__CreatePullPointSubscriptionResponse *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tev__CreatePullPointSubscriptionResponse(struct soap*, const struct _tev__CreatePullPointSubscriptionResponse *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__tev__CreatePullPointSubscriptionResponse(struct soap*, const char*, int, const struct _tev__CreatePullPointSubscriptionResponse *, const char*);
SOAP_FMAC3 struct _tev__CreatePullPointSubscriptionResponse * SOAP_FMAC4 soap_in__tev__CreatePullPointSubscriptionResponse(struct soap*, const char*, struct _tev__CreatePullPointSubscriptionResponse *, const char*);

#ifndef soap_write__tev__CreatePullPointSubscriptionResponse
#define soap_write__tev__CreatePullPointSubscriptionResponse(soap, data) ( soap_begin_send(soap) || (soap_serialize__tev__CreatePullPointSubscriptionResponse(soap, data), 0) || soap_put__tev__CreatePullPointSubscriptionResponse(soap, data, "tev:CreatePullPointSubscriptionResponse", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__tev__CreatePullPointSubscriptionResponse(struct soap*, const struct _tev__CreatePullPointSubscriptionResponse *, const char*, const char*);

#ifndef soap_read__tev__CreatePullPointSubscriptionResponse
#define soap_read__tev__CreatePullPointSubscriptionResponse(soap, data) ( soap_begin_recv(soap) || !soap_get__tev__CreatePullPointSubscriptionResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tev__CreatePullPointSubscriptionResponse * SOAP_FMAC4 soap_get__tev__CreatePullPointSubscriptionResponse(struct soap*, struct _tev__CreatePullPointSubscriptionResponse *, const char*, const char*);

#ifndef SOAP_TYPE__tev__CreatePullPointSubscription
#define SOAP_TYPE__tev__CreatePullPointSubscription (1566)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__tev__CreatePullPointSubscription(struct soap*, struct _tev__CreatePullPointSubscription *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tev__CreatePullPointSubscription(struct soap*, const struct _tev__CreatePullPointSubscription *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__tev__CreatePullPointSubscription(struct soap*, const char*, int, const struct _tev__CreatePullPointSubscription *, const char*);
SOAP_FMAC3 struct _tev__CreatePullPointSubscription * SOAP_FMAC4 soap_in__tev__CreatePullPointSubscription(struct soap*, const char*, struct _tev__CreatePullPointSubscription *, const char*);

#ifndef soap_write__tev__CreatePullPointSubscription
#define soap_write__tev__CreatePullPointSubscription(soap, data) ( soap_begin_send(soap) || (soap_serialize__tev__CreatePullPointSubscription(soap, data), 0) || soap_put__tev__CreatePullPointSubscription(soap, data, "tev:CreatePullPointSubscription", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__tev__CreatePullPointSubscription(struct soap*, const struct _tev__CreatePullPointSubscription *, const char*, const char*);

#ifndef soap_read__tev__CreatePullPointSubscription
#define soap_read__tev__CreatePullPointSubscription(soap, data) ( soap_begin_recv(soap) || !soap_get__tev__CreatePullPointSubscription(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tev__CreatePullPointSubscription * SOAP_FMAC4 soap_get__tev__CreatePullPointSubscription(struct soap*, struct _tev__CreatePullPointSubscription *, const char*, const char*);

#ifndef SOAP_TYPE_PointerTo_tev__CreatePullPointSubscription_SubscriptionPolicy
#define SOAP_TYPE_PointerTo_tev__CreatePullPointSubscription_SubscriptionPolicy (1568)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tev__CreatePullPointSubscription_SubscriptionPolicy(struct soap*, struct _tev__CreatePullPointSubscription_SubscriptionPolicy *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tev__CreatePullPointSubscription_SubscriptionPolicy(struct soap*, const char *, int, struct _tev__CreatePullPointSubscription_SubscriptionPolicy *const*, const char *);
SOAP_FMAC3 struct _tev__CreatePullPointSubscription_SubscriptionPolicy ** SOAP_FMAC4 soap_in_PointerTo_tev__CreatePullPointSubscription_SubscriptionPolicy(struct soap*, const char*, struct _tev__CreatePullPointSubscription_SubscriptionPolicy **, const char*);

#ifndef soap_write_PointerTo_tev__CreatePullPointSubscription_SubscriptionPolicy
#define soap_write_PointerTo_tev__CreatePullPointSubscription_SubscriptionPolicy(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_tev__CreatePullPointSubscription_SubscriptionPolicy(soap, data), 0) || soap_put_PointerTo_tev__CreatePullPointSubscription_SubscriptionPolicy(soap, data, "tev:CreatePullPointSubscription-SubscriptionPolicy", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tev__CreatePullPointSubscription_SubscriptionPolicy(struct soap*, struct _tev__CreatePullPointSubscription_SubscriptionPolicy *const*, const char*, const char*);

#ifndef soap_read_PointerTo_tev__CreatePullPointSubscription_SubscriptionPolicy
#define soap_read_PointerTo_tev__CreatePullPointSubscription_SubscriptionPolicy(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_tev__CreatePullPointSubscription_SubscriptionPolicy(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tev__CreatePullPointSubscription_SubscriptionPolicy ** SOAP_FMAC4 soap_get_PointerTo_tev__CreatePullPointSubscription_SubscriptionPolicy(struct soap*, struct _tev__CreatePullPointSubscription_SubscriptionPolicy **, const char*, const char*);

#ifndef SOAP_TYPE__tev__CreatePullPointSubscription_SubscriptionPolicy
#define SOAP_TYPE__tev__CreatePullPointSubscription_SubscriptionPolicy (1567)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__tev__CreatePullPointSubscription_SubscriptionPolicy(struct soap*, struct _tev__CreatePullPointSubscription_SubscriptionPolicy *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tev__CreatePullPointSubscription_SubscriptionPolicy(struct soap*, const struct _tev__CreatePullPointSubscription_SubscriptionPolicy *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__tev__CreatePullPointSubscription_SubscriptionPolicy(struct soap*, const char*, int, const struct _tev__CreatePullPointSubscription_SubscriptionPolicy *, const char*);
SOAP_FMAC3 struct _tev__CreatePullPointSubscription_SubscriptionPolicy * SOAP_FMAC4 soap_in__tev__CreatePullPointSubscription_SubscriptionPolicy(struct soap*, const char*, struct _tev__CreatePullPointSubscription_SubscriptionPolicy *, const char*);

#ifndef soap_write__tev__CreatePullPointSubscription_SubscriptionPolicy
#define soap_write__tev__CreatePullPointSubscription_SubscriptionPolicy(soap, data) ( soap_begin_send(soap) || (soap_serialize__tev__CreatePullPointSubscription_SubscriptionPolicy(soap, data), 0) || soap_put__tev__CreatePullPointSubscription_SubscriptionPolicy(soap, data, "tev:CreatePullPointSubscription-SubscriptionPolicy", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__tev__CreatePullPointSubscription_SubscriptionPolicy(struct soap*, const struct _tev__CreatePullPointSubscription_SubscriptionPolicy *, const char*, const char*);

#ifndef soap_read__tev__CreatePullPointSubscription_SubscriptionPolicy
#define soap_read__tev__CreatePullPointSubscription_SubscriptionPolicy(soap, data) ( soap_begin_recv(soap) || !soap_get__tev__CreatePullPointSubscription_SubscriptionPolicy(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tev__CreatePullPointSubscription_SubscriptionPolicy * SOAP_FMAC4 soap_get__tev__CreatePullPointSubscription_SubscriptionPolicy(struct soap*, struct _tev__CreatePullPointSubscription_SubscriptionPolicy *, const char*, const char*);

#ifndef SOAP_TYPE___depps__PullMessages
#define SOAP_TYPE___depps__PullMessages (2157)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default___depps__PullMessages(struct soap*, struct __depps__PullMessages *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___depps__PullMessages(struct soap*, const struct __depps__PullMessages *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out___depps__PullMessages(struct soap*, const char*, int, const struct __depps__PullMessages *, const char*);
SOAP_FMAC3 struct __depps__PullMessages * SOAP_FMAC4 soap_in___depps__PullMessages(struct soap*, const char*, struct __depps__PullMessages *, const char*);

#ifndef soap_write___depps__PullMessages
#define soap_write___depps__PullMessages(soap, data) ( soap_begin_send(soap) || (soap_serialize___depps__PullMessages(soap, data), 0) || soap_put___depps__PullMessages(soap, data, "-depps:PullMessages", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put___depps__PullMessages(struct soap*, const struct __depps__PullMessages *, const char*, const char*);

#ifndef soap_read___depps__PullMessages
#define soap_read___depps__PullMessages(soap, data) ( soap_begin_recv(soap) || !soap_get___depps__PullMessages(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct __depps__PullMessages * SOAP_FMAC4 soap_get___depps__PullMessages(struct soap*, struct __depps__PullMessages *, const char*, const char*);

#ifndef SOAP_TYPE_PointerTo_tev__PullMessages
#define SOAP_TYPE_PointerTo_tev__PullMessages (2154)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tev__PullMessages(struct soap*, struct _tev__PullMessages *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tev__PullMessages(struct soap*, const char *, int, struct _tev__PullMessages *const*, const char *);
SOAP_FMAC3 struct _tev__PullMessages ** SOAP_FMAC4 soap_in_PointerTo_tev__PullMessages(struct soap*, const char*, struct _tev__PullMessages **, const char*);

#ifndef soap_write_PointerTo_tev__PullMessages
#define soap_write_PointerTo_tev__PullMessages(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_tev__PullMessages(soap, data), 0) || soap_put_PointerTo_tev__PullMessages(soap, data, "tev:PullMessages", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tev__PullMessages(struct soap*, struct _tev__PullMessages *const*, const char*, const char*);

#ifndef soap_read_PointerTo_tev__PullMessages
#define soap_read_PointerTo_tev__PullMessages(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_tev__PullMessages(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tev__PullMessages ** SOAP_FMAC4 soap_get_PointerTo_tev__PullMessages(struct soap*, struct _tev__PullMessages **, const char*, const char*);

#ifndef SOAP_TYPE__tev__PullMessages
#define SOAP_TYPE__tev__PullMessages (1570)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__tev__PullMessages(struct soap*, struct _tev__PullMessages *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tev__PullMessages(struct soap*, const struct _tev__PullMessages *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__tev__PullMessages(struct soap*, const char*, int, const struct _tev__PullMessages *, const char*);
SOAP_FMAC3 struct _tev__PullMessages * SOAP_FMAC4 soap_in__tev__PullMessages(struct soap*, const char*, struct _tev__PullMessages *, const char*);

#ifndef soap_write__tev__PullMessages
#define soap_write__tev__PullMessages(soap, data) ( soap_begin_send(soap) || (soap_serialize__tev__PullMessages(soap, data), 0) || soap_put__tev__PullMessages(soap, data, "tev:PullMessages", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__tev__PullMessages(struct soap*, const struct _tev__PullMessages *, const char*, const char*);

#ifndef soap_read__tev__PullMessages
#define soap_read__tev__PullMessages(soap, data) ( soap_begin_recv(soap) || !soap_get__tev__PullMessages(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tev__PullMessages * SOAP_FMAC4 soap_get__tev__PullMessages(struct soap*, struct _tev__PullMessages *, const char*, const char*);

#ifndef SOAP_TYPE__tev__PullMessagesResponse
#define SOAP_TYPE__tev__PullMessagesResponse (1571)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__tev__PullMessagesResponse(struct soap*, struct _tev__PullMessagesResponse *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tev__PullMessagesResponse(struct soap*, const struct _tev__PullMessagesResponse *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__tev__PullMessagesResponse(struct soap*, const char*, int, const struct _tev__PullMessagesResponse *, const char*);
SOAP_FMAC3 struct _tev__PullMessagesResponse * SOAP_FMAC4 soap_in__tev__PullMessagesResponse(struct soap*, const char*, struct _tev__PullMessagesResponse *, const char*);

#ifndef soap_write__tev__PullMessagesResponse
#define soap_write__tev__PullMessagesResponse(soap, data) ( soap_begin_send(soap) || (soap_serialize__tev__PullMessagesResponse(soap, data), 0) || soap_put__tev__PullMessagesResponse(soap, data, "tev:PullMessagesResponse", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__tev__PullMessagesResponse(struct soap*, const struct _tev__PullMessagesResponse *, const char*, const char*);

#ifndef soap_read__tev__PullMessagesResponse
#define soap_read__tev__PullMessagesResponse(soap, data) ( soap_begin_recv(soap) || !soap_get__tev__PullMessagesResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tev__PullMessagesResponse * SOAP_FMAC4 soap_get__tev__PullMessagesResponse(struct soap*, struct _tev__PullMessagesResponse *, const char*, const char*);

#ifndef SOAP_TYPE___depps__SetSynchronizationPoint
#define SOAP_TYPE___depps__SetSynchronizationPoint (2161)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default___depps__SetSynchronizationPoint(struct soap*, struct __depps__SetSynchronizationPoint *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___depps__SetSynchronizationPoint(struct soap*, const struct __depps__SetSynchronizationPoint *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out___depps__SetSynchronizationPoint(struct soap*, const char*, int, const struct __depps__SetSynchronizationPoint *, const char*);
SOAP_FMAC3 struct __depps__SetSynchronizationPoint * SOAP_FMAC4 soap_in___depps__SetSynchronizationPoint(struct soap*, const char*, struct __depps__SetSynchronizationPoint *, const char*);

#ifndef soap_write___depps__SetSynchronizationPoint
#define soap_write___depps__SetSynchronizationPoint(soap, data) ( soap_begin_send(soap) || (soap_serialize___depps__SetSynchronizationPoint(soap, data), 0) || soap_put___depps__SetSynchronizationPoint(soap, data, "-depps:SetSynchronizationPoint", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put___depps__SetSynchronizationPoint(struct soap*, const struct __depps__SetSynchronizationPoint *, const char*, const char*);

#ifndef soap_read___depps__SetSynchronizationPoint
#define soap_read___depps__SetSynchronizationPoint(soap, data) ( soap_begin_recv(soap) || !soap_get___depps__SetSynchronizationPoint(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct __depps__SetSynchronizationPoint * SOAP_FMAC4 soap_get___depps__SetSynchronizationPoint(struct soap*, struct __depps__SetSynchronizationPoint *, const char*, const char*);

#ifndef SOAP_TYPE_PointerTo_tev__SetSynchronizationPoint
#define SOAP_TYPE_PointerTo_tev__SetSynchronizationPoint (2158)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tev__SetSynchronizationPoint(struct soap*, struct _tev__SetSynchronizationPoint *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tev__SetSynchronizationPoint(struct soap*, const char *, int, struct _tev__SetSynchronizationPoint *const*, const char *);
SOAP_FMAC3 struct _tev__SetSynchronizationPoint ** SOAP_FMAC4 soap_in_PointerTo_tev__SetSynchronizationPoint(struct soap*, const char*, struct _tev__SetSynchronizationPoint **, const char*);

#ifndef soap_write_PointerTo_tev__SetSynchronizationPoint
#define soap_write_PointerTo_tev__SetSynchronizationPoint(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_tev__SetSynchronizationPoint(soap, data), 0) || soap_put_PointerTo_tev__SetSynchronizationPoint(soap, data, "tev:SetSynchronizationPoint", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tev__SetSynchronizationPoint(struct soap*, struct _tev__SetSynchronizationPoint *const*, const char*, const char*);

#ifndef soap_read_PointerTo_tev__SetSynchronizationPoint
#define soap_read_PointerTo_tev__SetSynchronizationPoint(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_tev__SetSynchronizationPoint(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tev__SetSynchronizationPoint ** SOAP_FMAC4 soap_get_PointerTo_tev__SetSynchronizationPoint(struct soap*, struct _tev__SetSynchronizationPoint **, const char*, const char*);

#ifndef SOAP_TYPE__tev__SetSynchronizationPoint
#define SOAP_TYPE__tev__SetSynchronizationPoint (1573)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__tev__SetSynchronizationPoint(struct soap*, struct _tev__SetSynchronizationPoint *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tev__SetSynchronizationPoint(struct soap*, const struct _tev__SetSynchronizationPoint *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__tev__SetSynchronizationPoint(struct soap*, const char*, int, const struct _tev__SetSynchronizationPoint *, const char*);
SOAP_FMAC3 struct _tev__SetSynchronizationPoint * SOAP_FMAC4 soap_in__tev__SetSynchronizationPoint(struct soap*, const char*, struct _tev__SetSynchronizationPoint *, const char*);

#ifndef soap_write__tev__SetSynchronizationPoint
#define soap_write__tev__SetSynchronizationPoint(soap, data) ( soap_begin_send(soap) || (soap_serialize__tev__SetSynchronizationPoint(soap, data), 0) || soap_put__tev__SetSynchronizationPoint(soap, data, "tev:SetSynchronizationPoint", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__tev__SetSynchronizationPoint(struct soap*, const struct _tev__SetSynchronizationPoint *, const char*, const char*);

#ifndef soap_read__tev__SetSynchronizationPoint
#define soap_read__tev__SetSynchronizationPoint(soap, data) ( soap_begin_recv(soap) || !soap_get__tev__SetSynchronizationPoint(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tev__SetSynchronizationPoint * SOAP_FMAC4 soap_get__tev__SetSynchronizationPoint(struct soap*, struct _tev__SetSynchronizationPoint *, const char*, const char*);



#ifndef SOAP_TYPE__tev__SetSynchronizationPointResponse
#define SOAP_TYPE__tev__SetSynchronizationPointResponse (1574)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__tev__SetSynchronizationPointResponse(struct soap*, struct _tev__SetSynchronizationPointResponse *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tev__SetSynchronizationPointResponse(struct soap*, const struct _tev__SetSynchronizationPointResponse *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__tev__SetSynchronizationPointResponse(struct soap*, const char*, int, const struct _tev__SetSynchronizationPointResponse *, const char*);
SOAP_FMAC3 struct _tev__SetSynchronizationPointResponse * SOAP_FMAC4 soap_in__tev__SetSynchronizationPointResponse(struct soap*, const char*, struct _tev__SetSynchronizationPointResponse *, const char*);

#ifndef soap_write__tev__SetSynchronizationPointResponse
#define soap_write__tev__SetSynchronizationPointResponse(soap, data) ( soap_begin_send(soap) || (soap_serialize__tev__SetSynchronizationPointResponse(soap, data), 0) || soap_put__tev__SetSynchronizationPointResponse(soap, data, "tev:SetSynchronizationPointResponse", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__tev__SetSynchronizationPointResponse(struct soap*, const struct _tev__SetSynchronizationPointResponse *, const char*, const char*);

#ifndef soap_read__tev__SetSynchronizationPointResponse
#define soap_read__tev__SetSynchronizationPointResponse(soap, data) ( soap_begin_recv(soap) || !soap_get__tev__SetSynchronizationPointResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tev__SetSynchronizationPointResponse * SOAP_FMAC4 soap_get__tev__SetSynchronizationPointResponse(struct soap*, struct _tev__SetSynchronizationPointResponse *, const char*, const char*);


///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef SOAP_TYPE__tan__GetAnalyticsModulesResponse
#define SOAP_TYPE__tan__GetAnalyticsModulesResponse (257)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__GetAnalyticsModulesResponse(struct soap*, struct _tan__GetAnalyticsModulesResponse *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__GetAnalyticsModulesResponse(struct soap*, const struct _tan__GetAnalyticsModulesResponse *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__GetAnalyticsModulesResponse(struct soap*, const char*, int, const struct _tan__GetAnalyticsModulesResponse *, const char*);
SOAP_FMAC3 struct _tan__GetAnalyticsModulesResponse * SOAP_FMAC4 soap_in__tan__GetAnalyticsModulesResponse(struct soap*, const char*, struct _tan__GetAnalyticsModulesResponse *, const char*);

#ifndef soap_write__tan__GetAnalyticsModulesResponse
#define soap_write__tan__GetAnalyticsModulesResponse(soap, data) ( soap_begin_send(soap) || (soap_serialize__tan__GetAnalyticsModulesResponse(soap, data), 0) || soap_put__tan__GetAnalyticsModulesResponse(soap, data, "tan:GetAnalyticsModulesResponse", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__GetAnalyticsModulesResponse(struct soap*, const struct _tan__GetAnalyticsModulesResponse *, const char*, const char*);

#ifndef soap_read__tan__GetAnalyticsModulesResponse
#define soap_read__tan__GetAnalyticsModulesResponse(soap, data) ( soap_begin_recv(soap) || !soap_get__tan__GetAnalyticsModulesResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__GetAnalyticsModulesResponse * SOAP_FMAC4 soap_get__tan__GetAnalyticsModulesResponse(struct soap*, struct _tan__GetAnalyticsModulesResponse *, const char*, const char*);

#ifndef SOAP_TYPE__tan__GetAnalyticsModules
#define SOAP_TYPE__tan__GetAnalyticsModules (256)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__GetAnalyticsModules(struct soap*, struct _tan__GetAnalyticsModules *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__GetAnalyticsModules(struct soap*, const struct _tan__GetAnalyticsModules *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__GetAnalyticsModules(struct soap*, const char*, int, const struct _tan__GetAnalyticsModules *, const char*);
SOAP_FMAC3 struct _tan__GetAnalyticsModules * SOAP_FMAC4 soap_in__tan__GetAnalyticsModules(struct soap*, const char*, struct _tan__GetAnalyticsModules *, const char*);

#ifndef soap_write__tan__GetAnalyticsModules
#define soap_write__tan__GetAnalyticsModules(soap, data) ( soap_begin_send(soap) || (soap_serialize__tan__GetAnalyticsModules(soap, data), 0) || soap_put__tan__GetAnalyticsModules(soap, data, "tan:GetAnalyticsModules", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__GetAnalyticsModules(struct soap*, const struct _tan__GetAnalyticsModules *, const char*, const char*);

#ifndef soap_read__tan__GetAnalyticsModules
#define soap_read__tan__GetAnalyticsModules(soap, data) ( soap_begin_recv(soap) || !soap_get__tan__GetAnalyticsModules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__GetAnalyticsModules * SOAP_FMAC4 soap_get__tan__GetAnalyticsModules(struct soap*, struct _tan__GetAnalyticsModules *, const char*, const char*);

#ifndef SOAP_TYPE__tan__ModifyAnalyticsModulesResponse
#define SOAP_TYPE__tan__ModifyAnalyticsModulesResponse (255)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__ModifyAnalyticsModulesResponse(struct soap*, struct _tan__ModifyAnalyticsModulesResponse *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__ModifyAnalyticsModulesResponse(struct soap*, const struct _tan__ModifyAnalyticsModulesResponse *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__ModifyAnalyticsModulesResponse(struct soap*, const char*, int, const struct _tan__ModifyAnalyticsModulesResponse *, const char*);
SOAP_FMAC3 struct _tan__ModifyAnalyticsModulesResponse * SOAP_FMAC4 soap_in__tan__ModifyAnalyticsModulesResponse(struct soap*, const char*, struct _tan__ModifyAnalyticsModulesResponse *, const char*);

#ifndef soap_write__tan__ModifyAnalyticsModulesResponse
#define soap_write__tan__ModifyAnalyticsModulesResponse(soap, data) ( soap_begin_send(soap) || (soap_serialize__tan__ModifyAnalyticsModulesResponse(soap, data), 0) || soap_put__tan__ModifyAnalyticsModulesResponse(soap, data, "tan:ModifyAnalyticsModulesResponse", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__ModifyAnalyticsModulesResponse(struct soap*, const struct _tan__ModifyAnalyticsModulesResponse *, const char*, const char*);

#ifndef soap_read__tan__ModifyAnalyticsModulesResponse
#define soap_read__tan__ModifyAnalyticsModulesResponse(soap, data) ( soap_begin_recv(soap) || !soap_get__tan__ModifyAnalyticsModulesResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__ModifyAnalyticsModulesResponse * SOAP_FMAC4 soap_get__tan__ModifyAnalyticsModulesResponse(struct soap*, struct _tan__ModifyAnalyticsModulesResponse *, const char*, const char*);

#ifndef SOAP_TYPE__tan__ModifyAnalyticsModules
#define SOAP_TYPE__tan__ModifyAnalyticsModules (254)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__ModifyAnalyticsModules(struct soap*, struct _tan__ModifyAnalyticsModules *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__ModifyAnalyticsModules(struct soap*, const struct _tan__ModifyAnalyticsModules *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__ModifyAnalyticsModules(struct soap*, const char*, int, const struct _tan__ModifyAnalyticsModules *, const char*);
SOAP_FMAC3 struct _tan__ModifyAnalyticsModules * SOAP_FMAC4 soap_in__tan__ModifyAnalyticsModules(struct soap*, const char*, struct _tan__ModifyAnalyticsModules *, const char*);

#ifndef soap_write__tan__ModifyAnalyticsModules
#define soap_write__tan__ModifyAnalyticsModules(soap, data) ( soap_begin_send(soap) || (soap_serialize__tan__ModifyAnalyticsModules(soap, data), 0) || soap_put__tan__ModifyAnalyticsModules(soap, data, "tan:ModifyAnalyticsModules", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__ModifyAnalyticsModules(struct soap*, const struct _tan__ModifyAnalyticsModules *, const char*, const char*);

#ifndef soap_read__tan__ModifyAnalyticsModules
#define soap_read__tan__ModifyAnalyticsModules(soap, data) ( soap_begin_recv(soap) || !soap_get__tan__ModifyAnalyticsModules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__ModifyAnalyticsModules * SOAP_FMAC4 soap_get__tan__ModifyAnalyticsModules(struct soap*, struct _tan__ModifyAnalyticsModules *, const char*, const char*);

#ifndef SOAP_TYPE__tan__DeleteAnalyticsModulesResponse
#define SOAP_TYPE__tan__DeleteAnalyticsModulesResponse (253)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__DeleteAnalyticsModulesResponse(struct soap*, struct _tan__DeleteAnalyticsModulesResponse *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__DeleteAnalyticsModulesResponse(struct soap*, const struct _tan__DeleteAnalyticsModulesResponse *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__DeleteAnalyticsModulesResponse(struct soap*, const char*, int, const struct _tan__DeleteAnalyticsModulesResponse *, const char*);
SOAP_FMAC3 struct _tan__DeleteAnalyticsModulesResponse * SOAP_FMAC4 soap_in__tan__DeleteAnalyticsModulesResponse(struct soap*, const char*, struct _tan__DeleteAnalyticsModulesResponse *, const char*);

#ifndef soap_write__tan__DeleteAnalyticsModulesResponse
#define soap_write__tan__DeleteAnalyticsModulesResponse(soap, data) ( soap_begin_send(soap) || (soap_serialize__tan__DeleteAnalyticsModulesResponse(soap, data), 0) || soap_put__tan__DeleteAnalyticsModulesResponse(soap, data, "tan:DeleteAnalyticsModulesResponse", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__DeleteAnalyticsModulesResponse(struct soap*, const struct _tan__DeleteAnalyticsModulesResponse *, const char*, const char*);

#ifndef soap_read__tan__DeleteAnalyticsModulesResponse
#define soap_read__tan__DeleteAnalyticsModulesResponse(soap, data) ( soap_begin_recv(soap) || !soap_get__tan__DeleteAnalyticsModulesResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__DeleteAnalyticsModulesResponse * SOAP_FMAC4 soap_get__tan__DeleteAnalyticsModulesResponse(struct soap*, struct _tan__DeleteAnalyticsModulesResponse *, const char*, const char*);

#ifndef SOAP_TYPE__tan__DeleteAnalyticsModules
#define SOAP_TYPE__tan__DeleteAnalyticsModules (252)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__DeleteAnalyticsModules(struct soap*, struct _tan__DeleteAnalyticsModules *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__DeleteAnalyticsModules(struct soap*, const struct _tan__DeleteAnalyticsModules *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__DeleteAnalyticsModules(struct soap*, const char*, int, const struct _tan__DeleteAnalyticsModules *, const char*);
SOAP_FMAC3 struct _tan__DeleteAnalyticsModules * SOAP_FMAC4 soap_in__tan__DeleteAnalyticsModules(struct soap*, const char*, struct _tan__DeleteAnalyticsModules *, const char*);

#ifndef soap_write__tan__DeleteAnalyticsModules
#define soap_write__tan__DeleteAnalyticsModules(soap, data) ( soap_begin_send(soap) || (soap_serialize__tan__DeleteAnalyticsModules(soap, data), 0) || soap_put__tan__DeleteAnalyticsModules(soap, data, "tan:DeleteAnalyticsModules", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__DeleteAnalyticsModules(struct soap*, const struct _tan__DeleteAnalyticsModules *, const char*, const char*);

#ifndef soap_read__tan__DeleteAnalyticsModules
#define soap_read__tan__DeleteAnalyticsModules(soap, data) ( soap_begin_recv(soap) || !soap_get__tan__DeleteAnalyticsModules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__DeleteAnalyticsModules * SOAP_FMAC4 soap_get__tan__DeleteAnalyticsModules(struct soap*, struct _tan__DeleteAnalyticsModules *, const char*, const char*);

#ifndef SOAP_TYPE__tan__CreateAnalyticsModulesResponse
#define SOAP_TYPE__tan__CreateAnalyticsModulesResponse (251)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__CreateAnalyticsModulesResponse(struct soap*, struct _tan__CreateAnalyticsModulesResponse *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__CreateAnalyticsModulesResponse(struct soap*, const struct _tan__CreateAnalyticsModulesResponse *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__CreateAnalyticsModulesResponse(struct soap*, const char*, int, const struct _tan__CreateAnalyticsModulesResponse *, const char*);
SOAP_FMAC3 struct _tan__CreateAnalyticsModulesResponse * SOAP_FMAC4 soap_in__tan__CreateAnalyticsModulesResponse(struct soap*, const char*, struct _tan__CreateAnalyticsModulesResponse *, const char*);

#ifndef soap_write__tan__CreateAnalyticsModulesResponse
#define soap_write__tan__CreateAnalyticsModulesResponse(soap, data) ( soap_begin_send(soap) || (soap_serialize__tan__CreateAnalyticsModulesResponse(soap, data), 0) || soap_put__tan__CreateAnalyticsModulesResponse(soap, data, "tan:CreateAnalyticsModulesResponse", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__CreateAnalyticsModulesResponse(struct soap*, const struct _tan__CreateAnalyticsModulesResponse *, const char*, const char*);

#ifndef soap_read__tan__CreateAnalyticsModulesResponse
#define soap_read__tan__CreateAnalyticsModulesResponse(soap, data) ( soap_begin_recv(soap) || !soap_get__tan__CreateAnalyticsModulesResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__CreateAnalyticsModulesResponse * SOAP_FMAC4 soap_get__tan__CreateAnalyticsModulesResponse(struct soap*, struct _tan__CreateAnalyticsModulesResponse *, const char*, const char*);

#ifndef SOAP_TYPE__tan__CreateAnalyticsModules
#define SOAP_TYPE__tan__CreateAnalyticsModules (250)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__CreateAnalyticsModules(struct soap*, struct _tan__CreateAnalyticsModules *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__CreateAnalyticsModules(struct soap*, const struct _tan__CreateAnalyticsModules *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__CreateAnalyticsModules(struct soap*, const char*, int, const struct _tan__CreateAnalyticsModules *, const char*);
SOAP_FMAC3 struct _tan__CreateAnalyticsModules * SOAP_FMAC4 soap_in__tan__CreateAnalyticsModules(struct soap*, const char*, struct _tan__CreateAnalyticsModules *, const char*);

#ifndef soap_write__tan__CreateAnalyticsModules
#define soap_write__tan__CreateAnalyticsModules(soap, data) ( soap_begin_send(soap) || (soap_serialize__tan__CreateAnalyticsModules(soap, data), 0) || soap_put__tan__CreateAnalyticsModules(soap, data, "tan:CreateAnalyticsModules", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__CreateAnalyticsModules(struct soap*, const struct _tan__CreateAnalyticsModules *, const char*, const char*);

#ifndef soap_read__tan__CreateAnalyticsModules
#define soap_read__tan__CreateAnalyticsModules(soap, data) ( soap_begin_recv(soap) || !soap_get__tan__CreateAnalyticsModules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__CreateAnalyticsModules * SOAP_FMAC4 soap_get__tan__CreateAnalyticsModules(struct soap*, struct _tan__CreateAnalyticsModules *, const char*, const char*);



SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__SupportedAnalyticsModules(struct soap*, const struct tt__SupportedAnalyticsModules *, const char*, const char*);

#ifndef soap_read_tt__SupportedAnalyticsModules
#define soap_read_tt__SupportedAnalyticsModules(soap, data) ( soap_begin_recv(soap) || !soap_get_tt__SupportedAnalyticsModules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct tt__SupportedAnalyticsModules * SOAP_FMAC4 soap_get_tt__SupportedAnalyticsModules(struct soap*, struct tt__SupportedAnalyticsModules *, const char*, const char*);


SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__GetSupportedAnalyticsModules(struct soap*, const struct _tan__GetSupportedAnalyticsModules *, const char*, const char*);

#ifndef soap_read__tan__GetSupportedAnalyticsModules
#define soap_read__tan__GetSupportedAnalyticsModules(soap, data) ( soap_begin_recv(soap) || !soap_get__tan__GetSupportedAnalyticsModules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__GetSupportedAnalyticsModules * SOAP_FMAC4 soap_get__tan__GetSupportedAnalyticsModules(struct soap*, struct _tan__GetSupportedAnalyticsModules *, const char*, const char*);

#ifndef SOAP_TYPE__tan__GetRulesResponse
#define SOAP_TYPE__tan__GetRulesResponse (245)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__GetRulesResponse(struct soap*, struct _tan__GetRulesResponse *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__GetRulesResponse(struct soap*, const struct _tan__GetRulesResponse *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__GetRulesResponse(struct soap*, const char*, int, const struct _tan__GetRulesResponse *, const char*);
SOAP_FMAC3 struct _tan__GetRulesResponse * SOAP_FMAC4 soap_in__tan__GetRulesResponse(struct soap*, const char*, struct _tan__GetRulesResponse *, const char*);

#ifndef soap_write__tan__GetRulesResponse
#define soap_write__tan__GetRulesResponse(soap, data) ( soap_begin_send(soap) || (soap_serialize__tan__GetRulesResponse(soap, data), 0) || soap_put__tan__GetRulesResponse(soap, data, "tan:GetRulesResponse", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__GetRulesResponse(struct soap*, const struct _tan__GetRulesResponse *, const char*, const char*);

#ifndef soap_read__tan__GetRulesResponse
#define soap_read__tan__GetRulesResponse(soap, data) ( soap_begin_recv(soap) || !soap_get__tan__GetRulesResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__GetRulesResponse * SOAP_FMAC4 soap_get__tan__GetRulesResponse(struct soap*, struct _tan__GetRulesResponse *, const char*, const char*);

#ifndef SOAP_TYPE__tan__GetRules
#define SOAP_TYPE__tan__GetRules (244)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__GetRules(struct soap*, struct _tan__GetRules *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__GetRules(struct soap*, const struct _tan__GetRules *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__GetRules(struct soap*, const char*, int, const struct _tan__GetRules *, const char*);
SOAP_FMAC3 struct _tan__GetRules * SOAP_FMAC4 soap_in__tan__GetRules(struct soap*, const char*, struct _tan__GetRules *, const char*);

#ifndef soap_write__tan__GetRules
#define soap_write__tan__GetRules(soap, data) ( soap_begin_send(soap) || (soap_serialize__tan__GetRules(soap, data), 0) || soap_put__tan__GetRules(soap, data, "tan:GetRules", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__GetRules(struct soap*, const struct _tan__GetRules *, const char*, const char*);

#ifndef soap_read__tan__GetRules
#define soap_read__tan__GetRules(soap, data) ( soap_begin_recv(soap) || !soap_get__tan__GetRules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__GetRules * SOAP_FMAC4 soap_get__tan__GetRules(struct soap*, struct _tan__GetRules *, const char*, const char*);

#ifndef SOAP_TYPE__tan__ModifyRulesResponse
#define SOAP_TYPE__tan__ModifyRulesResponse (243)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__ModifyRulesResponse(struct soap*, struct _tan__ModifyRulesResponse *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__ModifyRulesResponse(struct soap*, const struct _tan__ModifyRulesResponse *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__ModifyRulesResponse(struct soap*, const char*, int, const struct _tan__ModifyRulesResponse *, const char*);
SOAP_FMAC3 struct _tan__ModifyRulesResponse * SOAP_FMAC4 soap_in__tan__ModifyRulesResponse(struct soap*, const char*, struct _tan__ModifyRulesResponse *, const char*);

#ifndef soap_write__tan__ModifyRulesResponse
#define soap_write__tan__ModifyRulesResponse(soap, data) ( soap_begin_send(soap) || (soap_serialize__tan__ModifyRulesResponse(soap, data), 0) || soap_put__tan__ModifyRulesResponse(soap, data, "tan:ModifyRulesResponse", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__ModifyRulesResponse(struct soap*, const struct _tan__ModifyRulesResponse *, const char*, const char*);

#ifndef soap_read__tan__ModifyRulesResponse
#define soap_read__tan__ModifyRulesResponse(soap, data) ( soap_begin_recv(soap) || !soap_get__tan__ModifyRulesResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__ModifyRulesResponse * SOAP_FMAC4 soap_get__tan__ModifyRulesResponse(struct soap*, struct _tan__ModifyRulesResponse *, const char*, const char*);

#ifndef SOAP_TYPE__tan__ModifyRules
#define SOAP_TYPE__tan__ModifyRules (242)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__ModifyRules(struct soap*, struct _tan__ModifyRules *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__ModifyRules(struct soap*, const struct _tan__ModifyRules *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__ModifyRules(struct soap*, const char*, int, const struct _tan__ModifyRules *, const char*);
SOAP_FMAC3 struct _tan__ModifyRules * SOAP_FMAC4 soap_in__tan__ModifyRules(struct soap*, const char*, struct _tan__ModifyRules *, const char*);

#ifndef soap_write__tan__ModifyRules
#define soap_write__tan__ModifyRules(soap, data) ( soap_begin_send(soap) || (soap_serialize__tan__ModifyRules(soap, data), 0) || soap_put__tan__ModifyRules(soap, data, "tan:ModifyRules", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__ModifyRules(struct soap*, const struct _tan__ModifyRules *, const char*, const char*);

#ifndef soap_read__tan__ModifyRules
#define soap_read__tan__ModifyRules(soap, data) ( soap_begin_recv(soap) || !soap_get__tan__ModifyRules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__ModifyRules * SOAP_FMAC4 soap_get__tan__ModifyRules(struct soap*, struct _tan__ModifyRules *, const char*, const char*);

#ifndef SOAP_TYPE__tan__DeleteRulesResponse
#define SOAP_TYPE__tan__DeleteRulesResponse (241)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__DeleteRulesResponse(struct soap*, struct _tan__DeleteRulesResponse *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__DeleteRulesResponse(struct soap*, const struct _tan__DeleteRulesResponse *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__DeleteRulesResponse(struct soap*, const char*, int, const struct _tan__DeleteRulesResponse *, const char*);
SOAP_FMAC3 struct _tan__DeleteRulesResponse * SOAP_FMAC4 soap_in__tan__DeleteRulesResponse(struct soap*, const char*, struct _tan__DeleteRulesResponse *, const char*);

#ifndef soap_write__tan__DeleteRulesResponse
#define soap_write__tan__DeleteRulesResponse(soap, data) ( soap_begin_send(soap) || (soap_serialize__tan__DeleteRulesResponse(soap, data), 0) || soap_put__tan__DeleteRulesResponse(soap, data, "tan:DeleteRulesResponse", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__DeleteRulesResponse(struct soap*, const struct _tan__DeleteRulesResponse *, const char*, const char*);

#ifndef soap_read__tan__DeleteRulesResponse
#define soap_read__tan__DeleteRulesResponse(soap, data) ( soap_begin_recv(soap) || !soap_get__tan__DeleteRulesResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__DeleteRulesResponse * SOAP_FMAC4 soap_get__tan__DeleteRulesResponse(struct soap*, struct _tan__DeleteRulesResponse *, const char*, const char*);

#ifndef SOAP_TYPE__tan__DeleteRules
#define SOAP_TYPE__tan__DeleteRules (239)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__DeleteRules(struct soap*, struct _tan__DeleteRules *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__DeleteRules(struct soap*, const struct _tan__DeleteRules *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__DeleteRules(struct soap*, const char*, int, const struct _tan__DeleteRules *, const char*);
SOAP_FMAC3 struct _tan__DeleteRules * SOAP_FMAC4 soap_in__tan__DeleteRules(struct soap*, const char*, struct _tan__DeleteRules *, const char*);

#ifndef soap_write__tan__DeleteRules
#define soap_write__tan__DeleteRules(soap, data) ( soap_begin_send(soap) || (soap_serialize__tan__DeleteRules(soap, data), 0) || soap_put__tan__DeleteRules(soap, data, "tan:DeleteRules", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__DeleteRules(struct soap*, const struct _tan__DeleteRules *, const char*, const char*);

#ifndef soap_read__tan__DeleteRules
#define soap_read__tan__DeleteRules(soap, data) ( soap_begin_recv(soap) || !soap_get__tan__DeleteRules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__DeleteRules * SOAP_FMAC4 soap_get__tan__DeleteRules(struct soap*, struct _tan__DeleteRules *, const char*, const char*);

#ifndef SOAP_TYPE__tan__CreateRulesResponse
#define SOAP_TYPE__tan__CreateRulesResponse (238)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__CreateRulesResponse(struct soap*, struct _tan__CreateRulesResponse *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__CreateRulesResponse(struct soap*, const struct _tan__CreateRulesResponse *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__CreateRulesResponse(struct soap*, const char*, int, const struct _tan__CreateRulesResponse *, const char*);
SOAP_FMAC3 struct _tan__CreateRulesResponse * SOAP_FMAC4 soap_in__tan__CreateRulesResponse(struct soap*, const char*, struct _tan__CreateRulesResponse *, const char*);

#ifndef soap_write__tan__CreateRulesResponse
#define soap_write__tan__CreateRulesResponse(soap, data) ( soap_begin_send(soap) || (soap_serialize__tan__CreateRulesResponse(soap, data), 0) || soap_put__tan__CreateRulesResponse(soap, data, "tan:CreateRulesResponse", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__CreateRulesResponse(struct soap*, const struct _tan__CreateRulesResponse *, const char*, const char*);

#ifndef soap_read__tan__CreateRulesResponse
#define soap_read__tan__CreateRulesResponse(soap, data) ( soap_begin_recv(soap) || !soap_get__tan__CreateRulesResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__CreateRulesResponse * SOAP_FMAC4 soap_get__tan__CreateRulesResponse(struct soap*, struct _tan__CreateRulesResponse *, const char*, const char*);

#ifndef SOAP_TYPE_tt__Config
#define SOAP_TYPE_tt__Config (236)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__Config(struct soap*, struct tt__Config *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__Config(struct soap*, const struct tt__Config *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__Config(struct soap*, const char*, int, const struct tt__Config *, const char*);
SOAP_FMAC3 struct tt__Config * SOAP_FMAC4 soap_in_tt__Config(struct soap*, const char*, struct tt__Config *, const char*);

#ifndef soap_write_tt__Config
#define soap_write_tt__Config(soap, data) ( soap_begin_send(soap) || (soap_serialize_tt__Config(soap, data), 0) || soap_put_tt__Config(soap, data, "tt:Config", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__Config(struct soap*, const struct tt__Config *, const char*, const char*);

#ifndef soap_read_tt__Config
#define soap_read_tt__Config(soap, data) ( soap_begin_recv(soap) || !soap_get_tt__Config(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct tt__Config * SOAP_FMAC4 soap_get_tt__Config(struct soap*, struct tt__Config *, const char*, const char*);

#ifndef SOAP_TYPE__tan__CreateRules
#define SOAP_TYPE__tan__CreateRules (235)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__CreateRules(struct soap*, struct _tan__CreateRules *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__CreateRules(struct soap*, const struct _tan__CreateRules *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__CreateRules(struct soap*, const char*, int, const struct _tan__CreateRules *, const char*);
SOAP_FMAC3 struct _tan__CreateRules * SOAP_FMAC4 soap_in__tan__CreateRules(struct soap*, const char*, struct _tan__CreateRules *, const char*);

#ifndef soap_write__tan__CreateRules
#define soap_write__tan__CreateRules(soap, data) ( soap_begin_send(soap) || (soap_serialize__tan__CreateRules(soap, data), 0) || soap_put__tan__CreateRules(soap, data, "tan:CreateRules", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__CreateRules(struct soap*, const struct _tan__CreateRules *, const char*, const char*);

#ifndef soap_read__tan__CreateRules
#define soap_read__tan__CreateRules(soap, data) ( soap_begin_recv(soap) || !soap_get__tan__CreateRules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__CreateRules * SOAP_FMAC4 soap_get__tan__CreateRules(struct soap*, struct _tan__CreateRules *, const char*, const char*);

#ifndef SOAP_TYPE_tt__SupportedRules
#define SOAP_TYPE_tt__SupportedRules (233)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__SupportedRules(struct soap*, struct tt__SupportedRules *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__SupportedRules(struct soap*, const struct tt__SupportedRules *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__SupportedRules(struct soap*, const char*, int, const struct tt__SupportedRules *, const char*);
SOAP_FMAC3 struct tt__SupportedRules * SOAP_FMAC4 soap_in_tt__SupportedRules(struct soap*, const char*, struct tt__SupportedRules *, const char*);

#ifndef soap_write_tt__SupportedRules
#define soap_write_tt__SupportedRules(soap, data) ( soap_begin_send(soap) || (soap_serialize_tt__SupportedRules(soap, data), 0) || soap_put_tt__SupportedRules(soap, data, "tt:SupportedRules", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__SupportedRules(struct soap*, const struct tt__SupportedRules *, const char*, const char*);

#ifndef soap_read_tt__SupportedRules
#define soap_read_tt__SupportedRules(soap, data) ( soap_begin_recv(soap) || !soap_get_tt__SupportedRules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct tt__SupportedRules * SOAP_FMAC4 soap_get_tt__SupportedRules(struct soap*, struct tt__SupportedRules *, const char*, const char*);



SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__GetSupportedRulesResponse(struct soap*, const struct _tan__GetSupportedRulesResponse *, const char*, const char*);

#ifndef soap_read__tan__GetSupportedRulesResponse
#define soap_read__tan__GetSupportedRulesResponse(soap, data) ( soap_begin_recv(soap) || !soap_get__tan__GetSupportedRulesResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__GetSupportedRulesResponse * SOAP_FMAC4 soap_get__tan__GetSupportedRulesResponse(struct soap*, struct _tan__GetSupportedRulesResponse *, const char*, const char*);

#ifndef SOAP_TYPE__tan__GetSupportedRules
#define SOAP_TYPE__tan__GetSupportedRules (231)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default__tan__GetSupportedRules(struct soap*, struct _tan__GetSupportedRules *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize__tan__GetSupportedRules(struct soap*, const struct _tan__GetSupportedRules *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out__tan__GetSupportedRules(struct soap*, const char*, int, const struct _tan__GetSupportedRules *, const char*);
SOAP_FMAC3 struct _tan__GetSupportedRules * SOAP_FMAC4 soap_in__tan__GetSupportedRules(struct soap*, const char*, struct _tan__GetSupportedRules *, const char*);

#ifndef soap_write__tan__GetSupportedRules
#define soap_write__tan__GetSupportedRules(soap, data) ( soap_begin_send(soap) || (soap_serialize__tan__GetSupportedRules(soap, data), 0) || soap_put__tan__GetSupportedRules(soap, data, "tan:GetSupportedRules", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put__tan__GetSupportedRules(struct soap*, const struct _tan__GetSupportedRules *, const char*, const char*);

#ifndef soap_read__tan__GetSupportedRules
#define soap_read__tan__GetSupportedRules(soap, data) ( soap_begin_recv(soap) || !soap_get__tan__GetSupportedRules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__GetSupportedRules * SOAP_FMAC4 soap_get__tan__GetSupportedRules(struct soap*, struct _tan__GetSupportedRules *, const char*, const char*);


#ifndef SOAP_TYPE_PointerTo_tan__ModifyRulesResponse
#define SOAP_TYPE_PointerTo_tan__ModifyRulesResponse (1760)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__ModifyRulesResponse(struct soap*, struct _tan__ModifyRulesResponse *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__ModifyRulesResponse(struct soap*, const char *, int, struct _tan__ModifyRulesResponse *const*, const char *);
SOAP_FMAC3 struct _tan__ModifyRulesResponse ** SOAP_FMAC4 soap_in_PointerTo_tan__ModifyRulesResponse(struct soap*, const char*, struct _tan__ModifyRulesResponse **, const char*);

#ifndef soap_write_PointerTo_tan__ModifyRulesResponse
#define soap_write_PointerTo_tan__ModifyRulesResponse(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_tan__ModifyRulesResponse(soap, data), 0) || soap_put_PointerTo_tan__ModifyRulesResponse(soap, data, "tan:ModifyRulesResponse", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__ModifyRulesResponse(struct soap*, struct _tan__ModifyRulesResponse *const*, const char*, const char*);

#ifndef soap_read_PointerTo_tan__ModifyRulesResponse
#define soap_read_PointerTo_tan__ModifyRulesResponse(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_tan__ModifyRulesResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__ModifyRulesResponse ** SOAP_FMAC4 soap_get_PointerTo_tan__ModifyRulesResponse(struct soap*, struct _tan__ModifyRulesResponse **, const char*, const char*);

#ifndef SOAP_TYPE_PointerTo_tan__ModifyRules
#define SOAP_TYPE_PointerTo_tan__ModifyRules (1759)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__ModifyRules(struct soap*, struct _tan__ModifyRules *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__ModifyRules(struct soap*, const char *, int, struct _tan__ModifyRules *const*, const char *);
SOAP_FMAC3 struct _tan__ModifyRules ** SOAP_FMAC4 soap_in_PointerTo_tan__ModifyRules(struct soap*, const char*, struct _tan__ModifyRules **, const char*);

#ifndef soap_write_PointerTo_tan__ModifyRules
#define soap_write_PointerTo_tan__ModifyRules(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_tan__ModifyRules(soap, data), 0) || soap_put_PointerTo_tan__ModifyRules(soap, data, "tan:ModifyRules", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__ModifyRules(struct soap*, struct _tan__ModifyRules *const*, const char*, const char*);

#ifndef soap_read_PointerTo_tan__ModifyRules
#define soap_read_PointerTo_tan__ModifyRules(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_tan__ModifyRules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__ModifyRules ** SOAP_FMAC4 soap_get_PointerTo_tan__ModifyRules(struct soap*, struct _tan__ModifyRules **, const char*, const char*);

#ifndef SOAP_TYPE_PointerTo_tan__GetRulesResponse
#define SOAP_TYPE_PointerTo_tan__GetRulesResponse (1756)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__GetRulesResponse(struct soap*, struct _tan__GetRulesResponse *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__GetRulesResponse(struct soap*, const char *, int, struct _tan__GetRulesResponse *const*, const char *);
SOAP_FMAC3 struct _tan__GetRulesResponse ** SOAP_FMAC4 soap_in_PointerTo_tan__GetRulesResponse(struct soap*, const char*, struct _tan__GetRulesResponse **, const char*);

#ifndef soap_write_PointerTo_tan__GetRulesResponse
#define soap_write_PointerTo_tan__GetRulesResponse(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_tan__GetRulesResponse(soap, data), 0) || soap_put_PointerTo_tan__GetRulesResponse(soap, data, "tan:GetRulesResponse", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__GetRulesResponse(struct soap*, struct _tan__GetRulesResponse *const*, const char*, const char*);

#ifndef soap_read_PointerTo_tan__GetRulesResponse
#define soap_read_PointerTo_tan__GetRulesResponse(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_tan__GetRulesResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__GetRulesResponse ** SOAP_FMAC4 soap_get_PointerTo_tan__GetRulesResponse(struct soap*, struct _tan__GetRulesResponse **, const char*, const char*);

#ifndef SOAP_TYPE_PointerTo_tan__GetRules
#define SOAP_TYPE_PointerTo_tan__GetRules (1755)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__GetRules(struct soap*, struct _tan__GetRules *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__GetRules(struct soap*, const char *, int, struct _tan__GetRules *const*, const char *);
SOAP_FMAC3 struct _tan__GetRules ** SOAP_FMAC4 soap_in_PointerTo_tan__GetRules(struct soap*, const char*, struct _tan__GetRules **, const char*);

#ifndef soap_write_PointerTo_tan__GetRules
#define soap_write_PointerTo_tan__GetRules(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_tan__GetRules(soap, data), 0) || soap_put_PointerTo_tan__GetRules(soap, data, "tan:GetRules", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__GetRules(struct soap*, struct _tan__GetRules *const*, const char*, const char*);

#ifndef soap_read_PointerTo_tan__GetRules
#define soap_read_PointerTo_tan__GetRules(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_tan__GetRules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__GetRules ** SOAP_FMAC4 soap_get_PointerTo_tan__GetRules(struct soap*, struct _tan__GetRules **, const char*, const char*);

#ifndef SOAP_TYPE_PointerTo_tan__DeleteRulesResponse
#define SOAP_TYPE_PointerTo_tan__DeleteRulesResponse (1752)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__DeleteRulesResponse(struct soap*, struct _tan__DeleteRulesResponse *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__DeleteRulesResponse(struct soap*, const char *, int, struct _tan__DeleteRulesResponse *const*, const char *);
SOAP_FMAC3 struct _tan__DeleteRulesResponse ** SOAP_FMAC4 soap_in_PointerTo_tan__DeleteRulesResponse(struct soap*, const char*, struct _tan__DeleteRulesResponse **, const char*);

#ifndef soap_write_PointerTo_tan__DeleteRulesResponse
#define soap_write_PointerTo_tan__DeleteRulesResponse(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_tan__DeleteRulesResponse(soap, data), 0) || soap_put_PointerTo_tan__DeleteRulesResponse(soap, data, "tan:DeleteRulesResponse", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__DeleteRulesResponse(struct soap*, struct _tan__DeleteRulesResponse *const*, const char*, const char*);

#ifndef soap_read_PointerTo_tan__DeleteRulesResponse
#define soap_read_PointerTo_tan__DeleteRulesResponse(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_tan__DeleteRulesResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__DeleteRulesResponse ** SOAP_FMAC4 soap_get_PointerTo_tan__DeleteRulesResponse(struct soap*, struct _tan__DeleteRulesResponse **, const char*, const char*);

#ifndef SOAP_TYPE_PointerTo_tan__DeleteRules
#define SOAP_TYPE_PointerTo_tan__DeleteRules (1751)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__DeleteRules(struct soap*, struct _tan__DeleteRules *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__DeleteRules(struct soap*, const char *, int, struct _tan__DeleteRules *const*, const char *);
SOAP_FMAC3 struct _tan__DeleteRules ** SOAP_FMAC4 soap_in_PointerTo_tan__DeleteRules(struct soap*, const char*, struct _tan__DeleteRules **, const char*);

#ifndef soap_write_PointerTo_tan__DeleteRules
#define soap_write_PointerTo_tan__DeleteRules(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_tan__DeleteRules(soap, data), 0) || soap_put_PointerTo_tan__DeleteRules(soap, data, "tan:DeleteRules", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__DeleteRules(struct soap*, struct _tan__DeleteRules *const*, const char*, const char*);

#ifndef soap_read_PointerTo_tan__DeleteRules
#define soap_read_PointerTo_tan__DeleteRules(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_tan__DeleteRules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__DeleteRules ** SOAP_FMAC4 soap_get_PointerTo_tan__DeleteRules(struct soap*, struct _tan__DeleteRules **, const char*, const char*);

#ifndef SOAP_TYPE_PointerTo_tan__CreateRulesResponse
#define SOAP_TYPE_PointerTo_tan__CreateRulesResponse (1748)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__CreateRulesResponse(struct soap*, struct _tan__CreateRulesResponse *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__CreateRulesResponse(struct soap*, const char *, int, struct _tan__CreateRulesResponse *const*, const char *);
SOAP_FMAC3 struct _tan__CreateRulesResponse ** SOAP_FMAC4 soap_in_PointerTo_tan__CreateRulesResponse(struct soap*, const char*, struct _tan__CreateRulesResponse **, const char*);

#ifndef soap_write_PointerTo_tan__CreateRulesResponse
#define soap_write_PointerTo_tan__CreateRulesResponse(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_tan__CreateRulesResponse(soap, data), 0) || soap_put_PointerTo_tan__CreateRulesResponse(soap, data, "tan:CreateRulesResponse", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__CreateRulesResponse(struct soap*, struct _tan__CreateRulesResponse *const*, const char*, const char*);

#ifndef soap_read_PointerTo_tan__CreateRulesResponse
#define soap_read_PointerTo_tan__CreateRulesResponse(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_tan__CreateRulesResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__CreateRulesResponse ** SOAP_FMAC4 soap_get_PointerTo_tan__CreateRulesResponse(struct soap*, struct _tan__CreateRulesResponse **, const char*, const char*);

#ifndef SOAP_TYPE_PointerTo_tan__CreateRules
#define SOAP_TYPE_PointerTo_tan__CreateRules (1747)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__CreateRules(struct soap*, struct _tan__CreateRules *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__CreateRules(struct soap*, const char *, int, struct _tan__CreateRules *const*, const char *);
SOAP_FMAC3 struct _tan__CreateRules ** SOAP_FMAC4 soap_in_PointerTo_tan__CreateRules(struct soap*, const char*, struct _tan__CreateRules **, const char*);

#ifndef soap_write_PointerTo_tan__CreateRules
#define soap_write_PointerTo_tan__CreateRules(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_tan__CreateRules(soap, data), 0) || soap_put_PointerTo_tan__CreateRules(soap, data, "tan:CreateRules", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__CreateRules(struct soap*, struct _tan__CreateRules *const*, const char*, const char*);

#ifndef soap_read_PointerTo_tan__CreateRules
#define soap_read_PointerTo_tan__CreateRules(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_tan__CreateRules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__CreateRules ** SOAP_FMAC4 soap_get_PointerTo_tan__CreateRules(struct soap*, struct _tan__CreateRules **, const char*, const char*);




#ifndef soap_read_PointerTo_tan__GetSupportedRulesResponse
#define soap_read_PointerTo_tan__GetSupportedRulesResponse(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_tan__GetSupportedRulesResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__GetSupportedRulesResponse ** SOAP_FMAC4 soap_get_PointerTo_tan__GetSupportedRulesResponse(struct soap*, struct _tan__GetSupportedRulesResponse **, const char*, const char*);

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__GetSupportedRules(struct soap*, struct _tan__GetSupportedRules *const*, const char*, const char*);

#ifndef soap_read_PointerTo_tan__GetSupportedRules
#define soap_read_PointerTo_tan__GetSupportedRules(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_tan__GetSupportedRules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__GetSupportedRules ** SOAP_FMAC4 soap_get_PointerTo_tan__GetSupportedRules(struct soap*, struct _tan__GetSupportedRules **, const char*, const char*);

#ifndef SOAP_TYPE_PointerTo_tan__ModifyAnalyticsModulesResponse
#define SOAP_TYPE_PointerTo_tan__ModifyAnalyticsModulesResponse (1740)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__ModifyAnalyticsModulesResponse(struct soap*, struct _tan__ModifyAnalyticsModulesResponse *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__ModifyAnalyticsModulesResponse(struct soap*, const char *, int, struct _tan__ModifyAnalyticsModulesResponse *const*, const char *);
SOAP_FMAC3 struct _tan__ModifyAnalyticsModulesResponse ** SOAP_FMAC4 soap_in_PointerTo_tan__ModifyAnalyticsModulesResponse(struct soap*, const char*, struct _tan__ModifyAnalyticsModulesResponse **, const char*);

#ifndef soap_write_PointerTo_tan__ModifyAnalyticsModulesResponse
#define soap_write_PointerTo_tan__ModifyAnalyticsModulesResponse(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_tan__ModifyAnalyticsModulesResponse(soap, data), 0) || soap_put_PointerTo_tan__ModifyAnalyticsModulesResponse(soap, data, "tan:ModifyAnalyticsModulesResponse", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__ModifyAnalyticsModulesResponse(struct soap*, struct _tan__ModifyAnalyticsModulesResponse *const*, const char*, const char*);

#ifndef soap_read_PointerTo_tan__ModifyAnalyticsModulesResponse
#define soap_read_PointerTo_tan__ModifyAnalyticsModulesResponse(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_tan__ModifyAnalyticsModulesResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__ModifyAnalyticsModulesResponse ** SOAP_FMAC4 soap_get_PointerTo_tan__ModifyAnalyticsModulesResponse(struct soap*, struct _tan__ModifyAnalyticsModulesResponse **, const char*, const char*);

#ifndef SOAP_TYPE_PointerTo_tan__ModifyAnalyticsModules
#define SOAP_TYPE_PointerTo_tan__ModifyAnalyticsModules (1739)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__ModifyAnalyticsModules(struct soap*, struct _tan__ModifyAnalyticsModules *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__ModifyAnalyticsModules(struct soap*, const char *, int, struct _tan__ModifyAnalyticsModules *const*, const char *);
SOAP_FMAC3 struct _tan__ModifyAnalyticsModules ** SOAP_FMAC4 soap_in_PointerTo_tan__ModifyAnalyticsModules(struct soap*, const char*, struct _tan__ModifyAnalyticsModules **, const char*);

#ifndef soap_write_PointerTo_tan__ModifyAnalyticsModules
#define soap_write_PointerTo_tan__ModifyAnalyticsModules(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_tan__ModifyAnalyticsModules(soap, data), 0) || soap_put_PointerTo_tan__ModifyAnalyticsModules(soap, data, "tan:ModifyAnalyticsModules", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__ModifyAnalyticsModules(struct soap*, struct _tan__ModifyAnalyticsModules *const*, const char*, const char*);

#ifndef soap_read_PointerTo_tan__ModifyAnalyticsModules
#define soap_read_PointerTo_tan__ModifyAnalyticsModules(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_tan__ModifyAnalyticsModules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__ModifyAnalyticsModules ** SOAP_FMAC4 soap_get_PointerTo_tan__ModifyAnalyticsModules(struct soap*, struct _tan__ModifyAnalyticsModules **, const char*, const char*);

#ifndef SOAP_TYPE_PointerTo_tan__GetAnalyticsModulesResponse
#define SOAP_TYPE_PointerTo_tan__GetAnalyticsModulesResponse (1736)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__GetAnalyticsModulesResponse(struct soap*, struct _tan__GetAnalyticsModulesResponse *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__GetAnalyticsModulesResponse(struct soap*, const char *, int, struct _tan__GetAnalyticsModulesResponse *const*, const char *);
SOAP_FMAC3 struct _tan__GetAnalyticsModulesResponse ** SOAP_FMAC4 soap_in_PointerTo_tan__GetAnalyticsModulesResponse(struct soap*, const char*, struct _tan__GetAnalyticsModulesResponse **, const char*);

#ifndef soap_write_PointerTo_tan__GetAnalyticsModulesResponse
#define soap_write_PointerTo_tan__GetAnalyticsModulesResponse(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_tan__GetAnalyticsModulesResponse(soap, data), 0) || soap_put_PointerTo_tan__GetAnalyticsModulesResponse(soap, data, "tan:GetAnalyticsModulesResponse", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__GetAnalyticsModulesResponse(struct soap*, struct _tan__GetAnalyticsModulesResponse *const*, const char*, const char*);

#ifndef soap_read_PointerTo_tan__GetAnalyticsModulesResponse
#define soap_read_PointerTo_tan__GetAnalyticsModulesResponse(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_tan__GetAnalyticsModulesResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__GetAnalyticsModulesResponse ** SOAP_FMAC4 soap_get_PointerTo_tan__GetAnalyticsModulesResponse(struct soap*, struct _tan__GetAnalyticsModulesResponse **, const char*, const char*);

#ifndef SOAP_TYPE_PointerTo_tan__GetAnalyticsModules
#define SOAP_TYPE_PointerTo_tan__GetAnalyticsModules (1735)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__GetAnalyticsModules(struct soap*, struct _tan__GetAnalyticsModules *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__GetAnalyticsModules(struct soap*, const char *, int, struct _tan__GetAnalyticsModules *const*, const char *);
SOAP_FMAC3 struct _tan__GetAnalyticsModules ** SOAP_FMAC4 soap_in_PointerTo_tan__GetAnalyticsModules(struct soap*, const char*, struct _tan__GetAnalyticsModules **, const char*);

#ifndef soap_write_PointerTo_tan__GetAnalyticsModules
#define soap_write_PointerTo_tan__GetAnalyticsModules(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_tan__GetAnalyticsModules(soap, data), 0) || soap_put_PointerTo_tan__GetAnalyticsModules(soap, data, "tan:GetAnalyticsModules", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__GetAnalyticsModules(struct soap*, struct _tan__GetAnalyticsModules *const*, const char*, const char*);

#ifndef soap_read_PointerTo_tan__GetAnalyticsModules
#define soap_read_PointerTo_tan__GetAnalyticsModules(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_tan__GetAnalyticsModules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__GetAnalyticsModules ** SOAP_FMAC4 soap_get_PointerTo_tan__GetAnalyticsModules(struct soap*, struct _tan__GetAnalyticsModules **, const char*, const char*);

#ifndef SOAP_TYPE_PointerTo_tan__DeleteAnalyticsModulesResponse
#define SOAP_TYPE_PointerTo_tan__DeleteAnalyticsModulesResponse (1732)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__DeleteAnalyticsModulesResponse(struct soap*, struct _tan__DeleteAnalyticsModulesResponse *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__DeleteAnalyticsModulesResponse(struct soap*, const char *, int, struct _tan__DeleteAnalyticsModulesResponse *const*, const char *);
SOAP_FMAC3 struct _tan__DeleteAnalyticsModulesResponse ** SOAP_FMAC4 soap_in_PointerTo_tan__DeleteAnalyticsModulesResponse(struct soap*, const char*, struct _tan__DeleteAnalyticsModulesResponse **, const char*);

#ifndef soap_write_PointerTo_tan__DeleteAnalyticsModulesResponse
#define soap_write_PointerTo_tan__DeleteAnalyticsModulesResponse(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_tan__DeleteAnalyticsModulesResponse(soap, data), 0) || soap_put_PointerTo_tan__DeleteAnalyticsModulesResponse(soap, data, "tan:DeleteAnalyticsModulesResponse", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__DeleteAnalyticsModulesResponse(struct soap*, struct _tan__DeleteAnalyticsModulesResponse *const*, const char*, const char*);

#ifndef soap_read_PointerTo_tan__DeleteAnalyticsModulesResponse
#define soap_read_PointerTo_tan__DeleteAnalyticsModulesResponse(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_tan__DeleteAnalyticsModulesResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__DeleteAnalyticsModulesResponse ** SOAP_FMAC4 soap_get_PointerTo_tan__DeleteAnalyticsModulesResponse(struct soap*, struct _tan__DeleteAnalyticsModulesResponse **, const char*, const char*);

#ifndef SOAP_TYPE_PointerTo_tan__DeleteAnalyticsModules
#define SOAP_TYPE_PointerTo_tan__DeleteAnalyticsModules (1731)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__DeleteAnalyticsModules(struct soap*, struct _tan__DeleteAnalyticsModules *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__DeleteAnalyticsModules(struct soap*, const char *, int, struct _tan__DeleteAnalyticsModules *const*, const char *);
SOAP_FMAC3 struct _tan__DeleteAnalyticsModules ** SOAP_FMAC4 soap_in_PointerTo_tan__DeleteAnalyticsModules(struct soap*, const char*, struct _tan__DeleteAnalyticsModules **, const char*);

#ifndef soap_write_PointerTo_tan__DeleteAnalyticsModules
#define soap_write_PointerTo_tan__DeleteAnalyticsModules(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_tan__DeleteAnalyticsModules(soap, data), 0) || soap_put_PointerTo_tan__DeleteAnalyticsModules(soap, data, "tan:DeleteAnalyticsModules", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__DeleteAnalyticsModules(struct soap*, struct _tan__DeleteAnalyticsModules *const*, const char*, const char*);

#ifndef soap_read_PointerTo_tan__DeleteAnalyticsModules
#define soap_read_PointerTo_tan__DeleteAnalyticsModules(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_tan__DeleteAnalyticsModules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__DeleteAnalyticsModules ** SOAP_FMAC4 soap_get_PointerTo_tan__DeleteAnalyticsModules(struct soap*, struct _tan__DeleteAnalyticsModules **, const char*, const char*);

#ifndef SOAP_TYPE_PointerTo_tan__CreateAnalyticsModulesResponse
#define SOAP_TYPE_PointerTo_tan__CreateAnalyticsModulesResponse (1728)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__CreateAnalyticsModulesResponse(struct soap*, struct _tan__CreateAnalyticsModulesResponse *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__CreateAnalyticsModulesResponse(struct soap*, const char *, int, struct _tan__CreateAnalyticsModulesResponse *const*, const char *);
SOAP_FMAC3 struct _tan__CreateAnalyticsModulesResponse ** SOAP_FMAC4 soap_in_PointerTo_tan__CreateAnalyticsModulesResponse(struct soap*, const char*, struct _tan__CreateAnalyticsModulesResponse **, const char*);

#ifndef soap_write_PointerTo_tan__CreateAnalyticsModulesResponse
#define soap_write_PointerTo_tan__CreateAnalyticsModulesResponse(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_tan__CreateAnalyticsModulesResponse(soap, data), 0) || soap_put_PointerTo_tan__CreateAnalyticsModulesResponse(soap, data, "tan:CreateAnalyticsModulesResponse", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__CreateAnalyticsModulesResponse(struct soap*, struct _tan__CreateAnalyticsModulesResponse *const*, const char*, const char*);

#ifndef soap_read_PointerTo_tan__CreateAnalyticsModulesResponse
#define soap_read_PointerTo_tan__CreateAnalyticsModulesResponse(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_tan__CreateAnalyticsModulesResponse(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__CreateAnalyticsModulesResponse ** SOAP_FMAC4 soap_get_PointerTo_tan__CreateAnalyticsModulesResponse(struct soap*, struct _tan__CreateAnalyticsModulesResponse **, const char*, const char*);

#ifndef SOAP_TYPE_PointerTo_tan__CreateAnalyticsModules
#define SOAP_TYPE_PointerTo_tan__CreateAnalyticsModules (1727)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTo_tan__CreateAnalyticsModules(struct soap*, struct _tan__CreateAnalyticsModules *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTo_tan__CreateAnalyticsModules(struct soap*, const char *, int, struct _tan__CreateAnalyticsModules *const*, const char *);
SOAP_FMAC3 struct _tan__CreateAnalyticsModules ** SOAP_FMAC4 soap_in_PointerTo_tan__CreateAnalyticsModules(struct soap*, const char*, struct _tan__CreateAnalyticsModules **, const char*);

#ifndef soap_write_PointerTo_tan__CreateAnalyticsModules
#define soap_write_PointerTo_tan__CreateAnalyticsModules(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTo_tan__CreateAnalyticsModules(soap, data), 0) || soap_put_PointerTo_tan__CreateAnalyticsModules(soap, data, "tan:CreateAnalyticsModules", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__CreateAnalyticsModules(struct soap*, struct _tan__CreateAnalyticsModules *const*, const char*, const char*);

#ifndef soap_read_PointerTo_tan__CreateAnalyticsModules
#define soap_read_PointerTo_tan__CreateAnalyticsModules(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_tan__CreateAnalyticsModules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__CreateAnalyticsModules ** SOAP_FMAC4 soap_get_PointerTo_tan__CreateAnalyticsModules(struct soap*, struct _tan__CreateAnalyticsModules **, const char*, const char*);

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTo_tan__GetSupportedAnalyticsModules(struct soap*, struct _tan__GetSupportedAnalyticsModules *const*, const char*, const char*);

#ifndef soap_read_PointerTo_tan__GetSupportedAnalyticsModules
#define soap_read_PointerTo_tan__GetSupportedAnalyticsModules(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTo_tan__GetSupportedAnalyticsModules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tan__GetSupportedAnalyticsModules ** SOAP_FMAC4 soap_get_PointerTo_tan__GetSupportedAnalyticsModules(struct soap*, struct _tan__GetSupportedAnalyticsModules **, const char*, const char*);






///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////






#ifndef SOAP_TYPE_PointerTott__SupportedAnalyticsModulesExtension
#define SOAP_TYPE_PointerTott__SupportedAnalyticsModulesExtension (840)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__SupportedAnalyticsModulesExtension(struct soap*, struct tt__SupportedAnalyticsModulesExtension *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__SupportedAnalyticsModulesExtension(struct soap*, const char *, int, struct tt__SupportedAnalyticsModulesExtension *const*, const char *);
SOAP_FMAC3 struct tt__SupportedAnalyticsModulesExtension ** SOAP_FMAC4 soap_in_PointerTott__SupportedAnalyticsModulesExtension(struct soap*, const char*, struct tt__SupportedAnalyticsModulesExtension **, const char*);

#ifndef soap_write_PointerTott__SupportedAnalyticsModulesExtension
#define soap_write_PointerTott__SupportedAnalyticsModulesExtension(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTott__SupportedAnalyticsModulesExtension(soap, data), 0) || soap_put_PointerTott__SupportedAnalyticsModulesExtension(soap, data, "tt:SupportedAnalyticsModulesExtension", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__SupportedAnalyticsModulesExtension(struct soap*, struct tt__SupportedAnalyticsModulesExtension *const*, const char*, const char*);

#ifndef soap_read_PointerTott__SupportedAnalyticsModulesExtension
#define soap_read_PointerTott__SupportedAnalyticsModulesExtension(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTott__SupportedAnalyticsModulesExtension(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct tt__SupportedAnalyticsModulesExtension ** SOAP_FMAC4 soap_get_PointerTott__SupportedAnalyticsModulesExtension(struct soap*, struct tt__SupportedAnalyticsModulesExtension **, const char*, const char*);

#ifndef SOAP_TYPE_PointerTott__SupportedRulesExtension
#define SOAP_TYPE_PointerTott__SupportedRulesExtension (838)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__SupportedRulesExtension(struct soap*, struct tt__SupportedRulesExtension *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__SupportedRulesExtension(struct soap*, const char *, int, struct tt__SupportedRulesExtension *const*, const char *);
SOAP_FMAC3 struct tt__SupportedRulesExtension ** SOAP_FMAC4 soap_in_PointerTott__SupportedRulesExtension(struct soap*, const char*, struct tt__SupportedRulesExtension **, const char*);

#ifndef soap_write_PointerTott__SupportedRulesExtension
#define soap_write_PointerTott__SupportedRulesExtension(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTott__SupportedRulesExtension(soap, data), 0) || soap_put_PointerTott__SupportedRulesExtension(soap, data, "tt:SupportedRulesExtension", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__SupportedRulesExtension(struct soap*, struct tt__SupportedRulesExtension *const*, const char*, const char*);

#ifndef soap_read_PointerTott__SupportedRulesExtension
#define soap_read_PointerTott__SupportedRulesExtension(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTott__SupportedRulesExtension(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct tt__SupportedRulesExtension ** SOAP_FMAC4 soap_get_PointerTott__SupportedRulesExtension(struct soap*, struct tt__SupportedRulesExtension **, const char*, const char*);


SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__SupportedAnalyticsModules(struct soap*, struct tt__SupportedAnalyticsModules *const*, const char*, const char*);

#ifndef soap_read_PointerTott__SupportedAnalyticsModules
#define soap_read_PointerTott__SupportedAnalyticsModules(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTott__SupportedAnalyticsModules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct tt__SupportedAnalyticsModules ** SOAP_FMAC4 soap_get_PointerTott__SupportedAnalyticsModules(struct soap*, struct tt__SupportedAnalyticsModules **, const char*, const char*);






#ifndef SOAP_TYPE___dare__ModifyRules
#define SOAP_TYPE___dare__ModifyRules (1762)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default___dare__ModifyRules(struct soap*, struct __dare__ModifyRules *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___dare__ModifyRules(struct soap*, const struct __dare__ModifyRules *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out___dare__ModifyRules(struct soap*, const char*, int, const struct __dare__ModifyRules *, const char*);
SOAP_FMAC3 struct __dare__ModifyRules * SOAP_FMAC4 soap_in___dare__ModifyRules(struct soap*, const char*, struct __dare__ModifyRules *, const char*);

#ifndef soap_write___dare__ModifyRules
#define soap_write___dare__ModifyRules(soap, data) ( soap_begin_send(soap) || (soap_serialize___dare__ModifyRules(soap, data), 0) || soap_put___dare__ModifyRules(soap, data, "-dare:ModifyRules", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put___dare__ModifyRules(struct soap*, const struct __dare__ModifyRules *, const char*, const char*);

#ifndef soap_read___dare__ModifyRules
#define soap_read___dare__ModifyRules(soap, data) ( soap_begin_recv(soap) || !soap_get___dare__ModifyRules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct __dare__ModifyRules * SOAP_FMAC4 soap_get___dare__ModifyRules(struct soap*, struct __dare__ModifyRules *, const char*, const char*);

#ifndef SOAP_TYPE___dare__GetRules
#define SOAP_TYPE___dare__GetRules (1758)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default___dare__GetRules(struct soap*, struct __dare__GetRules *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___dare__GetRules(struct soap*, const struct __dare__GetRules *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out___dare__GetRules(struct soap*, const char*, int, const struct __dare__GetRules *, const char*);
SOAP_FMAC3 struct __dare__GetRules * SOAP_FMAC4 soap_in___dare__GetRules(struct soap*, const char*, struct __dare__GetRules *, const char*);

#ifndef soap_write___dare__GetRules
#define soap_write___dare__GetRules(soap, data) ( soap_begin_send(soap) || (soap_serialize___dare__GetRules(soap, data), 0) || soap_put___dare__GetRules(soap, data, "-dare:GetRules", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put___dare__GetRules(struct soap*, const struct __dare__GetRules *, const char*, const char*);

#ifndef soap_read___dare__GetRules
#define soap_read___dare__GetRules(soap, data) ( soap_begin_recv(soap) || !soap_get___dare__GetRules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct __dare__GetRules * SOAP_FMAC4 soap_get___dare__GetRules(struct soap*, struct __dare__GetRules *, const char*, const char*);

#ifndef SOAP_TYPE___dare__DeleteRules
#define SOAP_TYPE___dare__DeleteRules (1754)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default___dare__DeleteRules(struct soap*, struct __dare__DeleteRules *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___dare__DeleteRules(struct soap*, const struct __dare__DeleteRules *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out___dare__DeleteRules(struct soap*, const char*, int, const struct __dare__DeleteRules *, const char*);
SOAP_FMAC3 struct __dare__DeleteRules * SOAP_FMAC4 soap_in___dare__DeleteRules(struct soap*, const char*, struct __dare__DeleteRules *, const char*);

#ifndef soap_write___dare__DeleteRules
#define soap_write___dare__DeleteRules(soap, data) ( soap_begin_send(soap) || (soap_serialize___dare__DeleteRules(soap, data), 0) || soap_put___dare__DeleteRules(soap, data, "-dare:DeleteRules", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put___dare__DeleteRules(struct soap*, const struct __dare__DeleteRules *, const char*, const char*);

#ifndef soap_read___dare__DeleteRules
#define soap_read___dare__DeleteRules(soap, data) ( soap_begin_recv(soap) || !soap_get___dare__DeleteRules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct __dare__DeleteRules * SOAP_FMAC4 soap_get___dare__DeleteRules(struct soap*, struct __dare__DeleteRules *, const char*, const char*);

#ifndef SOAP_TYPE___dare__CreateRules
#define SOAP_TYPE___dare__CreateRules (1750)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default___dare__CreateRules(struct soap*, struct __dare__CreateRules *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___dare__CreateRules(struct soap*, const struct __dare__CreateRules *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out___dare__CreateRules(struct soap*, const char*, int, const struct __dare__CreateRules *, const char*);
SOAP_FMAC3 struct __dare__CreateRules * SOAP_FMAC4 soap_in___dare__CreateRules(struct soap*, const char*, struct __dare__CreateRules *, const char*);

#ifndef soap_write___dare__CreateRules
#define soap_write___dare__CreateRules(soap, data) ( soap_begin_send(soap) || (soap_serialize___dare__CreateRules(soap, data), 0) || soap_put___dare__CreateRules(soap, data, "-dare:CreateRules", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put___dare__CreateRules(struct soap*, const struct __dare__CreateRules *, const char*, const char*);

#ifndef soap_read___dare__CreateRules
#define soap_read___dare__CreateRules(soap, data) ( soap_begin_recv(soap) || !soap_get___dare__CreateRules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct __dare__CreateRules * SOAP_FMAC4 soap_get___dare__CreateRules(struct soap*, struct __dare__CreateRules *, const char*, const char*);

#ifndef SOAP_TYPE___dare__GetSupportedRules
#define SOAP_TYPE___dare__GetSupportedRules (1746)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default___dare__GetSupportedRules(struct soap*, struct __dare__GetSupportedRules *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___dare__GetSupportedRules(struct soap*, const struct __dare__GetSupportedRules *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out___dare__GetSupportedRules(struct soap*, const char*, int, const struct __dare__GetSupportedRules *, const char*);
SOAP_FMAC3 struct __dare__GetSupportedRules * SOAP_FMAC4 soap_in___dare__GetSupportedRules(struct soap*, const char*, struct __dare__GetSupportedRules *, const char*);

#ifndef soap_write___dare__GetSupportedRules
#define soap_write___dare__GetSupportedRules(soap, data) ( soap_begin_send(soap) || (soap_serialize___dare__GetSupportedRules(soap, data), 0) || soap_put___dare__GetSupportedRules(soap, data, "-dare:GetSupportedRules", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put___dare__GetSupportedRules(struct soap*, const struct __dare__GetSupportedRules *, const char*, const char*);

#ifndef soap_read___dare__GetSupportedRules
#define soap_read___dare__GetSupportedRules(soap, data) ( soap_begin_recv(soap) || !soap_get___dare__GetSupportedRules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct __dare__GetSupportedRules * SOAP_FMAC4 soap_get___dare__GetSupportedRules(struct soap*, struct __dare__GetSupportedRules *, const char*, const char*);

#ifndef SOAP_TYPE___daae__ModifyAnalyticsModules
#define SOAP_TYPE___daae__ModifyAnalyticsModules (1742)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default___daae__ModifyAnalyticsModules(struct soap*, struct __daae__ModifyAnalyticsModules *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___daae__ModifyAnalyticsModules(struct soap*, const struct __daae__ModifyAnalyticsModules *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out___daae__ModifyAnalyticsModules(struct soap*, const char*, int, const struct __daae__ModifyAnalyticsModules *, const char*);
SOAP_FMAC3 struct __daae__ModifyAnalyticsModules * SOAP_FMAC4 soap_in___daae__ModifyAnalyticsModules(struct soap*, const char*, struct __daae__ModifyAnalyticsModules *, const char*);

#ifndef soap_write___daae__ModifyAnalyticsModules
#define soap_write___daae__ModifyAnalyticsModules(soap, data) ( soap_begin_send(soap) || (soap_serialize___daae__ModifyAnalyticsModules(soap, data), 0) || soap_put___daae__ModifyAnalyticsModules(soap, data, "-daae:ModifyAnalyticsModules", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put___daae__ModifyAnalyticsModules(struct soap*, const struct __daae__ModifyAnalyticsModules *, const char*, const char*);

#ifndef soap_read___daae__ModifyAnalyticsModules
#define soap_read___daae__ModifyAnalyticsModules(soap, data) ( soap_begin_recv(soap) || !soap_get___daae__ModifyAnalyticsModules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct __daae__ModifyAnalyticsModules * SOAP_FMAC4 soap_get___daae__ModifyAnalyticsModules(struct soap*, struct __daae__ModifyAnalyticsModules *, const char*, const char*);

#ifndef SOAP_TYPE___daae__GetAnalyticsModules
#define SOAP_TYPE___daae__GetAnalyticsModules (1738)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default___daae__GetAnalyticsModules(struct soap*, struct __daae__GetAnalyticsModules *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___daae__GetAnalyticsModules(struct soap*, const struct __daae__GetAnalyticsModules *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out___daae__GetAnalyticsModules(struct soap*, const char*, int, const struct __daae__GetAnalyticsModules *, const char*);
SOAP_FMAC3 struct __daae__GetAnalyticsModules * SOAP_FMAC4 soap_in___daae__GetAnalyticsModules(struct soap*, const char*, struct __daae__GetAnalyticsModules *, const char*);

#ifndef soap_write___daae__GetAnalyticsModules
#define soap_write___daae__GetAnalyticsModules(soap, data) ( soap_begin_send(soap) || (soap_serialize___daae__GetAnalyticsModules(soap, data), 0) || soap_put___daae__GetAnalyticsModules(soap, data, "-daae:GetAnalyticsModules", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put___daae__GetAnalyticsModules(struct soap*, const struct __daae__GetAnalyticsModules *, const char*, const char*);

#ifndef soap_read___daae__GetAnalyticsModules
#define soap_read___daae__GetAnalyticsModules(soap, data) ( soap_begin_recv(soap) || !soap_get___daae__GetAnalyticsModules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct __daae__GetAnalyticsModules * SOAP_FMAC4 soap_get___daae__GetAnalyticsModules(struct soap*, struct __daae__GetAnalyticsModules *, const char*, const char*);

#ifndef SOAP_TYPE___daae__DeleteAnalyticsModules
#define SOAP_TYPE___daae__DeleteAnalyticsModules (1734)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default___daae__DeleteAnalyticsModules(struct soap*, struct __daae__DeleteAnalyticsModules *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___daae__DeleteAnalyticsModules(struct soap*, const struct __daae__DeleteAnalyticsModules *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out___daae__DeleteAnalyticsModules(struct soap*, const char*, int, const struct __daae__DeleteAnalyticsModules *, const char*);
SOAP_FMAC3 struct __daae__DeleteAnalyticsModules * SOAP_FMAC4 soap_in___daae__DeleteAnalyticsModules(struct soap*, const char*, struct __daae__DeleteAnalyticsModules *, const char*);

#ifndef soap_write___daae__DeleteAnalyticsModules
#define soap_write___daae__DeleteAnalyticsModules(soap, data) ( soap_begin_send(soap) || (soap_serialize___daae__DeleteAnalyticsModules(soap, data), 0) || soap_put___daae__DeleteAnalyticsModules(soap, data, "-daae:DeleteAnalyticsModules", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put___daae__DeleteAnalyticsModules(struct soap*, const struct __daae__DeleteAnalyticsModules *, const char*, const char*);

#ifndef soap_read___daae__DeleteAnalyticsModules
#define soap_read___daae__DeleteAnalyticsModules(soap, data) ( soap_begin_recv(soap) || !soap_get___daae__DeleteAnalyticsModules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct __daae__DeleteAnalyticsModules * SOAP_FMAC4 soap_get___daae__DeleteAnalyticsModules(struct soap*, struct __daae__DeleteAnalyticsModules *, const char*, const char*);

#ifndef SOAP_TYPE___daae__CreateAnalyticsModules
#define SOAP_TYPE___daae__CreateAnalyticsModules (1730)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default___daae__CreateAnalyticsModules(struct soap*, struct __daae__CreateAnalyticsModules *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___daae__CreateAnalyticsModules(struct soap*, const struct __daae__CreateAnalyticsModules *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out___daae__CreateAnalyticsModules(struct soap*, const char*, int, const struct __daae__CreateAnalyticsModules *, const char*);
SOAP_FMAC3 struct __daae__CreateAnalyticsModules * SOAP_FMAC4 soap_in___daae__CreateAnalyticsModules(struct soap*, const char*, struct __daae__CreateAnalyticsModules *, const char*);

#ifndef soap_write___daae__CreateAnalyticsModules
#define soap_write___daae__CreateAnalyticsModules(soap, data) ( soap_begin_send(soap) || (soap_serialize___daae__CreateAnalyticsModules(soap, data), 0) || soap_put___daae__CreateAnalyticsModules(soap, data, "-daae:CreateAnalyticsModules", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put___daae__CreateAnalyticsModules(struct soap*, const struct __daae__CreateAnalyticsModules *, const char*, const char*);

#ifndef soap_read___daae__CreateAnalyticsModules
#define soap_read___daae__CreateAnalyticsModules(soap, data) ( soap_begin_recv(soap) || !soap_get___daae__CreateAnalyticsModules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct __daae__CreateAnalyticsModules * SOAP_FMAC4 soap_get___daae__CreateAnalyticsModules(struct soap*, struct __daae__CreateAnalyticsModules *, const char*, const char*);

#ifndef SOAP_TYPE___daae__GetSupportedAnalyticsModules
#define SOAP_TYPE___daae__GetSupportedAnalyticsModules (1726)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default___daae__GetSupportedAnalyticsModules(struct soap*, struct __daae__GetSupportedAnalyticsModules *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize___daae__GetSupportedAnalyticsModules(struct soap*, const struct __daae__GetSupportedAnalyticsModules *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out___daae__GetSupportedAnalyticsModules(struct soap*, const char*, int, const struct __daae__GetSupportedAnalyticsModules *, const char*);
SOAP_FMAC3 struct __daae__GetSupportedAnalyticsModules * SOAP_FMAC4 soap_in___daae__GetSupportedAnalyticsModules(struct soap*, const char*, struct __daae__GetSupportedAnalyticsModules *, const char*);

#ifndef soap_write___daae__GetSupportedAnalyticsModules
#define soap_write___daae__GetSupportedAnalyticsModules(soap, data) ( soap_begin_send(soap) || (soap_serialize___daae__GetSupportedAnalyticsModules(soap, data), 0) || soap_put___daae__GetSupportedAnalyticsModules(soap, data, "-daae:GetSupportedAnalyticsModules", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put___daae__GetSupportedAnalyticsModules(struct soap*, const struct __daae__GetSupportedAnalyticsModules *, const char*, const char*);

#ifndef soap_read___daae__GetSupportedAnalyticsModules
#define soap_read___daae__GetSupportedAnalyticsModules(soap, data) ( soap_begin_recv(soap) || !soap_get___daae__GetSupportedAnalyticsModules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct __daae__GetSupportedAnalyticsModules * SOAP_FMAC4 soap_get___daae__GetSupportedAnalyticsModules(struct soap*, struct __daae__GetSupportedAnalyticsModules *, const char*, const char*);

///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef SOAP_TYPE_PointerTott__SupportedRules
#define SOAP_TYPE_PointerTott__SupportedRules (234)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_PointerTott__SupportedRules(struct soap*, struct tt__SupportedRules *const*);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_PointerTott__SupportedRules(struct soap*, const char *, int, struct tt__SupportedRules *const*, const char *);
SOAP_FMAC3 struct tt__SupportedRules ** SOAP_FMAC4 soap_in_PointerTott__SupportedRules(struct soap*, const char*, struct tt__SupportedRules **, const char*);

#ifndef soap_write_PointerTott__SupportedRules
#define soap_write_PointerTott__SupportedRules(soap, data) ( soap_begin_send(soap) || (soap_serialize_PointerTott__SupportedRules(soap, data), 0) || soap_put_PointerTott__SupportedRules(soap, data, "tt:SupportedRules", NULL) || soap_end_send(soap) )
#endif

SOAP_FMAC3 int SOAP_FMAC4 soap_put_PointerTott__SupportedRules(struct soap*, struct tt__SupportedRules *const*, const char*, const char*);

#ifndef soap_read_PointerTott__SupportedRules
#define soap_read_PointerTott__SupportedRules(soap, data) ( soap_begin_recv(soap) || !soap_get_PointerTott__SupportedRules(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct tt__SupportedRules ** SOAP_FMAC4 soap_get_PointerTott__SupportedRules(struct soap*, struct tt__SupportedRules **, const char*, const char*);


#ifndef SOAP_TYPE_tt__SupportedAnalyticsModulesExtension
#define SOAP_TYPE_tt__SupportedAnalyticsModulesExtension (839)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__SupportedAnalyticsModulesExtension(struct soap*, struct tt__SupportedAnalyticsModulesExtension *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__SupportedAnalyticsModulesExtension(struct soap*, const struct tt__SupportedAnalyticsModulesExtension *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__SupportedAnalyticsModulesExtension(struct soap*, const char*, int, const struct tt__SupportedAnalyticsModulesExtension *, const char*);
SOAP_FMAC3 struct tt__SupportedAnalyticsModulesExtension * SOAP_FMAC4 soap_in_tt__SupportedAnalyticsModulesExtension(struct soap*, const char*, struct tt__SupportedAnalyticsModulesExtension *, const char*);

#ifndef soap_write_tt__SupportedAnalyticsModulesExtension
#define soap_write_tt__SupportedAnalyticsModulesExtension(soap, data) ( soap_begin_send(soap) || (soap_serialize_tt__SupportedAnalyticsModulesExtension(soap, data), 0) || soap_put_tt__SupportedAnalyticsModulesExtension(soap, data, "tt:SupportedAnalyticsModulesExtension", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__SupportedAnalyticsModulesExtension(struct soap*, const struct tt__SupportedAnalyticsModulesExtension *, const char*, const char*);

#ifndef soap_read_tt__SupportedAnalyticsModulesExtension
#define soap_read_tt__SupportedAnalyticsModulesExtension(soap, data) ( soap_begin_recv(soap) || !soap_get_tt__SupportedAnalyticsModulesExtension(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct tt__SupportedAnalyticsModulesExtension * SOAP_FMAC4 soap_get_tt__SupportedAnalyticsModulesExtension(struct soap*, struct tt__SupportedAnalyticsModulesExtension *, const char*, const char*);

#ifndef SOAP_TYPE_tt__SupportedRulesExtension
#define SOAP_TYPE_tt__SupportedRulesExtension (837)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__SupportedRulesExtension(struct soap*, struct tt__SupportedRulesExtension *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__SupportedRulesExtension(struct soap*, const struct tt__SupportedRulesExtension *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__SupportedRulesExtension(struct soap*, const char*, int, const struct tt__SupportedRulesExtension *, const char*);
SOAP_FMAC3 struct tt__SupportedRulesExtension * SOAP_FMAC4 soap_in_tt__SupportedRulesExtension(struct soap*, const char*, struct tt__SupportedRulesExtension *, const char*);

#ifndef soap_write_tt__SupportedRulesExtension
#define soap_write_tt__SupportedRulesExtension(soap, data) ( soap_begin_send(soap) || (soap_serialize_tt__SupportedRulesExtension(soap, data), 0) || soap_put_tt__SupportedRulesExtension(soap, data, "tt:SupportedRulesExtension", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__SupportedRulesExtension(struct soap*, const struct tt__SupportedRulesExtension *, const char*, const char*);

#ifndef soap_read_tt__SupportedRulesExtension
#define soap_read_tt__SupportedRulesExtension(soap, data) ( soap_begin_recv(soap) || !soap_get_tt__SupportedRulesExtension(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct tt__SupportedRulesExtension * SOAP_FMAC4 soap_get_tt__SupportedRulesExtension(struct soap*, struct tt__SupportedRulesExtension *, const char*, const char*);


#ifndef SOAP_TYPE_tt__ConfigDescriptionExtension
#define SOAP_TYPE_tt__ConfigDescriptionExtension (834)
#endif
SOAP_FMAC3 void SOAP_FMAC4 soap_default_tt__ConfigDescriptionExtension(struct soap*, struct tt__ConfigDescriptionExtension *);
SOAP_FMAC3 void SOAP_FMAC4 soap_serialize_tt__ConfigDescriptionExtension(struct soap*, const struct tt__ConfigDescriptionExtension *);
SOAP_FMAC3 int SOAP_FMAC4 soap_out_tt__ConfigDescriptionExtension(struct soap*, const char*, int, const struct tt__ConfigDescriptionExtension *, const char*);
SOAP_FMAC3 struct tt__ConfigDescriptionExtension * SOAP_FMAC4 soap_in_tt__ConfigDescriptionExtension(struct soap*, const char*, struct tt__ConfigDescriptionExtension *, const char*);

#ifndef soap_write_tt__ConfigDescriptionExtension
#define soap_write_tt__ConfigDescriptionExtension(soap, data) ( soap_begin_send(soap) || (soap_serialize_tt__ConfigDescriptionExtension(soap, data), 0) || soap_put_tt__ConfigDescriptionExtension(soap, data, "tt:ConfigDescriptionExtension", NULL) || soap_end_send(soap) )
#endif


SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__ConfigDescriptionExtension(struct soap*, const struct tt__ConfigDescriptionExtension *, const char*, const char*);

#ifndef soap_read_tt__ConfigDescriptionExtension
#define soap_read_tt__ConfigDescriptionExtension(soap, data) ( soap_begin_recv(soap) || !soap_get_tt__ConfigDescriptionExtension(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct tt__ConfigDescriptionExtension * SOAP_FMAC4 soap_get_tt__ConfigDescriptionExtension(struct soap*, struct tt__ConfigDescriptionExtension *, const char*, const char*);



SOAP_FMAC3 int SOAP_FMAC4 soap_put__tt__ConfigDescription_Messages(struct soap*, const struct _tt__ConfigDescription_Messages *, const char*, const char*);

#ifndef soap_read__tt__ConfigDescription_Messages
#define soap_read__tt__ConfigDescription_Messages(soap, data) ( soap_begin_recv(soap) || !soap_get__tt__ConfigDescription_Messages(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct _tt__ConfigDescription_Messages * SOAP_FMAC4 soap_get__tt__ConfigDescription_Messages(struct soap*, struct _tt__ConfigDescription_Messages *, const char*, const char*);



SOAP_FMAC3 int SOAP_FMAC4 soap_put_tt__ConfigDescription(struct soap*, const struct tt__ConfigDescription *, const char*, const char*);

#ifndef soap_read_tt__ConfigDescription
#define soap_read_tt__ConfigDescription(soap, data) ( soap_begin_recv(soap) || !soap_get_tt__ConfigDescription(soap, data, NULL, NULL) || soap_end_recv(soap) )
#endif

SOAP_FMAC3 struct tt__ConfigDescription * SOAP_FMAC4 soap_get_tt__ConfigDescription(struct soap*, struct tt__ConfigDescription *, const char*, const char*);



#ifdef __cplusplus
}
#endif

#endif