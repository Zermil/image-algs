@echo off

REM Change this to your visual studio's 'vcvars64.bat' script path
set MSVC_PATH="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build"
set CXXFLAGS=/std:c++17 /EHsc /W4 /MT /WX /FC /wd4996 /wd4201 /nologo %*

call %MSVC_PATH%\vcvars64.bat

pushd %~dp0
if not exist .\build mkdir build

cl %CXXFLAGS% ppm_saver.cpp /Fo:build\ /Fe:build\ppm_saver.exe
cl %CXXFLAGS% huffman.cpp /Fo:build\ /Fe:build\huffman.exe
cl %CXXFLAGS% png_parser.cpp /Fo:build\ /Fe:build\png_parser.exe
cl %CXXFLAGS% dct.cpp /Fo:build\ /Fe:build\dct.exe

REM Clean the build folder
cd .\build
del *.obj
cd ..

popd