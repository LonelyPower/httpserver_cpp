CXX = g++
CXXFLAGS = -Wall -O2

TARGETS = server client

all: $(TARGETS)

server: server.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

client: client.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

# 编译并运行 server
run: server
	./server

# 清理
clean:
	rm -f $(TARGETS) *.o
