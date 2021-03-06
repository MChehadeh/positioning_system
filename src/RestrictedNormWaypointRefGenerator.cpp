#include "RestrictedNormWaypointRefGenerator.hpp"
pthread_mutex_t RestrictedNormWaypointRefGenerator::lock;


void RestrictedNormWaypointRefGenerator::receive_msg_data(DataMessage* t_msg){
    if (t_msg->getType()==msg_type::POSES)
    {
        PosesMsg* t_pos_msg=(PosesMsg*) t_msg;
        #ifdef mutex_safety
        pthread_mutex_lock(&lock);
        #endif
        for(int i=0;i<t_pos_msg->p.poses.size();i++){
            Waypoint t_waypoint;
            t_waypoint.position.x=t_pos_msg->p.poses[i].x;
            t_waypoint.position.y=t_pos_msg->p.poses[i].y;
            t_waypoint.position.z=t_pos_msg->p.poses[i].z;
            t_waypoint.yaw=t_pos_msg->p.poses[i].yaw;
            std::cout << "waypoint X : " << t_waypoint.position.x << std::endl;
            std::cout << "waypoint Y : " << t_waypoint.position.y << std::endl;
            std::cout << "waypoint Z : " << t_waypoint.position.z << std::endl;
            std::cout << "waypoint Yaw : " << t_waypoint.yaw << std::endl;
            Waypoints.push_back(t_waypoint);
        }
        if(old_size != Waypoints.size()){
            ros_msg.setNumberOfWaypoints(Waypoints.size());
            this->emit_message((DataMessage*) &ros_msg);
            old_size = Waypoints.size();
        }
        #ifdef mutex_safety
        pthread_mutex_unlock(&lock);
        #endif
    }
    else if(t_msg->getType()==msg_type::RESTNORMREF_SETTINGS)
    {
        #ifdef mutex_safety
        pthread_mutex_lock(&lock);
        #endif
        RestrictedNormRefSettingsMsg* t_settings_msg=(RestrictedNormRefSettingsMsg*) t_msg;
        max_norm=t_settings_msg->getMaxNorm();
        enabled=t_settings_msg->enabled;
        std::cout << "ENABLED: " << enabled << std::endl;
        std::cout << "DELETE : " << t_settings_msg->delete_existing_waypoints << std::endl;
        std::cout << "MAX NORM : " << max_norm << std::endl;
        if (t_settings_msg->delete_existing_waypoints){
            Waypoints.clear();
        }
        if(old_size != Waypoints.size()){
            ros_msg.setNumberOfWaypoints(Waypoints.size());
            this->emit_message((DataMessage*) &ros_msg);
            old_size = Waypoints.size();
        }
        #ifdef mutex_safety
        pthread_mutex_unlock(&lock);
        #endif
    }
    else if(t_msg->getType()==msg_type::VECTOR3D){
        Vector3DMessage* t_current_pos=(Vector3DMessage*) t_msg;
        Vector3D<double> t_current_pos_vec;
        t_current_pos_vec = t_current_pos->getData();
        #ifdef mutex_safety
        pthread_mutex_lock(&lock);
        #endif
        if (Waypoints.size()>0 && enabled){
            Vector3D<double> diff_pos_waypoint=Waypoints[0].position-t_current_pos_vec;
            double t_dist= Vector3D<double>::getL2Norm(diff_pos_waypoint);
            if (t_dist>=max_norm){
                Vector3D<double> restricted_ref;
                restricted_ref=t_current_pos_vec+(diff_pos_waypoint/t_dist)*max_norm;
                updateControlSystemsReferences(restricted_ref,Waypoints[0].yaw);
            }
            else
            {
                if (Waypoints.size()==1){
                    updateControlSystemsReferences(Waypoints[0].position,Waypoints[0].yaw);
                    Waypoints.clear();
                }else{
                    auto firstWaypoint=Waypoints.begin();
                    Waypoints.erase(firstWaypoint);
                }
            }
        }
        if(old_size != Waypoints.size()){
            ros_msg.setNumberOfWaypoints(Waypoints.size());
            this->emit_message((DataMessage*) &ros_msg);
            old_size = Waypoints.size();
        }
        #ifdef mutex_safety
        pthread_mutex_unlock(&lock);
        #endif
    }

}


void RestrictedNormWaypointRefGenerator::updateControlSystemsReferences(Vector3D<double> t_pos_ref, double t_yaw){
    
    ControlSystemMessage x_cont_ref;
    x_cont_ref.setControlSystemMessage(control_system::null_type, control_system_msg_type::to_system, t_pos_ref.x);
    //std::cout << "Setting X Reference: " << t_pos_ref.x << std::endl;
    ((msg_receiver*)x_control_system)->receive_msg_data(&x_cont_ref);

    ControlSystemMessage y_cont_ref;
    y_cont_ref.setControlSystemMessage(control_system::null_type, control_system_msg_type::to_system, t_pos_ref.y);
    //std::cout << "Setting Y Reference: " << t_pos_ref.y << std::endl;
    ((msg_receiver*)y_control_system)->receive_msg_data(&y_cont_ref);

    ControlSystemMessage z_cont_ref;
    z_cont_ref.setControlSystemMessage(control_system::null_type, control_system_msg_type::to_system, t_pos_ref.z);
    //std::cout << "Setting Z Reference: " << t_pos_ref.z << std::endl;
    ((msg_receiver*)z_control_system)->receive_msg_data(&z_cont_ref);

    ControlSystemMessage yaw_cont_ref;
    yaw_cont_ref.setControlSystemMessage(control_system::null_type, control_system_msg_type::to_system, t_yaw);
    //std::cout << "Setting Yaw Reference: " << t_yaw << std::endl;
    ((msg_receiver*)yaw_control_system)->receive_msg_data(&yaw_cont_ref);
}

void RestrictedNormWaypointRefGenerator::add_x_control_system(ControlSystem* t_sys){
    x_control_system=t_sys;
}
void RestrictedNormWaypointRefGenerator::add_y_control_system(ControlSystem* t_sys){
    y_control_system=t_sys;
}
void RestrictedNormWaypointRefGenerator::add_z_control_system(ControlSystem* t_sys){
    z_control_system=t_sys;
}
void RestrictedNormWaypointRefGenerator::add_yaw_control_system(ControlSystem* t_sys){
    yaw_control_system=t_sys;
}