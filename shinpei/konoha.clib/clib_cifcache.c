/* Clib cache
 *  ffi package, for DEOS
 *  written by shinpei(c)2011
 */


#if defined(K_USING_MACOSX_)
//#define MACOSX
#include <ffi/ffi.h>
#elif defined(K_USING_LINUX_)
#include <ffi.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* ------------------------------------------------------------------------ */
/* [Glue] */
#include <dglue.h> // for DEOS
// #include <glue.h>

/* ------------------------------------------------------------------------ */





/* ------------------------------------------------------------------------ */

#ifdef __cplusplus
}
#endif
