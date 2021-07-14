#######################################################################
#
# Copyright (C) 2018-2021 David C. Harrison. All right reserved.
#
# You may not use, distribute, publish, or modify this code without 
# the express written permission of the copyright holder.
#
#######################################################################

#######################################################################
#
# Do not modify this file. 
#
# If you modify it and your code relies on those modifications, your code
# will not compile in the automated test harness and will be unable to
# execute any tests.
#
# If you want to use additional standard libraries, modify Makefile.libs
#
#######################################################################

include Makefile.libs

CFLAGS = -Wall
CC = gcc

# Assignment No.
a = 1

rpnc: rpnc.c
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) 2>&1 | tee make.out

RpncC: rpnc 
	@./check.sh "RpncC: "    "./rpnc" 
RpncSH: 
	@./check.sh "RpncSH: "   "bash rpnc.sh"    
RpncPY: 
	@./check.sh "RpncPY: "   "python3 rpnc.py" 
RpncAWK: 
	@./check.sh "RpncAWK: "  "awk -f rpnc.awk" 
RpncPerl: 
	@./check.sh "RpncPerl: " "perl rpnc.pl"    
RpncJS: 
	@./check.sh "RpncJS: "   "nodejs rpnc.js"  

check: rpnc 
	@echo "" | tee check.out
	@date    | tee -a check.out
	@echo "" | tee -a check.out
	@./check.sh "RpncC: "    "./rpnc"          | tee -a check.out
	@./check.sh "RpncSH: "   "bash rpnc.sh"    | tee -a check.out
	@./check.sh "RpncPY: "   "python3 rpnc.py" | tee -a check.out
	@./check.sh "RpncAWK: "  "awk -f rpnc.awk" | tee -a check.out
	@./check.sh "RpncPerl: " "perl rpnc.pl"    | tee -a check.out
	@./check.sh "RpncJS: "   "nodejs rpnc.js"  | tee -a check.out

grade: check
	@./grade.sh 

clean:
	@rm -f rpnc check.out make.out

submit: clean
	@echo ""
	@echo "#### Only these file are submitted:"
	@echo ""
	@tar czvf ~/CSE111-Assignment$(a).tar.gz \
		Makefile.libs rpnc.awk rpnc.c rpnc.js rpnc.pl rpnc.py rpnc.sh
	@echo ""
	@echo "##### Confirming submission is good..."
	@./confirm.sh $(a)
	@echo "#### If that failed, fix the problem and try again."
	@echo ""
	@echo "Finally, don't forget to upload ~/CSE111-Assignment$(a).tar.gz to Canvas!"
	@echo ""

