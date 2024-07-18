CXX = g++
CXXFLAGS += -g -MMD


SRCS := $(shell find . -type f -name '*.cpp' -not -path "./build/*" -not -path "./test/*")
OBJS=$(SRCS:.cpp=.o)
DEPS=$(SRCS:.cpp=.d)

server: $(OBJS)
	$(CXX) $(CXXFLAGS) -o server $^ -lpthread -lmysqlclient

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c -o $@ $< 
# 下面的事测试类
test_block_queue: test/test_block_queue.cpp
	$(CXX) $(CXXFLAGS) -o test_block_queue $^

test_log: test/test_log.cpp log.o
	$(CXX) $(CXXFLAGS) -o test_log $^

-include $(DEPS)

clean:
	find . -name "*.o" -type f -delete
	find . -name "*.d" -type f -delete
	rm server
	rm test_block_queue

