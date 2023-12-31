# INC=-I headers
all:  clean master server drone keyboard window watchdog

clean:
	rm bin/*

master: 

    gcc src/master.c -lm -lrt  -o  bin/master
server: 
	gcc src/server.c -lm -lrt  -o  bin/server

drone: 
	gcc src/drone.c -lm -lrt  -o  bin/drone

keyboard: 
	gcc src/keyboardManager.c -lm -lrt  -o  bin/keyboardManager

window: 
	gcc src/window.c -lncurses -lm -lrt  -o  bin/window

watchdog: 
	gcc src/watch_dog.c -lm -lrt  -o  bin/watch_dog

run:
	./bin/master

