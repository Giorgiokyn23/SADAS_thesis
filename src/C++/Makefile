################################
#
#     Makefile SADAS application
#    
################################

# preferenze compilatore e flag di compilazione
CC=g++
# compilazione 
CFLAGS=-std=c++17 -Wall -pedantic -O3

# Clean all executable files (e.g. files with no dot at the ending
# 	excluding Makefile)
cleanall:
	ls | grep -v "\.\|Makefile" | xargs rm -f

# Make utility files
read_utilities: read_utilities.cpp read_utilities.hpp
	$(CC) $(CFLAGS) -o read_utilities read_utilities.cpp

prova_small: prova.cpp utils.h AUI_graph.h CTP_graph.h NEXI_graph.h SADAS_graph.h read_utilities.hpp
	$(CC) $(CFLAGS) -D SMALL -o prova prova.cpp

prova: prova.cpp utils.h AUI_graph.h CTP_graph.h NEXI_graph.h SADAS_graph.h read_utilities.hpp
	$(CC) $(CFLAGS) -o prova prova.cpp

prova_debug_small: prova.cpp utils.h AUI_graph.h CTP_graph.h NEXI_graph.h SADAS_graph.h read_utilities.hpp
	$(CC) -D SMALL -Og -g -fsanitize=address -o prova_debug prova.cpp

test_blue: test_blue.cpp utils.h AUI_graph.h SADAS_graph.h read_utilities.hpp
	$(CC) $(CFLAGS) -D BLUE -o test_blue test_blue.cpp

test_orange: test_orange.cpp utils.h AUI_graph.h SADAS_graph.h read_utilities.hpp
	$(CC) $(CFLAGS) -D ORANGE -o test_orange test_orange.cpp
# Run tests
run_prova:
	make -s cleanall
	make -s prova
	@echo "------------------->Running prova<---------------------"
	# Comment 2> /dev/null for printing log
	./prova 2> /dev/null
	@echo "------------------>Done with prova<--------------------"

run_prova_small:
	make -s cleanall
	make -s prova_small
	@echo "------------------->Running prova small<---------------------"
	# Comment 2> /dev/null for printing log
	./prova_small 2> /dev/null
	@echo "------------------>Done with prova small<--------------------"

run_blue:
	make -s cleanall
	make -s test_blue
	@echo "------------------->Running test of blue function<---------------------"
	# Comment 2> /dev/null for printing log
	./test_blue 2> /dev/null
	@echo "----------------------->Done with testing blue<------------------------"

run_orange:
	make -s cleanall
	make -s test_orange
	@echo "------------------->Running test of orange function<---------------------"
	# Comment 2> /dev/null for printing log
	./test_orange 2> /dev/null
	@echo "----------------------->Done with testing orange<------------------------"

