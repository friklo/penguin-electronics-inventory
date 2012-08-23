#Set up include paths
INCLUDE_DIRECTORIES(
	${CMAKE_BINARY_DIR}
	${GTKMM_INCLUDE_DIRS}
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
)

