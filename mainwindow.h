#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFrame>
#include <vector>
#include<QAction>
#include<QDialog>
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    // data to hold the dot matrix canvas
    std::vector<std::vector<char>> canvas;
    int rows;
    int cols;
    int dx;
    int dy;
    void createActions(void);
    void createMenus(void);
    char matrix[256][256];
    QMenu* fileMenu;
    QMenu* editMenu;

    QAction* exitAction;
    QAction* redoAction;
    QAction* saveAction;
    QAction* openAction;
    QAction* newAction;
    QAction* undoAction;


public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent *);
 public slots:
    void newDialog();
    void saveDialog();
    void openDialog();
};

#endif // MAINWINDOW_H
