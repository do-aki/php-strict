
#ifndef PHP_STRICT_H
#define PHP_STRICT_H

extern zend_module_entry strict_module_entry;
#define phpext_strict_ptr &strict_module_entry

#ifdef PHP_WIN32
#	define PHP_STRICT_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_STRICT_API __attribute__ ((visibility("default")))
#else
#	define PHP_STRICT_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINIT_FUNCTION(strict);
PHP_MSHUTDOWN_FUNCTION(strict);
PHP_RINIT_FUNCTION(strict);
PHP_RSHUTDOWN_FUNCTION(strict);
PHP_MINFO_FUNCTION(strict);

#define NO_STRICT "@no strict"

ZEND_BEGIN_MODULE_GLOBALS(strict)
	long use;
	long dump;
	long verbose;
	long execute;
	char *filename;
	int  detect;
ZEND_END_MODULE_GLOBALS(strict)

ZEND_EXTERN_MODULE_GLOBALS(strict)

#ifdef ZTS
#define STRICT_G(v) TSRMG(strict_globals_id, zend_strict_globals *, v)
#else
#define STRICT_G(v) (strict_globals.v)
#endif

#if PHP_VERSION_ID >= 50399
# define STRICT_ZNODE znode_op
# define STRICT_ZNODE_ELEM(node,var) node.var
# define STRICT_TYPE(t) t##_type
# define STRICT_EXTENDED_VALUE(o) extended_value
#else
# define STRICT_ZNODE znode
# define STRICT_ZNODE_ELEM(node,var) node.u.var
# define STRICT_TYPE(t) t.op_type
# define STRICT_EXTENDED_VALUE(o) o.u.EA.type
#endif

void strict_verbose(const char* format, ...);


#endif	/* PHP_STRICT_H */

