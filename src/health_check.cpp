#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "rosgraph_msgs/msg/clock.hpp"
#include "std_msgs/msg/header.hpp"
#include "adma_ros_driver_msgs/msg/adma_data_scaled.hpp"

using namespace std::chrono_literals;

class Checker : public rclcpp::Node
{
  public:
    Checker() : Node("minimal_subscriber")
    {
      ouster_sub = this->create_subscription<rosgraph_msgs::msg::Clock>
        ("/clock", 10, std::bind(&Checker::ouster_callback, this, std::placeholders::_1));

      roscco_sub = this->create_subscription<std_msgs::msg::Header>
        ("/time_from_roscco", 10, std::bind(&Checker::roscco_callback, this, std::placeholders::_1));
      
      adma_sub = this->create_subscription<adma_ros_driver_msgs::msg::AdmaDataScaled>
        ("/genesys/adma/data_scaled", 10, std::bind(&Checker::adma_callback, this, std::placeholders::_1));

      timer = this->create_wall_timer(2000ms, std::bind(&Checker::timer_callback, this));

      ouster_check = 0;
      ouster_check_prev = 0;

      roscco_check = 0;
      roscco_check_prev = 0;

      adma_check = 0;
      adma_check_prev = 0;

      isOuster = false;
      isRoscco = false;
      isAdma= false;
    }

  private:

    rclcpp::Subscription<rosgraph_msgs::msg::Clock>::SharedPtr ouster_sub;
    rclcpp::Subscription<std_msgs::msg::Header>::SharedPtr roscco_sub;
    rclcpp::Subscription<adma_ros_driver_msgs::msg::AdmaDataScaled>::SharedPtr adma_sub;
    rclcpp::TimerBase::SharedPtr timer;

    bool isOuster;
    bool isRoscco;
    bool isAdma;

    int ouster_check;
    int ouster_check_prev;

    int roscco_check;
    int roscco_check_prev;  

    int adma_check;
    int adma_check_prev;

    void ouster_callback(const rosgraph_msgs::msg::Clock &msg)
    {
      ouster_check = msg.clock.nanosec;
    }    

    void roscco_callback(const std_msgs::msg::Header &msg)
    {
      roscco_check = msg.stamp.nanosec;
    }    

    void adma_callback(const adma_ros_driver_msgs::msg::AdmaDataScaled &msg)
    {
      adma_check = msg.header.stamp.nanosec;
    }

    void timer_callback()
    {

      if(ouster_check_prev != ouster_check)
      {
        isOuster = true;
      }
      else
      {
        isOuster = false;
      }

      if(roscco_check_prev != roscco_check)
      {
       isRoscco = true;
      }
      else
      {
       isRoscco = false;
      }      

      if(adma_check_prev != adma_check)
      {
       isAdma = true;
      }
      else
      {
        isAdma = false;

      }

      adma_check_prev = adma_check;
      roscco_check_prev = roscco_check;
      ouster_check_prev = ouster_check;
      
      RCLCPP_INFO(rclcpp::get_logger(""),"------------HEALTH_CHECK------------");

      if(isOuster)
      {
        RCLCPP_DEBUG(rclcpp::get_logger(""),"             ouster");
      }
      else
      {
        RCLCPP_ERROR(rclcpp::get_logger(""),"             ouster");
      }    

      if(isRoscco)
      {
        RCLCPP_DEBUG(rclcpp::get_logger(""),"             roscco");
      }
      else
      {
        RCLCPP_ERROR(rclcpp::get_logger(""),"             roscco");
      }      

      if(isAdma)
      {
        RCLCPP_DEBUG(rclcpp::get_logger(""),"             adma");
      }
      else
      {
        RCLCPP_ERROR(rclcpp::get_logger(""),"             adma");
      }
    }

};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  std::shared_ptr<Checker> checker_obj = std::make_shared<Checker>();
  rclcpp::spin(checker_obj);
  rclcpp::shutdown();
  return 0;
}
