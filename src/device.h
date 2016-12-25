// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2015 Intel Corporation. All Rights Reserved.

#pragma once

#include "backend.h"
#include "archive.h"
#include <chrono>
#include <memory>
#include <vector>
#include "hw-monitor.h"
#include "option.h"
#include "subdevice.h"

namespace rsimpl
{
    class device
    {
    public:
        device();

        virtual ~device() = default;

        unsigned int get_endpoints_count() const { return static_cast<unsigned int>(_endpoints.size()); }
        endpoint& get_endpoint(int subdevice)
        {
            return *_endpoints[subdevice];
        }

        rs_extrinsics get_extrinsics(int from, int to);

        virtual rs_intrinsics get_intrinsics(int subdevice, stream_profile profile) const = 0;

        float get_depth_scale() const { return _static_info.nominal_depth_scale; }

        virtual std::vector<uint8_t> send_receive_raw_data(const std::vector<uint8_t>& input)
        { 
            throw std::runtime_error(to_string() << __FUNCTION__ << " is not implemented for this device!"); 
        }

    protected:
        int add_endpoint(std::shared_ptr<endpoint> endpoint);

        uvc_endpoint& get_uvc_endpoint(int subdevice);

        void register_endpoint_info(int sub, std::map<rs_camera_info, std::string> camera_info);

        void declare_capability(supported_capability cap);

        void set_depth_scale(float scale);

    private:
        std::vector<std::shared_ptr<endpoint>> _endpoints;
        static_device_info _static_info;
        std::atomic<bool> _keep_fw_logger_alive;
        std::unique_ptr<std::thread> _fw_logger_thread;
    };
}
