#include "readline.h"

// Array with the names of of the Tcl Commands
char *commands[] = {
	// Vanilla Tcl Commands (Tcl 8.6) //
	// "after", "append", "apply", "argc", "argv()", "array", "auto_execok", "auto_import", "auto_load",
	// "auto_mkindex", "auto_path", "auto_quality", "auto_reset", "bgerror", "binary", "break", "catch",
	// "cd", "chan", "clock", "close", "concat", "continue", "dde", "dict", "encoding", "env", "eof",
	// "error", "errorCode", "errorInfo", "eval", "exec", "exit", "expr", "fblocked", "fconfigure",
	// "fcopy", "file", "file", "fileevent", "filename", "flush", "for", "foreach", "format", "gets",
	// "glob", "global", "history", "http", "if", "incr", "info", "interp", "join", "lappend", "lassign",
	// "lindex", "linsert", "list", "llength", "lmap", "load", "lrange", "lrepeat", "lreplace",
	// "lreverse", "lsearch", "lset", "lsort", "mathfunc", "mathop", "memory", "msgcat", "my", "namespace",
	// "next", "nextto", "oo::class", "oo::copy", "oo::define", "oo:objdefine", "oo:object", "open",
	// "package", "parray", "pid", "pkg::create", "pkg_mkIndex", "platform", "platform::shell", "proc",
	// "puts", "pwd", "re_syntax", "read", "refchan", "regexp", "regsub", "rename", "return", "safe",
	// "scan", "seek", "self", "set", "socket", "source", "split", "string", "subst", "switch", "tailcall",
	// "Tcl", "tcl::prefix", "tcl_endOfWord", "tcl_findLibrary", "tcl_library", "tcl_nonwordchars",
	// "tcl_patchLevel", "tcl_pkgPath", "tcl_platform", "tcl_precision", "tcl_rcFileName",
	// "tcl_startOfNextWord", "tcl_startOfPreviousWord", "tcl_traceCompile", "tcl_traceExec", "tcl_version",
	// "tcl_wordBreakAfter", "tcl_wordBreakBefore", "tcl_wordchars", "tcltest", "tell", "throw", "time",
	// "tm", "trace", "transchan", "try", "unknown", "unload", "unset", "update", "uplevel", "upvar",
	// "variable", "vwait", "while", "yield", "yieldto", "zlib",
	"history",
	"start_gui", "read_points", 

	NULL
};

void init_readline()
{
	rl_completion_entry_function = NULL;
	
	rl_attempted_completion_function = custom_completion;
}

char **custom_completion(const char *text, int start, int end)
{
	char **matches = NULL;
	rl_completion_append_character = '\0';
	//  If the wold is at the start of the line, then the custom completer is //
	// used. Otherwise the default file completer of Readline is called.      //
	if (start == 0)
		return rl_completion_matches(text, custom_completer);

	// If matches is NULL, then the default completer of Readline is called.  //
	return matches;
}

char *custom_completer(const char * text, int state)
{
	//  The index is defined as static because we need to continue the search //
	// on the list from where we stopped.                                     //
	static int list_index;
	// The length of the input text.                                          //
	static int length;
	// 
	char *name;

	if (state == 0)
	{
		list_index = 0;
		length = strlen(text);
	}

	// Search through the commands until you reach the last value NULL.       //
	while ((name = commands[list_index]))
	{
		list_index++;

		// If there is a match, return an allocated string with the match.    //
		if (strncmp(name, text, length) == 0)
		{
			return strdup(name);
		}
	}

	// When all the commands are checked, return NULL.                        //
	return NULL;
}