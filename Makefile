
LIBS	=  -L/usr/X11/lib -lXext -lX11 -lXmu -lm

CC	= gcc
CCC	= gcc
CFLAGS	= -O2
OBJECTS	= Border.o Buttons.o Client.o Events.o Main.o Manager.o Rotated.o

.c.o:
	$(CC) -c $(CFLAGS) $<

.C.o:
	$(CCC) -c $(CFLAGS) $<

wm2:	$(OBJECTS)
	mv -f wm2 wm2.old >& /dev/null || true
	$(CCC) -o wm2 $(OBJECTS) $(LIBS)

depend:
	makedepend -- $(CFLAGS) -- *.C

clean:
	rm -f *.o core

distclean: clean
	rm -f wm2 wm2.old *~

