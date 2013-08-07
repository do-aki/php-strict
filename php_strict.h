
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

PHP_FUNCTION(confirm_strict_compiled);	/* For testing, remove later. */

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(strict)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(strict)
*/

/* In every utility function you add that needs to use variables 
   in php_strict_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as STRICT_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define STRICT_G(v) TSRMG(strict_globals_id, zend_strict_globals *, v)
#else
#define STRICT_G(v) (strict_globals.v)
#endif

#endif	/* PHP_STRICT_H */

