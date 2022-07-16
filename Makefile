# Note: can change the version of cpp and also the optimisation levels below (O0 being no optimisation and O3 being the most)

SRCS := $(wildcard ./*.cpp ./src/*.cpp ./vendor/*.cpp ./src/EngineFiles/*.cpp ./src/CharacterFiles/*.cpp ./src/SceneFiles/*.cpp)

OBJS := $(SRCS:%.cpp=%.o)
# OBJS := $(subst src/,obj/,$(OBJS))

DEPENDS := $(patsubst %.cpp,%.d,$(SRCS))

CFLAGS = -Wall -Wno-missing-braces
# CFLAGS = -Wall -Wno-missing-braces


LDFLAGS = -static-libstdc++
LDLIBS = -Llib/ -lraylib -lopengl32 -lgdi32 -lwinmm -lbox2d

CPP_VERSION = -std=c++17
BUILD_MODE ?= RELEASE

ifeq ($(BUILD_MODE),DEBUG)
	OPTIMISE_FLAG = -g -O0
else
	OPTIMISE_FLAG = -s -O1
endif

all: output

output: $(OBJS)
	g++ $(LDFLAGS) $^ -o $@ $(LDLIBS)

-include $(DEPENDS)

%.o: %.cpp
	g++ $(CFLAGS) $(CPP_VERSION) $(OPTIMISE_FLAG) -MMD -MP -c $< -I./include/ -o $@


clean:
	rm $(OBJS) ./output $(DEPENDS)
