cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(CMAKE_BUILD_TYPE "Release")

function(LibNameFromPath libPath libName)
    get_filename_component(libFileName "${libPath}" NAME_WE)
    string(REPLACE "lib" "" candidate "${libFileName}")
    set(${libName} "${candidate}" PARENT_SCOPE)
endfunction()

#
# Project definitions and minimum system requirement, that is having wxWidgets installed on the system.
#
project(Cross-Platfrom-GUI-Programming-WxWidgets LANGUAGES CXX C)
if(DEFINED ENV{wxWidgets_CONFIGURATION})
    set(wxWidgets_CONFIGURATION $ENV{wxWidgets_CONFIGURATION})
endif()
if(DEFINED ENV{wxWidgets_LIB_DIR})
    set(wxWidgets_LIB_DIR $ENV{wxWidgets_LIB_DIR})
endif()
if(DEFINED ENV{wxWidgets_ROOT_DIR})
    set(wxWidgets_ROOT_DIR $ENV{wxWidgets_ROOT_DIR})
endif()
find_package(wxWidgets REQUIRED core base OPTIONAL_COMPONENTS adv aui gl html media net propgrid qa ribbon richtext stc xrc)
if(wxWidgets_FOUND)
    include(${wxWidgets_USE_FILE})
    message(STATUS "wxWidgets use file: ${wxWidgets_USE_FILE}")
    message(STATUS "wxWidgets found: ${wxWidgets_VERSION_STRING}")
    message(STATUS "wxWidgets include directories: ${wxWidgets_INCLUDE_DIRS}")
    message(STATUS "wxWidgets library directories: ${wxWidgets_LIBRARY_DIRS}")
    message(STATUS "wxWidgets libraries: ${wxWidgets_LIBRARIES}")
    if(MINGW)
        set(WX_base_LibName "")
        set(WX_core_LibName "")
        set(WX_adv_LibName "")
        set(WX_aui_LibName "")
        set(WX_gl_LibName "")
        set(WX_html_LibName "")
        set(WX_media_LibName "")
        set(WX_net_LibName "")
        set(WX_propgrid_LibName "")
        set(WX_qa_LibName "")
        set(WX_ribbon_LibName "")
        set(WX_richtext_LibName "")
        set(WX_stc_LibName "")
        set(WX_xrc_LibName "")
        if(NOT "${WX_base}" STREQUAL "")
            LibNameFromPath(${WX_base} WX_base_LibName)
            message(STATUS "WX_base library name: ${WX_base_LibName}")
        endif()
        if(NOT "${WX_core}" STREQUAL "")
            LibNameFromPath(${WX_core} WX_core_LibName)
            message(STATUS "WX_core library name: ${WX_core_LibName}")
        endif()
        if(NOT "${WX_adv}" STREQUAL "")
            LibNameFromPath(${WX_adv} WX_adv_LibName)
            message(STATUS "WX_adv library name: ${WX_adv_LibName}")
        endif()
        if(NOT "${WX_aui}" STREQUAL "")
            LibNameFromPath(${WX_aui} WX_aui_LibName)
            message(STATUS "WX_aui library name: ${WX_aui_LibName}")
        endif()
        if(NOT "${WX_gl}" STREQUAL "")
            LibNameFromPath(${WX_gl} WX_gl_LibName)
            message(STATUS "WX_gl library name: ${WX_gl_LibName}")
        endif()
        if(NOT "${WX_html}" STREQUAL "")
            LibNameFromPath(${WX_html} WX_html_LibName)
            message(STATUS "WX_html library name: ${WX_html_LibName}")
        endif()
        if(NOT "${WX_media}" STREQUAL "")
            LibNameFromPath(${WX_media} WX_media_LibName)
            message(STATUS "WX_media library name: ${WX_media_LibName}")
        endif()
        if(NOT "${WX_net}" STREQUAL "")
            LibNameFromPath(${WX_net} WX_net_LibName)
            message(STATUS "WX_net library name: ${WX_net_LibName}")
        endif()
        if(NOT "${WX_propgrid}" STREQUAL "")
            LibNameFromPath(${WX_propgrid} WX_propgrid_LibName)
            message(STATUS "WX_propgrid library name: ${WX_propgrid_LibName}")
        endif()
        if(NOT "${WX_qa}" STREQUAL "")
            LibNameFromPath(${WX_qa} WX_qa_LibName)
            message(STATUS "WX_qa library name: ${WX_qa_LibName}")
        endif()
        if(NOT "${WX_ribbon}" STREQUAL "")
            LibNameFromPath(${WX_ribbon} WX_ribbon_LibName)
            message(STATUS "WX_ribbon library name: ${WX_ribbon_LibName}")
        endif()
        if(NOT "${WX_richtext}" STREQUAL "")
            LibNameFromPath(${WX_richtext} WX_richtext_LibName)
            message(STATUS "WX_richtext library name: ${WX_richtext_LibName}")
        endif()
        if(NOT "${WX_stc}" STREQUAL "")
            LibNameFromPath(${WX_stc} WX_stc_LibName)
            message(STATUS "WX_stc library name: ${WX_stc_LibName}")
        endif()
        if(NOT "${WX_xrc}" STREQUAL "")
            LibNameFromPath(${WX_xrc} WX_xrc_LibName)
            message(STATUS "WX_xrc library name: ${WX_xrc_LibName}")
        endif()
    endif()
else()
    message(FATAL_ERROR "wxWidgets not found. Please set wxWidgets_ROOT_DIR or install wxWidgets.")
endif()

#
# Header Section for notifying the old sources and checking compiler versions
#

message("The sources of the books is old.. see: https://forums.wxwidgets.org/viewtopic.php?t=50926")
message(STATUS "Detected generator: ${CMAKE_GENERATOR}")
message(STATUS "Detected c compiler: ${CMAKE_C_COMPILER}")
message(STATUS "Detected c++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Detected resource compiler: ${CMAKE_RC_COMPILER}")

if(MINGW)
    message(STATUS "Using MinGW")
elseif(MSVC)
    message(STATUS "Using Microsoft Visual Studio")
endif()

#
# Global configurations for all the compiled targets
#

# For certain IDEs like Visual Studio and Xcode supports organizing the targets in internal folders.
# (Be aware this is only in IDE not in file structures in e.g. file explorer.)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Global link directories to wxWidgets. This applies to all compiled targets in the project; fine for this scope.
if(NOT "${wxWidgets_LIBRARY_DIRS}" STREQUAL "")
    link_directories(${wxWidgets_LIBRARY_DIRS})
endif()

# Global _UNICODE definition - wxWidgets requires this
add_compile_definitions(_UNICODE )

include_directories(${wxWidgets_INCLUDE_DIRS})

#
# Main function "CreateExe" to create the targets of the source code organized in "item" (Path)
#

function(CreateExe item )
    string(REPLACE "/" ";" pathName ${item})
    list(LENGTH pathName PathLength)
    if((CMAKE_GENERATOR MATCHES "Visual Studio" OR CMAKE_GENERATOR MATCHES "Xcode") AND PathLength GREATER 1)
        set(FOLDER_FLAG TRUE)
    endif()
    if(FOLDER_FLAG)
        list(POP_FRONT pathName  exePath)
        list(JOIN pathName "-" exeName)
    else()
        string(REPLACE "/" "-" exeName ${item})
    endif()
    message(STATUS "Creating build for: ${exeName}")
    if(WIN32)
        file(GLOB SRC_FILES ${item}/*.cpp ${item}/*.h ${item}/*.rc  ${item}/*.xpm)
    else()
        file(GLOB SRC_FILES ${item}/*.cpp ${item}/*.h ${item}/*.xpm)
    endif()
    if(WIN32)
        add_executable(${exeName} WIN32 ${SRC_FILES})
    else()
        add_executable(${exeName} ${SRC_FILES})
    endif()
    target_include_directories(${exeName} PUBLIC ${item})
    if(FOLDER_FLAG)
        set_target_properties(${exeName} PROPERTIES FOLDER ${exePath})
    endif()
    target_link_libraries(${exeName} PRIVATE ${wxWidgets_LIBRARIES})
    if(WIN32)    
        if(NOT "${WX_base_LibName}" STREQUAL "")
            target_link_libraries(${exeName} PRIVATE ${WX_base_LibName})
        endif()
        if(NOT "${WX_core_LibName}" STREQUAL "")
            target_link_libraries(${exeName} PRIVATE ${WX_core_LibName})
        endif()
        if(NOT "${WX_adv_LibName}" STREQUAL "")
            target_link_libraries(${exeName} PRIVATE ${WX_adv_LibName})
        endif()
        if(NOT "${WX_aui_LibName}" STREQUAL "")
            target_link_libraries(${exeName} PRIVATE ${WX_aui_LibName})
        endif()
        if(NOT "${WX_gl_LibName}" STREQUAL "")
            target_link_libraries(${exeName} PRIVATE ${WX_gl_LibName})
        endif()
        if(NOT "${WX_html_LibName}" STREQUAL "")
            target_link_libraries(${exeName} PRIVATE ${WX_html_LibName})
        endif()
        if(NOT "${WX_media_LibName}" STREQUAL "")
            target_link_libraries(${exeName} PRIVATE ${WX_media_LibName})
        endif()
        if(NOT "${WX_net_LibName}" STREQUAL "")
            target_link_libraries(${exeName} PRIVATE ${WX_net_LibName})
        endif()
        if(NOT "${WX_propgrid_LibName}" STREQUAL "")
            target_link_libraries(${exeName} PRIVATE ${WX_propgrid_LibName})
        endif()
        if(NOT "${WX_qa_LibName}" STREQUAL "")
            target_link_libraries(${exeName} PRIVATE ${WX_qa_LibName})
        endif()
        if(NOT "${WX_ribbon_LibName}" STREQUAL "")
            target_link_libraries(${exeName} PRIVATE ${WX_ribbon_LibName})
        endif()
        if(NOT "${WX_richtext_LibName}" STREQUAL "")
            target_link_libraries(${exeName} PRIVATE ${WX_richtext_LibName})
        endif()
        if(NOT "${WX_stc_LibName}" STREQUAL "")
            target_link_libraries(${exeName} PRIVATE ${WX_stc_LibName})
        endif()
        if(NOT "${WX_xrc_LibName}" STREQUAL "")
            target_link_libraries(${exeName} PRIVATE ${WX_xrc_LibName})
        endif()
    endif()
endfunction()

#
# For-loop to create all the compiled targets, except libraries and the Riffle app.
#

foreach(
    item
    "chap02" 
    "chap09"
    "chap10"
    "chap11"
    "chap16"
    "chap17"
    "chap18/SocketEvents/Client"
    "chap18/SocketEvents/Server"
    "chap18/SocketStreams/Client"
    "chap18/SocketStreams/Server"
    "chap19/doodle")
    CreateExe(${item})
endforeach()
        
file(GLOB Chap03_SRC  chap03/* )
add_library(chap03 ${Chap03_SRC})

if(MINGW)
    add_custom_command(
        TARGET "chap16" POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/chap16/flag.png "${CMAKE_BINARY_DIR}/")
    add_custom_command(
        TARGET "chap16" POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${CMAKE_CURRENT_SOURCE_DIR}/chap16/de" "${CMAKE_BINARY_DIR}/de")
    add_custom_command(
        TARGET "chap16" POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${CMAKE_CURRENT_SOURCE_DIR}/chap16/fr" "${CMAKE_BINARY_DIR}/fr")
    add_custom_command(
        TARGET "chap18-SocketStreams-Server" POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${CMAKE_CURRENT_SOURCE_DIR}/chap18/SocketStreams/ReadFrom.jpg "${CMAKE_BINARY_DIR}/")
else()
    add_custom_command(
        TARGET "chap16" POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/chap16/flag.png "${CMAKE_BINARY_DIR}/$<CONFIG>/")
    add_custom_command(
        TARGET "chap16" POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${CMAKE_CURRENT_SOURCE_DIR}/chap16/de" "${CMAKE_BINARY_DIR}/$<CONFIG>/de")
    add_custom_command(
        TARGET "chap16" POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${CMAKE_CURRENT_SOURCE_DIR}/chap16/fr" "${CMAKE_BINARY_DIR}/$<CONFIG>/fr")
    if(CMAKE_GENERATOR MATCHES "MinGW Makefiles")
        add_custom_command(
            TARGET "chap18-SocketStreams-Server" POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${CMAKE_CURRENT_SOURCE_DIR}/chap18/SocketStreams/ReadFrom.jpg "${CMAKE_BINARY_DIR}/$<CONFIG>/")
    elseif (CMAKE_GENERATOR MATCHES "Visual Studio")
        add_custom_command(
            TARGET "SocketStreams-Server" POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${CMAKE_CURRENT_SOURCE_DIR}/chap18/SocketStreams/ReadFrom.jpg "${CMAKE_BINARY_DIR}/$<CONFIG>/")
    endif()
endif()

# Chapter 20 builds of libraries 
if(CMAKE_GENERATOR MATCHES "Visual Studio" OR CMAKE_GENERATOR MATCHES "Xcode")
    add_library(findapppath chap20/findapppath/findapppath.cpp)
    set_target_properties(findapppath PROPERTIES FOLDER chap20)
    add_library(launch chap20/launch/launch.cpp chap20/launch/launch.h)
    set_target_properties(launch PROPERTIES FOLDER chap20)
    add_library(valconfig  chap20/valconfig/valconfig.cpp chap20/valconfig/valconfig.h)
    set_target_properties(valconfig PROPERTIES FOLDER chap20)
else()
    add_library(chap20-findapppath chap20/findapppath/findapppath.cpp)
    add_library(chap20-launch chap20/launch/launch.cpp chap20/launch/launch.h)
    add_library(chap20-valconfig chap20/valconfig/valconfig.cpp chap20/valconfig/valconfig.h)
endif()
