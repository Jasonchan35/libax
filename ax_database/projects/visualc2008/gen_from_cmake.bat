mkdir Win32
cd Win32
cmake -G "Visual Studio 9 2008" ../../cmake/axSQLite3
cd ..


mkdir x64
cd x64
cmake -G "Visual Studio 9 2008 Win64" ../../cmake/axSQLite3
cd ..


pause