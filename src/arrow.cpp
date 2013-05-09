/****************************************************************************
** Copyright (C) 2012 Ali Diouri.
** All rights reserved.
** Contact: Ali Diouri (ali.diouri@gmail.com)
**
** Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are met:
**     * Redistributions of source code must retain the above copyright
**       notice, this list of conditions and the following disclaimer.
**     * Redistributions in binary form must reproduce the above copyright
**       notice, this list of conditions and the following disclaimer in the
**       documentation and/or other materials provided with the distribution.
**     * Neither the name of the <organization> nor the
**       names of its contributors may be used to endorse or promote products
**       derived from this software without specific prior written permission.

** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
** ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
** DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
** DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
** (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
** LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
** ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
****************************************************************************/

#include "arrow.h"

#define PI 3.14159265


Arrow::Arrow(QPointF startPointNode, QPointF endPointNode,
             QGraphicsItem *parent)
    : QGraphicsLineItem(parent)
{
    setFlags(QGraphicsItem::ItemIsMovable| QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    myStartPoint = startPointNode;
    myEndPoint = endPointNode;
    myColor = Qt::black;
    myPen=QPen(myColor, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
}
/*********************************************************************/

QRectF Arrow::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

/*********************************************************************/

QPainterPath Arrow::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}

/*********************************************************************/

void Arrow::setColor(const QColor color)
{
    myColor = color;
}

/*********************************************************************/
void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *,
          QWidget *)
{
    painter->setClipping(true);
    painter->setPen(QPen(myColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawLine(myStartPoint,myEndPoint);
}

/*********************************************************************/
QPointF Arrow::startPoint() const
{
    return myStartPoint;
}

/*********************************************************************/

QPointF Arrow::endPoint() const
{
    return myEndPoint;
}

/*********************************************************************/

void Arrow::setStartPoint(QPointF startPoint)
{
    myStartPoint = startPoint;
}

/*********************************************************************/

void Arrow::setEndPoint(QPointF endPoint)
{
    myEndPoint = endPoint;
}



