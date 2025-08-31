CXX = g++
CXXFLAGS = -Wall -O2 -MMD -MP   # 自动依赖

TARGETS = server client
OBJS_SERVER = server.o Mysocket.o
OBJS_CLIENT = client.o Mysocket.o   # client 也需要 Mysocket.o

all: $(TARGETS)

server: $(OBJS_SERVER)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS_SERVER)

client: $(OBJS_CLIENT)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS_CLIENT)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 自动包含依赖
-include $(OBJS_SERVER:.o=.d) $(OBJS_CLIENT:.o=.d)

run: server
	./server

clean:
	rm -f $(TARGETS) *.o *.d
