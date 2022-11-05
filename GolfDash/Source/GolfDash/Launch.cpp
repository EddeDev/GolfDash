#include "GolfDashPCH.h"
#include "GolfDash.h"

namespace gd {

	void Main()
	{
		GolfDash* instance = new GolfDash();
		instance->Run();
		delete instance;
	}

}

#ifdef GD_OS_WINDOWS
int main(int args, char* argv[])
{
	gd::Main();
	return 0;
}
#endif