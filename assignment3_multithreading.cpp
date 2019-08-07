
#include<iostream>
#include<conio.h>
#include<vector>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;


    static int AB_packets;      // counter for AB packets
    static int BA_packets;      // counter for BA packets
    static int delayed_packets; // counter for waiting packets


// Channel info

    static int MaxPackets; // int for maximum number of packets in channel
    static int CurrentPackets; // current packets in channel
    static string ChannelInfo; // tells the status of the channel 0 = closed; 1 = AB; 2= BA;

class packetInput
{
public:
packetInput() =default;

     int arrival_time; // first number of input; time the packet arrived
    int cross_time; // the 3 number of input; time spent in tunnel
    string location; // the 2nd column of input; destination of packet
    string protocol_Var;
       bool AB_transmitted=bool() ;
       bool BA_transmitted=bool() ;
       bool AB_waited_var=bool();
       bool BA_waited_var=bool();
   
   
	packetInput(int arrival, int cross, string location, string protocol):
arrival_time(arrival), cross_time(cross), location(location),  protocol_Var(protocol){}

};

 
class wait_Vector
{
public:
 static vector<packetInput>my_wait_vector_AB; 
  static vector<packetInput>my_wait_vector_BA; 
   wait_Vector();
   
 
};
std::vector<packetInput> wait_Vector::my_wait_vector_AB;
std::vector<packetInput> wait_Vector::my_wait_vector_BA;
wait_Vector::wait_Vector()
{
}


/*
void wait_Vector:: initialize_Stat_Array()


{
    
    string array[4] ={"FTP","HTTP","SSH","SMTP"};
       
       packetInput obj;
       for( int i =0; i< 4; i++)
       {
           obj.protocol_Var=array[i];
           
         wait_Vector::my_wait_vector_AB.push_back(obj);
          wait_Vector::my_wait_vector_BA.push_back(obj);
            
           
       }
    
    
    
    
}


*/
struct packetInfo
{
    int packetID; //identify packet #
    int pass_channel; // variable for packet passing channel
    string protocol;
};


    static pthread_mutex_t channel_lock; // locks the channel and changes traffic
    pthread_cond_t change_traffic = PTHREAD_COND_INITIALIZER; // changes the traffic of the channel

// Declarations 

    void *channel(void *arg) ;// thread for the channel pass by reference
    void *B(void *arg); // thread for first channel
    void *A(void *arg); // thread for the A bound traffic

struct StatArrays
  {
       static int AB_wait_packet;
       
       static int BA_wait_packet;
       
      vector<packetInput> PI_arr_trans_AB = {};
      vector<packetInput>  PI_arr_trans_BA = {};
      vector<packetInput> AB_waited = {};
      vector<packetInput> BA_waited = {};
      
      void initialize_StatVects();

  };
  
     
   void StatArrays :: initialize_StatVects(){
        string array[4] ={"FTP","HTTP","SSH","SMTP"};
       
       packetInput obj;
       for( int i =0; i< 4; i++)
       {
           obj.protocol_Var=array[i];
           
            PI_arr_trans_AB.push_back(obj);
            PI_arr_trans_BA.push_back(obj);
            AB_waited.push_back(obj);
            BA_waited.push_back(obj);
            
           
       }
   }

int main(int argc, const char *argv[])
    {
        
        string array[4] ={"FTP","HTTP","SSH","SMTP"};
       
       packetInput obj;
       for( int i =0; i< 4; i++)
       {
           obj.protocol_Var=array[i];
           
         wait_Vector::my_wait_vector_AB.push_back(obj);
          wait_Vector::my_wait_vector_BA.push_back(obj);
            
           
       }
    
    
    
    
        
        
        
        
        
        StatArrays StatObj;
 
 
StatObj.initialize_StatVects();
 
 
        AB_packets = 0;
        BA_packets = 0;
        CurrentPackets = 0;
        delayed_packets = 0;
        pthread_mutex_init(&channel_lock, NULL);
        vector<pthread_t> ids;

    //Obtaining Data
        
        cout << "Enter the file name (include .txt) : ";
        string infile;
        cin >> infile;

    // Opening the file

        vector<packetInput> list;
        string lines; // input
        int firstline = 0;
        ifstream Inputfile(infile.c_str());
        if (Inputfile.is_open())
        {
		string l;	
		getline(Inputfile, l);
		stringstream line(l);
		line >> MaxPackets;
    // Reading through all the inputs
    while (getline(Inputfile, lines))
            {
            if (lines != "")
                {
                    stringstream ss(lines);
                    string protocol; 
                    string destination; 

                    int counter = 0;
                    int arrival = 0; 
                    int passtime = 0;
		    ss >> protocol >> arrival >> destination >> passtime;
			auto p = packetInput(arrival, passtime, destination, protocol);
			list.push_back(p);
                }
            }
        }
	for(auto p: list){
		cout<<p.protocol_Var<<" "<<p.arrival_time<<" "<<p.location<<" "<<p.cross_time<<endl;	
	}
        Inputfile.close();

        int seqNo = 1;
        pthread_t tid;
        pthread_create(&tid, NULL, channel, (void *)&seqNo);

        //cycles through input vector
        
        packetInput packet_Var;
        vector<packetInput>::iterator i;
        for (i = list.begin(); i != list.end(); ++i)
        {

            sleep((*i).arrival_time);
            packetInfo *temp = new packetInfo;
            temp->packetID = seqNo;
            packet_Var.protocol_Var= temp->pass_channel = (*i).cross_time;
            temp->protocol = (*i).protocol_Var;
            pthread_t cid;

        // AB Thread
            if ((*i).location == "AB")
            {   
                  for( int j =0; j <StatObj.PI_arr_trans_AB.size(); j++ ){
             if(StatObj.PI_arr_trans_AB.at(j).protocol_Var==(*i).protocol_Var)
            {
                
                StatObj.PI_arr_trans_AB.at(j).AB_transmitted =true; 
                
                 
               }
                
               
            }
      
 
                       
                  
                pthread_create(&cid, NULL, B, (void *)temp);
                ids.push_back(cid);
            }

        // BA Thread
            else
            {
                   for( int j =0; j <StatObj.PI_arr_trans_BA.size(); j++ ){
             if(StatObj.PI_arr_trans_BA.at(j).protocol_Var==(*i).protocol_Var)
            {
                
               StatObj.PI_arr_trans_BA.at(j).BA_transmitted =true; 
               
                 
            }
           
                  }
                pthread_create(&cid, NULL, A, (void *)temp);
                ids.push_back(cid);
            }

            seqNo++;
        }

       
        vector<pthread_t>::iterator ia;
        for (ia = ids.begin(); ia != ids.end(); ++ia)
        {
            pthread_join(*ia, NULL);
        }


    // Printing Summary

        //cout << AB_packets << " car(s) going to B arrived at the tunnel.\n";
        //cout << BA_packets << " car(s) going to A arrived at the tunnel.\n";
        //cout << delayed_packets << " car(s) were delayed.\n";

    //Killing the channel thread

        pthread_kill(tid, 0);
        
         cout << endl;
         cout << "Packets transmitted:" << endl;
         cout << "\t From A to B:" << endl;
         cout << "\t \t ";
         int counter = 0;
        
        for( int i =0; i<  StatObj.PI_arr_trans_AB.size(); i++)
       {
           if(counter <3)
           {
                cout << " " << StatObj.PI_arr_trans_AB.at(i).AB_transmitted << " " << StatObj.PI_arr_trans_AB.at(i).protocol_Var << ",";
                counter ++;
           }
           else{
                cout << " " << StatObj.PI_arr_trans_AB.at(i).AB_transmitted << " " << StatObj.PI_arr_trans_AB.at(i).protocol_Var << ".";
                counter = 0;
           }
        
       }
       cout << endl;
       cout << "\t From B to A:" << endl;
       cout << "\t \t ";

         
         for( int i =0; i<  StatObj.PI_arr_trans_BA.size(); i++)
       {
            if(counter <3)
           {
                cout << " " << StatObj.PI_arr_trans_BA.at(i).BA_transmitted << " " << StatObj.PI_arr_trans_BA.at(i).protocol_Var << ",";
                counter ++;
           }
           else{
                cout << " " << StatObj.PI_arr_trans_BA.at(i).BA_transmitted << " " << StatObj.PI_arr_trans_BA.at(i).protocol_Var << ".";
                counter = 0;
           }
            
       }
       
         cout << endl;
         cout << "Packets that waited:" << endl;
         cout << "\t From A to B:" << endl;
         cout << "\t \t ";
         
         for( int i =0; i<   wait_Vector::my_wait_vector_AB.size(); i++)
       {
            if(counter <3)
           {
                cout << " " << wait_Vector::my_wait_vector_AB.at(i).AB_waited_var << " " << wait_Vector::my_wait_vector_AB.at(i).protocol_Var  << ",";
                counter ++;
           }
           else{
                cout << " " << wait_Vector::my_wait_vector_AB.at(i).AB_waited_var << " " <<wait_Vector::my_wait_vector_AB.at(i).protocol_Var<< ".";
                counter = 0;
           }
           
       }
        cout << endl;
        cout << "\t From B to A:" << endl;
        cout << "\t \t ";

         
         for( int i =0; i<   wait_Vector::my_wait_vector_BA.size(); i++)
       {
            if(counter <3)
           {
                cout << " " << wait_Vector::my_wait_vector_BA.at(i).BA_waited_var << " " << wait_Vector::my_wait_vector_BA.at(i).protocol_Var  << ",";
                counter ++;
           }
           else{
                cout << " " << wait_Vector::my_wait_vector_BA.at(i).BA_waited_var << " " <<wait_Vector::my_wait_vector_BA.at(i).protocol_Var<< ".";
                counter = 0;
           }
           
       }
       cout << endl;
       
    return 0;

    } // End of Main

void *channel(void *arg)
    {
        while (true)
        {
            cout << "The current direction is AB \n";
            ChannelInfo = "AB";
            pthread_cond_broadcast(&change_traffic);
            sleep(5); // sleep 5 seconds

            cout << "The current direction is NONE \n";
            ChannelInfo = "Closed";
            pthread_cond_broadcast(&change_traffic);
            sleep(5); // sleep 5 seconds

            cout << "The current direction is BA. \n";
            ChannelInfo = "BA";
            pthread_cond_broadcast(&change_traffic);
            sleep(5);

            cout << "The current direction is NONE \n";
            ChannelInfo = "Closed";
            pthread_cond_broadcast(&change_traffic);
            sleep(5);
        }
} // End of channel

void *B(void *arg)
{
    int packetID;
    int pass_channel;
    string protocol;

    AB_packets++; // increment the counter for cars going to WB
    packetInfo *temp; // pointer for the struct
    temp = (packetInfo*) arg;
    packetID =temp->packetID;
    pass_channel=temp->pass_channel;

// Booleans to test
    bool wait = true; // for delayed cars
    bool finished = false; // for cars that finishes

// Channel process
    cout << "Packet # " << temp->packetID << "("<< temp->protocol<< ")"<< " going to B arrives at the system.\n";
    pthread_mutex_lock(&channel_lock); // locks the traffic

    while(!finished) // while not finished
    {
        if (ChannelInfo == "AB" && CurrentPackets < MaxPackets)
        {
            cout << "Packet # " << temp->packetID << "("<< temp->protocol<< ")"<<" going to B is using the channel.\n";
            CurrentPackets++;
            pthread_mutex_unlock(&channel_lock);
            sleep(temp->pass_channel);
            finished = true;
        }
        else
        {
            if (wait && CurrentPackets == MaxPackets && ChannelInfo == "AB")
            {
                
                for( int i =0; i< wait_Vector::my_wait_vector_AB.size(); i++ ){
                    
                  if(wait_Vector::my_wait_vector_AB.at(i).protocol_Var ==temp->protocol)
                  {
                      
                       wait_Vector::my_wait_vector_AB.at(i).AB_waited_var=true;
                  }
                    
                }
                
                delayed_packets++;
                wait = false;
            }
            pthread_cond_wait(&change_traffic, &channel_lock);
        }
    }
// Managing the threads

    pthread_mutex_lock(&channel_lock);
    CurrentPackets--;
    cout << "Packet # " << temp->packetID << "("<< temp->protocol<< ")"<< " going to B exits the channel.\n";
    pthread_cond_broadcast(&change_traffic);
    pthread_mutex_unlock(&channel_lock);
    pthread_exit((void *)0);
} // End of B-Bound thread   

  void *A(void *arg)
{
    int packetID;
    int pass_channel;
    string protocol;

    BA_packets++;         // increment the counter for cars going to WB
    packetInfo *temp; // pointer for the struct
    temp = (packetInfo *)arg;
    packetID = temp->packetID;
    pass_channel = temp->pass_channel;

// Booleans to test
    bool wait = true;      // for delayed cars
    bool finished = false; // for cars that finishes

// Channel process
    cout << "Packet # " << temp->packetID << "("<< temp->protocol<< ")"<< " going to A arrives at the system.\n";
    pthread_mutex_lock(&channel_lock); // locks the traffic

    while (!finished) // while not finished
    {
        if (ChannelInfo == "BA" && CurrentPackets < MaxPackets)
        {
            cout << "Packet # " << temp->packetID << "("<< temp->protocol<< ")"<< " going to A is using the channel.\n";
            CurrentPackets++;
            pthread_mutex_unlock(&channel_lock);
            sleep(temp->pass_channel);
            finished = true;
        }
        else
        {
            if (wait && CurrentPackets == MaxPackets && ChannelInfo == "BA")
            {
                 
                for( int i =0; i< wait_Vector::my_wait_vector_BA.size(); i++ ){
                    
                  if(wait_Vector::my_wait_vector_BA.at(i).protocol_Var ==temp->protocol)
                  {
                      
                       wait_Vector::my_wait_vector_BA.at(i).BA_waited_var=true;
                  }
                    
                }
                
                
                delayed_packets++;
                wait = false;
            }
            pthread_cond_wait(&change_traffic, &channel_lock);
        }
    }

// Managing the threads
    pthread_mutex_lock(&channel_lock);
    CurrentPackets--;
    cout << "Packet # " << temp->packetID << "("<< temp->protocol<< ")"<< " going to A exits the channel.\n";
    pthread_cond_broadcast(&change_traffic);
    pthread_mutex_unlock(&channel_lock);
    pthread_exit((void *)0);

} // End of A-Bound thread
