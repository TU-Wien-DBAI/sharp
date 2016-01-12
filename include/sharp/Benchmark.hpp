#ifndef SHARP_SHARP_BENCHMARK_H_
#define SHARP_SHARP_BENCHMARK_H_

#include <sharp/global>

#include <iostream>
#include <string>

namespace sharp
{
	class SHARP_API Benchmark
	{
	public:
		static void registerTimestamp(const std::string &name);
		static void printBenchmarks(std::ostream &out, bool csv);

	private:
		Benchmark();
	};

} // namespace sharp

#endif // SHARP_SHARP_BENCHMARK_H_
