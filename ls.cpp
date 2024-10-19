#include "main.h"

void func(char *out_val,long long  buf_perm){
    if(buf_perm&S_IRUSR){
        out_val[0] = 'r';
    }
    if(buf_perm&S_IWUSR){
        out_val[1] = 'w';
    }
    if(buf_perm&S_IXUSR){
        out_val[2] = 'x';
    }
    if(buf_perm&S_IRGRP){
        out_val[3] = 'r';
    }
    if(buf_perm&S_IWGRP){
        out_val[4] = 'w';
    }
    if(buf_perm&S_IXGRP){
        out_val[5] = 'x';
    }
    if(buf_perm&S_IROTH){
        out_val[6] = 'r';
    }
    if(buf_perm&S_IWOTH){
        out_val[7] = 'w';
    }
    if(buf_perm&S_IXOTH){
        out_val[8] = 'x';
    }
    if((buf_perm&S_IFMT) == S_IFDIR ){
        out_val[9] = 'd';
    }
}
string ls_dir(string dir_name,string flags){
    vector<string> res;
    char* tmp_buff= new char[100];
    if(dir_name == string{}){
        getcwd(tmp_buff,100);
        dir_name = string(tmp_buff);
    }
    struct dirent* dir_val;
    DIR * dir = opendir(dir_name.c_str());
    while((dir_val = readdir(dir)) != NULL){

        res.push_back(dir_val->d_name);
    }
    if(flags.find("-l") != string::npos){
        for(int i = 0; i < res.size();++i){
            char out_val[10];
            for(int j = 0 ; j < 10; ++j){out_val[j] = '-';}
            struct stat curr_buff;
            stat(res[i].c_str(),&curr_buff);
            func(out_val,curr_buff.st_mode);
            struct group *one;
            struct passwd *two;
            two = getpwuid(curr_buff.st_uid);
            one = getgrgid(curr_buff.st_gid);
            string user_name = two->pw_name;
            string group_name = one->gr_name;
            string permission{};
            permission.push_back(out_val[9]);
            for(int j = 0; j < 9; ++j){permission.push_back(out_val[j]);}
            long long file_size = curr_buff.st_size;
            struct timespec time_ac ;
            time_ac = curr_buff.st_ctim;
            timespec_get(&time_ac,TIME_UTC);
            char time_buff[100] ;
            strftime(time_buff,sizeof(time_buff),"%x - %I:%M%p",gmtime(&time_ac.tv_nsec));
            res[i] =  permission + "\t" + user_name + "\t" + group_name + "\t"+ to_string(file_size)+ "\t"+ string(time_buff)+ "\t"+ res[i];
        }
    }
    string r1;
    for(auto i : res){
        r1 = r1 + i + "\n";
    }
    closedir(dir);
    delete[] tmp_buff;
    return r1;
}
