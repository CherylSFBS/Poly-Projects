#!/usr/bin/env python

import sys				#library required for passing argument
import rospy				#rospy - python library for ROS
import moveit_commander			#moveit_commander - main library for MoveIt move_group interface
import moveit_msgs.msg			#moveit_msg.msg -  message to be passed to the Move_IT
from geometry_msgs.msg import Pose 	#ROS standard msg format for passing robot pose and orientation
import tf.transformations as tft	#for conversation between quaternion & euler
import copy
from pilz_robot_programming import *
from geometry_msgs.msg import Point 	#ROS standard msg format for Position (x,y,z)
from geometry_msgs.msg import PoseStamped 	#ROS standard msg format for passing robot pose and orientation

def pnp_movement(msg):
     global pilz_robot
     d_r = 3.14/180.0
     pick_pos = msg
	
     #move above pick position
     pilz_robot.move(Lin(goal=Pose(position=Point(pick_pos.x,  pick_pos.y, pick_pos.z-0.05), orientation=from_euler(0.0, 180.0*d_r, 0.0)),
    		         planning_group = 'ur3e_suction', target_link = 'tool1', reference_frame = 'base_link', relative=False, vel_scale = 0.1, acc_scale = 0.1))

     #move downward to pick position
     pilz_robot.move(Lin(goal=Pose(position=Point(0.0,  0.0, 0.05), orientation=from_euler(0.0, 0.0, 0.0)),
    		         planning_group = 'ur3e_suction', target_link = 'tool1', reference_frame = 'tool1', relative=False, vel_scale = 0.1, acc_scale = 0.1))
     
     #insert code to on suction here (grasp part)

     #move upward from pick position
     pilz_robot.move(Lin(goal=Pose(position=Point(0.0,  0.0, -0.05), orientation=from_euler(0.0, 0.0, 0.0)),
    		         planning_group = 'ur3e_suction', target_link = 'tool1', reference_frame = 'tool1', relative=False, vel_scale = 0.1, acc_scale = 0.1))

     #move above place position
     pilz_robot.move(Lin(goal=Pose(position=Point(0.4,  0.15, 0.15), orientation=from_euler(0.0, 180.0*d_r, 0.0)),
    		         planning_group = 'ur3e_suction', target_link = 'tool1', reference_frame = 'base_link', relative=False, vel_scale = 0.1, acc_scale = 0.1))

     #move downward to place position
     pilz_robot.move(Lin(goal=Pose(position=Point(0.0,  0.0, 0.05), orientation=from_euler(0.0, 0.0, 0.0)),
    		         planning_group = 'ur3e_suction', target_link = 'tool1', reference_frame = 'tool1', relative=False, vel_scale = 0.1, acc_scale = 0.1))

     #insert code to off suction here (release part)


     #move upward from place position
     pilz_robot.move(Lin(goal=Pose(position=Point(0.0,  0.0, -0.05), orientation=from_euler(0.0, 0.0, 0.0)),
    		         planning_group = 'ur3e_suction', target_link = 'tool1', reference_frame = 'tool1', relative=False, vel_scale = 0.1, acc_scale = 0.1))

     #move robot to wait position
     pilz_robot.move(Lin(goal=Pose(position=Point(0.3,  -0.1, 0.15), orientation=from_euler(0.0, 180.0*d_r, 0.0)),
    		         planning_group = 'ur3e_suction', target_link = 'tool1', reference_frame = 'base_link', relative=False, vel_scale = 0.3, acc_scale = 0.3))


def main():
     rospy.init_node("robot_subscriber")
     __REQUIRED_API_VERSION__ = '1'   

     global pilz_robot
     pilz_robot = Robot(__REQUIRED_API_VERSION__)
     
     d_r = 3.14/180.0

     #move robot Away from table
     pilz_robot.move(Ptp(goal=[0.0,-45.0*d_r,0.0,0.0,0.0,0.0],
                             planning_group = 'ur3e_suction', target_link = 'tool1', reference_frame = 'base_link', relative=False, vel_scale = 0.5, acc_scale = 0.5))
     pilz_robot.move(Ptp(goal=[45.0*d_r,-45.0*d_r,45.0*d_r,-90.0*d_r,-90.0*d_r,0.0],
                             planning_group = 'ur3e_suction', target_link = 'tool1', reference_frame = 'base_link', relative=False, vel_scale = 0.5, acc_scale = 0.5))

     #move robot to wait position
     pilz_robot.move(Lin(goal=Pose(position=Point(0.3,  -0.1, 0.15), orientation=from_euler(0.0, 180.0*d_r, 0.0)),
    		         planning_group = 'ur3e_suction', target_link = 'tool1', reference_frame = 'base_link', relative=False, vel_scale = 0.3, acc_scale = 0.3))


     pick_pos_subscriber = rospy.Subscriber('/pick_pos', Point, pnp_movement)  #declare subscriber     
     rospy.spin()							       #keep subscriber and ROS operation running at background    					


if __name__ == "__main__":
    main()


