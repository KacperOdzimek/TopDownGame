#include "source/common/common.h"
#include "source/common/abort.h"

#include "source/rendering/renderer.h"
#include "source/assets/assets_manager.h"
#include "source/entities/world.h"
#include "source/behaviors/behaviors_manager.h"
#include "source/physics/collision_solver.h"

#include <time.h>

//Can be removed in final version
#include "source/entities/geometry_component.h"
#include "source/components/camera.h"
#include "source/components/behavior.h"
#include "source/components/sprite.h"
#include "source/components/collider.h"

#include "source/assets/texture_asset.h"
#include "source/assets/behavior_asset.h"
#include "source/assets/shader_asset.h"

#include "source/utilities/hash_string.h"

#include "source/physics/collision_present.h"
#include <iostream>
#include <bitset>
//

int main()
{
	common::renderer->create_window();
	common::renderer->create_api_instance();

	common::assets_manager->set_assets_path("C:/Projekty/TopDownGame/mods/example_mod/assets");

	common::assets_manager->load_asset_from_json("/shaders/cat_shader.json");
	common::assets_manager->load_asset_from_json("/textures/cat_texture.json");
	common::assets_manager->load_asset_from_json("/behaviors/move_right.json");
	common::assets_manager->load_asset_from_json("/behaviors/move_left.json");

	common::world->create_active_scene();

	/*
		The Box
	*/
	auto create_spining_box = [&](glm::vec2 pos, std::string beh_path)
	{
		auto box = new entities::entity;
		box->teleport(pos);

		box->attach_component(
			new entities::components::sprite
			(
				utilities::hash_string("geo"),
				entities::geometry_draw_settings
		{
			assets::cast_asset<assets::shader>(common::assets_manager->get_asset(utilities::hash_string("/shaders/cat_shader.json"))),
			{
				assets::cast_asset<assets::texture>(common::assets_manager->get_asset(utilities::hash_string("/textures/cat_texture.json")))
			}
		}
		)
		);

		auto f = common::collision_solver->gen_flag(0, { physics::collision_response::collide });
		auto col = new entities::components::collider
		{
			utilities::hash_string("collider1"), f, {0.5f, 0.5f}
		};
		box->attach_component(
			col
		);

		box->attach_component(
			new entities::components::behavior
			{
				utilities::hash_string("bhv"),
				assets::cast_asset<assets::behavior>(common::assets_manager->get_asset(utilities::hash_string(beh_path)))
			}
		);

		return std::pair<entities::entity*, entities::components::collider*>{ box, col };
	};

	auto b1 = create_spining_box({-1.0f, 0.0f}, "/behaviors/move_left.json");
	auto b2 = create_spining_box({1.0f, 0.0f}, "/behaviors/move_right.json");

	/*
		Camera Actor
	*/
	auto camera_entity = new entities::entity;
	auto camera_comp = new entities::components::camera{ utilities::hash_string("cam"), 16 };
	camera_entity->attach_component(camera_comp);
	common::renderer->set_active_camera(camera_comp);
	camera_entity->teleport({ 0.0f,0.0f });

	while (!common::renderer->should_window_close())
	{
		double frame_start = ((double)clock()) / (double)CLOCKS_PER_SEC;

		common::behaviors_manager->call_update_functions();

		common::renderer->collect_geometry_data();
		common::renderer->render();
		common::renderer->update_window();

		double frame_end = ((double)clock()) / (double)CLOCKS_PER_SEC;
		common::delta_time = frame_end - frame_start;
		common::delta_time = 0.02;
	}
	common::world.reset();
	common::assets_manager.reset();
	common::renderer.reset();
}