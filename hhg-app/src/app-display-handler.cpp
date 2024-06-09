/***************************************************************************
 *
 * Hi Happy Garden
 * Copyright (C) 2023/2024 Antonio Salsi <passy.linux@zresa.it>
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

#include "hhg-app/app-display-handler.hpp"
using namespace os;
using hhg::iface::lcd;
using hhg::iface::button;
using hhg::iface::rotary_encoder;

#include "assets/font_5x8.hpp"
#include "assets/font_8x8.hpp"
#include "assets/ic_wifi_on.hpp"
#include "assets/ic_wifi_off.hpp"


namespace hhg::app
{
inline namespace v1
{

    app_display_handler::app_display_handler(const iface::lcd::ptr &lcd, const iface::rotary_encoder::ptr &rotary_encoder, const iface::button::ptr &button)
    : lcd(lcd)
    , rotary_encoder(rotary_encoder)
    , button(button)
    {

    }

    os::exit app_display_handler::init(os::error **error) OS_NOEXCEPT
    {

        button->set_on_button_click(this, &button::event::on_button_click);
        rotary_encoder->set_on_rotary_encoder_event(this, &rotary_encoder::event::on_rotary_encoder_event);

        printFrame(false);

        lcd->send_buffer();

        return os::exit::OK;
    }

    void app_display_handler::on_button_click(iface::button::status status) OS_NOEXCEPT
    {

    }

    void app_display_handler::on_rotary_encoder_event(bool ccw, bool cw, bool click) OS_NOEXCEPT
    {

    }

    void app_display_handler::printFrame(bool wifi, time_t now) const OS_NOEXCEPT
    {
        auto [display_width, display_height] = lcd->get_size();

        uint16_t ic_wifi_width = 0;
        uint16_t ic_wifi_height = 0;
        const uint8_t* ic_wifi = nullptr;
        uint8_t ic_wifi_size = 0;

        if(wifi)
        {
            ic_wifi_width = ic_wifi_on_width;
            ic_wifi_height = ic_wifi_on_height;
            ic_wifi = ic_wifi_on;
            ic_wifi_size = sizeof ic_wifi_on;
        }
        else
        {
            ic_wifi_width = ic_wifi_off_width;
            ic_wifi_height = ic_wifi_off_height;
            ic_wifi = ic_wifi_off;
            ic_wifi_size = sizeof ic_wifi_off;
        }
        lcd->set_bitmap_image(3 , 0, ic_wifi_width, ic_wifi_height, ic_wifi, ic_wifi_size);

        lcd->set_rect(0, 11, 132, 1, lcd::write_mode::ADD);

        lcd->set_str("ciao antonio", 0, 30, font_8x8, sizeof font_8x8);

    }


}
}