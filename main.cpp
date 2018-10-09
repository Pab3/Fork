#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>


using namespace std;
int main()
{
    //embed another pair into your vector to hold a bool value at the end
    vector < pair<int, pair<int, pair<int,bool> > > > parentVect;
    vector < pair<int, pair<int, pair<int,bool> > > > childVect;
    vector < pair<int, pair<int, pair<int,bool> > > > mergedVect;
    auto comparer = [](const pair<int, pair<int, pair<int,bool> > >& a, const pair<int, pair<int, pair<int,bool> > >& b){return a.first < b.first; };
    int quantum, pid, exectime, priority;
    bool procFinished = true;
    pid_t forkPid;
    cin >> quantum;
    pid = 0;
    
    while(cin >> exectime >> priority)
    {
        if(exectime > quantum){
            int childExecTime = exectime - quantum;
            childVect.push_back(make_pair(priority, make_pair(childExecTime, make_pair(pid, procFinished))));
            parentVect.push_back(make_pair(priority, make_pair(quantum, make_pair(pid, !procFinished))));
            pid++;
        }
        else{
        parentVect.push_back(make_pair(priority, make_pair(exectime, make_pair(pid, procFinished))));
        pid++;
        }
    }
    sort(parentVect.begin(), parentVect.end());
    sort(childVect.begin(), childVect.end());
    merge(parentVect.begin(), parentVect.end(), childVect.begin(), childVect.end(), std::back_inserter(mergedVect), comparer);
    
    cout << "Scheduling queue: " << "\n" << "\t";
        for(int i = 0; i<mergedVect.size(); i++)
        {
            if(i != mergedVect.size()-1){
            cout << "(" <<mergedVect[i].second.second.first << "," << mergedVect[i].second.first << "," << mergedVect[i].first << "),";
            }
            else
            {
                cout << "(" <<mergedVect[i].second.second.first << "," << mergedVect[i].second.first << "," << mergedVect[i].first << ")";
            }
        }
    
    cout << "\n\n";
    for(int i=0; i<mergedVect.size(); i++){
        if((forkPid = fork())==0){
                if(mergedVect[i].second.first == quantum && mergedVect[i].second.second.second == !procFinished ){
                    cout << "Process " << mergedVect[i].second.second.first << ": exec time = " << mergedVect[i].second.first << ", priority = " << mergedVect[i].first << "\n";
                    sleep(mergedVect[i].second.first);
                    _exit(0);
                }
                else if (mergedVect[i].second.first == quantum && mergedVect[i].second.second.second == true){
                    cout << "Process " << mergedVect[i].second.second.first << ": exec time = " << mergedVect[i].second.first << ", priority = " << mergedVect[i].first << "\n";
                    sleep(mergedVect[i].second.first);
                    cout << "Process " << mergedVect[i].second.second.first << " ends." << "\n";
                    _exit(0);
                }
                else if (mergedVect[i].second.first < quantum){
                    cout << "Process " << mergedVect[i].second.second.first << ": exec time = " << mergedVect[i].second.first << ", priority = " << mergedVect[i].first << "\n";
                    sleep(mergedVect[i].second.first);
                    cout << "Process " << mergedVect[i].second.second.first << " ends." << "\n";
                    _exit(0);
                }
        }
        wait(NULL);
    }
    return 0;
}

