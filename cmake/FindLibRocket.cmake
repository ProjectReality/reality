# Based on the FindRocket.cmake script
# - Try to find libRocket
# Once done this will define
#
# ROCKET_FOUND - system has Rocket
# ROCKET_INCLUDE_DIR - the Rocket include directory
# ROCKET_LIBRARY - Link these to use Rocket

set(_rocket_INCLUDE_SEARCH_DIRS_SYSTEM
    C:/Rocket/include
    C:/Rocket
    $ENV{ROCKETDIR}
    "$ENV{ProgramFiles}/Rocket/include"
    "$ENV{ProgramFiles}/Rocket"
    /sw/local/include
  )

set(_rocket_LIB_SEARCH_DIRS_SYSTEM
    C:/Rocket/lib
    C:/Rocket
    $ENV{ROCKETDIR}
    "$ENV{ProgramFiles}/Rocket/lib"
    "$ENV{ProgramFiles}/Rocket"
    /sw/local/lib
  )

FIND_PATH(ROCKET_INCLUDE_DIR Rocket/Core.h
$ENV{ROCKETDIR}/include
$ENV{ROCKETDIR}
$ENV{ROCKETDIR}/..
$ENV{ROCKETDIR}/../Include
~/Library/Frameworks/Rocket.framework/Headers
/Library/Frameworks/Rocket.framework/Headers
/usr/local/include/Rocket
/usr/local/include
/usr/include/Rocket
/usr/include
/sw/include/Rocket # Fink
/sw/include
/opt/local/include/Rocket # DarwinPorts
/opt/local/include
/opt/csw/include/Rocket # Blastwave
/opt/csw/include
/opt/include/Rocket
/opt/include
${_rocket_INCLUDE_SEARCH_DIRS_SYSTEM}
)

FIND_LIBRARY(ROCKET_LIBRARY_CORE
NAMES RocketCore
PATHS
$ENV{ROCKETDIR}/RocketCore/Release
$ENV{ROCKETDIR}/lib
/usr/local/lib
/usr/lib
/sw/lib
/opt/local/lib
/opt/csw/lib
/opt/lib
${_rocket_LIB_SEARCH_DIRS_SYSTEM}
)

FIND_LIBRARY(ROCKET_LIBRARY_CONTROLS
NAMES RocketControls
PATHS
$ENV{ROCKETDIR}
$ENV{ROCKETDIR}/lib
/usr/local/lib
/usr/lib
/sw/lib
/opt/local/lib
/opt/csw/lib
/opt/lib
${_rocket_LIB_SEARCH_DIRS_SYSTEM}
)

FIND_LIBRARY(ROCKET_LIBRARY_DEBUG
NAMES RocketDebugger
PATHS
$ENV{ROCKETDIR}
$ENV{ROCKETDIR}/lib
/usr/local/lib
/usr/lib
/sw/lib
/opt/local/lib
/opt/csw/lib
/opt/lib
${_rocket_LIB_SEARCH_DIRS_SYSTEM}
)


SET(ROCKET_FOUND "NO")
IF(ROCKET_INCLUDE_DIR AND ROCKET_LIBRARY_CORE)
SET(ROCKET_FOUND "YES")
ENDIF(ROCKET_INCLUDE_DIR AND ROCKET_LIBRARY_CORE)

SET (ROCKET_LIBRARIES ${ROCKET_LIBRARY_CORE} ${ROCKET_LIBRARY_CONTROLS} ${ROCKET_LIBRARY_DEBUG})

mark_as_advanced(ROCKET_LIBRARIES ROCKET_INCLUDE_DIR)
