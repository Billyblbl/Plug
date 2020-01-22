/*
** EPITECH PROJECT, 2019
** rtype
** File description:
** DynLib
*/

#include "DynLib.hpp"

#if defined(_WIN32)
//Windows

Plug::DynLib::DynLib(const std::string &path):
	_handle{LoadLibrary(path.c_str())}
{
	if (_handle == NULL)
		throw std::runtime_error(std::string(__func__) + " : " + GetLastError());
}

Plug::DynLib::~DynLib()
{
	if (_handle != NULL && FreeLibrary(_handle) == 0)
		throw std::runtime_error(std::string("FreeLibrary : ") + GetLastError());
}

#else
//UNIX

Plug::DynLib::DynLib(const std::string &path):
	_handle{dlopen(path.c_str(), RTLD_NOW), [](void *hdl){
		auto	err = dlclose(hdl);
		if (err !=  0)
			throw std::runtime_error(std::string("dlclose : ") + dlerror());
	}}
{
	if (_handle == nullptr)
		throw std::runtime_error(std::string(__func__) + " : " + dlerror());
}

#endif
