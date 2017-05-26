#--------------------------------------------------------------------------
# MathLib project
#--------------------------------------------------------------------------

PROJECT(MathLib)
FILE(GLOB MathLib_headers code/*.h)
FILE(GLOB MathLib_sources code/*.cpp)

SET(files_MathLib ${MathLib_headers} ${MathLib_sources} code/Skinning.cpp code/Skinning.h code/nvx2fileformatstructs.h)
SOURCE_GROUP("MathLib" FILES ${files_MathLib})

ADD_LIBRARY(MathLib STATIC ${files_MathLib})
TARGET_LINK_LIBRARIES(MathLib core render)
ADD_DEPENDENCIES(MathLib core render)

TARGET_INCLUDE_DIRECTORIES(MathLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/code)
