CXX = g++
EXEC_file = run.out

#=================================
#Compilation options, maybe to be added later
DEBUG = YES

#=================================


#=================================
#default rayulib flags
CXXFLAGS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

#=================================
#Directories and flags

ifeq ($(DEBUG),YES)
	EXEC_DIR = ./../bin/debug
	CXXFLAGS += -g -Wall
else
	EXEC_DIR = ./../bin/release
	CXXFLAGS += -O3 -Wall
endif


SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, %.o, $(SRC))

#=================================
#Rules
all: $(EXEC_DIR)/$(EXEC_file)

$(EXEC_DIR)/$(EXEC_file): $(OBJ)
	@mkdir -p $(EXEC_DIR)
	$(CXX) $(OBJ) $(CXXFLAGS) -o $(EXEC_DIR)/$(EXEC_file)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC_DIR)/$(EXEC_file)

.PHONY: all clean




