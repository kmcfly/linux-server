###########################################################################
#CXX=arm-hisiv100-linux-g++ 
#CC=$(CXX)
###########################################################################
DIR:=../../..
OBJECT:=$(DIR)/product/plugin/libonvifdevice.so
SOURCES:=$(wildcard *.cpp)
SOURCES+=$(wildcard ./public/*.cpp)
SOURCES+=$(wildcard ../rtsp/*.cpp)
SOURCES+=$(wildcard ./utility/*.cpp)
SOURCES+=$(wildcard ../netdevice/*.cpp)

DELETE:=$(patsubst %.cpp,%.o,$(SOURCES))
DELETE+=$(patsubst %.cpp,%.d,$(SOURCES))

###########################################################################
-include $(DIR)/project/Comdef
###########################################################################

override CXXFLAGS+=-D__ENVIRONMENT_LINUX__
override CXXFLAGS+=-I$(DIR)/project/include/ONVIFNetDevice/ -I$(DIR)/project/include/netdevice -I$(DIR)/project/include/ONVIFNetDevice/public -I$(DIR)/project/include/rtsp -I$(DIR)/project/include/ONVIFNetDevice/utility -I$(DIR)/project/include/common -I$(DIR)/project/include/common/openssl -I$(DIR)/project/include/upnpapp
override CFLAGS+=$(CXXFLAGS)
override LDFLAGS+=-rdynamic
override MAKEFLAGS += -s
############################################################################
$(OBJECT):$(patsubst %.cpp,%.o,$(SOURCES))
	$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) -fpic -shared $^ -L$(DIR)/project/lib -lssl -lcrypto -luuid -lmem
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
