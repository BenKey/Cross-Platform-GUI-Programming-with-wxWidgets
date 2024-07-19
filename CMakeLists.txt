cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(Cross-Platfrom-GUI-Programming-WxWidgets LANGUAGES CXX C)

find_package(wxWidgets REQUIRED)

message("The sources of the books is old.. see: https://forums.wxwidgets.org/viewtopic.php?t=50926")

#set (WXWIDGETS_DIR $ENV{WXWIDGETS_DIR} CACHE PATH "wxWidgets binary directory.")
set (WXWIDGETS_DIR ${wxWidgets_ROOT_DIR} CACHE PATH "wxWidgets binary directory.")


#if(WIN32)
#   message("https://cmake.org/cmake/help/latest/variable/WIN32.html?highlight=win32")
#elseif(APPLE)
#   message("https://cmake.org/cmake/help/latest/variable/APPLE.html?highlight=apple")
#elseif(UNIX)
#    message("https://cmake.org/cmake/help/latest/variable/UNIX.html?highlight=unix")
#endif()

#target_link_libraries(
#target_link_directories(02_Minimal ${wxWidgets_LIB_DIR})
#target_link_libraries(02_Minimal ${wxWidgets_LIBRARIES})


if (MSVC)
	set (WXWIDGETS_LIBS_DIR ${WXWIDGETS_DIR}/lib/vc_x64_lib)
	set (WXWIDGETS_PLATFORM_INCLUDE_DIR ${WXWIDGETS_DIR}/include 
					    ${WXWIDGETS_DIR}/include/msvc
  					    ${WXWIDGETS_DIR}/lib/vc_x64_lib/mswu/wx )
endif ()

link_directories (${WXWIDGETS_LIBS_DIR})
message(STATUS "WXWIDGETS_LIBS_DIR=${WXWIDGETS_LIBS_DIR}")

#target_compile_definitions(my_target PUBLIC foo=1 bar=2)

#add_compile_definitions(WinMain)
add_compile_definitions(_UNICODE )
#add_compile_definitions(wxTOOLKIT_PREFIX=${wxWidgets_CONFIGURATION})
#remove_definitions()

#set(commonIncludes PUBLIC ${VSE_DEVKIT_DIR}/include
#		${WXWIDGETS_DIR}/include
#		${WXWIDGETS_PLATFORM_INCLUDE_DIR )


include_directories( 
					${WXWIDGETS_DIR}/include
					${WXWIDGETS_PLATFORM_INCLUDE_DIR}
					)



#target_link_libraries(02_Minimal wx::net wx::core wx::base)



#file (GLOB wxWidgetsTestAppHeaderFiles chap02/*.hpp chap02/*.h)
#file (GLOB wxWidgetsTestAppSourceFiles chap02/*.cpp)
#set (
#	wxWidgetsTestAppFiles
#	${wxWidgetsTestAppHeaderFiles}
#	${wxWidgetsTestAppSourceFiles}
#)


#foreach(Number RANGE 2 20 )
#	
#	if (Number LESS 10)
#		set(examplePrefix "0${Number}")
#	else ()
#		set(examplePrefix "${Number}")
#	endif()
#endforeach()


function(CrateExe item)
	message(STATUS "item=${item}")
	string(REPLACE "/" "-" exeName ${item})
	message(STATUS "exeName=${exeName}")
	file(GLOB SRC_FILES ${item}/*.cpp)

	#message(STATUS "additionalSources=${additionalSources}")
	#message(STATUS "additionalIncludes=${additionalIncludes}")
	
	
	if(WIN32)
		add_executable(${exeName} WIN32 ${SRC_FILES} ) # ${additionalSources})
	else()
		add_executable(${exeName}  ${SRC_FILES} ) # ${additionalSources})
	endif()
	target_include_directories(${exeName} PUBLIC 
					${WXWIDGETS_DIR}/include
					${WXWIDGETS_PLATFORM_INCLUDE_DIR}
					#${additionalIncludes}
					${item})
endfunction()

foreach(item "chap02" "chap03" "chap09" "chap10" "chap11" "chap16" "chap17" )
	CrateExe(${item} )
endforeach()

file(GLOB SRC_FILES_chap12	chap12/*.cpp)
file(GLOB ThumbNailFiles	chap12/thumbnail/*.cpp) 
if(WIN32)
	add_executable(chap12 WIN32 ${SRC_FILES_chap12} ${ThumbNailFiles}  )
else()
	add_executable(chap12  ${SRC_FILES} ${ThumbNailFiles} )
endif()
target_include_directories(chap12 PUBLIC 
					${WXWIDGETS_DIR}/include
					${WXWIDGETS_PLATFORM_INCLUDE_DIR}
					"chap12/thumbnail"
					"chap12")






add_custom_command(
	TARGET "chap16" POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/chap16/flag.png  ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/flag.png #${CMAKE_CURRENT_BINARY_DIR}/flag.png
)
message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}S")



CrateExe("chap18/SocketEvents")
CrateExe("chap18/SocketStreams")
CrateExe("chap19/doodle")
CrateExe("chap20/findapppath")
CrateExe("chap20/launch")
CrateExe("chap20/pipedprocess")
CrateExe("chap20/valconfig")



#include_directories()
#link_directories()
#${wxWidgets_LIB_DIR}/