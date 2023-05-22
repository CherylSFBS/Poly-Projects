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

def main():
	rospy.init_node('robot_info')
	moveit_commander.roscpp_initialize(sys.argv)
	link_up_robot()
	get_robot_info()

if __name__=='__main__':
	main()
