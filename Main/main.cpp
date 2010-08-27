#include <boost/thread.hpp>
#include <boost/date_time.hpp>

using namespace boost;

static void MakeWindow();

int main()
{
	MakeWindow();
	for(;;)
		this_thread::sleep(posix_time::millisec(50));

	return 0;
}

void MakeWindow()
{

}
