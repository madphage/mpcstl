.PHONY: all clean test test_ctest

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
CL_FLAGS := /nologo /Wall /WX /wd4710 /wd4668 /wd4100 /Itestlib /Isrc
# CL_FLAGS := /nologo /wd4710 /Itestlib
LINK_FLAGS := /nologo

all: test_mpcstl.exe test_ctest.exe

test: test_mpcstl.exe
	test_mpcstl

test_ctest: test_ctest.exe
	test_ctest

clean:
	rm test_ctest.exe test/test_ctest.obj

# $@ is the target of the rule
# $^ is all inputs
# $< is the first input
test_ctest.exe: testlib/ctest.obj testlib/example_suite1.obj testlib/example_suite2.obj
	link $(LINK_FLAGS) /out:$@ $^

test_mpcstl.exe: testlib/ctest.obj test/vector.obj
	link $(LINK_FLAGS) /out:$@ $^

# make obj files from their corresponding c files
# /Tc<file> says to compile the next file as c, we need it because we're using
# .E to indicate a preprocessed and pretty-printed c file
%.obj: %.E
	cl $(CL_FLAGS) /c /Fo:$@ /Tc$<

# don't delete the .E files that we're using
.PRECIOUS: %.E

# make preprocessed files from their corresponding c files
# -n is don't keep .orig, just modify in place without making a backup
%.E: %.c
	cl $(CL_FLAGS) /P /Fi:$@ $<
	astyle --style=google -n $@

