CXX = g++
FREEGLUT_DIR = C:/freeglut
ARCH = x64  # or 'x86' for 32-bit
CXXFLAGS = -Wall -std=c++17 -g -I$(FREEGLUT_DIR)/include -DFREEGLUT_STATIC
LDFLAGS = -L$(FREEGLUT_DIR)/lib/$(ARCH) -lfreeglut_static -lopengl32 -lglu32 -lwinmm -lgdi32

SRCS = GLUTWork.cpp
OBJS = GLUTWork.o
TARGET = rubik

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LDFLAGS)

GLUTWork.o: GLUTWork.cpp
	$(CXX) $(CXXFLAGS) -c GLUTWork.cpp -o GLUTWork.o

clean:
	del /Q $(OBJS) $(TARGET).exe

.PHONY: all clean


