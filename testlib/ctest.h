/* Copyright 2011,2012 Bas van den Berg
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef CTEST_H
#define CTEST_H

#if _WIN32
// problem avoided here was:
// C:\Program Files (x86)\Windows Kits\10\include\10.0.10586.0\ucrt\corecrt_io.h(49): warning C4820: '_finddata32i64_t': '4' bytes padding added after data member 'name'
#pragma warning( disable : 4820 )
#include "io.h" // isatty
#pragma warning( default : 4820 )
#define isatty _isatty
#endif


/*
typedef void (*SetupFunc)(void*);
typedef void (*TearDownFunc)(void*);
*/

#pragma pack(push, 1)
struct ctest {
    const char* ssname;  // suite name
    const char* ttname;  // test name
    void (*run)(void);
    int skip;

    void* data;
//    SetupFunc setup;
//    TearDownFunc teardown;
    unsigned int magic;
};
#pragma pack(pop)

#define __FNAME(sname, tname) __ctest_##sname##_##tname##_run
#define __TNAME(sname, tname) __ctest_##sname##_##tname

#define __CTEST_MAGIC (0xdeadbeef)
#ifdef __APPLE__
#define __Test_Section __attribute__ ((unused,section ("__DATA, .ctest")))
#else
#define __Test_Section __attribute__ ((unused,section (".ctest")))
#endif

#if __linux__ || __APPLE__
#define __CTEST_STRUCT(sname, tname, _skip, __data, __setup, __teardown) \
    struct ctest __TNAME(sname, tname) __Test_Section = { \
        .ssname=#sname, \
        .ttname=#tname, \
        .run = __FNAME(sname, tname), \
        .skip = _skip, \
        .data = __data, \
/*		.setup = (SetupFunc)__setup,                    \
        .teardown = (TearDownFunc)__teardown,                \
*/		.magic = __CTEST_MAGIC };
#elif _WIN32
#define __CTEST_STRUCT(sname, tname, _skip, __data, __setup, __teardown) \
    static const char __TNAME(sname, tname)_sname[] = #sname; \
    static const char __TNAME(sname, tname)_tname[] = #tname; \
    __pragma(data_seg(push, "ctest")); \
    struct ctest __TNAME(sname, tname) = { \
        .ssname = __TNAME(sname, tname)_sname, \
        .ttname = __TNAME(sname, tname)_tname, \
        .run = __FNAME(sname, tname), \
        .skip = _skip, \
        .data = __data, \
/*      .setup = (SetupFunc)__setup,                    \
        .teardown = (TearDownFunc)__teardown,                \
*/      .magic = __CTEST_MAGIC }; \
    __pragma(data_seg(pop));
#else
#error "unknown arch"
#endif

/*
#define CTEST_DATA(sname) struct sname##_data

#define CTEST_SETUP(sname) \
    void __attribute__ ((weak)) sname##_setup(struct sname##_data* data)

#define CTEST_TEARDOWN(sname) \
    void __attribute__ ((weak)) sname##_teardown(struct sname##_data* data)
*/

#define __CTEST_INTERNAL(sname, tname, _skip) \
    void __FNAME(sname, tname)(void); \
    __CTEST_STRUCT(sname, tname, _skip, NULL, NULL, NULL); \
    void __FNAME(sname, tname)(void)

/*
#ifdef __APPLE__
#define SETUP_FNAME(sname) NULL
#define TEARDOWN_FNAME(sname) NULL
#else
#define SETUP_FNAME(sname) sname##_setup
#define TEARDOWN_FNAME(sname) sname##_teardown
#endif
*/

/*
#define __CTEST2_INTERNAL(sname, tname, _skip) \
    static struct sname##_data  __ctest_##sname##_data; \
    CTEST_SETUP(sname); \
    CTEST_TEARDOWN(sname); \
    void __FNAME(sname, tname)(struct sname##_data* data); \
    __CTEST_STRUCT(sname, tname, _skip, &__ctest_##sname##_data, SETUP_FNAME(sname), TEARDOWN_FNAME(sname)) \
    void __FNAME(sname, tname)(struct sname##_data* data)
*/


void CTEST_LOG(char *fmt, ...);

#define CTEST(sname, tname) __CTEST_INTERNAL(sname, tname, 0)
#define CTEST_SKIP(sname, tname) __CTEST_INTERNAL(sname, tname, 1)

#define CTEST2(sname, tname) __CTEST2_INTERNAL(sname, tname, 0)
#define CTEST2_SKIP(sname, tname) __CTEST2_INTERNAL(sname, tname, 1)


void assert_str(const char* exp, const char*  real, const char* caller, int line);
#define ASSERT_STR(exp, real) assert_str(exp, real, __FILE__, __LINE__)

void assert_equal(int exp, int real, const char* caller, int line);
#define ASSERT_EQUAL(exp, real) assert_equal(exp, real, __FILE__, __LINE__)

void assert_not_equal(int exp, int real, const char* caller, int line);
#define ASSERT_NOT_EQUAL(exp, real) assert_not_equal(exp, real, __FILE__, __LINE__)

void assert_null(void* real, const char* caller, int line);
#define ASSERT_NULL(real) assert_null((void*)real, __FILE__, __LINE__)

void assert_not_null(void* real, const char* caller, int line);
#define ASSERT_NOT_NULL(real) assert_not_null(real, __FILE__, __LINE__)

void assert_true(int real, const char* caller, int line, const char * expr);
#define ASSERT_TRUE(real) assert_true(real, __FILE__, __LINE__, #real)

void assert_false(int real, const char* caller, int line);
#define ASSERT_FALSE(real) assert_false(real, __FILE__, __LINE__)

void assert_fail(const char* caller, int line);
#define ASSERT_FAIL() assert_fail(__FILE__, __LINE__)


#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#if !(defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__))
#include <unistd.h>
#endif
#include <stdint.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <dlfcn.h>
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#else
#include <sys/time.h>
#endif


#endif

