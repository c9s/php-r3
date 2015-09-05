#include "php_r3.h"
#include "ext/standard/info.h"
#include "thirdparty/r3/include/r3.h"

ZEND_DECLARE_MODULE_GLOBALS(r3);

PHP_MINIT_FUNCTION(r3);
PHP_MSHUTDOWN_FUNCTION(r3);
PHP_MINFO_FUNCTION(r3);

/*
ZEND_BEGIN_ARG_INFO_EX(arginfo_tree_create, 0, 0, 1)
	ZEND_ARG_INFO(0, capacity)
ZEND_END_ARG_INFO()
*/

static const zend_function_entry r3_functions[] = {
    PHP_FE(r3_tree_create, NULL)
    PHP_FE(r3_tree_create_persist, NULL)
    PHP_FE(r3_tree_delete_persist, NULL)
    PHP_FE(r3_tree_store, NULL)
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
    "R3",
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


static void php_r3_resource_persist_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
    php_r3_resource *res = (php_r3_resource*)rsrc->ptr;
    if (res) {
        if (res->node) {
            r3_tree_free(res->node);
        }
        pefree(res, 1);
    }
}

static void php_r3_resource_dtor(zend_rsrc_list_entry *rsrc TSRMLS_DC)
{
    php_r3_resource *res = (php_r3_resource*)rsrc->ptr;
    if (res) {
        if (res->node) {
            r3_tree_free(res->node);
        }
        efree(res);
    }
}

PHP_MINIT_FUNCTION(r3) {
    // CLASS_ENTRY_FUNCTION_C(R3);
    le_r3_resource = zend_register_list_destructors_ex(php_r3_resource_dtor, NULL, PHP_R3_RESOURCE_NAME, module_number);
    le_r3_resource_persist = zend_register_list_destructors_ex(NULL, php_r3_resource_persist_dtor, PHP_R3_RESOURCE_NAME, module_number);
    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION(r3) {
    return SUCCESS;
}


PHP_MINFO_FUNCTION(r3) {
    php_info_print_table_start();
    php_info_print_table_header(2, "r3 support", "enabled");
    php_info_print_table_end();
    // DISPLAY_INI_ENTRIES();
}

PHP_FUNCTION(r3_tree_store)
{
    char *namespace;
    int namespace_len;

    zval *zres;
    php_r3_resource *res;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sr",
                    &namespace, &namespace_len, &zres) == FAILURE) {
        RETURN_FALSE;
    }

    ZEND_FETCH_RESOURCE(res, php_r3_resource*, &zres, -1, PHP_R3_RESOURCE_NAME, le_r3_resource);

    // TODO store in the global

    RETURN_TRUE;
}

PHP_FUNCTION(r3_tree_create)
{
    long  capacity = 10;

    /* parse parameters */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l",
            &capacity) == FAILURE) {
        RETURN_FALSE;
    }

    php_r3_resource *res;
    res = emalloc(sizeof(php_r3_resource));
    res->node = r3_tree_create(capacity);
    ZEND_REGISTER_RESOURCE(return_value, res, le_r3_resource);
}

PHP_FUNCTION(r3_tree_create_persist)
{
    long  capacity = 10;
    char *namespace;
    int namespace_len;

    zend_rsrc_list_entry *le;

    /* parse parameters */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "ls",
            &capacity, &namespace, &namespace_len) == FAILURE) {
        RETURN_FALSE;
    }


    char *key;
    int key_len = spprintf(&key, 0, "r3_%s", namespace);
    if (zend_hash_find(&EG(persistent_list), key, key_len + 1, (void**) &le) == SUCCESS) {
        ZEND_REGISTER_RESOURCE(return_value, le->ptr, le_r3_resource_persist);
        efree(key);
        return;
    }

    php_r3_resource *res;
    res = pemalloc(sizeof(php_r3_resource), 1);
    res->node = r3_tree_create(capacity);
    ZEND_REGISTER_RESOURCE(return_value, res, le_r3_resource_persist);


    zend_rsrc_list_entry new_le;
    new_le.ptr = res;
    Z_TYPE(new_le) = le_r3_resource_persist;
    if (zend_hash_update(&EG(persistent_list), key, key_len + 1, (void*) &new_le, sizeof(zend_rsrc_list_entry), NULL) == FAILURE) {
        RETURN_FALSE;
    }
    efree(key);
}

PHP_FUNCTION(r3_tree_delete_persist)
{
    char *namespace;
    int namespace_len;

    zend_rsrc_list_entry *le;

    /* parse parameters */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
            &namespace, &namespace_len) == FAILURE) {
        RETURN_FALSE;
    }


    char *key;
    int key_len = spprintf(&key, 0, "r3_%s", namespace);
    if (zend_hash_find(&EG(persistent_list), key, key_len + 1, (void**) &le) == SUCCESS) {
        // ZEND_REGISTER_RESOURCE(return_value, le->ptr, le_r3_resource_persist);
        // zend_list_delete(Z_LVAL_P(zperson));
        zend_hash_del(&EG(persistent_list), key, key_len + 1);
        efree(key);
        return;
    }
    efree(key);
}







