CALL "D:\Program Files\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" x86
cd ../GameCommon
CALL devenv ./GameCommon.sln /build "Debug|Win32"
cd ../Protocol
CALL devenv ./Protocol.sln /build "Debug|Win32"
cd ../Gateway
CALL devenv ./Gateway.sln /build "Debug|Win32"
cd ../Logic
CALL devenv ./Logic.sln /build "Debug|Win32"
cd ../Bin