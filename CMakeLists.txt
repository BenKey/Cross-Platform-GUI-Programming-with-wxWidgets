cmake_minimum_required(VERSION 3.14 FATAL_ERROR)
project(Cross-Platfrom-GUI-Programming-WxWidgets LANGUAGES CXX C)

find_package(wxWidgets REQUIRED)

message("The sources of the books is old.. see: https://forums.wxwidgets.org/viewtopic.php?t=50926")
message(STATUS "Detected generator: ${CMAKE_GENERATOR}")

set (WXWIDGETS_DIR ${wxWidgets_ROOT_DIR} CACHE PATH "wxWidgets binary directory.")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

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


function(CrateExe item )
	#message(STATUS "item=${item}")
	string(REPLACE "/" ";" pathName ${item})
	list(LENGTH pathName PathLenght )
	if ( ( CMAKE_GENERATOR MATCHES "Visual Studio" OR CMAKE_GENERATOR MATCHES "Xcode") AND PathLenght GREATER 1)
		set(FOLDER_FLAG TRUE)
	endif()
	if (FOLDER_FLAG)
		list(POP_FRONT pathName  exePath)
		list(JOIN pathName "-" exeName)
	else()
		string(REPLACE "/" "-" exeName ${item})
	endif()
	#message(STATUS "exeName=${exeName}")
	message(STATUS "Creating build for: ${exeName}")
	file(GLOB SRC_FILES ${item}/*.cpp ${item}/*.h ${item}/*.xpm )

	#message(STATUS "additionalSources=${additionalSources}")
	#message(STATUS "additionalIncludes=${additionalIncludes}")
	
	
	if(WIN32)
		add_executable(${exeName} WIN32 ${SRC_FILES} ) # ${additionalSources})
	else()
		add_executable(${exeName}  ${SRC_FILES} ) # ${additionalSources})
	endif()
	target_include_directories(${exeName} PUBLIC 
					#${WXWIDGETS_DIR}/include
					#${WXWIDGETS_PLATFORM_INCLUDE_DIR}
					#${additionalIncludes}
					${item})
	if (FOLDER_FLAG)
		message(STATUS "exeName=${exeName} ; exePath=${exePath}" )
		set_target_properties(${exeName} PROPERTIES FOLDER ${exePath})
	endif()
endfunction()

foreach(item "chap02" "chap09" "chap10" "chap11" "chap12/thumbnail" "chap12/taskbar" "chap12/wizard" "chap16" "chap17" )
	CrateExe(${item} )
endforeach()




		
file(GLOB Chap03_SRC  chap03/* )
add_library(chap03  ${Chap03_SRC} ) # ${additionalSources})





#file(GLOB SRC_FILES_chap12	chap12/*.cpp chap12/*.h chap12/*.xpm  )
#file(GLOB ThumbNailFiles	chap12/thumbnail/*.cpp chap12/thumbnail/*.h chap12/*.xpm ) 
#if(WIN32)
#	add_executable(chap12 WIN32 ${SRC_FILES_chap12} ${ThumbNailFiles}  )
#else()
#	add_executable(chap12  ${SRC_FILES} ${ThumbNailFiles} )
#endif()
#target_include_directories(chap12 PUBLIC 
					#${WXWIDGETS_DIR}/include
					#${WXWIDGETS_PLATFORM_INCLUDE_DIR}
#					"chap12/thumbnail"
#					"chap12")






#add_custom_command(
#	TARGET "chap16" POST_BUILD
 #   COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/chap16/flag.png  ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/flag.png #${CMAKE_CURRENT_BINARY_DIR}/flag.png
#)
#message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}S")

#if (CMAKE_BUILD_TYPE)
#	message(STATUS "CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
#	FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/chap16/flag.png DESTINATION "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
#elseif(CMAKE_CONFIGURATION_TYPES)
#	foreach(buildType ${CMAKE_CONFIGURATION_TYPES})
#		message(STATUS "CMAKE_CONFIGURATION_TYPE=${buildType}")
#		FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/chap16/flag.png DESTINATION "${CMAKE_BINARY_DIR}/${buildType}")
#	endforeach()
#endif()


#FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/chap16/flag.png DESTINATION "${CMAKE_BINARY_DIR}/$<CONFIG>" )
add_custom_command( TARGET "chap16" POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/chap16/flag.png "${CMAKE_BINARY_DIR}/$<CONFIG>/" )
FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/chap16/flag.png DESTINATION ${CMAKE_BINARY_DIR})

#message(STATUS "Creating build for: chap16")
#file(GLOB chap16_SRC chap16/*.cpp chap16/*.h chap16/*.xpm chap16/*.png )
#
#if(WIN32)
#	add_executable(chap16 WIN32 ${chap16_SRC} ) # ${additionalSources})
#else()
#	add_executable(chap16  ${chap16_SRC} ) # ${additionalSources})
#endif()
#target_include_directories(chap16 PUBLIC 
#				chap16)











CrateExe("chap18/SocketEvents/Client")
CrateExe("chap18/SocketEvents/Server")
CrateExe("chap18/SocketStreams/Client")
CrateExe("chap18/SocketStreams/Server")
CrateExe("chap19/doodle")
#CrateExe("chap20/findapppath")
#CrateExe("chap20/launch")
CrateExe("chap20/pipedprocess")
#CrateExe("chap20/valconfig")


if ( CMAKE_GENERATOR MATCHES "Visual Studio" OR CMAKE_GENERATOR MATCHES "Xcode") 
	add_library(findapppath  chap20/findapppath/findapppath.cpp )
	set_target_properties(findapppath PROPERTIES FOLDER chap20)

	add_library(launch  chap20/launch/launch.cpp chap20/launch/launch.h  ) 
	set_target_properties(launch PROPERTIES FOLDER chap20)

	add_library(valconfig  chap20/valconfig/valconfig.cpp chap20/valconfig/valconfig.h  ) 
	set_target_properties(valconfig PROPERTIES FOLDER chap20)

else()
	add_library(chap20-findapppath  chap20/findapppath/findapppath.cpp )
	add_library(chap20-launch  chap20/launch/launch.cpp chap20/launch/launch.h  ) 
	add_library(chap20-valconfig  chap20/valconfig/valconfig.cpp chap20/valconfig/valconfig.h  ) 
endif()





file(GLOB RIFFLE_SRC riffle/src/*.cpp riffle/src/*.h )

message(STATUS "RIFFLE_SRC=${RIFFLE_SRC}")
if(WIN32)
	add_executable(Riffle-app WIN32 ${RIFFLE_SRC} )
else()
	add_executable(Riffle-app  ${RIFFLE_SRC} )
endif()

target_include_directories(Riffle-app PUBLIC 
				${WXWIDGETS_DIR}/include
				${WXWIDGETS_PLATFORM_INCLUDE_DIR}
				riffle/src
				riffle/manual)


# Manual & Help files post-build

#file(GLOB SRC_FILES ${item}/*.cpp ${item}/*.h ${item}/*.xpm )
add_custom_command( TARGET Riffle-app POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${CMAKE_CURRENT_SOURCE_DIR}/riffle/manual" "${CMAKE_BINARY_DIR}/$<CONFIG>/RiffleManual" )

#target_link_libraries(Riffle-app wx::net wx::core wx::base)

#include_directories()
#link_directories()
#${wxWidgets_LIB_DIR}/