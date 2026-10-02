#pragma once

#include <QtCore/qglobal.h>

#if defined(TOOLBLOCKCONTROL_LIB)
#  define TOOLBLOCKCONTROL_EXPORT Q_DECL_EXPORT
#else
#  define TOOLBLOCKCONTROL_EXPORT Q_DECL_IMPORT
#endif
