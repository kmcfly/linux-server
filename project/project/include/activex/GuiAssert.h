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

//用法与assert 相同 检测表达式false时输出,后接函数 func()
#define gui_assert(GUI_Expression,GUI_Func)\
	{\
		if(!(GUI_Expression)) \
		{ \
			printf("#:error! expression=\"%s\",file=%s,func=%s,line=%d\n",(#GUI_Expression),(__FILE__),(__FUNCTION__),__LINE__);\
			(GUI_Func);\
		}\
		assert(GUI_Expression);\
	}

//exp1,false时中断,always=true时一直输出信息
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

//条件exp 为true 时,使用后面函数(输出信息)
#define gui_trace(GUI_Expression1,GUI_Func) \
	{ \
		if(GUI_Expression1) \
		{ \
			printf("$:trace. file=%s,func=%s,line=%d\n",(__FILE__),(__FUNCTION__),__LINE__);\
			(GUI_Func);\
		}\
	}

//exp1,exp2都为true时,输出信息
#define gui_trace_and(GUI_Expression1,GUI_Expression2,GUI_Func) \
	{ \
		if((GUI_Expression1) && (GUI_Expression2)) \
		{ \
			printf("$:trace. file=%s,func=%s,line=%d\n",(__FILE__),(__FUNCTION__),__LINE__);\
			(GUI_Func); \
		}\
	}

//exp1 或 exp2为true时,输出信息
#define gui_trace_or(GUI_Expression1,GUI_Expression2,GUI_Func) \
	{ \
		if((GUI_Expression1) || (GUI_Expression2)) \
		{ \
			printf("$:trace. file=%s,func=%s,line=%d\n",(__FILE__),(__FUNCTION__),__LINE__);\
			(GUI_Func); \
		} \
	}

//条件exp 为true 时,使用后面函数(输出信息),不带路径no path
#define gui_trace_np(GUI_Expression1,GUI_Func) \
	{ \
		if( GUI_Expression1 ) \
		{ \
			(GUI_Func); \
		} \
	}

#endif  /* NDEBUG */

#endif
