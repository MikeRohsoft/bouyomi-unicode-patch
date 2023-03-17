#include <string>
#include <windows.h>

// I'm not a C Programmer, but I saw alot of void* memes
void* AquesTalk_SynthePtr = nullptr;
void* AquesTalk_FreeWavePtr = nullptr;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call != DLL_PROCESS_ATTACH)
		return TRUE;

	// Get current DLL Path
	std::wstring libPathBuffer(MAX_PATH, L'\0');
	GetModuleFileName(hModule, &libPathBuffer[0], MAX_PATH);

	// Remove Filename from path and append new dll string to it
	std::wstring::size_type pos = libPathBuffer.find_last_of(L"\\/");
	std::wstring libFilePath = libPathBuffer.substr(0, pos) + L"\\_AquesTalk.dll";

	// Load DLL
	HMODULE mod = LoadLibraryW(libFilePath.c_str());
	if (mod == nullptr)
		return TRUE;

	// Load DLL Function Pointers
	AquesTalk_FreeWavePtr = GetProcAddress(mod, "AquesTalk_FreeWave");
	AquesTalk_SynthePtr = GetProcAddress(mod, "AquesTalk_Synthe");

	return TRUE;
}

// I blame Windows for that we have to do it in the first place, so we will at least abuse it to fix it
std::string reEncodeString(const std::string& str, unsigned int from, unsigned int to)
{
	if (from == to)
		return str;

	int wstringBufferLength = MultiByteToWideChar(from, 0, str.c_str(), -1, nullptr, 0);
	std::wstring wstringBuffer(wstringBufferLength, L'\0');
	MultiByteToWideChar(from, 0, str.c_str(), -1, &wstringBuffer[0], wstringBufferLength);

	int stringBufferLength = WideCharToMultiByte(to, 0, wstringBuffer.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::string stringBuffer(stringBufferLength, '\0');
	WideCharToMultiByte(to, 0, wstringBuffer.c_str(), -1, &stringBuffer[0], stringBufferLength, nullptr, nullptr);

	return stringBuffer;
}

// We only need fullfill the signature of Bouyomi-chan, it's C#, calling convention doesn't matters, this is the easiest way
extern "C" __declspec(dllexport) unsigned char* AquesTalk_Synthe(const std::string& koe, int speed, int* size)
{
	if (AquesTalk_SynthePtr == nullptr)
		return nullptr;

	const unsigned int SHIFTJIS = 932;
	// unsigned char * __stdcall AquesTalk_Synthe(const char *koe, int iSpeed, int *pSize);
	return static_cast<unsigned char*(__stdcall *)(const std::string&, int, int*)>
		(AquesTalk_SynthePtr)(reEncodeString(koe, GetACP(), SHIFTJIS), speed, size);
}

// We only need fullfill the signature of Bouyomi-chan, it's C#, calling convention doesn't matters, this is the easiest way
extern "C" __declspec(dllexport) void AquesTalk_FreeWave(unsigned char* wav)
{
	if (AquesTalk_FreeWavePtr != nullptr)
		// void __stdcall AquesTalk_FreeWave(unsigned char *wav);
		static_cast<void(__stdcall *)(unsigned char*)>(AquesTalk_FreeWavePtr)(wav);
}
