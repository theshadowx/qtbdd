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

#include "node.h"

Node::Node(QGraphicsItem* parent)
    : QGraphicsItem(parent)
{
    ltrl = 0;
    Nl = 0;
    Nm = 0;
    Ncount = 0;

    fv_0 = 0;
    fv_1 = 0;
    fv_prv = 0;

    PosX = 0;
    PosY = 0;

    arrow_0 = new Arrow(QPointF(PosX,PosY), QPointF(PosX,PosY), this);
    arrow_1 = new Arrow(QPointF(PosX,PosY), QPointF(PosX,PosY), this);
    arrow_prv = new Arrow(QPointF(PosX,PosY), QPointF(PosX,PosY), this);
    nodeEllipse = new EllipseItem(QRectF(this->boundingRect()),this);

    color[0] = Qt::yellow;
    color[1] = Qt::darkYellow;

}

Node::Node(QString Func,QGraphicsItem* parent)
    : QGraphicsItem(parent)
{

    ltrl=0;
    Nl=0;
    Nm=0;
    Ncount=0;

    fv_0=0;
    fv_1=0;
    fv_prv=0;

    PosX=0;
    PosY=0;
    Type=ROOT;

    arrow_0 = new Arrow(QPointF(PosX,PosY), QPointF(PosX,PosY), this);
    arrow_1 = new Arrow(QPointF(PosX,PosY), QPointF(PosX,PosY), this);
    arrow_prv = new Arrow(QPointF(PosX,PosY), QPointF(PosX,PosY), this);
    nodeEllipse = new EllipseItem(QRectF(this->boundingRect()),this);

    color[0] = Qt::yellow;
    color[1] = Qt::darkYellow;

    fillArray(Func);

}

Node::~Node()
{
}

/*********************************************************************/

void	Node::fillArray(QString const & Func)
{

    QString Function=Func;
    Function=Function.replace(" ","");
    Mtab=Function.split("+");
    Mtab.removeDuplicates();
    for(int j=0;j<Mtab.count();j++) {
        if(Mtab[j]=="") {
            Mtab.removeAt(j);
            j--;
        }
    }

    Nm=Mtab.count();

    // Remplissage du tableau LitTab ( tableau des literaux)

    for(int i=0;i<Nm;i++) {
        QString str=Mtab[i];
        for(int j=0;j<str.count();j++) {
            QChar ch = str[j];
            if(!LitTab.contains(ch.toLower()))
                LitTab.append(ch.toLower());
        }
    }
    Nl=LitTab.count();
    ExpFunc = Function;
    ltrl =LitTab[0];
    verMon();
}

/*********************************************************************/
// ajout d'une branche Gauche/Droite au noeud

void	Node::addLeafLeft()
{


    fv_0=new Node(this);
    fv_0->fv_prv=this;
    fv_0->Type=CHILD_0;

    fv_0->Nm=Nm;
    fv_0->Nl=Nl;
    fv_0->Mtab=Mtab;
    fv_0->LitTab=LitTab;
    fv_0->Ncount=Ncount+1;

    QChar ltrl_=fv_0->LitTab[0];

    fv_0->removeLit(ltrl_);
    fv_0->setToZero(ltrl_);
    int NumM=fv_0->Nm-1;

    for(int i=0;i<fv_0->Nl;i++) {
        ltrl_=fv_0->LitTab[i];
        bool j=fv_0->verLit(ltrl_,NumM);
        if(j) i=0;
    }

    if(fv_0->Nm) {
        fv_0->ltrl=fv_0->LitTab[0];
    }else {
        if(fv_0->ltrl!=1) {
            fv_0->LitTab.clear();
            fv_0->Nl=0;
        }
    }

    fv_0->makeFunExp();

}

/*********************************************************************/

void	Node::addLeafRight()
{
    fv_1=new Node(this);
    fv_1->fv_prv=this;
    fv_1->Type=CHILD_1;

    fv_1->Nm = Nm;
    fv_1->Nl = Nl;
    fv_1->Mtab = Mtab;
    fv_1->LitTab = LitTab;
    fv_1->Ncount = Ncount+1;

    QChar ltrl_=fv_1->LitTab[0];

    fv_1->removeLit(ltrl_);
    fv_1->setToOne(ltrl_);

    int NumM=fv_1->Nm-1;
    for(int i=0;i<fv_1->Nl;i++) {
        ltrl_=fv_1->LitTab[i];
        bool j=fv_1->verLit(ltrl_,NumM);
        if(j) i=0;
    }

    if(fv_1->Nm)
        fv_1->ltrl=fv_1->LitTab[0];

    fv_1->makeFunExp();
}

/*********************************************************************/
// verification de l'existance d'un literal dans un monome numero Im

bool	Node::hasLit(QChar lit,int Im)const // Lit:literal Im:indice monome
{
    Q_ASSERT_X(Im>=0,"HasLit()","Negatif second argument");
    QString str=Mtab[Im];
    return str.contains(lit);
}

/*********************************************************************/
// Supression d'un literal de la liste des literaux

void	Node::removeLit(QChar lit)
{
    int i=LitTab.indexOf(lit);
    LitTab.removeAt(i);
    Nl--;
}
//! [7]
/*********************************************************************/
// Supression d'un monome depuis la liste des monomes

void	Node::removeMon(int Num_mon)   // Num_mon = numero du monome à suprimer
{
    Mtab.removeAt(Num_mon);
    Nm--;

}
/*********************************************************************/
// Mis à 0 d'un literal et tous les monomes le contenant

void	Node::setToZero(QChar lit)    // Lit: literal, NumM:numero du monome
{
    for(int i=0;i<Nm;i++) {
        if(hasLit(lit,i)) {
            removeMon(i);
            i--;
        }else {
            if(hasLit(lit.toUpper(),i)) {
                QString monome=Mtab[i];
                monome.remove(lit.toUpper());
                if(!monome.count()) {
                    Mtab.clear();
                    LitTab.clear();
                    Nm=0;
                    Nl=0;
                    ltrl='1';
                    return;
                }else
                Mtab.replace(i,monome);
            }
        }
    }
    if(!Nm)
        ltrl='0';
}

/*********************************************************************/
// mis à 1 d'un literal et le supprimer de tous les monomes le contenant
// s'il ne reste pas de literal dans un monome, on le remplace par 1.

void	Node::setToOne(QChar ch)
{
    for(int i=0;i<Nm;i++) {
         QString monome=Mtab[i];
         if(monome.contains(ch)) {
             monome.remove(ch);
             if(!monome.count()) {
                 Mtab.clear();
                 LitTab.clear();
                 Nm=0;
                 Nl=0;
                 ltrl='1';
             }else {
                 Mtab.replace(i,monome);
             }
         }
         if(monome.contains(ch.toUpper())) {
             removeMon(i);
             if(!Nm) {
                ltrl='0';
                return;
             }
             i--;
         }

    }
}

/*******************************************************************/
// ecrire l'expression de la fonction actuelle

void	Node::makeFunExp(void)
{
    if(Nm) {
        ExpFunc=Mtab[0];
        for(int i=1;i<Nm;i++) {
            if(Mtab[i].count()!=0)	ExpFunc=ExpFunc + '+' + Mtab[i];
        }
    }else {
       ExpFunc=ltrl;
    }
}

/*********************************************************************/
// suppression d'un literal s'il n'existe plus dans dans les monomes

bool Node::verLit(QChar lit,int NumM)    // literal, numero du monome
{
    bool litNotExist = true;

    if (NumM>=0) {
        for(int i=0;i<NumM+1 ;i++) {
            QString str=Mtab[i];
            if(str.contains(lit)||str.contains(lit.toUpper()))
                 litNotExist = false;
        }
        if(litNotExist){
            NumM--;
            verLit(lit,NumM);
        }
    }else {
        removeLit(lit);
        litNotExist = true;
    }
    return litNotExist;
}

/*********************************************************************/

void Node::verMon()
{
    for(int i=0;i<Nm ;i++) {
        if (Mtab[i]=="0") {
            Mtab.removeAt(i);
            --Nm;
            --i;
        }else {
            if (Mtab[i]=="1") {
                Mtab.clear();
                LitTab.clear();
                Nm=0;
                Nl=0;
                ltrl='1';
                return;
            }
        }
    }
}

/*********************************************************************/

Node *Node::getNd(qreal x, qreal y)
{
    Node *nodePtr=0;
    if(boundingRect().contains(x,y)){
        return this;
    }else{
        if(fv_0)
            nodePtr = fv_0->getNd(x,y);
        if(!nodePtr){
            if(fv_1)
                nodePtr = fv_1->getNd(x,y);
        }
    }
    return nodePtr;
}

/*********************************************************************/

void Node::operator =(const Node &node)
{
    ltrl=node.ltrl;
    ExpFunc=node.ExpFunc;
    Ncount=node.Ncount;
    Nm=node.Nm;
    Nl=node.Nl;
    Mtab=node.Mtab;
    LitTab=node.LitTab;
    PosX=node.PosX;
    PosY=node.PosY;
    Type=node.Type;
}

/*********************************************************************/

QRectF Node::boundingRect() const
{
    qreal radius=10;
    qreal diameter=2*radius;
    return QRectF(PosX-radius,
                  PosY-radius,
                  diameter,
                  diameter);
}

/*********************************************************************/

Node *Node::getRootNd()
{

    Node* nd;
    if(fv_prv) {
        nd =fv_prv->getRootNd();        
    }else {
        nd = this;
    }
    return nd;
}

/*********************************************************************/

void Node::setPrevArrowColor(QColor color)
{
    this->arrow_prv->setColor(color);
    if(this->fv_prv){
        if (this->Type == CHILD_0){
            this->fv_prv->arrow_0->setColor(color);
        }else if(this->Type == CHILD_1){
            this->fv_prv->arrow_1->setColor(color);
        }
        this->fv_prv->setPrevArrowColor(color);
    }
    return;
}

/*********************************************************************/

void Node::setNextArrowColor(QColor color)
{
    this->arrow_0->setColor(color);
    this->arrow_1->setColor(color);
    if(this->fv_0){
        this->fv_0->arrow_prv->setColor(color);
        this->fv_0->setNextArrowColor(color);
    }
    if(this->fv_1){
        this->fv_1->arrow_prv->setColor(color);
        this->fv_1->setNextArrowColor(color);
    }
    return;
}

/*********************************************************************/

void Node::paint(QPainter*painter, const QStyleOptionGraphicsItem *option, QWidget*)
{

    // Position of node
    int dist=0;
    for(int i=1;i<getRootNd()->Nl ;i++)
        dist+=qPow(2,i)*15;
    if(getRootNd()->Nl<=2)
        dist=64;

    if(Type==ROOT) {
         PosX=0;
         PosY=-100;

    }else if(Type==CHILD_1) {
            PosX=fv_prv->PosX+(dist/qPow(2,Ncount));
            PosY=fv_prv->PosY+40;
            arrow_prv->setStartPoint(QPointF(PosX,PosY-10));
            arrow_prv->setEndPoint(QPointF(fv_prv->PosX+(10*sqrt(2))/2,fv_prv->PosY+(10*sqrt(2))/2));
            fv_prv->arrow_1->setStartPoint(QPointF(fv_prv->PosX+(10*sqrt(2))/2,fv_prv->PosY+(10*sqrt(2))/2));
            fv_prv->arrow_1->setEndPoint(QPointF(PosX,PosY-10));

    }else if(Type==CHILD_0) {
            PosX=fv_prv->PosX-(dist/qPow(2,Ncount));
            PosY=fv_prv->PosY+40;
            arrow_prv->setStartPoint(QPointF(PosX,PosY-10));
            arrow_prv->setEndPoint(QPointF(fv_prv->PosX-(10*sqrt(2))/2,fv_prv->PosY+(10*sqrt(2))/2));
            fv_prv->arrow_0->setStartPoint(QPointF(fv_prv->PosX-(10*sqrt(2))/2,fv_prv->PosY+(10*sqrt(2))/2));
            fv_prv->arrow_0->setEndPoint(QPointF(PosX,PosY-10));
    }

    nodeEllipse->setRect(this->boundingRect());

    // Color of node
    QRadialGradient gradient(PosX, PosY, 15);
    if (option->state & QStyle::State_Sunken) {
        gradient.setCenter(PosX+3, PosY+3);
        gradient.setFocalPoint(PosX+3, PosY+3);
        gradient.setColorAt(1, QColor(color[0]).light(120));
        gradient.setColorAt(0, QColor(color[1]).light(200));
    } else {
        gradient.setColorAt(0, color[0]);
        gradient.setColorAt(1, color[1]);
    }

    nodeEllipse->setBrush(gradient);
    nodeEllipse->ltrl=ltrl;

}

