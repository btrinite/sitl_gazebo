/*
 * Copyright (C) 2012-2016 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/
/* Desc: A basic gimbal controller
 * Author: John Hsu
 */

#ifndef _GAZEBO_GIMBAL_CONTROLLER_PLUGIN_HH_
#define _GAZEBO_GIMBAL_CONTROLLER_PLUGIN_HH_

#include <string>
#include <vector>

#include <gazebo/common/PID.hh>
#include <gazebo/common/Plugin.hh>
#include <gazebo/physics/physics.hh>
#include <gazebo/transport/transport.hh>
#include <gazebo/util/system.hh>
#include <gazebo/sensors/sensors.hh>
#include <ignition/math.hh>

#include "Imu.pb.h"

namespace gazebo
{
  // Default PID gains
  static double kPIDPitchP = 5.0;
  static double kPIDPitchI = 0.0;
  static double kPIDPitchD = 0.0;
  static double kPIDPitchIMax = 0.0;
  static double kPIDPitchIMin = 0.0;
  static double kPIDPitchCmdMax = 0.3;
  static double kPIDPitchCmdMin = -0.3;

  static double kPIDRollP = 5.0;
  static double kPIDRollI = 0.0;
  static double kPIDRollD = 0.0;
  static double kPIDRollIMax = 0.0;
  static double kPIDRollIMin = 0.0;
  static double kPIDRollCmdMax = 0.3;
  static double kPIDRollCmdMin = -0.3;

  static double kPIDYawP = 1.0;
  static double kPIDYawI = 0.0;
  static double kPIDYawD = 0.0;
  static double kPIDYawIMax = 0.0;
  static double kPIDYawIMin = 0.0;
  static double kPIDYawCmdMax = 1.0;
  static double kPIDYawCmdMin = -1.0;

  typedef const boost::shared_ptr<const sensor_msgs::msgs::Imu> ImuPtr;

  class GAZEBO_VISIBLE GimbalControllerPlugin : public ModelPlugin
  {
    /// \brief Constructor
    public: GimbalControllerPlugin();

    public: virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf);

    public: virtual void Init();

    private: void OnUpdate();

    private: void ImuCallback(ImuPtr& imu_message);

#if GAZEBO_MAJOR_VERSION >= 7 && GAZEBO_MINOR_VERSION >= 4
    /// only gazebo 7.4 and above support Any
    private: void OnPitchStringMsg(ConstAnyPtr &_msg);
    private: void OnRollStringMsg(ConstAnyPtr &_msg);
    private: void OnYawStringMsg(ConstAnyPtr &_msg);
#else
    private: void OnPitchStringMsg(ConstGzStringPtr &_msg);
    private: void OnRollStringMsg(ConstGzStringPtr &_msg);
    private: void OnYawStringMsg(ConstGzStringPtr &_msg);
#endif

    /// \TODO something to move into Angle class
    /// \brief returns _angle1 normalized about
    /// (_reference - M_PI, _reference + M_PI]
    /// \param[in] _angle1 input angle
    /// \param[in] _reference reference input angle for normalization
    /// \return normalized _angle1 about _reference
    private: double NormalizeAbout(double _angle, double _reference);

    /// \TODO something to move into Angle class
    /// \brief returns shortest angular distance from _from to _to
    /// \param[in] _from starting anglular position
    /// \param[in] _to end angular position
    /// \return distance traveled from starting to end angular positions
    private: double ShortestAngularDistance(double _from, double _to);

    private: sdf::ElementPtr sdf;

    private: std::vector<event::ConnectionPtr> connections;

    private: transport::SubscriberPtr imuSub;
    private: transport::SubscriberPtr pitchSub;
    private: transport::SubscriberPtr rollSub;
    private: transport::SubscriberPtr yawSub;

    private: transport::PublisherPtr pitchPub;
    private: transport::PublisherPtr rollPub;
    private: transport::PublisherPtr yawPub;

    private: physics::ModelPtr model;

    /// \brief yaw camera
    private: physics::JointPtr yawJoint;

    /// \brief camera roll joint
    private: physics::JointPtr rollJoint;

    /// \brief camera pitch joint
    private: physics::JointPtr pitchJoint;

    private: sensors::ImuSensorPtr cameraImuSensor;
    private: double lastImuYaw;

    private: std::string status;

    private: double pitchCommand;
    private: double yawCommand;
    private: double rollCommand;

    private: transport::NodePtr node;

    private: common::PID pitchPid;
    private: common::PID rollPid;
    private: common::PID yawPid;
    private: common::Time lastUpdateTime;

    private: ignition::math::Vector3d ThreeAxisRot(
      double r11, double r12, double r21, double r31, double r32);
    private: ignition::math::Vector3d QtoZXY(
      const ignition::math::Quaterniond &_q);
  };
}
#endif
