.PHONY: all

# /Wall /WX is enable all warnings and treat them as errors
#  /wd<num> is disable a warning
#  Disabled Warnings:
#  -------- /wd4710:
#  warning C4710: 'int printf(const char *const ,...)': function not inlined
#  C:\Program Files (x86)\Windows Kits\10\include\10.0.10586.0\ucrt\stdio.h(944): note: see declaration of 'printf'
#  cl /nologo /Wall /WX /wd4710 data_seg.c /c /I%DIR%..\testlib /Fo:data_seg.obj

# TODO: make this file relative
# CL_FLAGS := /nologo /Wall /WX /wd4710 /Itestlib
CL_FLAGS := /nologo /wd4710 /Itestlib
LINK_FLAGS := /nologo

all: test_ctest.exe

test_ctest.exe: test/test_ctest.obj
	link $(LINK_FLAGS) test/test_ctest.obj /out:test_ctest.exe

test/test_ctest.obj: test/test_ctest.c
	cl $(CL_FLAGS) /c /Fo:test/test_ctest.obj test/test_ctest.c

