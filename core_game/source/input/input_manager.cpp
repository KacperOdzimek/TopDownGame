#include "input_manager.h"
#include "source/common/crash.h"

#include "source/assets/input_config_asset.h"

#include "source/common/common.h"
#include "source/window/window_manager.h"

using namespace input;

void input_manager::load_config(std::weak_ptr<assets::input_config> _config_asset)
{
	config_asset = _config_asset.lock();

	a_action_state_is_current = false;

	action_mappings_states.clear();
	axis_mappings_states.clear();

	for (const auto& action : config_asset->action_mappings)
	{
		action_mappings_states.insert({ action.first, {} });
		for (auto& key : action.second.keys)
			keys_to_check.insert(key);
	}

	for (const auto& axis : config_asset->axis_mappings)
	{
		axis_mappings_states.insert({ axis.first, 0.0f });
		for (auto& key : axis.second.keys)
			keys_to_check.insert(key);
	}
}

void input_manager::update_mappings_states()
{
	a_action_state_is_current = !a_action_state_is_current;

	std::vector<key_state> states;
	for (auto& k : keys_to_check)
	{
		switch (k.key_type)
		{
		case input::key_type::keyboard: case input::key_type::mouse:
			states.push_back(common::window_manager->get_key_state(k));
			break;
		case input::key_type::gamepad:
			//TODO
			break;
		}
	}
	
	std::vector<key> pressed_keys;
	for (auto& k : states)
		if (k.state == 1)
			pressed_keys.push_back(k.key);

	for (const auto& action : config_asset->action_mappings)
	{
		auto& state = action_mappings_states.at(action.first);
		if (a_action_state_is_current)
			state.a = action.second.get_value(pressed_keys);
		else
			state.b = action.second.get_value(pressed_keys);
	}

	for (const auto& axis : config_asset->axis_mappings)
		axis_mappings_states.at(axis.first) = axis.second.get_value(states);
}

bool input_manager::get_action_mapping_value(const std::string& mapping_name)
{
	auto itr = action_mappings_states.find(mapping_name);
	if (itr == action_mappings_states.end())
		error_handling::crash(error_handling::error_source::core, "[input_manager::get_action_mapping_value]", 
			"Trying to get value of non-existent action mapping: " + mapping_name);
	if (a_action_state_is_current)
		return itr->second.a;
	return itr->second.b;
}

bool input_manager::get_action_mapping_was_just_pressed(const std::string& mapping_name)
{
	auto itr = action_mappings_states.find(mapping_name);
	if (itr == action_mappings_states.end())
		error_handling::crash(error_handling::error_source::core, "[input_manager::get_action_mapping_was_just_pressed]",
			"Trying to get value of non-existent action mapping: " + mapping_name);
	if (a_action_state_is_current)
		return (itr->second.a && !itr->second.b);
	return (itr->second.b && !itr->second.a);
}

bool input_manager::get_action_mapping_was_just_relased(const std::string& mapping_name)
{
	auto itr = action_mappings_states.find(mapping_name);
	if (itr == action_mappings_states.end())
		error_handling::crash(error_handling::error_source::core, "[input_manager::get_action_mapping_was_just_relased]",
			"Trying to get value of non-existent action mapping: " + mapping_name);
	if (a_action_state_is_current)
		return (!itr->second.a && itr->second.b);
	return (!itr->second.b && itr->second.a);
}

float input_manager::get_axis_mapping_value(const std::string& mapping_name)
{
	auto itr = axis_mappings_states.find(mapping_name);
	if (itr == axis_mappings_states.end())
		error_handling::crash(error_handling::error_source::core, "[input_manager::get_axis_mapping_value]",
			"Trying to get value of non-existent axis mapping: " + mapping_name);
	return itr->second;
}