#pragma once

#include <boost/format.hpp>
#include <cassert>
#include <cstdio>

namespace Logger
{
	class Handle
	{
	private:
		friend Handle RequestHandle(const char* tag);

		const char* tag;

		inline Handle(const char* _tag)
		{
			assert(_tag);
			tag = _tag;
		}

	public:
#ifdef NDEBUG
		inline void Debug(const char*) const {}
		inline void Debug(const boost::format&) const {}
#else
		void Debug(const char* message) const;
		inline void Debug(const boost::format& message) const { return Debug(message.str().c_str()); }
#endif

		void Fatal(const char* message) const;
		inline void Fatal(const boost::format& message) const { return Fatal(message.str().c_str()); }

		inline Handle(const Handle& other) { tag = other.tag; }
		inline ~Handle() {}
	};

	// Requests a LogHandle which is required to make logging calls. Each time
	// something is logged from this handle, the "tag" will be prepended to the
	// text.
	inline Handle RequestHandle(const char* tag) { return Handle(tag); }
}