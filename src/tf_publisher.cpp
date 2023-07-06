#include <ros/ros.h>
#include <ros/time.h>
#include <tf/transform_broadcaster.h>

#include "nav_msgs/Odometry.h"

class TfPublisher {
public:
    TfPublisher() {
        odom = node.subscribe("/t265/odom", 10, &TfPublisher::on_odom, this);
    }

private:
    void on_odom(const boost::shared_ptr<nav_msgs::Odometry>& msg) {
        auto raw_position = msg->pose.pose.position;
        auto raw_orientation = msg->pose.pose.orientation;

        tf::Transform transform;
        transform.setOrigin(tf::Vector3(raw_position.x, raw_position.y, raw_position.z));
        transform.setRotation(tf::Quaternion(raw_orientation.x, raw_orientation.y, raw_orientation.z, raw_orientation.w));

        broadcaster.sendTransform(tf::StampedTransform(transform, msg->header.stamp, "odom", "t265"));
    }

    ros::NodeHandle node;
    
    ros::Subscriber odom;
    tf::TransformBroadcaster broadcaster;
};

int main(int argc, char *argv[]) {
    // Ros initialization
    ros::init(argc, argv, "tf_publisher");
    ros::start();

    TfPublisher node;

    // Main loop
    ros::spin();
    
    // Final teardown
    ros::shutdown();
    return 0;
}