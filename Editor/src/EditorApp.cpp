#include <Injin.h>

class EditorApp : public engin::Application
{
public:
	EditorApp() {

	}
	~EditorApp() {

	}


};

int main()
{

	EditorApp* app = new EditorApp();
	app->runApp();
	return 0;
}