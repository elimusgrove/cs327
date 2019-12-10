all: info change rankmoves edit

info: info.o processfile.o
	gcc -Wall -o info info.o processfile.o

change: change.o processfile.o updategameboard.o
	gcc -Wall -o change change.o processfile.o updategameboard.o

rankmoves: rankmoves.o processfile.o parsemoves.o
	gcc -Wall -o rankmoves rankmoves.o processfile.o parsemoves.o

edit: edit.o processfile.o updategameboard.o editFunc.o
	gcc -Wall -o edit edit.o processfile.o updategameboard.o editFunc.o -ltermbox

info.o: info.c processfile.h
	gcc -Wall -c info.c

change.o: change.c processfile.h
	gcc -Wall -c change.c

rankmoves.o: rankmoves.c processfile.c parsemoves.c
	gcc -Wall -c rankmoves.c

edit.o: edit.c termbox/src/termbox.h processfile.h editFunc.h updategameboard.h
	gcc -Wall -c edit.c -ltermbox

processfile.o: processfile.c processfile.h
	gcc -Wall -c processfile.c

updategameboard.o: updategameboard.c updategameboard.h
	gcc -Wall -c updategameboard.c

parsemoves.o: parsemoves.c parsemoves.h
	gcc -Wall -c parsemoves.c

editFunc.o: editFunc.c editFunc.h termbox/src/termbox.h
	gcc -Wall -c editFunc.c -ltermbox

clean:
	rm info.o change.o rankmoves.o processfile.o updategameboard.o parsemoves.o editFunc.o edit.o info change rankmoves edit