#--------------------------------------------------------------------------
# CGLab project
#--------------------------------------------------------------------------

PROJECT(CGLab)
FILE(GLOB CGLab_headers code/*.h)
FILE(GLOB CGLab_sources code/*.cc)


SET(files_CGLab ${CGLab_headers} ${CGLab_sources})
SOURCE_GROUP("CGLab" FILES ${files_CGLab})

ADD_EXECUTABLE(CGLab ${files_CGLab})
TARGET_LINK_LIBRARIES(CGLab core render MathLib)
ADD_DEPENDENCIES(CGLab core render)