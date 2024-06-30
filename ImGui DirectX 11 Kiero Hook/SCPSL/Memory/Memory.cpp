#include "Memory.h"

Internal _internal;

uintptr_t Internal::PatternScan(const char* signature)
{
	static auto pattern_to_byte = [](const char* pattern)
	{
		auto bytes = std::vector<char>{};
		auto start = const_cast<char*>(pattern);
		auto end = const_cast<char*>(pattern) + strlen(pattern);

		for (auto current = start; current < end; ++current)
		{
			if (*current == '?')
			{
				++current;
				if (*current == '?')
					++current;
				bytes.push_back('\?');
			}
			else
			{
				bytes.push_back(strtoul(current, &current, 16));
			}
		}
		return bytes;
	};

	uintptr_t base = (uintptr_t)this->Base.modBaseAddr;
	uintptr_t sizeOfImage = (uintptr_t)this->Base.modBaseSize;
	auto patternBytes = pattern_to_byte(signature);

	uintptr_t patternLength = patternBytes.size();
	auto data = patternBytes.data();

	for (uintptr_t i = 0; i < sizeOfImage - patternLength; i++)
	{
		bool found = true;
		for (uintptr_t j = 0; j < patternLength; j++)
		{
			char a = '\?';
			char b = *(char*)(base + i + j);
			found &= data[j] == a || data[j] == b;
		}
		if (found)
		{
			return base + i;
		}
	}
	return 0;
}

uintptr_t Internal::GetProcAddress(const wchar_t* wAPIName)
{
	unsigned char* lpBase = reinterpret_cast<unsigned char*>(this->Base.hModule);
	IMAGE_DOS_HEADER* idhDosHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(lpBase);
	if (idhDosHeader->e_magic == 0x5A4D)
	{
		IMAGE_NT_HEADERS64* inhNtHeader = reinterpret_cast<IMAGE_NT_HEADERS64*>(lpBase + idhDosHeader->e_lfanew);
		if (inhNtHeader->Signature == 0x4550)
		{
			IMAGE_EXPORT_DIRECTORY* iedExportDirectory = reinterpret_cast<IMAGE_EXPORT_DIRECTORY*>(lpBase + inhNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
			for (register unsigned int uiIter = 0; uiIter < iedExportDirectory->NumberOfNames; ++uiIter)
			{
				char* szNames = reinterpret_cast<char*>(lpBase + reinterpret_cast<unsigned long*>(lpBase + iedExportDirectory->AddressOfNames)[uiIter]);
				if (this->isEqual(szNames, wAPIName))
				{
					unsigned short usOrdinal = reinterpret_cast<unsigned short*>(lpBase + iedExportDirectory->AddressOfNameOrdinals)[uiIter];
					return reinterpret_cast<uintptr_t>(lpBase + reinterpret_cast<unsigned long*>(lpBase + iedExportDirectory->AddressOfFunctions)[usOrdinal]);
				}
			}
		}
	}
	return 0;
}

bool Internal::isEqual(char* names, const wchar_t* API)
{
	std::wstring wc(API);
	std::string tmp(names);
	std::wstring wc2(tmp.begin(), tmp.end());

	return wc.compare(wc2) == 0;
}

MODULEENTRY32W Internal::GetModule(const wchar_t* modName)
{
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, GetCurrentProcessId());
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32W modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32FirstW(hSnap, &modEntry))
		{
			do
			{
				if (!_wcsicmp(modEntry.szModule, modName))
				{
					return modEntry;
				}
			} while (Module32NextW(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return MODULEENTRY32W();
}

void Internal::init()
{
	this->Base = GetModule(L"GameAssembly.dll");
	this->BaseAddress = (uintptr_t)this->Base.modBaseAddr;
}