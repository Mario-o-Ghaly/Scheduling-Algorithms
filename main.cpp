#include <bits/stdc++.h>
#include <fstream>
using namespace std;
#define QUANTUM_0 8
#define QUANTUM_1 16

struct proc {
    int PID, arrivalTime, CPUTime, savedBurstTime, startTime, endTime;
};

bool sort_arrival(const proc& p1, const proc& p2){
    return p1.arrivalTime < p2.arrivalTime;
}

void FCFS(vector<proc> procs, float &avg_turnaroundTime, float &avg_responseTime, float &avg_waitingTime){
    sort(procs.begin(), procs.end(), sort_arrival);

    float total_turnaroundTime = 0;
    float total_responseTime = 0;
    float total_waitingTime = 0;
    int time = 0;
    for(int i = 0; i < procs.size(); ++i){
        if(time < procs[i].arrivalTime) time = procs[i].arrivalTime;
        cout << "This is PID " << procs[i].PID << " at time " << time << "\n";
        procs[i].startTime = time;
        time += procs[i].CPUTime;
        procs[i].endTime = time;
        total_turnaroundTime += (procs[i].endTime - procs[i].arrivalTime);
        total_responseTime += (procs[i].startTime - procs[i].arrivalTime);
    }
    //bcs wait = turnaround - burst
    //    wait = end - arrival - burst
    //in fcfs, end - burst is start
    total_waitingTime = total_responseTime ;
    avg_turnaroundTime = total_turnaroundTime / procs.size();
    avg_responseTime = total_responseTime / procs.size();
    avg_waitingTime = total_waitingTime / procs.size();
    cout<<"\nAverage Turn around time = "<<avg_turnaroundTime;
    cout<<"\nAverage response time = "<<avg_responseTime;
    cout<<"\nAverage waiting time = "<<avg_waitingTime;
}


void SJF(vector<proc> procs, float &avg_turnaroundTime, float &avg_responseTime, float &avg_waitingTime){

    float total_turnaroundTime = 0;
    float total_responseTime = 0;
    float total_waitingTime = 0;

    sort(procs.begin(), procs.end(), sort_arrival);
    map<int, proc> processes; //create a map of proccesses by its PID
    map<int, vector<int>> CPUTime, arrivalTime, zeft;  //retrieve pid by cputime or arrivaltime
    for(int i = 0; i < procs.size(); ++i){
        procs[i].startTime = -1;  //initialization to distinguish whether the process has started
        processes[procs[i].PID] = procs[i]; 
        arrivalTime[procs[i].arrivalTime].push_back(procs[i].PID); 
        zeft[procs[i].arrivalTime].push_back(procs[i].PID); 
    }

    auto T_arrive = arrivalTime.begin();
    int time = T_arrive->first;  
    //loop on the vector of PIDs that has arrived to put it in the CPUTime map
    for(auto pid : T_arrive->second){  
        CPUTime[processes[pid].CPUTime].push_back(pid);
    }
    
    int PID = CPUTime.begin()->second.back();  
    auto curproc = &processes[PID];
    bool finished = false;

    do {
        // cout<<"size = "<<CPUTime.size()<<"\n";
        //set the current PID to the shortest job that has come at the end of the vec
        PID = CPUTime.begin()->second.back();
        curproc = &processes[PID];
        cout<<"This is PID: "<<PID<<" at time "<<time<<" with CPUTime "<<curproc->CPUTime<<"\n";
        if(curproc->startTime == -1){ 
            curproc->startTime = time;
            total_responseTime += (curproc->startTime - curproc->arrivalTime);
        }

        // traverse to the next arrival time
        ++T_arrive;
        if(T_arrive != arrivalTime.end()){  //check if this was not the last time of arrival
            int given_chance = (T_arrive->first - time);
            if(given_chance <= curproc->CPUTime){ //this means possible preemption
                cout<<"preempted:\n";
                //update time
                time += given_chance;
                //remove process from the CPUTime to be updated
                CPUTime[curproc->CPUTime].pop_back();
                //if this is the last process with this CPUTime, delete the key
                if(CPUTime[curproc->CPUTime].empty()) CPUTime.erase((curproc->CPUTime)); 
                //update curproc cputime
                curproc->CPUTime -= given_chance;
                //re-add curproc to cputime map if it has not finished
                if(curproc->CPUTime != 0){
                    CPUTime[curproc->CPUTime].push_back(curproc->PID);
                }
                else { //proc has ended
                    curproc->endTime = time; 
                    total_turnaroundTime += (curproc->endTime - curproc->arrivalTime);
                    total_waitingTime += ( (curproc->endTime - curproc->arrivalTime) - curproc->savedBurstTime );
                }
                //loop on the vector of PIDs that has arrived to put it in the CPUTime map
                for(auto pid : T_arrive->second){ 
                    CPUTime[processes[pid].CPUTime].push_back(pid);
                }
            }
            else finished = true;
        }

        if(T_arrive == arrivalTime.end() || finished){  //no premption, just the job finished
            finished = false;
            //update time
            time += curproc->CPUTime;
            --T_arrive; //as it has not happened yet
            
            //remove process from the CPUTime to be updated
            CPUTime[curproc->CPUTime].pop_back();
            //if this is the last process with this CPUTime, delete the key
            if(CPUTime[curproc->CPUTime].empty()) CPUTime.erase((curproc->CPUTime)); 

            curproc->CPUTime = 0;
            curproc->endTime = time;
            total_turnaroundTime += (curproc->endTime - curproc->arrivalTime);
            total_waitingTime += ( (curproc->endTime - curproc->arrivalTime) - curproc->savedBurstTime );
        }

        //if there is a big gap between processes
        if(CPUTime.empty()){
            // cout<<"Edge case not achieved\n";
            //traverse to the next arrival time
            T_arrive++;
            //check if the next time exists
            if(T_arrive != arrivalTime.end()){
                // cout<<"Edge case achieved\n";
                time = T_arrive->first;
                for(auto pid : T_arrive->second){ 
                    CPUTime[processes[pid].CPUTime].push_back(pid);
                }
            }
        }   
    
    } while(!CPUTime.empty());

    avg_turnaroundTime = total_turnaroundTime / procs.size();
    avg_responseTime = total_responseTime / procs.size();
    avg_waitingTime = total_waitingTime / procs.size();
    cout<<"\nAverage Turn around time = "<<avg_turnaroundTime;
    cout<<"\nAverage response time = "<<avg_responseTime;
    cout<<"\nAverage waiting time = "<<avg_waitingTime;
}


void RR (vector<proc> procs, int quantum, float &avg_turnaroundTime, float &avg_responseTime, float &avg_waitingTime){

    float total_turnaroundTime = 0;
    float total_responseTime = 0;
    float total_waitingTime = 0;

    sort(procs.begin(), procs.end(), sort_arrival);
    queue <proc> processes;
    for(int i = 0; i < procs.size(); ++i){
        procs[i].startTime = -1;
    }

    int time = 0, id = 0;

    proc curproc;
    while(id < procs.size()){
        // in case there is a big gap between processes and for initialization
        processes.push(procs[id++]);
        time = processes.front().arrivalTime;
        while(!processes.empty()){
            // give the CPU to the front scheduled process
            curproc = processes.front();
            processes.pop();
            cout<<"This is PID "<<curproc.PID<<" at time "<<time<<"\n";

            //update startTime
            if(curproc.startTime == -1){
                curproc.startTime = time;
                total_responseTime += (curproc.startTime - curproc.arrivalTime);
            }
            
            curproc.CPUTime -= quantum; 
            time += quantum;

            if(curproc.CPUTime <= 0){  //finished
                time += curproc.CPUTime;
                while(id < procs.size() && procs[id].arrivalTime <= time){  //push processes arrived to the queue
                    processes.push(procs[id++]);
                }
                curproc.CPUTime = 0; //for meaning purposes nothing special here
                curproc.endTime = time;
                // cout << "PID " << curproc.PID << " ended at " << time << "\n";
                total_turnaroundTime += (curproc.endTime - curproc.arrivalTime);
                total_waitingTime += ( (curproc.endTime - curproc.arrivalTime) - curproc.savedBurstTime);
            }
            else {
                while(id < procs.size() && procs[id].arrivalTime <= time){  //push processes arrived to the queue
                    processes.push(procs[id++]);
                }
                processes.push(curproc);
            }
        }
    }

    avg_turnaroundTime = total_turnaroundTime / procs.size();
    avg_responseTime = total_responseTime / procs.size();
    avg_waitingTime = total_waitingTime / procs.size();
    cout<<"\nAverage Turn around time = "<<avg_turnaroundTime;
    cout<<"\nAverage response time = "<<avg_responseTime;
    cout<<"\nAverage waiting time = "<<avg_waitingTime;
}


void MLFQ(vector<proc> procs, int quantum0, int quantum1,  float &avg_turnaroundTime, float &avg_responseTime, float &avg_waitingTime){
    float total_turnaroundTime = 0;
    float total_responseTime = 0;
    float total_waitingTime = 0;

    sort(procs.begin(), procs.end(), sort_arrival);
    queue <proc> q0, q1;
    for(int i = 0; i < procs.size(); ++i){
        procs[i].startTime = -1;
    }

    int time = 0, id = 0;

    proc curproc;
    while(id < procs.size()){
        // in case there is a big gap between processes and for initialization
        q0.push(procs[id++]);
        time = q0.front().arrivalTime;
        while(!q0.empty() || !q1.empty()){
            while(!q0.empty()){
                // give the CPU to the front scheduled process
                curproc = q0.front();
                q0.pop();
                cout<<"This is PID "<<curproc.PID<<" at queue 0 at time "<<time<<"\n";

                //update startTime
                if(curproc.startTime == -1){
                    curproc.startTime = time;
                    total_responseTime += (curproc.startTime - curproc.arrivalTime);
                }
                
                curproc.CPUTime -= quantum0; 
                time += quantum0;

                if(curproc.CPUTime <= 0){  //finished
                    time += curproc.CPUTime;
                    while(id < procs.size() && procs[id].arrivalTime <= time){  //push processes arrived to queue0
                        q0.push(procs[id++]);
                    }
                    curproc.CPUTime = 0;  //for meaning purposes nothing special here
                    curproc.endTime = time;
                    total_turnaroundTime += (curproc.endTime - curproc.arrivalTime);
                    total_waitingTime += ( (curproc.endTime - curproc.arrivalTime) - curproc.savedBurstTime);
                }
                else {
                    while(id < procs.size() && procs[id].arrivalTime <= time){  //push processes arrived to queue0
                        q0.push(procs[id++]);
                    }
                    q1.push(curproc);  //send it to queue1
                }
            }

            while(!q1.empty()){
                curproc = q1.front();
                q1.pop();
                cout<<"This is PID "<<curproc.PID<<" at queue 1 at time "<<time<<"\n";

                curproc.CPUTime -= quantum1; 
                time += quantum1;

                if(curproc.CPUTime <= 0){  //finished
                    time += curproc.CPUTime;
                    while(id < procs.size() && procs[id].arrivalTime <= time){  //push processes arrived to queue0
                        q0.push(procs[id++]);
                    }
                    curproc.CPUTime = 0;  //for meaning purposes nothing special here
                    curproc.endTime = time;

                    total_turnaroundTime += (curproc.endTime - curproc.arrivalTime);
                    total_waitingTime += ( (curproc.endTime - curproc.arrivalTime) - curproc.savedBurstTime);
                }
                else {
                    while(id < procs.size() && procs[id].arrivalTime <= time){  //push processes arrived to queue0
                        q0.push(procs[id++]);
                    }
                    q1.push(curproc);
                }
                break;
            }
        }
    }

    avg_turnaroundTime = total_turnaroundTime / procs.size();
    avg_responseTime = total_responseTime / procs.size();
    avg_waitingTime = total_waitingTime / procs.size();
    cout<<"\nAverage Turn around time = "<<avg_turnaroundTime;
    cout<<"\nAverage response time = "<<avg_responseTime;
    cout<<"\nAverage waiting time = "<<avg_waitingTime;
}


void set_procs(vector<proc>& procs){
    for(int i = 0; i < procs.size(); ++i){
        procs[i].PID = i;
        procs[i].arrivalTime = rand() % 10;
        procs[i].CPUTime = rand() % 10 + 1; //not to be zero
        procs[i].savedBurstTime = procs[i].CPUTime;
    }
    // procs[0].PID = 0;
    // procs[0].arrivalTime = 0;
    // procs[0].CPUTime = 53;
    // procs[0].savedBurstTime = procs[0].CPUTime;
    // procs[1].PID = 1;
    // procs[1].arrivalTime = 0;
    // procs[1].CPUTime = 8;
    // procs[1].savedBurstTime = procs[1].CPUTime;
    // procs[2].PID = 2;
    // procs[2].arrivalTime = 0;
    // procs[2].CPUTime = 68;
    // procs[2].savedBurstTime = procs[2].CPUTime;
    // procs[3].PID = 3;
    // procs[3].arrivalTime = 0;
    // procs[3].CPUTime = 24;
    // procs[3].savedBurstTime = procs[3].CPUTime;
    }


void to_csv(string filename, const vector<float>& one, const vector<float>& two, const vector<float>& three, const vector<float>& four) {
    ofstream csv;
    csv.open(filename);
    csv << "n, FCFS, RR, SJF, MLFQ\n";

    // Write data row by row
    int j = 100;
    for (size_t i = 0; i < one.size(); ++i) {
        csv << j<<","<<one[i] <<","<< two[i]<<","<< three[i] << "," << four[i] << "\n";
        j+=100;
    }
    csv.close();
}


void test(){
    float avg_turnaroundTime = 0, avg_responseTime = 0, avg_waitingTime = 0;
    vector<float> FCFS_turnaroundTimes, RR_turnaroundTimes, SJF_turnaroundTimes, MLFQ_turnaroundTimes;
    vector<float> FCFS_responseTimes, RR_responseTimes, SJF_responseTimes, MLFQ_responseTimes;
    vector<float> FCFS_waitingTimes, RR_waitingTimes, SJF_waitingTimes, MLFQ_waitingTimes;
    vector<float> read1, read2, read3;
    for (int i = 100; i < 5000; i+=100)
    {
        vector<proc> procs(i);
        set_procs(procs);
        //
        avg_turnaroundTime = 0, avg_responseTime = 0, avg_waitingTime = 0;
        for(int j = 0; j < 11; ++j){
            FCFS(procs, avg_turnaroundTime, avg_responseTime, avg_waitingTime);
            read1.push_back(avg_turnaroundTime);
            read2.push_back(avg_responseTime);
            read3.push_back(avg_waitingTime);
        }
        sort(read1.begin(), read1.end());
        sort(read2.begin(), read2.end());
        sort(read3.begin(), read3.end());

        avg_turnaroundTime = read1[5];
        avg_responseTime = read2[5];
        avg_waitingTime = read3[5];
        read1.clear(); read2.clear(); read3.clear();

        FCFS_turnaroundTimes.push_back(avg_turnaroundTime);
        FCFS_responseTimes.push_back(avg_responseTime);
        FCFS_waitingTimes.push_back(avg_waitingTime);

        
        //
        avg_turnaroundTime = 0, avg_responseTime = 0, avg_waitingTime = 0;
        for(int j = 0; j < 11; ++j){
            RR(procs, QUANTUM_0, avg_turnaroundTime, avg_responseTime, avg_waitingTime);
            read1.push_back(avg_turnaroundTime);
            read2.push_back(avg_responseTime);
            read3.push_back(avg_waitingTime);
        }
        sort(read1.begin(), read1.end());
        sort(read2.begin(), read2.end());
        sort(read3.begin(), read3.end());

        avg_turnaroundTime = read1[5];
        avg_responseTime = read2[5];
        avg_waitingTime = read3[5];
        read1.clear(); read2.clear(); read3.clear();

        RR_turnaroundTimes.push_back(avg_turnaroundTime);
        RR_responseTimes.push_back(avg_responseTime);
        RR_waitingTimes.push_back(avg_waitingTime);


        //
        avg_turnaroundTime = 0, avg_responseTime = 0, avg_waitingTime = 0;
        for(int j = 0; j < 11; ++j){
            SJF(procs, avg_turnaroundTime, avg_responseTime, avg_waitingTime);
            read1.push_back(avg_turnaroundTime);
            read2.push_back(avg_responseTime);
            read3.push_back(avg_waitingTime);
        }
        sort(read1.begin(), read1.end());
        sort(read2.begin(), read2.end());
        sort(read3.begin(), read3.end());

        avg_turnaroundTime = read1[5];
        avg_responseTime = read2[5];
        avg_waitingTime = read3[5];
        read1.clear(); read2.clear(); read3.clear();

        SJF_turnaroundTimes.push_back(avg_turnaroundTime);
        SJF_responseTimes.push_back(avg_responseTime);
        SJF_waitingTimes.push_back(avg_waitingTime);

        
        //
        avg_turnaroundTime = 0, avg_responseTime = 0, avg_waitingTime = 0;
        for(int j = 0; j < 11; ++j){
            MLFQ(procs, QUANTUM_0, QUANTUM_1, avg_turnaroundTime, avg_responseTime, avg_waitingTime);
            read1.push_back(avg_turnaroundTime);
            read2.push_back(avg_responseTime);
            read3.push_back(avg_waitingTime);
        }
        sort(read1.begin(), read1.end());
        sort(read2.begin(), read2.end());
        sort(read3.begin(), read3.end());

        avg_turnaroundTime = read1[5];
        avg_responseTime = read2[5];
        avg_waitingTime = read3[5];
        read1.clear(); read2.clear(); read3.clear();

        MLFQ_turnaroundTimes.push_back(avg_turnaroundTime);
        MLFQ_responseTimes.push_back(avg_responseTime);
        MLFQ_waitingTimes.push_back(avg_waitingTime);
        break;
    }
    cout << "\n\n";
    cout << FCFS_responseTimes[0] << " " << RR_responseTimes[0] << " " <<  SJF_responseTimes[0] << " " <<  MLFQ_responseTimes[0] << "\n";

    // to_csv("TurnaroundReport.csv", FCFS_turnaroundTimes, RR_turnaroundTimes, SJF_turnaroundTimes, MLFQ_turnaroundTimes);
    // to_csv("ResponseReport.csv", FCFS_responseTimes, RR_responseTimes, SJF_responseTimes, MLFQ_responseTimes);
    // to_csv("WaitingReport.csv", FCFS_waitingTimes, RR_waitingTimes, SJF_waitingTimes, MLFQ_waitingTimes);
}


void userTest(){
    int n, a, b, c, q0, q1;
    cout << "Please choose which algorithm you want to simulate by entering its number:\n1. FCFS\n2. SRTF\n3. RR\n4. MLFQ\n";
    while(true){
        cin >> c;
        if (c < 1 || c > 4){
            cout << "Invalid choice. Please try again: ";
            continue;
        }
        break;
    }
    if(c == 3){
        cout << "Enter Quantum value: ";
        cin >> q0;
    }
    if(c == 4){
        cout << "Enter Quantum 0 value: ";
        cin >> q0;        
        cout << "Enter Quantum 1 value: ";
        cin >> q1;
    }
    cout << "Please Enter the number of processes: ";
    cin >> n;
    vector<proc> procs(n);
    cout << "Enter the following input comma separated please\n\n";
    for(int i = 0; i < n; ++i){
        procs[i].PID = i + 1;
        cout << "For PID " << i + 1 << ", enter Arrival Time and Burst Time: ";
        cin >> a >> b;
        procs[i].arrivalTime = a;
        procs[i].CPUTime = b;
        procs[i].savedBurstTime = b;
    }

    float avg_turnaroundTime = 0, avg_responseTime = 0, avg_waitingTime = 0;
    switch (c)
    {
    case 1:
        FCFS(procs, avg_turnaroundTime, avg_responseTime, avg_waitingTime);
        break;
    case 2:
        SJF(procs, avg_turnaroundTime, avg_responseTime, avg_waitingTime);
        break;  
    case 3:
        RR(procs, q0, avg_turnaroundTime, avg_responseTime, avg_waitingTime);
        break;

    case 4:
        MLFQ(procs, q0, q1, avg_turnaroundTime, avg_responseTime, avg_waitingTime);
        break;
    }
    // RR(procs, 1, avg_turnaroundTime, avg_responseTime, avg_waitingTime);
    // SJF(procs, avg_turnaroundTime, avg_responseTime, avg_waitingTime);
    // MLFQ(procs, QUANTUM_0, QUANTUM_1, avg_turnaroundTime, avg_responseTime, avg_waitingTime);
}


int main(){
    srand(time(NULL));
    // userTest();
    test();
    return 0;
}