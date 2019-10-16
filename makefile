DIR_OUTPUT = ./bin
DIR_SRC = ./


all: release

# Compilación principal del ejecutable
release:
	@echo 'Compilando target: $@'
	mkdir -p $(DIR_OUTPUT)
	gcc mandelbrot.c -o $(DIR_OUTPUT)/mandelbrot -lm	
	gcc mandelbrotp.c -o $(DIR_OUTPUT)/mandelbrotp -fopenmp -lm
	@echo $@ compilado en el directorio $(DIR_OUTPUT)

# Other Targets
clean:
	rm -rf $(DIR_OUTPUT)/*
	-@echo ' '