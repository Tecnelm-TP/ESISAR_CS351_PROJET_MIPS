LANGAGE = C
EXEC = Mips
DEBUG = false
CFLAGS = -Wall -ansi -pedantic -std=c99
STATICDEFINE = 
lib = 

OUT = out
OBJDIR = obj
SRCDIR = src
INCDIR = includes
LIBDIR = lib
DATADIR = data
MAIN = main

D ?= 
FLAGS ?=

ifeq ($(LANGAGE),C)
	CC = gcc
	EXTP =c 
	EXTS =h
endif

ifeq ($(LANGAGE),C++)
	CC = g++
	EXTP =cpp 
	EXTS =h
endif

SRCS=$(wildcard $(SRCDIR)/*.$(EXTP))
OBJS= $(SRCS:$(SRCDIR)/%.c=$(OUT)/$(OBJDIR)/%.o)
HEADER =$(wildcard $(INCDIR)/*.$(EXTS))


CFLAGS += $(FLAGS)
CFLAGS += $(foreach headerdir,$(INCDIR),-I$(headerdir)) 
CFLAGS += $(foreach librarydir,$(LIBDIR),-L$(librarydir))
CFLAGS += $(foreach library,$(lib),-l$(library))
CFLAGS += $(foreach defined,$(D),-D$(defined))
CFLAGS += $(foreach defined,$(STATICDEFINE),-D$(defined))

ifeq ($(DEBUG),yes)
	CFLAGS := $(CFLAGS) -g
endif


$(OUT)/$(EXEC):$(OBJS) $(HEADER)
	@mkdir -p $(OUT)
	@mkdir -p $(OUT)/$(OBJDIR)

	$(CC) $(CFLAGS) $(OBJS) -o $@


$(OUT)/$(OBJDIR)/%.o: $(SRCDIR)/%.$(EXTP) $(HEADER)
	@mkdir -p $(OUT)
	@mkdir -p $(OUT)/$(OBJDIR)
	$(CC) -o $@ -c $< $(CFLAGS)




.PHONY: clean mrproper run create debug

clean:
	@rm -rf $(OUT)/$(OBJDIR)/*.o

mrproper: clean
	@rm -rf $(OUT)

run: $(OUT)/$(EXEC)
	@./$(OUT)/$(EXEC)
create:
	mkdir -p $(DATADIR)
	mkdir -p $(SRCDIR)
	mkdir -p $(INCDIR)
	mkdir -p $(LIBDIR)

debug:
	@make FLAGS="-g"

