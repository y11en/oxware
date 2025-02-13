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

#include "precompiled.h"

void CFeatureManager::initialize()
{
	CConsole::the().info("Initializing Feature Manager...");

	precache_features();
}

void CFeatureManager::shutdown()
{
	m_features.clear();
}

void CFeatureManager::add_new_feature(const BaseFeature& feature)
{
	m_features.push_back(feature);
	CConsole::the().info("'{}' assigned to variable '{}'", feature.get_name(), feature.get_variable()->get_name());
}

void CFeatureManager::precache_features()
{
	// esp
	add_new_feature(BaseFeature(&esp_enable, "ESP"));

	// fov
	add_new_feature(BaseFeature(&custom_fov, "Custom FOV"));

	// bypass constrain
	add_new_feature(BaseFeature(&bypass_constrain_renderdist_enable, "Render distance"));
	add_new_feature(BaseFeature(&bypass_constrain_noclip, "Re-enable Noclip"));
	add_new_feature(BaseFeature(&bypass_constrain_renderer_cvars, "Disable R_ForceCVars"));
	add_new_feature(BaseFeature(&bypass_constrain_sponly_cvars, "Disable SP-Only cvars"));

	// chams
	add_new_feature(BaseFeature(&mdlchams_enable, "Model chams"));

	// removals
	add_new_feature(BaseFeature(&remove_screenshake, "Remove screenshake"));
	add_new_feature(BaseFeature(&remove_hud_enable, "HUD removal"));

	// viewmodel offset
	add_new_feature(BaseFeature(&viewmodel_offset_enable, "Viewmodel offset"));

	// smoke visuals
	add_new_feature(BaseFeature(&smoke_visuals, "Smoke visuals"));
	
	CConsole::the().info("Precached all features.");
}
