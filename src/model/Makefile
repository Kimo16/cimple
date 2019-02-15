SOURCES = in.c out.c parse.c m_color.c m_transform.c m_frame.c
ODIR = ../../build
OBJS = $(ODIR)/in.o $(ODIR)/out.o $(ODIR)/parse.o $(ODIR)/m_color.o $(ODIR)/m_transform.o $(ODIR)/m_frame.o
INCLUDES = -I .

all : $(OBJS)

$(ODIR)/%.o : %.c
	gcc -c $(INCLUDES) -o $@ $< 

$(ODIR)/in.o : in.c
$(ODIR)/out.o : out.c
$(ODIR)/parse.o : parse.c
$(ODIR)/m_color.o : m_color.c
$(ODIR)/m_transform.o : m_transform.c
$(ODIR)/m_frame.o : m_frame.c



