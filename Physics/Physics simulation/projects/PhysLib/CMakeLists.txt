#--------------------------------------------------------------------------
# MathLib project
#--------------------------------------------------------------------------

PROJECT(PhysLib)
FILE(GLOB PhysLib_headers code/*.h)
FILE(GLOB PhysLib_sources code/*.cpp)

SET(files_PhysLib ${PhysLib_headers} ${PhysLib_sources} code/Plane.cpp code/Plane.h code/Ray.cpp code/Ray.h code/OABB.cpp code/OABB.h code/RigidBody.cpp code/RigidBody.h code/PhysicsNode.cpp code/PhysicsNode.h code/PhysicsDevice.cpp code/PhysicsDevice.h)
SOURCE_GROUP("PhysLib" FILES ${files_PhysLib})

ADD_LIBRARY(PhysLib STATIC ${files_PhysLib})
TARGET_LINK_LIBRARIES(PhysLib core render MathLib)
ADD_DEPENDENCIES(PhysLib core render MathLib)

TARGET_INCLUDE_DIRECTORIES(PhysLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/code)
