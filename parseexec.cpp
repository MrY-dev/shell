#include "main.h"

extern int  global_parent_pid;
void handle_redirect(string pars);

void suspend_handler(int sig){
    setpgid(0,0);
}

int child_pid ;

void child_interrupt(int sig){
    cout << "killing child" << child_pid << "\n";
    kill(child_pid ,SIGTERM);
}

int ret_in(vector<string> x,string l);
int getpipes(vector<string> pars_com){
    int count = 0;
    for(auto i : pars_com){
        if(i == "|"){count++;}
    }
    return count;
}
vector<string> tokenizer(string one){
    istringstream tok(one);
    vector<string> res;
    string tmp;
    while(tok >> tmp){
        res.push_back(tmp);
    }
    return res;
}
bool redirect_cond(string x){
    return (x.find(">") != string::npos)||(x.find("<")!=string::npos);
}
void core(string pars){
    vector<string> final = tokenizer(pars);
    if(final[0] == "ls"){
        string flag=string{};
        if(pars.find("-l") != std::string::npos){
            int l = pars.find("-l");
            pars.erase(pars.begin()+l,pars.begin()+l+2);
            flag = "-l";
        }
        if(pars.find("-a") != std::string::npos){
            int l = pars.find("-a");
            pars.erase(pars.begin()+l,pars.begin()+l+2);
            flag = flag + "-a";
        }
        vector<string> final2 = tokenizer(pars);
        string dir_name;
        (final2.size() == 1 )?(dir_name = string{}):(dir_name = final2[1]);
        cout << ls_dir(dir_name,flag);
    }
    else if(final[0] == "history"){
        extern vector<string> curr_comm;
        int k; string tmp = string {};
        if(final.size() == 1){k = curr_comm.size();}
        else {(stoi(final[1]) <= curr_comm.size())?(k=stoi(final[1])):(k=curr_comm.size());}
        if(k > 10){ k = 10;}
        for(int i = curr_comm.size() -k-1 ;i < curr_comm.size() ; ++i){tmp = tmp +"\n"+ curr_comm[i] ;}
        cout << tmp ;
    }
    else if(final[0] == "search"){
        cout << boolalpha << search(final[1]) << "\n";
    }
    else if(final[0] == "echo"){
        int l = pars.find("echo");
        pars.erase(l,4);
        cout << echo(pars) << "\n";
    }
    else if(final[0] == "cd"){
        string tmp;
        if(final.size()  == 1){tmp = getenv("HOME");}
        else if (final[1] == "~"){ tmp = getenv("HOME");}
        else {
            tmp = final[1];
        }
        chdir(tmp.c_str());
    }
    else if(final[0] == "pwd"){
        char * tmp = new  char[100];
        getcwd(tmp,100);
        cout << tmp << "\n";
        delete[] tmp;
    }
    else if(final[0] == "pinfo"){
        if(final.size() > 1){ cout <<pinfo(stoi(final[1]));}
        else{cout << pinfo(global_parent_pid);}
    }
    else{
        const char* argv[final.size()+1];
        argv[final.size()] = NULL;
        for(int i = 0; i < final.size(); ++i){
            argv[i] = final[i].c_str();
        }
        execvp(final[0].c_str(),(char*const*)argv);
    }
}

string parseexec(string comm){
    string res{};
    comm = comm + " |";
    istringstream tok(comm);
    vector<string> comm_tok;
    string tmp;
    while(tok >> tmp){
        comm_tok.push_back(tmp);
    }
    string in_loop{};
    vector<string> pars_comm;
    for(auto i : comm_tok){
        //if(i != "|" && i != ">" && i != "<"){
        if(i != "|"){
            in_loop = in_loop + " " + i;
        }
        else{
            pars_comm.push_back(in_loop);
            in_loop = {};
            pars_comm.push_back(i);
        }
    }
    pars_comm.pop_back();

    if(pars_comm.size() != 0 && pars_comm.size() > 1){
        int n = getpipes(pars_comm);
        int fd[2*n];
        // initialize n -pipes
        for(int i = 0; i < n ;++i){
            pipe(fd + i*2); //fd[][]
        }
        // fork() here and start child processes
        for(int i=0,j = 0; i < n+1;i++,j+=2){
            int pid = fork();
            if(pid == 0){
                if(i !=0 && i != n){
                    dup2(fd[j-2],0);
                    dup2(fd[j+1],1);
                    for(int k = 0; k < 2*n;++k ){close(fd[k]);}
                    core(pars_comm[2*i]);
                    exit(0);
                }
                if(i == 0){
                    dup2(fd[1],1);     
                    for(int k = 0; k < 2*n;++k ){close(fd[k]);}
                    core(pars_comm[2*i]);
                    exit(0);
                }
                if(i == n){
                    dup2(fd[2*n - 2],0);
                    for(int k = 0; k < 2*n; ++k){close(fd[k]);}
                    core(pars_comm[2*i]);
                    exit(0);
                }
            }
            else{ child_pid = pid;}
        }
        for(int k = 0; k< 2*n; ++k){
            close(fd[k]);
        }
        for(int k = 0; k < n+1; ++k){
            wait(NULL);
        }
    }
    else{
        if( pars_comm[0].find("&") == std::string::npos && pars_comm[0].find("cd") == std::string::npos && !redirect_cond(pars_comm[0]) ){
            int pid = fork();
            if(pid  == 0 ){
                core(pars_comm[0]);
                exit(0);
            }
            else if(pid > 0){
                signal(SIGTSTP,suspend_handler);
                child_pid = pid;
                waitpid(0,NULL,WUNTRACED );
            }
        }
        else if(pars_comm[0].find("cd") != std::string::npos){
            core(pars_comm[0]);
        }
        else if(!redirect_cond(pars_comm[0] )) {
            int pid = fork();
            if(pid == 0){
                setpgid(0,0);
                int l = pars_comm[0].find("&");
                pars_comm[0].erase(l);
                core(pars_comm[0]);
                exit(0);
            }
            else if(pid > 0){
                cout << pid << "\n";
                child_pid = pid;
                waitpid(pid,NULL,WNOHANG);
            }
        }
        else{
            handle_redirect(pars_comm[0]);
        }
    }
    return res;
}


