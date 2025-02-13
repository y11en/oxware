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
// dllmain.cpp -- Module interface for the utility module.
//

#include "precompiled.h"

// This
IBaseModule* g_thismodule_i = nullptr;

// Dependency loader
ILibraryLoader* g_libloader_i = nullptr;
IDeveloperConsole* g_devconsole_i = nullptr;

class CUtilityModule final : public IBaseModule
{
public:
	CUtilityModule() { g_thismodule_i = this; }
	~CUtilityModule() { g_thismodule_i = nullptr; }

	bool initialize(ModuleInitializationContext* context);
	void destroy();
	EUpdateStatus update();

	std::string get_module_name() { return MODULE_UTIL; }

	void kill_module()
	{
		m_update_status = UPDATE_APP_EXIT;
	}

private:
	EUpdateStatus m_update_status = UPDATE_CONTINUE;
};

CUtilityModule g_util_module;

//-------------------------------------------------------------------------------

bool CUtilityModule::initialize(ModuleInitializationContext* context)
{
	g_libloader_i = context->libloader_i;
	g_devconsole_i = context->devconsole_i;

	CConsole::the().initialize(EOutputModule::UTIL, g_devconsole_i);

	if (!g_importbank_i->initialize())
	{
		return false;
	}

	g_registry_i->initialize();

	return true;
}

void CUtilityModule::destroy()
{
	CConsole::the().destroy();

	// Dependency loader
	g_libloader_i = nullptr;
	g_devconsole_i = nullptr;

	// Local
	g_registry_i = nullptr;
	g_importbank_i = nullptr;
}

EUpdateStatus CUtilityModule::update()
{
	return m_update_status;
}

// Expose current module to the loader and initialize it
extern "C" DLLEXPORT bool ExposeModule(IBaseModule** module_interface, ModuleInitializationContext* context)
{
	*module_interface = &g_util_module;
	if (!(*module_interface)->initialize(context))
	{
		return false;
	}

	CConsole::the().info("Module loaded and initialized.");

#if 0 // benchmark
	float total_seconds = 0.0f;
	const int amount_to_find = 100000;
	for (int i = 0; i < amount_to_find; i++)
	{
		auto t1 = std::chrono::high_resolution_clock::now();
		auto var = g_variablemgr_i->get("random_var5s");
		auto t2 = std::chrono::high_resolution_clock::now();
		total_seconds += std::chrono::duration<float, std::ratio<1, 1>>(t2 - t1).count();
	}

	CConsole::the().info("Took {} seconds to find {} variables", total_seconds, amount_to_find);

	auto var = g_variablemgr_i->get("random_var5s");
	CConsole::the().info("{}: {} ({}) {} {}-{}", 
						 var->get_name(), var->get_description(), var->get_float(), 
						 var->has_bounds() ? "yes" : "no", 
						 var->get_min(), var->get_max());

	var->set_value(1000.0f);
	CConsole::the().info("{}: {} ({}) {} {}-{}", 
						 var->get_name(), var->get_description(), var->get_float(), 
						 var->has_bounds() ? "yes" : "no", 
						 var->get_min(), var->get_max());
#endif

	return true;
}

//-------------------------------------------------------------------------------

BOOL APIENTRY DllMain(HMODULE hModule,
					  DWORD  ul_reason_for_call,
					  LPVOID lpReserved)
{
	return TRUE;
}
