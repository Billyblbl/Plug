/*
** EPITECH PROJECT, 2020
** Plug
** File description:
** Raw
*/

#include "Raw.hpp"

namespace Plug {
    Cache<DynLib>   &Modules::Raw()
    {
        static Cache<DynLib>    raws;
        return raws;
    }
}