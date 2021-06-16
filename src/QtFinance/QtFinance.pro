NAME         = QtFinance
TARGET       = $${NAME}

QT           = core
QT          -= gui
QT          += network
QT          += sql
QT          += script
QT          += QtMtAPI
QT          += Essentials
QT          += Mathematics
QT          += QtFTP
QT          += QtRPC

load(qt_build_config)
load(qt_module)

INCLUDEPATH += $${PWD}/../../include/$${NAME}

HEADERS     += $${PWD}/../../include/$${NAME}/qtfinance.h

include ($${PWD}/Monetary/Monetary.pri)
include ($${PWD}/Connectors/Connectors.pri)
include ($${PWD}/Managers/Managers.pri)

OTHER_FILES += $${PWD}/../../include/$${NAME}/headers.pri

include ($${PWD}/../../doc/Qt/Qt.pri)

TRNAME       = $${NAME}
include ($${PWD}/../../Translations.pri)
