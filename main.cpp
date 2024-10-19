#include "main.h"
#include <fcntl.h>
#include <streambuf>


//global parent pid value
int global_parent_pid;

struct termios curr,previous;
void termio_init(){
    tcgetattr(0,&previous);
    curr = previous;
    curr.c_lflag &= ~(~ECHO | ECHOE | ICANON);
    tcsetattr(0,TCSANOW,&curr);
}
void reset_term(){
    tcsetattr(0,TCSANOW,&previous);
}
char getch(){
    char ch;
    termio_init();
    ch = getchar();
    reset_term();
    return ch;
}

string adjust(){
    string home = getenv("HOME");
    char* tmp_buff = new char[100];
    getcwd(tmp_buff,100);
    string res = string(tmp_buff);
    int in = res.find(home);
    if(in != std::string::npos){
        res.erase(in,in+home.length());
        res = "~"+res;
    }
    delete[] tmp_buff;
    return res;
}
void autocomplete(string& curr_comm,string & dir_name){
    cout << dir_name ;
}
//<---------------------------------------------pwd----------------------------------------------->// 
string pwd(){
    string res{};
    char* dir_name = new char[100];
    getcwd(dir_name,100);
    res = string(dir_name);
    delete[] dir_name;
    return res;
}
//<---------------------------------------------search----------------------------------------------->// 
bool is_dir(string name){
    bool res=0;
    struct stat cur_buff;
    stat(name.c_str(),&cur_buff);
    (((cur_buff.st_mode)&S_IFMT)==S_IFDIR)? res = 1 :res = 0;
    return res;
}

bool search(string file_name){
    static bool res_exit = 0;
    struct dirent* dir_val;
    string curr_dir= pwd();
    DIR* dir = opendir(".");
    while((dir_val = readdir(dir)) != NULL){
        if(string(dir_val->d_name) == file_name){
            res_exit = true;
            return res_exit;
        }
        else if(string(dir_val->d_name) != "."&& string(dir_val->d_name) != ".."&& is_dir(dir_val->d_name)){
            chdir(dir_val->d_name);
            search(file_name);
            chdir(curr_dir.c_str());
        }
    }
    closedir(dir);
    return res_exit;
}
//<---------------------------------------------echo----------------------------------------------->//
string echo(string val){
    istringstream tok(val);
    vector<string> res;
    string tmp= string {};
    while(tok >> tmp){
        res.push_back(tmp);
    }
    if(!res.empty()){
        string res2 = res[0];
        for(int i = 1; i < res.size();++i){
            res2 = res2+" " +res[i];
        }
        tmp = res2;
    }
    return tmp;
}
void interrupt_handler(int sig){

}
vector<string> tokenize_semi(string one){
    string tmp;
    stringstream s(one);
    vector<string> res;
    while(getline(s,tmp,';')){
        res.push_back(tmp);
    }
    return res;
}
void remove(vector<string> &x,int n){
    int len;
    (x.size() > n)?(len = x.size() - n):(len = 0);
    if(len >  0){
        x.erase(x.begin(),x.begin()+len);
    }
}
void write_to_hist(vector<string> x,int fd){
    remove(x,20);
    string tmp;
    for(int i = 0; i < x.size(); ++i){
        tmp = tmp + x[i] + "\n";
    }
    lseek(fd,0,SEEK_SET);
    const char* real = tmp.c_str();
    write(fd,real,strlen(real));
    close(fd);
    cout << tmp << "\n";
}
vector<string> curr_comm({});

int main(){
    struct utsname buf;
    uname(&buf);
    string sys_name = buf.nodename;
    string res = getlogin();
    // read the file for history;
    struct stat read_file;
    stat("hist.txt",&read_file);
    int fd = open("hist.txt",O_RDWR | O_CREAT,0644);
    // get file info
    int total = read_file.st_size;
    if(total > 0){
        char* tmp_file_buff = new char[total];
        read(fd,tmp_file_buff,total);
        curr_comm = tokenizer(string(tmp_file_buff));
        curr_comm.pop_back();
        delete[] tmp_file_buff;
    }
    global_parent_pid = getpid();

    // set the environment variable
    char* tmp_buff = new char[100];
    getcwd(tmp_buff,100);
    setenv("HOME",tmp_buff,1);
    vector<string> tmp_comm({});
    bool set_bit = 0;
    int hist_val = 0;
    bool hist_bit = 0;
    bool can_exec = 0;
    bool auto_bit = 0;
    string autocomp;
    signal(SIGINT,child_interrupt);
    while(true){
        can_exec = 1;
        remove(curr_comm,20);
        char ch{};
        string str;
        if(hist_bit != 1){ (curr_comm.size() == 0)?(hist_val = 0):(hist_val = curr_comm.size());}
        if(hist_bit ==  1){
            str = curr_comm[hist_val];
            hist_bit = 0;
        }
        if(set_bit == 1){
            str =  tmp_comm[tmp_comm.size() - 1];
            set_bit = 0;
        }
        if(auto_bit == 1){
            str = autocomp;
            auto_bit = 0;
        }
        string tmp_add = adjust();
        string prompt = res + "@" + sys_name +":" + tmp_add + "$ " + str;
        cout << prompt;
        while(ch != '\n'){

            ch = getch();
            if(ch == 4){write_to_hist(curr_comm,fd);kill(global_parent_pid,SIGTERM);}
            if(ch == 27){
                getch();
                char l = getch();
                if(l == 'A' && !curr_comm.empty()){
                    if(hist_val - 1 <= 0 ){
                        hist_val = 0;
                    }
                    else{
                        hist_val = hist_val - 1;
                    }
                    hist_bit = 1;
                    can_exec = 0;
                }
                cout << "\033[H\033[J";
                break;
            }
            if(ch != '\t'){str = str + ch;}
            if(ch == '\t'){
                if(autocomplete(str).second == 1){
                    autocomp = autocomplete(str).first;
                }
                else{
                    cout << autocomplete(str).first << "\n";
                    autocomp = str;
                }
                auto_bit = 1;
                can_exec = 0;
                break;
            }
            if(ch == 127 ){
                if(str.length() > 2){
                    str.erase(str.length()-2,2);
                    tmp_comm.push_back(str) ;
                    set_bit = 1;
                    can_exec = 0;
                    break;
                }
                else{
                    can_exec = 0;
                    break;
                }
            }
        }
        if(can_exec){
            if(!str.empty() && !sanitize(str)){
                curr_comm.push_back(echo(str));
                for(string i :tokenize_semi(str))
                {
                    parseexec(i);
                }
            }
        }
        cout << "\33[2K\r" << flush;
        fflush(stdout);
    }
    delete[] tmp_buff;
}
