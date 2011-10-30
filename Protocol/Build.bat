CALL %comspec% /k ""D:\Program Files\Microsoft Visual Studio 10.0\VC\vcvarsall.bat"" x86
CALL devenv ./Protocol.sln /build "Debug|Win32"