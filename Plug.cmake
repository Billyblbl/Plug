if (UNIX)
	set(LDLIB dl)
elseif(WIN32)
	set(LDLIB "")
endif(UNIX)

#PLUG
message(STATUS "Configuring Plug build")

option(STATIC_LIB "Library compilation mode" OFF)
if (STATIC_LIB)
	set(LIB_TYPE STATIC)
else(STATIC_LIB)
	set(LIB_TYPE SHARED)
endif(STATIC_LIB)

# set(LIB_TYPE STATIC)

message(STATUS "building as " ${LIB_TYPE})

file(GLOB PLUG_SRC ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB PLUG_HDR ${CMAKE_CURRENT_LIST_DIR}/include/*.hpp)

add_library(Plug ${LIB_TYPE})
target_sources(Plug PRIVATE ${PLUG_SRC})
target_sources(Plug INTERFACE ${PLUG_HDR})
target_include_directories(Plug PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_link_libraries(Plug ${LDLIB})


# #DOXYGEN

# # check if Doxygen is installed
# find_package(Doxygen)

# # indicate the documentation build as an option
# option(BUILD_DOC "Build documentation" ${DOXYGEN_FOUND})

# if (${BUILD_DOC} AND ${DOXYGEN_FOUND})
# 	# set input and output files
# 	set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
# 	set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

# 	# request to configure the file
# 	configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
# 	message(STATUS "Doxygen build started")

# 	add_custom_target(docs
# 		COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
# 		WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
# 		COMMENT "Generating API documentation with Doxygen"
# 		VERBATIM
# 	)
# 	message(STATUS "Doxygen build complete")
# elseif(${BUILD_DOC})
# 	message("Doxygen need to be installed to generate the doxygen documentation")
# endif (${BUILD_DOC} AND ${DOXYGEN_FOUND})
