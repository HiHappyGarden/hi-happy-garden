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

#include "hhg-driver/hardware.hpp"
#include "hhg-driver/os-config.hpp"
#include "hhg-config.h"
#include "pico/pico-uart.hpp"
#include "pico/pico-time.hpp"
#include "pico/pico-fsio.hpp"
#include "pico/pico-i2c.hpp"
#include "pico/pico-lcd.hpp"
#include "pico/pico-rotary-encored.hpp"


using namespace os;
using namespace hhg::iface;

namespace hhg::driver
{
inline namespace v1
{

namespace
{

constexpr const char APP_TAG[] = "HARDWARE";

}

hardware::hardware(class error** error) OS_NOEXCEPT
: time(new hhg::driver::pico_time)
, uart(new hhg::driver::pico_uart)
, fsio(new hhg::driver::pico_fsio)
, i2c(new hhg::driver::pico_i2c)
, lcd( new hhg::driver::pico_lcd(20, 4))
, rotary_encoder(new hhg::driver::pico_rotary_encoder)
{
    if(time.get() == nullptr && error)
    {
        *error = OS_ERROR_BUILD("io(new hhg::driver::stm32_timer) no mem.", error_type::OS_ENOMEM);
        OS_ERROR_PTR_SET_POSITION(*error);
        return;
    }

    if(uart.get() == nullptr && error)
	{
        *error = OS_ERROR_BUILD("io(new hhg::driver::pico_uart) no mem.", error_type::OS_ENOMEM);
        OS_ERROR_PTR_SET_POSITION(*error);
        return;
    }

    if(fsio.get() == nullptr && error)
	{
        *error = OS_ERROR_BUILD("io(new hhg::driver::pico_fsio) no mem.", error_type::OS_ENOMEM);
        OS_ERROR_PTR_SET_POSITION(*error);
        return;
    }

    if(i2c.get() == nullptr && error)
    {
        *error = OS_ERROR_BUILD("io(new hhg::driver::pico_i2c) no mem.", error_type::OS_ENOMEM);
        OS_ERROR_PTR_SET_POSITION(*error);
        return;
    }

    if(lcd.get() == nullptr && error)
    {
        *error = OS_ERROR_BUILD("io(new hhg::driver::lcd) no mem.", error_type::OS_ENOMEM);
        OS_ERROR_PTR_SET_POSITION(*error);
        return;
    }
}


    struct test : rotary_encoder::event
    {
        test() = default;
        ~test() override = default;

        int32_t idx = 0;

        void on_event(bool ccw, bool cw, bool click) OS_NOEXCEPT override
        {
            if(ccw)
            {
                idx--;
            }
            else if(cw)
            {
                idx++;
            }

            OS_LOG_INFO(APP_TAG, "idx: %ld click:%u", idx, click);
        }

    };

    test test_one;

os::exit hardware::init(error** error) OS_NOEXCEPT
{
	if(os_config_init() == os::exit::KO)
	{
		return exit::KO;
	}

    OS_LOG_PRINTF("--------------------------\r\n");
    OS_LOG_PRINTF("%s %s\r\n", HHG_NAME, HHG_VER);
    OS_LOG_PRINTF("--------------------------\r\n");

    OS_LOG_INFO(APP_TAG, "Init Time");
    auto time_init = reinterpret_cast<hhg::iface::time_init *>(time.get());
    if(time_init == nullptr)
    {
        if(error && *error)
        {
            *error = OS_ERROR_APPEND(*error, " reinterpret_cast<hhg::iface::time_init *>(time.get()) fail.", error_type::OS_EFAULT);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }
    if(time_init->init(error) == exit::KO)
    {
        if(error && *error)
        {
            *error = OS_ERROR_APPEND(*error, "io::init() fail.", error_type::OS_EFAULT);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }
    OS_LOG_INFO(APP_TAG, "Init Time - OK");

	OS_LOG_INFO(APP_TAG, "Init UART");
	if(uart->init(error) == exit::KO)
	{
		if(error && *error)
		{
	        *error = OS_ERROR_APPEND(*error, "io::init() fail.", error_type::OS_EFAULT);
	        OS_ERROR_PTR_SET_POSITION(*error);
		}
		return exit::KO;
	}
	OS_LOG_INFO(APP_TAG, "Init UART - OK");

	OS_LOG_INFO(APP_TAG, "Init FS IO");
	if(fsio->init(error) == exit::KO)
	{
		if(error && *error)
		{
	        *error = OS_ERROR_APPEND(*error, "fsio::init() fail.", error_type::OS_EFAULT);
	        OS_ERROR_PTR_SET_POSITION(*error);
		}
		return exit::KO;
	}
	OS_LOG_INFO(APP_TAG, "Init FS IO - OK");

//    OS_LOG_INFO(APP_TAG, "Init I2C");
//    if(i2c->init(error) == exit::KO)
//    {
//        if(error && *error)
//        {
//            *error = OS_ERROR_APPEND(*error, "i2c::init() fail.", error_type::OS_EFAULT);
//            OS_ERROR_PTR_SET_POSITION(*error);
//        }
//        return exit::KO;
//    }
//    OS_LOG_INFO(APP_TAG, "Init I2C - OK");

//    OS_LOG_INFO(APP_TAG, "Init LCD");
//    if(lcd->init(error) == exit::KO)
//    {
//        if(error && *error)
//        {
//            *error = OS_ERROR_APPEND(*error, "lcd::init() fail.", error_type::OS_EFAULT);
//            OS_ERROR_PTR_SET_POSITION(*error);
//        }
//        return exit::KO;
//    }
//    OS_LOG_INFO(APP_TAG, "Init LCD - OK");

    OS_LOG_INFO(APP_TAG, "Init Rotary Encoder");
    if(rotary_encoder->init(error) == exit::KO)
    {
        if(error && *error)
        {
            *error = OS_ERROR_APPEND(*error, "rotary_encoder::init() fail.", error_type::OS_EFAULT);
            OS_ERROR_PTR_SET_POSITION(*error);
        }
        return exit::KO;
    }
    OS_LOG_INFO(APP_TAG, "Init Init Rotary Encoder - OK");


    rotary_encoder->set_on_event(&test_one, &rotary_encoder::event::on_event);

	return exit::OK;
}

const string<128>& hardware::get_info() OS_NOEXCEPT
{
	static string<128> ret;

	return ret;
}

}
}

