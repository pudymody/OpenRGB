/*-----------------------------------------*\
|  i2c_smbus.h                              |
|                                           |
|  Definitions and types for SMBUS drivers  |
|                                           |
|  Adam Honse (CalcProgrammer1) 8/8/2018    |
|  Portions based on Linux source code      |
|  GNU GPL v2                               |
\*-----------------------------------------*/

#ifndef I2C_SMBUS_H
#define I2C_SMBUS_H

#include <atomic>
#include <thread>
#include <condition_variable>
#include <mutex>

typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned int    u32;
typedef int             s32;

#ifdef WIN32

//Data for SMBus Messages
#define I2C_SMBUS_BLOCK_MAX     32

union i2c_smbus_data
{
    u8          byte;
    u16         word;
    u8          block[I2C_SMBUS_BLOCK_MAX + 2];
};

#else /* WIN32 */

#include <linux/i2c.h>

#endif  /* WIN32 */

// i2c_smbus_xfer read or write markers
#define I2C_SMBUS_READ  1
#define I2C_SMBUS_WRITE 0

// SMBus transaction types (size parameter in the above functions)
#define I2C_SMBUS_QUICK             0
#define I2C_SMBUS_BYTE              1
#define I2C_SMBUS_BYTE_DATA         2
#define I2C_SMBUS_WORD_DATA         3
#define I2C_SMBUS_PROC_CALL         4
#define I2C_SMBUS_BLOCK_DATA        5
#define I2C_SMBUS_I2C_BLOCK_BROKEN  6
#define I2C_SMBUS_BLOCK_PROC_CALL   7           /* SMBus 2.0 */
#define I2C_SMBUS_I2C_BLOCK_DATA    8

class i2c_smbus_interface
{
public:
    char device_name[512];

    int port_id;
    int pci_device;
    int pci_vendor;
    int pci_subsystem_device;
    int pci_subsystem_vendor;

    i2c_smbus_interface();
    virtual ~i2c_smbus_interface() = default;

    void i2c_smbus_thread_function();

    //Functions derived from i2c-core.c
    s32 i2c_smbus_write_quick(u8 addr, u8 value);
    s32 i2c_smbus_read_byte(u8 addr);
    s32 i2c_smbus_write_byte(u8 addr, u8 value);
    s32 i2c_smbus_read_byte_data(u8 addr, u8 command);
    s32 i2c_smbus_write_byte_data(u8 addr, u8 command, u8 value);
    s32 i2c_smbus_read_word_data(u8 addr, u8 command);
	s32 i2c_smbus_write_word_data(u8 addr, u8 command, u16 value);
    s32 i2c_smbus_read_block_data(u8 addr, u8 command, u8 *values);
	s32 i2c_smbus_write_block_data(u8 addr, u8 command, u8 length, const u8 *values);
    s32 i2c_smbus_read_i2c_block_data(u8 addr, u8 command, u8 length, u8 *values);
    s32 i2c_smbus_write_i2c_block_data(u8 addr, u8 command, u8 length, const u8 *values);

    s32 i2c_smbus_xfer_call(u8 addr, char read_write, u8 command, int size, i2c_smbus_data* data);

    //Virtual function to be implemented by the driver
    virtual s32 i2c_smbus_xfer(u8 addr, char read_write, u8 command, int size, i2c_smbus_data* data) = 0;

private:
    std::thread *       i2c_smbus_thread;

    std::atomic<bool>       i2c_smbus_start;
    std::condition_variable i2c_smbus_start_cv;
    std::mutex              i2c_smbus_start_mutex;

    std::atomic<bool>       i2c_smbus_done;
    std::condition_variable i2c_smbus_done_cv;
    std::mutex              i2c_smbus_done_mutex;

    std::mutex              i2c_smbus_xfer_mutex;

    u8                  i2c_addr;
    char                i2c_read_write;
    u16                 i2c_command;
    int                 i2c_size;
    i2c_smbus_data*     i2c_data;
    s32                 i2c_ret;
};

#endif /* I2C_SMBUS_H */
