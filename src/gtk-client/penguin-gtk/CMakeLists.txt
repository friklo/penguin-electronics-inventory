#Set up include paths
INCLUDE_DIRECTORIES(
	${CMAKE_BINARY_DIR}
	${GTKMM_INCLUDE_DIRS}
	
	# TODO: need a script to find the correct relative path!!
	/home/azonenberg/native/programming/3rdparty/jsoncpp-src-0.5.0/include
)

###############################################################################
#C++ compilation
ADD_EXECUTABLE(penguin-gtk
	AddCategoryDialog.cpp
	MainWindow.cpp
	
	main.cpp
)

###############################################################################
#Linker settings
TARGET_LINK_LIBRARIES(penguin-gtk
	m rt curl
	${GTKMM_LIBRARIES}
	
	# TODO: need a script to find the correct relative path!!
	/home/azonenberg/native/programming/3rdparty/jsoncpp-src-0.5.0/libs/linux-gcc-4.4.5/libjson_linux-gcc-4.4.5_libmt.a
)

