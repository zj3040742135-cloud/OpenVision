#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(PROCESS_VISUALIZATION_LIB)
#  define PROCESS_VISUALIZATION_EXPORT Q_DECL_EXPORT
# else
#  define PROCESS_VISUALIZATION_EXPORT Q_DECL_IMPORT
# endif
#else
# define PROCESS_VISUALIZATION_EXPORT
#endif
