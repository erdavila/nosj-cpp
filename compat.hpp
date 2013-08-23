#ifndef COMPAT_HPP_
#define COMPAT_HPP_


#ifdef __CYGWIN__

# include <string>
# include <sstream>

namespace /*unnamed*/ {

std::string to_string(size_t n) {
	std::ostringstream ostr;
	ostr << n;
	return ostr.str();
}

void setMemoryLimit(unsigned long) {}

}

#else

# include <cassert>
# include <sys/resource.h>
# include <sys/time.h>

namespace /*unnamed*/ {

void setMemoryLimit(unsigned long mem) {
	struct rlimit rlim;
	rlim.rlim_cur = rlim.rlim_max = mem;
	assert(setrlimit(RLIMIT_AS, &rlim) == 0);
}

}

#endif


#endif /* COMPAT_HPP_ */
