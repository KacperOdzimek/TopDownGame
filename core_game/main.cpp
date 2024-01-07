#include "common/common.h"
#include "common/abort.h"

#include "source/engine/renderer.h"
#include "source/engine/assets_manager.h"
#include "source/engine/world.h"

#include "source/entities/geometry_component.h"
#include "source/utilities/hash_string.h"

#include <iostream>

int main()
{
	common::renderer->create_window();
	common::renderer->create_api_instance();

	common::assets_manager->set_assets_path("C:/Projekty/TopDownGame/mods/example_mod/assets");
	common::assets_manager->load_asset_from_json("/cat_shader.json");

	auto* scene = common::world->create_active_scene();
	auto entity = new entities::entity;

	std::weak_ptr<assets::shader> shader = assets::cast_asset<assets::shader>(
		common::assets_manager->get_asset(utilities::hash_string("/cat_shader.json")));

	auto geo_comp =
	new entities::test_geometry_component
	(
		entities::geometry_draw_settings{ std::shared_ptr<assets::shader>(shader) }
	);
	entity->attach_component(
		geo_comp
	);

	entity->position = { -0.75f, 0.75f };

	scene->add_entity(entity);
	while (!common::renderer->should_window_close())
	{
		common::renderer->collect_geometry_data();
		common::renderer->render();
		common::renderer->update_window();
		entity->position.y -= 0.001;
		entity->position.x += 0.001;
	}
	common::world.reset();
	common::assets_manager.reset();
	common::renderer.reset();
}