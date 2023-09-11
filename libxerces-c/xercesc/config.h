/* file      : xercesc/config.h -*- C -*-
 * license   : Apache License 2.0; see accompanying LICENSE file
 */

#ifndef XERCESC_CONFIG_H
#define XERCESC_CONFIG_H

/* For the semantics of the following macros refer to config.h.cmake.in.orig
 * and upstream's configure.ac.
 *
 * Note that we will explicitly undefine macros that are present in the
 * libxerces-c source code but should not be defined. While this is not
 * technically required, it simplifies the change tracking (see README-DEV).
 * As a bonus we also make sure that they are not get eventually defined by
 * some system headers.
 */

/* SIZE_MAX.
 */
#if defined(__cplusplus)
#  include <cstdint>
#else
#  include <stdint.h> /* Requires C99. */
#endif

/* Transcoder.
 *
 * Note that depending on config.libxerces_c.transcoder_icu and
 * cxx.target.class values one of the following macros is defined via the
 * preprocessor option (see buildfile and root.build for details).
 */
/*
#define XERCES_USE_TRANSCODER_ICU                   1
#undef  XERCES_USE_TRANSCODER_MACOSUNICODECONVERTER
#undef  XERCES_USE_TRANSCODER_WINDOWS
*/
#undef  XERCES_USE_TRANSCODER_GNUICONV
#undef  XERCES_USE_TRANSCODER_ICONV

/* Net accessor.
 *
 * Note that the XERCES_USE_NETACCESSOR_CURL macro is defined via the
 * preprocessor option if the network support is enabled (see buildfile and
 * root.build for details).
 */
/*
#define XERCES_USE_NETACCESSOR_CURL    1
*/
#undef  XERCES_USE_NETACCESSOR_CFURL
#undef  XERCES_USE_NETACCESSOR_SOCKET
#undef  XERCES_USE_NETACCESSOR_WINSOCK

/* Use the in-memory message loader.
 */
#define XERCES_USE_MSGLOADER_INMEMORY 1
#undef  XERCES_USE_MSGLOADER_ICONV
#undef  XERCES_USE_MSGLOADER_ICU

/* Use the standard mutex manager.
 */
#define XERCES_USE_MUTEXMGR_STD      1
#undef  XERCES_USE_MUTEXMGR_NOTHREAD
#undef  XERCES_USE_MUTEXMGR_POSIX
#undef  XERCES_USE_MUTEXMGR_WINDOWS

/* Use the OS-specific file manager.
 */
#ifndef _WIN32
#  define XERCES_USE_FILEMGR_POSIX   1
#else
#  define XERCES_USE_FILEMGR_WINDOWS 1
#endif

/* Specific for FreeBSD.
 */
#ifdef __FreeBSD__
#  define HAVE_CPUID_H         1
#  define XERCES_HAVE_GETCPUID 1
#endif

/* Specific for POSIX.
 */
#ifndef _WIN32
#  define HAVE_GETTIMEOFDAY 1
#  define HAVE_GMTIME_R     1
#  define HAVE_SNPRINTF     1

/* Is not specified by the POSIX standard but is present on all POSIX
 * platforms we support.
 */
#  define HAVE_TIMEGM       1

/* Specific for Windows.
 */
#else
#  define HAVE_SYS_TIMEB_H                1
#  define HAVE_FTIME                      1
#  define HAVE_STRICMP                    1
#  define HAVE_STRNICMP                   1
#  define HAVE_WCSICMP                    1
#  define HAVE_WCSLWR                     1
#  define HAVE_WCSNICMP                   1
#  define HAVE_WCSUPR                     1
#  define XERCES_HAVE_INTRIN_H            1
#  define XERCES_PATH_DELIMITER_BACKSLASH 1
#  define XERCES_HAVE_CPUID_INTRINSIC     1
#endif

/* Specific for (non-) VC.
 */
#ifndef _MSC_VER
#  define HAVE_PATH_MAX    1
#  define HAVE_STRCASECMP  1
#  define HAVE_STRNCASECMP 1
#  define HAVE_STRINGS_H   1
#  define HAVE_STRING_H    1
#  define HAVE_SYS_TIME_H  1
#  define HAVE_UNISTD_H    1
#endif

/* Common for all supported OSes/compilers.
 */
#define HAVE_ERRNO_H             1
#define HAVE_LIMITS_H            1
#define HAVE_SYS_TYPES_H         1
#define XERCES_HAS_CPP_NAMESPACE 1
#define XERCES_STD_NAMESPACE     1
#define XERCES_LSTRSUPPORT       1
#define XERCES_NEW_IOSTREAMS     1
#define XERCES_SIZE_MAX          SIZE_MAX

#undef XERCES_HAVE_EMMINTRIN_H
#undef XERCES_HAVE_SSE2_INTRINSIC
#undef XERCES_NO_MATCHING_DELETE_OPERATOR
#undef XERCES_NO_NATIVE_BOOL

/* Are also defined in Xerces_autoconf_config.hpp which is supposedly enough.
 * Note that Xerces_autoconf_config.hpp is expected to be (indirectly)
 * included into all public headers to properly export/import the API
 * functions.

#define XERCES_PLATFORM_EXPORT
#define XERCES_PLATFORM_IMPORT
 */

/* We can probably assume that on platforms we build for, these keywords
 * doesn't require definition.

#undef const
#undef inline
*/

#endif /* XERCESC_CONFIG_H */
