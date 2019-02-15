###################
#     Cimple      #
#  	 Makefile     #
###################

NAME = cimp
DIR = src
ODIR = build
SOURCES = $(DIR)/main.c
BUILDS = $(wildcard build/*.o)
INCLUDES = -I $(DIR) -I $(DIR)/controller -I $(DIR)/model -I $(DIR)/view


.PHONY: all $(NAME) clean

all :
	make -C $(DIR)/model
	make -C $(DIR)/view
	make -C $(DIR)/controller
	make $(NAME)

$(NAME) : $(BUILDS) $(ODIR)/main.o
	gcc -o $@ $^

$(ODIR)/main.o: $(DIR)/main.c
	gcc -c $(INCLUDES) -o $@ $<

clean :
	rm -rf build/*.o
	rm cimp
