.PHONY: all clean

# /Wall /WX is enable all warnings and treat them as errors
#  /wd<num> is disable a warning
#  Disabled Warnings:
#  -------- /wd4710:
#  warning C4710: 'int printf(const char *const ,...)': function not inlined
#  C:\Program Files (x86)\Windows Kits\10\include\10.0.10586.0\ucrt\stdio.h(944): note: see declaration of 'printf'
#  -------- /wd4668
#  testlib\ctest.h(46): warning C4668: '__linux__' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
#  -------- /wd4100
#  testlib\ctest.h(299): warning C4100: 't': unreferenced formal parameter

# TODO: make this file relative
CL_FLAGS := /nologo /Wall /WX /wd4710 /wd4668 /wd4100 /Itestlib
# CL_FLAGS := /nologo /wd4710 /Itestlib
LINK_FLAGS := /nologo

all: test_ctest.exe

clean:
	rm test_ctest.exe test/test_ctest.obj

test_ctest.exe: test/test_ctest.obj
	link $(LINK_FLAGS) test/test_ctest.obj /out:test_ctest.exe

test/test_ctest.obj: test/test_ctest.c
	cl $(CL_FLAGS) /c /Fo:test/test_ctest.obj test/test_ctest.c

