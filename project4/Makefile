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
	make splc
	@for i in test/*.ir; do bin/splc $$i > $$i.s; done
