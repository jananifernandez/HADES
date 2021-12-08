
message(STATUS "Configuring ehades library...")

project("ehades" LANGUAGES C)
add_library(${PROJECT_NAME} STATIC)

# Link with saf
target_link_libraries(${PROJECT_NAME} PRIVATE saf)

# Source files
target_sources(${PROJECT_NAME} 
PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/ehades/src/ehades_internal.c
    ${CMAKE_CURRENT_SOURCE_DIR}/ehades/src/ehades_internal.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ehades/src/ehades.c
)

# enable compiler warnings
if(UNIX)
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

# Include directory
target_include_directories(${PROJECT_NAME}
PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/ehades/include/>   
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)


