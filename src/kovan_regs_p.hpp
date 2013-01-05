/**************************************************************************
 *  Copyright 2012 KISS Institute for Practical Robotics                  *
 *                                                                        *
 *  This file is part of libkovan.                                        *
 *                                                                        *
 *  libkovan is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  libkovan is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with libkovan. Check the LICENSE file in the project root.      *
 *  If not, see <http://www.gnu.org/licenses/>.                           *
 **************************************************************************/

#ifndef _KOVAN_REGS_P_HPP_
#define _KOVAN_REGS_P_HPP_

// Permanent Read Only
#define DIG_IN 			1
#define AN_IN_0 		2
#define AN_IN_1 		3
#define AN_IN_2 		4
#define AN_IN_3 		5
#define AN_IN_4 		6
#define AN_IN_5 		7
#define AN_IN_6 		8
#define AN_IN_7 		9
#define AN_IN_8 		10
#define AN_IN_9 		11
#define AN_IN_10 		12
#define AN_IN_11 		13
#define AN_IN_12 		14
#define AN_IN_13 		15
#define AN_IN_14 		16
#define AN_IN_15 		17
#define AN_IN_16 		18
#define AC_CONNECTED		19

#define BEMF_0_LOW		20
#define BEMF_1_LOW		21
#define BEMF_2_LOW		22
#define BEMF_3_LOW		23

// Permanent Read/Write
#define SERVO_COMMAND_0 	25
#define SERVO_COMMAND_1 	26
#define SERVO_COMMAND_2 	27
#define SERVO_COMMAND_3 	28
#define DIG_OUT 		29
#define DIG_PULLUPS 		30
#define DIG_OUT_ENABLE 		31
#define AN_PULLUPS 		32
#define MOTOR_PWM_0 		33
#define MOTOR_PWM_1 		34
#define MOTOR_PWM_2 		35
#define MOTOR_PWM_3 		36
#define MOTOR_ALL_STOP 		40

#define BEMF_0_HIGH 		41
#define BEMF_1_HIGH 		42
#define BEMF_2_HIGH 		43
#define BEMF_3_HIGH 		44
#define SIDE_BUTTON 		45
#define MOT_BEMF_CLEAR 		46

#define GOAL_POS_0_LOW 		48
#define GOAL_POS_0_HIGH 	49
#define GOAL_POS_1_LOW 		50
#define GOAL_POS_1_HIGH 	51
#define GOAL_POS_2_LOW 		52
#define GOAL_POS_2_HIGH 	53
#define GOAL_POS_3_LOW 		54
#define GOAL_POS_3_HIGH		55

#define GOAL_SPEED_0_LOW 	56
#define GOAL_SPEED_0_HIGH 	57
#define GOAL_SPEED_1_LOW 	58
#define GOAL_SPEED_1_HIGH 	59
#define GOAL_SPEED_2_LOW 	60
#define GOAL_SPEED_2_HIGH 	61
#define GOAL_SPEED_3_LOW 	62
#define GOAL_SPEED_3_HIGH 	63

#define PID_MODES 		64
#define PID_STATUS 		65

#define MOTOR_DRIVE_CODE_T 	39

#endif
