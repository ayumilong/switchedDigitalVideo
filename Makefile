#*************************************************************************
#    > File Name: Makefile 
#    > Subject: CPSC851 Homework#3
#    > Author: ayumi_Long
#    > Mail: ayumilong41@gmail.com 
#    > Created Time: Sun 17 Nov 2013 12:09:09 PM EST
#*************************************************************************

CFLAGS = -g -W -Wall -Weffc++ -Wextra -pedantic -O0
VFLAGS = --leak-check=full --show-reachable=yes

SOURCES = $(shell find *.cpp) #All the cpp files of the current directory
#OBJS = $(SOURCES: .cpp = .o) #All the object files that should be generated, they have the same name with the corresponding cpp files, this one should work, I don't know the reason
OBJS = $(addsuffix .o, $(basename $(shell find *.cpp)))
EXEC = SDVSystem 


.PHONY: clang gcc setclang setgcc clean r vg

gcc: | setgcc $(EXEC)
clang: | setclang $(EXEC)

all : $(SOURCES) $(EXEC)
#Here must use $^, it represents all the OBJS but $< just represent the first one of OBJS
$(EXEC): $(OBJS)
	$(CXX) $(CFLAGS)  -o $@ $^ $(LDFLAGS)

#Here we can use ethier the $^ or $<, because for "%.**" the two behave the same
#$(OBJS): %.o : %.cpp  #This two sentences behave the same
.cpp.o:
	$(CXX) $(CFLAGS)  -c $^ -o $@ 

setclang:
	@echo "Setting clang"
	$(eval CXX = clang++)
	$(eval CXX_LINK = clang++)

setgcc:
	@echo "Setting gcc"
	$(eval CXX = g++)
	$(eval CXX_LINK = g++)

clean:
	rm -f $(EXEC) $(OBJS)

r:
	@./$(EXEC) 257 150 150 95 0.9 10000 4
	#@./$(EXEC) 10 6 6 5 0.9 500 4

vg:
	@valgrind $(VFLAGS) ./$(EXEC) 300 100 100 50 0.9 10000 3 
