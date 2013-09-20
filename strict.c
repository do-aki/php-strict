
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_strict.h"
#include "strict_op.h"
#include "source_file.h"

static zend_op_array* (*original_compile_file)(zend_file_handle* file_handle, int type TSRMLS_DC);
static zend_op_array* strict_compile_file(zend_file_handle*, int TSRMLS_DC);

static zend_op_array* (*original_compile_string)(zval *source_string, char *filename TSRMLS_DC);
static zend_op_array* strict_compile_string(zval *source_string, char *filename TSRMLS_DC);


#if PHP_VERSION_ID >= 50500
static void (*original_execute)(zend_execute_data *arg TSRMLS_DC);
static void strict_execute(zend_execute_data *arg TSRMLS_DC);
#else
static void (*original_execute)(zend_op_array *op_array TSRMLS_DC);
static void strict_execute(zend_op_array *arg TSRMLS_DC);
#endif
ZEND_DECLARE_MODULE_GLOBALS(strict)

/* True global resources - no need for thread safety here */
#ifdef ZTS
static int le_strict;
#endif

/* {{{ strict_functions[]
 *
 * Every user visible function must have an entry in strict_functions[].
 */
const zend_function_entry strict_functions[] = {
	PHP_FE_END	/* Must be the last line in strict_functions[] */
};
/* }}} */

/* {{{ strict_module_entry
 */
zend_module_entry strict_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"strict",
	strict_functions,
	PHP_MINIT(strict),
	PHP_MSHUTDOWN(strict),
	PHP_RINIT(strict),
	PHP_RSHUTDOWN(strict),
	PHP_MINFO(strict),
#if ZEND_MODULE_API_NO >= 20010901
	"0.0.1",
#endif
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_STRICT
ZEND_GET_MODULE(strict)
#endif

/* {{{ PHP_INI
 */
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("strict.use", "1", PHP_INI_ALL, OnUpdateBool, use, zend_strict_globals, strict_globals)
    STD_PHP_INI_ENTRY("strict.dump", "0", PHP_INI_ALL, OnUpdateBool, dump, zend_strict_globals, strict_globals)
    STD_PHP_INI_ENTRY("strict.verbose", "0", PHP_INI_ALL, OnUpdateBool, verbose, zend_strict_globals, strict_globals)
    STD_PHP_INI_ENTRY("strict.execute", "1", PHP_INI_ALL, OnUpdateBool, execute, zend_strict_globals, strict_globals)
PHP_INI_END()
/* }}} */

/* {{{ php_strict_init_globals
 */
static void php_strict_init_globals(zend_strict_globals *strict_globals)
{
	strict_globals->use = 1;
	strict_globals->dump = 0;
	strict_globals->verbose = 0;
	strict_globals->execute = 1;

	strict_globals->filename = NULL;
}
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(strict)
{
	ZEND_INIT_MODULE_GLOBALS(strict, php_strict_init_globals, NULL);
	REGISTER_INI_ENTRIES();
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(strict)
{
	UNREGISTER_INI_ENTRIES();
    return SUCCESS;
}
/* }}} */

/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(strict)
{
	original_compile_string = zend_compile_string;
	zend_compile_string = strict_compile_string;

	original_compile_file = zend_compile_file;
	zend_compile_file = strict_compile_file;

#if PHP_VERSION_ID >= 50500
	original_execute = zend_execute_ex;
	zend_execute_ex = strict_execute;
#else
	original_execute = zend_execute;
	zend_execute = strict_execute;
#endif

    return SUCCESS;
}
/* }}} */

/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(strict)
{
	zend_compile_string = original_compile_string;
	zend_compile_file = original_compile_file;
#if PHP_VERSION_ID >= 50500
	zend_execute_ex = original_execute;
#else
	zend_execute = original_execute;
#endif
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(strict)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "strict support", "enabled");
	php_info_print_table_end();

	DISPLAY_INI_ENTRIES();
}
/* }}} */

void strict_verbose(const char* format, ...) {

	if (STRICT_G(verbose)) {
		va_list args;
		char *buffer;
		int size;
		TSRMLS_FETCH();

		va_start(args, format);
		size = vspprintf(&buffer, 0, format, args);
		PHPWRITE(buffer, size);
		efree(buffer);
		va_end(args);
	}
}

/* {{{ int strict_scan_function
 */
static int strict_scan_function(zend_op_array* fopa TSRMLS_DC)
{
	if (fopa->type == ZEND_USER_FUNCTION) {
		if (STRICT_G(dump)) {
			strict_op_dump(fopa TSRMLS_CC);
		}

		if (!fopa->doc_comment || !strstr(fopa->doc_comment, NO_STRICT)) {
			strict_scan_op_array(fopa TSRMLS_CC);
		}
	}
	return ZEND_HASH_APPLY_KEEP;
}
/* }}} */

#if PHP_VERSION_ID >= 50400
#define CLASS_ENTRY_DOC_COMMENT(ce) ce->info.user.doc_comment
#else
#define CLASS_ENTRY_DOC_COMMENT(ce) ce->doc_comment
#endif

/* {{{ int strict_scan_class
 */
static int strict_scan_class(zend_class_entry **class_entry TSRMLS_DC)
{
	zend_class_entry *ce = *class_entry;
	if (ce->type == ZEND_USER_CLASS && (!CLASS_ENTRY_DOC_COMMENT(ce) || !strstr(CLASS_ENTRY_DOC_COMMENT(ce), NO_STRICT))) {
		zend_hash_apply(&ce->function_table, (apply_func_t) strict_scan_function TSRMLS_CC);
	}
	return ZEND_HASH_APPLY_KEEP;
}
/* }}} */

/* {{{ zend_op_array strict_compile_file (file_handle, type)
 */
static zend_op_array *strict_compile_file(zend_file_handle *file_handle, int type TSRMLS_DC)
{
	zend_op_array *op_array;

	op_array = original_compile_file(file_handle, type TSRMLS_CC);

	if (STRICT_G(use)) {
		STRICT_G(filename) = file_handle->filename;

		if (STRICT_G(verbose)) {
			prepare_source_file();
		}

		if (op_array) {
			if (STRICT_G(dump)) {
				strict_op_dump(op_array);
			}
			strict_scan_op_array(op_array);
		}
		zend_hash_apply(CG(function_table), (apply_func_t)strict_scan_function TSRMLS_CC);
		zend_hash_apply(CG(class_table), (apply_func_t) strict_scan_class TSRMLS_CC);

		STRICT_G(filename) = NULL;

		if (STRICT_G(verbose)) {
			cleanup_source_file();
		}


		if (STRICT_G(detect)) {
			zend_bailout();
		}
	}

	return op_array;
}
/* }}} */

/* {{{ zend_op_array strict_compile_string (source_string, filename)
 */
static zend_op_array *strict_compile_string(zval *source_string, char *filename TSRMLS_DC)
{
	zend_op_array *op_array;

	op_array = original_compile_string(source_string, filename TSRMLS_CC);

	if (STRICT_G(dump)) {
		strict_op_dump(op_array);
	}

	if (STRICT_G(use)) {
		STRICT_G(filename) = filename;

		if (op_array) {
			strict_scan_op_array(op_array);
		}
		zend_hash_apply(CG(function_table), (apply_func_t)strict_scan_function TSRMLS_CC);
		zend_hash_apply(CG(class_table), (apply_func_t) strict_scan_class TSRMLS_CC);

		STRICT_G(filename) = NULL;

		if (STRICT_G(detect)) {
			zend_bailout();
		}
	}

	return op_array;
}
/* }}} */

/* {{{ void strict_execute
 */
#if PHP_VERSION_ID >= 50500
static void strict_execute(zend_execute_data *arg TSRMLS_DC)
#else
static void strict_execute(zend_op_array *arg TSRMLS_DC)
#endif
{
	if (STRICT_G(execute)) {
		original_execute(arg TSRMLS_CC);
	}
}
/* }}} */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
