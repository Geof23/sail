/*  This file is part of SAIL (https://github.com/sailor-keg/sail)

    Copyright (c) 2020 Dmitry Baryshev <dmitrymq@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 3 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this library. If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef SAIL_SAIL_CPP_H
#define SAIL_SAIL_CPP_H

#ifdef SAIL_BUILD
    #include "error.h"
    #include "export.h"
#else
    #include <sail/error.h>
    #include <sail/export.h>
#endif

#include <memory>
#include <vector>

namespace sail
{

class plugin_info;

/*
 * A C++ interface to struct sail_context.
 */
class SAIL_EXPORT context
{
public:
    context();
    ~context();

    bool is_valid() const;

    std::vector<plugin_info> plugin_info_list() const;

private:
    sail_error_t init();

private:
    class pimpl;
    const std::unique_ptr<pimpl> d;
};

}

#endif
