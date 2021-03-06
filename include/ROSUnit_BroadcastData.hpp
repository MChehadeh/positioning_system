#pragma once
#include "ROSUnit.hpp"
#include "ROSMsg.hpp"
#include <geometry_msgs/PointStamped.h>
#include <geometry_msgs/Point.h>
#include <std_msgs/Float64MultiArray.h>
#include <positioning_system/Info.h>
#include <vector>
#include "PositionMsg.hpp"
#include "Vector3DMessage.hpp"
#include "FloatMsg.hpp"
#include "IntegerMsg.hpp"

class ROSUnit_BroadcastData : public ROSUnit{

private:
    ros::Publisher _pos_prov_pub;
    ros::Publisher _ori_prov_pub;
    ros::Publisher _xpv_prov_pub;
    ros::Publisher _ypv_prov_pub;
    ros::Publisher _zpv_prov_pub;
    ros::Publisher _rollpv_prov_pub;
    ros::Publisher _pitchpv_prov_pub;
    ros::Publisher _yawpv_prov_pub;
    ros::Publisher _cs_prov_pub;
    ros::Publisher _csr_prov_pub;
    ros::Publisher _act_prov_pub;
    ros::Publisher _yawratepv_prov_pub;
    ros::Publisher _info_prov_pub;
    ros::Publisher _error_prov_pub;
    bool roll_received = false, pitch_received = false, yaw_received = false,
         x_received = false, y_received = false, z_received = false,
         armed_received = false;
    bool _armed = false;
    float _voltage;
    int _number_of_waypoints = 0;
    int _error_accumulator = 0;
    static ROSUnit_BroadcastData* _instance_ptr;
    void receive_msg_data(DataMessage* t_msg); 
    void receive_msg_data(DataMessage* t_msg, int);
    int _seq_pos = 0, _seq_ori = 0, _seq_xpv = 0, _seq_ypv = 0, _seq_zpv = 0;
    int _seq_rollpv = 0, _seq_pitchpv = 0, _seq_yawpv = 0, _seq_cs = 0, _seq_act = 0;
    int _seq_yawratepv = 0, _seq_info = 0;
    std::vector<double> _cs_outputs{ 0, 0, 0, 0, 0, 0, 0 }; 
    std::vector<double> _cs_references{ 0, 0, 0, 0, 0, 0, 0 };
    std::vector<double> _act_outputs{ 0, 0, 0, 0, 0, 0 }; 
    PositionMsg _position;
    AttitudeMsg _att;
    HeadingMsg _head;

public:
    enum ros_broadcast_channels {broadcast, x, y, z, roll, pitch, yaw, yaw_rate};
    ROSUnit_BroadcastData(ros::NodeHandle&);
    ~ROSUnit_BroadcastData();
};