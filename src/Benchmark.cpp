#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include <sharp/Benchmark.hpp>

#include <deque>

//FIXME: use config.h info here, build check into autoconf
#include <sys/times.h>

#ifdef HAVE_UNISTD_H //FIXME: what if not def'd?
	#include <unistd.h>
#endif

namespace sharp
{
	using std::deque;
	using std::string;
	using std::endl;
	using std::ios;

	namespace
	{
		deque<clock_t> wallClock_;
		deque<struct tms> cpuClock_;
		deque<string> names_;

	} // namespace

	void Benchmark::registerTimestamp(const std::string &name)
	{
		struct tms cpu;
		clock_t wall = times(&cpu);

		wallClock_.push_front(wall);
		cpuClock_.push_front(cpu);
		names_.push_front(name);
	}

	void Benchmark::printBenchmarks(std::ostream &out)
	{
		if(names_.empty()) return;

		long tickpersec = 0;
		if((tickpersec = sysconf(_SC_CLK_TCK)) < 0) return;

		out.setf(ios::fixed, ios::floatfield);
		out.precision(2);
		
		clock_t lastWall = wallClock_.back();
		struct tms lastCpu = cpuClock_.back();

		out << "0.00s (usr),\t0.00s (sys),\t0.00s (wall) - "
			<< names_.back() << endl;

		names_.pop_back();
		wallClock_.pop_back();
		cpuClock_.pop_back();

		while(!names_.empty())
		{
			clock_t wall = wallClock_.back();
			struct tms cpu = cpuClock_.back();

			out << ((cpu.tms_utime - lastCpu.tms_utime) / (double)tickpersec)
				<< "s (usr),\t"
				<< ((cpu.tms_stime - lastCpu.tms_stime) / (double)tickpersec)
				<< "s (sys),\t"
				<< ((wall - lastWall) / (double)tickpersec)
				<< "s (wall) - "
				<< names_.back() << endl;

			lastWall = wall;
			lastCpu = cpu;

			names_.pop_back();
			wallClock_.pop_back();
			cpuClock_.pop_back();
		}
	}

} // namespace sharp
