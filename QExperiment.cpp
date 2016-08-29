/* 
* Copyright (C) 2008, Brian Tanner

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include <stdio.h>	/* for printf */
#include <rlglue/RL_glue.h> /* RL_ function prototypes and RL-Glue types */
#include <iostream>
#include <fstream>
#include <sstream>

int whichEpisode = 0;

std::ofstream outstream;

/* Run One Episode of length maximum cutOff*/
void runEpisode(int stepLimit, std::ofstream &os) {
  int terminal = RL_episode(stepLimit);
  printf("Episode %d\t %d steps \t%f total reward\t %d natural end \n", whichEpisode, RL_num_steps(), RL_return(),
         terminal);
  os << whichEpisode << "," << RL_num_steps() << "," << RL_return() << std::endl;
  whichEpisode++;
}

int main(int argc, char *argv[]) {
  const char *task_spec;
  const char *responseMessage;
  const reward_observation_action_terminal_t *stepResponse;
  const observation_action_t *startResponse;

  printf("\n\nExperiment starting up!\n");

  task_spec = RL_init();
  printf("RL_init called, the environment sent task spec: %s\n", task_spec);

  printf("\n\n----------Sending some sample messages----------\n");
  /*Talk to the agent and environment a bit...*/
  responseMessage = RL_agent_message("what is your name?");
  printf("Agent responded to \"what is your name?\" with: %s\n", responseMessage);
  responseMessage = RL_agent_message("If at first you don't succeed; call it version 1.0");
  printf("Agent responded to \"If at first you don't succeed; call it version 1.0\" with: %s\n\n", responseMessage);

  responseMessage = RL_env_message("what is your name?");
  printf("Environment responded to \"what is your name?\" with: %s\n", responseMessage);
  responseMessage = RL_env_message("If at first you don't succeed; call it version 1.0");
  printf("Environment responded to \"If at first you don't succeed; call it version 1.0\" with: %s\n",
         responseMessage);

  float alphas[] = {0.4, 0.3, 0.5, 0.7};
  for (int k = 0; k < 1; k++) {

    std::ostringstream stringStream;
    stringStream << "Q" << k << ".dat";
    std::ofstream os;

    os.open(stringStream.str());
    printf("\n\n----------Running a few episodes----------\n");
    /* Remember that stepLimit of 0 means there is no limit at all!*/

    int t = time(NULL);

    for (int j = 0; j < 500; j++) {
      RL_init();
      std::stringstream ss;
      ss << "a " << alphas[k];
      responseMessage = RL_agent_message(ss.str().c_str());
      std::cout << "RESPONSE: " << responseMessage;
      srand(t++);
      for (int i = 0; i < 500; i++) {
        runEpisode(0, os);
      }
      RL_cleanup();
    }

    os.close();
  }

  return 0;
}
