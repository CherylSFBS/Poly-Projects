#!/usr/bin/env python

import rospy
import sys
import moveit_commander
import moveit_msgs.msg
from geometry_msgs.msg import Pose
import tf.transformations as tft
import copy
from pilz_robot_programming import *
from geometry_msgs.msg import Point 






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

def pose_move(pose):
	global robot, scene, group_name, move_group
	
	convert_factor=3.14/180.0
	quaternion_values = tft.quaternion_from_euler(pose[3]*convert_factor, pose[4]*convert_factor, pose[5]*convert_factor)

	pose_goal = Pose()
	pose_goal.position.x = pose[0]
	pose_goal.position.y = pose[1]
	pose_goal.position.z = pose[2]
	pose_goal.orientation.x = quaternion_values[0]
	pose_goal.orientation.y = quaternion_values[1]
	pose_goal.orientation.z = quaternion_values[2]
	pose_goal.orientation.w = quaternion_values[3]

	move_group.set_pose_target(pose_goal)

	success = move_group.go(wait=True)

	if(success):
		move_group.stop()
		move_group.clear_pose_targets()
	else:
		print("Pose movement failed")

def cartesian_move(waypoints):
	global robot, scene, group_name, move_group
	
	convert_factor=3.14/180.0
	wpose = Pose()
	waypoint_poses = []

	for waypoint in waypoints:
		quaternion_values = tft.quaternion_from_euler(waypoint[3]*convert_factor, waypoint[4]*convert_factor, waypoint[5]*convert_factor)
		wpose.position.x = waypoint[0]
		wpose.position.y = waypoint[1]
		wpose.position.z = waypoint[2]
		wpose.orientation.x = quaternion_values[0]
		wpose.orientation.y = quaternion_values[1]
		wpose.orientation.z = quaternion_values[2]
		wpose.orientation.w = quaternion_values[3]
		waypoint_poses.append(copy.deepcopy(wpose))

	(plan, fraction) = move_group.compute_cartesian_path(waypoint_poses, 0.001, 0.0, avoid_collisions=True)

	if fraction == 1:
		print("execute plan")
		success = move_group.execute(plan, wait=True)
		if(success):
			move_group.stop()
			move_group.clear_pose_targets()
		else:
			print("Cartesian move execution failed")
	else:
		print("Cartesian planning failed or partial completed")




def main():
	rospy.init_node('pilz_ptp_cartesian')
	moveit_commander.roscpp_initialize(sys.argv)
	link_up_robot()

	__REQUIRED_API_VERSION__ = '1'   
        pilz_robot = Robot(__REQUIRED_API_VERSION__)

	d_r= 3.14/180.0

	pilz_robot.move(Ptp(goal=[0.0, -45.0*d_r,0.0,0.0,0.0,0.0],
		planning_group = 'ur3e_suction',
		target_link = 'tool1',
		reference_frame = 'base_link',
		relative=False,
		vel_scale = 0.1,
		acc_scale = 0.1))


	pilz_robot.move(Ptp(goal=Pose(position=Point(0.3, 0.2, 0.05),
		orientation=from_euler(0, 180*d_r, 0)),
		planning_group = 'ur3e_suction',
		target_link = 'tool1',
		reference_frame = 'base_link',
		relative=False,
		vel_scale = 0.1,
		acc_scale = 0.1))

	pilz_robot.move(Ptp(goal=Pose(position=Point(0.1, 0.0, 0.0),
		orientation=from_euler(0, 0, 0)),
		planning_group = 'ur3e_suction',
		target_link = 'tool1',
		reference_frame = 'base_link',
		relative=True,
		vel_scale = 0.1,
		acc_scale = 0.1))
	

	


if __name__=='__main__':
	main()
