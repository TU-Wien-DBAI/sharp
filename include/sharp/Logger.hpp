#ifndef SHARP_SHARP_LOGGER_H_
#define SHARP_SHARP_LOGGER_H_

#include <sharp/global>

#include <iostream>
#include <string>

namespace sharp
{
	class SHARP_API Logger
	{
	public:
		static void init(std::ostream &outputStream);

		static void info(const std::string &message);
		static void error(const std::string &message);

	private:
		Logger();
	};

} // namespace sharp

#endif // SHARP_SHARP_LOGGER_H_
