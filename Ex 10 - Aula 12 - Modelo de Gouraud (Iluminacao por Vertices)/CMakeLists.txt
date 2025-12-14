cmake_minimum_required(VERSION 3.16)
project(GouraudShading)

set(CMAKE_CXX_STANDARD 17)

# Add definition to suppress OpenGL deprecation warnings on macOS
if(APPLE)
    add_definitions(-DGL_SILENCE_DEPRECATION)
endif()

# Find OpenGL
find_package(OpenGL REQUIRED)

# Explicitly find FreeGLUT on macOS
if(APPLE)
    # Look for FreeGLUT in Homebrew location
    find_path(FREEGLUT_INCLUDE_DIR GL/freeglut.h 
              HINTS /opt/homebrew/include)
    find_library(FREEGLUT_LIBRARY glut
                 HINTS /opt/homebrew/lib)
    
    if(FREEGLUT_INCLUDE_DIR AND FREEGLUT_LIBRARY)
        message(STATUS "Found FreeGLUT: ${FREEGLUT_LIBRARY}")
        set(GLUT_INCLUDE_DIRS ${FREEGLUT_INCLUDE_DIR})
        set(GLUT_LIBRARIES ${FREEGLUT_LIBRARY})
    else()
        # Fall back to system GLUT
        find_package(GLUT REQUIRED)
        message(STATUS "Using system GLUT: ${GLUT_LIBRARY}")
    endif()
else()
    find_package(GLUT REQUIRED)
endif()

# Include directories
include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLUT_INCLUDE_DIRS})

# Add executable
add_executable(gouraud_shading
    main.cpp
    mesh.cpp
    mesh.h
    vector3d.h
)

# Link libraries
target_link_libraries(gouraud_shading ${OPENGL_LIBRARIES})

if(APPLE AND FREEGLUT_LIBRARY)
    target_link_libraries(gouraud_shading ${GLUT_LIBRARIES})
else()
    target_link_libraries(gouraud_shading ${GLUT_LIBRARY})
endif()

# macOS specific settings
if(APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    target_link_libraries(gouraud_shading ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
endif()
