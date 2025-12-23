CXX_VERSION := -std=c++23
LINKED_LIBS := -lstdc++exp
FLAGS := $(CXX_VERSION) -ggdb3
INCLUDE := include/

SRC_PATH = src

LEXER_PATH := $(SRC_PATH)/lexer
PARSER_PATH := $(SRC_PATH)/parser

CXX_SRC = \
	$(SRC_PATH)/*.cpp \
	$(SRC_PATH)/input/*.cpp \
	$(LEXER_PATH)/*.cpp \
	$(PARSER_PATH)/*.cpp \

EXE_NAME = kompilator

all: $(EXE_NAME) vm

$(EXE_NAME): $(CXX_SRC)
	$(CXX) $(FLAGS) -I $(INCLUDE) $(CXX_SRC) -o $@ $(LINKED_LIBS)

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
	$(MAKE) clean -C mw2025/