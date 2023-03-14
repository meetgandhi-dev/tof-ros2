# ROS2 Wrapper for [Time of Flight SDK](https://github.com/analogdevicesinc/ToF) of Analog Devices (for Ubuntu)


# 1. Install ROS2

- Install the recommended [ROS2 distribution](https://docs.ros.org/en/rolling/Releases.html) for your operating system**
  - [ROS Install page](https://docs.ros.org/en/foxy/Installation.html)

- In order to prepare the system to run the ROS wrapper in the general catkin workspace make sure to install correctly the following libraries:

# 2. ToF dependency
## Download debian package:
* ([Tof lib for Ubuntu 18.04](https://swdownloads.analog.com/cse/aditof/tof_deb_pkg/out_ubuntu18/tof_lib.deb))
* ([Tof lib for Ubuntu 20.04](https://swdownloads.analog.com/cse/aditof/tof_deb_pkg/out_ubuntu20/tof_lib.deb))
* ([Tof lib for Ubuntu 22.04](https://swdownloads.analog.com/cse/aditof/tof_deb_pkg/out_ubuntu22/tof_lib.deb))

Install command: ```sudo dpkg -i tof_lib.deb```
## Building from sources
In order to prepare the system to run the ROS wrapper in the general catkin workspace make sure to install correctly the following library:

- [Build ToF from sourcecode](https://github.com/analogdevicesinc/ToF/blob/master/doc/itof/linux_build_instructions.md) (Until the step: "Download and build the SDK only")

Also make sure to run ```sudo make install``` at the end of the build

# 3. Usage

In directory ```ros2_ws/src/``` clone the repository:

```console
git clone https://github.com/analogdevicesinc/tof-ros2
```

After cloning the repository in the ``ros2_ws/ run the following command:
 
```console
colcon build
source devel/setup.bash
```

### Starting camera node
- In the general ROS2 workspace run the following code, setting up the path towards shaed library:
```console
export LD_LIBRARY_PATH="$LD_LIBRARY_PATH;/opt/websockets/lib;/usr/local/lib"
```
- Starting the node
```console
    ros2 run tof_ros2cpp tof_ros2cpp ip="10.42.0.1" config_file="tof_config/config_walden_3500_nxp.json" mode=1
```
### Parameters
 [config_file:"<<b>path></b>"]

* ```config/config_adsd3500_adsd3030_new_modes.json```
* ```config/config_adsd3500_adsd3030_old_modes.json```
* ```config/config_crosby_adsd3500_new_modes.json```
* ```config/config_crosby_nxp.json```
* ```config/config_crosby_old_modes.json```



 [use_depthCompute] 
 - "true" for enabling Depth Compute libraries
 - "false" for disabling Depth Compute libraries 


 [mode]:

|          | New modes                                                                      | Old modes                                               |
|----------|--------------------------------------------------------------------------------|---------------------------------------------------------|
| adsd3500 | mode 0 - sr-native; mode 1 - lr-native; mode 2 - sr-qnative; mode 3 - lr-qnative  | mode 0 - lt_bin; mode 1 - pcmmp; mode 2 - qmp; mode 3 - mp |
| adsd3030 | mode 0 - sr-native; mode 1 - lr-native; mode 2 - sr-qnative; mode 3 - lr-qnative  | mode 0 - vga                                            |



