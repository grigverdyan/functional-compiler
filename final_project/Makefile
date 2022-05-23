.PHONY: all re clean fclean debug gcov lcov clean_gcda clean_gcno fclean_glcov

FOLDER		=	sources
SOURCES		=	$(wildcard $(FOLDER)/*.cpp)
OBJECTS		=	$(SOURCES:.cpp=.o)
CC			=	g++
CXXFLAGS	=	# -Wall -Wextra -Werror
CPPFLAGS	=
STANDARD	=	-std=c++17
EXECUTABLE	=	func_compiler.exe
RM			=	rm -f
LCOVTEST	=	$(EXECUTABLE:.exe=.info)
BROWSER		=	google-chrome

all : $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

%.o : %.cpp
	$(CC) $(STANDARD) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

re : fclean all

clean :
	$(RM) $(OBJECTS)

fclean : clean
	$(RM) $(EXECUTABLE)

debug :
	gdb $(EXECUTABLE)

gcov :
	$(CC) $(SOURCES) --coverage -o $(EXECUTABLE)
	./$(EXECUTABLE) input.txt output.txt

lcov :
	lcov -t "program" -o $(LCOVTEST) -c -d .
	genhtml -o report $(LCOVTEST)
	$(RM) *.gcda
	$(RM) *.gcno
	$(RM) $(LCOVTEST)
	$(RM) $(EXECUTABLE)
	$(BROWSER) report/index.html

clean_gcda :
	$(RM) *.gcda

clean_gcno :
	$(RM) *.gcno

fclean_glcov : clean_gcda clean_gcno
	$(RM) $(LCOVTEST)
	$(RM) -fr report
	$(RM) $(EXECUTABLE)
