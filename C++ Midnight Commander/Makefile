CXX = g++
FLAGS = -Wall -pedantic -Wno-long-long -O0 -ggdb -c -g -std=gnu++11

CURSES = -lncurses

.PHONY: all
all: compile doc

.PHONY: compile
compile: schejond

.PHONY: run
run: schejond
	./schejond

.PHONY: doc
doc: 
	doxygen Doxyfile

.PHONY: clean
clean:
	rm -rf build
	rm -f schejond
	rm -rf doc

schejond: build/Design_Maker.o build/File.o build/Files_Wrapper.o build/Folder.o build/Item.o build/Link.o build/main.o build/Window.o
	mkdir -p build
	$(CXX) $^ -o $@ $(CURSES)

build/%.o: src/%.cpp
	mkdir -p build
	$(CXX) $(FLAGS) $< -o $@ $(CURSES)

build/Design_Maker.o: src/Design_Maker.cpp src/Design_Maker.hpp src/Popup_Type.hpp src/Item.hpp
build/File.o: src/File.cpp src/File.hpp src/Item.hpp
build/Files_Wrapper.o: src/Files_Wrapper.cpp src/Files_Wrapper.hpp src/Item.hpp src/File.hpp src/Folder.hpp src/Link.hpp src/Regex_Matcher.hpp \
 src/Find_Link_Target.hpp src/Fill_Vector_With_Sorted_Names.hpp
build/Folder.o: src/Folder.cpp src/Folder.hpp src/Item.hpp
build/Item.o: src/Item.cpp src/Item.hpp
build/Link.o: src/Link.cpp src/Link.hpp src/Item.hpp
build/main.o: src/main.cpp src/Window.hpp src/Design_Maker.hpp src/Popup_Type.hpp src/Item.hpp src/Files_Wrapper.hpp src/File.hpp \
 src/Folder.hpp src/Link.hpp
build/Window.o: src/Window.cpp src/Window.hpp src/Design_Maker.hpp src/Popup_Type.hpp src/Item.hpp src/Files_Wrapper.hpp src/File.hpp \
 src/Folder.hpp src/Link.hpp src/Popup_Decision.hpp src/Check_Regex_Syntax.hpp src/Input_Checks.hpp