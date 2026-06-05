/*******************************************************************************
* \file cy_em_eeprom.h
*
* \brief
* This file provides the function prototypes and constants for the
* Emulated EEPROM middleware library.
*
********************************************************************************
* \copyright
* (c) (2019-2026), Infineon Technologies AG, or an affiliate of Infineon
* Technologies AG. All rights reserved.
* This software, associated documentation and materials ("Software") is
* owned by Infineon Technologies AG or one of its affiliates ("Infineon")
* and is protected by and subject to worldwide patent protection, worldwide
* copyright laws, and international treaty provisions. Therefore, you may use
* this Software only as provided in the license agreement accompanying the
* software package from which you obtained this Software. If no license
* agreement applies, then any use, reproduction, modification, translation, or
* compilation of this Software is prohibited without the express written
* permission of Infineon.
*
* Disclaimer: UNLESS OTHERWISE EXPRESSLY AGREED WITH INFINEON, THIS SOFTWARE
* IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
* INCLUDING, BUT NOT LIMITED TO, ALL WARRANTIES OF NON-INFRINGEMENT OF
* THIRD-PARTY RIGHTS AND IMPLIED WARRANTIES SUCH AS WARRANTIES OF FITNESS FOR A
* SPECIFIC USE/PURPOSE OR MERCHANTABILITY.
* Infineon reserves the right to make changes to the Software without notice.
* You are responsible for properly designing, programming, and testing the
* functionality and safety of your intended application of the Software, as
* well as complying with any legal requirements related to its use. Infineon
* does not guarantee that the Software will be free from intrusion, data theft
* or loss, or other breaches ("Security Breaches"), and Infineon shall have
* no liability arising out of any Security Breaches. Unless otherwise
* explicitly approved by Infineon, the Software may not be used in any
* application where a failure of the Product or any consequences of the use
* thereof can reasonably be expected to result in personal injury.
*******************************************************************************/
/**
 ********************************************************************************
 * \addtogroup group_emeeprom Em_EEPROM Middleware Library
 * \{
 * The Emulated EEPROM (Em_EEPROM) middleware emulates an EEPROM storage in
 * the MCU's non volatile memory and it is built on top of a block storage
 * devie that abstracts the underlying memory architecture so that the middleware
 * is portable to a wide range of devices.
 *
 * Use the Em_EEPROM to store non-volatile data on a target device when
 * increasing flash memory endurance and restoring corrupted data from
 * a redundant copy is required.
 *
 * <b>Features:</b>
 * * EEPROM-Like Non-Volatile Storage
 * * Easy to use Read and Write
 * * Optional Wear Leveling
 * * Optional Redundant Data Storage
 *
 * For quick start guide, configuration considerations, storage placement,
 * operating modes, limitations, and migration information, see the
 * 'How to Use' and 'Configuration Considerations' sections in README.md.
 *
 ********************************************************************************
 *
 */

#if !defined(CY_EM_EEPROM_H)
#define CY_EM_EEPROM_H

#include <stddef.h>
#include "cy_device_headers.h"
#include "cy_syslib.h"
#include "cy_result.h"
#include "mtb_block_storage.h"


/* The C binding of definitions if building with the C++ compiler */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */



/** An invalid parameter value is passed in. */
#define MTB_EM_EEPROM_BAD_PARAM                 \
    CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_MIDDLEWARE_EM_EEPROM, 0)
/** The data in EM_EEPROM is corrupted, the checksum failed */
#define MTB_EM_EEPROM_BAD_CHECKSUM                 \
    CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_MIDDLEWARE_EM_EEPROM, 1)
/** The provided configuration is invalid or placing Em_EEPROM in NVM failed */
#define MTB_EM_EEPROM_BAD_DATA                 \
    CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_MIDDLEWARE_EM_EEPROM, 2)
/** Write to Em_EEPROM failed. */
#define MTB_EM_EEPROM_WRITE_FAIL              \
    CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_MIDDLEWARE_EM_EEPROM, 3)
/** When redundant copy is enabled and one of the copies has bad CRC and other one valid CRC */
#define MTB_EM_EEPROM_REDUNDANT_COPY_USED               \
    CY_RSLT_CREATE(CY_RSLT_TYPE_ERROR, CY_RSLT_MODULE_MIDDLEWARE_EM_EEPROM, 4)




/*******************************************************************************
* API Constants
*******************************************************************************/
/** Library major version */
#define CY_EM_EEPROM_MW_VERSION_MAJOR       (2)

/** Library minor version */
#define CY_EM_EEPROM_MW_VERSION_MINOR       (70)



/*******************************************************************************
* Data Structure definitions
*******************************************************************************/

/** The Em_EEPROM return enumeration type. */
typedef enum
{
    /** The operation executed successfully */
    CY_EM_EEPROM_SUCCESS      = 0x00uL,
    /** An invalid parameter value is passed in. */
    CY_EM_EEPROM_BAD_PARAM    = MTB_EM_EEPROM_BAD_PARAM,
    /** The data in EM_EEPROM is corrupted, the checksum failed */
    CY_EM_EEPROM_BAD_CHECKSUM = MTB_EM_EEPROM_BAD_CHECKSUM,
    /** The provided configuration is invalid or placing Em_EEPROM in NVM failed */
    CY_EM_EEPROM_BAD_DATA     = MTB_EM_EEPROM_BAD_DATA,
    /** Write to Em_EEPROM failed. */
    CY_EM_EEPROM_WRITE_FAIL   = MTB_EM_EEPROM_WRITE_FAIL,
    /** When redundant copy is enabled and one of the copies has bad CRC and other one valid CRC */
    CY_EM_EEPROM_REDUNDANT_COPY_USED = MTB_EM_EEPROM_REDUNDANT_COPY_USED
} cy_en_em_eeprom_status_t;

/** Em_EEPROM configuration structure */
typedef struct
{
    /**
     * The logical size of data in bytes to store in the Em_EEPROM.
     * The size is rounded up to a full Em_EEPROM row size physically.
     * The row size is specific for a device family.
     * Refer to the specific PSOC device datasheet.<br>
     * Note this size is often smaller than the total amount of nvm used
     * for the Em_EEPROM storage. The Em_EEPROM storage size depends on
     * the Em_EEPROM configuration and can be bigger because increasing
     * nvm endurance (wear-leveling) and restoring corrupted data from
     * a redundant copy. See section 'Storage Variable Location and Size' in README.md
     * for size calculation equations.
     */
    uint32_t eepromSize;

    /**
     * Simple mode, when enabled (1 - enabled, 0 - disabled), means no
     * additional service information is stored by the Em_EEPROM middleware
     * like checksums, headers, a number of writes, etc.
     * Data is stored directly by the specified address.
     * The size of Em_EEPROM storage is equal to the number of
     * byte specified in the eepromSize parameter rounded up to a full row
     * size. The wear leveling and
     * redundant copy features are disabled, i.e. wearLevelingFactor and
     * redundantCopy parameters are ignored.
     */
    uint8_t simpleMode;

    /**
     * The higher the factor is, the more nvm is used,
     * but a higher number of erase/write cycles can be done on Em_EEPROM.
     * Multiply this number by the datasheet write endurance spec to determine
     * the max of write cycles.<br>
     * The amount of wear leveling from 1 to 10. 1 means no wear leveling is used.
     */
    uint8_t wearLevelingFactor;

    /**
     * If enabled (1 - enabled, 0 - disabled), a checksum
     * (stored in a row) is calculated on each row of data,
     * while a redundant copy of Em_EEPROM is stored in another location.
     * When data is read, first the checksum is checked. If that checksum is bad,
     * and the redundant copy's checksum is good, the copy is restored.
     */
    uint8_t redundantCopy;

    /**
     * If enabled (1 - enabled, 0 - disabled), the blocking writes
     * to nvm are used in the design. Otherwise, non-blocking nvm writes
     * are used. From the user's perspective, the behavior of blocking and
     * non-blocking writes are the same - the difference is that the
     * non-blocking writes do not block the interrupts.
     *
     * \note Non-blocking nvm write is only supported on limited device list:
     * PSOC Control PSC3/PSC3M8, PSOC6, XMC7xxx, and T2G-B-H. For other devices,
     * the blocking write is used by default.
     *
     * See section 'Non-Blocking Operation' in README.md for details on the
     * interrupt requirements when using non-blocking operation.
     */
    uint8_t blockingWrite;

    /**
     * The address of the nvm storage.
     * The storage start address is filled to the Emulated EEPROM
     * configuration structure and then the structure is passed
     * to the Cy_Em_EEPROM_Init() function.
     */
    uint32_t userFlashStartAddr;
} cy_stc_eeprom_config_t;

/** Em_EEPROM new configuration structure */
typedef struct
{
    /**
     * The logical size of data in bytes to store in the Em_EEPROM.
     * The size is rounded up to a full Em_EEPROM row size physically.
     * The row size is specific for a device family.
     * Refer to the specific PSOC device datasheet.<br>
     * Note this size is often smaller than the total amount of nvm used
     * for the Em_EEPROM storage. The Em_EEPROM storage size depends on
     * the Em_EEPROM configuration and can be bigger because increasing
     * nvm endurance (wear-leveling) and restoring corrupted data from
     * a redundant copy. See section 'Storage Variable Location and Size' in README.md
     * for size calculation equations.
     */
    uint32_t eepromSize;

    /**
     * Simple mode, when enabled (1 - enabled, 0 - disabled), means no
     * additional service information is stored by the Em_EEPROM middleware
     * like checksums, headers, a number of writes, etc.
     * Data is stored directly by the specified address.
     * The size of Em_EEPROM storage is equal to the number of
     * byte specified in the eepromSize parameter rounded up to a full row
     * size. The wear leveling and
     * redundant copy features are disabled, i.e. wearLevelingFactor and
     * redundantCopy parameters are ignored.
     */
    uint8_t simpleMode;

    /**
     * The higher the factor is, the more nvm is used,
     * but a higher number of erase/write cycles can be done on Em_EEPROM.
     * Multiply this number by the datasheet write endurance spec to determine
     * the max of write cycles.<br>
     * The amount of wear leveling from 1 to 10. 1 means no wear leveling is used.
     */
    uint8_t wearLevelingFactor;

    /**
     * If enabled (1 - enabled, 0 - disabled), a checksum
     * (stored in a row) is calculated on each row of data,
     * while a redundant copy of Em_EEPROM is stored in another location.
     * When data is read, first the checksum is checked. If that checksum is bad,
     * and the redundant copy's checksum is good, the copy is restored.
     */
    uint8_t redundantCopy;

    /**
     * If enabled (1 - enabled, 0 - disabled), the blocking writes
     * to nvm are used in the design. Otherwise, non-blocking nvm writes
     * are used. From the user's perspective, the behavior of blocking and
     * non-blocking writes are the same - the difference is that the
     * non-blocking writes do not block the interrupts.
     *
     * \note Non-blocking nvm write is only supported on limited device list:
     * PSOC Control PSC3/PSC3M8, PSOC6, XMC7xxx, and T2G-B-H. For other devices,
     * the blocking write is used by default.
     *
     * See section 'Non-Blocking Operation' in README.md for details on the
     * interrupt requirements when using non-blocking operation.
     */
    uint8_t blockingWrite;

    /**
     * The address of the nvm storage.
     * The storage start address is filled to the Emulated EEPROM
     * configuration structure and then the structure is passed
     * to the Cy_Em_EEPROM_Init() function.
     */
    uint32_t userNvmStartAddr;
} cy_stc_eeprom_config2_t;

/**
 * The Em_EEPROM context data structure. It is used to store specific
 * Em_EEPROM context data.
 * Do not modify the context structure since it may cause unexpected behavior
 * of the Cy_Em_EEPROM functions that rely on this context structure.
 */
typedef struct
{
    /** The number of nvm rows allocated for the Em_EEPROM excluding the number
     * of rows allocated for wear-leveling and a redundant copy overhead.
     */
    uint16_t numberOfRows;

    /** The size of a EEPROM row in bytes, equal to the program size
     * of the NVM or a multiple of it that is >= than CY_EM_EEPROM_MINIMUM_ROW_SIZE
     * for devices whose program size is too small.
     */
    uint16_t rowSize;

    /** The size of a EEPROM sector in bytes, equal to the erase size
     * of the NVM. It is larger or equal to program size.
     */
    uint32_t secSize;

    /** The number of bytes to store in Em_EEPROM. */
    uint32_t eepromSize;

    /** The amount of wear leveling from 1 to 10. 1 means no wear leveling is used. */
    uint8_t wearLevelingFactor;

    /** No headers configured */
    uint8_t simpleMode;

    /** If not zero, a redundant copy of the Em_EEPROM is included. */
    uint8_t redundantCopy;

    /** If not zero, a blocking write to nvm is used. Otherwise,
     * a non-blocking write is used. This parameter is used only for PSOC 6.
     */
    uint8_t blockingWrite;

    /** The start address for the Em_EEPROM memory in nvm. */
    uint32_t userNvmStartAddr;

    /** The number of user's data bytes in one row for non simple mode.
     * It is equal to half the row size as in non simple mode half of
     * the row is reserved for the headers data portion and the other
     * half is reserved for the actual user's data. */
    uint16_t byteInRow;

    /** Defines the length of data that can be stored
     * in the Em_EEPROM header.
     */
    uint16_t headerDataLength;

    /** The pointer to the last written row. */
    uint32_t* ptrLastWrittenRow;

    /**
     * The block storage object holding the information on prototypes for
     * the necessary operations
     */
    const mtb_block_storage_t* bd;
} cy_stc_eeprom_context_t;



/*******************************************************************************
*        Function Prototypes
*******************************************************************************/
/** Initializes the Emulated EEPROM library by filling the context structure.
 *
 * This function is called by the application program prior to calling any
 * other function of the Em_EEPROM middleware.
 *
 * Do not modify the context structure after it is filled with this function.
 * Modification of the context structure may cause unexpected behavior
 * of the Cy_Em_EEPROM functions that rely on this context structure.
 *
 * This function does the following:
 * 1. Checks the provided configuration for correctness.
 * 2. Copies the provided configuration into the context structure.
 *
 * @param[in]   config       The pointer to the configuration structure
 *                           Configuration to use to initialize the em_eeprom object
 * @param[out]  context          Pointer to a em_eeprom object. The caller must allocate the memory
 *                           for this object but the init function will initialize its contents.
 *                           The pointer to the Em_EEPROM context structure to be filled by the
 * function.
 * @param[in]   block_device Block device interface for the underlying memory to be used.
 *
 * @return Result of the initialization operation.
 * - CY_RSLT_SUCCESS   - The operation is successfully completed.
 * - MTB_EM_EEPROM_BAD_PARAM - The input parameter is invalid.
 * - MTB_EM_EEPROM_BAD_DATA  - The configuration is incorrect.
 */
cy_en_em_eeprom_status_t Cy_Em_EEPROM_Init_BD(
    const cy_stc_eeprom_config2_t* config,
    cy_stc_eeprom_context_t* context,
    mtb_block_storage_t* block_device);



/**
 * This function was the entry point up until version 2.20. All devices supported
 * (PSOC6 based devices, PSOC4 based devices, XMC7xxx and T2G-B-H based devices)
 * will continue to work as expected when using this function, however it still introduces a
 * distinction between memory architectures that should not be a concern for the Em_EEPROM library
 * Users should start using Cy_Em_EEPROM_Init_BD instead by defining a block storage device
 * beforehand. Using this function on version 2.30 onwards will imply that all devices use block
 * storage implementation based on cyhal_nvm except for cat2 devices which will use block storage
 * implementation specific to cat2 devices.
 * This function is kept for backwards compatibility.
 *
 * Initializes the Emulated EEPROM library by filling the context structure.
 *
 *
 * This function is called by the application program prior to calling any
 * other function of the Em_EEPROM middleware.
 *
 * This function does the following:
 * 1. Selects the appropriate block storage solution based on the DUT
 * 2. Calls Cy_Em_EEPROM_Init_BD
 *
 * @param[in]   config       The pointer to the configuration structure
 *                           Configuration to use to initialize the em_eeprom object
 * @param[out]  context          Pointer to a em_eeprom object. The caller must allocate the memory
 *                           for this object but the init function will initialize its contents.
 *                           The pointer to the Em_EEPROM context structure to be filled by the
 * function.
 *
 * @return Result of the initialization operation.
 * - CY_RSLT_SUCCESS   - The operation is successfully completed.
 * - MTB_EM_EEPROM_BAD_PARAM - The input parameter is invalid.
 * - MTB_EM_EEPROM_BAD_DATA  - The configuration is incorrect.
 */
cy_en_em_eeprom_status_t Cy_Em_EEPROM_Init(
    const cy_stc_eeprom_config_t* config,
    cy_stc_eeprom_context_t* context);



/** Reads data from a specified location. This function takes a logical
 * Em_EEPROM address, converts it to an actual physical address where
 * data is stored and returns the data to the user.
 *
 * This function uses a buffer of the flash row size to perform the read
 * operation. For the size of the row, refer to the specific PSOC device
 * datasheet.
 *
 * There are restrictions on using the read-while-write (RWW) feature for
 * EEPROM emulation. There are also multiple constraints for blocking and
 * nonblocking flash operations, relating to interrupts, power modes,
 * IPC usage, etc. Refer to the "Flash (Flash System Routine)" section of
 * the CAT1 Peripheral Driver Library (mtb-pdl-cat1) API Reference Manual.<br>
 *
 * @param[in]       addr       The logical start address in the Em_EEPROM storage to start reading
 * data
 * from.
 * @param[out]      eepromData The pointer to a user array to write data to.
 * @param[in,out]   size       The amount of data to read in bytes.
 * @param[in]       context    Pointer to a em_eeprom object
 *
 *  @return Result of the read operation.
 * - CY_RSLT_SUCCESS      - The operation is successfully completed.
 * - MTB_EM_EEPROM_BAD_CHECKSUM - One of the row the data read from contains bad checksum.
 * - MTB_EM_EEPROM_REDUNDANT_COPY_USED - Some data were read from redundant copy.
 * - MTB_EM_EEPROM_BAD_PARAM    - The function input parameter is invalid.
 */
cy_en_em_eeprom_status_t Cy_Em_EEPROM_Read(
    uint32_t addr,
    void* eepromData,
    uint32_t size,
    cy_stc_eeprom_context_t* context);

/**
 * Writes data to a specified location. This function takes the
 * logical Em_EEPROM address and converts it to an actual physical address
 * and writes data there. If wear leveling is enabled, the writing process
 * uses the wear leveling techniques.
 *
 * This is a blocking function and it does not return until the write
 * operation is completed. The user's application program cannot enter
 * Hibernate mode until the write is completed.
 * The write operation is allowed in CPU Sleep mode.
 * Do not reset your device during the flash operation, including the XRES pin,
 * a software reset, and watchdog reset sources. Also, configure low-voltage
 * detect circuits to generate an interrupt instead of a reset. Otherwise,
 * portions of flash may undergo unexpected changes.
 *
 * This function uses a buffer of the row size to perform the write
 * operation. All write operations are done for row size.
 * For the size of the row, refer to the specific device
 * datasheet.
 *
 * If the blocking write option is used (PSOC 6), and write or erase operations
 * are performed by CM4, the user's code on CM0P and CM4 is blocked until the
 * operations are completed. If the operations are performed by CM0P, the
 * user's code on CM4 is not blocked and the user code's on CM0P is blocked
 * until the operation is completed. Plan your task allocation accordingly.
 *
 * There are restrictions on using the read-while-write (RWW) feature for
 * EEPROM emulation. There are also multiple constraints for blocking and
 * nonblocking flash operations, relating to interrupts, Power modes,
 * IPC usage, etc. Refer to the "Flash (Flash System Routine)" section of
 * the CAT1 Peripheral Driver Library (mtb-pdl-cat1) API Reference Manual.<br>
 *
 * @param[in] addr        The logical start address in the Em_EEPROM storage to start writing data
 * to.
 * @param[in] eepromData  Pointer to the start of the data to be writtento Em_EEPROM.
 * @param[in] size        Total size of the data in bytes.
 * @param[in] context     Pointer to a em_eeprom object
 *
 * @return Result of the write operation.
 * - CY_RSLT_SUCCESS      - The operation is successfully completed.
 * - MTB_EM_EEPROM_BAD_CHECKSUM - One of the row the data read from contains bad checksum.
 * - MTB_EM_EEPROM_REDUNDANT_COPY_USED - Some data were read from redundant copy.
 * - MTB_EM_EEPROM_WRITE_FAIL   - The write operation is failed.
 * - MTB_EM_EEPROM_BAD_PARAM    - The function input parameter is invalid.
 */
cy_en_em_eeprom_status_t Cy_Em_EEPROM_Write(
    uint32_t addr,
    const void* eepromData,
    uint32_t size,
    cy_stc_eeprom_context_t* context);


/** This function erases the entire content of Em_EEPROM.
 *
 * Erased values are all zeros.
 *
 * In simple mode (see section 'Simple Mode' in README.md) the function just erases the
 * entire content of Em_EEPROM.
 *
 * When Simple Mode is disabled, the function first performs one write
 * operation to keep the number of writes completed so far and then erases
 * all rest content of Em_EEPROM memory. So, the Cy_Em_EEPROM_NumWrites()
 * function returns the correct value.
 * Refer to the Cy_Em_EEPROM_Write() function for the side effect of the
 * write operation. If the write operation is failed then corresponding
 * status is returned and no erase operation is executed.
 *
 * This function uses a buffer of the flash row size to perform the erase
 * operation. For the size of the row, refer to the specific PSOC device
 * datasheet.
 *
 * This is a blocking function and it does not return until the erase
 * operation is completed. The user's application program cannot enter
 * Hibernate mode until the write is completed.
 * The write operation is allowed in CPU Sleep mode.
 * Do not reset your device during the flash operation, including the XRES pin,
 * a software reset, and watchdog reset sources. Also, configure low-voltage
 * detect circuits to generate an interrupt instead of a reset. Otherwise,
 * portions of flash may undergo unexpected changes.
 *
 * If the blocking write option is used, and write or erase operations are
 * performed by CM4, the user's code on CM0P and CM4 is blocked until the
 * operations are completed. If the operations are performed by CM0P, the
 * user's code on CM4 is not blocked and the user code's on CM0P is blocked
 * until the operation is completed. Plan your task allocation accordingly.
 *
 * There are restrictions on using the read-while-write (RWW) feature for
 * EEPROM emulation. There are also multiple constraints for blocking and
 * nonblocking flash operations, relating to interrupts, power mode,
 * IPC usage, etc. Refer to the "Flash (Flash System Routine)" section of
 * the CAT1 Peripheral Driver Library (mtb-pdl-cat1) API Reference Manual.<br>
 * Also, refer to the section 'Limitations and Restrictions' in README.md for
 * the different Em_EEPROM middleware restrictions and limitations.
 *
 * @param[in] context         Pointer to a em_eeprom object
 *
 * @return Result of the erase operation.
 * - CY_RSLT_SUCCESS    - The operation is successfully completed.
 * - CY_EM_EEPROM_WRITE_FAIL - The either Write or Erase operation is failed.
 */
cy_en_em_eeprom_status_t Cy_Em_EEPROM_Erase(cy_stc_eeprom_context_t* context);

/** Returns the number of the Em_EEPROM Writes completed so far.
 *
 * This function returns zero in simple mode (see section 'Simple Mode' in README.md) since
 * the number of writes is not available in this case.
 *
 * Use this function to monitor the flash memory endurance. The higher
 * the value, the less the flash memory endurance.
 * Refer to the datasheet write endurance spec to determine the maximum
 * number of write cycles. If the Wear Leveling feature is enabled, then the
 * maximum of write cycles is increased by
 * \ref cy_stc_eeprom_context_t.wearLevelingFactor.
 *
 * The returned number does not include the number of writes into the
 * redundant copy if enabled.
 *
 * This function does the following:
 * 1. Returns the number of writes stored in the the last written row if the
 *    checksum of the row is correct.
 * 2. If the checksum of the last written row is invalid and the redundant copy
 *    feature is enabled, then the row checksum of the redundant copy is verified
 *    for correctness and corresponding number of writes is returned.
 * 3. If the checksum of the last written row is incorrect, then a special
 *    algorithm of searching the last written is executed and a corresponding
 *    number of writes is reported.
 * 4. If the algorithm is unable to find the last written row (there is no row
 *    with the correct checksum), then zero is returned.
 *
 * @param[in] context         Pointer to a em_eeprom object
 *
 * @return The number of writes performed to the Em_EEPROM.
 */
uint32_t Cy_Em_EEPROM_NumWrites(cy_stc_eeprom_context_t* context);


/** \} group_emeeprom */

/*******************************************************************************
* Internal Macro Definitions
*******************************************************************************/

/* CRC-8 constants */
#define CY_EM_EEPROM_CRC8_POLYNOM                   ((uint8_t)(0x31u))
#define CY_EM_EEPROM_CRC8_POLYNOM_LEN               (8u)
#define CY_EM_EEPROM_CRC8_SEED                      (0xFFu)
#define CY_EM_EEPROM_CRC8_XOR_VAL                   ((uint8_t) (0x80u))

#define CY_EM_EEPROM_CALCULATE_CRC8(crc)            \
                  ((CY_EM_EEPROM_CRC8_XOR_VAL == ((crc) & CY_EM_EEPROM_CRC8_XOR_VAL)) ? \
                  ((uint8_t)(((uint8_t)((uint8_t)((crc) << 1u))) ^ CY_EM_EEPROM_CRC8_POLYNOM)) : ((uint8_t)((crc) << 1u)))

/* Offsets for the 32-bit RAM buffer addressing */
#define CY_EM_EEPROM_HEADER_CHECKSUM_OFFSET_U32     (0u)
#define CY_EM_EEPROM_HEADER_SEQ_NUM_OFFSET_U32      (1u)
#define CY_EM_EEPROM_HEADER_ADDR_OFFSET_U32         (2u)
#define CY_EM_EEPROM_HEADER_LEN_OFFSET_U32          (3u)
#define CY_EM_EEPROM_HEADER_DATA_OFFSET_U32         (4u)

/* The same offsets as above used for direct memory addressing. */
#define CY_EM_EEPROM_HEADER_CHECKSUM_OFFSET         (0u)
#define CY_EM_EEPROM_HEADER_ADDR_OFFSET             (8u)
#define CY_EM_EEPROM_HEADER_LEN_OFFSET              (12u)
#define CY_EM_EEPROM_HEADER_DATA_OFFSET             (16u)

#define CY_EM_EEPROM_U32                            (4u)

/* The maximum wear-leveling value. */
#define CY_EM_EEPROM_MAX_WEAR_LEVELING_FACTOR       (10u)

/* The maximum allowed nvm row Write/Erase operation duration. */
#define CY_EM_EEPROM_MAX_WRITE_DURATION_MS          (50u)

#define CY_EM_EEPROM_MINIMUM_ROW_SIZE               (0x80)

#if (CPUSS_FLASHC_ECT == 1)
#define CY_EM_EEPROM_MAXIMUM_ROW_SIZE               (0x800)
#else
#define CY_EM_EEPROM_MAXIMUM_ROW_SIZE               (0x200)
#endif


/* BACK COMPATIBILITY MACROS AND DEFINES START
 * These defines are device dependent and kept for backward compatibility.
 * However since CY_EM_EEPROM_GET_PHYSICAL_SIZE is needed to declare the
 * emeprom storage this section is still present and update for all Flash
 * based devices
 */
#define CY_EM_EEPROM_VERSION_MAJOR                  (CY_EM_EEPROM_MW_VERSION_MAJOR)
#define CY_EM_EEPROM_VERSION_MINOR                  (CY_EM_EEPROM_MW_VERSION_MINOR)


#if (CPUSS_FLASHC_ECT == 1)

#ifdef EEPROM_LARGE_SECTOR_WFLASH

#define CY_EM_EEPROM_FLASH_SIZEOF_ROW       (CY_WORK_LES_SIZE_IN_BYTE)

#else /* EEPROM_LARGE_SECTOR_WFLASH */

#define CY_EM_EEPROM_FLASH_SIZEOF_ROW       (CY_WORK_SES_SIZE_IN_BYTE)

#endif /* EEPROM_LARGE_SECTOR_WFLASH */

#elif ((CY_IP_MXS22RRAMC_INSTANCES) > 0)
//The information analog to other devices would be CY_RRAM_BLOCK_SIZE_BYTES
//but that is too small to be used for EEPROM so in RRAM based devices the
//row size is computed as the minimum row size or the closest multiple of
//CY_RRAM_BLOCK_SIZE_BYTES that is bigger than minimum row size.
#define CY_EM_EEPROM_FLASH_SIZEOF_ROW       \
    ((((CY_EM_EEPROM_MINIMUM_ROW_SIZE - 1) / CY_RRAM_BLOCK_SIZE_BYTES) + 1) * CY_RRAM_BLOCK_SIZE_BYTES)

#else /* For PSOC 4/6 and all Flash based devices */

/** Defines the size of a flash row */
#define CY_EM_EEPROM_FLASH_SIZEOF_ROW       (CY_FLASH_SIZEOF_ROW)

#endif /* (CPUSS_FLASHC_ECT == 1) */

/** Defines the maximum data length that can be stored in one flash row */
#define CY_EM_EEPROM_EEPROM_DATA_LEN(simpleMode) \
                (CY_EM_EEPROM_FLASH_SIZEOF_ROW / (2uL - (simpleMode)))

/** The number of flash rows required to create an Em_EEPROM of dataSize */
#define CY_EM_EEPROM_GET_NUM_ROWS_IN_EEPROM(dataSize, simpleMode) \
                ((((dataSize) - 1uL) / (CY_EM_EEPROM_EEPROM_DATA_LEN(simpleMode))) + 1uL)

/** Defines the size of flash without wear leveling and redundant copy overhead */
#define CY_EM_EEPROM_GET_NUM_DATA(dataSize, simpleMode) \
                (CY_EM_EEPROM_GET_NUM_ROWS_IN_EEPROM(dataSize, simpleMode) * \
                CY_EM_EEPROM_FLASH_SIZEOF_ROW)

/** Returns the size of flash allocated for Em_EEPROM including wear leveling and a redundant copy
   overhead */
#define CY_EM_EEPROM_GET_PHYSICAL_SIZE(dataSize, simpleMode, wearLeveling, redundantCopy) \
                (CY_EM_EEPROM_GET_NUM_DATA(dataSize, simpleMode) * \
                ((((1uL - (simpleMode)) * (wearLeveling)) * ((redundantCopy) + 1uL)) + (simpleMode)))

/* BACK COMPATIBILITY MACROS AND DEFINES END
 */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CY_EM_EEPROM_H */


/* [] END OF FILE */
