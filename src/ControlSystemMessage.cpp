#include "ControlSystemMessage.hpp"

ControlSystemMessage::ControlSystemMessage(control_system_msg_type t_type, Block* t_to_remove, Block* t_to_add) {
    _to_remove = t_to_remove;
    _to_add = t_to_add;
    _type = msg_type::control_system;
    _control_system_msg_type = t_type;
}

ControlSystemMessage::ControlSystemMessage(control_system_msg_type t_type, Block* t_to_add) {
    _to_add = t_to_add;
    _type = msg_type::control_system;
    _control_system_msg_type = t_type;
}

ControlSystemMessage::ControlSystemMessage(control_system_msg_type t_type, PID_parameters* t_pid_para){
    _control_system_msg_type = t_type;
    _pid_para = t_pid_para;
    _type = msg_type::control_system;
}

ControlSystemMessage::ControlSystemMessage(control_system t_source, control_system t_destination, control_system_msg_type t_type, float t_data){
    _type = msg_type::control_system;
    _source = t_source;
    _destination = t_destination;
    _control_system_msg_type = t_type;
    _data = t_data;
}

ControlSystemMessage::ControlSystemMessage(control_system t_source, control_system_msg_type t_type, Vector3D t_data){
    _type = msg_type::control_system;
    _source = t_source;
    _control_system_msg_type = t_type;
    _v3d_data = t_data;
}

ControlSystemMessage::ControlSystemMessage(){
}

ControlSystemMessage::~ControlSystemMessage() {

}

msg_type ControlSystemMessage::getType(){
    return _type;
}

const int ControlSystemMessage::getSize()
{
    return sizeof(this);
}

Block* ControlSystemMessage::getBlockToRemove(){
    return _to_remove;
}
Block* ControlSystemMessage::getBlockToAdd(){
    return _to_add;
}
control_system_msg_type ControlSystemMessage::getControlSystemMsgType(){
    return _control_system_msg_type;
}
PID_parameters* ControlSystemMessage::getPIDSettings(){
    return _pid_para;
}
control_system ControlSystemMessage::getSource(){
    return _source;
}
control_system ControlSystemMessage::getDestination(){
    return _destination;
}
float ControlSystemMessage::getData(){
    return _data;
}
Vector3D ControlSystemMessage::getV3DData(){
    return _v3d_data;
}

void ControlSystemMessage::setControlSystemMessage(control_system_msg_type t_type, Block* t_to_remove, Block* t_to_add) {
    _to_remove = t_to_remove;
    _to_add = t_to_add;
    _type = msg_type::control_system;
    _control_system_msg_type = t_type;
}

void ControlSystemMessage::setControlSystemMessage(control_system_msg_type t_type, Block* t_to_add) {
    _to_add = t_to_add;
    _type = msg_type::control_system;
    _control_system_msg_type = t_type;
}

void ControlSystemMessage::setControlSystemMessage(control_system_msg_type t_type, PID_parameters* t_pid_para){
    _control_system_msg_type = t_type;
    _pid_para = t_pid_para;
    _type = msg_type::control_system;
}

void ControlSystemMessage::setControlSystemMessage(control_system t_source, control_system t_destination, control_system_msg_type t_type, float t_data){
    _type = msg_type::control_system;
    _source = t_source;
    _destination = t_destination;
    _control_system_msg_type = t_type;
    _data = t_data;
}

void ControlSystemMessage::setControlSystemMessage(control_system t_source, control_system_msg_type t_type, Vector3D t_data){
    _type = msg_type::control_system;
    _source = t_source;
    _control_system_msg_type = t_type;
    _v3d_data = t_data;
}