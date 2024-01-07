#include "load_asset.h"
#include "common/abort.h"

#include <fstream>

#include "shader_asset.h"
#include "source/utilities/hash_string.h"

namespace assets
{
	namespace loading
	{
		std::shared_ptr<asset> load_texture(nlohmann::json& data)
		{
			return nullptr;
		}

		std::shared_ptr<asset> load_shader(std::string& assets_folder_path, nlohmann::json& data)
		{
			std::vector<std::string> layout = { "" };
			std::string vertex_code;
			std::string fragment_code;

			if (!(data.contains("path") && data.at("path").is_string()))
				abort("Invalid asset: " + assets_folder_path + "\nMissing/Invalid path parameter");
			else
			{
				std::string source_path = data.at("path");
				std::fstream source_file;
				source_file.open(assets_folder_path + source_path);
				if (source_file.fail())
					abort("Invalid asset: " + assets_folder_path + source_path + "\nNo shader source file at given path");
				else
				{
					source_file.seekg(0, std::ios::end);
					size_t size = source_file.tellg();
					std::string source(size, ' ');
					source_file.seekg(0);
					source_file.read(&source[0], size);
					source_file.close();

					//seek start
					int i = 0;
					auto seek_start = [&]()
					{
						for (; i < source.size(); i++)
							if (source[i] == '<')
								break;
						i++;
					};
					seek_start();

					//load layout
					for (; i < source.size(); i++)
					{
						if (source[i] == '>')
							break;
						else if (source[i] == ',')
							layout.push_back("");
						else if (source[i] != ' ' && source[i] != '\r' && source[i] != '\t' && source[i] != '\n')
							layout.back().push_back(source[i]);
					}				

					//load vertex
					seek_start();
					for (; i < source.size(); i++)
					{
						if (source[i] == '>')
							break;
						else
							vertex_code.push_back(source[i]);
					}

					//load fragment
					seek_start();
					for (; i < source.size(); i++)
					{
						if (source[i] == '>')
							break;
						else
							fragment_code.push_back(source[i]);
					}
				}
			}
		
			std::vector<uint32_t> hashed_layout;
			for (auto& element : layout)
				hashed_layout.push_back(utilities::hash_string(element));

			return std::make_shared<shader>(vertex_code, fragment_code, hashed_layout);
		}
	}
}