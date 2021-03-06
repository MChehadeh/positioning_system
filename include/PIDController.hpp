#pragma once
#include "Block.hpp"
#include <math.h>
#include "ControllerMessage.hpp"
#include "PID_values.hpp"
#include "FloatMsg.hpp"
#include "Controller.hpp"
#include "SwitcherMessage.hpp"
#include "FloatMsg.hpp"
#include "ResetControllerMsg.hpp"
#include "ButterFilter_2nd_200Hz.hpp"
#include "SwitchOutMsg.hpp"
#include "logger.hpp"

class PIDController : public Controller{

    private:
        controller_type _controller_type;
        FloatMsg m_output_msg;
        SwitchOutMsg m_switchout_msg;
        block_id _id;
        float _command;
        ButterFilter_2nd_200Hz _filter;
        float _filter_y;
        //Chehadeh's code
        PID_parameters parameters;
        bool i_term, d_term, dd_term; //Comparing against booleans is faster
        float _dt;
	    bool en_pv_derivation = true, en_anti_windup = false;
        void set_internal_sw(PID_parameters pid_para_x);
        //---------------
        
    public:
        //Chehadeh's code
        float prev_err = 0, prev2_err = 0, prev_pv_rate = 0, accum_u = 0, accum_I = 0;
        void initialize(void*);
        float pid_inc(float err, float pv_first, float pv_second=-1);
        float pid_direct(float err, float pv_first, float pv_second=-1);
        void set_I_term(float);
        //---------------
        
        void switchIn(DataMessage*);
        DataMessage* switchOut();
        void receive_msg_data(DataMessage* t_msg); 
        void reset();
        DataMessage* receive_msg_internal(DataMessage*);
        controller_type getControllerType(){ return _controller_type; }
        block_id getID(){ return _id; }
        //TODO Send a message to Switcher
        //TODO Receive a message from Switcher

        PIDController(block_id t_id);
        ~PIDController();
};
