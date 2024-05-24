cmake_minimum_required(VERSION 3.24)
project(LangulusModInputSDL
    VERSION         1.0.0
    DESCRIPTION     "Langulus plug-in module for handling raw input using the SDL library"
    HOMEPAGE_URL    https://langulus.com
)

# Configure SDL library, it will be statically built inside this module			
set(SDL_DIRECTX OFF CACHE BOOL "" FORCE)
set(SDL_DISKAUDIO OFF CACHE BOOL "" FORCE)
set(SDL_DUMMYAUDIO OFF CACHE BOOL "" FORCE)
set(SDL_DUMMYCAMERA OFF CACHE BOOL "" FORCE)
set(SDL_DUMMYVIDEO OFF CACHE BOOL "" FORCE)
set(SDL_OFFSCREEN OFF CACHE BOOL "" FORCE)
set(SDL_OPENGL OFF CACHE BOOL "" FORCE)
set(SDL_OPENGLES OFF CACHE BOOL "" FORCE)
set(SDL_RENDER_D3D OFF CACHE BOOL "" FORCE)
set(SDL_RENDER_VULKAN OFF CACHE BOOL "" FORCE)
set(SDL_VULKAN OFF CACHE BOOL "" FORCE)
set(SDL_WASAPI OFF CACHE BOOL "" FORCE)
fetch_external_module(
    SDL
    GIT_REPOSITORY  https://github.com/libsdl-org/SDL.git
    GIT_TAG         main
)

file(GLOB_RECURSE
    LANGULUS_MOD_INPUTSDL_SOURCES 
    LIST_DIRECTORIES FALSE CONFIGURE_DEPENDS
    source/*.cpp
)

# Build the module                                                              
add_library(LangulusModInputSDL SHARED ${LANGULUS_MOD_INPUTSDL_SOURCES})

target_link_libraries(LangulusModInputSDL 
    PRIVATE     Langulus
				SDL3-static
)

if(LANGULUS_TESTING)
    enable_testing()
    add_subdirectory(test)
endif()