//Copyright 2017 Ryan Wick

//This file is part of Bandage

//Bandage is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//Bandage is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with Bandage.  If not, see <http://www.gnu.org/licenses/>.


#ifndef GRAPHICSITEMEDGE_H
#define GRAPHICSITEMEDGE_H

#include <QGraphicsPathItem>
#include <QPainterPath>
#include <QPointF>
#include <QLineF>

class DeBruijnEdge;
class AssemblyGraph;

namespace edgegeometry {
//The drawn start/end segments of an edge, taking single/double mode and
//reverse-complement display into account.
QLineF startSegment(const DeBruijnEdge *edge);
QLineF endSegment(const DeBruijnEdge *edge);
}

class GraphicsItemEdge : public QGraphicsPathItem {
public:
    explicit GraphicsItemEdge(DeBruijnEdge *deBruijnEdge, const AssemblyGraph &graph,
                              QGraphicsItem *parent = nullptr);

    void paint(QPainter * painter, const QStyleOptionGraphicsItem *, QWidget *) override;
    QPainterPath shape() const override;

    virtual void remakePath();
    DeBruijnEdge *edge() const { return m_deBruijnEdge; }

private:
    DeBruijnEdge *m_deBruijnEdge;
    QColor m_edgeColor;
    Qt::PenStyle m_penStyle;
    float m_width;
};

#endif // GRAPHICSITEMEDGE_H
