#ifndef URHO3DWIDGET_H
#define URHO3DWIDGET_H

#include <Urho3D/Container/Ptr.h>
#include <Urho3D/Core/Object.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3d/Core/Context.h>
#include <QWidget>

#include <Urho3D/Container/HashMap.h>
#include <Urho3D/Math/StringHash.h>

class Urho3dWidget : public QWidget, public Urho3D::Object {
  Q_OBJECT

  URHO3D_OBJECT(Urho3dWidget, Urho3D::Object)
 public:
  explicit Urho3dWidget(Urho3D::Context* context, QWidget* parent = nullptr);
  ~Urho3dWidget();

  void Start();
  void Stop();
  void Setup();
 Q_SIGNALS:

 public Q_SLOTS:

 protected:
  void closeEvent(QCloseEvent* e);
  void timerEvent(QTimerEvent* e);
  void resizeEvent(QResizeEvent* e);
  void keyPressEvent(QKeyEvent* e); 

 private:
  void HandleUpdate(Urho3D::StringHash eventType,
                    Urho3D::VariantMap& eventData);
  void createScene();
  void setupViewport();
  void MoveCamera();

  Urho3D::SharedPtr<Urho3D::Engine> engine_;
  Urho3D::SharedPtr<Urho3D::Scene> scene_;
  Urho3D::SharedPtr<Urho3D::Node> cameraNode_;
  /// Camera yaw angle.
  float yaw_;
  /// Camera pitch angle.
  float pitch_;
  int timerId;
  float timeStep=1;
  // Movement speed as world units per second
  const float MOVE_SPEED = 40.0f;
};

#endif  // URHO3DWIDGET_H