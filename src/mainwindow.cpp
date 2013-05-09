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

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setGeometry(0,0,605,429);
    this->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    this->setWindowTitle("Binary Decision Diagram (BDD)");

    menuBar     = new QMenuBar(this);
    newAct      = new QAction(tr("&New"), this);
    printAct    = new QAction(tr("&Print..."), this);
    quitAct     = new QAction(tr("&Quit"), this);
    panAct      = new QAction(tr("P&an Tool"), this);
    selectorAct = new QAction(tr("Se&lect Tool"), this);
    aboutAct    = new QAction(tr("A&bout"), this);
    aboutQtAct  = new QAction(tr("Abo&ut Qt"), this);

    newAct->setShortcut(tr("Ctrl+N"));
    printAct->setShortcut(tr("Ctrl+P"));
    selectorAct->setShortcut(tr("Ctrl+L"));
    aboutAct->setShortcut(tr("Ctrl+B"));
    aboutQtAct->setShortcut(tr("Ctrl+U"));
    quitAct->setShortcut(tr("Ctrl+Q"));


    fileMenu = menuBar->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAct);

    editMenu = menuBar->addMenu(tr("&Edit"));
    editMenu->addAction(panAct);
    editMenu->addAction(selectorAct);

    helpMenu = menuBar->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    this->setMenuBar(menuBar);

    /// Label
    QFont font;
    font.setBold(true);
    label = new QLabel("Function Z=",this);
    label->setGeometry(1,1,81,27);
    label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    label->setFont(font);

    /// Line Edit
    lineEdit = new QLineEdit(this);
    lineEdit->setGeometry(88,2,405,25);
    lineEdit->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    lineEdit->setFocus();

    /// Push Button
    pushButton = new QPushButton("OK",this);
    pushButton->setGeometry(499,1,85,27);
    pushButton->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

    /// Line Separator
    line = new QFrame(this);
    line->setGeometry(QRect(/* ... */));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    /// View
    view = new graphicsView(this);
    view->setGeometry(10,53,585,344);
    view->setRenderHint(QPainter::Antialiasing,true);
    view->setRenderHint(QPainter::SmoothPixmapTransform,true);
    view->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->installEventFilter(this);

    /// Zoom Label
    zoomLabel= new QLabel(this);
    zoomLabel->setText("100%");

    /// Status Bar
    statusBar= new QStatusBar(this);
    statusBar->addPermanentWidget(zoomLabel);

    /// Grid Lauyout 0
    gridLayout_0 = new QGridLayout;
    gridLayout_0->addWidget(label,0,0,0);
    gridLayout_0->addWidget(lineEdit,0,1,0);
    gridLayout_0->addWidget(pushButton,0,2,0);
    gridLayout_0->addWidget(line,1,0,1,3,0);
    gridLayout_0->setMargin(0);
    gridLayout_0->setHorizontalSpacing(6);
    gridLayout_0->setVerticalSpacing(6);

    /// Grid Layout 1
    gridLayout_1 = new QGridLayout;
    gridLayout_1->addLayout(gridLayout_0,0,0);
    gridLayout_1->addWidget(view,1,0,0);
    gridLayout_1->setMargin(10);

    /// Central Widget
    centralWidget = new QWidget(this);
    centralWidget->setGeometry(0,23,605,406);
    centralWidget->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    centralWidget->setLayout(gridLayout_1);

    this->setCentralWidget(centralWidget);
    this->setStatusBar(statusBar);

    QObject::connect(pushButton, SIGNAL(clicked()), this, SLOT(on_pushButton_clicked()));
    QObject::connect(lineEdit, SIGNAL(returnPressed()), this, SLOT(on_lineEdit_returnPressed()));

    QObject::connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));
    QObject::connect(printAct, SIGNAL(triggered()), this, SLOT(print()));
    QObject::connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));

    QObject::connect(panAct, SIGNAL(triggered()), this, SLOT(pan()));
    QObject::connect(selectorAct, SIGNAL(triggered()), this, SLOT(selector()));

    QObject::connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));
    QObject::connect(aboutQtAct, SIGNAL(triggered()), this, SLOT(aboutQt()));
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_pushButton_clicked()
{
    /// Checking that something is written in lineEdit
    if(lineEdit->text().isEmpty()) {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText("Please write the function expression");
        msgBox.exec();
        this->activateWindow();
        lineEdit->setFocus();
        return;
    }

    /// Checking that all the literals are alphabetic characters
    QString lineEditText = lineEdit->text();
    lineEditText=lineEditText.replace("+","");
    lineEditText=lineEditText.replace(" ","");
    QStringList lineEditStrings = lineEditText.split("");
    QChar lineEditChar;
    for(int i=0;i<lineEditStrings.size();i++) {
        QString str = lineEditStrings[i];
        for(int j=0;j<str.size();j++) {
            lineEditChar = str[j];
            if((90>=lineEditChar && lineEditChar>=65) || (122>=lineEditChar && lineEditChar>=97)){
                ;
            }else {
                QMessageBox msgBox;
                msgBox.setIcon(QMessageBox::Warning);
                msgBox.setText("Literals must be alphabetic characters");
                msgBox.exec();
                this->activateWindow();
                lineEdit->setFocus();
                return;
            }
        }
    }

    /// Deleting the previous bdd, if it already exists
    if(view->scene->bdd) {
        delete view->scene->bdd;
        view->scene->bdd = 0;
    }

    /// Build the new BDD and add it to the scene
    view->scene->clear();
    view->scene->bdd = new BDD(lineEdit->text());

    /// Draw the BDD
    view->setSceneRect(view->scene->bdd->boundingRect());
    view->scene->addItem(view->scene->bdd);

    /// Setting the center of the view in the same position as BDD center
    view->fitInView(view->scene->bdd->boundingRect(),Qt::KeepAspectRatio);
    zoomLabel->setText(QString("%1%").arg((int)view->transform().m11()*100));
}
/**************************************************************************/
/**************************************************************************/
void MainWindow::on_lineEdit_returnPressed()
{
    on_pushButton_clicked();
}
/**************************************************************************/
/**************************************************************************/
bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::Wheel) {
        int scaleValue = view->transform().m11()*100;
        zoomLabel->setText(QString("%1%").arg(scaleValue));

        QMouseEvent* mouseEvent = static_cast<QMouseEvent*> (event);
        statusBar->showMessage(QString("%1,%2")
                               .arg((int)view->mapToScene(mouseEvent->pos()).x())
                               .arg((int)view->mapToScene(mouseEvent->pos()).y()));
    }

    if (event->type() == QEvent::MouseMove || event->type() == QEvent::Wheel) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*> (event);


        statusBar->showMessage(QString("%1,%2||%3,%4")
                               .arg((int)view->mapToScene(mouseEvent->pos()).x())
                               .arg((int)view->mapToScene(mouseEvent->pos()).y())
                               .arg(mouseEvent->pos().x())
                               .arg(mouseEvent->pos().y()));
    }

    return QMainWindow::eventFilter(obj, event);
}
/**************************************************************************/
/**************************************************************************/
void MainWindow::newFile()
{
    if(view->scene->bdd) {
        delete view->scene->bdd;
        view->scene->bdd=0;
    }
    delete view->scene;
    graphicsScene* scene = new graphicsScene(this);
    view->setScene(scene);
    lineEdit->clear();
    // Set the default scale
    qreal scaleFactor = view->transform().m11();
    view->scale(1/scaleFactor,1/scaleFactor);
    zoomLabel->setText(QString("%1").arg((int)scaleFactor*100));
}

/**************************************************************************/
/**************************************************************************/
void MainWindow::print()
{
    QString text("<p align='center'>Binary Decision Diagram(BDD)<br>"
            "Version 0.1<br>"
            "By Ali Diouri<br>"
          "ali.diouri@gmail.com<br>"
   "Licensed under BSD 3-Clause License</p>");

    QPrinter printer;
    printer.setPaperSize(QPrinter::A4);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("/home/theshadowx7/Bureau/BDD.pdf");
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec() == QDialog::Accepted) {
        QTextDocument textDoc(this);
        QPainter painter(&printer);
        painter.setRenderHint(QPainter::Antialiasing);
        //textDoc.setPlainText(text);
        textDoc.setPlainText(this->lineEdit->text());
        this->view->scene->render(&painter);
        textDoc.drawContents(&painter);
    }
}
/**************************************************************************/
/**************************************************************************/
void MainWindow::pan()
{
    view->setCursor(Qt::OpenHandCursor);
}
/**************************************************************************/
/**************************************************************************/
void MainWindow::selector()
{
    view->setCursor(Qt::ArrowCursor);
}
/**************************************************************************/
/**************************************************************************/
void MainWindow::about(){
    QMessageBox msgBox;
    msgBox.setWindowTitle("About");
    msgBox.addButton(QMessageBox::Ok);
    msgBox.setText("<p align='center'>Binary Decision Diagram(BDD)<br>"
                        "Version 0.1<br>"
                        "By Ali Diouri<br>"
                      "ali.diouri@gmail.com<br>"
               "Licensed under BSD 3-Clause License</p>");
    msgBox.setIcon(QMessageBox::Information);
    int selection = msgBox.exec();
    if(selection == QMessageBox::Yes) {
        this->activateWindow();
        lineEdit->setFocus();
    }
}
/**************************************************************************/
/**************************************************************************/
void MainWindow::aboutQt()
{
    QMessageBox::aboutQt(this, "About Qt");
}
/**************************************************************************/
/**************************************************************************/
void MainWindow::resizeEvent(QResizeEvent*)
{
    if(view->scene->bdd){
        view->fitInView(view->scene->bdd->boundingRect(),Qt::KeepAspectRatio);
        zoomLabel->setText(QString("%1%").arg((int)view->transform().m11()*100));
    }
}
