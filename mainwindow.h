#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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
    void fileMenuSelected();
    void stageMenuSelected();

    void on_actionStageSetting_triggered();
    void on_actionOpenStage_triggered(bool checked);

private:
    Ui::MainWindow *ui;
    void setMenu();
    void setToolbar();
};

#endif // MAINWINDOW_H
