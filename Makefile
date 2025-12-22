CXX_VERSION := -std=c++23
LINKED_LIBS := -lstdc++exp
FLAGS := $(CXX_VERSION) -ggdb3
INCLUDE := include/

LEXER_PATH := src/lexer
PARSER_PATH := src/parser

CXX_SRC = $(LEXER_PATH)/lexer.cpp $(PARSER_PATH)/parser.cpp
CXX_OBJ = build/parser.o build/lexer.o

DEPENDENCIES = $(LEXER_PATH)/* $(PARSER_PATH)/* 

BUILD_DIR = build

EXE_NAME = kompilator

all: $(EXE_NAME)

$(EXE_NAME): $(CXX_SRC)
	$(CXX) $(FLAGS) -I $(INCLUDE) $(CXX_SRC) -o $@ $(LINKED_LIBS)

# $(CXX_OBJ): $(CXX_SRC)
# 	$(CXX) $(FLAGS) -c $^
# 	mv *.o $(BUILD_DIR)

$(LEXER_PATH)/lexer.cpp: $(LEXER_PATH)/lexer.lex $(PARSER_PATH)/parser.hpp
	flex -o $@ $<

$(PARSER_PATH)/parser.cpp $(PARSER_PATH)/parser.hpp: $(PARSER_PATH)/parser.y
	bison -Wall -d -o $(PARSER_PATH)/parser.cpp $^

clean:
	rm -f $(EXE_NAME)