# - Find OculusSDK
# Find the native OculusSDK headers and libraries.
#
#  ARUCO_INCLUDE_DIR - where to find OVR.h, etc.
#  ARUCO_LIB    - List of libraries when using OculusSDK.
#  ARUCO_FOUND        - True if OculusSDK found.

IF (DEFINED ENV{ARUCO_ROOT})
  SET(ARUCO_ROOT "$ENV{ARUCO_ROOT}")
ENDIF()
SET(ARUCO_ROOT
  "${ARUCO_ROOT}"
  CACHE
  PATH
  "Root directory to search for OculusSDK")

if (EXISTS "/usr/local")
  set(ARUCO_ROOT "${ARUCO_ROOT}" "/usr/local")
endif()

# Look for the header file.
FIND_PATH(ARUCO_INCLUDE_DIR NAMES aruco.h HINTS
  ${ARUCO_ROOT}/include/aruco )

# Append "d" to debug libs on windows platform
IF (WIN32)
  SET(CMAKE_DEBUG_POSTFIX d)
ENDIF()


IF (WIN32)
  # Look for the library.
  FIND_LIBRARY(ARUCO_LIBRARY NAMES aruco125 HINTS ${ARUCO_ROOT}
    ${ARUCO_ROOT}/lib)

  # This will find release lib on Linux if no debug is available - on Linux this is no problem and avoids
  # having to compile in debug when not needed
  FIND_LIBRARY(ARUCO_LIBRARY_d NAMES aruco125 HINTS
    ${ARUCO_ROOT}/lib)
ENDIF()

IF(UNIX)
  FIND_LIBRARY(ARUCO_LIBRARY NAMES libaruco.so HINTS
    ${ARUCO_ROOT}/lib)

  FIND_LIBRARY(ARUCO_LIBRARY_d NAMES libaruco.so HINTS
    ${ARUCO_ROOT}/lib)
ENDIF()

MARK_AS_ADVANCED(ARUCO_LIBRARY)
MARK_AS_ADVANCED(ARUCO_LIBRARY_d)

SET(ARUCO_LIB optimized ${ARUCO_LIBRARY} debug ${ARUCO_LIBRARY_d})

IF(UNIX)
  SET(ARUCO_LIB ${ARUCO_LIB} X11 Xinerama udev)
ELSE(UNIX)
  SET(ARUCO_LIB ${ARUCO_LIB})
ENDIF(UNIX)

# handle the QUIETLY and REQUIRED arguments and set ARUCO_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Aruco DEFAULT_MSG ARUCO_LIB ARUCO_INCLUDE_DIR)

MARK_AS_ADVANCED(ARUCO_LIB ARUCO_INCLUDE_DIR)
