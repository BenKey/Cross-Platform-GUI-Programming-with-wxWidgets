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


# Copy the ReadFrom.jpg file for sample server/client file transfer.
add_custom_command( TARGET "SocketStreams-Server" POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/chap18/SocketStreams/ReadFrom.jpg" "${CMAKE_BINARY_DIR}/$<CONFIG>" )
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


# Signing app: https://stackoverflow.com/questions/72504366/how-to-sign-windows-binaries-and-nsis-installers-when-building-with-cmake-cpac

set( RIFFLE_WIZARD FALSE CACHE BOOL "Create a distributable Setup Wizard for end-user installation?")

if(RIFFLE_WIZARD)
	# Set the package 
	if(WIN32)
		# Set the package 
		set (RIFFLE_WIZARD_CREATOR "NSIS" CACHE STRING "Selected Package creator"  )
		set_property(CACHE RIFFLE_WIZARD_CREATOR PROPERTY STRINGS "InnoSetup" "NSIS" "WiX" "QtIFW" "TGZ")
		#set (RIFFLE_SIGN_APP FALSE CACHE BOOL "Create a signature for the Riffle App?")
		#set(CPACK_SET_DESTDIR true)

		set (RIFFLE_WIZARD_REQUIRE_ADMIN FALSE CACHE BOOL "Require Admin prevledges for the installer wizard?" )
		if (NOT RIFFLE_WIZARD_REQUIRE_ADMIN)
			set(CMAKE_INSTALL_PREFIX "%APPDATA%/Anthemion") #CPACK_PACKAGE_DIRECTORY CPACK_INSTALL_PREFIX
		else()
		endif()
	elseif(APPLE)
		
		set (RIFFLE_WIZARD_CREATOR "ProductBuild" CACHE STRING "Selected Package creator"  )
		set_property(CACHE RIFFLE_WIZARD_CREATOR PROPERTY STRINGS "ProductBuild" "Bundle" "DragNDrop" "QtIFW" "TGZ")
	elseif(UNIX)
		set (RIFFLE_WIZARD_CREATOR "DEB" CACHE STRING "Selected Package creator"  )
		set_property(CACHE RIFFLE_WIZARD_CREATOR PROPERTY STRINGS "FreeBSD" "DEB" "RPM" "QtIFW" "TGZ")
	else()
		message(WARNING "Unrecognized OS, skipping installer wizard creation")
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
	set(PROJECT_VERSION_MINOR 00) # New feature
	set(PROJECT_VERSION_PATCH 0) # Bugfix

	#set(CPACK_INSTALL_CMAKE_PROJECTS  "SubProject;Riffle-App;ALL;/")

	# Generic CPack settings
	set(CPACK_PACKAGE_NAME "Riffle")
    set(CPACK_PACKAGE_VENDOR "Anthemion")
	set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Directory picture thumbnail viewer")
	set(CPACK_PACKAGE_INSTALL_DIRECTORY ${CPACK_PACKAGE_NAME})  # Name of subfolder under 'Program Files'
	set(CPACK_PACKAGE_VERSION    "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")
	set(CPACK_PACKAGE_VERSION_MAJOR    ${PROJECT_VERSION_MAJOR})
	set(CPACK_PACKAGE_VERSION_MINOR    ${PROJECT_VERSION_MINOR})
	set(CPACK_PACKAGE_VERSION_PATCH    ${PROJECT_VERSION_PATCH})
	set(CPACK_VERBATIM_VARIABLES       TRUE)         # Ensure correct escaping - always set to true

	#Set resources used by the Windows installer
	set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/riffle/docs/license.txt")
	#set(CPACK_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/riffle/src/bitmaps/riffle64x64.png") # Logo in installer window

	set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/riffle/docs/readme.txt" )
	set(CPACK_RESOURCE_FILE_WELCOME "${CMAKE_CURRENT_SOURCE_DIR}/riffle/docs/readme.txt" )

	#Start Menu Shortcuts: executable without .exe and text label
	set(CPACK_PACKAGE_EXECUTABLES "Riffle App" "Riffle Application") 
	set(CPACK_CREATE_DESKTOP_LINKS "Riffle Application") # Desktop 

	set(CPACK_PACKAGE_INSTALL_DIRECTORY "Riffle App ${PROJECT_VERSION_MAJOR}") 
	
	set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/riffle/docs/readme.txt")
 
	#set(CPACK_PACKAGE_INSTALL_REGISTRY_KEY ${PROJECT_NAME})
	set(CPACK_SOURCE_GENERATOR "TGZ")
	set(CPACK_SOURCE_PACKAGE_FILE_NAME  ${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION} )
	set(CPACK_SOURCE_IGNORE_FILES ${CPACK_IGNORE_FILES})


	


	#set(CPACK_PACKAGE_ICON  "${CMAKE_CURRENT_SOURCE_DIR}/riffle/scripts/riffle128x128_wb2.bmp")
	#"${CMAKE_CURRENT_SOURCE_DIR}/riffle/src/bitmaps/riffle64x64.png" ) # "${CMAKE_CURRENT_SOURCE_DIR}/riffle/scripts/wizard_Anthemion_welcome.bmp")

endif()


if(RIFFLE_WIZARD_CREATOR STREQUAL "InnoSetup")
	message(STATUS "Preparing Inno Setup Install wizard, see https://cmake.org/cmake/help/latest/cpack_gen/innosetup.html" )
	set(CPACK_GENERATOR "INNOSETUP")
    set(CPACK_INNOSETUP_SETUP_UninstallDisplayIcon "${CMAKE_CURRENT_SOURCE_DIR}/riffle/src/bitmaps/riffle.ico") 
    set(CPACK_INNOSETUP_SETUP_SetupIconFile "${CMAKE_CURRENT_SOURCE_DIR}/riffle/src/bitmaps/riffle.ico")
       
    #Executable that user can run at end of install. It refers CPACK_PACKAGE_EXECUTABLES above.
    set ( CPACK_INNOSETUP_RUN_EXECUTABLES Riffle-app ) 
    
	
	set(CMAKE_INNOSETUP_SETUP_AllowNoIcons ON) #Default
	
	# set(CPACK_INNOSETUP_INSTALL_ROOT  <path>
	set(CPACK_INNOSETUP_ALLOW_CUSTOM_DIRECTORY ON) #Default
	set(CPACK_INNOSETUP_PROGRAM_MENU_FOLDER "Anthemion Software")
	set(CPACK_INNOSETUP_LANGUAGES "armenian;brazilianPortuguese;bulgarian;catalan;corsican;czech;danish;dutch;english;finnish;french;german;hebrew;icelandic;italian;japanese;norwegian;polish;portuguese;russian;slovak;slovenian;spanish;turkish;ukrainian")
	set(CPACK_INNOSETUP_IGNORE_LICENSE_PAGE OFF) # Default: OFF
	set(CPACK_INNOSETUP_IGNORE_README_PAGE OFF) # Default: ON
	
	#set(CPACK_INNOSETUP_PASSWORD "riffle")
	set(CPACK_INNOSETUP_USE_MODERN_WIZARD ON) #Default: OFF
	set(CPACK_INNOSETUP_ICON_FILE "${CMAKE_CURRENT_SOURCE_DIR}/riffle/src/bitmaps/riffle.ico")

	#set( CPACK_INNOSETUP_SETUP_<directive>  <value>) This group allows adapting any of the [Setup] section directives
	# e.g.
	set(CPACK_INNOSETUP_SETUP_WizardSmallImageFile "${CMAKE_CURRENT_SOURCE_DIR}/riffle/scripts/anthemion_logo.bmp")
	set(CPACK_INNOSETUP_SETUP_AllowNoIcons OFF) # This requires CPACK_INNOSETUP_USE_CMAKE_BOOL_FORMAT to be on

	set(CPACK_INNOSETUP_RUN_EXECUTABLES Riffle-app)

	if (NOT RIFFLE_WIZARD_REQUIRE_ADMIN)
		set(CPACK_INNOSETUP_INSTALL_ROOT "{userappdata}")
	endif()
    # OPTIONAL: Set up custom script for Inno Setup. Your Pascal functions to add in [code] block  
    # set (CPACK_INNOSETUP_CODE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/codefile.iss")
elseif(RIFFLE_WIZARD_CREATOR STREQUAL "NSIS")
	message(STATUS "Preparing NSIS Install wizard, see https://cmake.org/cmake/help/latest/cpack_gen/nsis.html and https://nsis.sourceforge.io/Docs/" )
	# https://blog.davidvelho.com/posts/cmake_cpack_nsis_windows/

	set(CPACK_GENERATOR "NSIS")
	set(CPACK_NSIS_HELP_LINK "http://www.anthemion.co.uk/index.htm")
	set(CPACK_NSIS_URL_INFO_ABOUT "http://www.anthemion.co.uk/aboutus.htm")
	set(CPACK_NSIS_CONTACT "harriet@anthemion.co.uk")
	set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/riffle/src/bitmaps/riffle.ico" )
	set(CPACK_NSIS_MUI_UNIICON "${CMAKE_CURRENT_SOURCE_DIR}/riffle/scripts/unriffle.ico" )
	set(CPACK_NSIS_INSTALLED_ICON_NAME "${CMAKE_CURRENT_SOURCE_DIR}/riffle/src/bitmaps/riffle.ico" ) 
	set (CPACK_NSIS_MODIFY_PATH ON)

	set(CPACK_NSIS_WELCOME_TITLE "Installing Riffle App")
	set(CPACK_NSIS_WELCOME_TITLE_3LINES ON)

	set(CPACK_NSIS_FINISH_TITLE "Thank you for installing Riffle!")
	set(CPACK_NSIS_FINISH_TITLE_3LINES ON)

	set(CPACK_NSIS_BRANDING_TEXT "An Anthemion Ltd. Application..")

	set(CPACK_NSIS_UNINSTALL_NAME "Uninstall Riffle")

	#set(CPACK_NSIS_INSTALLED_ICON_NAME "${CMAKE_CURRENT_SOURCE_DIR}/riffle/scripts/anthemion_logo.bmp")

	set(CPACK_NSIS_MUI_HEADERIMAGE "${CMAKE_CURRENT_SOURCE_DIR}\\riffle\\scripts\\banner_NSIS_256.bmp") #"\$\{NSISDIR\}\\Contrib\\Graphics\\Header\\nsis3-metro.bmp") # ${CMAKE_CURRENT_SOURCE_DIR}/riffle/scripts/banner-NSIS.bmp)

	# the NSIS installer script that CMake will read from and substitute the values into:
	#set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/assets/nsis ${CMAKE_MODULE_PATH})

	set(CPACK_NSIS_MUI_WELCOMEFINISHPAGE_BITMAP "${CMAKE_CURRENT_SOURCE_DIR}\\riffle\\scripts\\wizard_Anthemion_welcome_8bit.bmp") #"\$\{NSISDIR\}\\Contrib\\Graphics\\Header\\nsis3-metro.bmp") # "${CMAKE_CURRENT_SOURCE_DIR}/riffle/scripts/banner_NSIS.bmp")
	set(CPACK_NSIS_MUI_UNWELCOMEFINISHPAGE_BITMAP "${CMAKE_CURRENT_SOURCE_DIR}\\riffle\\scripts\\wizard_Anthemion_unwelcome.bmp") # wizard_Anthemion_unwelcome.bmp
	#set(CPACK_NSIS_MUI_HEADERIMAGE_BITMAP "${CMAKE_CURRENT_SOURCE_DIR}/riffle/scripts/banner_NSIS.bmp")



	# Extra NSIS commands that will be added to the beginning of the install Section, before your install tree is available on the target system.
	#set( CPACK_NSIS_EXTRA_PREINSTALL_COMMANDS "
	#set( CPACK_NSIS_EXTRA_PREINSTALL_COMMANDS " InstallDir \"\$LOCALAPPDATA\\" )

	# Extra NSIS commands that will be added to the end of the install Section, after your install tree is available on the target system.
	#set( CPACK_NSIS_EXTRA_INSTALL_COMMANDS )

	# Extra NSIS commands that will be added to the uninstall Section, before your install tree is removed from the target system.
	#set( CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS )



	#configure_file(
	#	${PROJECT_SOURCE_DIR}/riffle/scripts/nsis/NSIS.definitions.nsh.in
	#	${CMAKE_CURRENT_BINARY_DIR}/riffle/scripts/nsis/NSIS.definitions.nsh.in
	#)

	# Register standard app for file type *.riffle
	list (APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
     WriteRegStr HKCR '.riffle' '' 'RiffleFile'
	 System::Call \\
       'Shell32::SHChangeNotify(i 0x8000000, i 0, i 0, i 0)'" )

	set (CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "
     DeleteRegKey HKCR '.riffle'
	 ")

	# makensis.exe <preArgs>... "nsisFileName.nsi" <postArgs>...
	#set(CPACK_NSIS_EXECUTABLE_PRE_ARGUMENTS "-arg1 arv1value")
	#set(CPACK_NSIS_EXECUTABLE_POST_ARGUMENTS "-arg1 arv1value")

	#set( CPACK_NSIS_CREATE_ICONS_EXTRA )

	#set(CPACK_NSIS_MENU_LINKS
    #"bin/manual/doc-0004.htm" "Riffle Help" 
	#"http://www.anthemion.co.uk/" "Anthemion Web Site")

	#list(APPEND CPACK_NSIS_EXTRA_INSTALL_COMMANDS "
	# RequestExecutionLevel 'user'")
	if (NOT RIFFLE_WIZARD_REQUIRE_ADMIN)
		set(CPACK_NSIS_DEFINES "RequestExecutionLevel user")
		#set(CPACK_NSIS_INSTALL_ROOT "\%LOCALAPPDATA\%")
		#set(CPACK_INSTALL_PREFIX  "\%APPDATA\%")
		# SET(CMAKE_PACKAGE_INSTALL_PREFIX "\%LOCALAPPDATA\%" ) # "${CMAKE_USER_HOME_DIR}")
		set(CPACK_PACKAGE_INSTALL_DIRECTORY "\%RiffleApp\%")
		#message(${CPACK_NSIS_INSTALL_ROOT}) # CMAKE_INSTALL_PREFIX CPACK_NSIS_INSTALL_ROOT
	endif()
	string (REPLACE ";" "\n" CPACK_NSIS_EXTRA_INSTALL_COMMANDS "${CPACK_NSIS_EXTRA_INSTALL_COMMANDS}")


	# set(CPACK_NSIS_CREATE_ICONS_EXTRA "CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\abc.lnk' '$INSTDIR\\\\bin\\\\abc.exe' ")

elseif(RIFFLE_WIZARD_CREATOR STREQUAL "WiX")
	message(STATUS "Preparing WiX Install wizard, see https://cmake.org/cmake/help/latest/cpack_gen/wix.html" )
	message(FATAL_ERROR "${RIFFLE_WIZARD_CREATOR} yet implemented!")
	set(CPACK_GENERATOR WIX)
	# set(CPACK_WIX "${CMAKE_CURRENT_SOURCE_DIR}/riffle/src/bitmaps/riffle.ico" )

elseif(RIFFLE_WIZARD_CREATOR STREQUAL "QtIFW")
	message(STATUS "Preparing Qt Installer FrameWork wizard, see https://cmake.org/cmake/help/latest/cpack_gen/ifw.html and https://doc.qt.io/qtinstallerframework/ifw-tutorial.html" )
	message(FATAL_ERROR "${RIFFLE_WIZARD_CREATOR} yet implemented!")
	set(CPACK_GENERATOR "IFW")
	set(CPACK_IFW_PACKAGE_ICON "${CMAKE_CURRENT_SOURCE_DIR}/riffle/src/bitmaps/riffle.ico")

elseif (RIFFLE_WIZARD)
	message(FATAL_ERROR "${RIFFLE_WIZARD_CREATOR} yet implemented!")
endif()




if(RIFFLE_WIZARD_CREATOR STREQUAL "ProductBuild")
	message(STATUS "Preparing Qt Installer FrameWork wizard, see https://cmake.org/cmake/help/latest/cpack_gen/productbuild.html" )
	message(FATAL_ERROR "${RIFFLE_WIZARD_CREATOR} yet implemented!")
endif()


if(RIFFLE_WIZARD)
	include(CPack)

	cpack_add_component(applications)
	cpack_add_component(libraries)
	cpack_add_component(headers)

	if( RIFFLE_WIZARD_CREATOR STREQUAL "QtIFW")
		include(CPackIFW)
	endif()
	if(RIFFLE_WIZARD_CREATOR STREQUAL "NSIS")
		#configure_file("${CMAKE_CURRENT_SOURCE_DIR}/riffle/scripts/nsis/NSIS.definitions.nsh.in"
		#				"${CMAKE_CURRENT_BINARY_DIR}/assets/nsis/NSIS.definitions.nsh")
	endif()

	#cpack_add_component_group( Riffle_Group
	#	DISPLAY_NAME "Riffle Install Contents"
	#	INSTALL_TYPES Full Developer Minimal
	#
	#)

endif()