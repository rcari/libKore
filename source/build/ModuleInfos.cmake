# Kore module infos

SET( MODULE_NAME "Kore")
STRING( TOUPPER ${MODULE_NAME} MODULE_VAR_NAME )

SET( ${MODULE_VAR_NAME}_VERSION_MAJOR	0 )
SET( ${MODULE_VAR_NAME}_VERSION_MINOR	0 )
SET( ${MODULE_VAR_NAME}_VERSION_PATCH	1 )
SET( ${MODULE_VAR_NAME}_VERSION_BUILD	0 )
SET( ${MODULE_VAR_NAME}_VERSION_STRING
        "${${MODULE_VAR_NAME}_VERSION_MAJOR}.${${MODULE_VAR_NAME}_VERSION_MINOR}.${${MODULE_VAR_NAME}_VERSION_PATCH}.${${MODULE_VAR_NAME}_VERSION_BUILD}" )

GET_FILENAME_COMPONENT( ${MODULE_VAR_NAME}_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/../.. ABSOLUTE )
SET( ${MODULE_VAR_NAME}_BUILD_DIR	${CMAKE_CURRENT_BINARY_DIR}/pixelfr0g/modules/${MODULE_NAME} )
SET( ${MODULE_VAR_NAME}_LIBRARY		${MODULE_NAME}${${MODULE_VAR_NAME}_VERSION_MAJOR} )
SET( ${MODULE_VAR_NAME}_INCLUDES
        ${QT_INCLUDES}                          # Qt dependencies
        ${CMAKE_CURRENT_LIST_DIR}/../inc	# Headers
        ${CMAKE_CURRENT_LIST_DIR}/../		# Templates
)
SET( ${MODULE_VAR_NAME}_DEFINITIONS ${QT_DEFINITIONS} )
SET( ${MODULE_VAR_NAME}_QT_RESOURCES_FILE ${${MODULE_VAR_NAME}_ROOT_DIR}/resources/kore.qrc )

# For parent scope inclusion
SET(
        ${MODULE_VAR_NAME}_EXPORT_VARIABLES

        # Version
        ${MODULE_VAR_NAME}_VERSION_MAJOR
        ${MODULE_VAR_NAME}_VERSION_MINOR
        ${MODULE_VAR_NAME}_VERSION_PATCH
        ${MODULE_VAR_NAME}_VERSION_BUILD
        ${MODULE_VAR_NAME}_VERSION_STRING

        # Include paths and build params
        ${MODULE_VAR_NAME}_ROOT_DIR
        ${MODULE_VAR_NAME}_BUILD_DIR
        ${MODULE_VAR_NAME}_INCLUDES
        ${MODULE_VAR_NAME}_LIBRARY
        ${MODULE_VAR_NAME}_DEFINITIONS
        ${MODULE_VAR_NAME}_QT_RESOURCES_FILE

        # Export itself
        ${MODULE_VAR_NAME}_EXPORT_VARIABLES
)

FOREACH( VAR ${${MODULE_VAR_NAME}_EXPORT_VARIABLES} )
        SET( ${VAR} ${${VAR}} PARENT_SCOPE )
ENDFOREACH( VAR ${${MODULE_VAR_NAME}_EXPORT_VARIABLES} )
