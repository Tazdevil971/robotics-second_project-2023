#include <ros/ros.h>
#include <ros/time.h>
#include <actionlib/client/simple_action_client.h>
#include <move_base_msgs/MoveBaseAction.h>

#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

struct Pose {
    float x, y, yaw;
};

class Navigation {
public:
    Navigation() : move_base_client("move_base", true) {}

    void wait_for_server() {
        move_base_client.waitForServer();
    }

    void go_to(Pose pose) {
        move_base_msgs::MoveBaseGoal goal;
        goal.target_pose.header.frame_id = "map";
        goal.target_pose.header.stamp = ros::Time::now();

        goal.target_pose.pose.position.x = pose.x;
        goal.target_pose.pose.position.y = pose.y;

        // Now rebuild the quaternion
        goal.target_pose.pose.orientation.x = 0.0;
        goal.target_pose.pose.orientation.y = 0.0;
        goal.target_pose.pose.orientation.z = std::sin(pose.yaw * 0.5);
        goal.target_pose.pose.orientation.w = std::cos(pose.yaw * 0.5);

        move_base_client.sendGoal(goal);
        move_base_client.waitForResult();
    }

private:

    ros::NodeHandle node;
    actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> move_base_client;
};

int main(int argc, char *argv[]) {
    // Ros initialization
    ros::init(argc, argv, "navigation");
    ros::start();

    Navigation navigation;

    ROS_INFO("Waiting for server!");
    navigation.wait_for_server();
    ROS_INFO("move_base server online!");

    ros::NodeHandle private_node("~");

    std::string file_name;
    private_node.getParam("waypoints_csv", file_name);

    std::ifstream is(file_name);
    
    // Dummy read to get the header
    std::string header;
    std::getline(is, header);
    
    std::string line;
    while(std::getline(is, line)) {
        std::istringstream ss(line);
        
        std::string x, y, yaw;
        std::getline(ss, x, ',');
        std::getline(ss, y, ',');
        std::getline(ss, yaw, ',');
    
        Pose pose = Pose {
            .x = std::stof(x),
            .y = std::stof(y),
            .yaw = std::stof(yaw),
        };

        ROS_INFO("Sending goal x: %f, y: %f, yaw: %f", pose.x, pose.y, pose.yaw);
        navigation.go_to(pose);
    }

    ROS_INFO("Finished all goals!");

    // Final teardown
    ros::shutdown();
    return 0;
}