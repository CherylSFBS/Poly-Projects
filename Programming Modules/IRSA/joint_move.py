#!/usr/bin/env python

import rospy
import sys
import moveit_commander
import moveit_msgs.msg
from geometry_msgs.msg import Pose

def get_robot_info():
	global robot, scene, group_name, move_group

	planning_frame = move_group.get_planning_frame
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
	group_name = "ur3e_suction"
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
		print("ABS Single Joint movement failed")

def rel_joints_move(rel_joint_values):
	global robot, scene, group_name, move_group
	
	if(len(rel_joint_values) != 6):
		print("size of input joint values mismatch")
		exit()
	
	curr_joint = move_group.get_current_joint_values()
	
	joint_goal = curr_joint

	for index, values in enumerate(joint_goal):
		joint_goal[index] = values+(rel_joint_values[index]/180.0*3.14)

	success = move_group.go(joint_goal, wait=True)

	if(success):
		move_group.stop
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

	success = move_group.go(joint_goal, wait=True)

	if(success):
		move_group.stop
	else:
		print("Abs Joints movement failed")



def main():
	rospy.init_node('robot_info')
	moveit_commander.roscpp_initialize(sys.argv)
	link_up_robot()
	get_robot_info()

	move_group.set_max_velocity_scaling_factor (0.1)
	move_group.set_max_acceleration_scaling_factor(0.1)
	abs_joints_move([0,0,0,0,0,0])
	
	move_group.set_max_velocity_scaling_factor (0.3)
	move_group.set_max_acceleration_scaling_factor(0.3)
	abs_joints_move([90,0,0,0,0,0])


if __name__=='__main__':
	main()
