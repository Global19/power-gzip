#ifndef __CRB_H
#define __CRB_H
#include <linux/types.h>

#if 0
typedef u64 __be64;
//typedef unsigned short __be16;
typedef unsigned int __be32;
#endif

typedef unsigned char u8;
typedef unsigned int u32;
typedef unsigned long long u64;

/* From nx-842.h */

/* CCW 842 CI/FC masks
 * NX P8 workbook, section 4.3.1, figure 4-6
 * "CI/FC Boundary by NX CT type"
 */ 
#define CCW_CI_842              (0x00003ff8)
#define CCW_FC_842              (0x00000007)

/* end - nx-842.h */


#ifndef __aligned
#define __aligned(x)            __attribute__((aligned(x)))
#endif

#ifndef __packed
#define __packed        __attribute__((packed))
#endif



/* Chapter 6.5.8 Coprocessor-Completion Block (CCB) */

#define CCB_VALUE		(0x3fffffffffffffff)
#define CCB_ADDRESS		(0xfffffffffffffff8)
#define CCB_CM			(0x0000000000000007)
#define CCB_CM0			(0x0000000000000004)
#define CCB_CM12		(0x0000000000000003)

#define CCB_CM0_ALL_COMPLETIONS	(0x0)
#define CCB_CM0_LAST_IN_CHAIN	(0x4)
#define CCB_CM12_STORE		(0x0)
#define CCB_CM12_INTERRUPT	(0x1)

#define CCB_SIZE		(0x10)
#define CCB_ALIGN		CCB_SIZE

#if 0
#define	CCW_FC_842_COMP_NOCRC	0
#define	CCW_FC_842_COMP_CRC	1
#endif

struct coprocessor_completion_block {
	__be64 value;
	__be64 address;
} __packed __aligned(CCB_ALIGN);


/* Chapter 6.5.7 Coprocessor-Status Block (CSB) */

#define CSB_V			(0x80)
#define CSB_F			(0x04)
#define CSB_CH			(0x03)
#define CSB_CE_INCOMPLETE	(0x80)
#define CSB_CE_TERMINATION	(0x40)
#define CSB_CE_TPBC		(0x20)

#define CSB_CC_SUCCESS		(0)
#define CSB_CC_INVALID_ALIGN	(1)
#define CSB_CC_OPERAND_OVERLAP	(2)
#define CSB_CC_DATA_LENGTH	(3)
#define CSB_CC_TRANSLATION	(5)
#define CSB_CC_PROTECTION	(6)
#define CSB_CC_RD_EXTERNAL	(7)
#define CSB_CC_INVALID_OPERAND	(8)
#define CSB_CC_PRIVILEGE	(9)
#define CSB_CC_INTERNAL		(10)
#define CSB_CC_WR_EXTERNAL	(12)
#define CSB_CC_NOSPC		(13)
#define CSB_CC_EXCESSIVE_DDE	(14)
#define CSB_CC_WR_TRANSLATION	(15)
#define CSB_CC_WR_PROTECTION	(16)
#define CSB_CC_UNKNOWN_CODE	(17)
#define CSB_CC_ABORT		(18)
#define CSB_CC_TRANSPORT	(20)
#define CSB_CC_SEGMENTED_DDL	(31)
#define CSB_CC_PROGRESS_POINT	(32)
#define CSB_CC_DDE_OVERFLOW	(33)
#define CSB_CC_SESSION		(34)
#define CSB_CC_PROVISION	(36)
#define CSB_CC_CHAIN		(37)
#define CSB_CC_SEQUENCE		(38)
#define CSB_CC_HW		(39)

#define CSB_SIZE		(0x10)
#define CSB_ALIGN		CSB_SIZE

struct coprocessor_status_block {
	u8 flags;
	u8 cs;
	u8 cc;
	u8 ce;
	__be32 count;
	__be64 address;
} __packed __aligned(CSB_ALIGN);


/* Chapter 6.5.10 Data-Descriptor List (DDL)
 * each list contains one or more Data-Descriptor Entries (DDE)
 */

#define DDE_P			(0x8000)

#define DDE_SIZE		(0x10)
#define DDE_ALIGN		DDE_SIZE

struct data_descriptor_entry {
	__be16 flags;
	u8 count;
	u8 index;
	__be32 length;
	__be64 address;
} __packed __aligned(DDE_ALIGN);


/* Chapter 6.5.2 Coprocessor-Request Block (CRB) */

#define CRB_SIZE		(0x80)
#define CRB_ALIGN		(0x100) /* Errata: requires 256 alignment */

#if 0
/* CCW 842 CI/FC masks
 *  * NX P8 workbook, section 4.3.1, figure 4-6
 *   * "CI/FC Boundary by NX CT type"
 *    */ 
#define CCW_CI_842              (0x00003ff8)
#define CCW_FC_842              (0x00000007)
#endif


/* Coprocessor Status Block field
 *   ADDRESS	address of CSB
 *   C		CCB is valid
 *   AT		0 = addrs are virtual, 1 = addrs are phys
 *   M		enable perf monitor
 */
#define CRB_CSB_ADDRESS		(0xfffffffffffffff0)
#define CRB_CSB_C		(0x0000000000000008)
#define CRB_CSB_AT		(0x0000000000000002)
#define CRB_CSB_M		(0x0000000000000001)

struct coprocessor_request_block {
	__be32 ccw;
	__be32 flags;
	__be64 csb_addr;

	struct data_descriptor_entry source;
	struct data_descriptor_entry target;

	struct coprocessor_completion_block ccb;

	u8 reserved[48];

	struct coprocessor_status_block csb;
} __packed __aligned(CRB_ALIGN);

#define crb_csb_addr(c)         __be64_to_cpu(c->csb_addr)
#define crb_nx_fault_addr(c)    __be64_to_cpu(c->stamp.nx.fault_storage_addr)
#define crb_nx_flags(c)         c->stamp.nx.flags
#define crb_nx_fault_status(c)  c->stamp.nx.fault_status
#define crb_nx_pswid(c)		c->stamp.nx.pswid;


/* RFC02167 Initiate Coprocessor Instructions document
 * Chapter 8.2.1.1.1 RS
 * Chapter 8.2.3 Coprocessor Directive
 * Chapter 8.2.4 Execution
 *
 * The CCW must be converted to BE before passing to icswx()
 */

#define CCW_PS                  (0xff000000)
#define CCW_CT                  (0x00ff0000)
#define CCW_CD                  (0x0000ffff)
#define CCW_CL                  (0x0000c000)

#endif
