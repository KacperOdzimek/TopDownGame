#include "input_functions.h"

#include "utilities.h"

#include "source/common/common.h"
#include "source/input/input_manager.h"

namespace behaviors
{
	namespace lua_shared
	{
		namespace input
		{
			int _i_action(lua_State* L)
			{
				std::string action_name = lua_tostring(L, 1);
				bool v = common::input_mananger->get_action_mapping_value(action_name);
				lua_pushboolean(L, v);
				return 1;
			}

			int _i_action_just_pressed(lua_State* L)
			{
				std::string action_name = lua_tostring(L, 1);
				bool v = common::input_mananger->get_action_mapping_was_just_pressed(action_name);
				lua_pushboolean(L, v);
				return 1;
			}

			int _i_action_just_relased(lua_State* L)
			{
				std::string action_name = lua_tostring(L, 1);
				bool v = common::input_mananger->get_action_mapping_was_just_relased(action_name);
				lua_pushboolean(L, v);
				return 1;
			}

			int _i_axis(lua_State* L)
			{
				std::string axis_name = lua_tostring(L, 1);
				float v = common::input_mananger->get_axis_mapping_value(axis_name);
				lua_pushnumber(L, v);
				return 1;
			}

			void register_shared(lua_State* L)
			{
				lua_register(L, "_i_action", _i_action);
				lua_register(L, "_i_action_just_pressed", _i_action_just_pressed);
				lua_register(L, "_i_action_just_relased", _i_action_just_relased);
				lua_register(L, "_i_axis", _i_axis);
			}
		}
	}
}