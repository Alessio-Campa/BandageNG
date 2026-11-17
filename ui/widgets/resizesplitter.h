#ifndef RESIZESPLITTER_H
#define RESIZESPLITTER_H

#include <QSplitter>

//A QSplitter with a more forgiving handle: the handle is wider, it highlights
//on hover and it forces the resize cursor while the mouse is over it. The last
//part works around Qt/macOS sometimes failing to update the cursor over thin
//splitter handles.
class ResizeSplitterHandle : public QSplitterHandle
{
public:
    ResizeSplitterHandle(Qt::Orientation orientation, QSplitter *parent);

protected:
    void enterEvent(QEnterEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    void applyResizeCursor();
    void restoreCursor();

    bool m_cursorOverridden = false;
    bool m_hovered = false;
};

class ResizeSplitter : public QSplitter
{
public:
    explicit ResizeSplitter(QWidget *parent = nullptr);
    explicit ResizeSplitter(Qt::Orientation orientation, QWidget *parent = nullptr);

protected:
    QSplitterHandle *createHandle() override;
};

#endif // RESIZESPLITTER_H
