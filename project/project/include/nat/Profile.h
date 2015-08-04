#pragma once

typedef unsigned long long uint64;
// ��ź��������ܷ������, ÿ��FunctionProfile�����Ӧһ������
struct FunctionProfile
{
	const char * func;    // ������
	uint64 callCount;     // ���ô���
	uint64 timeExclusive; // ��ռ����ʱ��
	FunctionProfile(const char * _func);
	void Print();         // ��ӡ��������ķ������
};
struct ProfilerStackFrame
{
	FunctionProfile * profile;   // ��һ֡��Ӧ�ķ�������
	ProfilerStackFrame * caller; // ��һ֡�ĵ�����(��һ֡)
	uint64 timeBegin0;           // ����Profiler�����ʱ
	uint64 timeBegin1;           // ������Profiler�����ʱ
	uint64 timeSub;              // ��һ֡���õ������Ӻ����ĺ�ʱ�ܺ�
	ProfilerStackFrame(FunctionProfile * _profile);
	~ProfilerStackFrame();
};
// ������
void PrintProfileResult();

//#ifdef ENABLE_PROFILE
// ����һ�������ʹ��, ע��funcProfile�Ǿ�̬����, funcFrame�Ǿֲ�����
// #define FUNCTION_PROFILE /
// static FunctionProfile __funcProfile(__FUNCTION__);/
// ProfilerStackFrame __funcFrame(&__funcProfile);
// #else
// //�ر����ܷ���ʱ�������, ʲôҲ����
// #define FUNCTION_PROFILE ((void)0);
// #endif
