PROJECT(Protocol CXX)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

SET(CMAKE_CXX_COMPILER "g++")


INCLUDE_DIRECTORIES(../GameCommon/CommonDefine)
INCLUDE_DIRECTORIES(../GameCommon/CommonDefine/String)
INCLUDE_DIRECTORIES(../GameCommon/CommonDefine/TimeController)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/Lib)
SET(SOURCE_DEFULT ./GYLogin.cpp ./GYLogout.cpp ./GYProtocolDefine.cpp ./GYSCTest.cpp ./GYTestPacket.cpp)
SET(PROTOCOLTOOL )

ADD_LIBRARY(Protocol ${SOURCE_DEFULT} ${PROTOCOLTOOL} )
TARGET_LINK_LIBRARIES(Protocol GameCommon)
MESSAGE(STATUS ${CMAKE_SYSTEM})
ADD_DEFINITIONS(-D_DEBUG)
IF(WIN32)
ADD_DEFINITIONS(-DWIN32)
ELSEIF(UNIX)
ADD_DEFINITIONS(-DLINUX64 -Wall -std=gnu++11)
ENDIF(WIN32)

