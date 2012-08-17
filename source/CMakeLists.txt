CMAKE_MINIMUM_REQUIRED ( VERSION 2.6 )

INCLUDE( build/ModuleInfos.cmake )

PROJECT ( ${MODULE_NAME} C CXX )

# -- Qt 4.8.0 minimum --
FIND_PACKAGE( Qt4 4.8.0 COMPONENTS QtCore REQUIRED )
# -- Activate proper modules --
SET( QT_USE_QTGUI OFF )
INCLUDE( ${QT_USE_FILE} )

INCLUDE_DIRECTORIES( ${KORE_INCLUDES} )

### ------------- Kore -------------
SET (
	Kore_SRCS
	
	src/KoreApplication.cpp
	src/KoreEngine.cpp
)

INCLUDE ( src/data/srcs_data.txt )
INCLUDE ( src/event/srcs_event.txt )
INCLUDE ( src/memory/srcs_memory.txt )
INCLUDE ( src/parallel/srcs_parallel.txt )
INCLUDE ( src/plugin/srcs_plugin.txt )
INCLUDE ( src/serialization/srcs_serialization.txt )
INCLUDE ( src/system/srcs_system.txt )
INCLUDE ( src/util/srcs_util.txt )

SET (
	Kore_HDRS
	
	inc/KoreApplication.hpp
	inc/Macros.hpp
	inc/Types.hpp
	inc/KoreExport.hpp
)

# Visual Studio compiler specifics
IF ( MSVC )
	SET (
		Kore_HDRS
		
		${Kore_HDRS}
		inc/Macros_MSVC.hpp
	)
# Others compilers specifics
ELSE ( MSVC )
	SET (
		Kore_HDRS
		
		${Kore_HDRS}
		inc/Macros_GCC.hpp
	)
ENDIF ( MSVC )
	
SET (
	Kore_MOC_HDRS

	inc/KoreEngine.hpp
)

INCLUDE ( inc/data/hdrs_data.txt )
INCLUDE ( inc/event/hdrs_event.txt )
INCLUDE ( inc/memory/hdrs_memory.txt )
INCLUDE ( inc/parallel/hdrs_parallel.txt )
INCLUDE ( inc/plugin/hdrs_plugin.txt )
INCLUDE ( inc/serialization/hdrs_serialization.txt )
INCLUDE ( inc/system/hdrs_system.txt )
INCLUDE ( inc/util/hdrs_util.txt )

### ------------- MOC --------------
QT4_WRAP_CPP ( Kore_MOC_SRCS ${Kore_MOC_HDRS} )

# Target library: Kore
ADD_LIBRARY ( ${KORE_LIBRARY} SHARED ${Kore_SRCS} ${Kore_MOC_SRCS} ${Kore_HDRS} ${Kore_MOC_HDRS} )

# Mac OS specifics
IF ( APPLE )
	SET_TARGET_PROPERTIES ( 
		${KORE_LIBRARY} PROPERTIES
		COMPILE_DEFINITIONS "_K_BUILD_KORE;_KORE_VERSION=\"${KORE_VERSION_STRING}\";_K_MACX;${DEBUG_DEFINES}"
		COMPILE_FLAGS -fno-common
		#LINK_FLAGS "-undefined dynamic_lookup"
		VERSION "${KORE_VERSION_STRING}"
	)
	TARGET_LINK_LIBRARIES ( ${KORE_LIBRARY} ${QT_QTCORE_LIBRARY} )
# Windows specifics
ELSEIF ( WIN32 )
	# Visual Studio compiler specifics
	IF ( MSVC )
		SOURCE_GROUP ( "Header Files" FILES ${Kore_HDRS} ${Kore_MOC_HDRS} )
		SOURCE_GROUP ( "Source Files" FILES ${Kore_SRCS} )
		SOURCE_GROUP ( "Moc Files" FILES ${Kore_MOC_SRCS} )
		SET_TARGET_PROPERTIES ( 
			${KORE_LIBRARY} PROPERTIES
			COMPILE_DEFINITIONS "_K_BUILD_KORE;_KORE_VERSION=\"${KORE_VERSION_STRING}\";_K_WIN32;${DEBUG_DEFINES}"
			VERSION ${KORE_VERSION_STRING}
		)
		TARGET_LINK_LIBRARIES ( ${KORE_LIBRARY} ${QT_QTCORE_LIBRARY} )
	# Others compilers specifics
	ELSE ( MSVC )
		SET_TARGET_PROPERTIES ( 
			${KORE_LIBRARY} PROPERTIES
			COMPILE_FLAGS -msse
			COMPILE_DEFINITIONS "_K_BUILD_KORE;_KORE_VERSION=\"${KORE_VERSION_STRING}\";_K_WIN32;${DEBUG_DEFINES}"
			VERSION ${KORE_VERSION_STRING}
		)
		TARGET_LINK_LIBRARIES ( ${KORE_LIBRARY} ${QT_QTCORE_LIBRARY} )
	ENDIF ( MSVC )
# Linux specifics
ELSEIF ( UNIX )
	SET_TARGET_PROPERTIES ( 
		${KORE_LIBRARY} PROPERTIES
		COMPILE_FLAGS -msse
		COMPILE_DEFINITIONS "_K_BUILD_KORE;_KORE_VERSION=\"${KORE_VERSION_STRING}\";_K_UNIX;${DEBUG_DEFINES}"
		VERSION ${KORE_VERSION_STRING}
	)
	TARGET_LINK_LIBRARIES ( ${KORE_LIBRARY} ${QT_QTCORE_LIBRARY} )
ENDIF ( APPLE )

# Documentation
IF ( DOXYGEN_FOUND )
	SET ( DOXYGEN_OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/../doc/html )
	ADD_CUSTOM_COMMAND (
		OUTPUT ${DOXYGEN_OUTPUT}
		COMMAND ${CMAKE_COMMAND} -E echo_append "Building Kore Documentation..."
		COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/../doc/Doxyfile
		COMMAND ${CMAKE_COMMAND} -E echo "Done."
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../doc
	 )
	ADD_CUSTOM_TARGET ( kore-doc DEPENDS ${DOXYGEN_OUTPUT} )
ENDIF ( DOXYGEN_FOUND )
