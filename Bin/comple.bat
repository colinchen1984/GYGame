#!/bin/bash
cd ../Src
chmod +x updatemakefile.bat
./updatemakefile.bat
cd ../Bin
cd ./GameCommon
rm -rf *
cmake ../../Src/GameCommon/CommonDefine
make
cd ../NetWork
rm -rf * 
cmake ../../Src/GameCommon/NetWork
make
cd ../Thread
rm -rf *
cmake ../../Src/GameCommon/Thread
make
cd ../Table
rm -rf *
cmake ../../Src/GameCommon/Table
make
cd ../Protocol
cmake ../../Src/Protocol
make

cd ../test
cmake ../../Src/GameCommon/test
make

cd ../Gateway
cmake ../../Src/Gateway
make

cd ../Logic
cmake ../../Src/Logic
make
