EXECUTABLE := ape_unflod  							    # 可执行文件名
LIBDIR:=          									# 静态库目录
LIBS :=           							        # 静态库文件名
INCLUDES:=./include ./include/ape   ./ext     			# 头文件目录
SRCDIR:=./app  ./src/ ./src/ape ./src/data  ./ext         	# 除了当前目录外，其他的源代码文件目录
#
# # Now alter any implicit rules' variables if you like, e.g.:
TOOLCHAIN_PREFIX:=~/vp/buildroot/dl/gcc-linaro-7.2.1-2017.11-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-
CC:=$(TOOLCHAIN_PREFIX)gcc
CFLAGS := -g -Wall -O3
CPPFLAGS := $(CFLAGS)
CPPFLAGS += $(addprefix -I,$(INCLUDES))
CPPFLAGS += -MMD


RM-F := rm -f
 
 
# # You shouldn't need to change anything below this point.
#
SRCS := $(wildcard *.c) $(wildcard $(addsuffix /*.c, $(SRCDIR)))
OBJS := $(patsubst %.c,%.o,$(SRCS))
DEPS := $(patsubst %.o,%.d,$(OBJS))
MISSING_DEPS := $(filter-out $(wildcard $(DEPS)),$(DEPS))
MISSING_DEPS_SOURCES := $(wildcard $(patsubst %.d,%.c,$(MISSING_DEPS)))
 
 
.PHONY : all deps objs clean veryclean rb info
 
all: $(EXECUTABLE)
 
deps : $(DEPS)
 
objs : $(OBJS)
 
clean :
	@$(RM-F) $(OBJS)
	@$(RM-F) $(DEPS)
veryclean: clean
	@$(RM-F) $(EXECUTABLE)
 
rb: veryclean all
ifneq ($(MISSING_DEPS),)
$(MISSING_DEPS) :
	@$(RM-F) $(patsubst %.d,%.o,$@)
endif
-include $(DEPS)
$(EXECUTABLE) : $(OBJS)
	$(CC) -o $(EXECUTABLE) $(OBJS) $(addprefix -L,$(LIBDIR)) $(addprefix -l,$(LIBS))  -L . -ljpeg 
	@echo "\n\nBuild sucess!!!!\n\n"
 
info:
	@echo $(SRCS)
	@echo $(OBJS)
	@echo $(DEPS)
	@echo $(MISSING_DEPS)
	@echo $(MISSING_DEPS_SOURCES)
