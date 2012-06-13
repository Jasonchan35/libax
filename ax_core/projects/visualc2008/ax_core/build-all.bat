call "C:\Program Files (x86)\Microsoft Visual Studio 9.0\VC\vcvarsall.bat" x86


set S=ax_core.sln
MSBuild	%S% /p:Configuration=Release /p:Platform=Win32 
MSBuild	%S% /p:Configuration=Release /p:Platform=x64 
MSBuild	%S% /p:Configuration=Debug /p:Platform=Win32 
MSBuild	%S% /p:Configuration=Debug /p:Platform=x64 

pause

