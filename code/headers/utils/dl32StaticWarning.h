/* 
 * File:   dl32StaticWarning.h
 * Author: Manu343726
 *
 * Created on 6 de junio de 2013, 20:31
 */
/*******************************************************************************
 * cpp_lib32 project. C++ port of the dx_lib32 project.                         *
 *                                                                              *
 * Copyright (C) 2012 - 2013, Manuel Sánchez Pérez                              *                     
 *                                                                              *
 * This file is part of cpp_lib32 project.                                      *
 *                                                                              *
 * cpp_lib32 project is free software: you can redistribute it and/or modify    *
 * it under the terms of the GNU Lesser General Public License as published by  *
 * the Free Software Foundation, version 2 of the License.                      *
 *                                                                              *
 * cpp_lib32 is distributed in the hope that it will be useful,                 *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of               * 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                *
 * GNU Lesser General Public License for more details.                          *
 *                                                                              *
 * You should have received a copy of the GNU Lesser General Public License     *
 * along with cpp_lib32 project. If not, see <http://www.gnu.org/licenses/>.    *
 *******************************************************************************/

//Portable static-warning implementation extracted from http://stackoverflow.com/questions/8936063/does-there-exist-a-static-warning

#ifndef DL32STATICWARNING_H
#define	DL32STATICWARNING_H

#if defined(__GNUC__)
#define DEPRECATE(foo, msg) foo __attribute__((deprecated(msg)))
#elif defined(_MSC_VER)
#define DEPRECATE(foo, msg) __declspec(deprecated(msg)) foo
#else
#error This compiler is not supported
#endif

#define PP_CAT(x,y) PP_CAT1(x,y)
#define PP_CAT1(x,y) x##y
#define EXPAND(x) x

namespace detail
{
    struct true_type {};
    struct false_type {};
    template <int test> struct converter : public true_type {};
    template <> struct converter<0> : public false_type {};
}

#define STATIC_WARNING(cond, msg) \
struct PP_CAT(static_warning,__LINE__) { \
  DEPRECATE(void _(::detail::false_type const& ),msg) {}; \
  void _(::detail::true_type const& ) {}; \
  PP_CAT(static_warning,__LINE__)() {_(::detail::converter<(cond)>());} \
}

// Note: using STATIC_WARNING_TEMPLATE changes the meaning of a program in a small way.
// It introduces a member/variable declaration.  This means at least one byte of space
// in each structure/class instantiation.  STATIC_WARNING should be preferred in any 
// non-template situation.
//  'token' must be a program-wide unique identifier.
#define STATIC_WARNING_TEMPLATE(token, cond, msg) \
    STATIC_WARNING(cond, msg) PP_CAT(PP_CAT(_localvar_, token),__LINE__)

#endif	/* DL32STATICWARNING_H */

