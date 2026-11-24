#include "graphsession.h"

#include "bandagegraphicsscene.h"
#include "bandagegraphicsview.h"
#include "dialogs/graphsearchdialog.h"
#include "graphicsviewzoom.h"

#include <QStackedWidget>

GraphSession::~GraphSession()
{
    delete searchDialog;
    searchDialog = nullptr;

    if (view)
        view->setScene(nullptr);

    if (page) {
        page->setParent(nullptr);
        delete page;
        page = nullptr;
    }

    delete scene;
    scene = nullptr;
}
