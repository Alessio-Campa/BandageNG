//This class was written by Jason Dolan and acquired here:
//http://qt-project.org/forums/viewthread/13728


#include "verticalscrollarea.h"

#include <QEvent>
#include <QScreen>
#include <QScrollBar>


VerticalScrollArea::VerticalScrollArea(QWidget *parent) :
    QScrollArea(parent)
{
    setWidgetResizable(true);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
}

bool VerticalScrollArea::eventFilter(QObject *o, QEvent *e)
{
    // This works because QScrollArea::setWidget installs an eventFilter on the widget
    if(o && o == widget() && e->type() == QEvent::Resize) {
        int minimumWidth = widget()->minimumSizeHint().width() + verticalScrollBar()->width();

        //Never let the content force the whole window wider than the screen.
        //Otherwise a single long piece of text can make the window grow past
        //the display bounds (e.g. when leaving full screen mode on macOS).
        if (QScreen *currentScreen = screen())
            minimumWidth = qMin(minimumWidth, currentScreen->availableGeometry().width() / 2);

        setMinimumWidth(minimumWidth);
    }

    return QScrollArea::eventFilter(o, e);
}
