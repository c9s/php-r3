#ifndef _PHP_R3_H
#define _PHP_R3_H
#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#ifdef ZTS
    #warning php_ext_uv module will *NEVER* be thread-safe
    #include <TSRM.h>
#endif

#include <php.h>
#include <r3.h>
#include <r3_str.h>
#include "common.h"

extern zend_module_entry r3_module_entry;

DECLARE_CLASS_ENTRY(R3);

PHP_FUNCTION(r3_tree_create);

// global variable structure
ZEND_BEGIN_MODULE_GLOBALS(r3)
    // zval *mux_array;
    HashTable * persistent_list;
    // zend_bool direction;
ZEND_END_MODULE_GLOBALS(r3)

#ifdef ZTS
#define R3_G(v) TSRMG(r3_globals_id, zend_r3_globals *, v)
#else
#define R3_G(v) (r3_globals.v)
#endif

#endif
