SOURCES = cmd_view.c window.c event.c
ODIR = ../../build
OBJS = $(ODIR)/cmd_view.o $(ODIR)/window.o $(ODIR)/event.o
INCLUDES = -I .

all : $(OBJS)
#	mv *.o ../../build

$(ODIR)/%.o : %.c
	gcc -c $(INCLUDES) -o $@ $< 

$(ODIR)/cmd_view.o : cmd_view.c
$(ODIR)/window.o : window.c
$(ODIR)/event.o : event.c




