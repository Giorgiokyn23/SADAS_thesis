################################
#
#     Makefile SADAS application
#    
################################

# preferenze compilatore e flag di compilazione
CC=g++
# compilazione 
CFLAGS= -std=c++17 -Wall -pedantic -O3

# Clean all executable files (e.g. files with no dot at the ending
# 	excluding Makefile)
cleanall:
	ls | grep -v "\.\|Makefile" | xargs rm -f

# Make utility files
read_utilities: read_utilities.cpp read_utilities.hpp
	$(CC) $(CFLAGS) -o read_utilities read_utilities.cpp

prova_small: prova.cpp AUI_graph.h CTP_graph.h NEXI_graph.h SADAS_graph.h read_utilities.hpp
	$(CC) $(CFLAGS) -D SMALL -o prova prova.cpp

prova: prova.cpp AUI_graph.h CTP_graph.h NEXI_graph.h SADAS_graph.h read_utilities.hpp
	$(CC) $(CFLAGS) -o prova prova.cpp

prova_debug_small: prova.cpp AUI_graph.h CTP_graph.h NEXI_graph.h SADAS_graph.h read_utilities.hpp
	$(CC) -D SMALL -Og -g -fsanitize=address -o prova_debug prova.cpp
	
# Run tests
prova_exe:
	make -s cleanall
	make -s prova
	@echo "------------------->Running prova<---------------------"
	# Comment 2> /dev/null for printing log
	./prova 2> /dev/null
	    @echo "------------------>Done with prova<--------------------"

prova_small_exe:
	make -s cleanall
	make -s prova_small
	@echo "------------------->Running prova small<---------------------"
	# Comment 2> /dev/null for printing log
	./prova_small 2> /dev/null
	@echo "------------------>Done with prova small<--------------------"


