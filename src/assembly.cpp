#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include <sharp/global>

extern "C"
{
	SHARP_API const char *sharp_version()
	{
		return VERSION;
	}

	SHARP_API const char *sharp_version_2()
	{
		return VERSION;
	}

	SHARP_API const char *sharp_version_2_0()
	{
		return VERSION;
	}
}

