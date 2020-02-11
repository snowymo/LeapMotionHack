using Leap.Unity;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MessageHandler : MonoBehaviour
{

    public Network myUdpTcpNetwork;

    public RigidHand leftHand, rightHand;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    string constructHandInfo()
    {
        string str = "";
        if (leftHand.IsTracked) {
            // valid data
            Vector3 pos = leftHand.GetPalmPosition();
            str += "left hand:" + pos.ToString("F3") + "----";
        }
        if (rightHand.IsTracked) {
            // valid data
            Vector3 pos = rightHand.GetPalmPosition();
            str += "right hand:" + pos.ToString("F3");
        }
        return str;
    }

    // Update is called once per frame
    void Update()
    {
        string curMsg = constructHandInfo();
        if(curMsg.Length > 0)
            myUdpTcpNetwork.SendNetworkMessage("{\"leap_motion\":\"" + curMsg + "\"}");
    }
}
