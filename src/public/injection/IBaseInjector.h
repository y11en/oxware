/*
*	OXWARE developed by oxiKKK
*	Copyright (c) 2023
* 
*	This program is licensed under the MIT license. By downloading, copying, 
*	installing or using this software you agree to this license.
*
*	License Agreement
*
*	Permission is hereby granted, free of charge, to any person obtaining a 
*	copy of this software and associated documentation files (the "Software"), 
*	to deal in the Software without restriction, including without limitation 
*	the rights to use, copy, modify, merge, publish, distribute, sublicense, 
*	and/or sell copies of the Software, and to permit persons to whom the 
*	Software is furnished to do so, subject to the following conditions:
*
*	The above copyright notice and this permission notice shall be included 
*	in all copies or substantial portions of the Software. 
*
*	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
*	OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
*	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
*	THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
*	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
*	FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
*	IN THE SOFTWARE.
*/

//
// IBaseInjector.h -- Defines an interface which must the base injector class use as well as individual injected dll objects.
//

#ifndef IBASEINJECTOR_H
#define IBASEINJECTOR_H
#pragma once

#include "interface/IFileSystem.h"

#include "PortableExecutableObject.h"
#include "VirtualMemoryManager.h"

#include "InjectedDllIPCInterface.h"

class IInjectableModuleObject;

class IBaseInjector
{
public:
	IBaseInjector(EInjectionTechnique t) :
		m_technique(t)
	{
	}

	virtual void destroy() = 0;

	virtual bool inject_to_target_process(const char* execuatable_name, const FilePath_t& dll_path) = 0;

	virtual void update() = 0;

	virtual IInjectableModuleObject* get_injected_dll(const FilePath_t& dll_path) = 0;

	inline auto get_injection_technique() const { return m_technique; };

protected:
	EInjectionTechnique m_technique = INJECT_UNINITIALIZED;
};

typedef FARPROC(WINAPI*pfnGetProcAddress_t)(HMODULE hModule, LPCSTR lpProcName);
typedef HMODULE(WINAPI*pfnLoadLibraryA_t)(LPCSTR lpLibFileName);
typedef HMODULE(WINAPI*pfnLoadLibraryExA_t)(LPCSTR lpLibFileName, HANDLE hFile, DWORD dwFlags);
typedef VOID(WINAPI*pfnOutputDebugStringA_t)(LPCSTR lpOutputString);
typedef int(__cdecl*pfn_stricmp_t)(const char *String1, const char *String2);
typedef void*(__cdecl*pfnmemset_t)(void *, int Val, size_t Size);
typedef void*(__cdecl*pfnmemcpy_t)(void *, const void *Src, size_t Size);

inline session_id_t generate_new_session_id()
{
	static session_id_t session_id = 0;
	if (session_id != 0)
	{
		return session_id;
	}

	session_id = rand() % std::numeric_limits<session_id_t>::max();
	return session_id;
}

// stuff that we provide to the injected module through exposed CommunicativeDllEntryPoint routine
struct alignas(sizeof(uintptr_t)) injector_information_package_t
{
	wchar_t							m_loader_path[MAX_PATH];

	// good for generating log files etc.
	session_id_t					m_current_session_id;

	// communication block that the injected dll use to communicate between injector and them
	IPCInterface_t*					m_ipc_block_ptr = nullptr;
};

// The data that we provide to the shellcode routine so it can operate correctly as
// well as establishing communication between the mapper&module, if there'll be any.
struct alignas(sizeof(uintptr_t)) manualmap_shellcode_execution_context_t
{
	//------------------------------------------------------------------------
	// stuff we need in order to map the module

	uint8_t*						module_base = nullptr;
	pfnGetProcAddress_t				pfnGetProcAddress = nullptr;
	pfnLoadLibraryA_t				pfnLoadLibraryA = nullptr;
	pfnOutputDebugStringA_t			pfnOutputDebugStringA = nullptr;
	pfn_stricmp_t					pfn_stricmp = nullptr;
	pfnmemset_t						pfnmemset = nullptr;
	pfnmemcpy_t						pfnmemcpy = nullptr;

	// yes, the shellcode also cannot have any strings
	char							debug_messages[5][64] = {};
	char							export_names[3][64] = {};

	//------------------------------------------------------------------------
	// stuff we receive while mapping the module.

	// exposed functions that are needed when injected in order to then locate 
	// module interfaces, initialize the modules properly, etc.
	ExposeModuleFn					pfnExposeModule = nullptr;
	GetInterfaceInstanceFn			pfnGetInterfaceInstance = nullptr;

	// memory snapshot of nt headers saved right before the Dll's DllMain is called.
	// then the real nt headers are erased completely.
	// first 4096 bytes is the DOS header + DOS stub + NT headers + image sections. then padded with zeros up to 0x1000
	uint8_t							m_nt_headers_snapshot[0x1000] = {};

	//------------------------------------------------------------------------
	// communication

	injector_information_package_t	m_information_package;
};

// the data we provide to the thread that starts executing in the target process.
// this particular data is for the loadlibrary injection method.
struct alignas(sizeof(uintptr_t)) loadlibrary_shellcode_execution_context_t
{
	//------------------------------------------------------------------------
	// stuff we need in order to inject the module

	char							m_injected_dll_name[MAX_PATH];
	pfnLoadLibraryExA_t				pfnLoadLibraryExA = nullptr;
	pfnOutputDebugStringA_t			pfnOutputDebugStringA = nullptr;
	pfn_stricmp_t					pfn_stricmp = nullptr;

	// yes, the shellcode also cannot have any strings
	char							debug_messages[4][64] = {};
	char							export_names[3][64] = {};

	//------------------------------------------------------------------------
	// stuff we receive while injecting the module.
	
	// exposed functions that are needed when injected in order to then locate 
	// module interfaces, initialize the modules properly, etc.
	ExposeModuleFn					pfnExposeModule = nullptr;
	GetInterfaceInstanceFn			pfnGetInterfaceInstance = nullptr;
	HMODULE							m_loaded_module = NULL; // the module we load using LoadLibraryExA. This is for unloading the dll later on.

	//------------------------------------------------------------------------
	// communication

	injector_information_package_t	m_information_package;
};

class CInjectorUtils
{
public:
	DECL_BASIC_CLASS(CInjectorUtils)
public:

	// returns -1 on failure
	uint32_t get_running_process_id(const char* execuatable_name);
	HANDLE open_process(const char* execuatable_name);
};

class IInjectableModuleObject : public InjectedDllCommunicationInterface
{
public:
	virtual bool start_injection() = 0;

	virtual void unload(bool tell_client_dll) = 0;

	virtual bool communicate_with_injected_dll(EClient2Injector& code) = 0;

	virtual uintptr_t get_base_address() const = 0;

	// functions exported by the dll
	virtual inline ExposeModuleFn get_ExposeModule_fn() const = 0;
	virtual inline GetInterfaceInstanceFn get_GetInterfaceInstance_fn() const = 0;

	// only when the module is communicative
	inline bool is_successfully_initialized() const { return m_injected_module_successfully_initialized; }
	virtual void on_successfull_initialize() = 0;

	inline bool is_unloaded() const { return m_unloaded; }
	inline bool is_injecting_to_current_process() const { return m_inject_to_current_process; }

	inline bool is_successfully_injected() const { return m_successfully_injected; }

protected:
	FilePath_t m_dll_filepath;
	std::string m_exe_name;

	HANDLE m_target_process_handle = NULL;
	void close_process_handle();

	CPortableExecutableObject m_dll_to_be_injected;

	bool m_inject_to_current_process = false;

protected:
	virtual bool allocate_memory() = 0;
	virtual void deallocate_all_memory() = 0;

protected:
	bool m_unloaded = false;

	// time since we've injected the cheat. this is used for communication with it.
	inline static float k_load_and_init_threshold_seconds = 300; // also count the load time
	inline float calc_current_load_and_init_threshold_seconds()
	{
		return std::chrono::duration<float, std::ratio<1, 1>>(std::chrono::high_resolution_clock::now() - m_start_time).count();
	}

	std::chrono::high_resolution_clock::time_point m_start_time; // timestamp at the beginnin of the injecting
	std::chrono::high_resolution_clock::time_point m_initialization_time; // timestamp of when the module has been sucessfully initialized

	bool m_successfully_injected = false;
	bool m_injected_module_successfully_initialized = false;

	// the injector requests the dll to be unloaded. Is set to true after it is unloaded.
	bool m_requested_injected_dll_unload = false;
#ifdef _DEBUG
	inline static float k_seconds_for_the_client_dll_to_unload = 9999;
#else
	inline static float k_seconds_for_the_client_dll_to_unload = 20; // give him # amount of seconds to destroy properly and then respond to us so we can unload him
#endif
	// after this elapses, we unload it anyways.
	std::chrono::high_resolution_clock::time_point m_unload_request_time; // timestamp when we started to wait for the client dll to unload
	inline float calc_time_elapsed_since_unload_request_seconds()
	{
		return std::chrono::duration<float, std::ratio<1, 1>>(std::chrono::high_resolution_clock::now() - m_unload_request_time).count();
	}
};

#endif // IBASEINJECTIONMETHOD_H