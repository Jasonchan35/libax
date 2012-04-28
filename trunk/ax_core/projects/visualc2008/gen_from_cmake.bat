mkdir Win32
cd Win32
cmake -G "Visual Studio 9 2008" ../../cmake/ax_core
cd ..


mkdir x64
cd x64
cmake -G "Visual Studio 9 x64" ../../cmake/ax_core
cd ..


pause