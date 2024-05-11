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



#pragma once

#include "hhg-iface/button.hpp"

#include <pico/types.h>

namespace hhg::driver
{
inline namespace v1
{

class pico_button : public hhg::iface::button
{
    event *obj = nullptr;
    hhg::iface::button::event::callback callback = nullptr;
public:

    enum pin : uint
    {
        PIN = 26
    };

    pico_button();
    ~pico_button() override;
    OS_NO_COPY_NO_MOVE(pico_button);

    inline void set_on_event(event *obj, event::callback callback) OS_NOEXCEPT override
    {
        this->obj = obj;
        this->callback = callback;
    }

    os::exit init(os::error **error) OS_NOEXCEPT override;

};

}
}