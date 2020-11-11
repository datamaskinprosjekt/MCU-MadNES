#ifndef _SPI_H_
#define _SPI_H_

#include "spidrv.h"


#define count_ctrl 1 ///< The number of bytes to receive from a controller.
#define timeout_ms_ctrl 1 ///< NOTE: Should not time out even after 1ms.
                          /// Increase this definition if any problems arise.


SPIDRV_HandleData_t handle_data;
SPIDRV_Handle_t handle;

void setup_SPI();
void teardown_SPI();


/*************************************************************
 * @brief Helper function to receive controller input
 * Receives data from a controller and stores it in the
 * specified buffer.
 * 
 * @param buffer Pointer to data storage.
 *************************************************************/
void receive_ctrl_SPI(uint8_t* buffer);

#endif // _SPI_H_