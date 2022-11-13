#include "GolfDashPCH.h"
#include "GolfDash.h"

#ifdef GD_OS_WINDOWS
#include <Windows.h>
#endif

namespace gd {

	void Main()
	{
		GolfDash* instance = new GolfDash();
		instance->Run();
		delete instance;
	}

}

#ifdef GD_OS_WINDOWS

#ifdef GD_DIST
INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
{
	gd::Main();
	return 0;
}
#else
int main(int args, char* argv[])
{
	gd::Main();
	return 0;
}
#endif

#endif