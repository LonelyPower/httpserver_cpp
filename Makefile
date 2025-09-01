CXX = g++
CXXFLAGS = -Wall -O2 -fPIC -MMD -MP

TARGETS = server client
OBJDIR = build
LIBDIR = $(OBJDIR)
LIBNAME = libmynet.so

SRCS_COMMON = MySocket.cpp MyEpoll.cpp MyChannel.cpp
SRCS_SERVER = server.cpp
SRCS_CLIENT = client.cpp

OBJS_COMMON = $(SRCS_COMMON:%.cpp=$(OBJDIR)/%.o)
OBJS_SERVER = $(SRCS_SERVER:%.cpp=$(OBJDIR)/%.o)
OBJS_CLIENT = $(SRCS_CLIENT:%.cpp=$(OBJDIR)/%.o)

all: $(TARGETS) $(LIBDIR)/$(LIBNAME)

# server 依赖 so
server: $(OBJS_SERVER) $(LIBDIR)/$(LIBNAME)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS_SERVER) -L$(LIBDIR) -lmynet -Wl,-rpath=$(LIBDIR)

# client 依赖 so
client: $(OBJS_CLIENT) $(LIBDIR)/$(LIBNAME)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS_CLIENT) -L$(LIBDIR) -lmynet -Wl,-rpath=$(LIBDIR)

# 生成共享库 .so
$(LIBDIR)/$(LIBNAME): $(OBJS_COMMON) | $(OBJDIR)
	$(CXX) -shared -o $@ $(OBJS_COMMON)

# 通用规则：生成 build/*.o
$(OBJDIR)/%.o: %.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 确保 build/ 目录存在
$(OBJDIR):
	mkdir -p $(OBJDIR)

# 自动依赖
-include $(OBJS_COMMON:.o=.d) $(OBJS_SERVER:.o=.d) $(OBJS_CLIENT:.o=.d)

run: server
	./server

clean:
	rm -rf $(TARGETS) $(OBJDIR)
