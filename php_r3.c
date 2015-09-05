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


/**
 * This function allocates a persistent zval and copy the string with pestrndup (persistent).
 */
void MAKE_PZVAL_STR(zval** zdata, zval * zstr)
{
    *zdata = pemalloc(sizeof(zval), 1);
    Z_TYPE_PP(zdata) = Z_TYPE_P(zstr);
    Z_STRVAL_PP(zdata) = pestrndup(Z_STRVAL_P(zstr), Z_STRLEN_P(zstr), 1);
    Z_STRLEN_PP(zdata) = Z_STRLEN_P(zstr);
}

static const zend_function_entry r3_functions[] = {
    PHP_FE(r3_tree_create, NULL)
    PHP_FE(r3_tree_create_persist, NULL)
    PHP_FE(r3_tree_delete_persist, NULL)
    PHP_FE(r3_tree_store, NULL)
    PHP_FE(r3_tree_insert, NULL)
    PHP_FE(r3_tree_compile, NULL)
    PHP_FE(r3_tree_match, NULL)
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
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &capacity) == FAILURE) {
        RETURN_FALSE;
    }

    php_r3_resource *res;
    res = emalloc(sizeof(php_r3_resource));
    res->node = r3_tree_create(capacity);
    ZEND_REGISTER_RESOURCE(return_value, res, le_r3_resource);
}

PHP_FUNCTION(r3_tree_insert)
{
    zval *zres = NULL;
    zval *zcallback = NULL;
    char *path = NULL;
    int   path_len;
    php_r3_resource *res = NULL;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rsz",
                &zres, &path, &path_len, &zcallback) == FAILURE) {
        RETURN_FALSE;
    }


    ZEND_FETCH_RESOURCE(res, php_r3_resource*, &zres, -1, PHP_R3_RESOURCE_NAME, le_r3_resource_persist);
    if (res) {

        // copy data
        zval *zdata = NULL;

        // callable in string format
        if (Z_TYPE_P(zcallback) == IS_STRING) {

            MAKE_PZVAL_STR(&zdata, zcallback);

        } else if (Z_TYPE_P(zcallback) == IS_ARRAY) {

            // array_init

            zdata = pemalloc(sizeof(zval), 1);
            // INIT_ZVAL(zdata);
            Z_TYPE_P(zdata) = IS_ARRAY;
            Z_ARRVAL_P(zdata) = pemalloc(sizeof(HashTable), 1);
            zend_hash_init(Z_ARRVAL_P(zdata), 2, NULL, NULL, 1);
            zval **itemdata;
            HashTable *arr_hash = Z_ARRVAL_P(zcallback);
            HashPosition pointer;
            for(zend_hash_internal_pointer_reset_ex(arr_hash, &pointer);
                zend_hash_get_current_data_ex(arr_hash, (void**) &itemdata, &pointer) == SUCCESS; 
                zend_hash_move_forward_ex(arr_hash, &pointer)) 
            {
                if (Z_TYPE_PP(itemdata) != IS_STRING) {
                    php_error(E_ERROR, "Invalid callback data type.");
                }

                // zend_hash_next_index_insert(Z_ARRVAL_P(arg), &value, sizeof(zval *), NULL);
                zval *zp_str = NULL;
                MAKE_PZVAL_STR(&zp_str, *itemdata);
                Z_ADDREF_P(zp_str);
                add_next_index_zval(zdata, zp_str);
            }
        }

        // zend_hash_add(&fcgi_mgmt_vars, name, name_len, &zvalue, sizeof(zvalue), NULL);

        // int data = 10;
        char *errstr = NULL;
        node *ret = r3_tree_insert_pathl_ex(res->node, path, path_len, NULL, (void*) zdata, &errstr);
        if (ret == NULL) {
            // failed insertion
            // printf("error: %s\n", errstr);
            // php_error(E_ERROR, errstr);
            free(errstr); // errstr is created from `asprintf`, so you have to free it manually.
            RETURN_FALSE;
        }
        RETURN_TRUE;
    }
    RETURN_FALSE;
}


PHP_FUNCTION(r3_tree_match)
{
    zval *zres = NULL;
    char *path = NULL;
    int   path_len;
    php_r3_resource *res = NULL;


    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rs", &zres, &path, &path_len) == FAILURE) {
        RETURN_FALSE;
    }
    ZEND_FETCH_RESOURCE(res, php_r3_resource*, &zres, -1, PHP_R3_RESOURCE_NAME, le_r3_resource_persist);

    node *matched_node = NULL;
    if (res) {
        matched_node = r3_tree_matchl(res->node, path, path_len, NULL);

        zval *retzval = (zval*) matched_node->data;
        RETURN_ZVAL(retzval, 1, 0);
    }
    RETURN_FALSE;
}




PHP_FUNCTION(r3_tree_compile)
{
    zval *zres = NULL;
    php_r3_resource *res = NULL;
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zres) == FAILURE) {
        RETURN_FALSE;
    }

    ZEND_FETCH_RESOURCE(res, php_r3_resource*, &zres, -1, PHP_R3_RESOURCE_NAME, le_r3_resource_persist);
    if (res) {
        char *errstr = NULL;
        int err = r3_tree_compile(res->node, &errstr);
        if (err != 0) {
            // fail
            // printf("error: %s\n", errstr);
            free(errstr); // errstr is created from `asprintf`, so you have to free it manually.
            // php_error(E_ERROR, estrdup(errstr));
            RETURN_FALSE;
        }
    }
    RETURN_TRUE;
}

PHP_FUNCTION(r3_tree_create_persist)
{
    long  capacity = 10;
    char *namespace;
    int namespace_len;

    zend_rsrc_list_entry *le;

    /* parse parameters */
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sl",
            &namespace, &namespace_len, &capacity) == FAILURE) {
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
        RETURN_TRUE;
    }
    efree(key);
    RETURN_TRUE;
}







