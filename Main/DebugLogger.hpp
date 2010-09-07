#pragma once

// TODO: rather than having an Indent struct,
// have Log functions that not only log, but
// change indentation
// (e.g. ScopeIncLog and ScopeDecLog)
namespace DebugLogger
{
	struct Indent
	{
		Indent();
		~Indent();
	};

	void Log(const char* format, ...);
}
