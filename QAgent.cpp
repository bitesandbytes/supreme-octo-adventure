/* 
 */

#include <stdio.h>  /* for printf */
#include <string.h> /* for strcmp */
#include <time.h> /*for time()*/
#include <rlglue/Agent_common.h> /* agent_ function prototypes and RL-Glue types */
#include <rlglue/utils/C/RLStruct_util.h> /* helpful functions for allocating structs and cleaning them up */
#include "utils/utils.h"

#define EPSILON 0.1
#define GAMMA 0.9

action_t this_action;
action_t last_action;

observation_t *last_observation = 0;

float alpha = 0.5;

#define MAX_X 12
#define MAX_Y 12
#define MAX_ACTIONS 4

double *q;

void agent_init(const char *task_spec) {
  /*Seed the random number generator*/
  srand(time(0));
  /*Here is where you might allocate storage for parameters (value function or policy, last action, last observation, etc)*/
  q = new double[MAX_X * MAX_Y * MAX_ACTIONS];
  /*Here you would parse the task spec if you felt like it*/

  /*Allocate memory for a one-dimensional integer action using utility functions from RLStruct_util*/
  allocateRLStruct(&this_action, 1, 0, 0);
  last_observation = allocateRLStructPointer(2, 0, 0);
  /* That is equivalent to:
           this_action.numInts     =  1;
           this_action.intArray    = (int*)calloc(1,sizeof(int));
           this_action.numDoubles  = 0;
           this_action.doubleArray = 0;
           this_action.numChars    = 0;
           this_action.charArray   = 0;
  */

  for (int i = 0; i < MAX_X; i++)
    for (int j = 0; j < MAX_Y; j++)
      for (int k = 0; k < MAX_ACTIONS; k++)
        q[i * MAX_Y * MAX_ACTIONS + j * MAX_ACTIONS + k] = 0;

}

int maxvec[MAX_ACTIONS];

int maxAction(int cX, int cY) {
  int qmax = -1000;
  int max = -1;
  int num_max = 0;
  //printf("X: %d, Y: %d\n", cX, cY );
  //printf("Q1: %lf\n", q[ MAX_Y * MAX_ACTIONS * cX + MAX_ACTIONS * cY + 0] );
  //printf("Q2: %lf\n", q[ MAX_Y * MAX_ACTIONS * cX + MAX_ACTIONS * cY + 1] );
  //printf("Q3: %lf\n", q[ MAX_Y * MAX_ACTIONS * cX + MAX_ACTIONS * cY + 2] );
  //printf("Q4: %lf\n", q[ MAX_Y * MAX_ACTIONS * cX + MAX_ACTIONS * cY + 3] );
  for (int i = 0; i < MAX_ACTIONS; i++) {
    if (q[MAX_Y * MAX_ACTIONS * cX + MAX_ACTIONS * cY + i] > qmax) {
      qmax = q[MAX_Y * MAX_ACTIONS * cX + MAX_ACTIONS * cY + i];
      max = i;
      num_max = 1;
      for (int j = 0; j < MAX_ACTIONS; j++) {
        maxvec[j] = 0;
      }
      maxvec[i] = 1;
    } else if (q[MAX_Y * MAX_ACTIONS * cX + MAX_ACTIONS * cY + i] == qmax) {
      num_max++;
      maxvec[i] = 1;
    }
  }
  //printf("NUM MAX: %d\n", num_max);
  if (num_max == 1)
    return max;

  int num = Utils::randInRange(num_max - 1);
  //printf("RAND: %d\n", num);
  for (int k = 0; k < MAX_ACTIONS; k++) {
    if (maxvec[k]) {
      if (num == 0)
        return k;

      num--;
    }
  }
  //return randInRange(4);
}

const action_t *agent_start(const observation_t *this_observation) {
  // Tabular Q-learning implementation.

  // Get current state
  int cX = this_observation->intArray[0];
  int cY = this_observation->intArray[1];

  int action = -1;

  // Calculate an action.
  if (Utils::doubleRand() > EPSILON)
    action = maxAction(cX, cY);
  else
    action = Utils::randInRange(MAX_ACTIONS - 1);

  this_action.intArray[0] = action + 1;

/* In a real action you might want to store the last observation and last action*/
  replaceRLStruct(&this_action, &last_action);
  replaceRLStruct(this_observation, last_observation);

  return &this_action;
}

// Step.
const action_t *agent_step(double reward, const observation_t *this_observation) {

  // Get current state
  int cX = this_observation->intArray[0];
  int cY = this_observation->intArray[1];

  // Get the reward obtained.
  double cR = reward;

  // Get last state.
  int lX = last_observation->intArray[0];
  int lY = last_observation->intArray[1];

  // Get last action.
  int lA = last_action.intArray[0] - 1;

  // Update the last state with the Q learning update.( Optimal play update. )
  // q[lX][lY][lA] = cR + GAMMA * ( q[cX][cY][ maxAction(cX, cY) ] );

  double target = cR + GAMMA * (q[MAX_Y * MAX_ACTIONS * cX + MAX_ACTIONS * cY + maxAction(cX, cY)]);

  q[lX * MAX_Y * MAX_ACTIONS + lY * MAX_ACTIONS + lA] =
      alpha * target + (1 - alpha) * q[lX * MAX_Y * MAX_ACTIONS + lY * MAX_ACTIONS + lA];

  int action = -1;

  // Calculate an action.
  if (Utils::doubleRand() > EPSILON)
    action = maxAction(cX, cY);
  else
    action = Utils::randInRange(MAX_ACTIONS - 1);

  this_action.intArray[0] = action + 1;

  //printf("%d\n", last_observation->numInts);
  /* In a real action you might want to store the last observation and last action*/
  replaceRLStruct(&this_action, &last_action);
  replaceRLStruct(this_observation, last_observation);

  return &this_action;
}

void agent_end(double reward) {
  //printf("END of AGENT!");

  // Get last state.
  int lX = last_observation->intArray[0];
  int lY = last_observation->intArray[1];

  // Get last action.
  int lA = last_action.intArray[0] - 1;

  q[lX * MAX_Y * MAX_ACTIONS + lY * MAX_ACTIONS + lA] =
      (1 - alpha) * q[lX * MAX_Y * MAX_ACTIONS + lY * MAX_ACTIONS + lA] + alpha * reward;

  clearRLStruct(&last_action);
  clearRLStruct(last_observation);
}

void agent_cleanup() {
  //printf("CLEANING UP AGENT!");
  for (int j = 0; j < MAX_Y; j++) {
    printf("\n");
    for (int i = 0; i < MAX_X; i++) {

      int max = -1;
      double val = -1000;
      for (int k = 0; k < MAX_ACTIONS; k++) {

        double z = q[MAX_Y * MAX_ACTIONS * i + MAX_ACTIONS * j + k];
        if (z > val) {
          val = z;
          max = k;
        }

        //printf("%d: %lf\n", k, );
      }
      switch (max) {
        case 0:printf(">\t");
          break;
        case 1:printf("<\t");
          break;
        case 2:printf("V\t");
          break;
        case 3:printf("^\t");
          break;
      }
    }
  }
  clearRLStruct(&this_action);
  clearRLStruct(&last_action);
  freeRLStructPointer(last_observation);
}

const char *agent_message(const char *inMessage) {
  if (strcmp(inMessage, "what is your name?") == 0)
    return "THE TERMINATOR!";

  if (inMessage[0] == 'a' && inMessage[1] == ' ') {
    alpha = atof(inMessage + 2);
    return "Updated Training Rate";
  }

  return "I don't know how to respond to your message";
}
