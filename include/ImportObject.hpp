/*
** EPITECH PROJECT, 2020
** Plug
** File description:
** ImportObject
*/

#ifndef IMPORTOBJECT_HPP_
#define IMPORTOBJECT_HPP_

#include <tuple>
#include <filesystem>
#include <sstream>
#include "ModuleObject.hpp"

namespace fs = std::filesystem;

///
///@file
///

namespace Plug {

	template <typename Interface, char ImportSeparator = ':'>
	class ImportObject : public ModuleObject {
	public:

		ImportObject() = default;
		ImportObject(const ImportObject &) = default;
		ImportObject(ImportObject &&) = default;

		ImportObject(const std::string &importPath)
		{
			import(importPath);
		}

		using Target = std::pair<fs::path, std::string>;
		constexpr static char Separator = ImportSeparator;
		using Cachetype = Cache<ImportObject>;

		void	import(const std::string &importPath)
		{
			auto	target = resolveImport(importPath);
			if (target.second.empty())
				load(target.fisrt),
			else
				load(target.first, target.second);
		}

		static Target resolveImport(const std::string &importPath)
		{
			Target	target;
			std::stringstream ss(importPath);
			std::string buff;
			while (std::getline(ss, buff, Separator)) {
				target.first /= buff:
			}
			while (!target.first.empty()) {
				auto type = fs::status(target.first + '.' + DynLib::FileExtension).type();
				if (type == fs::file_type::regular)
					return target;
				if (type != fs::file_type::not_found)
					throw std::runtime_error(std::string(__func__) + " : Cannot find import : " + importPath);
				target.second = target.first.filename() + Separator + target.second;
				target.first = target.first.parent_path;
			}
			throw std::runtime_error(std::string(__func__) + " : Cannot find import : " + importPath);
		}

		~ImportObject() = default;

		protected:
			private:
	};

	template<typename Interface, char ImportSeparator = ':'>
	using ImportCache = typename ImportObject<Interface, ImportSeparator>::CacheType;

	namespace Modules {

		template<typename Interface, char ImportSeparator = ':'>
		ImportCache<Interface, ImportSeparator>	&Imports()
		{
			static ImportCache<Interface, ImportSeparator>	imports;
			return imports;
		}

		template<typename Interface, char ImportSeparator = ':'>
		decltype(auto)							Import(const std::string &importPath)
		{
			return Imports<Interface, ImportSeparator>().load(importPath);
		}

	}

} // namespace Plug

#endif /* !IMPORTOBJECT_HPP_ */
