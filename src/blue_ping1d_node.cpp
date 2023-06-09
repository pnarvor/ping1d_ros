#include <iostream>
#include <memory>

#include <rtac_asio/Stream.h>
using namespace rtac::asio;

#include <ping_protocol/PingClient.h>
#include <ping_protocol/messages/Ping1DMessages.h>
using namespace ping_protocol;

#include <blue_ping1d/Profile.h>

#include <ros/ros.h>

#include "conversions.h"

class Ping1DClient : public PingClient
{
    public:

    using Ptr      = std::shared_ptr<Ping1DClient>;
    using ConstPtr = std::shared_ptr<const Ping1DClient>;

    protected:
    
    std::shared_ptr<ros::NodeHandle> node_;
    ros::Publisher                   pingPub_;

    Ping1DClient(std::shared_ptr<ros::NodeHandle> node, const Stream::Ptr& stream) :
        PingClient(stream),
        node_(node)
    {
        std::string pingTopic;
        node_->param<std::string>("ping_topic", pingTopic, "ping");

        pingPub_ = node_->advertise<blue_ping1d::Profile>(pingTopic, 100);
    }

    public:
    
    static Ptr Create(std::shared_ptr<ros::NodeHandle> node,
                      const Stream::Ptr& stream)
    {
        return Ptr(new Ping1DClient(node, stream));
    }

    void message_callback(const ping_protocol::Message& msg) override
    {
        //std::cout << "Message !" << std::endl;
        if(msg.header().message_id == ping1d::Profile::MessageId) {
            auto pingMsg = reinterpret_cast<const ping1d::Profile*>(&msg);
            blue_ping1d::Profile rosMsg;
            copy_to_ros(rosMsg, *pingMsg);
            pingPub_.publish(rosMsg);
        }
        this->async_send(GeneralRequest(1300));
    }
};

int main(int argc, char** argv)
{
    ros::init(argc, argv, "blue_ping1d");

    auto node = std::make_shared<ros::NodeHandle>("blue_ping1d");
    std::string device;
    node->param<std::string>("device", device, "/dev/narval_ping1d");
    int baudrate;
    node->param<int>("baudrate", baudrate, 115200);

    auto client = Ping1DClient::Create(node, Stream::CreateSerial(device, baudrate));
    client->send(GeneralRequest(1300));

    //while(1) {
    //    getchar();
    //    client->send(GeneralRequest(1300));
    //}
    ros::spin();

    return 0;
}



