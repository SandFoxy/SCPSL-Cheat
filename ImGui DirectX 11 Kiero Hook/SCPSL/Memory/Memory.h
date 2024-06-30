#pragma once
#include "../includes.h"

class Internal {
private:
	bool isEqual(char* names, const wchar_t* API);
public:
	MODULEENTRY32W Base;
	uintptr_t BaseAddress;
	MODULEENTRY32W GetModule(const wchar_t* modName);
	uintptr_t GetProcAddress(const wchar_t* wAPIName);
	uintptr_t PatternScan(const char* signature);
	template<typename ret, typename... parameters>
	static ret Call(uintptr_t address, parameters... params)
	{
		typedef ret(*func_t)(parameters...);
		auto func = reinterpret_cast<func_t>(address);
		return func(params...);
	}
	template<typename ret, typename... parameters>
	static ret CallPointer(void* methodPointer, parameters... params)
	{
		//void* methodPointer = IL2CPP::Class::Utils::GetMethodPointer(il2CppClass, methodName);

		typedef ret(*func_t)(parameters...);
		auto func = reinterpret_cast<func_t>(methodPointer);

		return func(params...);
	}
	void init();
};

extern Internal _internal;