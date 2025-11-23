cmake_minimum_required(VERSION 3.10)
project(AStarPathfinding)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find GLFW
find_package(glfw3 REQUIRED)

# ImGui source files
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui)
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
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
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
