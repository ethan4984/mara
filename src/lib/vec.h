#ifndef VEC_H_
#define VEC_H_

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#define create_vec_struct(type) \
    struct { \
        type *data; \
        size_t size; \
        size_t element_cnt; \
        size_t current; \
    }
 
#define uninit_vec(type, name) \
    create_vec_struct(type) name;
 
#define vec(type, name) \
    create_vec_struct(type) name = { 0 };
 
#define extern_vec(type, name) \
    extern create_vec_struct(type) name;
 
#define global_vec(name) \
    typeof(name) name = { 0 };
 
#define static_vec(type, name) \
    static create_vec_struct(type) name = { 0 };

#define vec_push(type, name, element) ({ \
    int _ret = 0; \
    if(name.data == NULL) { \
        name.data = calloc(sizeof(type) * 32, sizeof(type)); \
        name.size = 32; \
    } \
    if(name.current >= name.size) { \
        name.size += 32; \
        name.data = realloc(name.data, name.size * sizeof(type)); \
    } \
    _ret = name.current; \
    name.data[name.current++] = element; \
    name.element_cnt++; \
    _ret; \
})

#define vec_search(type, name, index) ({ \
    __label__ lret; \
    type *_ret = NULL; \
    if(name.element_cnt <= (index)) { \
        goto lret; \
    } \
    _ret = &name.data[index]; \
lret: \
    _ret; \
})
 
#define vec_remove(type, name, index) ({ \
    __label__ lret; \
    int _ret = 0; \
    if(name.element_cnt <= index) { \
        _ret = -1; \
        goto lret; \
    } \
    type *tmp = malloc(sizeof(type) * name.size); \
    size_t _origin_cnt = 0; \
    for(size_t _i = 0; _i < name.element_cnt; _i++) { \
        if(_i != index) \
            tmp[_origin_cnt++] = name.data[_i]; \
    } \
    name.element_cnt--; \
    name.current--; \
    free(name.data); \
    name.data = tmp; \
lret: \
    _ret; \
})
 
#define vec_addr_remove(type, name, addr) ({ \
    int _ret = -1; \
    for(size_t _i = 0; _i < name.element_cnt; _i++) { \
        if(&name.data[_i] == (addr)) { \
            _ret = vec_remove(type, name, _i); \
            break; \
        } \
    } \
    _ret; \
})
 
#define vec_delete(name) \
    free(name.data);

#define create_stack_struct(type) \
    struct { \
        uninit_vec(type, data); \
        size_t top; \
    }

#define create_stack(type, name) \
    create_stack_struct(type) name = { 0 };

#define stack_push(type, stack, element) ({ \
    vec_push(type, stack.data, element); \
})

#define stack_pop(type, stack) ({ \
    __label__ _end; \
    type _data; \
    type *loc = vec_search(type, stack.data, stack.data.element_cnt - 1); \
    if(loc != NULL) \
        _data = *loc; \
    else \
        goto _end; \
    vec_remove(type, stack.data, stack.data.element_cnt - 1); \
_end: \
    _data; \
})

#define stack_view(type, stack) ({ \
    type _data; \
    type *loc = vec_search(type, stack.data, stack.data.element_cnt - 1); \
    if(loc != NULL) \
        _data = *loc; \
    _data; \
})

#define stack_delete(stack) \
    vec_delete(stack.data);

#endif
