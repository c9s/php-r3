#ifndef _COMMON_H
#define _COMMON_H

#define CLASS_ENTRY(name) uv_ce_##name

#define OBJECT_HANDLER(name) object_handler_##name

#define CLASS_ENTRY_FUNCTION_D(name) \
    void init_uv_ce_##name(TSRMLS_D)

#define CLASS_ENTRY_FUNCTION_C(name) \
    init_uv_ce_##name(TSRMLS_C)    

#define DECLARE_CLASS_ENTRY(name) \
    zend_object_handlers OBJECT_HANDLER(name); \
    zend_class_entry *CLASS_ENTRY(name); \
    CLASS_ENTRY_FUNCTION_D(name)

#define FUNCTION_ENTRY(name) \
    uv_fe_##name

#define DECLARE_FUNCTION_ENTRY(name) \
    static zend_function_entry FUNCTION_ENTRY(name)[]

#define REGISTER_INTERNAL_CLASS(name) \
    CLASS_ENTRY(name) = zend_register_internal_class_ex(&ce, NULL, NULL TSRMLS_CC)
    
#define REGISTER_INTERNAL_CLASS_EX(name, base) \
    CLASS_ENTRY(name) = zend_register_internal_class_ex(&ce, CLASS_ENTRY(base), NULL TSRMLS_CC)

#define INIT_CLASS_WITH_OBJECT_NEW(name, ns_name, create_function) \
    zend_class_entry ce; \
    memcpy(&OBJECT_HANDLER(name), zend_get_std_object_handlers(), sizeof(zend_object_handlers)); \
    INIT_CLASS_ENTRY(ce, ns_name, FUNCTION_ENTRY(name))

#define REGISTER_CLASS_WITH_OBJECT_NEW(name, ns_name, create_function) \
    INIT_CLASS_WITH_OBJECT_NEW(name, ns_name, create_function); \
    REGISTER_INTERNAL_CLASS(name); \
    CLASS_ENTRY(name)->create_object = create_function

#define EXTENDS_CLASS_WITH_OBJECT_NEW(name, ns_name, create_function, base) \
    INIT_CLASS_WITH_OBJECT_NEW(name, ns_name, create_function); \
    REGISTER_INTERNAL_CLASS_EX(name, base); \
    CLASS_ENTRY(name)->create_object = create_function    
 
#define REGISTER_CLASS(name, ns_name) \
    zend_class_entry ce; \
    INIT_CLASS_ENTRY(ce, ns_name, FUNCTION_ENTRY(name)); \
    REGISTER_INTERNAL_CLASS(name)
    
#define EXTENDS_CLASS(name, ns_name, base) \
    zend_class_entry ce; \
    INIT_CLASS_ENTRY(ce, ns_name, FUNCTION_ENTRY(name)); \
    REGISTER_INTERNAL_CLASS_EX(name, base)

#define ARGINFO(classname, method) \
    arginfo_##classname##_##method

#ifndef offsetof
#define offsetof(s,memb) ((size_t)((char *)&((s *)0)->memb-(char *)0))
#endif

#define FETCH_RESOURCE(pointer, type) (type *) (pointer - offsetof(type, zo))
    
#define FETCH_OBJECT_RESOURCE(object, type) FETCH_RESOURCE(zend_object_store_get_object(object TSRMLS_CC), type)

#define REGISTER_CLASS_CONSTANT_LONG(class, name) \
    zend_declare_class_constant_long(CLASS_ENTRY(class), ZEND_STRL(#name), name TSRMLS_CC)

#endif
