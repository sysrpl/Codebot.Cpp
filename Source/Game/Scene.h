#include <Codebot/Packages/System.h>
#include <Codebot/Packages/Graphics.h>
#include <Codebot/Packages/Input.h>
#include <Codebot/Game/GameExceptions.h>

//  Choose a framework using the options below

#define SdlScene

#ifdef SdlScene
#include <Codebot/Packages/OpenGL.h>
#endif

namespace Codebot
{
namespace Game
{

// SceneOptions struct

struct SceneOptions
{
	SceneOptions();
	void Save(const String& fileName);
	void Load(const String& fileName);
	Cardinal Samples;
	Cardinal DefaultWidth;
	Cardinal DefaultHeight;
	Cardinal FullscreenWidth;
	Cardinal FullscreenHeight;
	String Caption;
	Boolean Fullscreen;
	Boolean Resizable;
	Boolean ShowCursor;
};

struct SceneState : public SceneOptions
{
	SceneState();
	Integer Width;
	Integer Height;
	Boolean Terminated;
	Pointer Session;
	SceneState& operator = (const SceneOptions& value);
};

// Scene class

class Scene : public Object
{
private:
	Boolean initialized;
protected:
	SceneState state;
	void Initialize(SceneOptions& options);
	virtual void Update(const Time::Timer& timer);
	virtual void Render(const Time::Timer& timer);
	void SwapBuffers();
	// Event handling
	virtual void OnInit(EventArgs* e);
	virtual void OnKeyDown(Input::KeyboardEventArgs* e);
	virtual void OnKeyUp(Input::KeyboardEventArgs* e);
	virtual void OnMouseDown(Input::MouseEventArgs* e);
	virtual void OnMouseMove(Input::MouseEventArgs* e);
	virtual void OnMouseUp(Input::MouseEventArgs* e);
	virtual void OnQuit(EventArgs* e);
	virtual void OnResize(EventArgs* e);
public:
	static Scene* Current;
	Scene();
	Graphics::Image CaptureImage();
	void Quit();
	virtual void Run(SceneOptions& options);
	// Properties
	String Caption();
	void Caption(const String& value);
	Boolean Fullscreen();
	void Fullscreen(Boolean value);
	Cardinal Width();
	Cardinal Height();
	Boolean Terminated();
	void Terminated(Boolean value);
};

template <typename T>
int SceneRun(SceneOptions& options)
{
	int error = 1;
	auto scene = New<T>();
	try
	{
		scene->Run(options);
	}
	catch (Exception& e)
	{
		WriteLine(e.ToString());
	}
	catch (std::exception& e)
	{
		WriteLine("std::exception {0}", String(e.what()));
	}
	catch (...)
	{
		WriteLine("Unknown exception");
	}
	scene->Quit();
	return error;
}

template <typename T>
int SceneRun()
{
	SceneOptions options;
	return SceneRun<T>(options);
}

}
}
