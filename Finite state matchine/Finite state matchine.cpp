// Finite state matchine.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <ctime>
#include <string>
#include <sstream>

using namespace std;

int data[]={100,101,105,110,210,100,102,110,150,100};
int average;

class CSensor
{
   public:
   bool Calibrate(){return true;};
   void ReadData(int data[])
   {
    cout<<"Reading data..."<<endl;
	int i,sum=0;
      for(i=0;i<10;i++)
    {
       sum=sum+data[i];
    }
     average=sum/10;
   };
};


class CMotor
{
    public:
    void ResetMotor()
    {
      cout<<"Reseting motor..."<<endl;
    };
    void MoveMotor(int distance)
    {
      cout<<"Moving motor to "<<distance<<endl;
    };
};

class DoorLatch
{
    public:
    int DoorisOpen(){return 2;};
    int DoorisClosed(){return 1;};
};

// Basic state
class state {
    private:
        int this_state;
    public:
        state(int new_state) : this_state(new_state) { };
        virtual state * change_state(int choice) = 0;
        void wait_a_second();
};

// On stay state, also the start
class state_1 : public state {
    public:
        state_1() : state(1) 
		{ 

		};
        state * change_state(int choice);
};

// Move motor state
class state_2 : public state {
	CMotor cm;
    public:
        state_2() : state(2)
		{ 
		cout<<"Now it is state 2..."<<endl;
		wait_a_second();
		cm.ResetMotor();
		wait_a_second();
		cm.MoveMotor(average);
		};
        state * change_state(int choice);
};

// A cross-platform wait a second or two method
void state::wait_a_second() {
    time_t time_current = time(NULL);
    while ( time(NULL) <= time_current+1 );
};

// On stay state.
state * state_1::change_state(int choice) {
	CSensor cs;
	
	if ( choice == 1 ){
		cout<<"State 1..."<<endl;
		wait_a_second();
		cout<<"Calibrating..."<<endl;
		wait_a_second();
		if(cs.Calibrate()==true)
		{
			cs.ReadData(data);
			wait_a_second();
			cout<<"The average is: "<<average<<endl;
			wait_a_second();
			reinterpret_cast<state_2 *>(this)->state_2::state_2();
		}
		
		}
    else
		 {
	     cout<<"We are on sate 1..."<<endl;
    	 wait_a_second();
         cout<<"The door is open, system stops..."<<endl;
		 wait_a_second();
		 cout<<"Still on state 1..."<<endl;
		 wait_a_second();
	}
    return this;
};
state * state_2::change_state(int choice) {
  
	if(choice == 2){
		reinterpret_cast<state_1 *>(this)->state_1::state_1();
		cout<<"The door is open."<<endl;
	};
	wait_a_second();
	return this;
};

// The finite state machine
class finite_state_machine {
    private:
        state * current_state;
    public:
        finite_state_machine() : current_state(new state_1()) { srand(static_cast<unsigned int>(time(NULL))); };
        void next_state(int choice) { current_state = current_state->change_state(choice); };
        void userinput() {
			    DoorLatch dr;
				int state_shift=0;
				while(true){
				cout<<"Please enter a number, 1 means door is closed, 2 means door is open: ";
				cin>>state_shift;
				   if(state_shift!=1&&state_shift!=2)
				   {
					   cout << "Invalid number, please try again" << endl;
				   }
				   else 
					   break;
					}
                cout << "You entered: " << state_shift << endl << endl;
				if(state_shift==1){
					next_state(dr.DoorisClosed());
				}
				if(state_shift==2){
					next_state(dr.DoorisOpen());
				}
        };
};

int main () {
    finite_state_machine fsm;
    fsm.userinput();
	return 0;
}


