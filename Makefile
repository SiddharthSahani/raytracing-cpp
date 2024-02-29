
CXXFLAGS = -O2 -fopenmp
INCLUDES = -I . -I external/ -I external/raylib/include

SRC_FILES = $(wildcard src/*.cpp)
OBJ_FILES = $(SRC_FILES:.cpp=.o)

TARGET = raytracing_cpp.exe


ifeq ($(STANDALONE), y)
	LDFLAGS = 
	DEFINES = -DRT_NO_RAYLIB
else
	LDFLAGS = -L external/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm
	DEFINES = 
endif


$(TARGET): main.cpp $(OBJ_FILES)
	g++ -o $@ $^ $(DEFINES) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS)

%.o: %.cpp
	g++ -o $@ -c $< $(DEFINES) $(CXXFLAGS) $(INCLUDES)

clean:
	rm $(OBJ_FILES) $(TARGET)
