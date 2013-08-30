nosj-cpp
========

This is just another C++ JSON library. It works with UTF-8 encoded strings and
hopes to be fully compliant with the [JSON RFC](http://tools.ietf.org/html/rfc4627).
(If you find some non-compliance, please let me know!)


Usage
-----
1. Copy the `nosj/` directory to your project or configure your compiler to look
for headers in the directory where `nosj/` is.
2. Include the headers in your source code as required:

``` C++
#include "nosj/values.hpp"     // JSON values
#include "nosj/stringify.hpp"  // Functions for generating JSON strings from JSON values
#include "nosj/parse.hpp"      // Functions for parsing JSON strings into JSON values
```

All you need is defined in the `nosj` namespace of the header files. You don't
need to link your project to any object file.

The `nosj::Value` class is the wrapper for any JSON value. The wrapped value types
are:
 * `nosj::Null` - A dummy class representing the JSON `null` value.
 * `nosj::Boolean` - Represents the JSON boolean values `false` and `true`. It is
   an alias for the C++ `bool` type.
 * `nosj::Number` - A class that represents JSON number values. It may store the
   number value as C++ types `long long` or `long double`, and converts as needed.
 * `nosj::String` - Represents JSON strings encoded in UTF-8. It is an alias for
   the C++ `std::string` type.
 * `nosj::Array` - Represents JSON arrays. It is an alias for the C++
   `std::deque<Value>` type.
 * `nosj::Object` - Represents JSON objects. It is an alias for the C++11
   `std::unordered_map<String, Value>` type.

Check the files `nosj/*.hpp` for the available methods and the `nosj-test-*.cpp`
files for examples of usage.


-------
Tested with GCC 4.7.3 (or earlier) on Linux and Cygwin.
