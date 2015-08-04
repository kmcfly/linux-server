#ifndef _GUI_ASSERT_
#define _GUI_ASSERT_

#include <assert.h>

#undef gui_assert
#undef gui_assert_always
#undef gui_trace
#undef gui_trace_and
#undef gui_trace_or
#undef gui_trace_np

#ifdef  NDEBUG

#define gui_assert(GUI_Expression,GUI_Func) ((void)0)
#define gui_assert_always(GUI_Expression,GUI_Expression_Always,GUI_Func) ((void)0)

#define gui_trace(GUI_Expression1,GUI_Func) ((void)0)
#define gui_trace_and(GUI_Expression1,GUI_Expression2,GUI_Func) ((void)0)
#define gui_trace_or(GUI_Expression1,GUI_Expression2,GUI_Func) ((void)0)

#define gui_trace_np(GUI_Expression1,GUI_Func) ((void)0)

#else

//�÷���assert ��ͬ �����ʽfalseʱ���,��Ӻ��� func()
#define gui_assert(GUI_Expression,GUI_Func)\
	{\
		if(!(GUI_Expression)) \
		{ \
			printf("#:error! expression=\"%s\",file=%s,func=%s,line=%d\n",(#GUI_Expression),(__FILE__),(__FUNCTION__),__LINE__);\
			(GUI_Func);\
		}\
		assert(GUI_Expression);\
	}

//exp1,falseʱ�ж�,always=trueʱһֱ�����Ϣ
#define gui_assert_always(GUI_Expression,GUI_Expression_Always,GUI_Func) \
	{ \
		if((!(GUI_Expression)) || (GUI_Expression_Always)) \
		{ \
			if( !(GUI_Expression) ) \
			{ \
				printf("#:error! expression=\"%s\",file=%s,func=%s,line=%d\n",(#GUI_Expression),(__FILE__),(__FUNCTION__),__LINE__);\
			} \
			else \
			{ \
				printf("$:out. file=%s,func=%s,line=%d\n",(__FILE__),(__FUNCTION__),__LINE__);\
			} \
			(GUI_Func); \
		} \
		assert(GUI_Expression); \
	}

//����exp Ϊtrue ʱ,ʹ�ú��溯��(�����Ϣ)
#define gui_trace(GUI_Expression1,GUI_Func) \
	{ \
		if(GUI_Expression1) \
		{ \
			printf("$:trace. file=%s,func=%s,line=%d\n",(__FILE__),(__FUNCTION__),__LINE__);\
			(GUI_Func);\
		}\
	}

//exp1,exp2��Ϊtrueʱ,�����Ϣ
#define gui_trace_and(GUI_Expression1,GUI_Expression2,GUI_Func) \
	{ \
		if((GUI_Expression1) && (GUI_Expression2)) \
		{ \
			printf("$:trace. file=%s,func=%s,line=%d\n",(__FILE__),(__FUNCTION__),__LINE__);\
			(GUI_Func); \
		}\
	}

//exp1 �� exp2Ϊtrueʱ,�����Ϣ
#define gui_trace_or(GUI_Expression1,GUI_Expression2,GUI_Func) \
	{ \
		if((GUI_Expression1) || (GUI_Expression2)) \
		{ \
			printf("$:trace. file=%s,func=%s,line=%d\n",(__FILE__),(__FUNCTION__),__LINE__);\
			(GUI_Func); \
		} \
	}

//����exp Ϊtrue ʱ,ʹ�ú��溯��(�����Ϣ),����·��no path
#define gui_trace_np(GUI_Expression1,GUI_Func) \
	{ \
		if( GUI_Expression1 ) \
		{ \
			(GUI_Func); \
		} \
	}

#endif  /* NDEBUG */

#endif
