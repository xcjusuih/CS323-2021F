CC=gcc
BIN=bin
SRC=main.c tac.c mips32.c

splc: clean
	@mkdir -p $(BIN)
	$(CC) -fcommon $(SRC) -o $(BIN)/$@
clean:
	@rm -rf $(BIN)
.PHONY: clean
testing: 
	make splc && bin/splc sample/test_4_fact.ir>sample.s
