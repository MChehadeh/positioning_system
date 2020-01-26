#include "ROSUnit_BroadcastData.hpp"
ROSUnit_BroadcastData* ROSUnit_BroadcastData::_instance_ptr = NULL;

ROSUnit_BroadcastData::ROSUnit_BroadcastData(ros::NodeHandle& t_main_handler) : ROSUnit(t_main_handler){

    _pos_prov_pub = t_main_handler.advertise<geometry_msgs::PointStamped>("body_position", 1);
    _ori_prov_pub = t_main_handler.advertise<geometry_msgs::PointStamped>("body_orientation", 1);
    _xpv_prov_pub = t_main_handler.advertise<geometry_msgs::PointStamped>("x_provider", 1);
    _ypv_prov_pub = t_main_handler.advertise<geometry_msgs::PointStamped>("y_provider", 1);
    _zpv_prov_pub = t_main_handler.advertise<geometry_msgs::PointStamped>("z_provider", 1);
    _rollpv_prov_pub = t_main_handler.advertise<geometry_msgs::PointStamped>("roll_provider", 1);
    _pitchpv_prov_pub = t_main_handler.advertise<geometry_msgs::PointStamped>("pitch_provider", 1);
    _yawpv_prov_pub = t_main_handler.advertise<geometry_msgs::PointStamped>("yaw_provider", 1);
    _yawratepv_prov_pub = t_main_handler.advertise<geometry_msgs::PointStamped>("yaw_rate_provider", 1);
    _cs_prov_pub = t_main_handler.advertise<std_msgs::Float64MultiArray>("control_systems_output", 1);
    _act_prov_pub = t_main_handler.advertise<std_msgs::Float64MultiArray>("actuation_output", 1);

    _att.roll = 0;
    _head.yaw = 0;

    _instance_ptr = this;
}

ROSUnit_BroadcastData::~ROSUnit_BroadcastData() {

}


void ROSUnit_BroadcastData::receive_msg_data(DataMessage* t_msg){
    //TODO refactor to remove ROS message, because these messages can go to more places
    if(t_msg->getType() == msg_type::ROS){
        ROSMsg* ros_msg = (ROSMsg*)t_msg;

        if(ros_msg->getROSMsgType() == ros_msg_type::CONTROLSYSTEM){
            int i = (int)ros_msg->getSource();
            _cs_outputs[i] = ros_msg->getControlSystem();

            std_msgs::Float64MultiArray msg;
            msg.data = _cs_outputs;
            _cs_prov_pub.publish(msg);

        }else if(ros_msg->getROSMsgType() == ros_msg_type::ACTUATION){
            float* pointer = ros_msg->getActuation();

            for(int i=0;i<6;i++){
                _act_outputs[i] = *pointer++;
            }
            
            std_msgs::Float64MultiArray msg;
            msg.data = _act_outputs;
            _act_prov_pub.publish(msg);
            
        }
    }
}

void ROSUnit_BroadcastData::receive_msg_data(DataMessage* t_msg, int t_channel){

    if(t_msg->getType() == msg_type::VECTOR3D){
        Vector3DMessage* vector3d_msg = (Vector3DMessage*)t_msg;
        if(t_channel == (int)control_system::x){
            Vector3D<float> xpv = vector3d_msg->getData();
            _position.x = xpv.x;
            x_received = true;
            geometry_msgs::PointStamped msg;
            msg.header.seq = ++_seq_xpv;
            msg.header.stamp = ros::Time::now();
            msg.header.frame_id = "";
            msg.point.x = xpv.x;
            msg.point.y = xpv.y;
            msg.point.z = xpv.z;
            _xpv_prov_pub.publish(msg);

        }else if(t_channel == (int)control_system::y){
            Vector3D<float> ypv = vector3d_msg->getData();
            _position.y = ypv.x;
            y_received = true;
            geometry_msgs::PointStamped msg;
            msg.header.seq = ++_seq_ypv;
            msg.header.stamp = ros::Time::now();
            msg.header.frame_id = "";
            msg.point.x = ypv.x;
            msg.point.y = ypv.y;
            msg.point.z = ypv.z;
            _ypv_prov_pub.publish(msg);

        }else if(t_channel == (int)control_system::z){
            Vector3D<float> zpv = vector3d_msg->getData();
            _position.z = zpv.x;
            z_received = true;
            geometry_msgs::PointStamped msg;
            msg.header.seq = ++_seq_zpv;
            msg.header.stamp = ros::Time::now();
            msg.header.frame_id = "";
            msg.point.x = zpv.x;
            msg.point.y = zpv.y;
            msg.point.z = zpv.z;
            _zpv_prov_pub.publish(msg);

        }else if(t_channel == (int)control_system::yaw){
            Vector3D<float> yawpv = vector3d_msg->getData();
            _head.yaw = yawpv.x;
            yaw_received = true;
            geometry_msgs::PointStamped msg;
            msg.header.seq = ++_seq_yawpv;
            msg.header.stamp = ros::Time::now();
            msg.header.frame_id = "";
            msg.point.x = yawpv.x;
            msg.point.y = yawpv.y;
            msg.point.z = yawpv.z;
            _yawpv_prov_pub.publish(msg);

        }else if(t_channel == (int)control_system::yaw_rate){
            Vector3D<float> yawratepv = vector3d_msg->getData();
            geometry_msgs::PointStamped msg;
            msg.header.seq = ++_seq_yawratepv;
            msg.header.stamp = ros::Time::now();
            msg.header.frame_id = "";
            msg.point.x = yawratepv.x;
            msg.point.y = yawratepv.y;
            msg.point.z = yawratepv.z;
            _yawratepv_prov_pub.publish(msg);

        }else if(t_channel == (int)control_system::roll){
            Vector3D<float> rollpv = vector3d_msg->getData();
            _att.roll = rollpv.x;
            roll_received = true;
            geometry_msgs::PointStamped msg;
            msg.header.seq = ++_seq_rollpv;
            msg.header.stamp = ros::Time::now();
            msg.header.frame_id = "";
            msg.point.x = rollpv.x;
            msg.point.y = rollpv.y;
            msg.point.z = rollpv.z;
            _rollpv_prov_pub.publish(msg);
        }else if(t_channel == (int)control_system::pitch){
            Vector3D<float> pitchpv = vector3d_msg->getData();
            _att.pitch = pitchpv.x;
            pitch_received = true;
            geometry_msgs::PointStamped msg;
            msg.header.seq = ++_seq_pitchpv;
            msg.header.stamp = ros::Time::now();
            msg.header.frame_id = "";
            msg.point.x = pitchpv.x;
            msg.point.y = pitchpv.y;
            msg.point.z = pitchpv.z;
            _pitchpv_prov_pub.publish(msg);
        }
    }

    if(x_received && y_received && z_received){
        geometry_msgs::PointStamped msg;
        msg.header.seq = ++_seq_pos;
        msg.header.stamp = ros::Time::now();
        msg.header.frame_id = "body x y z";
        msg.point.x = _position.x;
        msg.point.y = _position.y;
        msg.point.z = _position.z;
        _pos_prov_pub.publish(msg);
        x_received = false;
        y_received = false;
        z_received = false;
    }

    if(roll_received && pitch_received && yaw_received){
            geometry_msgs::PointStamped msg;
            msg.header.seq = ++_seq_ori;
            msg.header.stamp = ros::Time::now();
            msg.header.frame_id = "body roll pitch yaw";
            msg.point.x = _att.roll;
            msg.point.y = _att.pitch;
            msg.point.z = _head.yaw;
            _ori_prov_pub.publish(msg);
            roll_received = false;
            pitch_received = false;
            yaw_received = false;

        }

}