TARGET=genfir
CXXFLAGS=-O0 -Wall -g3
LDFLAGS=-lfftw3f
CXX=g++

SRC=genfir.cpp InWaveFile.cpp
OBJ=$(SRC:.cpp=.o)

$(TARGET) : $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LDFLAGS)

include $(SRC:.cpp=.d)

%.o : 
	$(CXX) -c -o $@ $(CXXFLAGS) $<

%.d: %.cpp
	set -e; rm -f $@; \
	$(CXX) -MM $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

clean :
	rm -f $(OBJ)
	rm -f $(TARGET)

