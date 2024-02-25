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

#include "hhg-app/app-data.hpp"
using hhg::iface::fs_io;
using hhg::iface::data_type;

#include "hhg-utils/hhg-utils.hpp"
using hhg::utils::crc32;

#include "osal/osal.hpp"
using namespace os;

#include <time.h>

namespace hhg::app
{
inline namespace v1
{

namespace
{

constexpr char APP_TAG[] = "APP CONFIG";

}

app_data::app_data(const fs_io::ptr& fsio) OS_NOEXCEPT
: fsio(fsio)
{

}

app_data::~app_data() = default;

inline os::exit app_data::init(os::error** error) OS_NOEXCEPT
{
	return load(nullptr, error);
}

void app_data::reset() OS_NOEXCEPT
{
	data = {};
}

os::exit app_data::store(os::error** error) const OS_NOEXCEPT
{
	data.crc = crc32(reinterpret_cast<uint8_t *>(&data), sizeof(data));
	return fsio->write(data_type::DATA, reinterpret_cast<const uint8_t *>(&data), sizeof(data), error);
}


os::exit app_data::load(app_data::on_vesrion_change on_vesrion_change, os::error** error) OS_NOEXCEPT
{
	auto local_data = static_cast<class data*>(os_malloc(sizeof(class data)));
	if(local_data == nullptr)
	{
		if(error)
		{
			*error = OS_ERROR_BUILD("app_config::load() out of memory.", error_type::OS_ENOMEM);
			OS_ERROR_PTR_SET_POSITION(*error);
		}
		return exit::KO;
	}


	if(fsio->read(data_type::DATA, reinterpret_cast<uint8_t *>(local_data), sizeof(class data), error) == exit::KO)
	{
		return exit::KO;
	}

	uint32_t original_crc = local_data->crc;
	local_data->crc = MAIGC;
	uint32_t crc = crc32(reinterpret_cast<uint8_t *>(local_data), sizeof(class data));
	local_data->crc = original_crc;

	if(crc != original_crc)
	{
		if(error)
		{
			*error = OS_ERROR_BUILD("app_config::load() crc error.", error_type::OS_ERCRC);
			OS_ERROR_PTR_SET_POSITION(*error);
		}
		return exit::KO;
	}

	if(local_data->magic != MAIGC)
	{
		if(error)
		{
			*error = OS_ERROR_BUILD("app_config::load() magic number error.", error_type::OS_EBADF);
			OS_ERROR_PTR_SET_POSITION(*error);
		}
		return exit::KO;
	}

	if(on_vesrion_change)
	{
		on_vesrion_change(local_data->version);
	}

	data = *local_data;

	os_free(local_data);

	return exit::OK;
}

os::exit app_data::load_defaut(os::error** error) OS_NOEXCEPT
{
	reset();
	return store(error);
}

bool app_data::get_schedule(time_t timestamp, struct schedule& schedule) OS_NOEXCEPT
{
	tm* now = gmtime(&timestamp);

	for(uint8_t idx_schedule = 0; idx_schedule < HHG_SCHEDULES_SIZE; idx_schedule++)
	{
		auto&& current_schedule = data.schedules[idx_schedule];
		if(current_schedule.status != status::ACTIVE)
		{
			continue;
		}

		if( (current_schedule.hour == static_cast<uint8_t>(schedule::NOT_SET) && current_schedule.minute == static_cast<uint8_t>(schedule::NOT_SET)) || current_schedule.hour == static_cast<uint8_t>(schedule::NOT_SET) )
		{
			continue;
		}

		if( (current_schedule.hour == now->tm_hour && current_schedule.minute == now->tm_min)
			|| (current_schedule.hour == now->tm_hour && current_schedule.minute == static_cast<uint8_t>(schedule::NOT_SET) && now->tm_min == 0)
		)
		{

			schedule = current_schedule;
			return true;
		}
	}

	return false;
}

} /* namespace driver */
} /* namespace hhg */
