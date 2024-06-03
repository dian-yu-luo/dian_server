CXX = g++
CXXFLAGS = -Wall -std=c++17 -g
INC_DIR = -I./
LIBS = 
SRC_DIR = ./
OBJ_DIR = ./obj
TARGET = main

SRCS = csapp.cpp dian_log.cpp echoserver.cpp smart_chat.cpp
OBJS = $(addprefix $(OBJ_DIR)/, $(SRCS:.cpp=.o))

all: $(OBJ_DIR) $(OBJS) main 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INC_DIR) -c -o $@ $<

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

main: $(OBJS) main.cpp 
	$(CXX) $(CXXFLAGS) $(INC_DIR) -o main main.cpp obj/csapp.o obj/dian_log.o obj/smart_chat.o 

echoserver: $(OBJS) echoserver.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIR) -o echoserver echoserver.cpp obj/csapp.o 

echoclient: $(OBJS) echoclient.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIR) -o echoclient echoclient.cpp obj/csapp.o 
adder: $(OBJS) adder.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIR) -o adder adder.cpp obj/csapp.o 
tiny: $(OBJS) tiny.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIR) -o tiny tiny.cpp obj/csapp.o 
select: $(OBJS) select.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIR) -o select select.cpp obj/csapp.o 

.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJ_DIR)/*.o

