#SDK MAKEFILE 1.0
###########################################################################
DIR:=../..
OBJECT:=$(DIR)/lib/librtpserver.a
SOURCES:=$(wildcard *.cpp)
	

DELETE:=$(patsubst %.cpp,%.o,$(SOURCES))
DELETE+=$(patsubst %.cpp,%.d,$(SOURCES))
###########################################################################
-include $(DIR)/Comdef
-include $(DIR)/build
###########################################################################

ALL:ARLIB $(OBJECT) RM

ARLIB:
	$(OBJECT):$(patsubst %.cpp,%.o,$(SOURCES))
	ar r $@ $^ *.o
	
RM:
	rm ./*.a -f
###########################################################################
.PHONY:clean
clean:
	rm $(DELETE) -f
	rm $(OBJECT) -f
###########################################################################