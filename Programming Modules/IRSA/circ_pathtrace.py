#!/usr/bin/env python

import rospy
import sys
import moveit_commander
import moveit_msgs.msg
from geometry_msgs.msg import Pose
from pilz_robot_programming import *	#pilz_robot_programming library
from geometry_msgs.msg import Point 	#ROS standard msg format for Position (x,y,z)



def get_robot_info():
	global robot, scene, group_name, move_group
	
	planning_frame = move_group.get_planning_frame()
	print("============ Planning frame: {}".format(planning_frame))	
	
	eef_link = move_group.get_end_effector_link()		         
     	print("============ End effector link: {}".format(eef_link))

	group_names = robot.get_group_names()				
     	print("============ Available Planning Groups: {}".format(robot.get_group_names()))

	print("============ Printing robot state")
     	print(robot.get_current_state())


def link_up_robot():
	global robot, scene, group_name, move_group
	robot = moveit_commander.RobotCommander()
	scene = moveit_commander.PlanningSceneInterface()
	group_name = 'ur3e_suction'
	move_group = moveit_commander.MoveGroupCommander(group_name)


def rel_single_joint_move(joint_num, rel_value):
	global robot, scene, group_name, move_group
	curr_joint = move_group.get_current_joint_values()
	joint_goal = curr_joint
	joint_goal[joint_num] = joint_goal[joint_num]+(rel_value/180.0*3.14)

	success = move_group.go(joint_goal, wait=True)	
	if (success):
		move_group.stop()
	else:
		print("Rel Single Joint movement failed")	                                         

def abs_single_joint_move(joint_num, abs_value):
	global robot, scene, group_name, move_group
	curr_joint = move_group.get_current_joint_values()
	joint_goal = curr_joint
	joint_goal[joint_num] = (abs_value/180.0*3.14)

	success = move_group.go(joint_goal, wait=True)	
	if (success):
		move_group.stop()
	else:
		print("Abs Single Joint movement failed")

def rel_joints_move(rel_joint_values):
        global robot, scene, group_name, move_group	
  
        if(len(rel_joint_values) != 6):
		print("size of input joint values mismatch")
		exit()

    	curr_joint = move_group.get_current_joint_values()

    	joint_goal = curr_joint
    
    
    	for index, values in enumerate(joint_goal):
   	 	joint_goal[index] = values+(rel_joint_values[index]/180.0*3.14)
    
    	success = move_group.go(joint_goal, wait=True)	#wait = true, blocking function

    	if(success):
		move_group.stop()	# Calling ``stop()`` ensures that there is no residual movement
    	else:
		print("Rel Joints movement failed")

def abs_joints_move(abs_joint_values):
    	global robot, scene, group_name, move_group	
  
    	if(len(abs_joint_values) != 6):
		print("size of input joint values mismatch")
		exit()

    	curr_joint = move_group.get_current_joint_values()

    	joint_goal = curr_joint
    
    
    	for index, values in enumerate(joint_goal):
   	 	joint_goal[index] = (abs_joint_values[index]/180.0*3.14)
    
    	success = move_group.go(joint_goal, wait=True)	#wait = true, blocking function

    	if(success):
		move_group.stop()	# Calling ``stop()`` ensures that there is no residual movement
    	else:
		print("Abs Joints movement failed")


def main():
	rospy.init_node('pilz_circ_pathtrace')
	moveit_commander.roscpp_initialize(sys.argv)
	link_up_robot()
	
	__REQUIRED_API_VERSION__ = '1'   
 
    	pilz_robot = Robot(__REQUIRED_API_VERSION__)
    	d_r= 3.14/180.0
    	#position robot to start position
    	pilz_robot.move(Ptp(goal=[0.0,-45.0*d_r,0.0,0.0,0.0,0.0],
                        planning_group = 'ur3e_suction', 
			target_link = 'tool1', 
			reference_frame = 'base_link', 
			relative=False, 
			vel_scale = 0.5, 
			acc_scale = 0.5))
    	
	pilz_robot.move(Ptp(goal=[45.0*d_r,-45.0*d_r,45.0*d_r,-90.0*d_r,-90.0*d_r,0.0],
                        planning_group = 'ur3e_suction', 
			target_link = 'tool1', 
			reference_frame = 'base_link', 
			relative=False, 
			vel_scale = 0.5, 
			acc_scale = 0.5))

    	#Lin move above start point of path (wrt base_link)
    	pilz_robot.move(Lin(goal=Pose(position=Point(0.3,  0.1, 0.15), 
			orientation=from_euler(0.0, 180*d_r, 0.0)),
    		        planning_group = 'ur3e_suction', 
			target_link = 'tool1', 
			reference_frame = 'base_link', 
			relative=False, 
			vel_scale = 0.3, 
			acc_scale = 0.3))

    	#move downward to start point of path (wrt tool1)
    	pilz_robot.move(Lin(goal=Pose(position=Point(0.0,  0.0, 0.075), 
			orientation=from_euler(0.0, 0.0, 0.0)),
    		        planning_group = 'ur3e_suction', 
			target_link = 'tool1', 
			reference_frame = 'tool1', 
			relative=False, 
			vel_scale = 0.1, 
			acc_scale = 0.1))

    	#Circ move path 1 (wrt tool1)
    	pilz_robot.move(Circ(goal=Pose(position=Point(-0.2, 0.0, 0.0), 
			orientation=from_euler(0.0, 0.0, 0.0)),
                        interim = Point(-0.1, 0.1, 0.0),
                        center = None,
    		        planning_group = 'ur3e_suction', 
			target_link = 'tool1', 
			reference_frame = 'tool1', 
			relative=False, 
			vel_scale = 0.1, 
			acc_scale = 0.1))

    	#Lin move to start point of Circ move path 2 (wrt tool1)
    	pilz_robot.move(Lin(goal=Pose(position=Point(0.0,  -0.1, 0.0), 
			orientation=from_euler(0.0, 0.0, 0.0)),
    		        planning_group = 'ur3e_suction', 
			target_link = 'tool1', 
			reference_frame = 'tool1', 
			relative=False, 
			vel_scale = 0.1, 
			acc_scale = 0.1))

    	#Circ move path 2 (wrt tool1)
    	pilz_robot.move(Circ(goal=Pose(position=Point(0.2, 0.0, 0.0), 
			orientation=from_euler(0.0, 0.0, 0.0)),
                        interim = Point(0.1, -0.1, 0.0),
                        center = None,
    		        planning_group = 'ur3e_suction', 
			target_link = 'tool1', 
			reference_frame = 'tool1', 
			relative=False, 
			vel_scale = 0.1, 
			acc_scale = 0.1))
    
    	#Lin move to complete the external contour (wrt tool1)
    	pilz_robot.move(Lin(goal=Pose(position=Point(0.0,  0.1, 0.0), 
			orientation=from_euler(0.0, 0.0, 0.0)),
    		        planning_group = 'ur3e_suction', 
			target_link = 'tool1', 
			reference_frame = 'tool1', 
			relative=False, 
			vel_scale = 0.1, 
			acc_scale = 0.1))

    	#Lin depart move (wrt tool1)
    	pilz_robot.move(Lin(goal=Pose(position=Point(0.0,  0.0, -0.075), 
			orientation=from_euler(0.0, 0.0, 0.0)),
    		        planning_group = 'ur3e_suction', 
			target_link = 'tool1', 
			reference_frame = 'tool1', 
			relative=False, 
			vel_scale = 0.1, 
			acc_scale = 0.1))

    	#Lin move above start point of circle path (wrt tool1)
    	pilz_robot.move(Lin(goal=Pose(position=Point(-0.05,  0.0, 0.0), 
			orientation=from_euler(0.0, 0.0, 0.0)),
    		        planning_group = 'ur3e_suction', 
			target_link = 'tool1',
			reference_frame = 'tool1',
			relative=False,
			vel_scale = 0.1,
			acc_scale = 0.1))

    	#Lin move to point of circle path (wrt tool1)
    	pilz_robot.move(Lin(goal=Pose(position=Point(0.0,  0.0, 0.075), 
			orientation=from_euler(0.0, 0.0, 0.0)),
    		        planning_group = 'ur3e_suction', 
			target_link = 'tool1', 
			reference_frame = 'tool1', 
			relative=False, 
			vel_scale = 0.1, 
			acc_scale = 0.1))

    	#Circ move path 3 (wrt tool1)
    	pilz_robot.move(Circ(goal=Pose(position=Point(-0.1, 0.0, 0.0), 
			orientation=from_euler(0.0, 0.0, 0.0)),
                        interim = Point(-0.05, 0.05, 0.0),
                        center = None,
    		        planning_group = 'ur3e_suction', 
			target_link = 'tool1', 
			reference_frame = 'tool1', 
			relative=False, 
			vel_scale = 0.1, 
			acc_scale = 0.1))
   
    	#Circ move path 4 (wrt tool1)
    	pilz_robot.move(Circ(goal=Pose(position=Point(0.1, 0.0, 0.0), 
			orientation=from_euler(0.0, 0.0, 0.0)),
                       	interim = Point(0.05, -0.05, 0.0),
                        center = None,
    		        planning_group = 'ur3e_suction', 
			target_link = 'tool1', 
			reference_frame = 'tool1', 
			relative=False, 
			vel_scale = 0.1, 
			acc_scale = 0.1))

    	#Lin depart move (wrt tool1)
    	pilz_robot.move(Lin(goal=Pose(position=Point(0.0,  0.0, -0.075), 
			orientation=from_euler(0.0, 0.0, 0.0)),
			planning_group = 'ur3e_suction', 
			target_link = 'tool1', 
			reference_frame = 'tool1', 
			relative=False, 
			vel_scale = 0.1, 
			acc_scale = 0.1))



    


if __name__== '__main__':
	main()

