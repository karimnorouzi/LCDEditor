#include "mainwindow.h"
#include<QRect>
#include<QPainter>
#include <QPoint>
#include<QMenuBar>
#include<QDialog>
#include<QFileDialog>
#include<QVBoxLayout>
#include<QComboBox>
#include<QLineEdit>
#include<QDialogButtonBox>
#include<QLabel>
#include<QMouseEvent>
#include<vector>
#include<QMessageBox>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("LCD Painter ...");
    setGeometry(100,100, 800, 600);
    createActions();
    createMenus();
    cols = 0;
    rows = 0;


}
void MainWindow::paintEvent(QPaintEvent*)
{
    QPainter painter(this);
    int h = this->height()-1;
    int w = this->width()-1;

    QRect rect(0,0,w,h);
    if (rows > 0 && cols > 0)
    {
        dx = (w)/cols;
        dy = (h-50)/rows;
        for(int i=0; i <cols; i++)
        for(int j=0; j< rows; j++)
        {
            rect.setCoords(i*dx, j*dy+45, i*dx+dx, j*dy+dy+45);
            if (matrix[i][j])
            {
                painter.setBrush(Qt::SolidPattern);
                painter.drawRect(rect);
            }
            else
            {

                painter.setBrush(Qt::NoBrush);
                painter.drawRect(rect);
            }
        }

    }
 //painter.drawRect(rect);
 //   painter.drawEllipse(QPoint(0,0), 40,60);
}
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    int h = this->height()-1;
    int w = this->width()-1;
    dx = (w)/cols;
    dy = (h-45)/rows;

    QRect rect(0,0,w,h);

    int x = event->x();
    int y = event->y()-45;
    int i = x/dx;
    int j = y/dy;
    if (matrix[i][j])
        matrix[i][j] = 0;
    else
        matrix[i][j] = 1;
    update();


}

void MainWindow::createActions()
{
    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
    exitAction->setStatusTip(tr("Quit Scenediagram example"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    newAction = new QAction(tr("&New"), this);
    newAction->setShortcuts(QKeySequence::New);
    newAction->setStatusTip(tr("new LCD image"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newDialog()));

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcuts(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save to current file"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveDialog()));

    openAction = new QAction(tr("&Open"), this);
    openAction->setShortcuts(QKeySequence::Open);
    openAction->setStatusTip(tr("Open existing file"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(openDialog()));

    redoAction = new QAction(tr("&Redo"), this);
    redoAction->setShortcuts(QKeySequence::Redo);
    redoAction->setStatusTip(tr("Redo"));
    connect(redoAction, SIGNAL(triggered()), this, SLOT(close()));



}
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(exitAction);


    //editMenu = menuBar()->addMenu(tr("&Edit"));
    //editMenu->addAction(undoAction);
    //editMenu->addAction(redoAction);
}
void MainWindow::newDialog()
{
    QDialog* newSizeDialog = new QDialog();
    QVBoxLayout* vbox = new QVBoxLayout();
    QLineEdit* lineEditRows = new QLineEdit();
    QLineEdit* lineEditCols = new QLineEdit();
    QLabel* LabelRows = new QLabel("Number of rows:");
    QLabel* LabelCols = new QLabel("Number of columns:");
    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                        | QDialogButtonBox::Cancel);

    QObject::connect(buttonBox, SIGNAL(accepted()), newSizeDialog, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), newSizeDialog, SLOT(reject()));

    lineEditRows->setValidator( new QIntValidator(1, 255, this) );
    lineEditCols->setValidator( new QIntValidator(1, 255, this) );

    vbox->addWidget(LabelRows);
    vbox->addWidget(lineEditRows);
    vbox->addWidget(LabelCols);
    vbox->addWidget(lineEditCols);
    vbox->addWidget(buttonBox);

    newSizeDialog->setLayout(vbox);
    newSizeDialog->setWindowTitle("Enter new LCD size");
    int result = newSizeDialog->exec();
    if(result == QDialog::Accepted)
    {
        rows = lineEditRows->text().toInt();
        cols = lineEditCols->text().toInt();
        // call the new method to create a new matrix.
        for(int i=0; i< rows; i++)
            for (int j=0; j< cols; j++)
                matrix[i][j] = 0;
    }
}
void MainWindow::saveDialog()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save to File"),
                                                    ".",
                                                    tr("LCD file (*.lcd)"));
    if (fileName.isEmpty())
            return;
        else {
            QFile file(fileName);
            if (!file.open(QIODevice::WriteOnly)) {
                QMessageBox::information(this, tr("Unable to open file"),
                    file.errorString());
                return;
            }
            QDataStream out(&file);

            out << rows << cols;
            for(int i=0; i < rows; i++)
                for (int j=0; j< cols; j++)
                    out << int(matrix[i][j]);

        }
}



void MainWindow::openDialog()
{
    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open existing file"),
            ".",
            tr("LCD file (*.lcd);;All Files (*)"));
    if (fileName.isEmpty())
            return;
        else {

            QFile file(fileName);

            if (!file.open(QIODevice::ReadOnly)) {
                QMessageBox::information(this, tr("Unable to open file"),
                    file.errorString());
                return;
            }

            QDataStream in(&file);
            //in.setVersion(QDataStream::Qt_4_5);
            int number;
            in >> rows >> cols ;
            for(int i=0; i < rows; i++)
                for (int j=0; j< cols; j++)
                {
                    in >> number;
                    matrix[i][j] = number;
                };

        }
}

MainWindow::~MainWindow()
{

}
