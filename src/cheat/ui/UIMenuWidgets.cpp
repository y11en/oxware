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

bool CUIMenuWidgets::add_checkbox(const std::string& label, VarBoolean* var, const char* additional_desc)
{
	float value = var->get_value();
	bool ret = g_gui_widgets_i->add_checkbox(label, &value);

	var->set_value(value);

	add_description_text(additional_desc);

	handle_widget_hover(var);

	return ret;
}

bool CUIMenuWidgets::add_checkbox_with_color(const std::string& label, VarBoolean* toggle_var, VarColor* colors_var, const char* additional_desc)
{
	auto color = colors_var->get_value();

	float value = (float)toggle_var->get_value();
	float c[4] = { color.r, color.g, color.b, color.a };
	bool ret = g_gui_widgets_i->add_checkbox_with_color(label, &value, c);

	toggle_var->set_value((int)value);
	colors_var->set_value(CColor(c[0], c[1], c[2], c[3]));

	add_description_text(additional_desc);

	handle_widget_hover(toggle_var);

	return ret;
}

bool CUIMenuWidgets::add_color_edit(const std::string& label, VarColor* colors_var, const char* additional_desc)
{
	auto color = colors_var->get_value();
	float c[4] = { color.r, color.g, color.b, color.a };
	bool ret = g_gui_widgets_i->add_color_edit(label, c);

	colors_var->set_value(CColor(c[0], c[1], c[2], c[3]));

	add_description_text(additional_desc);

	handle_widget_hover(colors_var);

	return ret;
}

template bool CUIMenuWidgets::add_slider_t<VarFloat>(const std::string&, const char*, VarFloat*, const char*);
template bool CUIMenuWidgets::add_slider_t<VarInteger>(const std::string&, const char*, VarInteger*, const char*);

template<typename T>
bool CUIMenuWidgets::add_slider_t(const std::string& label, const char* format, T* var, const char* additional_desc)
{
	assert(var->has_bounds() && "Variable used in slider must have bounds.");

	// get bounds
	float min = var->get_min(), max = var->get_max();
	float value = var->get_value();

	bool ret = g_gui_widgets_i->add_slider(label, &value, &min, &max, format);

	// we have to distinguish in order to call appropriate function
	var->set_value(value);

	add_description_text(additional_desc);

	handle_widget_hover(var);

	return ret;
}

bool CUIMenuWidgets::add_slider(const std::string& label, const char* format, VarFloat* var, const char* additional_desc)
{
	return add_slider_t<VarFloat>(label, format, var, additional_desc);
}

bool CUIMenuWidgets::add_slider(const std::string& label, const char* format, VarInteger* var, const char* additional_desc)
{
	return add_slider_t<VarInteger>(label, format, var, additional_desc);
}

/*
void CUIMenuWidgets::add_pair_textinput(const std::string& label, BaseVariable* var, const char* first_column_header, const char* second_column_header)
{
	assert(var->get_type() == VARDT_KeyValue && "The variable that is used inside pair textinput widget must be key value!");

	float left_textinput_width = CMenuStyle::get_child_width_w_padding() / 1.5f;

	g_gui_widgets_i->begin_columns(label.c_str(), 2);
	g_gui_widgets_i->set_column_width(0, left_textinput_width);

	if (first_column_header)
	{
		g_gui_widgets_i->add_text(first_column_header);
	}

	bool reclaim_focus_value = false;
	if (g_gui_widgets_i->add_text_input_ex(label + "1", (char*)var->get_keyvalue()->key().c_str(), var->get_keyvalue()->key().length(), Vector2D(-1.0f, 0.0f)))
	{
		if (!var->get_keyvalue()->key().empty())
		{
			var->set_keyvalue_directly(KeyValue(var->get_keyvalue()->key(), var->get_keyvalue()->value()));
		}
		reclaim_focus_value = true;
	};

	if (reclaim_focus_value)
	{
		g_gui_widgets_i->set_item_default_focus();
		g_gui_widgets_i->set_keyboard_focus_here(-1); // Auto focus previous widget
	}

	g_gui_widgets_i->goto_next_column();

	if (first_column_header)
	{
		g_gui_widgets_i->add_text(second_column_header);
	}

	bool reclaim_focus_key = false;
	if (g_gui_widgets_i->add_text_input_ex(label + "2", (char*)var->get_keyvalue()->value().c_str(), var->get_keyvalue()->value().length(), Vector2D(-1.0f, 0.0f)))
	{
		if (!var->get_keyvalue()->value().empty())
		{
			var->set_keyvalue_directly(KeyValue(var->get_keyvalue()->key(), var->get_keyvalue()->value()));
		}

		reclaim_focus_key = true;
	};

	// Auto-focus on window apparition
	//g_gui_widgets_i->set_item_default_focus();
	if (reclaim_focus_key)
	{
		g_gui_widgets_i->set_item_default_focus();
		g_gui_widgets_i->set_keyboard_focus_here(-1); // Auto focus previous widget
	}

	g_gui_widgets_i->end_columns(1);
 }*/

void CUIMenuWidgets::add_description_text(const char* additional_desc, const char* readmore_string)
{
	if (!additional_desc)
		return;

	g_gui_widgets_i->push_font(g_gui_fontmgr_i->get_imgui_font("segoeui", FONT_SMALL, FONTDEC_Regular));
	g_gui_widgets_i->add_text(additional_desc, TEXTPROP_Wrapped);
	g_gui_widgets_i->pop_font();
	if (!readmore_string)
	{
		g_gui_widgets_i->add_padding({ 0.0f, 3.0f });
	}
	else
	{
		g_gui_widgets_i->add_readmore_on_hover_widget(readmore_string);
	}
}

void CUIMenuWidgets::handle_widget_hover(BaseVariable* var)
{
	if (g_gui_widgets_i->is_last_widget_hovered())
	{
		CMenuWidgetDescription::the().set_desc(var->get_description());
	}
}
