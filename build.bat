@echo building......
set PATH_ROOT=%CD%
set PATH_INT_BIN=%PATH_ROOT%\bin_int
set OUTPUT_NAME=dt

mkdir %PATH_ROOT%\bin\win
cd bin_int

cl /EHsc /nologo ^
/D_UNICODE /DWIN ^
%PATH_ROOT%\DoveTranslator\src\*.cpp ^
%PATH_ROOT%\DoveTranslator\vendor\src\json\*.cpp ^
/Fe:%PATH_ROOT%\bin\win\dt.exe ^
/I %PATH_ROOT%\DoveTranslator\vendor\include ^
/I %PATH_ROOT%\DoveTranslator\depen\win\Curl\x64\include ^
/I %PATH_ROOT%\DoveTranslator\depen\win\OpenSSL\Win64\include ^
/link ^
%PATH_ROOT%\DoveTranslator\depen\win\Curl\x64\lib\libcurl.dll.lib ^
%PATH_ROOT%\DoveTranslator\depen\win\OpenSSL\Win64\lib\*.lib ^
KERNEL32.lib Shell32.lib

cd %PATH_ROOT%