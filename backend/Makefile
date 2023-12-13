CXX = g++-13
EXECUTABLE = compiler
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
			-Wstack-usage=8192 -fPIE -Werror=vla
COMMON_DIR = ../common
SOURCES = main.cpp compiler.cpp read_file_in_buffer.cpp
OUT_SOURCES = ../common/log_funcs.cpp
EXECUTABLE = compiler.out

OBJ_DIR = obj

OBJECTS = $(addprefix $(OBJ_DIR)/, $(SOURCES:%.cpp=%.o))
OUT_OBJECTS = $(addprefix $(OBJ_DIR)/, $(OUT_SOURCES:$(COMMON_DIR)/%.cpp=%.o))

ERR_DIR = errors
ERROR_FILES = $(SOURCES:%.cpp=$(ERR_DIR)/%.err)

.PHONY: all clean

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(OUT_OBJECTS)
	$(CXX) $^ -o $@ $(CXXFLAGS)

$(OBJ_DIR)/%.o: $(COMMON_DIR)/%.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(OBJ_DIR)/%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

$(ERR_DIR):
	mkdir -p $(ERR_DIR)

$(ERR_DIR)/%.err: %.cpp
	$(CXX) -c $< -o /dev/null $(CXXFLAGS) 2> $@

run: $(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	rm -rf $(EXECUTABLE) $(OBJ_DIR) $(ERR_DIR)
