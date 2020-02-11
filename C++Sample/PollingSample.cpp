/******************************************************************************\
* Copyright (C) 2012-2017 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Leap Motion and you, your company or other organization.             *
\******************************************************************************/
#include "UDPClient.h"
// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
//#undef __cplusplus

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "LeapC.h"
#include "ExampleConnection.h"

int64_t lastFrameID = 0; //The last frame received

int main(int argc, char** argv) {
  OpenConnection();
  while(!IsConnected)
    millisleep(100); //wait a bit to let the connection complete

  printf("Connected.");
  LEAP_DEVICE_INFO* deviceProps = GetDeviceProperties();
  if(deviceProps)
    printf("Using device %s.\n", deviceProps->serial);

  UDPClient* udpClient = new UDPClient();
  if(argc == 2) // ip
    udpClient = new UDPClient(argv[1]);


  for(;;){
    LEAP_TRACKING_EVENT *frame = GetFrame();
    if(frame && (frame->tracking_frame_id > lastFrameID)){
      lastFrameID = frame->tracking_frame_id;
      printf("Frame %lli with %i hands.\n", (long long int)frame->tracking_frame_id, frame->nHands);
      std::string msg;
      for(uint32_t h = 0; h < frame->nHands; h++){
        LEAP_HAND* hand = &frame->pHands[h];
        printf("    Hand id %i is a %s hand with position (%f, %f, %f).\n",
                    hand->id,
                    (hand->type == eLeapHandType_Left ? "left" : "right"),
                    hand->palm.position.x,
                    hand->palm.position.y,
                    hand->palm.position.z);
        msg += "hand " + std::to_string(hand->id) + " is " + (hand->type == eLeapHandType_Left ? "left" : "right")
            + ", palm pos: (" + std::to_string(hand->palm.position.x) + ","
            + std::to_string(hand->palm.position.y) + ","
            + std::to_string(hand->palm.position.z) + ")\n";
      }
      // sending msg
      udpClient->send((char*)(msg.c_str()), msg.size());
      
    }
  } //ctrl-c to exit
  udpClient->closeConnection();
  return 0;
}
//End-of-Sample
