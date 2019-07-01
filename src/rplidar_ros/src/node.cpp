/*
 *  RPLIDAR ROS NODE

 *
 */


#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "std_srvs/Empty.h"
#include "sensor_msgs/Imu.h"
#include "rplidar.h"



#include "windows.h"
#include "C:/usr/include/rplidar/RPlidarNeedfullsForDLL.h"
#include "c:/usr/include/gregutils/strguple.h"
#include "C:/catkin_ws/devel/include/rplidar_ros/tilter.h"

#include "C:/usr/include/WIT/WITReader.h"
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>

INIT_STRGUPLE

#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif

#define DEG2RAD(x) ((x)*M_PI/180.)

using namespace rp::standalone::rplidar;

RPlidarDriver * drv = NULL;
ros::Publisher sp_pub_WIT;
std::shared_ptr<WITAsio> sp_ws;


void publish_scan(ros::Publisher *pub,
                  rplidar_response_measurement_node_hq_t *nodes,
                  size_t node_count, ros::Time start,
                  double scan_time, bool inverted,
                  float angle_min, float angle_max,
                  float max_distance,
                  std::string frame_id)
{
	SGUP_DEBUGLINE
    static int scan_count = 0;
    sensor_msgs::LaserScan scan_msg;

    scan_msg.header.stamp = start;
    scan_msg.header.frame_id = frame_id;
    scan_count++;

    bool reversed = (angle_max > angle_min);
    if ( reversed ) {
      scan_msg.angle_min =  M_PI - angle_max;
      scan_msg.angle_max =  M_PI - angle_min;
    } else {
      scan_msg.angle_min =  M_PI - angle_min;
      scan_msg.angle_max =  M_PI - angle_max;
    }
    scan_msg.angle_increment =
        (scan_msg.angle_max - scan_msg.angle_min) / (double)(node_count-1);

    scan_msg.scan_time = scan_time;
    scan_msg.time_increment = scan_time / (double)(node_count-1);
    scan_msg.range_min = 0.15;
    scan_msg.range_max = max_distance;//8.0;
	SGUP_DEBUGLINE
    scan_msg.intensities.resize(node_count);
    scan_msg.ranges.resize(node_count);
    bool reverse_data = (!inverted && reversed) || (inverted && !reversed);

	
    if (!reverse_data) {
        for (size_t i = 0; i < node_count; i++) {
            float read_value = (float) nodes[i].dist_mm_q2/4.0f/1000;
            if (read_value == 0.0)
                scan_msg.ranges[i] = std::numeric_limits<float>::infinity();
            else
                scan_msg.ranges[i] = read_value;
            scan_msg.intensities[i] = (float) (nodes[i].quality >> 2);
        }
    } else {
        for (size_t i = 0; i < node_count; i++) {
            float read_value = (float)nodes[i].dist_mm_q2/4.0f/1000;
            if (read_value == 0.0)
                scan_msg.ranges[node_count-1-i] = std::numeric_limits<float>::infinity();
            else
                scan_msg.ranges[node_count-1-i] = read_value;
            scan_msg.intensities[node_count-1-i] = (float) (nodes[i].quality >> 2);
        }
    }
	SGUP_DEBUGLINE
    pub->publish(scan_msg);
	SGUP_DEBUGLINE
}

bool getRPLIDARDeviceInfo(RPlidarDriver * drv)
{
    u_result     op_result;
    rplidar_response_device_info_t devinfo;

    op_result = drv->getDeviceInfo(devinfo);
    if (IS_FAIL(op_result)) {
        if (op_result == RESULT_OPERATION_TIMEOUT) {
            ROS_ERROR("Error, operation time out. RESULT_OPERATION_TIMEOUT! ");
        } else {
            ROS_ERROR("Error, unexpected error, code: %x",op_result);
        }
        return false;
    }

    // print out the device serial number, firmware and hardware version number..
    printf("RPLIDAR S/N: ");
    for (int pos = 0; pos < 16 ;++pos) {
        printf("%02X", devinfo.serialnum[pos]);
    }
    printf("\n");
    ROS_INFO("Firmware Ver: %d.%02d",devinfo.firmware_version>>8, devinfo.firmware_version & 0xFF);
    ROS_INFO("Hardware Rev: %d",(int)devinfo.hardware_version);
    return true;
}

bool checkRPLIDARHealth(RPlidarDriver * drv)
{
    u_result     op_result;
    rplidar_response_device_health_t healthinfo;

    op_result = drv->getHealth(healthinfo);
    if (IS_OK(op_result)) { 
        ROS_INFO("RPLidar health status : %d", healthinfo.status);
        if (healthinfo.status == RPLIDAR_STATUS_ERROR) {
            ROS_ERROR("Error, rplidar internal error detected. Please reboot the device to retry.");
            return false;
        } else {
            return true;
        }

    } else {
        ROS_ERROR("Error, cannot retrieve rplidar health code: %x", op_result);
        return false;
    }
}

bool stop_motor(std_srvs::Empty::Request &req,
                               std_srvs::Empty::Response &res)
{
  if(!drv)
       return false;

  ROS_DEBUG("Stop motor");
  drv->stop();
  drv->stopMotor();
  return true;
}

bool start_motor(std_srvs::Empty::Request &req,
                               std_srvs::Empty::Response &res)
{
  if(!drv)
       return false;
  ROS_DEBUG("Start motor");
  drv->startMotor();
  drv->startScan(0,1);
  return true;
}

static float getAngle(const rplidar_response_measurement_node_hq_t& node)
{
    return node.angle_z_q14 * 90.f / 16384.f;
}

INIT_RP_RPLIDAR_PROXY

#include <functional>

bool tiltercommand(rplidar_ros::tilter::Request &req, rplidar_ros::tilter::Response & res)
{

	std::function<void(void)> fnAbsoluteTilt= [&]() {
		SG2("tilt lambda called, command:", req.command)

			res.result = "OK";
	};

	std::map<std::string, decltype(fnAbsoluteTilt) > fmap
		= {
		{"tilt",fnAbsoluteTilt}
	
	};

	try {
		fmap[req.command]();
	}
	catch (...) {
		SG2("called tiltercommane with not recognized, error:", req.command);
	}
	   


	return true;
}



//#WITAsio::cbtAngles
void anglecb(WITAsio::Angles & a) {

	sensor_msgs::Imu im;
	tf2::Quaternion myQuaternion;
	
	myQuaternion.setRPY(a.roll, a.pitch,a.yaw);
	myQuaternion.normalize();

	tf2::convert(im.orientation, myQuaternion);

	/*im.orientation.w = myQuaternion.getW();
	im.orientation.x = myQuaternion.getX();
	im.orientation.y = myQuaternion.getY();
	im.orientation.z = myQuaternion.getZ();*/

	im.orientation_covariance[0] = -1;

	//publish it?
	if(WITAsio::isReading()) //this tells me there is a living thread for wit
		sp_pub_WIT.publish(im);

	//im.orientation
	ros::spinOnce();

}



void killWITReader(boost::thread * t)
{
	if(sp_ws)
		WITAsio::interrupt();
		//ideally we should wait, but whatever, let the host worry about it

}


void spinWITReader(std::string comWIT) {
	sp_ws.reset( new WITAsio(comWIT));
	sp_ws->cbAngles = anglecb;
	sp_ws->runThreaded();
}




std::string getPort(std::string desc) {


	std::map<std::string, std::pair<std::string, std::string> > defaults = {
		{"serial_port",{"CP210x","COM3"}}, //rplidar
		{"tilter_serial_port",{"Arduino",""}},
		{"witdevice_serial_port",{"CH340","COM9"}}

		};

	auto isThereComInEnv=rp::RplidarProxy::getEnvVar(desc);

	if (isThereComInEnv) {
		SGUP_ROS_INFO("COM Port exists in ENV:",*isThereComInEnv);
		return *isThereComInEnv;
	}

#ifdef WITH_GREGS_RPLIDAR_DLL
	auto p=rp::RplidarProxy::findRplidarComPort(defaults[desc].first); 

	if (p) {
		//auto s=STRGUPLE(__FUNCTION__, *p);
		SGUP_ROS_INFO(__FUNCTION__,"returning:", *p);
		return *p;
	}
	else {
		SGUP_ROS_ERROR(__FUNCTION__, "did not find a port for:", desc);
		return "COM0";
	}
	
#else
	
	return defaults[desc].second;

#endif

}

//C:\catkin_ws\src\rplidar_ros\launch\rplidar_a3.launch
int main(int argc, char * argv[]) {
	using namespace rp::helpers;




	try {
		ros::init(argc, argv, "rplidar_node");
	}
	catch (...)
	{

	}

#ifdef WITH_GREGS_RPLIDAR_DLL
#ifdef _DEBUG
	HMODULE h = LoadLibraryA(R"(C:\repos\lidar\rplidar_sdk\sdk\workspaces\vc10\x64\Debug\rplidarReader.dll)");
#else
	HMODULE h = LoadLibraryA(R"(C:\repos\lidar\rplidar_sdk\sdk\workspaces\vc10\x64\Release\rplidarReader.dll)");
#endif
	RP_INIT_DLL_FUNCTIONS(h);
#endif

    std::string channel_type;
    std::string tcp_ip;
    std::string serial_port;
    int tcp_port = 20108;
    int serial_baudrate = 256000;
    std::string frame_id;
    bool inverted = false;
    bool angle_compensate = true;
    float max_distance = 8.0;
    int angle_compensate_multiple = 1;//it stand of angle compensate at per 1 degree
    std::string scan_mode;
	std::string topic;
	std::string topicWIT;
	std::string witdevice_serial_port;
	std::optional<std::string> tilter_sp;
    ros::NodeHandle nh;
    ros::NodeHandle nh_private("~");
	std::string tilter_serial_port;
	bool useWIT ;
	bool useTilter;
	bool useRplidar;
	
	nh_private.param<std::string>("topic", topic, "rplidarScan");
	nh_private.param<std::string>("topicWIT", topicWIT, "rplidarWIT");
    
	ros::Publisher scan_pub = nh.advertise<sensor_msgs::LaserScan>(topic, 1000);


    nh_private.param<std::string>("channel_type", channel_type, "serial");
    nh_private.param<std::string>("tcp_ip", tcp_ip, "192.168.0.7"); 
    nh_private.param<int>("tcp_port", tcp_port, 20108);

	//we are looking up serialports even if they not in (commented out) the .launch, getPort will return them
	//but they will only be opened if the useService corresponding service = true in the .launch file
    nh_private.param<std::string>("serial_port", serial_port, getPort("serial_port")); 
	nh_private.param<std::string>("tilter_serial_port", tilter_serial_port, getPort("tilter_serial_port"));
	nh_private.param<std::string>("witdevice_serial_port", witdevice_serial_port, getPort("witdevice_serial_port"));
	
	nh_private.param<bool>("useWIT", useWIT, false);
	nh_private.param<bool>("useTilter", useTilter, false);
	nh_private.param<bool>("useRplidar", useRplidar, true); //assumed to be true always, not used yet in this src

	
	if (useWIT) {
		ROS_INFO("Launching WIT reader:%s", witdevice_serial_port.c_str() );
		sp_pub_WIT = nh.advertise<sensor_msgs::Imu>(topicWIT, 1000);
		spinWITReader(witdevice_serial_port);

	}

	if (useTilter) {
		//launch tilter based stuff, nothing here yet though
	}



    nh_private.param<int>("serial_baudrate", serial_baudrate, 256000);//ros run for A1 A2, change to 256000 if A3
    nh_private.param<std::string>("frame_id", frame_id, "laser_frame");
    nh_private.param<bool>("inverted", inverted, false);
    nh_private.param<bool>("angle_compensate", angle_compensate, false);
    nh_private.param<std::string>("scan_mode", scan_mode, std::string());

    ROS_INFO("RPLIDAR running on ROS package rplidar_ros. SDK Version:" RPLIDAR_SDK_VERSION);

    u_result     op_result;

    // create the driver instance
    if(channel_type == "tcp"){
        drv = RPlidarDriver::CreateDriver(rp::standalone::rplidar::DRIVER_TYPE_TCP);
    }
    else{
        drv = RPlidarDriver::CreateDriver(rp::standalone::rplidar::DRIVER_TYPE_SERIALPORT);
    }

    
    if (!drv) {
        ROS_ERROR("Create Driver fail, exit");
        return -2;
    }

    if(channel_type == "tcp"){
        // make connection...
        if (IS_FAIL(drv->connect(tcp_ip.c_str(), (_u32)tcp_port))) {
            ROS_ERROR("Error, cannot bind to the specified serial port %s.",serial_port.c_str());
            RPlidarDriver::DisposeDriver(drv);
            return -1;
        }

    }
    else{
       // make connection...
        if (IS_FAIL(drv->connect(serial_port.c_str(), (_u32)serial_baudrate))) {
            ROS_ERROR("Error, cannot bind to the specified serial port %s.",serial_port.c_str());
            RPlidarDriver::DisposeDriver(drv);
            return -1;
        }
		 
    } 
    
    // get rplidar device info
    if (!getRPLIDARDeviceInfo(drv)) {
        return -1;
    }

    // check health...
    if (!checkRPLIDARHealth(drv)) {
        RPlidarDriver::DisposeDriver(drv);
        return -1;
    }

    ros::ServiceServer stop_motor_service = nh.advertiseService("stop_motor", stop_motor);
    ros::ServiceServer start_motor_service = nh.advertiseService("start_motor", start_motor);

	ros::ServiceServer service = nh.advertiseService("tilter", tiltercommand);


    drv->startMotor();

    RplidarScanMode current_scan_mode;
    if (scan_mode.empty()) {
        op_result = drv->startScan(false /* not force scan */, true /* use typical scan mode */, 0, &current_scan_mode);
    } else {
        std::vector<RplidarScanMode> allSupportedScanModes;
        op_result = drv->getAllSupportedScanModes(allSupportedScanModes);

        if (IS_OK(op_result)) {
            _u16 selectedScanMode = _u16(-1);
            for (std::vector<RplidarScanMode>::iterator iter = allSupportedScanModes.begin(); iter != allSupportedScanModes.end(); iter++) {
                if (iter->scan_mode == scan_mode) {
                    selectedScanMode = iter->id;
                    break;
                }
            }

            if (selectedScanMode == _u16(-1)) {
                ROS_ERROR("scan mode `%s' is not supported by lidar, supported modes:", scan_mode.c_str());
                for (std::vector<RplidarScanMode>::iterator iter = allSupportedScanModes.begin(); iter != allSupportedScanModes.end(); iter++) {
                    ROS_ERROR("\t%s: max_distance: %.1f m, Point number: %.1fK",  iter->scan_mode,
                            iter->max_distance, (1000/iter->us_per_sample));
                }
                op_result = RESULT_OPERATION_FAIL;
            } else {
                op_result = drv->startScanExpress(false /* not force scan */, selectedScanMode, 0, &current_scan_mode);
            }
        }
    }

    if(IS_OK(op_result))
    {
        //default frequent is 10 hz (by motor pwm value),  current_scan_mode.us_per_sample is the number of scan point per us
        angle_compensate_multiple = (int)(1000*1000/current_scan_mode.us_per_sample/10.0/360.0);
        if(angle_compensate_multiple < 1) 
          angle_compensate_multiple = 1;
        max_distance = current_scan_mode.max_distance;
        ROS_INFO("current scan mode: %s, max_distance: %.1f m, Point number: %.1fK , angle_compensate: %d",  current_scan_mode.scan_mode,
                 current_scan_mode.max_distance, (1000/current_scan_mode.us_per_sample), angle_compensate_multiple);
    }
    else
    {
        ROS_ERROR("Can not start scan: %08x!", op_result);
    }

    ros::Time start_scan_time;
    ros::Time end_scan_time;
    double scan_duration;


	if (ros::ok() && useWIT) {
		spinWITReader(witdevice_serial_port);
	}


    while (ros::ok()) {
        rplidar_response_measurement_node_hq_t nodes[360*8];
        size_t   count = _countof(nodes);

        start_scan_time = ros::Time::now();
        op_result = drv->grabScanDataHq(nodes, count);
        end_scan_time = ros::Time::now();
        scan_duration = (end_scan_time - start_scan_time).toSec();

        if (op_result == RESULT_OK) {
            op_result = drv->ascendScanData(nodes, count);
            float angle_min = DEG2RAD(0.0f);
            float angle_max = DEG2RAD(359.0f);
            if (op_result == RESULT_OK) {
                if (angle_compensate) {
                    //const int angle_compensate_multiple = 1;
                    const int angle_compensate_nodes_count = 360*angle_compensate_multiple; 
				   
                    int angle_compensate_offset = 0;
              //      rplidar_response_measurement_node_hq_t  angle_compensate_nodes[angle_compensate_nodes_count]; //greg
					rplidar_response_measurement_node_hq_t  * angle_compensate_nodes=new rplidar_response_measurement_node_hq_t[angle_compensate_nodes_count];
                    memset((void*)angle_compensate_nodes, 0, angle_compensate_nodes_count*sizeof(rplidar_response_measurement_node_hq_t));

                    int i = 0, j = 0;
                    for( ; i < count; i++ ) {
                        if (nodes[i].dist_mm_q2 != 0) {
                            float angle = getAngle(nodes[i]);
                            int angle_value = (int)(angle * angle_compensate_multiple);
                            if ((angle_value - angle_compensate_offset) < 0) angle_compensate_offset = angle_value;
                            for (j = 0; j < angle_compensate_multiple; j++) {
                                angle_compensate_nodes[angle_value-angle_compensate_offset+j] = nodes[i];
                            }
                        }
                    }
  
                    publish_scan(&scan_pub, angle_compensate_nodes, angle_compensate_nodes_count,
                             start_scan_time, scan_duration, inverted,
                             angle_min, angle_max, max_distance,
                             frame_id);
							 
					delete angle_compensate_nodes;
                } else {
                    int start_node = 0, end_node = 0;
                    int i = 0;
                    // find the first valid node and last valid node
                    while (nodes[i++].dist_mm_q2 == 0);
                    start_node = i-1;
                    i = count -1;
                    while (nodes[i--].dist_mm_q2 == 0);
                    end_node = i+1;

                    angle_min = DEG2RAD(getAngle(nodes[start_node]));
                    angle_max = DEG2RAD(getAngle(nodes[end_node]));

                    publish_scan(&scan_pub, &nodes[start_node], end_node-start_node +1,
                             start_scan_time, scan_duration, inverted,
                             angle_min, angle_max, max_distance,
                             frame_id);
               }
            } else if (op_result == RESULT_OPERATION_FAIL) {
                // All the data is invalid, just publish them
                float angle_min = DEG2RAD(0.0f);
                float angle_max = DEG2RAD(359.0f);

                publish_scan(&scan_pub, nodes, count,
                             start_scan_time, scan_duration, inverted,
                             angle_min, angle_max, max_distance,
                             frame_id);
            }
        }

        ros::spinOnce();
    }

	if(WITAsio::isReading())
		WITAsio::interrupt();
    // done!
    drv->stop();
    drv->stopMotor();
    RPlidarDriver::DisposeDriver(drv);
    return 0;
}
