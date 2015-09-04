#include "php_r3.h"

ZEND_DECLARE_MODULE_GLOBALS(r3);

PHP_MINIT_FUNCTION(r3);
PHP_MSHUTDOWN_FUNCTION(r3);
PHP_MINFO_FUNCTION(r3);

static const zend_function_entry r3_functions[] = {
    PHP_FE(r3_tree_create, NULL)
    // PHP_FE(r3_tree_insert_path, NULL)
    PHP_FE_END
};


#if COMPILE_DL_R3
ZEND_GET_MODULE(r3)
#endif

zend_module_entry r3_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
    STANDARD_MODULE_HEADER,
#endif
    "R3 router",
    r3_functions,
    PHP_MINIT(r3),
    PHP_MSHUTDOWN(r3),
    NULL,
    NULL,
    PHP_MINFO(r3),
#if ZEND_MODULE_API_NO >= 20010901
    "0.1",
#endif
    STANDARD_MODULE_PROPERTIES
};


PHP_MINIT_FUNCTION(r3) {
    CLASS_ENTRY_FUNCTION_C(R3);
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(r3) {
    return SUCCESS;
}


PHP_MINFO_FUNCTION(r3) {
    php_info_print_table_start();
    php_info_print_table_header(2, "r3 support", "enabled");
    php_info_print_table_end();
}


PHP_FUNCTION(r3_tree_create)
{
    zval *z_routes;
    char *path;
    int  path_len;

    /* parse parameters */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "as", 
                    &z_routes, 
                    &path, &path_len ) == FAILURE) {
        RETURN_FALSE;
    }
    RETURN_FALSE;
}




