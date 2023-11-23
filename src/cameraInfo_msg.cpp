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
#include "cameraInfo_msg.h"

using namespace aditof;

CameraInfoMsg::CameraInfoMsg() {}

CameraInfoMsg::CameraInfoMsg(const std::shared_ptr<aditof::Camera> &camera) {

}

void CameraInfoMsg::FrameDataToMsg(const std::shared_ptr<Camera> &camera,
                                   aditof::Frame **frame, rclcpp::Time tStamp) {
    FrameDetails fDetails;
    (*frame)->getDetails(fDetails);

    setMembers(camera, fDetails.width, fDetails.height, tStamp);
}

void CameraInfoMsg::setMembers(const std::shared_ptr<Camera> &camera, int width,
                               int height, rclcpp::Time tStamp) {
    msg.header.stamp = tStamp;
    msg.header.frame_id = "aditof_camera_info";

    msg.width = width;
    msg.height = height;
    msg.distortion_model = "plumb_bob";

    aditof::IntrinsicParameters intr = getIntrinsics(camera);

    msg.d = {intr.k1, intr.k2, intr.p1, intr.p2,
             intr.k3, intr.k4, intr.k5, intr.k6};
    msg.k = {intr.fx / 2, 0.0, intr.cx / 2, 0.0, intr.fy / 2,
             intr.cy / 2, 0.0, 0.0,         1.0};
    msg.r = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
    msg.p = {msg.k[0], msg.k[1], msg.k[2], 0.0f,     msg.k[3], msg.k[4],
             msg.k[5], 0.0f,     msg.k[6], msg.k[7], msg.k[8], 0.0f};

}