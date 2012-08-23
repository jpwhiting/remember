# Add more folders to ship with the application, here
folder_01.source = qml/remember
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

CONFIG += link_pkgconfig
PKGCONFIG += QJson
DEFINES += QTONLY

QMAKE_LFLAGS += -Wl,--rpath=/opt/remember/lib
LIBS += rtm/librtm.so
PRE_TARGETDEPS += rtm/librtm.so

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
CONFIG += qdeclarative-boostable

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += src/main.cpp \
    src/rememberapp.cpp \
    src/filteredtasksmodel.cpp \
    src/listsmodel.cpp \
    src/tasksmodel.cpp

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog

HEADERS += \
    src/apikey.h \
    src/rememberapp.h \
    src/filteredtasksmodel.h \
    src/listsmodel.h \
    src/tasksmodel.h
