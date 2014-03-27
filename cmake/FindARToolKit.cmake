# - Find ARToolKit SDK
# Find the native ARToolKit SDK headers and libraries.
#
#  AR_SDK_INCLUDE_DIR - where to find ARToolKit Header, etc.
#  AR_SDK_LIBRARIES    - List of libraries when using OculusSDK.
#  AR_SDK_FOUND        - True if ARToolKit SDK found.

IF (DEFINED ENV{AR_ROOT})
    SET(AR_ROOT "$ENV{AR_ROOT}")
ENDIF()
SET(AR_ROOT
    "${AR_ROOT}"
    CACHE
    PATH
    "Root directory to search for OculusSDK")

# Look for the header file.
FIND_PATH(AR_SDK_INCLUDE_DIR NAMES AR/ar.h HINTS
	${AR_ROOT}/include )

# Append "d" to debug libs on windows platform
IF (WIN32)
	SET(CMAKE_DEBUG_POSTFIX d)
ENDIF()

# Look for the library.
FIND_LIBRARY(AR_SDK_LIBRARY NAMES libAR HINTS ${AR_ROOT}
                                                      ${AR_ROOT}/lib
                                                    )
													
													
FIND_LIBRARY(AR_SDK_LIBRARY_d NAMES libARd HINTS ${AR_ROOT}
                                                      ${AR_ROOT}/lib
                                                    )

MARK_AS_ADVANCED(AR_SDK_LIBRARY)
MARK_AS_ADVANCED(AR_SDK_LIBRARY_d)

SET(AR_SDK_LIBRARIES optimized ${AR_SDK_LIBRARIES} debug ${AR_SDK_LIBRARY_d})

# handle the QUIETLY and REQUIRED arguments and set OCULUS_SDK_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ARToolKit DEFAULT_MSG AR_SDK_LIBRARIES AR_SDK_INCLUDE_DIR)

MARK_AS_ADVANCED(AR_SDK_LIBRARIES AR_SDK_INCLUDE_DIR)
