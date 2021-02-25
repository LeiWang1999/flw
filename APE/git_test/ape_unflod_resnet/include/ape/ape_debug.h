#ifndef __APE_DEBUG_H_
#define __APE_DEBUG_H_

#include "ape_dev_if.h"
#if RM_JPEG
#include "ape_types.h"
#endif

#if JPEG
#include "ape_type.h"
#include "ape_types.h"
#endif

void
ape_debug_conv_surface_desc(struct conv_surface_desc *desc, ScS32 roi);

void 
ape_debug_cdp_surface_desc(struct cdp_surface_desc *desc, ScS32 roi);

void
ape_debug_sdp_surface_desc(struct sdp_surface_desc *desc, ScS32 roi);

void
ape_debug_pdp_surface_desc(struct pdp_surface_desc *desc, ScS32 roi);

void
ape_debug_conv_op_desc(struct conv_op_desc *desc, ScS32 roi);

void
ape_debug_cdp_op_desc(struct cdp_op_desc *desc, ScS32 roi);

void
ape_debug_sdp_op_desc(struct sdp_op_desc *desc, ScS32 roi);

void
ape_debug_pdp_op_desc(struct pdp_op_desc *desc, ScS32 roi);

#define assert(condition)
#define STRINGIFY(s) #s
#define DEFER_STRINGIFY(s) STRINGIFY(s)
#define FILELINE DEFER_STRINGIFY(__LINE__)
#define FILENAME DEFER_STRINGIFY(__FILE__)
//#define DEBUG_ASSERT
#ifdef DEBUG_ASSERT
#define ASSERT_GOTO(_condition, _ret, _err_value, _goto)	\
do {								\
	if (!(_condition)) {					\
			\
		_ret = _err_value;				\
		goto _goto;					\
	} else {						\
		_ret = 0;					\
	}							\
} while (0)
#else
#define ASSERT_GOTO(_condition, _ret, _err_value, _goto) 
//assert(condition)
#endif /* DEBUG_ASSERT */

#endif