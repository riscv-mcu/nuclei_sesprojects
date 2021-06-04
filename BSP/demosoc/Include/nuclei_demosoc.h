// See LICENSE for license details.
#ifndef _NUCLEI_DEMOSOC_H
#define _NUCLEI_DEMOSOC_H

#include <stdint.h>
#include <stdio.h>

#include "riscv_encoding.h"
#include "demosoc_eclic.h"
#include "demosoc_systimer.h"
#include "demosoc_uart.h"
#include "demosoc_gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

#if __riscv_xlen == 32
  typedef uint32_t rv_csr_t;
#elif __riscv_xlen == 64
  typedef uint64_t rv_csr_t;
#else
  typedef uint32_t rv_csr_t;
#endif

#define read_csr                    __RV_CSR_READ
#define set_csr                     __write_csr
#define write_csr                   __write_csr
#define clear_csr                   __clear_bits_csr

// IOF Settings for UART0 and UART1
#define IOF0_UART0_MASK             _AC(0x00030000, UL)
#define IOF_UART0_RX                (16u)
#define IOF_UART0_TX                (17u)
  
#define IOF0_UART1_MASK             _AC(0x03000000, UL)
#define IOF_UART1_RX                (24u)
#define IOF_UART1_TX                (25u)

// Peripherals memory map
#define DEMOSOC_PERIPH_BASE         (0x10000000UL)                          /*!< (Peripheral) Base Address */

#define GPIO_BASE                   (DEMOSOC_PERIPH_BASE + 0x12000)         /*!< (GPIO) Base Address */
#define UART0_BASE                  (DEMOSOC_PERIPH_BASE + 0x13000)         /*!< (UART0) Base Address */
#define ECLIC_BASE                  (0x0C000000UL)                          /*!< Set to ECLIC baseaddr of your device */
#define SYSTIMER_BASE               (0x02000000UL)                          /*!< Set to SysTimer baseaddr of your device */

/** register structures to access demosoc resources */
#define GPIO                        ((GPIO_TypeDef *) GPIO_BASE)
#define UART0                       ((UART_TypeDef *) UART0_BASE)
#define ECLIC                       ((CLIC_Type *) ECLIC_BASE)
#define SysTimer                    ((SysTimer_Type *) SYSTIMER_BASE)

/** Debug UART */
#define SOC_DEBUG_UART              UART0

#define RTC_FREQ                    32768
// The TIMER frequency is just the RTC frequency
#define SOC_TIMER_FREQ              RTC_FREQ

#ifndef SYSTEM_CLOCK
#define SYSTEM_CLOCK                (16000000UL)
#endif

/** Set DEBUG_SEMIHOST to 1 if you want to use Segger RTT or IAR Debug Print */
#ifndef DEBUG_SEMIHOST
#define DEBUG_SEMIHOST              1
#endif

#define __ASM                       __asm
/**
 * \brief CSR operation Macro for csrrw instruction.
 * \details
 * Read the content of csr register to __v,
 * then write content of val into csr register, then return __v
 * \param csr   CSR macro definition defined in
 *              \ref NMSIS_Core_CSR_Registers, eg. \ref CSR_MSTATUS
 * \param val   value to store into the CSR register
 * \return the CSR register value before written
 */
#define __RV_CSR_SWAP(csr, val)                                 \
    ({                                                          \
        register rv_csr_t __v = (unsigned long)(val);           \
        __ASM volatile("csrrw %0, " STRINGIFY(csr) ", %1"       \
                     : "=r"(__v)                                \
                     : "rK"(__v)                                \
                     : "memory");                               \
        __v;                                                    \
    })

/**
 * \brief CSR operation Macro for csrr instruction.
 * \details
 * Read the content of csr register to __v and return it
 * \param csr   CSR macro definition defined in
 *              \ref NMSIS_Core_CSR_Registers, eg. \ref CSR_MSTATUS
 * \return the CSR register value
 */
#define __RV_CSR_READ(csr)                                      \
    ({                                                          \
        register rv_csr_t __v;                                  \
        __ASM volatile("csrr %0, " STRINGIFY(csr)               \
                     : "=r"(__v)                                \
                     :                                          \
                     : "memory");                               \
        __v;                                                    \
    })

/**
 * \brief CSR operation Macro for csrw instruction.
 * \details
 * Write the content of val to csr register
 * \param csr   CSR macro definition defined in
 *              \ref NMSIS_Core_CSR_Registers, eg. \ref CSR_MSTATUS
 * \param val   value to store into the CSR register
 */
#define __RV_CSR_WRITE(csr, val)                                \
    ({                                                          \
        register rv_csr_t __v = (rv_csr_t)(val);                \
        __ASM volatile("csrw " STRINGIFY(csr) ", %0"            \
                     :                                          \
                     : "rK"(__v)                                \
                     : "memory");                               \
    })

/**
 * \brief CSR operation Macro for csrrs instruction.
 * \details
 * Read the content of csr register to __v,
 * then set csr register to be __v | val, then return __v
 * \param csr   CSR macro definition defined in
 *              \ref NMSIS_Core_CSR_Registers, eg. \ref CSR_MSTATUS
 * \param val   Mask value to be used wih csrrs instruction
 * \return the CSR register value before written
 */
#define __RV_CSR_READ_SET(csr, val)                             \
    ({                                                          \
        register rv_csr_t __v = (rv_csr_t)(val);                \
        __ASM volatile("csrrs %0, " STRINGIFY(csr) ", %1"       \
                     : "=r"(__v)                                \
                     : "rK"(__v)                                \
                     : "memory");                               \
        __v;                                                    \
    })

/**
 * \brief CSR operation Macro for csrs instruction.
 * \details
 * Set csr register to be csr_content | val
 * \param csr   CSR macro definition defined in
 *              \ref NMSIS_Core_CSR_Registers, eg. \ref CSR_MSTATUS
 * \param val   Mask value to be used wih csrs instruction
 */
#define __RV_CSR_SET(csr, val)                                  \
    ({                                                          \
        register rv_csr_t __v = (rv_csr_t)(val);                \
        __ASM volatile("csrs " STRINGIFY(csr) ", %0"            \
                     :                                          \
                     : "rK"(__v)                                \
                     : "memory");                               \
    })

/**
 * \brief CSR operation Macro for csrrc instruction.
 * \details
 * Read the content of csr register to __v,
 * then set csr register to be __v & ~val, then return __v
 * \param csr   CSR macro definition defined in
 *              \ref NMSIS_Core_CSR_Registers, eg. \ref CSR_MSTATUS
 * \param val   Mask value to be used wih csrrc instruction
 * \return the CSR register value before written
 */
#define __RV_CSR_READ_CLEAR(csr, val)                           \
    ({                                                          \
        register rv_csr_t __v = (rv_csr_t)(val);                \
        __ASM volatile("csrrc %0, " STRINGIFY(csr) ", %1"       \
                     : "=r"(__v)                                \
                     : "rK"(__v)                                \
                     : "memory");                               \
        __v;                                                    \
    })

/**
 * \brief CSR operation Macro for csrc instruction.
 * \details
 * Set csr register to be csr_content & ~val
 * \param csr   CSR macro definition defined in
 *              \ref NMSIS_Core_CSR_Registers, eg. \ref CSR_MSTATUS
 * \param val   Mask value to be used wih csrc instruction
 */
#define __RV_CSR_CLEAR(csr, val)                                \
    ({                                                          \
        register rv_csr_t __v = (rv_csr_t)(val);                \
        __ASM volatile("csrc " STRINGIFY(csr) ", %0"            \
                     :                                          \
                     : "rK"(__v)                                \
                     : "memory");                               \
    })

extern volatile uint32_t SystemCoreClock;

#ifdef __cplusplus
}
#endif
#endif
