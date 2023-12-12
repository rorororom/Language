CXX = g++-13
CXXFLAGS =  -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations \
			-Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts       \
			-Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal  \
			-Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline          \
			-Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked            \
			-Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo  \
			-Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn                \
			-Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default      \
			-Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast           \
			-Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing   \
			-Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation    \
			-fstack-protector -fstrict-overflow -fno-omit-frame-pointer -Wstack-usage=8192         \
			-fsanitize=address,leak,undefined -fPIE -Werror=vla
COMMON_DIR = ../common
SOURCES = main.cpp build_tree.cpp
OUT_SOURCES = $(COMMON_DIR)/log_funcs.cpp $(COMMON_DIR)/write_in_file.cpp $(COMMON_DIR)/read_from_file_in_buffer.cpp  $(COMMON_DIR)/differentiation.cpp $(COMMON_DIR)/print_tree.cpp
OBJ_DIR = obj
OBJECTS = $(addprefix $(OBJ_DIR)/, $(SOURCES:%.cpp=%.o))
OUT_OBJECTS = $(addprefix $(OBJ_DIR)/, $(OUT_SOURCES:$(COMMON_DIR)/%.cpp=%.o))
EXECUTABLE = middleend.out

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(OUT_OBJECTS)
	$(CXX) $^ -o $@ $(CXXFLAGS)

$(OBJ_DIR)/main.o: main.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(EXECUTABLE): $(OBJECTS) $(OUT_OBJECTS)
	$(CXX) $^ -o $@ $(CXXFLAGS)

$(OBJ_DIR)/main.o: main.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/log_funcs.o: $(COMMON_DIR)/log_funcs.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/write_in_file.o: $(COMMON_DIR)/write_in_file.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/differentiation.o: $(COMMON_DIR)/differentiation.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/read_from_file_in_buffer.o: $(COMMON_DIR)/read_from_file_in_buffer.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/print_tree.o: $(COMMON_DIR)/print_tree.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

run:
	./$(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE) $(OBJECTS) $(OUT_OBJECTS)
