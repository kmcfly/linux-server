#SDK MAKEFILE 1.0
###########################################################################
DIR:=../..
OBJECT:=$(DIR)/lib/libcommon.a
SOURCES:=$(wildcard *.cpp)

DELETE:=$(patsubst %.cpp,%.o,$(SOURCES))
DELETE+=$(patsubst %.cpp,%.d,$(SOURCES))
###########################################################################
-include $(DIR)/Comdef
-include $(DIR)/build
###########################################################################
