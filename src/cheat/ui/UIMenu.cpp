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

std::array<TabCallbackFn, UIMENU_Max> CUIMenu::s_active_tab_callback_translation =
{
	{
		&CUIMenu::tab_blank, 				// UIMENU_Blank
		&CUIMenu::tab_ragebot, 				// UIMENU_RageBot
		&CUIMenu::tab_legitbot,				// UIMENU_LegitBot
		&CUIMenu::tab_antibot,				// UIMENU_AntiBot
		&CUIMenu::tab_waybot,				// UIMENU_WayBot
		&CUIMenu::tab_viewmodel,			// UIMENU_Viewmodel
		&CUIMenu::tab_world,				// UIMENU_World
		&CUIMenu::tab_render,				// UIMENU_Render
		&CUIMenu::tab_visuals4,				// UIMENU_Visuals4
		&CUIMenu::tab_exploits,				// UIMENU_Exploits
		&CUIMenu::tab_miscellaneous2,		// UIMENU_Miscellaneous2
		&CUIMenu::tab_miscellaneous3,		// UIMENU_Miscellaneous3
		&CUIMenu::tab_config,				// UIMENU_Config
	}
};

//---------------------------------------------------------------------------------------------------

void MenuTabItem::render(Vector2D& offset, Vector2D& relative_offset, EMenuTabId& active_tab_id)
{
	g_gui_widgets_i->set_cursor_pos(relative_offset);

	auto button_font = g_gui_fontmgr_i->get_imgui_font("segoeui", FONT_MEDIUM, FONTDEC_Bold);
	g_gui_widgets_i->push_font(button_font);

	// text color
	g_gui_widgets_i->push_color(ImGuiCol_Text, g_gui_thememgr_i->get_current_theme()->get_color<GUICLR_TextDark>());

	bool selected = (active_tab_id == m_tab_id);
	if (g_gui_widgets_i->add_toggle_button(m_label.c_str(), { CMenuStyle::k_tab_select_size.x - 20.0f, 0.0f }, selected))
	{
		active_tab_id = m_tab_id; // make current one selected
	}

	if (g_gui_widgets_i->is_last_widget_hovered())
	{
		CMenuWidgetDescription::the().set_desc(m_desc.c_str());
	}

	g_gui_widgets_i->pop_color(1);

	g_gui_widgets_i->pop_font();

	relative_offset.y += button_font->FontSize + 4.0f;
	offset.y += button_font->FontSize + 4.0f;
}

//---------------------------------------------------------------------------------------------------

void MenuTabSection::render(Vector2D& offset, Vector2D& relative_offset, const Vector2D& child_size, EMenuTabId& active_tab_id)
{
	render_current_label(offset, relative_offset, child_size);

	for (auto& tab : m_menu_tabs)
	{
		tab.render(offset, relative_offset, active_tab_id);
	}
}

void MenuTabSection::render_current_label(Vector2D& offset, Vector2D& relative_offset, const Vector2D& child_size)
{
	auto section_label_font = g_gui_fontmgr_i->get_font("segoeui", FONT_MEDIUM, FONTDEC_Regular);

	// section title
	g_gui_window_rendering_i->render_text(g_gui_window_rendering_i->get_current_drawlist(), 
										  section_label_font,
										  { offset.x, offset.y + 5.0f },
										  g_gui_thememgr_i->get_current_theme()->get_color<GUICLR_TextLight>(),
										  m_label);

	relative_offset.y += section_label_font->get_size_px();
	offset.y += section_label_font->get_size_px();

	// separator underneath
	g_gui_window_rendering_i->render_line(g_gui_window_rendering_i->get_current_drawlist(), 
										  { offset.x, offset.y + 4.0f },
										  { offset.x + child_size.x - 10.f * 2.f, offset.y + 5.f },
										  g_gui_thememgr_i->get_current_theme()->get_color<GUICLR_Separator>());

	relative_offset.y += 8.f;
	offset.y += 8.f;
}

//---------------------------------------------------------------------------------------------------

void CUIMenu::on_initialize()
{
	//m_tabsec_AIHelpers.set_label("AI Helpers");
	//m_tabsec_AIHelpers.add_tab({ UIMENU_RageBot, &CUIMenu::tab_ragebot, "Ragebot", "TODO item description" });
	//m_tabsec_AIHelpers.add_tab({ UIMENU_LegitBot, &CUIMenu::tab_legitbot, "Legitbot", "TODO item description" });
	//m_tabsec_AIHelpers.add_tab({ UIMENU_AntiBot, &CUIMenu::tab_antibot, "Antibot", "TODO item description" });
	//m_tabsec_AIHelpers.add_tab({ UIMENU_WayBot, &CUIMenu::tab_waybot, "Waybot", "TODO item description" });
	//m_tab_sections.push_back(&m_tabsec_AIHelpers);

	m_tabsec_Visuals.set_label("Visuals");
	m_tabsec_Visuals.add_tab({ UIMENU_World, &CUIMenu::tab_world, "World", "TODO item description" });
	m_tabsec_Visuals.add_tab({ UIMENU_Render, &CUIMenu::tab_render, "Render", "TODO item description" });
	//m_tabsec_Visuals.add_tab({ UIMENU_Visuals4, &CUIMenu::tab_visuals4, "Visuals4", "TODO item description" });
	m_tab_sections.push_back(&m_tabsec_Visuals);

	m_tabsec_Miscellaneous.set_label("Miscellaneous");
	m_tabsec_Miscellaneous.add_tab({ UIMENU_Exploits, &CUIMenu::tab_exploits, "Exploits", "TODO item description" });
	//m_tabsec_Miscellaneous.add_tab({ UIMENU_Miscellaneous2, &CUIMenu::tab_miscellaneous2, "Miscellaneous2", "TODO item description" });
	//m_tabsec_Miscellaneous.add_tab({ UIMENU_Miscellaneous3, &CUIMenu::tab_miscellaneous3, "Miscellaneous3", "TODO item description" });
	m_tab_sections.push_back(&m_tabsec_Miscellaneous);

	m_tabsec_Configuration.set_label("Config");
	m_tabsec_Configuration.add_tab({ UIMENU_Config, &CUIMenu::tab_config, "Config", "TODO item description" });
	m_tab_sections.push_back(&m_tabsec_Configuration);
}

void CUIMenu::on_render()
{
	g_gui_widgets_i->set_next_window_pos({ 100, 100 }, ImGuiCond_Once);
	g_gui_widgets_i->set_next_window_size(CMenuStyle::k_menu_rect_size, ImGuiCond_Once);
	g_gui_widgets_i->set_next_window_rounding(CMenuStyle::k_rounding_factor, ImDrawFlags_RoundCornersTopRight | ImDrawFlags_RoundCornersBottomLeft);

	static constexpr auto window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoNav;

	auto segoeui_extra = g_gui_fontmgr_i->get_font("segoeui", FONT_EXTRA, FONTDEC_Bold);
	auto segoeui_small = g_gui_fontmgr_i->get_font("segoeui", FONT_SMALL, FONTDEC_Bold);

	g_gui_widgets_i->push_stylevar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });

	CMenuWidgetDescription::the().clear_desc();

	g_gui_widgets_i->create_new_window(
		"menu", window_flags, [&]()
		{
			auto window_pos = g_gui_widgets_i->get_current_window_pos();
			auto window_size = g_gui_widgets_i->get_current_window_size();

			g_gui_widgets_i->set_next_window_rounding(CMenuStyle::k_rounding_factor, ImDrawFlags_RoundCornersBottomLeft);
			g_gui_widgets_i->add_child(
				"menu_left",
				CMenuStyle::k_tab_select_size,
				false,
				ImGuiWindowFlags_None,
				[&]()
				{
					auto child_pos = g_gui_widgets_i->get_current_window_pos();
					auto child_size = g_gui_widgets_i->get_current_window_size();

					auto label_size = g_gui_fontmgr_i->calc_font_text_size(segoeui_extra, "OXWARE");

					g_gui_window_rendering_i->render_text(g_gui_window_rendering_i->get_current_drawlist(), 
														  segoeui_extra,
														  { child_pos.x + child_size.x / 2 - label_size.x / 2, window_pos.y + 10 },
														  g_gui_thememgr_i->get_current_theme()->get_color<GUICLR_TextDark>(),
														  "OXWARE");

					m_sectab_relative_active_offset = { 10.0f, 50.0f };
					m_sectab_active_offs = child_pos + m_sectab_relative_active_offset;

					for (auto sec : m_tab_sections)
					{
						sec->render(m_sectab_active_offs, m_sectab_relative_active_offset,
									child_size, m_active_tab_id);
					}
				}
			);

			// menu and tab select separator
			g_gui_window_rendering_i->render_line(g_gui_window_rendering_i->get_current_drawlist(), 
												  { window_pos.x + CMenuStyle::k_tab_select_size.x, window_pos.y - 0.5f },
												  { window_pos.x + CMenuStyle::k_tab_select_size.x, window_pos.y + CMenuStyle::k_tab_select_size.y },
												  g_gui_thememgr_i->get_current_theme()->get_color<GUICLR_Separator>());


			// top side menu separator
			g_gui_window_rendering_i->render_line(g_gui_window_rendering_i->get_current_drawlist(), 
												  { window_pos.x + CMenuStyle::k_tab_select_size.x, window_pos.y + CMenuStyle::k_top_region_size_h },
												  { window_pos.x + CMenuStyle::k_menu_rect_size.x, window_pos.y + CMenuStyle::k_top_region_size_h },
												  g_gui_thememgr_i->get_current_theme()->get_color<GUICLR_Separator>());

			//
			// render topside contents
			//
			auto topside_font = g_gui_fontmgr_i->get_font("segoeui", FONT_SMALL, FONTDEC_Regular);

			auto version_label = std::format("Version: {}", OXVER_STRING);
			g_gui_window_rendering_i->render_text(g_gui_window_rendering_i->get_current_drawlist(), 
												  topside_font,
												  { window_pos.x + CMenuStyle::k_tab_select_size.x + 5.0f, window_pos.y + 0.0f + 3.0f },
												  g_gui_thememgr_i->get_current_theme()->get_color<GUICLR_TextDark>(), 
												  version_label);

			auto build_label = std::format("Build: {}", OX_BUILD);
			g_gui_window_rendering_i->render_text(g_gui_window_rendering_i->get_current_drawlist(), 
												  topside_font,
												  { window_pos.x + CMenuStyle::k_tab_select_size.x + 5.0f, window_pos.y + 15.0f + 3.0f },
												  g_gui_thememgr_i->get_current_theme()->get_color<GUICLR_TextDark>(), 
												  build_label);

			auto compiletm_label = std::format("Compiled at: {}", OX_COMPILE_TIMESTAMP);
			g_gui_window_rendering_i->render_text(g_gui_window_rendering_i->get_current_drawlist(), 
												  topside_font,
												  { window_pos.x + CMenuStyle::k_tab_select_size.x + 5.0f, window_pos.y + 30.0f + 3.0f },
												  g_gui_thememgr_i->get_current_theme()->get_color<GUICLR_TextDark>(), 
												  compiletm_label);

			//
			// now render contents of the menu.
			//

			g_gui_widgets_i->set_cursor_pos({
				CMenuStyle::k_tab_select_size.x + CMenuStyle::k_menu_contents_padding,
				CMenuStyle::k_top_region_size_h + CMenuStyle::k_menu_contents_padding });

			g_gui_widgets_i->add_child(
				"menu_contents",
				{
					window_size.x - CMenuStyle::k_tab_select_size.x - (CMenuStyle::k_menu_contents_padding * 2.0f),
					window_size.y - CMenuStyle::k_top_region_size_h - (CMenuStyle::k_menu_contents_padding * 1.0f + CMenuStyle::k_menu_contents_padding_bottom) - CMenuStyle::k_bottom_reserved_rect_h,
				},
				false,
				ImGuiWindowFlags_NoBackground,
				[&]()
				{
					auto tab_fn = s_active_tab_callback_translation[m_active_tab_id];
					(this->*tab_fn)();
				});

			// bottom separator
			g_gui_window_rendering_i->render_line(g_gui_window_rendering_i->get_current_drawlist(), 
												  { window_pos.x + CMenuStyle::k_tab_select_size.x + 1.0f, window_pos.y + (window_size.y - CMenuStyle::k_bottom_reserved_rect_h - 1.0f) },
												  { window_pos.x + window_size.x,  window_pos.y + (window_size.y - CMenuStyle::k_bottom_reserved_rect_h - 1.0f) },
												  g_gui_thememgr_i->get_current_theme()->get_color<GUICLR_Separator>());

			// bottom background
			g_gui_window_rendering_i->render_box(g_gui_window_rendering_i->get_current_drawlist(), 
												 { window_pos.x + CMenuStyle::k_tab_select_size.x + 1.0f, window_pos.y + (window_size.y - CMenuStyle::k_bottom_reserved_rect_h) },
												  { window_pos.x + window_size.x, window_pos.y + (window_size.y - 1.0f) },
												  g_gui_thememgr_i->get_current_theme()->get_color<GUICLR_ChildBackground>());

			// bottom right timestamp label
			auto timestamp_label = CStringTools::the().format_date(std::chrono::system_clock::now());
			auto timestamp_label_size = g_gui_fontmgr_i->calc_font_text_size(segoeui_small, timestamp_label.c_str());
			g_gui_window_rendering_i->render_text(g_gui_window_rendering_i->get_current_drawlist(), 
												  segoeui_small,
												  { 
													  window_pos.x + (window_size.x - CMenuStyle::k_bottom_right_timestamp_rightside_padding - timestamp_label_size.x), 
													  window_pos.y + (window_size.y - CMenuStyle::k_bottom_right_timestamp_bottomside_padding - timestamp_label_size.y)
												  },
												  g_gui_thememgr_i->get_current_theme()->get_color<GUICLR_TextDark>(),
												  timestamp_label);

			// bottom-left description
			auto desc_label = CMenuWidgetDescription::the().get_desc();
			auto desc_label_size = g_gui_fontmgr_i->calc_font_text_size(segoeui_small, desc_label);
			g_gui_window_rendering_i->render_text(g_gui_window_rendering_i->get_current_drawlist(),
												  segoeui_small,
												  {
													  window_pos.x + CMenuStyle::k_tab_select_size.x + CMenuStyle::k_bottom_left_desc_padding.x,
													  window_pos.y + (window_size.y - CMenuStyle::k_bottom_left_desc_padding.y - desc_label_size.y)
												  },
												  g_gui_thememgr_i->get_current_theme()->get_color<GUICLR_TextDark>(),
												 desc_label);
			
			// Unload button
			g_gui_widgets_i->set_cursor_pos(CMenuStyle::k_about_button_pos);

			if (g_gui_widgets_i->add_button("manually unload", CMenuStyle::k_about_button_size, false, BUTTONFLAG_CenterLabel))
			{
				// end next frame.
				CoXWARE::the().end_cheat_execution();
			}
		});

	auto screen = g_imgui_platform_layer_i->get_screen_size();

	auto font = g_gui_fontmgr_i->get_font("segoeui", FONT_REGULAR, FONTDEC_Regular);

	const char* label = "www.github.com/oxiKKK/oxware";
	auto label_size = g_gui_fontmgr_i->calc_font_text_size(font, label);

	g_gui_widgets_i->set_next_window_pos({ screen.x - label_size.x, screen.y - label_size.y }, ImGuiCond_Always);

	static constexpr auto window_flags1 = 
		ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings | 
		ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBackground |
		ImGuiWindowFlags_NoMove;
	g_gui_widgets_i->create_new_window(
		"github_link", window_flags1, 
		[&]()
		{
			if (g_gui_widgets_i->add_hypertext_link(label))
			{
				CGenericUtil::the().open_link_inside_browser(label);
			}

			// render github repository link
			//g_gui_window_rendering_i->render_text(
			//	g_gui_window_rendering_i->get_foreground_drawlist(),
			//	font,
			//	,
			//	CColor(255, 255, 255, 255), label);
		});

	g_gui_widgets_i->pop_stylevar();
}

void CUIMenu::on_destroy()
{
	CConsole::the().info(__FUNCTION__ "()");
}

//---------------------------------------------------------------------------------------------------

void CUIMenu::tab_blank()
{
	g_gui_widgets_i->add_window_centered_text_disabled("Start by looking through the tabs on the left!");
}

void CUIMenu::tab_ragebot()
{
}

void CUIMenu::tab_legitbot()
{
	g_gui_widgets_i->add_text(__FUNCTION__ "()");
}

void CUIMenu::tab_antibot()
{
	g_gui_widgets_i->add_text(__FUNCTION__ "()");
}

void CUIMenu::tab_waybot()
{
	g_gui_widgets_i->add_text(__FUNCTION__ "()");
}

void CUIMenu::tab_viewmodel()
{
	g_gui_widgets_i->add_text(__FUNCTION__ "()");
}

void CUIMenu::tab_world()
{
	g_gui_widgets_i->begin_columns(__FUNCTION__, 2);
	
	add_menu_child(
		"Removals", CMenuStyle::calc_child_size(210), false, ImGuiWindowFlags_AlwaysUseWindowPadding,
		[]()
		{
			CUIMenuWidgets::the().add_checkbox("Remove screenshake", &remove_screenshake);

			g_gui_widgets_i->add_padding({ 0, 5.0f });
			g_gui_widgets_i->add_separtor_with_text("Remove HUD");

			CUIMenuWidgets::the().add_checkbox("Enable", &remove_hud_enable);
			g_gui_widgets_i->add_padding({ 0, 5.0f });

			g_gui_widgets_i->begin_columns("removals_hud", 2);
			CUIMenuWidgets::the().add_checkbox("Weapons", &remove_hud_weapons);
			CUIMenuWidgets::the().add_checkbox("Crosshair", &remove_hud_crosshair);
			CUIMenuWidgets::the().add_checkbox("Flashlight", &remove_hud_flashlight);

			g_gui_widgets_i->goto_next_column();

			CUIMenuWidgets::the().add_checkbox("Health", &remove_hud_health);
			CUIMenuWidgets::the().add_checkbox("Timer", &remove_hud_timer);
			CUIMenuWidgets::the().add_checkbox("Money", &remove_hud_money);

			g_gui_widgets_i->end_columns(1);
		});

	add_menu_child(
		"Smoke visuals", CMenuStyle::calc_child_size(180), false, ImGuiWindowFlags_AlwaysUseWindowPadding,
		[]()
		{
			CUIMenuWidgets::the().add_checkbox("Enable", &smoke_visuals);

			CUIMenuWidgets::the().add_color_edit("Color", &smoke_color);
			CUIMenuWidgets::the().add_checkbox("Rainbow smoke", &smoke_rainbow);

			CUIMenuWidgets::the().add_slider("Opacity", "%0.0f %%", &smoke_opacity);

			CUIMenuWidgets::the().add_description_text("Everything changed here will take effect only on new smoke creation.");
		});
	
	g_gui_widgets_i->goto_next_column();
	
	add_menu_child(
		"Viewmodel offset", CMenuStyle::calc_child_size(100), false, ImGuiWindowFlags_AlwaysUseWindowPadding,
		[]()
		{
			CUIMenuWidgets::the().add_checkbox("Enable", &viewmodel_offset_enable);

			CUIMenuWidgets::the().add_slider("Amount", "%0.1f", &viewmodel_offset_value);
		});

	g_gui_widgets_i->end_columns(1);
}

void CUIMenu::tab_render()
{
	g_gui_widgets_i->begin_columns(__FUNCTION__, 2);
	
	add_menu_child(
		"Field of view", CMenuStyle::calc_child_size(100), false, ImGuiWindowFlags_AlwaysUseWindowPadding,
		[]()
		{
			CUIMenuWidgets::the().add_checkbox("Enable", &custom_fov);
			CUIMenuWidgets::the().add_slider("FOV scale", "%0.01fx", &custom_fov_value);
		});

	add_menu_child(
		"Flashbang fade", CMenuStyle::calc_child_size(100), false, ImGuiWindowFlags_AlwaysUseWindowPadding,
		[]()
		{
			CUIMenuWidgets::the().add_checkbox_with_color("Enable", &flashfademod_enable, &flashfademod_color);

			CUIMenuWidgets::the().add_slider("Fade factor", "%0.1f", &flashfademod_fade_factor);
		});
	
	add_menu_child(
		"ESP", CMenuStyle::calc_child_size(340), false, ImGuiWindowFlags_AlwaysUseWindowPadding,
		[]()
		{
			CUIMenuWidgets::the().add_checkbox("Enable", &esp_enable);

			g_gui_widgets_i->add_padding({ 0, 5.0f });
			g_gui_widgets_i->add_separtor_with_text("Player");
			CUIMenuWidgets::the().add_checkbox("Enable ##player", &esp_player_enable);
			CUIMenuWidgets::the().add_checkbox("Show name", &esp_player_name);

			g_gui_widgets_i->add_padding({ 0, 5.0f });
			g_gui_widgets_i->add_separtor_with_text("Entity");
			CUIMenuWidgets::the().add_checkbox("Enable ##ents", &esp_entity_enable);

			g_gui_widgets_i->add_padding({ 0, 5.0f });
			g_gui_widgets_i->add_separtor_with_text("Sound");
			CUIMenuWidgets::the().add_checkbox("Enable ##sound", &esp_sound_enable);
			CUIMenuWidgets::the().add_slider("Display life", "%0.1f seconds", &esp_sound_interval);
			CUIMenuWidgets::the().add_checkbox("Filter local", &esp_sound_filter_local);
			CUIMenuWidgets::the().add_checkbox("Only enemy team", &esp_only_enemy_team);
		});

	g_gui_widgets_i->goto_next_column();

	add_menu_child(
		"Model chams", CMenuStyle::calc_child_size(390), false, ImGuiWindowFlags_AlwaysUseWindowPadding,
		[]()
		{
			CUIMenuWidgets::the().add_checkbox("Enable", &mdlchams_enable);

			g_gui_widgets_i->add_padding({ 0, 5.0f });
			g_gui_widgets_i->add_separtor_with_text("Viewmodel");
			CUIMenuWidgets::the().add_checkbox_with_color("Enable ##VM", &mdlchams_viewmodel_enable, &mdlchams_viewmodel_color);
			CUIMenuWidgets::the().add_slider("Type ##VM", "%0.0f", &mdlchams_viewmodel_type);

			g_gui_widgets_i->add_padding({ 0, 5.0f });
			g_gui_widgets_i->add_separtor_with_text("Players - T");
			CUIMenuWidgets::the().add_checkbox_with_color("Enable ##T", &mdlchams_players_t_enable, &mdlchams_players_t_color);
			CUIMenuWidgets::the().add_slider("Type ##T", "%0.0f", &mdlchams_players_t_type);

			g_gui_widgets_i->add_padding({ 0, 5.0f });
			g_gui_widgets_i->add_separtor_with_text("Players - CT");
			CUIMenuWidgets::the().add_checkbox_with_color("Enable ##CT", &mdlchams_players_ct_enable, &mdlchams_players_ct_color);
			CUIMenuWidgets::the().add_slider("Type ##CT", "%0.0f", &mdlchams_players_ct_type);

			g_gui_widgets_i->add_padding({ 0, 5.0f });
			g_gui_widgets_i->add_separtor_with_text("Properties");
			CUIMenuWidgets::the().add_checkbox("Flat-shaded", &mdlchams_flatshaded);
			CUIMenuWidgets::the().add_checkbox("Blend", &mdlchams_blend);
		});


	g_gui_widgets_i->end_columns(1);
}

void CUIMenu::tab_visuals4()
{
	g_gui_widgets_i->add_text(__FUNCTION__ "()");
}

void CUIMenu::tab_exploits()
{
	g_gui_widgets_i->begin_columns(__FUNCTION__, 2);
	
	add_menu_child(
		"Bypass game constrains", CMenuStyle::calc_child_size(335), false, ImGuiWindowFlags_AlwaysUseWindowPadding,
		[]()
		{
			CUIMenuWidgets::the().add_checkbox("Re-enable noclip", &bypass_constrain_noclip, "Makes it possible to re-enable noclip in singleplayer. sv_cheats must be enabled.");

			CUIMenuWidgets::the().add_checkbox("Enable", &bypass_constrain_renderdist_enable);
			CUIMenuWidgets::the().add_slider("Render distance", "%0.0f units", &bypass_constrain_renderdist_value);

			CUIMenuWidgets::the().add_checkbox("Re-enable renderer cvars", &bypass_constrain_renderer_cvars);

			CUIMenuWidgets::the().add_description_text(
				"Disables R_ForceCVars, which is responsible for preventing some renderer cvars to be set.",

				"These cvars are:\n\nr_lightmap, gl_clear, r_novis, r_fullbright, snd_show, chase_active, v_lambert, gl_monolights, gl_wireframe, r_dynamic, gl_alphamin, gl_max_size, gl_polyoffset, r_drawentities, v_lightgamma.");


			CUIMenuWidgets::the().add_checkbox("Disable SP-Only cvars", &bypass_constrain_sponly_cvars, 
											   "Some cvars can be only set in singleplayer. Set this to be able to control these cvars also in MP. "
											   "See the console output for which cvars this affects.");
		});

	g_gui_widgets_i->goto_next_column();
	
	add_menu_child(
		"Frame skip", CMenuStyle::calc_child_size(175), false, ImGuiWindowFlags_AlwaysUseWindowPadding,
		[]()
		{
			CUIMenuWidgets::the().add_checkbox("Enable", &frame_skip_enable);

			CUIMenuWidgets::the().add_slider("Amount", "%0.0f frames", &frame_skip_amount);

			CUIMenuWidgets::the().add_description_text(
				"Frame skip or \"Frame simulation\" enables \"fake\" fps, that are not visual, but physical.", 

				"What it does is that it allows execution of the main rendering code only every Nth frame (based on the settings).\n"
				"This allows for massive fps boosts, since basically everything is being rendered each Nth frame only.\n"
				"If you set this up to maximum value, you will get maximum fps boost, but bigger visual lags. On the contrary, you will get less fps boost, but without visual lags.\n\n"
				"Note that above 1000fps the engine isn't really functioning as it should.");

			CUIMenuWidgets::the().add_slider("FPS limit", "~%0.0f frames/sec", &frame_skip_maxfps);
		});

#if 0
	add_menu_child(
		"Lie to the server", CMenuStyle::calc_child_size(300), false, ImGuiWindowFlags_AlwaysUseWindowPadding,
		[this]()
		{
			CUIMenuWidgets::the().add_pair_textinput("fps_max", &cvarfilter_fps_max, "Cvar name", "Value");
		});
#endif

	g_gui_widgets_i->end_columns(1);
}

void CUIMenu::tab_miscellaneous2()
{
	g_gui_widgets_i->add_text(__FUNCTION__ "()");
}

void CUIMenu::tab_miscellaneous3()
{
	g_gui_widgets_i->add_text(__FUNCTION__ "()");
}

void CUIMenu::tab_config()
{
	add_menu_child(
		"Configuration", { -1.0f, -1.0f }, false, ImGuiWindowFlags_AlwaysUseWindowPadding,
		[]()
		{
			// first = error or not, second = status message
			static std::pair<bool, std::string> current_status = {};
			static auto status_tm = std::chrono::high_resolution_clock::now();
			auto status_msg = [](const std::string& msg, bool error)
			{
				current_status = std::make_pair(error, msg);
				status_tm = std::chrono::high_resolution_clock::now();
			};

			static std::vector<FilePath_t> configs;
			static int num_configs = 0;
			auto search_for_configs = []()
			{
				configs.clear();

				g_config_mgr_i->for_each_cfg(
						[](const FilePath_t& cfg_path)
						{
							num_configs++;

							auto relative_cfg_path = g_filesystem_i->get_relative_to_appdata_ex("config\\", cfg_path);

							configs.push_back(relative_cfg_path);
						});
			};

			static auto last_searched = std::chrono::high_resolution_clock::now();

			// search each # amount of ms
			static constexpr float interval_ms = 250;
			if (std::chrono::duration<float, std::milli>(std::chrono::high_resolution_clock::now() - last_searched).count() > interval_ms)
			{
				search_for_configs();
				last_searched = std::chrono::high_resolution_clock::now();
			}

			static FilePath_t selected_cfg = {};
			if (configs.empty() && !selected_cfg.empty())
			{
				selected_cfg.clear();
			}

			g_gui_widgets_i->begin_columns(__FUNCTION__, 2);

			g_gui_widgets_i->add_text("Configuration files");

			g_gui_widgets_i->add_child(
				"configs", { -1.0f, -1.0f }, true, ImGuiWindowFlags_AlwaysUseWindowPadding,
				[]()
				{
					if (configs.empty())
					{
						g_gui_widgets_i->add_window_centered_text_disabled("No configuration files available");
					}
					else
					{
						for (const auto& cfg_path : configs)
						{
							bool is_selected = selected_cfg == cfg_path;
							if (g_gui_widgets_i->add_toggle_button(cfg_path.string(), { -1.0f, 0.0f },
																   is_selected, false))
							{
								if (is_selected)
								{
									// clicking on currently selected entry, toggle it.
									selected_cfg.clear();
								}
								else
								{
									selected_cfg = cfg_path;
								}
							}
						}
					}
				});

			g_gui_widgets_i->goto_next_column();

			g_gui_widgets_i->add_text("Options on selected cfg");

			g_gui_widgets_i->add_child(
				"selection", { -1.0f, 55 }, true, ImGuiWindowFlags_AlwaysUseWindowPadding,
				[&]()
				{
					if (!selected_cfg.empty())
					{
						if (g_gui_widgets_i->add_button("load", { -1.0f, 0.0f }, false, BUTTONFLAG_CenterLabel))
						{
							if (g_config_mgr_i->load_configuration(CFG_Variables, selected_cfg.string()))
							{
								status_msg(std::format("Loaded from {}.", selected_cfg.string()), false);
							}
							else
							{
								status_msg(std::format("Failed to load {}!", selected_cfg.string()), true);
							}
						}

						if (g_gui_widgets_i->add_button("delete", { -1.0f, 0.0f }, false, BUTTONFLAG_CenterLabel))
						{
							if (g_filesystem_i->remove(g_appdata_mgr_i->get_known("config\\") / selected_cfg))
							{
								status_msg(std::format("Deleted {}.", selected_cfg), false);
							}
							else
							{
								status_msg(std::format("Failed to delete {}.", selected_cfg), true);
							}
						}
					}
					else
					{
						g_gui_widgets_i->add_window_centered_text_disabled("No cfg selected");
					}
				});

			g_gui_widgets_i->add_text("Other");

			g_gui_widgets_i->add_child(
				"information", { -1.0f, 140 }, true, ImGuiWindowFlags_AlwaysUseWindowPadding,
				[&]()
				{
					if (g_gui_widgets_i->add_button("create new", { -1.0f, 0.0f }, false, BUTTONFLAG_CenterLabel))
					{
						static char name_buffer[64];
						COxWareUI::the().create_popup(
							[]()
							{
								g_gui_widgets_i->add_text_input("config_name", name_buffer, sizeof(name_buffer));

								g_gui_widgets_i->add_text("Enter name of the config");
							},
							[&]() // on close
							{
								auto path = g_config_mgr_i->get_config_directory(name_buffer);

								if (!path.has_extension())
								{
									path.replace_extension("json");
								}

								CfgFileVariables cfg(path);

								if (cfg.write())
								{
									status_msg(std::format("Saved '{}'.", path.filename().string()), false);
								}
								else
								{
									status_msg(std::format("Couldn't save '{}'.", path.filename().string()), true);
								}
							});
					}

					if (g_gui_widgets_i->add_button("copy config directory", { -1.0f, 0.0f }, false, BUTTONFLAG_CenterLabel))
					{
						auto dir = g_config_mgr_i->get_config_directory().string();
						CGenericUtil::the().copy_to_clipboard(dir);
						status_msg("Copied to clipboard.", false);
					}

					if (g_gui_widgets_i->add_button("save current", { -1.0f, 0.0f }, false, BUTTONFLAG_CenterLabel))
					{
						if (g_config_mgr_i->write_configuration(CFG_Variables, "saved.json"))
						{
							status_msg("Saved to saved.json.", false);
						}
						else
						{
							status_msg("Failed to save to saved.json!", false);
						}
					}

					if (g_gui_widgets_i->add_button("restore defaults", { -1.0f, 0.0f }, false, BUTTONFLAG_CenterLabel))
					{
						// this if-else tree is kinda dumb, but whatever xd
						if (g_config_mgr_i->load_configuration(CFG_Variables, "default.json"))
						{
							status_msg("Restored default configuration.", false);
						}
						else
						{
							if (!g_config_mgr_i->write_configuration(CFG_Variables, "default.json"))
							{
								status_msg("Failed to restore defaults!", true);
							}
							else
							{
								if (g_config_mgr_i->load_configuration(CFG_Variables, "default.json"))
								{
									status_msg("Restored default configuration.", false);
								}
								else
								{
									status_msg("Failed to restore defaults!", true);
								}
							}

						}
					}

					g_gui_widgets_i->add_spacing();
					CUIMenuWidgets::the().add_slider("Autosave interval", "%0.0f seconds", g_config_mgr_i->get_save_cfg_interval_var());

					g_gui_widgets_i->add_progress_bar("Test", { -1.0f, 0.0f }, g_config_mgr_i->get_duration_last_saved_sec(), (float)g_config_mgr_i->get_save_cfg_interval_var()->get_value());
				});


			g_gui_widgets_i->add_child(
				"status", { -1.0f, -1.0f }, true, ImGuiWindowFlags_AlwaysUseWindowPadding,
				[&]()
				{
						float dur_sec = std::chrono::duration<float, std::ratio<1, 1>>(std::chrono::high_resolution_clock::now() - status_tm).count();
						auto [error, msg] = current_status;
						if (!msg.empty() && dur_sec < 3.0f)
						{
							auto color = error ? CColor(112, 0, 0, 170) : CColor(0, 112, 0, 170);

							g_gui_widgets_i->push_font(g_gui_fontmgr_i->get_imgui_font("segoeui", FONT_MEDIUM, FONTDEC_Regular));
							g_gui_widgets_i->add_colored_text(color, msg);
							g_gui_widgets_i->pop_font();
						}
				});

			g_gui_widgets_i->end_columns(1);
		});
}

//---------------------------------------------------------------------------------------------------

void CUIMenu::add_menu_child(const std::string& label, const Vector2D& size, bool border, ImGuiWindowFlags flags, const std::function<void()>& pfn_contents)
{
	g_gui_widgets_i->set_next_window_rounding(CMenuStyle::k_child_contents_rounding, ImDrawFlags_RoundCornersTopLeft | ImDrawFlags_RoundCornersTopRight);

	g_gui_widgets_i->push_stylevar(ImGuiStyleVar_WindowPadding, CMenuStyle::k_child_contents_padding);

	g_gui_widgets_i->add_child_with_header(label, size, border, flags, pfn_contents);

	g_gui_widgets_i->pop_stylevar(1);
}
