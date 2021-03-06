/*
** EPITECH PROJECT, 2020
** Plug
** File description:
** Modules
*/

#ifndef RAW_HPP_
#define RAW_HPP_

#include "Cache.hpp"
#include "DynLib.hpp"

///@file

namespace Plug { namespace Modules {

    ///
    ///@brief Global cache access for raw dynlib modules
    ///
    ///
    Cache<DynLib>   &Raw();
}} // namespace Plug

#endif /* !RAW_HPP_ */
