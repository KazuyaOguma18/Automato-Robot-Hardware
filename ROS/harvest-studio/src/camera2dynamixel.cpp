#include "ros/ros.h"
#include "ros/time.h"

#include "std_msgs/Float64.h"
#include "std_msgs/String.h"

#include "trajectory_msgs/JointTrajectory.h"
#include "sensor_msgs/JointState.h"
#include <vector>

std_msgs::Float64 joint_pos;
std_msgs::Float64 joint_vel;
std_msgs::Float64 joint_acc;
std_msgs::Float64 joint_eff;

//コールバックがあるとグローバルに読み込み
void monitorJointState_callback(const sensor_msgs::JointState::ConstPtr& jointstates)
{
    
    if(jointstates->name[0] == "camera_joint"){
        if(jointstates->position.size() > 0){
            joint_pos.data = jointstates->position[0];
        }
        if(jointstates->velocity.size() > 0){
            joint_vel.data = jointstates->velocity[0];
        }
        if(jointstates->effort.size() > 0){
            joint_eff.data = jointstates->effort[0];
        }      
    }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "camera2dynamixel");   // ノードの初期化
  ros::NodeHandle nh;                             // ノードハンドラ

  ros::Subscriber sub_joints;                     // サブスクライバの作成
  sub_joints = nh.subscribe("/camera_angle", 1, monitorJointState_callback);    // moveitから取る

  ros::Publisher arm_pub;                         //パブリッシャの作成
  arm_pub = nh.advertise<trajectory_msgs::JointTrajectory>("/dynamixel_workbench/joint_trajectory",100);

  ros::Rate loop_rate(45);  // 制御周期45Hz(サーボの更新が20msecなのですが、50Hzだと余裕ない感じ)

  trajectory_msgs::JointTrajectory jtp0;
  
  jtp0.header.frame_id = "base_link";   // ポーズ名（モーション名)

  jtp0.joint_names.resize(1); // 名前
  jtp0.joint_names[0] ="camera_joint";
  
  //初期化
  joint_pos.data = 0.0;
  joint_vel.data = 0.0;
  joint_acc.data = 0.0;
  jtp0.points.resize(1);

  jtp0.points[0].positions.resize(1);
  jtp0.points[0].velocities.resize(1);
  jtp0.points[0].accelerations.resize(1);
  jtp0.points[0].effort.resize(1);

  jtp0.points[0].time_from_start = ros::Duration(0.02);  //実行時間20msec (10msecだと動作しない;;)
  
  while (ros::ok()) { // 無限ループ

    jtp0.header.stamp = ros::Time::now();
    
    jtp0.points[0].positions[0] = joint_pos.data;
    jtp0.points[0].velocities[0] = joint_vel.data;
    jtp0.points[0].effort[0] = joint_eff.data;

    arm_pub.publish(jtp0);

    loop_rate.sleep(); // 待ち
    ros::spinOnce();   // コールバック関数を呼ぶ;

  }
  
  return 0;
}