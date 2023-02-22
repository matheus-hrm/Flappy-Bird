TARGET=program.exe
CC=gcc
ALLEGRO=C:\src\allegro\include
STATIC=liballegro_monolith.dll.a
LD=gcc
OBJS=main.o
all: $(OBJS)
	$(LD) -I $(ALLEGRO) -o $(TARGET) $(OBJS)  $(STATIC)
	$(TARGET)
main.o: main.c
	$(CC) -I $(ALLEGRO) -c main.c -o main.o