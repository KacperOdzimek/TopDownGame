#pragma once
#include "asset.h"

namespace behaviors
{
	class behaviors_manager;
}

namespace assets
{
	struct behavior : public asset
	{
	friend behaviors::behaviors_manager;
	protected:
		std::string name;
	public:
		behavior(std::string& lua_file_path);
		~behavior();
	};
}