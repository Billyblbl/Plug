/*
** EPITECH PROJECT, 2020
** Plug
** File description:
** Modules
*/

#ifndef MODULES_HPP_
#define MODULES_HPP_

///@file

#include "static_string.hpp"

#define STRINGYFY(str) #str

#if defined (_WIN32)
    ///
    ///@brief Module Export API Hook
    ///
    ///
    #define PLUG_MODULE_HOOK extern "C" __declspec(dllexport)
#else
    ///
    ///@brief Module Export API HOOK
    ///
    ///
    #define PLUG_MODULE_HOOK extern "C"
#endif

///
///@brief Object module API Hooks defining macro
///
/// Default unique object
///
///@arg Object Module object class
///
#define PLUG_MODULE_OBJECT(Object)\
 PLUG_MODULE_HOOK Object    *PLUG_MODULE_OBJECT_CREATE() {return new Object();}\
 PLUG_MODULE_HOOK void      PLUG_MODULE_OBJECT_DESTROY(Object *obj) {delete obj;}\
// /PLUG_MODULE_OBJECT

///
///@brief Object module API Hooks defining macro
///
/// Generic object, can be used several times in the same binary
///
///@arg Object Module object class
///@arg name name of the object as known by the host program
///
#define PLUG_MODULE_OBJECT(Object, name)\
 PLUG_MODULE_HOOK Object    *PLUG_MODULE_OBJECT_CREATE_##name() {return new Object();}\
 PLUG_MODULE_HOOK void      PLUG_MODULE_OBJECT_DESTROY_##name(Object *obj) {delete obj;}\
// /PLUG_MODULE_OBJECT


namespace Plug { namespace Modules {

    ///
    ///@brief Object module creator function standard symbol string
    ///
    ///
    constexpr decltype(auto)    CreateObject = STRINGYFY(PLUG_MODULE_OBJECT_CREATE);

    template<long unsigned size>
    constexpr decltype(auto)    CreateNamedObject(static_string<size> name)
    {
        return MakeStaticStr(CreateObject) + "_" + name;
    }

    ///
    ///@brief Object module destructor function standard symbol string
    ///
    ///
    constexpr decltype(auto)    DestroyObject = STRINGYFY(PLUG_MODULE_OBJECT_DESTROY);

    template<long unsigned size>
    constexpr decltype(auto)    DestroyNamedObject(static_string<size> name)
    {
        return MakeStaticStr(DestroyObject) + "_" + name;
    }

}}

#endif /* !MODULES_HPP_ */
