
#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "General.h"
#include "Manager.h"
#include "Border.h"


class Client {
public:
    Client(WindowManager *const, Window);
    void release();

    /* for call from WindowManager: */

    void activate();		/* active() */
    void deactivate();		/* setactive(0) */
    void gravitate(Boolean invert);
    void installColormap();
    void unreparent();
    void withdraw();
    void hide();
    void unhide(Boolean map);
    void rename();
    void kill();
    void mapRaised();		// without activating

    void move(XButtonEvent *);		// event for grab timestamp & coords
    void resize(XButtonEvent *, Boolean, Boolean);
    void moveOrResize(XButtonEvent *);
    void ensureVisible();	// make sure x, y are on-screen

    void manage(Boolean mapped);
    Boolean hasWindow(Window w) {
	return ((m_window == w) || m_border->hasWindow(w));
    }

    Client *revertTo() { return m_revert; }
    void setRevertTo(Client *c) { m_revert = c; }

    Boolean isHidden()    { return (m_state == IconicState);    }
    Boolean isWithdrawn() { return (m_state == WithdrawnState); }
    Boolean isNormal()    { return (m_state == NormalState);    }
    Boolean isTransient() { return (m_transient != None);       }
    Window  transientFor() { return m_transient; }
    Boolean isFixedSize()  { return m_fixedSize; }

    Boolean isInitialising() { return m_initialising; }
    Boolean isReparenting()  { return m_reparenting;  }

    const char *label()    { return m_label;    }
    const char *name()     { return m_name;     }
    const char *iconName() { return m_iconName; }

    void sendMessage(Atom, long);
    void sendConfigureNotify();

    /* for call from within: */

    void fatal(char *m)    { m_windowManager->fatal(m);              }
    Display *display()     { return m_windowManager->display();      }
    Window parent()        { return m_border->parent();              }
    Window root()          { return m_windowManager->root();         }
    Client *activeClient() { return m_windowManager->activeClient(); }
    Boolean isActive()     { return (activeClient() == this);        }

    WindowManager *windowManager() { return m_windowManager; }

    // for call from equivalent wm functions in Events.C:

    void eventButton(XButtonEvent *);
    void eventMapRequest(XMapRequestEvent *);
    void eventConfigureRequest(XConfigureRequestEvent *);
    void eventUnmap(XUnmapEvent *);
    void eventCreate(XCreateWindowEvent *);
    void eventColormap(XColormapEvent *);
    void eventProperty(XPropertyEvent *);
    void eventEnter(XCrossingEvent *);
    void eventFocusIn(XFocusInEvent *);
//    void eventShapeNotify(XShapeEvent *);
    void eventExposure(XExposeEvent *);

private:
    // gcc says: class Client only defines a private destructor and
    // has no friends.  I don't think that's very nice of gcc.  My
    // code always has plenty of friends.
    ~Client();

    Window m_window;
    Window m_transient;
    Border *m_border;

    Client *m_revert;

    int m_x;
    int m_y;
    int m_w;
    int m_h;
    int m_bw;

    XSizeHints m_sizeHints;
    Boolean m_fixedSize;
    int m_minWidth;
    int m_minHeight;
    void fixResizeDimensions(int &, int &, int &, int &);

    int m_state;
    int m_protocol;
    Boolean m_initialising;
    Boolean m_reparenting;

    char *m_name;
    char *m_iconName;
    const char *m_label;	// alias: one of (instance,class,name,iconName)
    static const char *const m_defaultLabel;

    Colormap m_colormap;
    int m_colormapWinCount;
    Window *m_colormapWindows;
    Colormap *m_windowColormaps;

    WindowManager *const m_windowManager;

    char *getProperty(Atom);
    int getAtomProperty(Atom, Atom);
    int getIntegerProperty(Atom);

    // accessors 
    Boolean getState(int *);
    void setState(int);

    // internal instantiation requests
    Boolean setLabel(void);	// returns True if changed
    void getColormaps(void);
    void getProtocols(void);
    void getTransient(void);

    void decorate(Boolean active);
};

#define Pdelete    1
#define PtakeFocus 2

#endif

