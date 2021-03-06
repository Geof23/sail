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

#ifndef SAIL_AT_SCOPE_EXIT_CPP_H
#define SAIL_AT_SCOPE_EXIT_CPP_H

namespace sail
{

template<typename F>
class scope_cleanup
{
public:
    scope_cleanup(F f)
        : m_f(f)
    {
    }
    ~scope_cleanup()
    {
        m_f();
    }

private:
    F m_f;
};

}

/*
 * Executes the specified code when the scope exits. This macro could be used to perform
 * complex cleanup procedures that cannot be achieved by using destructors.
 *
 * For example:
 *
 *    SAIL_AT_SCOPE_EXIT (
 *        delete data1;
 *        delete data2;
 *    );
 *
 *    SAIL_TRY(...);
 *    SAIL_TRY(...);
 */
#define SAIL_AT_SCOPE_EXIT(code)                           \
    auto lambda = [&] {                                    \
        code                                               \
    };                                                     \
    sail::scope_cleanup<decltype(lambda)> scp_ext(lambda); \
do {                                                       \
    (void)scp_ext;                                         \
} while(0)

#endif
