
GCC       = g++
FLAGS     = -Wall -std=c++17 -O3
LIBS      = 
MAIN_RULE = ScrabbleSolver

OBJ       = obj/
SRC       = src/
BIN       = bin/
O_X       = .o
C_X       = .cpp
H_X       = .hpp

all : makedir $(MAIN_RULE)

# take *.cpp in $(SRC), add parent folder $(OBJ) and replace .cpp to .o
OBJ_FILES = $(patsubst %$(C_X), $(OBJ)%$(O_X), $(notdir $(wildcard $(SRC)*$(C_X))))

# main rule, ld
$(MAIN_RULE) : $(OBJ_FILES) 
	$(GCC) $(FLAGS) $(LIBS) $^ -o $(BIN)$@

# for all the .cpp with a related .hpp
$(OBJ)%$(O_X) : $(SRC)%$(C_X) $(SRC)%.$(H_X)
	$(GCC) $(FLAGS) -c $< -o $@

# for all the .cpp without related .hpp
$(OBJ)%$(O_X) : $(SRC)%$(C_X)
	$(GCC) $(FLAGS) -c $< -o $@

.PHONY : doc

doc :
	doxygen doc/doxyfile.doxy

makedir :
	@mkdir -p $(OBJ) $(BIN)

clean :
	rm -rvf $(OBJ)*$(O_X)

superclean : clean
	rm -rvf $(BIN) $(OBJ) doc/html doc/latex
