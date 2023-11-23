/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2019, Analog Devices, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "publisher_factory.h"

PublisherFactory::PublisherFactory(){}

void PublisherFactory::createNew(
  rclcpp::Node * node, const std::shared_ptr<aditof::Camera> & camera, aditof::Frame ** frame,
  bool enableDepthCompute)
{
  // Get frame types
  aditof::CameraDetails * details_tmp = new aditof::CameraDetails;
  getCameraDataDetails(camera, *details_tmp);
  //rclcpp::Time timeStamp= node->get_clock()->now();
  cameraInfoPublisher = node->create_publisher<sensor_msgs::msg::CameraInfo>("/aditof_roscpp/aditof_camera_info", 2);
  cameraInfoMsg = std::make_shared<CameraInfoMsg>(camera);

  for (auto iter : (*details_tmp).frameType.dataDetails) {
   
	  if (!strcmp(iter.type.c_str(), "ir")) {
      img_publishers.emplace_back(
        node->create_publisher<sensor_msgs::msg::Image>("/aditof_roscpp/aditof_ir", 10));
      imgMsgs.emplace_back(new IRImageMsg(camera, frame, sensor_msgs::image_encodings::MONO16));
      LOG(INFO) << "Added ir publisher";
    } 
    
    if (!strcmp(iter.type.c_str(), "depth")) {
      img_publishers.emplace_back(
        node->create_publisher<sensor_msgs::msg::Image>("/aditof_roscpp/aditof_depth", 10));
      imgMsgs.emplace_back(new DepthImageMsg(camera, frame, sensor_msgs::image_encodings::MONO16));
      LOG(INFO) << "Added depth publisher";
    }
   /* 
     else if (!strcmp(iter.type.c_str(), "raw")) {
      img_publishers.emplace_back(
        node->create_publisher<sensor_msgs::msg::Image>("tof_camera/raw", 2));
      imgMsgs.emplace_back(new RAWImageMsg(camera, frame, sensor_msgs::image_encodings::MONO16));
      LOG(INFO) << "Added raw data publisher";
    } else if (!strcmp(iter.type.c_str(), "xyz")) {
      pointCloud_publisher.emplace_back(
        node->create_publisher<sensor_msgs::msg::PointCloud2>("tof_camera/xyz", 2));
      pointCloudMsgs.emplace_back(
        new XYZImageMsg(camera, frame, sensor_msgs::image_encodings::MONO16));
      LOG(INFO) << "Added xyz data publisher";
    } else if (!strcmp(iter.type.c_str(), "conf")) {
      img_publishers.emplace_back(
        node->create_publisher<sensor_msgs::msg::Image>("tof_camera/conf", 2));
      imgMsgs.emplace_back(new ConfImageMsg(camera, frame, sensor_msgs::image_encodings::MONO16));
      LOG(INFO) << "Added conf data publisher";
    }
    */
  }
  // startCamera(camera);
}
void PublisherFactory::updatePublishers(
  const std::shared_ptr<aditof::Camera> & camera, aditof::Frame ** frame, rclcpp::Time timeStamp)
{
 
  for (unsigned int i = 0; i < imgMsgs.size(); ++i) {
    imgMsgs.at(i)->FrameDataToMsg(camera, frame, timeStamp);
    imgMsgs.at(i)->publishMsg(*img_publishers.at(i));

  }
  cameraInfoMsg->FrameDataToMsg(camera, frame, timeStamp);
  cameraInfoPublisher->publish(cameraInfoMsg->getMessage());

/*
  for (unsigned int i = 0; i < pointCloudMsgs.size(); ++i) {
    pointCloudMsgs.at(i)->FrameDataToMsg(camera, frame);
    pointCloudMsgs.at(i)->publishMsg(*pointCloud_publisher.at(i));
  }
*/
}
void PublisherFactory::deletePublishers(const std::shared_ptr<aditof::Camera> & camera)
{
  stopCamera(camera);
  img_publishers.clear();
  imgMsgs.clear();
}
void PublisherFactory::setDepthFormat(const int val)
{
  for (unsigned int i = 0; i < imgMsgs.size(); ++i) {
    if (std::dynamic_pointer_cast<DepthImageMsg>(imgMsgs[i])) {
      std::dynamic_pointer_cast<DepthImageMsg>(imgMsgs[i]).get()->setDepthDataFormat(val);
    }
  }
}
