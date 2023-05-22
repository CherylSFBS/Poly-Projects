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
	rospy.init_node('pilz_ptp')
	moveit_commander.roscpp_initialize(sys.argv)
	link_up_robot()
	
	__REQUIRED_API_VERSION__ = '1'   
 
    	pilz_robot = Robot(__REQUIRED_API_VERSION__)
    	d_r= 3.14/180.0
    	#position robot to start position
    	#lets add a box of size (0.1, 0.05, 0.1) to scene
    	#box Pose wrt base_link [0.3, 0.1, 0.1, 0.0, 180.0*d_r, 0.0]
    	global scene
    	box_pose = PoseStamped()
    	box_pose.header.frame_id = "base_link"
    	box_pose.pose.position = Point(0.3, -0.1, 0.05)	#x, y position is aribtrary, z=0.05 to ensure box is above table
    	box_pose.pose.orientation = from_euler(0.0, 180.0*d_r, 0.0)		#box orientation to be the same as base_link
    	scene.add_box("box1", box_pose, size=(0.1, 0.05, 0.1))
    
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

	#Approach above box with a offset of 0.03 in z-axis from the top of the box (wrt base_link)
    	approach_pose = box_pose.pose
    	approach_pose.position.z = 0.1+0.03 #box height + offset
    	pilz_robot.move(Lin(goal=approach_pose,
                        planning_group = 'ur3e_suction', 
			target_link = 'tool1', 
			reference_frame = 'base_link', 
			relative=False, 
			vel_scale = 0.3, 
			acc_scale = 0.3))
    
    	#Move downward 0.03 in z-axis to contact box (actual: 0.03-0.001 = 0.029 to avoid collision conflict) (wrt tool1)
    	pilz_robot.move(Lin(goal=Pose(position=Point(0.0,  0.0, 0.029), 
			orientation=from_euler(0.0, 0.0, 0.0)),
    		        planning_group = 'ur3e_suction', 
			target_link = 'tool1', 
			reference_frame = 'tool1', 
			relative=False, 
			vel_scale = 0.1, 
			acc_scale = 0.1))
                        
	#attached box to tool1(i.e. end_effector link)
    	scene.attach_box("tool1", "box1", touch_links="ur3e_suction")

	#Depart from pick position upward by 0.03 in z-axis  (wrt tool1)
    	pilz_robot.move(Lin(goal=Pose(position=Point(0.0,  0.0, -0.03), 
			orientation=from_euler(0.0, 0.0, 0.0)),
    		        planning_group = 'ur3e_suction', 
			target_link = 'tool1', 
			reference_frame = 'tool1', 
			relative=False, 
			vel_scale = 0.1, 
			acc_scale = 0.1))
		     
	#Approach above placement Pose [x=0.3, y=-0.1, z= 0.1 (box height) + 0.03 (offset) with +90 degree rotation in Z-Axis (wrt 		base_link)
	above_place_pose = Pose(position=Point(0.3, 0.1, 0.13), 
			   orientation=from_euler(0.0, 180*d_r, 90*d_r))
    	
	pilz_robot.move(Lin(goal=above_place_pose,
                        planning_group = 'ur3e_suction', 
			target_link = 'tool1', 
			reference_frame = 'base_link', 
			relative=False, 
			vel_scale = 0.3, 
			acc_scale = 0.3))
 
    	#Move downward 0.03 in z-axis to place box (actual: 0.03-0.001 = 0.029 to avoid collision conflict) (wrt tool1)
    	pilz_robot.move(Lin(goal=Pose(position=Point(0.0,  0.0, 0.03), 
			orientation=from_euler(0.0, 0.0, 0.0)),
    		        planning_group = 'ur3e_suction', 
			target_link = 'tool1', 
			reference_frame = 'tool1', 
			relative=False, 
			vel_scale = 0.1, 
			acc_scale = 0.1))

    	#attached box to tool1(i.e. end_effector link)
    	scene.remove_attached_object("tool1","box1")

    	#Depart from place position upward by 0.03 in z-axis  (wrt tool1)
    	pilz_robot.move(Lin(goal=Pose(position=Point(0.0,  0.0, -0.03), 
			orientation=from_euler(0.0, 0.0, 0.0)),
     		        planning_group = 'ur3e_suction', 
			target_link = 'tool1', 
			reference_frame = 'tool1', 
			relative=False, 
			vel_scale = 0.1, 
			acc_scale = 0.1))



    


if __name__== '__main__':
	main()

