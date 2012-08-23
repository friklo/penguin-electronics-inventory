#include "MainWindow.h"
#include <string>

using std::string;

#ifdef _WIN32
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#else
int main(int argc, char* argv[])
#endif
{
	Gtk::Main kit(argc, argv);
	
 	MainWindow wnd;
	kit.run(wnd);

	return 0;
}
