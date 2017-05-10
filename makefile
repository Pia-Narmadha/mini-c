GRAMMAR = parser.y

CFLAGS = -I. -funsigned-char -g -DYYDEBUG 	
YFLAGS = -v -d

mcc: y.tab.o lex.yy.o symtab.o tree.o driver.o semchk.o codegen.o
	gcc $(CFLAGS) -o mcc y.tab.o lex.yy.o tree.o driver.o symtab.o semchk.o codegen.o -ll

y.tab.o: y.tab.c y.tab.h 
	gcc $(CFLAGS) -c y.tab.c 

y.tab.c: $(GRAMMAR)
	yacc $(YFLAGS) $(GRAMMAR)

lex.yy.o: lex.yy.c y.tab.h 
	gcc $ -c lex.yy.c

lex.yy.c: scanner.l
	lex scanner.l

tree.o: tree.c tree.h
	gcc $(CFLAGS) -c tree.c

symtab.o: symtab.c symtab.h
	gcc $(CFLAGS) -c symtab.c

codegen.o: codegen.c codegen.h 
	$gcc  $(CFLAGS) -c codegen.c codegen.h

driver.o: driver.c 
	$(CC)  $(CFLAGS) -c driver.c 

semchk.o: semchk.c 
	$(CC)  $(CFLAGS) -c semchk.c 

clean:
	rm -f y.tab.* y.output lex.yy.* *.o *~ mcc 