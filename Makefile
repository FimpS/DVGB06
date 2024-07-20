PARMS = -lSDL2 -lSDL2_image -lm -g
noPARMS = -lm -g
O = main.o objects.o dynList.o map.o state.o event.o object_state.o gfx.o global.o runes.o info.o font.o


game: $(O)
	gcc -o game *.o -lSDL2 -lSDL2_image -g -lm

main.o: main.c
	gcc $(PARMS) -c main.c

global.o: global.c
	gcc $(PARMS) -c global.c

objects.o: objects.c objects.h
	gcc $(PARMS) -c objects.c

dynList.o: dynList.c dynList.h
	gcc $(noPARMS) -c dynList.c

map.o: map.c map.h
	gcc $(PARMS) -c map.c

state.o: state.c state.h
	gcc $(noPARMS) -c state.c

event.o: event.c event.h
	gcc $(noPARMS) -c event.c

object_state.o: object_state.c object_state.h
	gcc $(noPARMS) -c object_state.c

runes.o: runes.c runes.h
	gcc $(PARMS) -c runes.c

gfx.o: gfx.c gfx.h
	gcc $(PARMS) -c gfx.c

info.o: info.c info.h
	gcc $(PARMS) -c info.c

font.o: font.c font.h
	gcc $(noPARMS) -c font.c

clean:
	rm *.o game 

.PHONY: clean


