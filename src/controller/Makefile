SOURCES = cmd_line.o
ODIR = ../../build
OBJS = $(ODIR)/cmd_line.o
INCLUDES = -I .

all : $(OBJS)

$(ODIR)/%.o : %.c
	gcc -c $(INCLUDES) -o $@ $< 

$(ODIR)/cmd_line.o : cmd_line.c




