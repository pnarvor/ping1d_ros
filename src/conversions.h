#ifndef _DEF_BLUE_PING1D_ROS_CONVERSIONS_H_
#define _DEF_BLUE_PING1D_ROS_CONVERSIONS_H_

#include <cstring>

#include <ros/ros.h>

#include <ping_protocol/messages/Ping1DMessages.h>
#include <blue_ping1d/Profile.h>

inline void copy_to_ros(blue_ping1d::Profile& msg,
                        const ping_protocol::ping1d::Profile& profile)
{
    msg.header.stamp    = ros::Time::now();
    msg.header.frame_id = "blue_ping1d";

    msg.distance           = profile.metadata().distance;
    msg.confidence         = profile.metadata().confidence;
    msg.transmit_duration  = profile.metadata().transmit_duration;
    msg.ping_number        = profile.metadata().ping_number;
    msg.min_range          = profile.metadata().min_range;
    msg.range_length       = profile.metadata().range_length;
    msg.gain_setting       = profile.metadata().gain_setting;

    msg.ping_data.resize(profile.metadata().profile_data_length);
    std::memcpy(msg.ping_data.data(), profile.data(), msg.ping_data.size());
}

#endif //_DEF_BLUE_PING1D_ROS_CONVERSIONS_H_
