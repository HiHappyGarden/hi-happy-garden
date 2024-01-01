/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023/2024  Antonio Salsi <passy.linux@zresa.it>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***************************************************************************/

#ifndef __DRIVER_LPUART_H_
#define _DRIVER_LPUART_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct __UART_HandleTypeDef;
typedef struct __UART_HandleTypeDef UART_HandleTypeDef;

uint8_t driver_lpuart_init(void);

uint8_t driver_lpuart_transmit(const uint8_t* data, uint16_t size);

void driver_lpuart_register(UART_HandleTypeDef* hlpuart1);


void driver_lpuart_rx_callback(void);
void driver_lpuart_tx_callback(void);
void driver_lpuart_tx_complete_callback(void);
void driver_lpuart_error_callback(void);

#ifdef __cplusplus
 }
#endif


#endif /* _DRIVER_LPUART_H_ */
