/*
** EPITECH PROJECT, 2020
** Plug
** File description:
** Modules
*/

#ifndef MODULES_HPP_
#define MODULES_HPP_

#if defined (_WIN32)
    #define PLUG_MODULE_HOOK extern "C" __declspec(dllexport)
#else
    #define PLUG_MODULE_HOOK extern "C"
#endif

#define STRINGYFY(str) #str

#define PLUG_MODULE_OBJECT(Object)\
 PLUG_MODULE_HOOK Object    *PLUG_MODULE_OBJECT_CREATE() {return new Object();}\
 PLUG_MODULE_HOOK void      PLUG_MODULE_OBJECT_DESTROY(Object *obj) {delete obj;}\
// /PLUG_MODULE_OBJECT

namespace Plug { namespace Modules {
    constexpr decltype(auto)    CreateObject = STRINGYFY(PLUG_MODULE_OBJECT_CREATE);
    constexpr decltype(auto)    DestroyObject = STRINGYFY(PLUG_MODULE_OBJECT_DESTROY);
}}

#endif /* !MODULES_HPP_ */
