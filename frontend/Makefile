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
			-fstack-protector -fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192         \
			-Wstack-usage=8192 -fsanitize=address,leak,undefined -fPIE -Werror=vla
COMMON_DIR = ../common
SOURCES = main.cpp recurs.cpp
OUT_SOURCES = log_funcs.cpp read_from_file_in_buffer.cpp write_in_file.cpp print_tree.cpp differentiation.cpp
OBJECTS = $(SOURCES:%.cpp=$(OBJ_DIR)/%.o)
OUT_OBJECTS = $(OUT_SOURCES:%.cpp=$(OBJ_DIR)/%.o)
OBJ_DIR = obj
EXECUTABLE = frontend.out

OBJECTS = $(addprefix $(OBJ_DIR)/, $(SOURCES:%.cpp=%.o))

.PHONY: all clean generate_image

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(OUT_OBJECTS)
	$(CXX) $^ -o $@ $(CXXFLAGS)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/%.o: $(COMMON_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) -c $< -o $@ $(CXXFLAGS)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	rm -rf $(OBJ_DIR) $(EXECUTABLE)
