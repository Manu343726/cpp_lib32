/* 
 * File:   dl32Assert.h
 * Author: Manu343726
 *
 * Created on 26 de mayo de 2013, 17:15
 */

#ifndef DL32ASSERT_H
#define	DL32ASSERT_H

#include "dl32Typing.h"
#include <iostream>
#include <typeinfo>
#include <string>

using namespace std;

#define DL32ASSERT_USE_CASSERT FALSE
#define DL32ASSERT_USE_COLOR FALSE

#define EXPAND(x) x

#if DL32ASSERT_USE_CASSERT
#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include <assert.h>
#else

#define assert_data(x) __FILE__ << ", in function " << __FUNCTION__  << " (line " <<  __LINE__ << "):" << " '" << #x << "' --> "

#if DL32ASSERT_USE_COLOR

#include "dl32ConsoleColor.h"

#ifdef assert
#undef assert
#endif /* ASSERT */

#define assert(x) (x) ? cout << assert_data(x) << "OK!" << endl : cout << push_style << dl32ChangeForegroundColor( dl32ConsoleColor::RED ) << assert_data(x) << "ERROR (Cascao raro)" << pop_style << endl 
#else

#ifdef assert
#undef assert
#endif /* ASSERT */

#define assert(x) (x) ? cout << assert_data(x) << "OK!" << endl : cout << assert_data(x) << "ERROR (Cascao raro)" << endl
#endif /* TESTING_USE_COLOR */
#endif /* TESTING_USEASSERT */

#endif	/* DL32ASSERT_H */

