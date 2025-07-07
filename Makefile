TARGET:=snake
snake: CFLAGS=$(shell sdl2-config --cflags --libs)
all: $(TARGET)
clean:
	$(RM) $(TARGET)
