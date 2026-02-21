# Cmake is a mistake to humanity

# cmake -S . -B build -G "Visual Studio 17 2022" -A x64

cmake_minimum_required(VERSION 3.20)
project(GreyHeavens)

# set the output directory for built objects.
# This makes sure that the dynamic library goes into the build directory automatically.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# Variables
set(VENDOR_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor")
file(GLOB_RECURSE SRC_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
core/source/* core/include/*
)

include_directories(core/include)

# Third Parties
add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)
include_directories(vendor/stb_image)


set(PROJECT_FILES ${SRC_FILES})
add_executable(${PROJECT_NAME} WIN32 ${PROJECT_FILES} core/main.cpp)
# add_executable(${PROJECT_NAME} WIN32 core/main.cpp)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# Glad
add_library(glad
  STATIC
    ${VENDOR_DIR}/glad/src/glad.c
)

target_include_directories(glad
  PRIVATE
    ${VENDOR_DIR}/glad/include
)

target_include_directories(${PROJECT_NAME}
  PRIVATE
    ${VENDOR_DIR}/glad/include
)

# Shaders
file(GLOB_RECURSE GLSL_SOURCE_FILES
    "${PROJECT_SOURCE_DIR}/resource/shaders/*.frag"
    "${PROJECT_SOURCE_DIR}/resource/shaders/*.vert"
    "${PROJECT_SOURCE_DIR}/resource/shaders/*.comp"
    )

# Add shader files to IDE
target_sources(${PROJECT_NAME} PRIVATE ${GLSL_SOURCE_FILES})
source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/resource/shaders"
    PREFIX "Shaders"
    FILES ${GLSL_SOURCE_FILES}
)

# Copy assets files to .exe directory
# Might need a rework soon, might be worth to copy the .exe file into the asset folder
# or use symlink. The point is to not make copy of asset folder
set (destination "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>/assets")
add_custom_command(
     TARGET ${PROJECT_NAME} POST_BUILD
     COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${CMAKE_CURRENT_SOURCE_DIR}/resource" "${destination}"
)

# glm
include(FetchContent)
FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	0af55ccecd98d4e5a8d1fad7de25ba429d60e863 #refs/tags/1.0.1
)
FetchContent_MakeAvailable(glm)

# Linking
target_link_libraries(${PROJECT_NAME} PRIVATE 
    SDL3::SDL3
#    OpenGL::OpenGL
    glad
    glm::glm
    ${CMAKE_DL_LIBS} # Needed for glad - https://stackoverflow.com/a/56842079/2394163
)