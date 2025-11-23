cmake_minimum_required(VERSION 3.14)
project(AStarPathfinding)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenGL (system library)
find_package(OpenGL REQUIRED)

# Use FetchContent to download dependencies
include(FetchContent)

# Download GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.9
    GIT_SHALLOW TRUE
)

# Download Dear ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.90.1
    GIT_SHALLOW TRUE
)

# Download stb (for stb_image.h)
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
    GIT_SHALLOW TRUE
)

# Don't build GLFW tests, examples, and docs
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glfw imgui stb)

# ImGui source files
set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Project source files
set(PROJECT_SOURCES
    src/main.cpp
    src/astar.cpp
    src/renderer.cpp
    src/application.cpp
)

# Project header files
set(PROJECT_HEADERS
    src/astar.h
    src/fixinclude.h
    src/renderer.h
    src/application.h
)

# Create executable
add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${IMGUI_SOURCES}
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${stb_SOURCE_DIR}
    ${OPENGL_INCLUDE_DIR}
)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    glfw
    ${OPENGL_LIBRARIES}
    ${CMAKE_DL_LIBS}
)

# Copy images to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Pathfinding/images
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

# Platform-specific settings
if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} GL)
elseif(APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")
elseif(WIN32)
    target_link_libraries(${PROJECT_NAME} opengl32)
endif()
