#!/bin/bash
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
cd ../Gateway
cmake ../../Src/Gateway
make
