define robot_scan ranger
(
    sensor
    (
        pose [0.0 0.0 0.0 0.0]
        size [0.01 0.01 0.1]
        range [0 10.0]
        fov 350
        samples 3488
        color "green"
    )
)

define robot position
(
    pose [0.0 0.0 0.0 0.0]
    localization "odom"
    odom_error [0.0 0.0 0.0 0.0]

    size [0.6 0.4 0.10]
    gui_nose 1

    drive "diff"
    color "blue"

    robot_scan(pose [0.18 0.0 0.07 0.0])
)