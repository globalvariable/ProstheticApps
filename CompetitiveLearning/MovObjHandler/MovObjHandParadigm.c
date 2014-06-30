#include "MovObjHandParadigm.h"



bool submit_cartesian_robotic_space_borders(ThreeDofRobot *robot_arm,  MovObjHandParadigmRobotReach *paradigm, double depth_min, double depth_max, double lateral_min, double lateral_max, double height_min, double height_max)
{
	if (robot_arm->cartesian_security_limits.depth_min >= depth_min)
		return print_message(ERROR_MSG ,"ExperimentHandlers", "MovObjHandParadigm", "submit_cartesian_robotic_space_borders", "robot_arm->cartesian_security_limits.depth_min > depth_min.");
	if (robot_arm->cartesian_security_limits.depth_max <= depth_max)
		return print_message(ERROR_MSG ,"ExperimentHandlers", "MovObjHandParadigm", "submit_cartesian_robotic_space_borders", "robot_arm->cartesian_security_limits.depth_max <= depth_max.");
	if (robot_arm->cartesian_security_limits.lateral_min >= lateral_min)
		return print_message(ERROR_MSG ,"ExperimentHandlers", "MovObjHandParadigm", "submit_cartesian_robotic_space_borders", "robot_arm->cartesian_security_limits.lateral_min >= lateral_min.");
	if (robot_arm->cartesian_security_limits.lateral_max <= lateral_max)
		return print_message(ERROR_MSG ,"ExperimentHandlers", "MovObjHandParadigm", "submit_cartesian_robotic_space_borders", "robot_arm->cartesian_security_limits.lateral_max <= lateral_max.");
	if (robot_arm->cartesian_security_limits.height_min >= height_min)
		return print_message(ERROR_MSG ,"ExperimentHandlers", "MovObjHandParadigm", "submit_cartesian_robotic_space_borders", "robot_arm->cartesian_security_limits.height_min = height_min.");
	if (robot_arm->cartesian_security_limits.height_max <= height_max)
		return print_message(ERROR_MSG ,"ExperimentHandlers", "MovObjHandParadigm", "submit_cartesian_robotic_space_borders", "robot_arm->cartesian_security_limits.height_max <= height_max.");

	paradigm->cartesian_space_limits.depth_min = depth_min;
	paradigm->cartesian_space_limits.depth_max = depth_max;
	paradigm->cartesian_space_limits.lateral_min = lateral_min;
	paradigm->cartesian_space_limits.lateral_max = lateral_max;
	paradigm->cartesian_space_limits.height_min = height_min;
	paradigm->cartesian_space_limits.height_max = height_max;

	return true;
}

bool submit_polar_robotic_space_borders(ThreeDofRobot *robot_arm, MovObjHandParadigmRobotReach *paradigm, double joint_angle_base_lower_limit, double joint_angle_base_upper_limit, double joint_angle_shoulder_lower_limit, double joint_angle_shoulder_upper_limit, double joint_angle_elbow_lower_limit, double joint_angle_elbow_upper_limit)
{
	if (robot_arm->servos[BASE_SERVO].angular_security_limit.min >= joint_angle_base_lower_limit)
		return print_message(ERROR_MSG ,"ExperimentHandlers", "MovObjHandParadigm", "submit_polar_robotic_space_borders", "robot_arm->servos[BASE_SERVO].angular_security_limit.min >= joint_angle_base_lower_limit.");
	if (robot_arm->servos[BASE_SERVO].angular_security_limit.max <= joint_angle_base_upper_limit)
		return print_message(ERROR_MSG ,"ExperimentHandlers", "MovObjHandParadigm", "submit_polar_robotic_space_borders", "robot_arm->servos[BASE_SERVO].angular_security_limit.max <= joint_angle_base_upper_limit.");

	if (robot_arm->servos[SHOULDER_SERVO].angular_security_limit.min >= joint_angle_shoulder_lower_limit)
		return print_message(ERROR_MSG ,"ExperimentHandlers", "MovObjHandParadigm", "submit_polar_robotic_space_borders", "robot_arm->servos[SHOULDER_SERVO].angular_security_limit.min >= joint_angle_shoulder_lower_limit.");
	if (robot_arm->servos[SHOULDER_SERVO].angular_security_limit.max <= joint_angle_shoulder_upper_limit)
		return print_message(ERROR_MSG ,"ExperimentHandlers", "MovObjHandParadigm", "submit_polar_robotic_space_borders", "robot_arm->servos[SHOULDER_SERVO].angular_security_limit.max <= joint_angle_shoulder_upper_limit.");

	if (robot_arm->servos[ELBOW_SERVO].angular_security_limit.min >= joint_angle_elbow_lower_limit)
		return print_message(ERROR_MSG ,"ExperimentHandlers", "MovObjHandParadigm", "submit_polar_robotic_space_borders", "robot_arm->servos[ELBOW_SERVO].angular_security_limit.min >= joint_angle_elbow_lower_limit.");
	if (robot_arm->servos[ELBOW_SERVO].angular_security_limit.max <= joint_angle_elbow_upper_limit)
		return print_message(ERROR_MSG ,"ExperimentHandlers", "MovObjHandParadigm", "submit_polar_robotic_space_borders", "robot_arm->servos[ELBOW_SERVO].angular_security_limit.max <= joint_angle_elbow_upper_limit.");

	paradigm->polar_space_limits[BASE_SERVO].min = joint_angle_base_lower_limit;
	paradigm->polar_space_limits[BASE_SERVO].max = joint_angle_base_upper_limit;
	paradigm->polar_space_limits[SHOULDER_SERVO].min = joint_angle_shoulder_lower_limit;
	paradigm->polar_space_limits[SHOULDER_SERVO].max = joint_angle_shoulder_upper_limit;
	paradigm->polar_space_limits[ELBOW_SERVO].min = joint_angle_elbow_lower_limit;
	paradigm->polar_space_limits[ELBOW_SERVO].max = joint_angle_elbow_upper_limit;
	return true;
}

bool check_robot_space_borders(ThreeDofRobot *robot_arm, MovObjHandParadigmRobotReach *paradigm)
{
	ThreeDofRobotPosition *tip_position =&(robot_arm->tip_position);

	if ((paradigm->cartesian_space_limits.depth_min > tip_position->depth) || (paradigm->cartesian_space_limits.depth_max < tip_position->depth))
		return false;
	if ((paradigm->cartesian_space_limits.lateral_min > tip_position->lateral) || (paradigm->cartesian_space_limits.lateral_max < tip_position->lateral))
		return false;
	if ((paradigm->cartesian_space_limits.height_min > tip_position->height) || (paradigm->cartesian_space_limits.height_max < tip_position->height))
		return false;

	if ((paradigm->polar_space_limits[BASE_SERVO].min > robot_arm->servos[BASE_SERVO].current_angle) || (paradigm->polar_space_limits[BASE_SERVO].max < robot_arm->servos[BASE_SERVO].current_angle))
		return false;
	if ((paradigm->polar_space_limits[SHOULDER_SERVO].min > robot_arm->servos[SHOULDER_SERVO].current_angle) || (paradigm->polar_space_limits[SHOULDER_SERVO].max < robot_arm->servos[SHOULDER_SERVO].current_angle))
		return false;
	if ((paradigm->polar_space_limits[ELBOW_SERVO].min > robot_arm->servos[ELBOW_SERVO].current_angle) || (paradigm->polar_space_limits[ELBOW_SERVO].max < robot_arm->servos[ELBOW_SERVO].current_angle))
		return false;

	return true;
}
