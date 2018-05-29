#include "urho3dwidget.h"

#include <SDL/SDL.h>
#include <QMap>
#include <QResizeEvent>
#include <QTimerEvent>
#include <QKeyEvent>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Math/Quaternion.h>
#include <Urho3D/Math/Vector3.h>
#include <QApplication>
#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/UIElement.h>
#include <Urho3D/UI/Font.h>
#include <QCloseEvent>

//------------------------------------------------------------------------------------------------------

// key utilities to convert Qt key to SDL key
//------------------------------------------------------------------------------------------------------
static QMap<Qt::Key, SDL_Keycode> __keymap;
static void __initKeyMap();
static Uint16 __convertQtKeyModifierToSDL(Qt::KeyboardModifiers qtKeyModifiers);
static SDL_Keycode __convertQtKeyToSDL(Qt::Key qtKey);

//------------------------------------------------------------------------------------------------------
// map keys Qt/SDL
//------------------------------------------------------------------------------------------------------
void __initKeyMap()
{
__keymap[Qt::Key_unknown] = SDLK_UNKNOWN;
__keymap[Qt::Key_Escape] = SDLK_ESCAPE;
__keymap[Qt::Key_Tab] = SDLK_TAB;
__keymap[Qt::Key_Backspace] = SDLK_BACKSPACE;
__keymap[Qt::Key_Return] = SDLK_RETURN;
__keymap[Qt::Key_Enter] = SDLK_KP_ENTER;
__keymap[Qt::Key_Insert] = SDLK_INSERT;
__keymap[Qt::Key_Delete] = SDLK_DELETE;
__keymap[Qt::Key_Pause] = SDLK_PAUSE;
__keymap[Qt::Key_Print] = SDLK_PRINTSCREEN;
__keymap[Qt::Key_SysReq] = SDLK_SYSREQ;
__keymap[Qt::Key_Home] = SDLK_HOME;
__keymap[Qt::Key_Home] = SDLK_AC_HOME;
__keymap[Qt::Key_End] = SDLK_END;
__keymap[Qt::Key_Left] = SDLK_LEFT;
__keymap[Qt::Key_Right] = SDLK_RIGHT;
__keymap[Qt::Key_Up] = SDLK_UP;
__keymap[Qt::Key_Down] = SDLK_DOWN;
__keymap[Qt::Key_PageUp] = SDLK_PAGEUP;
__keymap[Qt::Key_PageDown] = SDLK_PAGEDOWN;
__keymap[Qt::Key_Shift] = SDLK_LSHIFT;
__keymap[Qt::Key_Control] = SDLK_LCTRL;
__keymap[Qt::Key_Alt] = SDLK_LALT;
__keymap[Qt::Key_CapsLock] = SDLK_CAPSLOCK;
__keymap[Qt::Key_NumLock] = SDLK_NUMLOCKCLEAR;
__keymap[Qt::Key_ScrollLock] = SDLK_SCROLLLOCK;
__keymap[Qt::Key_F1] = SDLK_F1;
__keymap[Qt::Key_F2] = SDLK_F2;
__keymap[Qt::Key_F3] = SDLK_F3;
__keymap[Qt::Key_F4] = SDLK_F4;
__keymap[Qt::Key_F5] = SDLK_F5;
__keymap[Qt::Key_F6] = SDLK_F6;
__keymap[Qt::Key_F7] = SDLK_F7;
__keymap[Qt::Key_F8] = SDLK_F8;
__keymap[Qt::Key_F9] = SDLK_F9;
__keymap[Qt::Key_F10] = SDLK_F10;
__keymap[Qt::Key_F11] = SDLK_F11;
__keymap[Qt::Key_F12] = SDLK_F12;
__keymap[Qt::Key_F13] = SDLK_F13;
__keymap[Qt::Key_F14] = SDLK_F14;
__keymap[Qt::Key_F15] = SDLK_F15;
__keymap[Qt::Key_Back]=SDLK_AC_BACK;
__keymap[Qt::Key_Menu] = SDLK_MENU;
__keymap[Qt::Key_Help] = SDLK_HELP;

// A-Z
for(int key='A'; key<='Z'; key++)
    __keymap[Qt::Key(key)] = key + 32;

// 0-9
for(int key='0'; key<='9'; key++)
    __keymap[Qt::Key(key)] = key;
}

//------------------------------------------------------------------------------------------------------
// get SDL key from Qt key
//------------------------------------------------------------------------------------------------------
SDL_Keycode __convertQtKeyToSDL(Qt::Key qtKey)
{
SDL_Keycode sldKey = __keymap.value(Qt::Key(qtKey));

if(sldKey == 0)
    printf("Warning: Key %d not mapped", qtKey);

return sldKey;
}

//------------------------------------------------------------------------------------------------------
// get SDL key modifier from Qt key modifier
//------------------------------------------------------------------------------------------------------
Uint16 __convertQtKeyModifierToSDL(Qt::KeyboardModifiers qtKeyModifiers)
{
Uint16 sdlModifiers = KMOD_NONE;

if(qtKeyModifiers.testFlag(Qt::ShiftModifier))
    sdlModifiers |= KMOD_LSHIFT | KMOD_RSHIFT;
if(qtKeyModifiers.testFlag(Qt::ControlModifier))
    sdlModifiers |= KMOD_LCTRL | KMOD_RCTRL;
if(qtKeyModifiers.testFlag(Qt::AltModifier))
    sdlModifiers |= KMOD_LALT | KMOD_RALT;

return sdlModifiers;
}


Urho3dWidget::Urho3dWidget(Urho3D::Context *context, QWidget *parent) : QWidget(parent),Urho3D::Object(context),
    engine_(nullptr),scene_(nullptr),cameraNode_(nullptr)
{
    __initKeyMap();
    
    // start timer to refresh engine at each frame
    timerId = startTimer(0);
}

Urho3dWidget::~Urho3dWidget()
{  
    
}

void Urho3dWidget::Start()
{
    using namespace Urho3D;
    auto* cache = GetSubsystem<ResourceCache>();
   
       // Construct new Text object
       SharedPtr<Text> helloText(new Text(context_));
   
       // Set String to display
       helloText->SetText("Hello World from Urho3D inside Qt5 Widgets !");
   
       // Set font and text color
       helloText->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"),20);
       helloText->SetColor(Color(0.0f, 1.0f, 0.0f));
   
       // Align Text center-screen 
       helloText->SetHorizontalAlignment(HA_CENTER);
       helloText->SetVerticalAlignment(VA_CENTER);
   
       // Add Text instance to the UI root element
     GetSubsystem<UI>()->GetRoot()->AddChild(helloText);
   SubscribeToEvent(E_UPDATE, URHO3D_HANDLER(Urho3dWidget, HandleUpdate));
}

void Urho3dWidget::Stop()
{
    if(engine_.NotNull () && engine_->IsInitialized ()){
        engine_->Exit ();
    }
}

void Urho3dWidget::Setup()
{
    Urho3D::VariantMap engineParameters_;
    engineParameters_["FullScreen"]=false;
    engineParameters_["WindowWidth"]=1280;
    engineParameters_["WindowHeight"]=720;
    engineParameters_["WindowResizable"]=true;
    engineParameters_["ResourcePrefixPaths"]=";D:/masteraplikasi/transferh11nov/urho3d/msvc/release/share/Resources";
    engineParameters_["ExternalWindow"]=(void*)winId();
    
    engine_ = new Urho3D::Engine(context_);
    engine_->Initialize(engineParameters_);
}

void Urho3dWidget::closeEvent(QCloseEvent *e)
{
     e->accept ();
     engine_->Exit ();
}

void Urho3dWidget::timerEvent(QTimerEvent *e)
{
    QWidget::timerEvent(e);
   // qApp->processEvents();
    
    if(engine_.NotNull () && engine_->IsInitialized ()){
        engine_->RunFrame ();
    }
}

void Urho3dWidget::resizeEvent(QResizeEvent *e)
{
    if(engine_->IsInitialized())
    {
        int width = e->size().width();
        int height = e->size().height();
    
        Urho3D::Graphics* graphics = GetSubsystem<Urho3D::Graphics>();
    
        SDL_Window * win = (SDL_Window*)graphics->GetWindow();
        SDL_SetWindowSize(win, width, height);
    }
}

void Urho3dWidget::keyPressEvent(QKeyEvent *e)
{
    // Transmit key press event to SDL
    SDL_Event sdlEvent;
    sdlEvent.type = SDL_KEYDOWN;
    sdlEvent.key.keysym.sym = __convertQtKeyToSDL( Qt::Key(e->key()) );
    sdlEvent.key.keysym.mod = __convertQtKeyModifierToSDL(e->modifiers());
    SDL_PushEvent(&sdlEvent);
}

void Urho3dWidget::HandleUpdate(Urho3D::StringHash eventType, Urho3D::VariantMap &eventData)
{
    Q_UNUSED(eventType)
    float timeStep = eventData["TimeStep"].GetFloat();
    
   if(cameraNode_.NotNull ()) cameraNode_->Rotate (Urho3D::Quaternion(30.0f * timeStep, Urho3D::Vector3(0, 1, 0)));
}
