# Variables
PROJECT   = csrc
SRC_DIR   = ~/wrk/$(PROJECT)
BLD_DIR   = ~/wrk/$(PROJECT)
BIN_DIR   = ~/wrk/$(PROJECT)
EXEC      = cnv_f01 cnv_f02 preg1 preg2 preg3

# Comandos
CC      = gcc
#CFLAGS = -Wall -O2
CFLAGS  = 

# Tareas
.PHONY: all clean 

all: 		$(EXEC)

clean:
	@echo "Borrando files temporarios ... "
	@rm -f *.o *.exe *.log log
	@echo "Listo ..."


# para compilar .c a .o
$(BLD_DIR)/%.o:	$(SRC_DIR)/%.c
		$(CC) $(CFLAGS) -c -o $@ $<

# para link de objetos a exe
$(BLD_DIR)/%: 	$(BLD_DIR)/%.o
		$(CC) $(CFLAGS) -o $@ $^














