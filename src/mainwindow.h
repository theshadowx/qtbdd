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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphicsview.h"
#include <QMainWindow>
#include <QtGui>
#include <QtCore>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_pushButton_clicked();
    void on_lineEdit_returnPressed();

    void newFile();
    void print();
    void pan();
    void selector();
    void about();
    void aboutQt();

private:

    QMenu          *fileMenu;
    QMenu          *editMenu;
    QMenu          *helpMenu;
    QActionGroup   *alignmentGroup;
    QAction        *newAct;
    QAction        *selectorAct;
    QAction        *panAct;
    QAction        *printAct;
    QAction        *quitAct;
    QAction        *aboutAct;
    QAction        *aboutQtAct;
    QLabel         *infoLabel;

    QMenuBar       *menuBar;
    QStatusBar     *statusBar;
    QLabel         *zoomLabel;
    QWidget        *centralWidget;
    QGridLayout    *gridLayout_0;
    QGridLayout    *gridLayout_1;
    QLabel         *label;
    QLineEdit      *lineEdit;
    QPushButton    *pushButton;
    QFrame         *line;
    graphicsView   *view;

protected:

    virtual bool eventFilter(QObject * obj, QEvent *event);
    virtual void resizeEvent(QResizeEvent* );

};

#endif // MAINWINDOW_H
