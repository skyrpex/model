#-------------------------------------------------
#
# Project created by QtCreator 2012-10-05T09:47:29
#
#-------------------------------------------------

QT       += core gui

TARGET = model
TEMPLATE = app

include(extlibs/QxGraphics/QxGraphics.pri)
include(extlibs/QxMesh/QxMesh.pri)

SOURCES += main.cpp \
    model/JointModel.cpp \
    model/Joint.cpp \
    model/AnimModel.cpp \
    model/Anim.cpp \
    widgets/time/JointProxyModel.cpp \
    widgets/TimeWidget.cpp \
    widgets/AnimsWidget.cpp \
    widgets/time/KeyFrameItem.cpp \
    model/Transform.cpp \
    widgets/time/KeyFramesEditor.cpp \
    widgets/time/KeyFrameMime.cpp \
    model/KeyFrames.cpp \
    widgets/time/JointHeaderView.cpp \
    widgets/time/JointDelegate.cpp \
    dialogs/AnimDialog.cpp \
    Application.cpp \
    widgets/graphics/GraphicsJoint.cpp \
    MainWindow.cpp \
    widgets/GraphicsWidget.cpp

HEADERS  += \
    model/JointModel.h \
    model/Joint.h \
    model/AnimModel.h \
    model/Anim.h \
    widgets/time/JointProxyModel.h \
    widgets/TimeWidget.h \
    widgets/AnimsWidget.h \
    model/KeyFrames.h \
    widgets/time/KeyFrameItem.h \
    model/Transform.h \
    widgets/time/KeyFramesEditor.h \
    Constants.h \
    widgets/time/KeyFrameMime.h \
    widgets/time/JointHeaderView.h \
    widgets/time/JointDelegate.h \
    dialogs/AnimDialog.h \
    Application.h \
    widgets/graphics/GraphicsJoint.h \
    MainWindow.h \
    widgets/GraphicsWidget.h

RESOURCES += \
    model/images/images.qrc

FORMS += \
    dialogs/AnimDialog.ui \
    MainWindow.ui
