#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include <sharp/global>

extern "C"
{
	SHARP_API const char *sharp_version();
}

const char *sharp_version()
{
	return VERSION;
}
