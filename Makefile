
LIBS	= -lXext -lX11 -lXmu -lm
CC	= ncc
CCC	= NCC
CFLAGS	= -O2
OBJECTS	= Border.o Buttons.o Client.o Events.o Main.o Manager.o Rotated.o listimpl.o

.c.o:
	$(CC) -c $(CFLAGS) $<

.C.o:
	$(CCC) -c $(CFLAGS) $<

wm2:	$(OBJECTS)
	$(CCC) -o wm2 $(OBJECTS) $(LIBS)

depend:
	makedepend -- $(CFLAGS) -- *.C

clean:
	rm -f *.o wm2 core

# DO NOT DELETE

Border.o: Border.h General.h /usr/include/unistd.h /usr/include/sys/types.h
Border.o: /usr/include/sgidefs.h /usr/include/sys/bsd_types.h
Border.o: /usr/include/sys/select.h /usr/include/sys/time.h
Border.o: /usr/include/stdio.h /usr/include/signal.h
Border.o: /usr/include/sys/signal.h /usr/include/sys/siginfo.h
Border.o: /usr/include/errno.h /usr/include/sys/errno.h /usr/include/stdlib.h
Border.o: /usr/include/X11/X.h /usr/include/X11/Xlib.h
Border.o: /usr/include/X11/Xfuncproto.h /usr/include/X11/Xosdefs.h
Border.o: /usr/include/stddef.h /usr/include/X11/Xos.h /usr/include/string.h
Border.o: /usr/include/fcntl.h /usr/include/sys/fcntl.h
Border.o: /usr/include/X11/Xutil.h /usr/include/X11/Xatom.h
Border.o: /usr/include/X11/extensions/shape.h Config.h Rotated.h Client.h
Border.o: Manager.h listmacro.h
Buttons.o: Manager.h General.h /usr/include/unistd.h /usr/include/sys/types.h
Buttons.o: /usr/include/sgidefs.h /usr/include/sys/bsd_types.h
Buttons.o: /usr/include/sys/select.h /usr/include/sys/time.h
Buttons.o: /usr/include/stdio.h /usr/include/signal.h
Buttons.o: /usr/include/sys/signal.h /usr/include/sys/siginfo.h
Buttons.o: /usr/include/errno.h /usr/include/sys/errno.h
Buttons.o: /usr/include/stdlib.h /usr/include/X11/X.h /usr/include/X11/Xlib.h
Buttons.o: /usr/include/X11/Xfuncproto.h /usr/include/X11/Xosdefs.h
Buttons.o: /usr/include/stddef.h /usr/include/X11/Xos.h /usr/include/string.h
Buttons.o: /usr/include/fcntl.h /usr/include/sys/fcntl.h
Buttons.o: /usr/include/X11/Xutil.h /usr/include/X11/Xatom.h
Buttons.o: /usr/include/X11/extensions/shape.h Config.h listmacro.h Client.h
Buttons.o: Border.h Rotated.h
Client.o: Manager.h General.h /usr/include/unistd.h /usr/include/sys/types.h
Client.o: /usr/include/sgidefs.h /usr/include/sys/bsd_types.h
Client.o: /usr/include/sys/select.h /usr/include/sys/time.h
Client.o: /usr/include/stdio.h /usr/include/signal.h
Client.o: /usr/include/sys/signal.h /usr/include/sys/siginfo.h
Client.o: /usr/include/errno.h /usr/include/sys/errno.h /usr/include/stdlib.h
Client.o: /usr/include/X11/X.h /usr/include/X11/Xlib.h
Client.o: /usr/include/X11/Xfuncproto.h /usr/include/X11/Xosdefs.h
Client.o: /usr/include/stddef.h /usr/include/X11/Xos.h /usr/include/string.h
Client.o: /usr/include/fcntl.h /usr/include/sys/fcntl.h
Client.o: /usr/include/X11/Xutil.h /usr/include/X11/Xatom.h
Client.o: /usr/include/X11/extensions/shape.h Config.h listmacro.h Client.h
Client.o: Border.h Rotated.h
Events.o: Manager.h General.h /usr/include/unistd.h /usr/include/sys/types.h
Events.o: /usr/include/sgidefs.h /usr/include/sys/bsd_types.h
Events.o: /usr/include/sys/select.h /usr/include/sys/time.h
Events.o: /usr/include/stdio.h /usr/include/signal.h
Events.o: /usr/include/sys/signal.h /usr/include/sys/siginfo.h
Events.o: /usr/include/errno.h /usr/include/sys/errno.h /usr/include/stdlib.h
Events.o: /usr/include/X11/X.h /usr/include/X11/Xlib.h
Events.o: /usr/include/X11/Xfuncproto.h /usr/include/X11/Xosdefs.h
Events.o: /usr/include/stddef.h /usr/include/X11/Xos.h /usr/include/string.h
Events.o: /usr/include/fcntl.h /usr/include/sys/fcntl.h
Events.o: /usr/include/X11/Xutil.h /usr/include/X11/Xatom.h
Events.o: /usr/include/X11/extensions/shape.h Config.h listmacro.h Client.h
Events.o: Border.h Rotated.h
Main.o: Manager.h General.h /usr/include/unistd.h /usr/include/sys/types.h
Main.o: /usr/include/sgidefs.h /usr/include/sys/bsd_types.h
Main.o: /usr/include/sys/select.h /usr/include/sys/time.h
Main.o: /usr/include/stdio.h /usr/include/signal.h /usr/include/sys/signal.h
Main.o: /usr/include/sys/siginfo.h /usr/include/errno.h
Main.o: /usr/include/sys/errno.h /usr/include/stdlib.h /usr/include/X11/X.h
Main.o: /usr/include/X11/Xlib.h /usr/include/X11/Xfuncproto.h
Main.o: /usr/include/X11/Xosdefs.h /usr/include/stddef.h
Main.o: /usr/include/X11/Xos.h /usr/include/string.h /usr/include/fcntl.h
Main.o: /usr/include/sys/fcntl.h /usr/include/X11/Xutil.h
Main.o: /usr/include/X11/Xatom.h /usr/include/X11/extensions/shape.h Config.h
Main.o: listmacro.h
Manager.o: Manager.h General.h /usr/include/unistd.h /usr/include/sys/types.h
Manager.o: /usr/include/sgidefs.h /usr/include/sys/bsd_types.h
Manager.o: /usr/include/sys/select.h /usr/include/sys/time.h
Manager.o: /usr/include/stdio.h /usr/include/signal.h
Manager.o: /usr/include/sys/signal.h /usr/include/sys/siginfo.h
Manager.o: /usr/include/errno.h /usr/include/sys/errno.h
Manager.o: /usr/include/stdlib.h /usr/include/X11/X.h /usr/include/X11/Xlib.h
Manager.o: /usr/include/X11/Xfuncproto.h /usr/include/X11/Xosdefs.h
Manager.o: /usr/include/stddef.h /usr/include/X11/Xos.h /usr/include/string.h
Manager.o: /usr/include/fcntl.h /usr/include/sys/fcntl.h
Manager.o: /usr/include/X11/Xutil.h /usr/include/X11/Xatom.h
Manager.o: /usr/include/X11/extensions/shape.h Config.h listmacro.h Client.h
Manager.o: Border.h Rotated.h /usr/include/X11/Xproto.h
Manager.o: /usr/include/X11/Xmd.h /usr/include/X11/Xprotostr.h
Manager.o: /usr/include/sys/wait.h Cursors.h
Rotated.o: /usr/include/X11/Xlib.h /usr/include/sys/types.h
Rotated.o: /usr/include/sgidefs.h /usr/include/sys/bsd_types.h
Rotated.o: /usr/include/sys/select.h /usr/include/X11/X.h
Rotated.o: /usr/include/X11/Xfuncproto.h /usr/include/X11/Xosdefs.h
Rotated.o: /usr/include/stddef.h /usr/include/X11/Xutil.h
Rotated.o: /usr/include/stdlib.h /usr/include/string.h /usr/include/stdio.h
Rotated.o: Rotated.h
listimpl.o: /usr/include/stdio.h /usr/include/stdlib.h /usr/include/sgidefs.h
