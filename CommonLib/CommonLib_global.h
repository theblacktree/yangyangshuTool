#ifndef COMMONLIB_GLOBAL_H
#define COMMONLIB_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(COMMONLIB_LIBRARY)
#define COMMONLIB_EXPORT Q_DECL_EXPORT
#else
#define COMMONLIB_EXPORT Q_DECL_IMPORT
#endif

#endif // COMMONLIB_GLOBAL_H
