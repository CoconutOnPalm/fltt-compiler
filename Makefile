CXX_VERSION := -std=c++23
LINKED_LIBS := -lstdc++exp
FLAGS := $(CXX_VERSION) -ggdb3 -fconcepts-diagnostics-depth=2

BUILD_DIR := build
SRC_PATH = src

LEXER_PATH := $(SRC_PATH)/lexer
PARSER_PATH := $(SRC_PATH)/parser

# CXX_SRC = \
# 	$(SRC_PATH)/*.cpp \
# 	$(SRC_PATH)/input/*.cpp \
# 	$(SRC_PATH)/procedure/*.cpp \
# 	$(SRC_PATH)/symbol/*.cpp \
# 	$(SRC_PATH)/AST/*.cpp \
# 	$(SRC_PATH)/TAC/*.cpp \
# 	$(PARSER_PATH)/*.cpp \
# 	$(LEXER_PATH)/*.cpp \

CXX_SRC = \
	$(wildcard $(SRC_PATH)/*.cpp) \
	$(wildcard $(SRC_PATH)/input/*.cpp) \
	$(wildcard $(SRC_PATH)/procedure/*.cpp) \
	$(wildcard $(SRC_PATH)/symbol/*.cpp) \
	$(wildcard $(SRC_PATH)/AST/*.cpp) \
	$(wildcard $(SRC_PATH)/TAC/*.cpp) \
	$(wildcard $(SRC_PATH)/ASM/*.cpp) \
	$(wildcard $(SRC_PATH)/ASM/memory/*.cpp) \
# 	$(wildcard $(PARSER_PATH)/*.cpp) \
# 	$(wildcard $(LEXER_PATH)/*.cpp) \

CXX_SRC += $(PARSER_PATH)/parser.cpp $(LEXER_PATH)/lexer.cpp

# CXX_HEAD = \
# 	$(SRC_PATH)/*.hpp \
# 	$(SRC_PATH)/utils/*.hpp \
# 	$(SRC_PATH)/input/*.hpp \
# 	$(SRC_PATH)/procedure/*.hpp \
#  	$(SRC_PATH)/symbol/*.hpp \
#  	$(SRC_PATH)/AST/*.hpp \
#  	$(SRC_PATH)/AST/blocks/*.hpp \
#  	$(SRC_PATH)/TAC/*.hpp \
#  	$(SRC_PATH)/TAC/codes/*.hpp \
#  	$(SRC_PATH)/TAC/codes/expressions/*.hpp \
#  	$(PARSER_PATH)/*.hpp \

CXX_HEAD = \
	$(wildcard $(SRC_PATH)/*.hpp) \
	$(wildcard $(SRC_PATH)/utils/*.hpp) \
	$(wildcard $(SRC_PATH)/input/*.hpp) \
	$(wildcard $(SRC_PATH)/procedure/*.hpp) \
	$(wildcard $(SRC_PATH)/symbol/*.hpp) \
	$(wildcard $(SRC_PATH)/AST/*.hpp) \
	$(wildcard $(SRC_PATH)/AST/blocks/*.hpp) \
	$(wildcard $(SRC_PATH)/AST/blocks/expressions/*.hpp) \
	$(wildcard $(SRC_PATH)/TAC/*.hpp) \
	$(wildcard $(SRC_PATH)/TAC/codes/*.hpp) \
	$(wildcard $(SRC_PATH)/TAC/codes/expressions/*.hpp) \
	$(wildcard $(SRC_PATH)/TAC/codes/comparisons/*.hpp) \
	$(wildcard $(SRC_PATH)/TAC/codes/special/*.hpp) \
	$(wildcard $(SRC_PATH)/TAC/codes/elementary/*.hpp) \
	$(wildcard $(SRC_PATH)/TAC/codes/jump/*.hpp) \
	$(wildcard $(SRC_PATH)/TAC/codes/other/*.hpp) \
	$(wildcard $(SRC_PATH)/ASM/*.hpp) \
	$(wildcard $(SRC_PATH)/ASM/instructions/*.hpp) \
	$(wildcard $(SRC_PATH)/ASM/memory/*.hpp) \
	$(wildcard $(PARSER_PATH)/*.hpp) \


CXX_OBJ = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(CXX_SRC))


EXE_NAME = kompilator

all: lexer parser $(EXE_NAME) vm

lexer: $(LEXER_PATH)/lexer.cpp
parser: $(PARSER_PATH)/parser.cpp

$(EXE_NAME): $(CXX_OBJ)
	$(CXX) $(FLAGS) $^ -o $@ $(LINKED_LIBS)


$(BUILD_DIR)/%.o: %.cpp $(CXX_HEAD)
	@mkdir -p $(dir $@)
	$(CXX) $(FLAGS) -c $< -o $@

# $(EXE_NAME): $(CXX_SRC) $(CXX_HEAD) $(LEXER_PATH)/lexer.lex $(PARSER_PATH)/parser.y
# 	$(CXX) $(FLAGS) -I $(INCLUDE) $(CXX_SRC) -o $@ $(LINKED_LIBS)

# $(CXX_OBJ): $(CXX_SRC)
# 	$(CXX) $(FLAGS) -c $^
# 	mv *.o $(BUILD_DIR)

$(LEXER_PATH)/lexer.cpp: $(LEXER_PATH)/lexer.lex $(PARSER_PATH)/parser.hpp
	flex -o $@ $<

$(PARSER_PATH)/parser.cpp $(PARSER_PATH)/parser.hpp: $(PARSER_PATH)/parser.y
	bison -Wall -d -o $(PARSER_PATH)/parser.cpp $^

vm:
	$(MAKE) -C mw2025/

clean:
	rm -f $(EXE_NAME)
	rm -f src/parser/parser.cpp src/parser/parser.hpp
	rm -f src/lexer/lexer.cpp
	rm -rf ./build/
	$(MAKE) clean -C mw2025/