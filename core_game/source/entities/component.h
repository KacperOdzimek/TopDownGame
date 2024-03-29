#pragma once
#include <cstdint>
#include <memory>
#include "include/glm/vec2.hpp"

namespace entities
{
	class entity;

	class component
	{
		friend entities::entity;
	protected:
		uint32_t id;
		entity* owner = nullptr;
		glm::vec2& get_owner_location();
	public:
		component(uint32_t _id) : id(_id) {};
		std::weak_ptr<entity> get_owner_weak();
		virtual ~component() {};
		virtual void on_attach() = 0;
	};
}