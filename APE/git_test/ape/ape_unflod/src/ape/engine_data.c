#include "ape_cmd.h"
#include"conv.h"
#include"bdma.h"
#include"sdp.h"
#include"cdp.h"
#include"rbk.h"
#include"pdp.h"
#include "log.h"
void (*ape_fun[CMD_OP_NUM])(void *) =  {
                                            bdma_program,bdma_set_hw_idx,bdma_enable,
                                            conv_program,conv_set_hw_idx,conv_enable,
                                            sdp_program,sdp_set_hw_idx,sdp_enable,
                                            pdp_program,pdp_set_hw_idx,pdp_enable,
                                            cdp_program,cdp_set_hw_idx,cdp_enable,
                                            rbk_program,rbk_set_hw_idx,rbk_enable
                                        };
void (*ape_parse[APE_OP_NUM])(void *) = {
                                        bdma_parse,conv_parse,sdp_parse,pdp_parse,cdp_parse,rbk_parse
};
