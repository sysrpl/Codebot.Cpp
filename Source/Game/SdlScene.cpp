#include <Codebot/Game/Scene.h>
#ifdef SdlScene

#include <SDL/SDL.h>

namespace Codebot
{
namespace Game
{

using namespace Codebot::Graphics;
using namespace Codebot::Graphics::OpenGL;
using namespace Codebot::Input;
using namespace Codebot::Math;
using namespace Codebot::Time;

// SceneDefaults struct

SceneOptions::SceneOptions() :
	Samples(0),
	DefaultWidth(640),
	DefaultHeight(480),
	FullscreenWidth(0),
	FullscreenHeight(0),
	Caption("New Game"),
	Fullscreen(false),
	Resizable(false),
	ShowCursor(true)
{
}

// SceneState struct

SceneState::SceneState() :
	Width(0),
	Height(0),
	Terminated(false),
	Session(null)
{
}

SceneState& SceneState::operator = (const SceneOptions& value)
{
	Samples = value.Samples;
	DefaultWidth = value.DefaultWidth;
	DefaultHeight = value.DefaultHeight;
	FullscreenWidth = 0;
	FullscreenHeight = 0;
	Caption = value.Caption;
	Fullscreen = value.Fullscreen;
	Resizable = value.Resizable;
	ShowCursor = value.ShowCursor;
	return *this;
}

// Scene class

Scene* Scene::Current = null;

Scene::Scene() :
	initialized(false),
	state()
{
	Current = this;
}

void CenterWindow()
{
	static const String Centered = "SDL_VIDEO_CENTERED=1";
	putenv((PChar)&Centered[0]);
}

void UncenterWindow()
{
	static const String Uncentered = "SDL_VIDEO_CENTERED";
	putenv((PChar)&Uncentered[0]);
}

void Scene::Initialize(SceneOptions& options)
{
	if (initialized)
		return;
	state = options;
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		ThrowVideoInitializeException(ThisMethod);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	if (state.Samples >  0)
	{
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, state.Samples);
	}
	else
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
	if (state.FullscreenHeight == 0 || state.FullscreenWidth == 0)
	{
		auto info = SDL_GetVideoInfo();
		state.FullscreenWidth = info->current_w;
		state.FullscreenHeight = info->current_h;
	}
	LongWord flags = SDL_OPENGL;
	if (state.Fullscreen)
	{
		state.Width = state.FullscreenWidth;
		state.Height = state.FullscreenHeight;
		flags |= SDL_FULLSCREEN;
	}
	else
	{
		state.Width = state.DefaultWidth;
		state.Height = state.DefaultHeight;
	}
	CenterWindow();
	SDL_Surface* surface = SDL_SetVideoMode(state.Width, state.Height, 0, flags);
	UncenterWindow();
	if (surface == null)
		ThrowVideoInitializeException(ThisMethod);
	state.Session = surface;
	SDL_ShowCursor(state.ShowCursor ? 1 : 0);
	SDL_WM_SetCaption(state.Caption, null);
	initialized = true;
}

void Scene::Update(const Time::Timer& timer)
{
}

void Scene::Render(const Time::Timer& timer)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	SwapBuffers();
}

void Scene::SwapBuffers()
{
	SDL_GL_SwapBuffers();
}

Image Scene::CaptureImage()
{
	if (not initialized)
		ThrowVideoInitializeException(ThisMethod);
	Image image;
	image.Generate(state.Width, state.Height);
	glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, state.Width, state.Height, GL_BGRA, GL_UNSIGNED_BYTE, image.Pixels());
	return image;
}

void Scene::Quit()
{
	if (initialized)
	{
		OnQuit(EventArgs::Empty);
		Current = null;
		initialized = false;
		state.Terminated = true;
		SDL_Quit();
	}
}

void Scene::OnInit(EventArgs* e)
{
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glColor4f(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Scene::OnKeyDown(KeyboardEventArgs* e)
{
	if (e->Code == KeyCode::Escape)
		state.Terminated = true;
	else if (e->Code == KeyCode::F1)
		Fullscreen(!state.Fullscreen);
	else if (e->Code == KeyCode::F2)
	{
		static const String screenshot = "screenshot.png";
		Image image = CaptureImage();
		image.Save(screenshot);
	}
}

void Scene::OnKeyUp(KeyboardEventArgs* e)
{

}

void Scene::OnMouseDown(MouseEventArgs* e)
{

}

void Scene::OnMouseMove(MouseEventArgs* e)
{

}

void Scene::OnMouseUp(MouseEventArgs* e)
{

}

void Scene::OnQuit(EventArgs* e)
{

}

void Scene::OnResize(EventArgs* e)
{

}

KeyModifiers TranslateModifier(Cardinal mod)
{
	KeyModifiers m;
	if ((mod & KMOD_ALT) > 0)
		m.Include(KeyModifier::Alt);
	if ((mod & KMOD_CTRL) > 0)
		m.Include(KeyModifier::Control);
	if ((mod & KMOD_SHIFT) > 0)
		m.Include(KeyModifier::Shift);
	return m;
}


MouseButtons TranslateButton(Cardinal button)
{
	MouseButtons b;
	if ((button & SDL_BUTTON_LEFT) > 0)
		b.Include(MouseButton::Left);
	if ((button & SDL_BUTTON_MIDDLE) > 0)
		b.Include(MouseButton::Middle);
	if ((button & SDL_BUTTON_RIGHT) > 0)
		b.Include(MouseButton::Right);
	if ((button & SDL_BUTTON_WHEELUP) > 0)
		b.Include(MouseButton::WheelUp);
	if ((button & SDL_BUTTON_WHEELDOWN) > 0)
		b.Include(MouseButton::WheelDown);
	return b;
}

KeyCode TranslateKey(Cardinal key)
{
	if ((key >= '0') && (key <= '9'))
		return (KeyCode)key;
	if ((key >= 'A') && (key <= 'Z'))
		return (KeyCode)key;
	if ((key >= SDLK_a) && (key <= SDLK_x))
		return (KeyCode)(key - 0x20);
	switch (key)
	{
		case SDLK_BACKSPACE: return KeyCode::Backspace;
		case SDLK_TAB: return KeyCode::Tab;
		case SDLK_ESCAPE: return KeyCode::Escape;
		case SDLK_SPACE: return KeyCode::Space;
		case SDLK_UP: return KeyCode::Up;
		case SDLK_DOWN: return KeyCode::Down;
		case SDLK_RIGHT: return KeyCode::Right;
		case SDLK_LEFT: return KeyCode::Left;
		case SDLK_INSERT: return KeyCode::Insert;
		case SDLK_HOME: return KeyCode::Home;
		case SDLK_END: return KeyCode::End;
		case SDLK_PAGEUP: return KeyCode::Prior;
		case SDLK_PAGEDOWN: return KeyCode::Next;
		case SDLK_F1: return KeyCode::F1;
		case SDLK_F2: return KeyCode::F2;
		case SDLK_F3: return KeyCode::F3;
		case SDLK_F4: return KeyCode::F4;
		case SDLK_F5: return KeyCode::F5;
		case SDLK_F6: return KeyCode::F6;
		case SDLK_F7: return KeyCode::F7;
		case SDLK_F8: return KeyCode::F8;
		case SDLK_F9: return KeyCode::F9;
		case SDLK_F10: return KeyCode::F10;
		case SDLK_F11: return KeyCode::F11;
		case SDLK_F12: return KeyCode::F12;
		case SDLK_DELETE: return KeyCode::Delete;
		case SDLK_NUMLOCK: return KeyCode::NumLock;
		case SDLK_CAPSLOCK: return KeyCode::CapsLock;
		case SDLK_SCROLLOCK: return KeyCode::ScrollLock;
		case SDLK_RSHIFT: return KeyCode::RShift;
		case SDLK_LSHIFT: return KeyCode::LShift;
		case SDLK_RCTRL: return KeyCode::RControl;
		case SDLK_LCTRL: return KeyCode::LControl;
		case SDLK_RALT: return KeyCode::RAlt;
		case SDLK_LALT: return KeyCode::LAlt;
		default: return KeyCode::Sleep;
	}
}

void TranslateKeyboardEvent(const SDL_Event& event, KeyboardEventArgs& e)
{
	e.Modifiers = TranslateModifier(event.key.keysym.mod);
	e.Code = TranslateKey(event.key.keysym.sym);
}

void TranslateMouseButtonEvent(const SDL_Event& event, MouseEventArgs& e)
{
	e.Buttons = TranslateButton(event.button.button);
	e.Modifiers = TranslateModifier(0);
	e.X = event.button.x;
	e.Y = event.button.y;
	e.DeltaX = 0;
	e.DeltaY = 0;
}

void TranslateMouseMoveEvent(const SDL_Event& event, MouseEventArgs& e)
{
	e.Buttons = TranslateButton(event.motion.state);
	e.Modifiers = TranslateModifier(0);
	e.X = event.motion.x;
	e.Y = event.motion.y;
	e.DeltaX = event.motion.xrel;
	e.DeltaY = event.motion.yrel;
}

void Scene::Run(SceneOptions& options)
{
	Initialize(options);
	state.Terminated = false;
	OnInit(EventArgs::Empty);
	OnResize(EventArgs::Empty);
	Timer timer;
	SDL_Event event;
	KeyboardEventArgs keyboardEventArgs;
	MouseEventArgs mouseEventArgs;
	while (!state.Terminated)
	{
		timer.Update();
		Update(timer);
		Render(timer);
		while (SDL_PollEvent(&event))
			switch (event.type)
			{
				case SDL_QUIT:
					state.Terminated = true;
					break;
				case SDL_KEYDOWN:
					TranslateKeyboardEvent(event, keyboardEventArgs);
					OnKeyDown(&keyboardEventArgs);
					break;
				case SDL_KEYUP:
					TranslateKeyboardEvent(event, keyboardEventArgs);
					OnKeyUp(&keyboardEventArgs);
					break;
				case SDL_MOUSEMOTION:
					TranslateMouseMoveEvent(event, mouseEventArgs);
					OnMouseMove(&mouseEventArgs);
					break;
				case SDL_MOUSEBUTTONDOWN:
					TranslateMouseButtonEvent(event, mouseEventArgs);
					OnMouseDown(&mouseEventArgs);
					break;
				case SDL_MOUSEBUTTONUP:
					TranslateMouseButtonEvent(event, mouseEventArgs);
					OnMouseUp(&mouseEventArgs);
					break;
			}
	}
}

// Scene::Properties

String Scene::Caption()
{
	return state.Caption;
}

void Scene::Caption(const String& value)
{
	state.Caption = value;
	if (initialized)
		SDL_WM_SetCaption(state.Caption, null);
}

Boolean Scene::Fullscreen()
{
    return state.Fullscreen;
}

void Scene::Fullscreen(Boolean value)
{
	// SDL does not allow for changing wndow size without destroying
	// the OpenGL context
}

Cardinal Scene::Width()
{
	return state.Width;
}
Cardinal Scene::Height()
{
	return state.Height;
}

Boolean Scene::Terminated()
{
	return state.Terminated;
}

void Scene::Terminated(Boolean value)
{
	if (value)
		state.Terminated = value;
}

}
}
#endif
