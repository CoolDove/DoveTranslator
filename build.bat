@echo off
set PATH_ROOT=%CD%
set PATH_INT_BIN=%PATH_ROOT%\bin_int
set OUTPUT_NAME=dt

mkdir %PATH_ROOT%\bin_int
mkdir %PATH_ROOT%\bin\win

cd bin_int

echo compiling...

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

echo copydll...

copy ..\DoveTranslator\depen\win\libcrypto-1_1-x64.dll ..\bin\win\libcrypto-1_1-x64.dll
copy ..\DoveTranslator\depen\win\libcurl-x64.dll ..\bin\win\libcurl-x64.dll

cd %PATH_ROOT%

echo done!!!

pause
