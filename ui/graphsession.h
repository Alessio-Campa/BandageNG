#pragma once

#include "graph/graphscope.h"
#include "graph/nodecolorer.h"
#include "graph/path.h"
#include "program/settings.h"

#include <QList>
#include <QSharedPointer>
#include <QString>

#include <memory>

class AssemblyGraph;
class AnnotationsManager;
class BandageGraphicsView;
class BandageGraphicsScene;
class GraphSearchDialog;
class GraphicsViewZoom;
class QStackedWidget;
class QWidget;

namespace search {
class GraphSearch;
}

//Everything that is specific to one open graph tab.
struct GraphSession {
    QSharedPointer<AssemblyGraph> graph;
    std::shared_ptr<AnnotationsManager> annotations;
    std::shared_ptr<search::GraphSearch> search;
    GraphSearchDialog *searchDialog = nullptr;

    BandageGraphicsView *view = nullptr;
    BandageGraphicsScene *scene = nullptr;
    GraphicsViewZoom *zoom = nullptr;
    QStackedWidget *page = nullptr;
    QWidget *startPage = nullptr;

    QString fileName;
    int uiState = 0; //UiState values

    double absoluteZoom = 1.0;
    double zoomReference = 1.0;
    double previousZoomSpinBoxValue = 100.0;
    double lastAbsoluteZoom = 1.0;

    GraphScope graphScope = WHOLE_GRAPH;
    int nodeDistance = 0;
    bool startingNodesExactMatch = true;
    QString startingNodes;
    QString startingPaths;
    QString startingWalks;
    double minDepthRange = 10.0;
    double maxDepthRange = 100.0;
    double nodeWidth = 5.0;
    NodeColorScheme colorScheme = RANDOM_COLOURS;

    QString pathSelection1;
    QString pathSelection2;
    QString walkSelection1;
    QString walkSelection2;
    QString selectionSearchText;
    QString queryName;
    QString tagName;

    bool displayNodeCustomLabels = false;
    bool displayNodeCsvData = false;
    int displayNodeCsvDataCol = 0;
    AnnotationSettings annotationsSettings;

    Path userSpecifiedPath;
    QString userSpecifiedPathString;
    bool userSpecifiedPathCircular = false;
    QList<Path> queryPaths;

    ~GraphSession();
};
