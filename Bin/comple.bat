#!/bin/bash
cd ../Src
chmod +x updatemakefile.bat
./updatemakefile.bat
cd ../Bin
cd ./GameCommon
rm -rf *
cmake ../../Src/GameCommon/CommonDefine
make clean
make
cd ../NetWork
rm -rf * 
cmake ../../Src/GameCommon/NetWork
make clean
make
cd ../Thread
rm -rf *
cmake ../../Src/GameCommon/Thread
make clean
make
cd ../Table
rm -rf *
cmake ../../Src/GameCommon/Table
make clean
make
cd ../Protocol
cmake ../../Src/Protocol
make clean
make

cd ../test
cmake ../../Src/GameCommon/test
make clean
make

cd ../Gateway
cmake ../../Src/Gateway
make clean
make

cd ../Logic
cmake ../../Src/Logic
make clean
make
