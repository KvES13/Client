#-------------------------------------------------
#
# Project created by QtCreator 2019-04-07T18:49:41
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = uclient
TEMPLATE = app

prj_dir = $${_PRO_FILE_PWD_}
bld_dir = $${prj_dir}/../bld
src_dir = $${prj_dir}
gui_dir = $${prj_dir}
cfg_dir = $${prj_dir}/../exec/data
res_dir = $${prj_dir}/res

INCLUDEPATH += $${src_dir}
INCLUDEPATH += $${src_dir}/statistics


DEFINES += QT_DEPRECATED_WARNINGS

MOC_DIR     = $${bld_dir}/moc
OBJECTS_DIR = $${bld_dir}/obj
RCC_DIR     = $${bld_dir}/rcc
UI_DIR      = $${bld_dir}/gui
DESTDIR = $${prj_dir}/../exec

CONFIG += c++17

win32 {
    SOURCES     += $$system("where /r .\ *.c* 2> nul")
    HEADERS     += $$system("where /r .\ *.h* 2> nul")
    FORMS       += $$system("where /r .\ *.ui 2> nul")
} else {
    SOURCES     += $$system("find '$${src_dir}/' -name '*.c*' 2>/dev/null")
    HEADERS     += $$system("find '$${src_dir}/' -name '*.h*' 2>/dev/null")
    FORMS       += $$system("find '$${gui_dir}/' -name '*.ui' 2>/dev/null")
}

