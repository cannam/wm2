
LIBS	= -L/usr/X11R6/lib -lXext -lX11 -lXt -lXmu -lSM -lICE -lm
INCS	= -I/usr/X11R6/include

CC	= gcc
CCC	= g++
CFLAGS	= -O2 $(INCS)
OBJECTS	= Border.o Buttons.o Client.o Events.o Main.o Manager.o Rotated.o

.c.o:
	$(CC) -c $(CFLAGS) $<

.C.o:
	$(CCC) -c $(CFLAGS) $<

wm2:	$(OBJECTS)
	$(CCC) -o wm2 $(OBJECTS) $(LIBS)

depend:
	makedepend -- $(CFLAGS) -- *.C

clean:
	rm -f *.o core

distclean: clean
	rm -f wm2 wm2.old *~

