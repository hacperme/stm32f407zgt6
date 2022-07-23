#include <string.h>

#include "FreeRTOS.h"

/************** wrap C library functions **************/
void *__wrap_malloc(size_t size)
{
    return pvPortMalloc(size);
}

void *__wrap__malloc_r(void *p, size_t size)
{

    return pvPortMalloc(size);
}

void __wrap_free(void *pv)
{
    vPortFree(pv);
}

void *__wrap_calloc(size_t a, size_t b)
{
    void *pvReturn;

    pvReturn = pvPortMalloc(a * b);
    if (pvReturn)
    {
        memset(pvReturn, 0, a * b);
    }

    return pvReturn;
}

void *__wrap_realloc(void *pv, size_t size)
{

    return NULL;
}

void __wrap__free_r(void *p, void *x)
{
    __wrap_free(x);
}

void *__wrap__realloc_r(void *p, void *x, size_t sz)
{
    return __wrap_realloc(x, sz);
}

void *__wrap_zalloc(size_t size)
{
    void *pvReturn;
    pvReturn = pvPortMalloc(size);
    if (pvReturn)
    {
        memset(pvReturn, 0, size);
    }
    return pvReturn;
}

// void __assert_func(const char *file, int line, const char *func, const char *failedexpr)
// {

// }