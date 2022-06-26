.PHONY: all re clean fclean 
	
SOURCES_1		=   sources/assembly_interpreter.cpp sources/helpers.cpp sources/interpreter.cpp
SOURCES_2   	=   sources/assembly_interpreter.cpp sources/byte_compiler.cpp sources/helpers.cpp sources/compiler.cpp

OBJECTS_1		=   sources/assembly_interpreter.o sources/helpers.o sources/interpreter.o
OBJECTS_2   	=   sources/assembly_interpreter.o sources/byte_compiler.o sources/helpers.o sources/compiler.o

CC				=	g++
INCS			=	-Iincludes
FLAGS			=	-Wall -Wextra -Werror $(INCS)
STANDARD		=	-std=c++17
EXECUTABLE_1 	=	interpreter
EXECUTABLE_2 	= 	compiler
RM				=	rm -rf

all : $(EXECUTABLE_1) $(EXECUTABLE_2)

$(EXECUTABLE_1) : $(OBJECTS_1)
	$(CC) $(OBJECTS_1) -o $@

$(EXECUTABLE_2) : $(OBJECTS_2)
	$(CC) $(OBJECTS_2) -o $@

%.o : %.cpp
	$(CC) $(STANDARD) $(FLAGS) -c -g $< -o $@

re : fclean all

clean :
	$(RM) $(OBJECTS_1) $(OBJECTS_2)

fclean : clean
	$(RM) $(EXECUTABLE_1) $(EXECUTABLE_2)
