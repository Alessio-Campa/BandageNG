#include "resizesplitter.h"

#include <QEnterEvent>
#include <QGuiApplication>
#include <QHideEvent>
#include <QPainter>


ResizeSplitterHandle::ResizeSplitterHandle(Qt::Orientation orientation, QSplitter *parent) :
    QSplitterHandle(orientation, parent)
{
    setAttribute(Qt::WA_Hover, true);
    setCursor(orientation == Qt::Horizontal ? Qt::SplitHCursor : Qt::SplitVCursor);

    //Never leave an override cursor behind if the application loses focus
    //while the mouse is over the handle.
    if (qApp) {
        connect(qApp, &QGuiApplication::applicationStateChanged, this, [this](Qt::ApplicationState state) {
            if (state != Qt::ApplicationActive)
                restoreCursor();
        });
    }
}

void ResizeSplitterHandle::applyResizeCursor()
{
    if (m_cursorOverridden)
        return;

    QGuiApplication::setOverrideCursor(orientation() == Qt::Horizontal ? Qt::SplitHCursor : Qt::SplitVCursor);
    m_cursorOverridden = true;
}

void ResizeSplitterHandle::restoreCursor()
{
    if (!m_cursorOverridden)
        return;

    QGuiApplication::restoreOverrideCursor();
    m_cursorOverridden = false;
}

void ResizeSplitterHandle::enterEvent(QEnterEvent *event)
{
    m_hovered = true;
    applyResizeCursor();
    update();
    QSplitterHandle::enterEvent(event);
}

void ResizeSplitterHandle::leaveEvent(QEvent *event)
{
    m_hovered = false;
    restoreCursor();
    update();
    QSplitterHandle::leaveEvent(event);
}

void ResizeSplitterHandle::hideEvent(QHideEvent *event)
{
    m_hovered = false;
    restoreCursor();
    QSplitterHandle::hideEvent(event);
}

void ResizeSplitterHandle::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    const QRect r = rect();
    const QColor lineColour = m_hovered ? palette().color(QPalette::Highlight)
                                        : palette().color(QPalette::Mid);
    const int thickness = m_hovered ? 2 : 1;

    if (orientation() == Qt::Horizontal) {
        //A horizontal splitter has vertical handles.
        const int x = r.center().x() - thickness / 2;
        painter.fillRect(QRect(x, r.top() + 2, thickness, r.height() - 4), lineColour);
    } else {
        const int y = r.center().y() - thickness / 2;
        painter.fillRect(QRect(r.left() + 2, y, r.width() - 4, thickness), lineColour);
    }
}

ResizeSplitter::ResizeSplitter(QWidget *parent) : QSplitter(parent)
{
    setHandleWidth(8);
}

ResizeSplitter::ResizeSplitter(Qt::Orientation orientation, QWidget *parent) :
    QSplitter(orientation, parent)
{
    setHandleWidth(8);
}

QSplitterHandle *ResizeSplitter::createHandle()
{
    return new ResizeSplitterHandle(orientation(), this);
}
