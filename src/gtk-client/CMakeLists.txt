###############################################################################
#CMake build script for achd-soc utilities

###############################################################################
#Core setup
CMAKE_MINIMUM_REQUIRED( VERSION 2.6 )
PROJECT( penguin-gtk )

IF(UNIX)
	FIND_PACKAGE(PkgConfig REQUIRED)

	pkg_check_modules(GTKMM gtkmm-2.4)

ELSEIF(WINDOWS)
	SET(GTKMM_LIBRARIES gtkmm-vc90-2_4)
ENDIF()


###############################################################################
#Build settings - configurable
SET( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)
SET( LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/bin)

IF(UNIX)
	# TODO: maybe use the following as well
	# -Wunused
	# -Wdisabled-optimization
	# -Wstack-protector

	# Not used due to excessive output in gtk etc
	# -Wpadded
	# -Wmissing-include-dirs 
	# -Winline 

	#TODO:
	# -Werror
	SET(EXTRA_WARNINGS "-Wcast-align -Winit-self -Wmissing-declarations -Wswitch -Wwrite-strings")
	SET( CMAKE_CXX_FLAGS "-O3 -Wall -Wextra -pedantic  ${EXTRA_WARNINGS}")
	SET( CMAKE_CXX_FLAGS_DEBUG "-g3" )
ENDIF()

ADD_SUBDIRECTORY(penguin-gtk)
