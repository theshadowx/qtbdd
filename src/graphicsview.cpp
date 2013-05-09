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

#include "graphicsview.h"
 
/**
* Sets up the subclassed QGraphicsView
*/

graphicsView::graphicsView(QWidget* parent) : QGraphicsView(parent)
{
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    //Set-up the scene
    scene = new graphicsScene(this);
    setScene(scene);
    setCursor(Qt::ArrowCursor);
    setMouseTracking(true);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);

}
 
/**
  * Handles when the mouse button is pressed
  */

void graphicsView::mousePressEvent(QMouseEvent* event)
{
    QGraphicsView::mousePressEvent(event);
    //For panning the view
    if(cursor().shape()==Qt::OpenHandCursor) {
        LastPanPoint = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
}

/**
  * Handles when the mouse button is released
  */
void graphicsView::mouseReleaseEvent(QMouseEvent* event) {

    QGraphicsView::mouseReleaseEvent(event);

    if(cursor().shape()==Qt::ClosedHandCursor) {
        setCursor(Qt::OpenHandCursor);
        LastPanPoint = QPoint();
    }
}

/**
*Handles the mouse move event
*/
void graphicsView::mouseMoveEvent(QMouseEvent* event)
{
    event->ignore();
}

/**
  * Zoom the view in and out.
  */

void graphicsView::wheelEvent(QWheelEvent* event)
{

    if(scene->bdd){
        //Get the position of the mouse before scaling, in scene coords
        QPointF pointBeforeScaleScene(mapToScene(event->pos()));

        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

        //Scale the view ie. do the zoom
        double scaleFactor = 1.15; //How fast we zoom

        if(event->delta() > 0) {
            //Zoom in, limit at 300%
            if(transform().m11() <= (qreal) 3.0)
                scale(scaleFactor, scaleFactor);

        }else {
            //Zooming out

            QRect bddBr =  scene->bdd->mapRectToScene(scene->bdd->boundingRect()).toRect();
            QPointF bddBrPointSceneF = mapToScene(QPoint(bddBr.x(),bddBr.y()));
            QRect  bddBrScene  = QRect(bddBrPointSceneF.x(),
                                       bddBrPointSceneF.y(),
                                       bddBr.width(),
                                       bddBr.height());


            if(!this->visibleRegion().boundingRect().contains(bddBrScene))
                scale(1.0 / scaleFactor, 1.0 / scaleFactor);

        }


        QPointF pointAfterScaleScene(mapToScene(event->pos()));
        QPointF offset = pointAfterScaleScene - pointBeforeScaleScene ;

        scene->bdd->moveBy(offset.x(),offset.y());
    }

    QApplication::sendEvent(parentWidget(),event);
}

/**
  * Need to update the center so there is no jolt in the
  * interaction after resizing the widget.
  */
void graphicsView::resizeEvent(QResizeEvent* event)
{
    //Get the rectangle of the visible area in scene coords
    QRectF visibleArea = mapToScene(rect()).boundingRect();

    //Call the subclass resize so the scrollbars are updated correctly
    QGraphicsView::resizeEvent(event);
}

void graphicsView::drawBackground(QPainter*painter, const QRectF &rect)
{

    Q_UNUSED(rect);

    QRectF sceneRect = this->sceneRect();
    QLinearGradient gradient(sceneRect.topLeft(), sceneRect.bottomRight());
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect, gradient);
    painter->setBrush(Qt::NoBrush);
}
