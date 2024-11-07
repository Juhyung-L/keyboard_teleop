#include<unistd.h>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/twist.hpp"

#include"SDL.h"

void cleanUp() {
    SDL_Quit();
    rclcpp::shutdown();
}

int main(int argc, char** argv) {

    rclcpp::init(argc,argv);

    auto node_ = rclcpp::Node::make_shared("keyboard_teleop");
    node_->declare_parameter("vx", 1.0);
    node_->declare_parameter("vyaw", 1.0);
    node_->declare_parameter("twist_topic", rclcpp::ParameterValue("cmd_vel"));
    
    std::string twist_topic;
    node_->get_parameter("twist_topic", twist_topic);
    auto pub_ = node_->create_publisher<geometry_msgs::msg::Twist>(twist_topic, rclcpp::SystemDefaultsQoS());
    
    double vx;
    double vyaw;
    node_->get_parameter("vx", vx);
    node_->get_parameter("vyaw", vyaw);

    geometry_msgs::msg::Twist robot_vel;
    SDL_Event event;
    
    if (SDL_Init(SDL_INIT_EVENTS) < 0)
    {
        fprintf(stderr, "Could not initialise SDL: %s\n", SDL_GetError());
        cleanUp();
        return 0;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "Could not initialise SDL: %s\n", SDL_GetError());
        cleanUp();
        return 0;
    }

    SDL_Window* window = SDL_CreateWindow(
        "ROS Teleop",                  // window title
        SDL_WINDOWPOS_UNDEFINED,       // initial x position
        SDL_WINDOWPOS_UNDEFINED,       // initial y position
        100,                           // width, in pixels
        100,                           // height, in pixels
        SDL_WINDOW_SHOWN               // flags
    );

    if(window == nullptr)
    {
        fprintf(stderr, "Could not create SDL window: %s\n", SDL_GetError());
        cleanUp();
        return 0;
    }

    bool is_running{true};
    while (is_running && rclcpp::ok())
    {
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                // check if even is keyup or keydown
                case SDL_KEYDOWN:
                    // check which key was pressed
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_w:
                            robot_vel.linear.x = vx;
                            break;
                        case SDLK_a:
                            robot_vel.angular.z = vyaw;
                            break;
                        case SDLK_s:
                            robot_vel.linear.x = -vx;
                            break;
                        case SDLK_d:
                            robot_vel.angular.z = -vyaw;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    // check which key was released
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_w:
                            if(robot_vel.linear.x > 0.0)
                                robot_vel.linear.x = 0.0;
                            break;
                        case SDLK_a:
                            if(robot_vel.angular.z > 0.0)
                                robot_vel.angular.z = 0.0;
                            break;
                        case SDLK_s:
                            if(robot_vel.linear.x < 0.0)
                                robot_vel.linear.x = 0.0;
                            break;
                        case SDLK_d:
                            if(robot_vel.angular.z < 0.0)
                                robot_vel.angular.z = 0.0;
                            break;
                        default:
                            break;
                    }
                    break;
                case SDL_QUIT:
                    is_running = false;
                    break;
                default:
                    break;
            }

            pub_->publish(robot_vel);
        }
        usleep(10000); // 100Hz
    }

    SDL_DestroyWindow(window);
    cleanUp();
    return 0;
}