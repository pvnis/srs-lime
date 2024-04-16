/*
 *
 * Copyright 2021-2024 Software Radio Systems Limited
 *
 * This file is part of srsRAN.
 *
 * srsRAN is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * srsRAN is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * A copy of the GNU Affero General Public License can be found in
 * the LICENSE file in the top-level directory of this distribution
 * and at http://www.gnu.org/licenses/.
 *
 */

#include <mutex>
#include <chrono>
#include <map>
#include <set>
#include <memory>

#include <limesuiteng/limesuiteng.hpp>
// #include <limesuite/SDRDevice.h>
// #include <limesuite/DeviceRegistry.h>
// #include <limesuite/LMS7002M.h>
// #include <limesuite/commonTypes.h>

class LimeHandle
{
public:
	mutable std::recursive_mutex accessMutex;

	// Getter for the device and Pointer Deref Op.
	lime::SDRDevice* dev() { return _dev; }
	operator lime::SDRDevice* () { return _dev; }

	// Number of devices, that are found
	unsigned count() { return devcnt; }

	// Destructors and constructors
	LimeHandle() = delete;
	LimeHandle(lime::DeviceHandle& devHandle);
	~LimeHandle();

	lime::SDRConfig& GetDeviceConfig() { return _dev_config; }
	lime::StreamConfig& GetStreamConfig() { return _stream_config; }	

	// Configuration Path (TODO: Take the configs from OAI and others as reference!)
	std::string& GetLMSConfPath() { return config_path; }

	// Get the device handle in srs from a limeDevice Handle
	static std::shared_ptr<LimeHandle> get(lime::DeviceHandle& devHandle);

	// Argument parsing for config.yaml arguments
	static bool split_args(std::string args, std::vector<std::pair<std::string, std::string>>& arg_list);

protected:
	lime::SDRDevice* _dev = NULL;
	lime::SDRConfig _dev_config;
	lime::StreamConfig _stream_config;
	std::string config_path = "";

	unsigned devcnt;

	static std::map<std::string, std::weak_ptr<LimeHandle>> s_created;
};
