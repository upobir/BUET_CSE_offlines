CC = g++
CPPFLAGS = -std=c++17 -g -MMD

DIR = tests
SRC := $(notdir $(wildcard $(DIR)/*.cpp))

OBJ_DIR = tests/objs
OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))

BIN_DIR = tests/bin

vpath %.cpp $(DIR)

dummy:
	echo $(OBJ_DIR)


build: $(BIN_DIR)/main.exe

$(BIN_DIR)/main.exe: $(OBJ)
	$(CC) $(CPPFLAGS) -o $@ $(OBJ_DIR)/*.o

$(OBJ_DIR)/%.o : %.cpp
	$(CC) $(CPPFLAGS) -c -o $@ $<

clean:
	rm $(OBJ_DIR)/* $(BIN_DIR)/*

include $(wildcard $(OBJ_DIR)/*.d)

.SUFFIXES: