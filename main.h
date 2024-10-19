#pragma once
#include<iostream>
#include <cstdio>
#include<unistd.h>
#include<map>
#include<iostream>
#include<fcntl.h>
#include<sys/stat.h>
#include<string>
#include<sys/sysinfo.h>
#include<errno.h>
#include<dirent.h>
#include <stdexcept>
#include<unistd.h>
#include<sys/utsname.h>
#include<iostream>
#include <cstdio>
#include<unistd.h>
#include<map>
#include<iostream>
#include<string>
#include<sys/sysinfo.h>
#include<errno.h>
#include<vector>
#include<signal.h>
#include<termios.h>
#include<sys/syscall.h>
#include <cstring>
#include <sys/types.h>
#include <pwd.h>
#include<grp.h>
#include<time.h>
#include<chrono>
#include <ctime>
#include<sys/wait.h>
#include<sys/shm.h>
#include<sys/mman.h>
#include<sstream>

using namespace std;
std::string ls_dir(std::string dir_name = string{},std::string flags = string{});
std::string pinfo(int val = 0);
std::string pwd();
std::string cd(std::string dir_name);
std::string echo(std::string val);
bool search(std::string val);
std::string parseexec(std::string comm);
std::string ext(std::string comm_name,std::string flags);
std::pair<std::string,bool> autocomplete(std::string str={});
bool sanitize(std::string val);
void core(string pars);
vector<string> tokenizer(string one) ;
void handle_redirect(string pars);
void child_interrupt(int sig);



