#include "urho3dwidget.h"

#include <SDL/SDL.h>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Math/Quaternion.h>
#include <Urho3D/Math/Vector3.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIElement.h>
#include <QApplication>
#include <QCloseEvent>
#include <QKeyEvent>
#include <QMap>
#include <QResizeEvent>
#include <QTimerEvent>
#include <QPainter>
#include <QPoint>
 

Urho3dWidget::Urho3dWidget(Urho3D::Context *context, QWidget *parent)
    : QWidget(parent),
      Urho3D::Object(context),
      engine_(nullptr),
      scene_(nullptr),
      cameraNode_(nullptr)
// here there is now scene_ and cameraNode_ ready for use.
// but for the simple demos, currently not needed yet
{ 

  // start timer to refresh engine at each frame
  timerId = startTimer(0);
  setFocusPolicy (Qt::StrongFocus);
}

Urho3dWidget::~Urho3dWidget() {}

void Urho3dWidget::Start() {
  using namespace Urho3D;
  createScene();
  auto *cache = GetSubsystem<ResourceCache>();

  // Construct new Text object
  SharedPtr<Text> helloText(new Text(context_));

  // Set String to display
  helloText->SetText("Hello World from Urho3D inside Qt5 Widgets !");

  // Set font and text color
  helloText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"), 20);
  helloText->SetColor(Color(0.0f, 1.0f, 0.0f));

  // Align Text center-screen
  helloText->SetHorizontalAlignment(HA_CENTER);
  helloText->SetVerticalAlignment(VA_CENTER);

  // Add Text instance to the UI root element
  GetSubsystem<UI>()->GetRoot()->AddChild(helloText);
  setupViewport();
  SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(Urho3dWidget, HandleUpdate));
}

void Urho3dWidget::Stop() {
  if (engine_.NotNull() && engine_->IsInitialized()) {
    engine_->Exit();
  }
}

void Urho3dWidget::Setup() {
  Urho3D::VariantMap engineParameters_;
  engineParameters_["FullScreen"] = false;
  engineParameters_["WindowWidth"] = 1280;
  engineParameters_["WindowHeight"] = 720;
  engineParameters_["WindowResizable"] = true;
#ifdef Q_OS_WIN32
  engineParameters_["ForceGL2"] = true;
#endif
  engineParameters_["ResourcePrefixPaths"] =
      ";D:/masteraplikasi/transferh11nov/urho3d/msvc/release/share/Resources";
  engineParameters_["ExternalWindow"] = (void *)winId();

  engine_ = new Urho3D::Engine(context_);
  engine_->Initialize(engineParameters_);
}

void Urho3dWidget::closeEvent(QCloseEvent *e) {
  e->accept();
  engine_->Exit();
}

void Urho3dWidget::timerEvent(QTimerEvent *e) {
  QWidget::timerEvent(e);
  // FIXME : Uncommenting following line sometimes crashes the ui in release
  // build
  // still don't know why. But, I believe this should be better removed.
  // qApp->processEvents();

  if (engine_.NotNull() && engine_->IsInitialized()) {
    engine_->RunFrame();
  }
}

void Urho3dWidget::resizeEvent(QResizeEvent *e) {
  if (engine_->IsInitialized()) {
    int width = e->size().width();
    int height = e->size().height();

    Urho3D::Graphics *graphics = GetSubsystem<Urho3D::Graphics>();

    SDL_Window *win = (SDL_Window *)graphics->GetWindow();
    SDL_SetWindowSize(win, width, height);
  }
}

void Urho3dWidget::keyPressEvent(QKeyEvent *e) { 
    
    if(cameraNode_.NotNull ()){
    if (e->key ()==Qt::Key_W)
      cameraNode_->Translate(Urho3D::Vector3::FORWARD * MOVE_SPEED * timeStep);
    if (e->key ()==Qt::Key_S)
      cameraNode_->Translate(Urho3D::Vector3::BACK * MOVE_SPEED * timeStep);
    if (e->key ()==Qt::Key_A)
      cameraNode_->Translate(Urho3D::Vector3::LEFT * MOVE_SPEED * timeStep);
    if (e->key ()==Qt::Key_D)
      cameraNode_->Translate(Urho3D::Vector3::RIGHT * MOVE_SPEED * timeStep);
    }
}
  
void Urho3dWidget::HandleUpdate(Urho3D::StringHash eventType,
                                Urho3D::VariantMap &eventData) {
  Q_UNUSED(eventType)
   timeStep = eventData["TimeStep"].GetFloat();

  if (cameraNode_.NotNull()) MoveCamera();
}

void Urho3dWidget::createScene() {
  auto *cache = GetSubsystem<Urho3D::ResourceCache>();
  scene_ = new Urho3D::Scene(context_);
  scene_->CreateComponent<Urho3D::Octree>();
  Urho3D::Node *planeNode = scene_->CreateChild("Plane");
  planeNode->SetScale(Urho3D::Vector3(100.0f, 1.0f, 100.0f));
  auto *planeObject = planeNode->CreateComponent<Urho3D::StaticModel>();
  planeObject->SetModel(cache->GetResource<Urho3D::Model>("Models/Plane.mdl"));
  planeObject->SetMaterial(
      cache->GetResource<Urho3D::Material>("Materials/StoneTiled.xml"));

  // Create a directional light to the world so that we can see something. The
  // light scene node's orientation controls the
  // light direction; we will use the SetDirection() function which calculates
  // the orientation from a forward direction vector.
  // The light will use default settings (white light, no shadows)
  Urho3D::Node *lightNode = scene_->CreateChild("DirectionalLight");
  lightNode->SetDirection(Urho3D::Vector3(
      0.6f, -1.0f,
      0.8f));  // The direction vector does not need to be normalized
  auto *light = lightNode->CreateComponent<Urho3D::Light>();
  light->SetLightType(Urho3D::LIGHT_DIRECTIONAL);

  cameraNode_ = scene_->CreateChild("Camera");
  cameraNode_->CreateComponent<Urho3D::Camera>();

  // Set an initial position for the camera scene node above the plane
  cameraNode_->SetPosition(Urho3D::Vector3(0.0f, 5.0f, 0.0f));
}

void Urho3dWidget::setupViewport() {
  auto *renderer = GetSubsystem<Urho3D::Renderer>();

  // Set up a viewport to the Renderer subsystem so that the 3D scene can be
  // seen. We need to define the scene and the camera
  // at minimum. Additionally we could configure the viewport screen size and
  // the rendering path (eg. forward / deferred) to
  // use, but now we just use full screen and default render path configured in
  // the engine command line options
  Urho3D::SharedPtr<Urho3D::Viewport> viewport(new Urho3D::Viewport(
      context_, scene_, cameraNode_->GetComponent<Urho3D::Camera>()));
  renderer->SetViewport(0, viewport);
}

void Urho3dWidget::MoveCamera() {
 // if (GetSubsystem<Urho3D::UI>()->GetFocusElement()) return;

  auto *input = GetSubsystem<Urho3D::Input>();

  // Mouse sensitivity as degrees per pixel
  const float MOUSE_SENSITIVITY = 1.2f;

  // Use this frame's mouse motion to adjust camera node yaw and pitch. Clamp
  // the pitch between -90 and 90 degrees
  Urho3D::IntVector2 mouseMove = input->GetMouseMove();
  yaw_ += MOUSE_SENSITIVITY * mouseMove.x_;
  pitch_ += MOUSE_SENSITIVITY * mouseMove.y_;
  pitch_ = Urho3D::Clamp(pitch_, -90.0f, 90.0f);

  // Construct new orientation for the camera scene node from yaw and pitch.
  // Roll is fixed to zero
  cameraNode_->SetRotation(Urho3D::Quaternion(pitch_, yaw_, 0.0f));
 
}
