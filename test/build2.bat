SET DIR=%~dp0
:: /MD is link against msvcrt.dll
@cl /nologo /I%DIR%..\testlib test_ctest.c
:: link /MD test_vector.obj /out:test_vector.exe
