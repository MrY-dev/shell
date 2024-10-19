#include "main.h"

extern int global_parent_pid;
string pinfo(int val){
    string res{};
    int pid = val;
    char* buff =  new char[1000];
    char* exe_buff = new char[120];
    if(val == 0){
        pid = getpid();
    }
    const string  path_name = "/proc/"+to_string(pid) + "/stat";
    const string  exe_path = "/proc/"+to_string(pid) + "/cmdline";
    int fd = open(path_name.c_str(),O_RDONLY);
    int wd = open(exe_path.c_str(),O_RDONLY);
    read(fd,buff,1000);
    read(wd,exe_buff,100);
    vector<string> l;
    char *ptr;
    ptr = strtok(buff," ");
    while(ptr != NULL){
        l.push_back(string(ptr));
        ptr = strtok(NULL," ");
    }
    string state = l[2];
    if(getpgid(pid) == getpgid(global_parent_pid)){
        state = state + "+";
    }
    string exe = string(exe_buff);
    long long mb = 1024*1024;
    long long  vmsize = stol(l[22]);
    res = "pid: "+ l[0]+"\n"  +"ProcessStatus: "+ state + "\n" +"executable path: "+ exe + "\n" +"virutal memory: "+ to_string(vmsize) +"\n" ;
    close(fd);
    close(wd);
    delete[] exe_buff;
    delete[] buff;
    return res;
}
