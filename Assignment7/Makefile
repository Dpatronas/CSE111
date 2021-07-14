#######################################################################
#
# Copyright (C) 2018-2021 David C. Harrison. All right reserved.
#
# You may not use, distribute, publish, or modify this code without 
# the express written permission of the copyright holder.
#
#######################################################################

CFLAGS = -Wall -Iinclude -Llib -pthread

CC = g++ -g -std=c++17

# Assignment No.
a = 7

SRC=$(wildcard src/*.cc)

all: radix 

radix: $(SRC)
	$(CC) -g -o $@ $^ $(CFLAGS) -lradix $(LIBS) 2>&1 | tee make.out

check: radix
	@./check.sh 1000

grade: clean radix 
	@./grade.sh 10000

clean:
	@rm -f radix make.out

submit: clean
	@echo ""
	@echo "#### Only these file are submitted:"
	@echo ""
	@tar czvf ~/CSE111-Assignment$(a).tar.gz src/*
	@echo ""
	@echo "##### Confirming submission is good..."
	@echo ""
	@./confirm.sh $(a)
	@echo ""
	@echo "#### If that failed, fix the problem and try again."
	@echo ""
	@echo "Finally, don't forget to upload ~/CSE111-Assignment$(a).tar.gz to Canvas!"
	@echo ""
