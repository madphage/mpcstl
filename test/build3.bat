SET DIR=%~dp0
:: /Wall /WX is enable all warnings and treat them as errors
:: /wd<num> is disable a warning
:: Disabled Warnings:
:: -------- /wd4710:
:: data_seg.c(6): warning C4710: 'int printf(const char *const ,...)': function not inlined
:: C:\Program Files (x86)\Windows Kits\10\include\10.0.10586.0\ucrt\stdio.h(944): note: see declaration of 'printf'
:: cl /nologo /Wall /WX /wd4710 data_seg.c /c /I%DIR%..\testlib /Fo:data_seg.obj
cl /nologo /wd4710 data_seg.c /c /I%DIR%..\testlib /Fo:data_seg.obj
link data_seg.obj /out:data_seg.exe
