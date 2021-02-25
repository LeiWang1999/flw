#ifndef __FIRMWARE_APE_ENGINE_INTERNAL_H_
#define __FIRMWARE_APE_ENGINE_INTERNAL_H_

#define BITS(num, range) ((((0xFFFFFFFF >> (31 - (1 ? range))) & \
			(0xFFFFFFFF << (0 ? range))) & num) >> \
			(0 ? range))
#define HIGH32BITS(val64bit) ((uint32_t)(val64bit >> 32))
#define LOW32BITS(val64bit) ((uint32_t)(val64bit))

#ifdef MIN
#undef MIN
#endif /* MIN */

#ifdef MAX
#undef MAX
#endif /* MAX */

#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/*********************************************************/
/******************** Utilities **************************/
/*********************************************************/
#ifdef DEBUG
#define CHECK_ALIGN(val, align)		 assert((val&(align-1)) == 0)
#else
#define CHECK_ALIGN(val, align)
#endif /* DEBUG */

#define MASK(reg, field)		(reg##_##field##_FIELD)
#define FIELD_ENUM(r, f, e)		(r##_##f##_##e)
#define SHIFT(reg, field)		(reg##_##field##_SHIFT)

#define GLB_REG(name)                GLB_##name##_0
#define MCIF_REG(name)               MCIF_##name##_0
#define CVIF_REG(name)               CVIF_##name##_0
#define BDMA_REG(name)               BDMA_##name##_0
#define CDMA_REG(name)               CDMA_##name##_0
#define CSC_REG(name)                CSC_##name##_0
#define CMAC_A_REG(name)             CMAC_A_##name##_0
#define CMAC_B_REG(name)             CMAC_B_##name##_0
#define CACC_REG(name)               CACC_##name##_0
#define SDP_RDMA_REG(name)           SDP_RDMA_##name##_0
#define SDP_REG(name)                SDP_##name##_0
#define PDP_RDMA_REG(name)           PDP_RDMA_##name##_0
#define PDP_REG(name)                PDP_##name##_0
#define CDP_RDMA_REG(name)           CDP_RDMA_##name##_0
#define CDP_REG(name)                CDP_##name##_0
#define RBK_REG(name)                RBK_##name##_0

/* alias for register read for each sub-module */
#define glb_reg_read(reg)           reg_read(GLB_REG(reg))
#define bdma_reg_read(reg)          reg_read(BDMA_REG(reg))
#define cdma_reg_read(reg)          reg_read(CDMA_REG(reg))
#define csc_reg_read(reg)           reg_read(CSC_REG(reg))
#define cmac_a_reg_read(reg)        reg_read(CMAC_A_REG(reg))
#define cmac_b_reg_read(reg)        reg_read(CMAC_B_REG(reg))
#define cacc_reg_read(reg)          reg_read(CACC_REG(reg))
#define sdp_rdma_reg_read(reg)      reg_read(SDP_RDMA_REG(reg))
#define sdp_reg_read(reg)           reg_read(SDP_REG(reg))
#define pdp_rdma_reg_read(reg)      reg_read(PDP_RDMA_REG(reg))
#define pdp_reg_read(reg)           reg_read(PDP_REG(reg))
#define cdp_rdma_reg_read(reg)      reg_read(CDP_RDMA_REG(reg))
#define cdp_reg_read(reg)           reg_read(CDP_REG(reg))
#define rubik_reg_read(reg)         reg_read(RBK_REG(reg))

/* alias for register write for each sub-module */
#define glb_reg_write(reg, val)      reg_write(GLB_REG(reg), val)
#define bdma_reg_write(reg, val)     reg_write(BDMA_REG(reg), val)
#define cdma_reg_write(reg, val)     reg_write(CDMA_REG(reg), val)
#define csc_reg_write(reg, val)      reg_write(CSC_REG(reg), val)
#define cmac_a_reg_write(reg, val)   reg_write(CMAC_A_REG(reg), val)
#define cmac_b_reg_write(reg, val)   reg_write(CMAC_B_REG(reg), val)
#define cacc_reg_write(reg, val)     reg_write(CACC_REG(reg), val)
#define sdp_rdma_reg_write(reg, val) reg_write(SDP_RDMA_REG(reg), val)
#define sdp_reg_write(reg, val)      reg_write(SDP_REG(reg), val)
#define pdp_rdma_reg_write(reg, val) reg_write(PDP_RDMA_REG(reg), val)
#define pdp_reg_write(reg, val)      reg_write(PDP_REG(reg), val)
#define cdp_rdma_reg_write(reg, val) reg_write(CDP_RDMA_REG(reg), val)
#define cdp_reg_write(reg, val)      reg_write(CDP_REG(reg), val)
#define rubik_reg_write(reg, val)    reg_write(RBK_REG(reg), val)

#define NULL (void*)0

void reg_write(uint32_t addr, uint32_t reg);
uint32_t reg_read(uint32_t addr);

int32_t dla_get_dma_address(void *driver_context, void *task_data,
					int16_t index, void *dst_ptr,
					uint32_t destination);

int32_t
dla_get_dma_cube_address(void *driver_context, void *task_data,
						int16_t index,
						uint32_t offset,
						void *dst_ptr,
						uint32_t destination);
int
dla_read_input_address(struct data_cube *data,uint64_t *address);
		   //    int16_t op_index,
		    //   uint8_t roi_index,
		     //  uint8_t bpp);
#endif
