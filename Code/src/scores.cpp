#include <iostream>
#include <string>
#include "ros/ros.h"
#include "std_msgs/String.h"
#include <std_msgs/Int8.h>
#include <sstream>
#include <vector>
using namespace std;

vector<int> scores;
vector<string> names;

void sortandprint(vector<int> score){

  cout << "\n\nNew highscore list:" << endl;
  cout << "\nName........";
  cout << "Score" << endl;

  //define loop that prints all the scores
  for(int i=0; i<scores.size(); i++){

    int highest = score[0]; //first element in loop will be set to the highest for now
    int rank = 0; //the location of the element with the highest score will be set to 0 for now

    //find the highest score in the vector
    for(int u=1; u<score.size(); u++){
      if(highest<score[u]){
        highest = score[u];
        rank = u;
      }
    }

    //print highest score and name belonging to it
    cout << names[rank] << "........";
    cout << scores[rank] << endl;
    score[rank] = -1; //the highest score will be set to -1 so it wouldn't
                     //be highest next time the loop runs
  }
}

void NameCallback(const std_msgs::String::ConstPtr& msg)
{
    names.push_back(msg->data.c_str());
}

void ScoreCallback(const std_msgs::Int8::ConstPtr& msg)
{
    scores.push_back(msg->data);
    sortandprint(scores);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "scores_node");
  ros::NodeHandle n;
  ros::Subscriber name_sub = n.subscribe("Name", 10, NameCallback); //sends to Callback()
  ros::Subscriber score_sub = n.subscribe("Score", 10, ScoreCallback); //sends to Callback()

  ros::spin();
  return 0;
}
