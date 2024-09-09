cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

#
# Project definitions and minimum system requirement, that is having wxWidgets installed on the system.
#
project(Cross-Platfrom-GUI-Programming-WxWidgets LANGUAGES CXX C)
find_package(wxWidgets REQUIRED)

#
# Header Section for notifying the old sources and checking compiler versions
#

message("The sources of the books is old.. see: https://forums.wxwidgets.org/viewtopic.php?t=50926")
message(STATUS "Detected generator: ${CMAKE_GENERATOR}")
message(STATUS "Detected c compiler: ${CMAKE_C_COMPILER}")
message(STATUS "Detected c++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Detected resource compiler: ${CMAKE_RC_COMPILER}")
set (WXWIDGETS_DIR ${wxWidgets_ROOT_DIR} CACHE PATH "wxWidgets binary directory.")


#
# Global configurations for all the compiled targets
#

# For certain IDEs like Visual Studio and Xcode supports organizing the targets in internal folders.
# (Be aware this is only in IDE not in file structures in e.g. file explorer.)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)



if (MSVC)
	set (WXWIDGETS_LIBS_DIR ${WXWIDGETS_DIR}/lib/vc_x64_lib)
	set (WXWIDGETS_PLATFORM_INCLUDE_DIR ${WXWIDGETS_DIR}/include 
					    ${WXWIDGETS_DIR}/include/msvc
  					    ${WXWIDGETS_DIR}/lib/vc_x64_lib/mswu/wx )
else()
	set (WXWIDGETS_LIBS_DIR ${WXWIDGETS_DIR}/lib)
	set (WXWIDGETS_PLATFORM_INCLUDE_DIR ${WXWIDGETS_DIR}/include 
					    ${WXWIDGETS_DIR}/include
  					    ${WXWIDGETS_DIR}/lib/wx )
endif ()

# Global link directories to wxWidgets. This applies to all compiled targets in the project; fine for this scope.
link_directories (${WXWIDGETS_LIBS_DIR})
message(STATUS "WXWIDGETS_LIBS_DIR=${WXWIDGETS_LIBS_DIR}")

# Global _UNICODE definition - wxWidgets requires this
add_compile_definitions(_UNICODE )


include_directories( 
					${WXWIDGETS_DIR}/include
					${WXWIDGETS_PLATFORM_INCLUDE_DIR}
					)

#
# Main function "CrateExe" to create the targets of the source code organized in "item" (Path)
#

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
		#message(STATUS "exeName=${exeName} ; exePath=${exePath}" )
		set_target_properties(${exeName} PROPERTIES FOLDER ${exePath})
	endif()
endfunction()



#
# For-loop to create all the compiled targets, except libraries and the Riffle app.
#

foreach(item "chap02" 
			 "chap09"
			 "chap10"
			 "chap11"
			 "chap12/thumbnail"
			 "chap12/taskbar"
			 "chap12/wizard"
			 "chap16"
			 "chap17"
			 "chap18/SocketEvents/Client"
			 "chap18/SocketEvents/Server"
			 "chap18/SocketStreams/Client"
			 "chap18/SocketStreams/Server"
			 "chap19/doodle"
			 #"chap20/pipedprocess"
			   )
	CrateExe(${item} )
endforeach()




		
file(GLOB Chap03_SRC  chap03/* )
add_library(chap03  ${Chap03_SRC} ) # ${additionalSources})

add_custom_command( TARGET "chap16" POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/chap16/flag.png "${CMAKE_BINARY_DIR}/$<CONFIG>/" )
add_custom_command( TARGET "chap16" POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${CMAKE_CURRENT_SOURCE_DIR}/chap16/de" "${CMAKE_BINARY_DIR}/$<CONFIG>/de" )
add_custom_command( TARGET "chap16" POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${CMAKE_CURRENT_SOURCE_DIR}/chap16/fr" "${CMAKE_BINARY_DIR}/$<CONFIG>/fr" )
#FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/chap16/flag.png DESTINATION ${CMAKE_BINARY_DIR})




# Chapter 20 builds of libraries 
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


#
# Configure the Riffle example application
#

file(GLOB RIFFLE_SRC riffle/src/*.cpp riffle/src/*.h )

if(WIN32)
	add_executable(Riffle-app WIN32 ${RIFFLE_SRC} )
else()
	add_executable(Riffle-app  ${RIFFLE_SRC} )
endif()

target_include_directories(Riffle-app PUBLIC 
				${WXWIDGETS_DIR}/include
				${WXWIDGETS_PLATFORM_INCLUDE_DIR}
				riffle/src
				)

#
# Riffle Manual & Help files post-build
#
 
# Logic for post-build of the compiled help contents (chm)
if(WIN32)
	set (USE_CHM FALSE CACHE BOOL "Use CHM compiled file for Riffle App?")
	if(USE_CHM)
		add_compile_definitions(WIN_USE_CHM)
		# Try to find the hhc.exe compiler on a x64 system, then x32 bit system.
		if (EXISTS "C:/Program Files (x86)/HTML Help Workshop/hhc.exe")
			set (CHM_PATH "C:/Program Files (x86)/HTML Help Workshop" CACHE PATH "Install Path for \"HTML Help WorkShop\"")
		elseif(EXISTS "C:/Program Files/HTML Help Workshop/hhc.exe")
			set (CHM_PATH "C:/Program Files/HTML Help Workshop" CACHE PATH "Install Path for \"HTML Help WorkShop\"")
		else()
			message(WARNING "HTML Help Workshop not found on system. Consider to install ${CMAKE_CURRENT_SOURCE_DIR}/Tools/htmlhelp.exe")
			set (CHM_PATH "NOT_FOUND" CACHE PATH "Install Path for \"HTML Help WorkShop\"")
		endif()

		if(EXISTS "${CHM_PATH}/hhc.exe")
			add_custom_command( TARGET Riffle-app POST_BUILD
							COMMAND "${CHM_PATH}/hhc.exe"  " ${CMAKE_CURRENT_SOURCE_DIR}\\riffle\\manual\\riffle.hhp" )#" ${CMAKE_BINARY_DIR}/$<CONFIG>/RiffleManual/rifle.hhp" )
		endif()
	endif()
endif()

# Move over the HTML help files to the build directory after successful build.
add_custom_command( TARGET Riffle-app POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different "${CMAKE_CURRENT_SOURCE_DIR}/riffle/manual" "${CMAKE_BINARY_DIR}/$<CONFIG>/RiffleManual" )


#
# Riffle Wizard Installer Package creation (optional)
#


set( RIFFLE_WIZARD FALSE CACHE BOOL "Create a distributable Setup Wizard for end-user installation?")

if(RIFFLE_WIZARD)
	# Set the package 
	if(WIN32)
		# Set the package 
		set (RIFFLE_WIZARD_CREATOR "NSIS" CACHE STRING "Selected Package creator"  )
		set_property(CACHE RIFFLE_WIZARD_CREATOR PROPERTY STRINGS "InnoSetup" "NSIS" "WiX")
		#set (RIFFLE_SIGN_APP FALSE CACHE BOOL "Create a signature for the Riffle App?")
	elseif(APPLE)
		set (RIFFLE_WIZARD_CREATOR "ProductBuild" CACHE STRING "Selected Package creator"  )
	endif()






	message(STATUS "Preparing CPack distributiom" )
	# Create signatures for the application
	install(TARGETS Riffle-app
		DESTINATION bin) # . # Here, bin is the relative path to the application, e.g. for Windows: C:\ProgramFiles\Anthemion\Riffle App\bin
	
	file(GLOB RIFFLE_MAN riffle/manual/* )	
	install(FILES ${RIFFLE_MAN}
		DESTINATION bin/manual)

    
	include (InstallRequiredSystemLibraries) 
	#include(CPack)

	# Add other files too: installing a config file (just as example, real application will rather use %ProgramData% )
	#install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/Config.xml" DESTINATION .)
    
	# semver versoning numbers.
	set(PROJECT_VERSION_MAJOR 1) # Uncompatible to previous versions (e.g. files, network protocols, etc.)
	set(PROJECT_VERSION_MINOR 0) # New feature
	set(PROJECT_VERSION_PATCH 0) # Bugfix

	#set(CPACK_INSTALL_CMAKE_PROJECTS  "SubProject;Riffle-App;ALL;/")

	# Generic CPack settings
	set(CPACK_PACKAGE_NAME "Riffle App")
    set(CPACK_PACKAGE_VENDOR "Anthemion")
	set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Directory picture thumbnail viewer")
	set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})  # Name of subfolder under 'Program Files'
	set(CPACK_PACKAGE_VERSION    "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")
	set(CPACK_PACKAGE_VERSION_MAJOR    ${PROJECT_VERSION_MAJOR})
	set(CPACK_PACKAGE_VERSION_MINOR    ${PROJECT_VERSION_MINOR})
	set(CPACK_PACKAGE_VERSION_PATCH    ${PROJECT_VERSION_PATCH})
	set(CPACK_VERBATIM_VARIABLES       TRUE)         # Ensure correct escaping - always set to true

	#Set resources used by the Windows installer
	#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/riffle/docs/license.txt")
	#set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/riffle/src/bitmaps/riffle.ico") # Logo in installer window

	#Start Menu Shortcuts: executable without .exe and text label
	#set(CPACK_PACKAGE_EXECUTABLES "Riffle App" "Riffle Application") 
	#set(CPACK_CREATE_DESKTOP_LINKS "Riffle Application") # Desktop 

	
	
 
	#set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY ${PROJECT_NAME})
	set(CPACK_SOURCE_GENERATOR "TGZ")
	set(CPACK_SOURCE_PACKAGE_FILE_NAME  ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION} )
	set(CPACK_SOURCE_IGNORE_FILES ${CPACK_IGNORE_FILES})
endif()


if(RIFFLE_WIZARD_CREATOR STREQUAL "InnoSetup")
	message(STATUS "Preparing Inno Setup Install wizard" )
	set(CPACK_GENERATOR "INNOSETUP")
    set(CPACK_INNOSETUP_SETUP_UninstallDisplayIcon "${CMAKE_CURRENT_SOURCE_DIR}/riffle/src/bitmaps/riffle.ico") 
    set(CPACK_INNOSETUP_SETUP_SetupIconFile "${CMAKE_CURRENT_SOURCE_DIR}/riffle/src/bitmaps/riffle.ico")
       
    #Executable that user can run at end of install. It refers CPACK_PACKAGE_EXECUTABLES above.
    set ( CPACK_INNOSETUP_RUN_EXECUTABLES Riffle-app ) 
    
    # OPTIONAL: Set up custom script for Inno Setup. Your Pascal functions to add in [code] block  
    # set (CPACK_INNOSETUP_CODE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/codefile.iss")
endif()

if(RIFFLE_WIZARD_CREATOR STREQUAL "NSIS")
	message(STATUS "Preparing NSIS Install wizard" )
	set(CPACK_GENERATOR "NSIS")
	#set(CPACK_NSIS_HELP_LINK "http://www.anthemion.co.uk/index.htm")
	#set(CPACK_NSIS_URL_INFO_ABOUT "http://www.anthemion.co.uk/aboutus.htm")
	#set(CPACK_NSIS_CONTACT "harriet@anthemion.co.uk")
	#set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/riffle/src/bitmaps/riffle.ico" )
	#set(CPACK_NSIS_INSTALLED_ICON_NAME "${CMAKE_CURRENT_SOURCE_DIR}/riffle/src/bitmaps/riffle.ico" ) 
	#set (CPACK_NSIS_MODIFY_PATH "ON")

	# the NSIS installer script that CMake will read from and substitute the values into:
	#set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/assets/nsis ${CMAKE_MODULE_PATH})

	#configure_file(
	#	${PROJECT_SOURCE_DIR}/riffle/scripts/nsis/NSIS.definitions.nsh.in
	#	${CMAKE_CURRENT_BINARY_DIR}/riffle/scripts/nsis/NSIS.definitions.nsh.in
	#)

	#set (CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
    # WriteRegStr HKCR '.cool' '' 'CoolFile'
	# System::Call \\
    #   'Shell32::SHChangeNotify(i 0x8000000, i 0, i 0, i 0)'" )

	#set (CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
    # DeleteRegKey HKCR '.cool'
	# ")

	# set(CPACK_NSIS_CREATE_ICONS_EXTRA "CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\abc.lnk' '$INSTDIR\\\\bin\\\\abc.exe' ")

endif()

if(RIFFLE_WIZARD_CREATOR STREQUAL "WiX")
	message(STATUS "Preparing WiX Install wizard" )
	set(CPACK_GENERATOR WIX)
	# set(CPACK_WIX "${CMAKE_CURRENT_SOURCE_DIR}/riffle/src/bitmaps/riffle.ico" )
endif()

if(RIFFLE_WIZARD_CREATOR STREQUAL "ProductBuild")
endif()


if(RIFFLE_WIZARD)
	include(CPack)
endif()