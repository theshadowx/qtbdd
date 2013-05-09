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


#ifndef NODE_H
#define NODE_H

#include "arrow.h"
#include "ellipseitem.h"
#include <QtGui>
#include <QtCore>
#include <QFile>



using namespace std;

class Node: public QGraphicsItem
{
public:
    enum Node_Type{ROOT=0,CHILD_0,CHILD_1};                     // Type of Node

    Node(QGraphicsItem* parent=0);                              // Constructor
    Node(QString Func,QGraphicsItem* parent=0);                 // Constructor
    virtual ~Node();                                            // destructor


    int Ncount;                                                 // Number of the the node
    int Nm;                                                     // Number of monominal
    int Nl;                                                     // Number of literals
    QChar ltrl;                                                 // Current literal
    QList<QChar> LitTab;                                        // Literals list
    QStringList Mtab;                                           // Monomials list
    QString ExpFunc;                                            // Expression of the current function
    Node_Type Type;

    Node *fv_0;                                                 // Left Leaf
    Node *fv_1;                                                 // Right Leaf
    Node *fv_prv;                                               // Previous node

    int PosX;                                                   // X coordinate of Node position
    int PosY;                                                   // Y coordinate of Node position
    QColor color[2];                                            // Color of the node


    Node *getRootNd();                                          // Get Root Node
    Node *getNd(qreal x, qreal y);                              // Get the that its bounding rect contain the point (x,y)

    void setNextArrowColor(QColor color);                       // Change the color of the next arrows
    void setPrevArrowColor(QColor color);                       // change the color of the previous arrow
    void fillArray(const QString &Func);                        // From the function given fill the list of monominals and literals
    void makeFunExp();                                          // Set function expression
    bool verLit(QChar lit, int NumM);                           // check whether there a literal still exists in at least a monominal, if not Remove the literals from the list.
    void verMon();                                              // check whether there is monominals equal to 0 or 1,
                                                                // if equals to 0 remove it from the list,
                                                                // if it is equal to 1 set 1 as the result of the function.
    void addLeafRight();                                        // Add Left leaf
    void addLeafLeft();                                         // Add Right leaf
    bool hasLit(QChar Lit, int Im) const;                       // Has the monome a literal
    void removeLit(QChar lit);                                  // Remove literal
    void removeMon(int Num_mon);                                // Remove monominal
    void setToZero(QChar Lit);                                  // Set literal to 0
    void setToOne(QChar ch);                                    // Set literal to 1

    void operator =(const Node &node);
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*);

//private:

    Arrow *arrow_0;                                             // Left Arrow
    Arrow *arrow_1;                                             // Right Arrow
    Arrow *arrow_prv;                                           // Previous Arrow
    EllipseItem *nodeEllipse;                                   // Ellipse (Node shape)

};
#endif // NODE_H
