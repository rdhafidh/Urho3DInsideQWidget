#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <Urho3D/Container/Ptr.h>
#include <Urho3D/Core/Context.h>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}
class Urho3dWidget;
class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

 private:
  Ui::MainWindow *ui;
  Urho3dWidget *w;
  Urho3D::SharedPtr<Urho3D::Context> context;
};

#endif  // MAINWINDOW_H
