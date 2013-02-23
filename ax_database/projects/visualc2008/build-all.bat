call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86

set S=axSQLite3.vcproj
MSBuild	%S% /p:Configuration=Release /p:Platform=Win32 
MSBuild	%S% /p:Configuration=Release /p:Platform=x64 
MSBuild	%S% /p:Configuration=Debug /p:Platform=Win32 
MSBuild	%S% /p:Configuration=Debug /p:Platform=x64 

set S=axMySQL.vcproj
MSBuild	%S% /p:Configuration=Release /p:Platform=Win32 
MSBuild	%S% /p:Configuration=Release /p:Platform=x64 
MSBuild	%S% /p:Configuration=Debug /p:Platform=Win32 
MSBuild	%S% /p:Configuration=Debug /p:Platform=x64 

set S=axPostgreSQL.vcproj
MSBuild	%S% /p:Configuration=Release /p:Platform=Win32 
MSBuild	%S% /p:Configuration=Release /p:Platform=x64 
MSBuild	%S% /p:Configuration=Debug /p:Platform=Win32 
MSBuild	%S% /p:Configuration=Debug /p:Platform=x64 

set S=axOracle.vcproj
MSBuild	%S% /p:Configuration=Release /p:Platform=Win32 
MSBuild	%S% /p:Configuration=Release /p:Platform=x64 
MSBuild	%S% /p:Configuration=Debug /p:Platform=Win32 
MSBuild	%S% /p:Configuration=Debug /p:Platform=x64 

pause

