CXX = g++
CXXFLAGS = -std=c++17 -Ofast -g -Wall -Wextra -Iimgui -Iimgui-sfml

SOURCES = 	imgui/imgui.cpp imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/imgui_widgets.cpp \
			imgui-sfml/imgui-SFML.cpp
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
EXE = main sfml-test pong

SFML = -lsfml-graphics -lsfml-window -lsfml-system
LIBS = -lGL # needed to add opengl due to imgui-sfml

%.o: imgui/%.cpp
	$(CXX) -c -o $@ $^ $(CXXFLAGS) $(LIBS)

%.o: imgui-sfml/%.cpp
	$(CXX) -c -o $@ $^ $(CXXFLAGS) $(LIBS) $(SFML)

pong: pong.cpp $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS) $(SFML)

main: main.cpp $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS) $(SFML)

sfml-test: sfml-test.cpp
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS) $(SFML)

all: $(EXE)

clean:
	rm -f $(EXE) *.o