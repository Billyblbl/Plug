/*
** EPITECH PROJECT, 2019
** rtype
** File description:
** DynLib
*/

#include <iostream>
#include "DynLib.hpp"

#if defined(_WIN32)
//Windows

static std::wstring s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
    std::wstring r(len, L'\0');
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, &r[0], len);
    return r;
}

Plug::DynLib::DynLib(const std::string &path):
	_handle{LoadLibrary((LPCWSTR)s2ws(path).c_str())}
{
	if (_handle == NULL)
		throw std::runtime_error(std::string(__func__) + " : " + (char *)GetLastError());
}

Plug::DynLib::~DynLib()
{
	if (_handle != NULL && FreeLibrary(_handle) == 0)
		std::cerr << "FreeLibrary : " << (char *)GetLastError() << '\n';
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
