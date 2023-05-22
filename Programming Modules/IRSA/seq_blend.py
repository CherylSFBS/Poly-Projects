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
################################################################################
# Connect to robot model
################################################################################
def link_up_robot():
     
     global robot, scene, group_name, move_group	#state  object variables as global : robot, scene, group_name and move_group
     
     robot = moveit_commander.RobotCommander()		#initiate robot object

     scene = moveit_commander.PlanningSceneInterface() 	#initiate scene object

     group_name = "ur3e_suction"			#our ur robot has a planning group name "ur3e_suction"
     
     move_group = moveit_commander.MoveGroupCommander(group_name) #initiate move_group and connected it our robot planning group

################################################################################
# get basic information from robot
# 
################################################################################
def get_robot_info():
     
     global robot, scene, group_name, move_group		#state  object variables as global : robot, scene, group_name and move_group
     
     ## Getting Basic Information
     ## ^^^^^^^^^^^^^^^^^^^^^^^^^
     #We can get the name of the reference frame for this robot:
	
     planning_frame = move_group.get_planning_frame()		#get planning frame - reference axis
     
     print("============ Planning frame: {}".format(planning_frame))	

     # We can also print the name of the end-effector link for this group:	

     eef_link = move_group.get_end_effector_link()		#get end_effector link (or tip link)
     print("============ End effector link: {}".format(eef_link))	

     # We can get a list of all the groups in the robot:	

     group_names = robot.get_group_names()			#get all planning group names
     print("============ Available Planning Groups: {}".format(robot.get_group_names()))

     # Sometimes for debugging it is useful to print the entire state of the  robot:
     print("============ Printing robot state")			#print robot states:  all information
     print(robot.get_current_state())



################################################################################
# Relative Joint move (i.e.Single Joint movement)
# input value : joint_num : 0-5 , rel_value = rel joint angle in degree
################################################################################
def rel_single_joint_move(joint_num, rel_value):
	
    global robot, scene, group_name, move_group	#state  object variables as global : robot, scene, group_name and move_group
    
    curr_joint = move_group.get_current_joint_values()
    #print(curr_joint)
    joint_goal = curr_joint
    
    joint_goal[joint_num] = joint_goal[joint_num]+(rel_value/180.0*3.14)
	
    
    success = move_group.go(joint_goal, wait=True)	#wait = true, blocking function

    if(success):
	move_group.stop()	# Calling ``stop()`` ensures that there is no residual movement
    else:
	print("Rel Single Joint movement failed")
    

################################################################################
# Absolute Joint move (i.e. Single Joint movement)
# input value : joint_num : 0-5 , abs_value = absolute joint angle in degree
################################################################################
def abs_single_joint_move(joint_num, abs_value):
	
    global robot, scene, group_name, move_group	#state  object variables as global : robot, scene, group_name and move_group
    
    curr_joint = move_group.get_current_joint_values()

    joint_goal = curr_joint
    
    joint_goal[joint_num] = (abs_value/180.0*3.14)
	
    
    success = move_group.go(joint_goal, wait=True)  #wait = true, blocking function

    
    if(success):
	move_group.stop()	# Calling ``stop()`` ensures that there is no residual movement
    else:
	print("Abs Single Joint movement failed")
   

################################################################################
# Relative Joint move (i.e. All Joints movement)
# input value : rel_joint_values = [j1,j2,j3,j4,j5,j6] (j1-6 = rel joint angle in degree)
################################################################################
def rel_joints_move(rel_joint_values):
	
    global robot, scene, group_name, move_group	#state  object variables as global : robot, scene, group_name and move_group
    
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


################################################################################
# Absolute Joint move (i.e. All Joints movement)
# input value : abs_joint_values = [j1,j2,j3,j4,j5,j6] (j1-6 = abs joint angle in degree)
################################################################################
def abs_joints_move(abs_joint_values):
	
    global robot, scene, group_name, move_group	#state  object variables as global : robot, scene, group_name and move_group
    
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
	print("abs Joints movement failed")


################################################################################
# Pose Movement (i.e. End-Effector movement)
# input value : pose = [x,y,z,rx,ry,rz] (rx,ry,rz = euler angle in degree)
# relative value in degree
################################################################################
def pose_move(pose):
    global robot, scene, group_name, move_group	#state  object variables as global : robot, scene, group_name and move_group

    convert_factor = 3.14/180.0
    quaternion_values = tft.quaternion_from_euler(pose[3]*convert_factor, pose[4]*convert_factor,  pose[5]*convert_factor)

    pose_goal = Pose()
    pose_goal.position.x = pose[0]
    pose_goal.position.y = pose[1]
    pose_goal.position.z = pose[2]
    pose_goal.orientation.x = quaternion_values[0]
    pose_goal.orientation.y = quaternion_values[1]
    pose_goal.orientation.z = quaternion_values[2]
    pose_goal.orientation.w = quaternion_values[3]

    move_group.set_pose_target(pose_goal)

        
    success = move_group.go(wait=True)	#wait = true, blocking function

    if(success):
	move_group.stop()		# Calling ``stop()`` ensures that there is no residual movement
        move_group.clear_pose_targets() # It is always good to clear your targets after planning with poses.
    else:
	print("Pose movement failed")	
    

################################################################################
# Cartesian Movement (i.e. End-Effector movement)
# input value : waypoints = [[x,y,z,rx,ry,rz], .....]  (rx,ry,rz = euler angle in degree)
# relative value in degree
################################################################################
def cartesian_move(waypoints):
        global robot, scene, group_name, move_group	#state  object variables as global : robot, scene, group_name and move_group

	convert_factor = 3.14/180.0
	wpose = Pose()
        waypoint_poses = []

        for waypoint in waypoints:
		#print(waypoint)
		quaternion_values = tft.quaternion_from_euler(waypoint[3]*convert_factor, waypoint[4]*convert_factor,  waypoint[5]*convert_factor)
		wpose.position.x = waypoint[0]
		wpose.position.y = waypoint[1]
		wpose.position.z = waypoint[2]
		wpose.orientation.x = quaternion_values[0]
		wpose.orientation.y = quaternion_values[1]	
		wpose.orientation.z = quaternion_values[2]
		wpose.orientation.w = quaternion_values[3]	
        	waypoint_poses.append(copy.deepcopy(wpose))
		

        # We want the Cartesian path to be interpolated at a resolution of 1 cm
        # which is why we will specify 0.001 as the eef_step in Cartesian
        # translation.  We will disable the jump threshold by setting it to 0.0,
        # ignoring the check for infeasible jumps in joint space, which is sufficient
        # for this tutorial.
        (plan, fraction) = move_group.compute_cartesian_path(waypoint_poses, 0.001, 0.0, avoid_collisions=True)  
	print(plan)
	if fraction == 1:
		print("execute plan")
	      	success = move_group.execute(plan, wait=True)
		if(success):
			move_group.stop()		# Calling ``stop()`` ensures that there is no residual movement
			move_group.clear_pose_targets() # It is always good to clear your targets after planning with poses.
		else:
			print("Cartesian move execution failed")	
	else:
		print("Cartesian planning failed or partial completed")	





def main():
    rospy.init_node("move_robot")
    moveit_commander.roscpp_initialize(sys.argv)
    link_up_robot()

    __REQUIRED_API_VERSION__ = '1'   
 
    pilz_robot = Robot(__REQUIRED_API_VERSION__)
    d_r = 3.14/180.0

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

    blend_sequence1 = Sequence()

    #Circ move path 1 (wrt tool1)
    blend_sequence1.append(Circ(goal=Pose(position=Point(-0.2, 0.0, 0.0),
			   orientation=from_euler(0.0, 0.0, 0.0)),
                           interim = Point(-0.1, 0.1, 0),
                           center = None,
    		           planning_group = 'ur3e_suction',
			   target_link = 'tool1', 
			   reference_frame = 'tool1', 
			   relative=False, 
			   vel_scale = 0.1, 
		           acc_scale = 0.1),
			   blend_radius=0.01)

    #Lin move to start point of Circ move path 2 (wrt tool1)
    blend_sequence1.append(Lin(goal=Pose(position=Point(-0.2,  -0.1, 0.0), 
			   orientation=from_euler(0.0, 0.0, 0.0)),
    		           planning_group = 'ur3e_suction',
			   target_link = 'tool1',
			   reference_frame = 'tool1', 
			   relative=False, 
			   vel_scale = 0.1, 
			   acc_scale = 0.1),
			   blend_radius=0.01) 

    #Circ move path 2 (wrt tool1)
    blend_sequence1.append(Circ(goal=Pose(position=Point(0.0, -0.1, 0.0), 
			   orientation=from_euler(0.0, 0.0, 0.0)),
                           interim = Point(-0.1, -0.2, 0.0),
                           center = None,
      		           planning_group = 'ur3e_suction', 
			   target_link = 'tool1', 
			   reference_frame = 'tool1', 
                           relative=False, 
			   vel_scale = 0.1, 
			   acc_scale = 0.1),
    			   blend_radius=0.01)
    
    #Lin move to complete the external contour (wrt tool1)
    blend_sequence1.append(Lin(goal=Pose(position=Point(0.0,  0.0, 0.0), 
			   orientation=from_euler(0.0, 0.0, 0.0)),
    		           planning_group = 'ur3e_suction', 
			   target_link = 'tool1', 
			   reference_frame = 'tool1', 
			   relative=False, 
                           vel_scale = 0.1, 
			   acc_scale = 0.1))
       
    pilz_robot.move(blend_sequence1)

    
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

    
    blend_sequence2 = Sequence()


    #Circ move path 3 (wrt tool1)
    blend_sequence2.append(Circ(goal=Pose(position=Point(-0.1, 0.0, 0.0), orientation=from_euler(0.0, 0.0, 0.0)),
                           interim = Point(-0.05, 0.05, 0.0),
                           center = None,
     		           planning_group = 'ur3e_suction', 
			   target_link = 'tool1', 
			   reference_frame = 'tool1', 
			   relative=False, 
			   vel_scale = 0.1, 
			   acc_scale = 0.1),
			   blend_radius = 0.01)
   
    #Circ move path 4 (wrt tool1)
    blend_sequence2.append(Circ(goal=Pose(position=Point(0.0, 0.0, 0.0), orientation=from_euler(0.0, 0.0, 0.0)),
                         interim = Point(-0.05, -0.05, 0.0),
                         center = None,
     		         planning_group = 'ur3e_suction', 
			 target_link = 'tool1', 
			 reference_frame = 'tool1', 
			 relative=False, 
			 vel_scale = 0.1, 
			 acc_scale = 0.1))

    pilz_robot.move(blend_sequence2)
   

    #Lin depart move (wrt tool1)
    pilz_robot.move(Lin(goal=Pose(position=Point(0.0,  0.0, -0.075), 
			orientation=from_euler(0.0, 0.0, 0.0)),
    		        planning_group = 'ur3e_suction', 
			target_link = 'tool1', 
			reference_frame = 'tool1', 
			relative=False, 
			vel_scale = 0.1, 
			acc_scale = 0.1))

   
if __name__ == "__main__":
    main()


