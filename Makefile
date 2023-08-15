
CXXFLAGS := -O2 -fopenmp
INCLUDES := -I . -I external/ -I external/raylib/include
LDFLAGS  := -L external/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm

SRC_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(SRC_FILES:.cpp=.o)

TARGET := main.exe

$(TARGET): main.cpp $(OBJ_FILES)
	g++ -o $@ $^ $(CXXFLAGS) $(INCLUDES) $(LDFLAGS)

%.o: %.cpp
	g++ -o $@ -c $< $(CXXFLAGS) $(INCLUDES)

clean:
	rm $(OBJ_FILES) $(TARGET)
