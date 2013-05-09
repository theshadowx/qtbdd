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

#include "bdd.h"

BDD::BDD(QString Func,QGraphicsItem *parent)
: QGraphicsItem(parent)
{
    setFlags(QGraphicsItem::ItemIsMovable|
             QGraphicsItem::ItemIsSelectable|
             QGraphicsItem::ItemIsFocusable);

    selectNd = 0;
    rootNd = new Node(this);
    rootNd->Type = rootNd->ROOT;
    rootNd->fillArray(Func);
    build(rootNd);
    acceptedMouseButtons();
}

BDD::~BDD()
{

}

/*********************************************************************/

void BDD::build(Node *Nd)
{
    if(Nd->Nm) {
        Nd->addLeafLeft();
        if(Nd->fv_0->Nm>0)
            build(Nd->fv_0);

        Nd->addLeafRight();
        if(Nd->fv_1->Nm>0)
            build(Nd->fv_1);
    }
}

/*********************************************************************/

QRectF BDD::boundingRect() const
{
    QPointF bddPos = this->pos();
    int dist = 0;
    int offsetPy = -3*40+30;
    for(int i=1;i<rootNd->Nl;i++)
        dist += qPow(2,i)*15;
    int Px = -45-dist+bddPos.x();
    int Py = -40+offsetPy+bddPos.y();
    int w = 2*(dist+45);
    int h = 40*2+40*(rootNd->Nl);
    return QRectF(QPoint(Px,Py),QSize(w,h));
}

/*********************************************************************/

void BDD::paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *)
{
}

///*********************************************************************/

void BDD::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    /// Get position of the mouse click
    QPointF posNode = event->pos();
    /// Change to the default color
    if(selectNd){
        selectNd->color[0]=Qt::yellow;
        selectNd->color[1]=Qt::darkYellow;
        selectNd->setPrevArrowColor(Qt::black);
        selectNd->setNextArrowColor(Qt::black);
    }
    /// look for node which its boundingRect countains the mouse click coordinates
    selectNd = rootNd->getNd(posNode.x(),posNode.y());

    /// Change the color of the selected node
    if(selectNd){
        selectNd->color[0]=Qt::red;
        selectNd->color[1]=Qt::darkRed;
        selectNd->setPrevArrowColor(Qt::red);
        selectNd->setNextArrowColor(Qt::blue);
    }

    update();
    QGraphicsItem::mousePressEvent(event);
}

/*********************************************************************/

void BDD::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);

}
