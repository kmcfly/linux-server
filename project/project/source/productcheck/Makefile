#SDK MAKEFILE 1.0
###########################################################################
DIR:=../..
OBJECT:=$(DIR)/../product/productcheck
-include $(DIR)/Comdef
	temp =$(subst -fopenmp, ,$(CXXFLAGS))
ALL:
	rm $(OBJECT) -rf
	$(CXX) -o $(OBJECT) $(temp) check.c


.PHONY:clean
clean:
	rm $(OBJECT) -rf	
