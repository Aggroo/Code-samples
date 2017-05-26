#--------------------------------------------------------------------------
# PhysicsLab project
#--------------------------------------------------------------------------

PROJECT(PhysicsLab)
FILE(GLOB PhysicsLab_headers code/*.h)
FILE(GLOB PhysicsLab_sourcescpp code/*.cpp)
FILE(GLOB PhysicsLab_sources code/*.cc)

SET(files_PhysicsLab ${PhysicsLab_headers} ${PhysicsLab_sources} ${PhysicsLab_sourcescpp})
SOURCE_GROUP("PhysicsLab" FILES ${files_PhysicsLab})

ADD_EXECUTABLE(PhysicsLab ${files_PhysicsLab})
TARGET_LINK_LIBRARIES(PhysicsLab core render MathLib PhysLib)
ADD_DEPENDENCIES(PhysicsLab core render MathLib PhysLib)