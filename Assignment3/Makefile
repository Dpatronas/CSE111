#######################################################################
#
# Copyright (C) 2018-202 David C. Harrison. All right reserved.
#
# You may not use, distribute, publish, or modify this code without 
# the express written permission of the copyright holder.
#
#######################################################################

################### DO NOT MODIFY THIS FILE ###########################
#
# It is not included in the subission archive ceated by 'make submit'.
#
# If you modify it and your code relies on those modifications, your code
# will not compile in the automated test harness and will be unable to
# execute any tests.
#
#######################################################################

CFLAGS = -Wall -Iinclude 
TFLAGS = --coverage -lCppUTest -lCppUTestExt
CC = g++ -std=c++17

# Assignment No.
a = 3

SRC=$(wildcard src/*.cc)
TRC=$(wildcard tests/*.cc)
CRC=$(wildcard comp/*.cc)

all: bounds
	@$(MAKE) -s tidy

tidy:
	@rm -f *.gcda 2>/dev/null
	@rm -f src/*.gcda src/*.gcno coverage.info 2>/dev/null

bounds: $(SRC) $(TRC) 
	$(CC) -o $@ $^ $(CFLAGS) $(TFLAGS) $(LIBS) 2>&1 | tee make.out
	@mv *.gcno src
	@./bounds -v 
	@mv *.gcda src
	#@lcov -q --capture --directory src --directory include --directory tests --output-file coverage.info --no-external
	@lcov -q --capture --directory src --directory include --output-file coverage.info --no-external
	@genhtml coverage.info --output-directory coverage
	@$(MAKE) -s tidy

compliance: $(SRC) $(CRC)
	@echo ""
	@echo "#### Checking compliance:"
	@echo ""
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) 

clean:
	@$(MAKE) -s tidy
	@rm -f compliance bounds make.out

test: clean bounds

grade: clean 
	@./grade.sh $a
	@$(MAKE) -s tidy

submit: compliance clean
	@echo ""
	@echo "#### Only these file are submitted:"
	@echo ""
	@tar czvf ~/CSE111-Assignment$(a).tar.gz \
		src include/*.h tests/* \
		--exclude=include/Containable.h \
		--exclude=include/Point.h 
	@echo ""
	@echo "##### Confirming submission is good..."
	@./confirm.sh $(a)
	@echo ""
	@echo "#### If that failed, fix the problem and try again."
	@echo ""
	@echo "Finally, don't forget to upload ~/CSE111-Assignment$(a).tar.gz to Canvas!"
	@echo ""

