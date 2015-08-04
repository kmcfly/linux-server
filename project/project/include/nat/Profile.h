#pragma once

typedef unsigned long long uint64;
// 存放函数的性能分析结果, 每个FunctionProfile对象对应一个函数
struct FunctionProfile
{
	const char * func;    // 函数名
	uint64 callCount;     // 调用次数
	uint64 timeExclusive; // 独占运行时间
	FunctionProfile(const char * _func);
	void Print();         // 打印这个函数的分析结果
};
struct ProfilerStackFrame
{
	FunctionProfile * profile;   // 这一帧对应的分析数据
	ProfilerStackFrame * caller; // 这一帧的调用者(上一帧)
	uint64 timeBegin0;           // 包含Profiler本身耗时
	uint64 timeBegin1;           // 不包含Profiler本身耗时
	uint64 timeSub;              // 这一帧调用的所有子函数的耗时总和
	ProfilerStackFrame(FunctionProfile * _profile);
	~ProfilerStackFrame();
};
// 输出结果
void PrintProfileResult();

//#ifdef ENABLE_PROFILE
// 定义一个宏便于使用, 注意funcProfile是静态变量, funcFrame是局部变量
// #define FUNCTION_PROFILE /
// static FunctionProfile __funcProfile(__FUNCTION__);/
// ProfilerStackFrame __funcFrame(&__funcProfile);
// #else
// //关闭性能分析时的替代宏, 什么也不做
// #define FUNCTION_PROFILE ((void)0);
// #endif
