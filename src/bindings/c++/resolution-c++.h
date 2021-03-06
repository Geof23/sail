/*  This file is part of SAIL (https://github.com/smoked-herring/sail)

    Copyright (c) 2020 Dmitry Baryshev

    The MIT License

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/

#ifndef SAIL_RESOLUTION_CPP_H
#define SAIL_RESOLUTION_CPP_H

#ifdef SAIL_BUILD
    #include "error.h"
    #include "export.h"
#else
    #include <sail-common/error.h>
    #include <sail-common/export.h>
#endif

struct sail_resolution;

namespace sail
{

/*
 * Image resolution unit and values.
 */
class SAIL_EXPORT resolution
{
    friend class image;

public:
    resolution();
    resolution(const resolution &res);
    resolution& operator=(const resolution &res);
    resolution(resolution &&res) noexcept;
    resolution& operator=(resolution &&res);
    ~resolution();

    /*
     * Returns true if the resolution has valid units and x/y values.
     */
    bool is_valid() const;

    /*
     * Returns the resolution unit.
     */
    SailResolutionUnit unit() const;

    /*
     * Returns the resolution x value.
     */
    float x() const;

    /*
     * Returns the resolution y value.
     */
    float y() const;

    /*
     * Sets a new resolution units.
     */
    resolution& with_unit(SailResolutionUnit unit);

    /*
     * Sets a new resolution x value.
     */
    resolution& with_x(float x);

    /*
     * Sets a new resolution y value.
     */
    resolution& with_y(float y);

private:
    /*
     * Makes a deep copy of the specified resolution.
     */
    resolution(const sail_resolution *res);

    sail_status_t to_sail_resolution(sail_resolution *res) const;

private:
    class pimpl;
    pimpl *d;
};

}

#endif
