
#include "Manager.h"
#include "Client.h"


// hack:
static unsigned long swallowNextEnterEvent = 0L;


int WindowManager::loop()
{
    XEvent ev;
    m_looping = True;

    while (m_looping) {

	nextEvent(&ev);
	m_currentTime = CurrentTime;

	switch (ev.type) {

	case ButtonPress:
	    eventButton(&ev.xbutton);
	    break;

	case ButtonRelease:
	    break;

	case MapRequest:
	    eventMapRequest(&ev.xmaprequest);
	    break;

	case ConfigureRequest:
	    eventConfigureRequest(&ev.xconfigurerequest);
	    break;

	case UnmapNotify:
	    eventUnmap(&ev.xunmap);
	    break;

	case CreateNotify:
	    eventCreate(&ev.xcreatewindow);
	    break;

	case DestroyNotify:
	    eventDestroy(&ev.xdestroywindow);
	    break;

	case ClientMessage:
	    eventClient(&ev.xclient);
	    break;

	case ColormapNotify:
	    eventColormap(&ev.xcolormap);
	    break;
	    
	case PropertyNotify:
	    eventProperty(&ev.xproperty);
	    break;
	    
	case SelectionClear:
	    fprintf(stderr, "wm2: SelectionClear (this should not happen)\n");
	    break;

	case SelectionNotify:
	    fprintf(stderr, "wm2: SelectionNotify (this should not happen)\n");
	    break;

	case SelectionRequest:
	    fprintf(stderr, "wm2: SelectionRequest (this should not happen)\n");
	    break;

	case EnterNotify:
	case LeaveNotify:
	    eventEnter(&ev.xcrossing);
	    break;

	case ReparentNotify:
	    eventReparent(&ev.xreparent);
	    break;

	case FocusIn:
	    eventFocusIn(&ev.xfocus);
	    break;

	case Expose:		// might be wm tab
	    eventExposure(&ev.xexpose);
	    break;

	case MotionNotify:
	case FocusOut:
	case ConfigureNotify:
	case MapNotify:
	case MappingNotify:

#ifdef DEBUG_EV
	    trace("ignore", 0, &ev);
#endif
	    break;

	default:
	    if (ev.type == m_shapeEvent) eventShapeNotify((XShapeEvent *)&ev);
	    else fprintf(stderr, "wm2: unsupported event type %d\n", ev.type);
	    break;
	}
    }
    
    release();
    return m_returnCode;
}


void WindowManager::nextEvent(XEvent *e)
{
    int fd;
    fd_set rfds;
    struct timeval t;

    if (!m_signalled) {

	if (QLength(m_display) > 0) {
	    XNextEvent(m_display, e);
	    return;
	}

	fd = ConnectionNumber(m_display);
	memset((void *)&rfds, 0, sizeof(fd_set)); // SGI's FD_ZERO is fucked
	FD_SET(fd, &rfds);
	t.tv_sec = t.tv_usec = 0;

	if (select(fd + 1, &rfds, NULL, NULL, &t) == 1) {
	    XNextEvent(m_display, e);
	    return;
	}

	XFlush(m_display);
	FD_SET(fd, &rfds);

	if (select(fd + 1, &rfds, NULL, NULL, NULL) == 1) {
	    XNextEvent(m_display, e);
	    return;
	}

	if (errno != EINTR || !m_signalled) {
	    perror("wm2: select failed");
	    m_looping = False;
	}
    }

    fprintf(stderr, "wm2: signal caught, exiting\n");
    m_looping = False;
    m_returnCode = 0;
}


void WindowManager::eventConfigureRequest(XConfigureRequestEvent *e)
{
    XWindowChanges wc;
    Client *c = windowToClient(e->window);

    e->value_mask &= ~CWSibling;
    if (c) c->eventConfigureRequest(e);
    else {

	wc.x = e->x;
	wc.y = e->y;
	wc.width  = e->width;
	wc.height = e->height;
	wc.border_width = 0;
	wc.sibling = None;
	wc.stack_mode = Above;
	e->value_mask &= ~CWStackMode;
	e->value_mask |= CWBorderWidth;

	XConfigureWindow(display(), e->window, e->value_mask, &wc);
    }
}


void Client::eventConfigureRequest(XConfigureRequestEvent *e)
{
    XWindowChanges wc;
    Boolean raise = False;

    e->value_mask &= ~CWSibling;

    gravitate(True);

    if (e->value_mask & CWX)      m_x = e->x;
    if (e->value_mask & CWY)      m_y = e->y;
    if (e->value_mask & CWWidth)  m_w = e->width;
    if (e->value_mask & CWHeight) m_h = e->height;
    if (e->value_mask & CWBorderWidth) m_bw = e->border_width;

    gravitate(False);

    if (e->value_mask & CWStackMode) {
	if (e->detail == Above) raise = True;
	e->value_mask &= ~CWStackMode;
    }

    if (parent() != root() && m_window == e->window) {
	m_border->configure(m_x, m_y, m_w, m_h, e->value_mask, e->detail);
	sendConfigureNotify();
    }

    if (m_initialising) {
	wc.x = m_border->xIndent();
	wc.y = m_border->yIndent();
    } else {
	wc.x = e->x;
	wc.y = e->y;
    }

    wc.width = e->width;
    wc.height = e->height;
    wc.border_width = 0;
    wc.sibling = None;
    wc.stack_mode = Above;
    e->value_mask &= ~CWStackMode;
    e->value_mask |= CWBorderWidth;

    XConfigureWindow(display(), e->window, e->value_mask, &wc);

    // if parent==root, it's not managed yet -- & it'll be raised when it is
    if (raise && parent() != root()) {
	mapRaised();
	if (CONFIG_CLICK_TO_FOCUS) activate();
    }
}


void WindowManager::eventMapRequest(XMapRequestEvent *e)
{
    Client *c = windowToClient(e->window);

    // some stuff for multi-screen fuckups here, omitted

    if (c) c->eventMapRequest(e);
    else {
	fprintf(stderr, "wm2: bad map request for window %lx\n", e->window);
    }
}


void Client::eventMapRequest(XMapRequestEvent *)
{
    switch(m_state) {

    case WithdrawnState:
	if (parent() == root()) {
	    manage(False);
	    return;
	}

	m_border->reparent();

	XAddToSaveSet(display(), m_window);
	// fall through

    case NormalState:
	XMapWindow(display(), m_window);
	mapRaised();
	setState(NormalState);

	if (CONFIG_CLICK_TO_FOCUS)/* ||
	    (m_transient != None && activeClient() &&
	     m_transient == activeClient()->m_window))*/ {
	    activate();
	}
	break;

    case IconicState:
	unhide(True);
	break;
    }
}


void WindowManager::eventUnmap(XUnmapEvent *e)
{
    Client *c = windowToClient(e->window);
    if (c) c->eventUnmap(e);
}


void Client::eventUnmap(XUnmapEvent *e)
{
    swallowNextEnterEvent = 0L;

    switch (m_state) {

    case IconicState:
	if (e->send_event) {
	    unhide(False);
	    withdraw();
	}
	break;

    case NormalState:
	if (isActive()) m_windowManager->clearFocus();
	if (!m_reparenting) withdraw();
	break;
    }

    m_reparenting = False;
}


void WindowManager::eventCreate(XCreateWindowEvent *e)
{
    if (e->override_redirect) return;
    Client *c = windowToClient(e->window, True);
    if (c) c->eventCreate(e);
}


void Client::eventCreate(XCreateWindowEvent *)
{
    if (!CONFIG_CLICK_TO_FOCUS) {
	Window r, ch;
	int x = -1, y = -1, wx, wy;
	unsigned int k;
	XQueryPointer(display(), root(), &r, &ch, &x, &y, &wx, &wy, &k);
	if (x > m_x && y > m_y && x < m_x + m_w && y < m_y + m_h) {
	    activate();
	}
    }
}


void WindowManager::eventDestroy(XDestroyWindowEvent *e)
{
    Client *c = windowToClient(e->window);

    if (c) {
	swallowNextEnterEvent = 0L;

	for (int i = m_clients.count()-1; i >= 0; --i) {
	    if (m_clients.item(i) == c) {
		m_clients.remove(i);
		break;
	    }
	}

	c->release();

	ignoreBadWindowErrors = True;
	XSync(display(), False);
	ignoreBadWindowErrors = False;
    }
}


void WindowManager::eventClient(XClientMessageEvent *e)
{
    Client *c = windowToClient(e->window);

    if (e->message_type == Atoms::wm_changeState) {
	if (c && e->format == 32 && e->data.l[0] == IconicState && c != 0) {
	    if (c->isNormal()) c->hide();
	    return;
	}
    }

    fprintf(stderr, "wm2: unexpected XClientMessageEvent, type 0x%lx, "
	    "window 0x%lx\n", e->message_type, e->window);
}


void WindowManager::eventColormap(XColormapEvent *e)
{
    Client *c = windowToClient(e->window);
    int i;

    if (e->c_new) {  // this field is called "new" in the old C++-unaware Xlib

	if (c) c->eventColormap(e);
	else {
	    for (i = 0; i < m_clients.count(); ++i) {
		m_clients.item(i)->eventColormap(e);
	    }
	}
    }
}


void Client::eventColormap(XColormapEvent *e)
{
    if (e->window == m_window || e->window == parent()) {

	m_colormap = e->colormap;
	if (isActive()) installColormap();

    } else {

	for (int i = 0; i < m_colormapWinCount; ++i) {
	    if (m_colormapWindows[i] == e->window) {
		m_windowColormaps[i] = e->colormap;
		if (isActive()) installColormap();
		return;
	    }
	}
    }
}


void WindowManager::eventProperty(XPropertyEvent *e)
{
    Client *c = windowToClient(e->window);
    if (c) c->eventProperty(e);
}


void Client::eventProperty(XPropertyEvent *e)
{
    Atom a = e->atom;
    Boolean shouldDelete = (e->state == PropertyDelete);

    switch (a) {

    case XA_WM_ICON_NAME:
	if (m_iconName) XFree((char *)m_iconName);
	m_iconName = shouldDelete ? 0 : getProperty(a);
	if (setLabel()) rename();
	return;

    case XA_WM_NAME:
	if (m_name) XFree((char *)m_name);
	m_name = shouldDelete ? 0 : getProperty(a);
	if (setLabel()) rename();
	return;

    case XA_WM_TRANSIENT_FOR:
	getTransient();
	return;
    }

    if (a == Atoms::wm_colormaps) {
	getColormaps();
	if (isActive()) installColormap();
    }
}


void WindowManager::eventReparent(XReparentEvent *e)
{
    if (e->override_redirect) return;
    (void)windowToClient(e->window, True); // create if absent

    // odd screen complications, omitted
}


void WindowManager::eventEnter(XCrossingEvent *e)
{
//    if (e->detail == NotifyVirtual || e->detail == NotifyNonlinearVirtual) {
//	return;
//    }

    m_currentTime = e->time;	// not CurrentTime

    Client *c = windowToClient(e->window);
    if (c) c->eventEnter(e);
}


void Client::eventEnter(XCrossingEvent *e)
{
    long s = swallowNextEnterEvent;

    if (s == 0L && e->type == LeaveNotify) {

	if (!CONFIG_CLICK_TO_FOCUS &&
	    e->window != m_window && e->window != parent() &&
	    // must be tab or button
	    ((e->x > 1 && e->x < m_border->xIndent() &&
	      e->y > 1 && e->y < m_border->xIndent()) ||
	     (e->x > m_border->xIndent() - m_border->yIndent() &&
	      e->x < m_border->xIndent() + m_w &&
	      e->y > 1 && e->y < m_border->yIndent()))) {

	    swallowNextEnterEvent = e->time; // so you can reach the button!
	}
	return;

    } else if (s != 0L) {

	swallowNextEnterEvent = 0L;

	if (e->time <= s || e->time - s < 500L) {
	    return;
	}
    }

    if (e->type == EnterNotify && !isActive() && !CONFIG_CLICK_TO_FOCUS) {
	activate();
    }
}


void WindowManager::eventFocusIn(XFocusInEvent *e)
{
    if (e->detail != NotifyNonlinearVirtual) return;
    Client *c = windowToClient(e->window);

    if (c) c->eventFocusIn(e);
}


void Client::eventFocusIn(XFocusInEvent *e)
{
    if (m_window == e->window && !isActive()) {
	activate();
    }
}


void WindowManager::eventExposure(XExposeEvent *e)
{
    if (e->count != 0) return;
    Client *c = windowToClient(e->window);
    if (c) c->eventExposure(e);
}


void Client::eventExposure(XExposeEvent *e)
{
    if (m_border->hasWindow(e->window)) {
	m_border->expose(e);
    }
}



// don't handle these (yet?)
void WindowManager::eventShapeNotify(XShapeEvent *) { }
