TEMPLATE = subdirs

SUBDIRS = ThermusLib MainApp

CONFIG += ordered

ThermusLib.subdir = ThermusLib
MainApp.subdir    = MainApp

MainApp.depends = ThermusLib
