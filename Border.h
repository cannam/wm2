
#ifndef _BORDER_H_
#define _BORDER_H_

#include "General.h"
#include "Rotated.h"

class Client;
class WindowManager;

class Border {			// friend of client
public:
    Border(Client *const, Window child);
    ~Border();


    void map();
    void unmap();
    void mapRaised();
    void decorate(Boolean active, int w, int h);
    void reparent();
    void configure(int x, int y, int w, int h, unsigned long mask, int detail,
		   Boolean force = False);
    void moveTo(int x, int y);

    WindowManager *windowManager(); // calls into Client
    Boolean isTransient();	// calls into Client
    Boolean isFixedSize();	// calls into Client
    Window parent() { return m_parent; }
    Boolean hasWindow(Window w) {
	return (w != root() && (w == m_parent || w == m_tab ||
				w == m_button || w == m_resize));
    }

    Display *display();
    Window root();

    void expose(XExposeEvent *);
    void eventButton(XButtonEvent *); // in Buttons.C

    int yIndent() {
	return isTransient() ? m_transientFrameWidth + 1 : m_frameWidth + 1;
    }
    int xIndent() {
	return isTransient() ? m_transientFrameWidth + 1 :
	    m_tabWidth + m_frameWidth + 1;
    }
    
private:
    Client *m_client;

    Window m_parent;
    Window m_tab;
    Window m_child;
    Window m_button;
    Window m_resize;

    void fatal(char *);

    char *m_label;

    void fixTabHeight(int);
    void drawLabel();

    void setFrameVisibility(Boolean, int, int);
    void setTransientFrameVisibility(Boolean, int, int);
    void shapeParent(int, int);
    void shapeTransientParent(int, int);
    void shapeTab(int, int);
    void resizeTab(int);	// for rename without changing window size
    void shapeResize();

    int m_prevW;
    int m_prevH;

private:
    int        m_tabHeight;	// depends on the label
    static int m_tabWidth;	// depends on the label font
    static int m_tabTopHeight;
    static int m_frameWidth;	// NB frameTopHeight = frameHeight-tabTopHeight
    static int m_transientFrameWidth;
    static XRotFontStruct *m_tabFont;
    static GC m_drawGC;
    static unsigned long m_foregroundPixel;
    static unsigned long m_backgroundPixel;
    static unsigned long m_frameBackgroundPixel;
    static unsigned long m_buttonBackgroundPixel;
    static unsigned long m_borderPixel;
};


#endif

