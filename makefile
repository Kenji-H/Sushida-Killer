#makefile
app  = SushidaKiller
objs = main.o bmp.o win.o util.o
CC   = g++

$(app).exe: $(objs)
	$(CC) -o $(app) $(objs) -lgdi32

main.o: main.cpp
	$(CC) -c main.cpp

bmp.o: bmp.cpp
	$(CC) -c bmp.cpp

win.o: win.cpp
	$(CC) -c win.cpp

util.o: util.cpp
	$(CC) -c util.cpp

.PHONY: clean
clean: 
	del $(app).exe $(objs) *~
