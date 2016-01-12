#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include <sharp/Logger.hpp>

namespace sharp
{
	using std::string;
	using std::endl;
	using std::ostream;

	namespace
	{
		ostream *out = &std::cout;

	} // namespace

	void Logger::init(ostream &outputStream)
	{
		out = &outputStream;
	}

	void Logger::info(const string &msg)
	{
		*out << msg << endl;
	}

	void Logger::error(const string &msg)
	{
		*out << "ERROR: " << msg << endl;
	}

} // namespace sharp
