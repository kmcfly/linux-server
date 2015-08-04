###########################################################################
#CXX=arm-hisiv100-linux-g++ 
#CC=$(CXX)
###########################################################################
DIR:=../../..
OBJECT:=$(DIR)/product/plugin/libtsnetdevice.so
SOURCES:=$(wildcard *.cpp)
SOURCES+=$(wildcard ../netdevice/*.cpp)

DELETE:=$(patsubst %.cpp,%.o,$(SOURCES))
DELETE+=$(patsubst %.cpp,%.d,$(SOURCES))

###########################################################################
-include $(DIR)/project/Comdef
###########################################################################

override CXXFLAGS+=-D__ENVIRONMENT_LINUX__
override CXXFLAGS+=-I$(DIR)/project/include/TSNetDevice/ -I$(DIR)/project/include/netdevice
override CFLAGS+=$(CXXFLAGS)
override LDFLAGS+=-rdynamic
override MAKEFLAGS += -s
############################################################################
$(OBJECT):$(patsubst %.cpp,%.o,$(SOURCES))
	$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) -fpic -shared $^ -L../../lib
###########################################################################
%.d:%.cpp
	$(CXX) -MM $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

-include $(SOURCES:.cpp=.d)
###########################################################################
.PHONY:clean
clean:
	rm $(DELETE) -f
	rm $(OBJECT) -f
