CXX = g++
CXXFLAGS += -g -MMD


SRCS := $(shell find . -type f -name '*.cpp' -not -path "./build/*")
OBJS=$(SRCS:.cpp=.o)
DEPS=$(SRCS:.cpp=.d)

server: $(OBJS)
	$(CXX) $(CXXFLAGS) -o server $^ -lpthread -lmysqlclient

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c -o $@ $< 

-include $(DEPS)

clean:
	find . -name "*.o" -type f -delete
	find . -name "*.d" -type f -delete
	rm -r server

