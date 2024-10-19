#include "main.h"

bool sanitize(string str){
    istringstream tok(str);
    vector<string> res;
    string tmp;
    while(tok >> tmp){
        res.push_back(tmp);
    }
    return res.empty();
}

pair<string,bool> autocomplete(string str){
    vector<string> comp;
    string r1;
    bool flag = 0;
    if(str.empty()){str = {};}
    //cout << str << "this  is the string " << "\n";
    vector<string> res;
    struct dirent* dir_val;
    DIR * dir = opendir(".");
    while((dir_val = readdir(dir)) != NULL){
        res.push_back(dir_val->d_name);
    }

    if(!sanitize(str)){
        vector<string> in;
        string tmp;
        istringstream tok(str);
        while(tok >> tmp){
            in.push_back(tmp);
        }

        for(auto i : res){
            if(i.find(in[in.size() - 1]) != string::npos && i[0] == (in[in.size()-1])[0]){
                comp.push_back(i);
            }
        }
        string part_st{};
        if(comp.size() == 0){
            r1 = {};
        }
        else if(comp.size() ==1){
            if(comp[0] == in[in.size()-1]){r1 = str;}
            else{
                int l = str.find(in[in.size() -1]);
                int e = l + (in[in.size() -1]).length();
                str.erase(str.begin()+l,str.begin()+e);
                r1 = str + comp[0];
                flag = 1;
            }
        }
        else if(comp.size() > 1){
            part_st = comp[0];
            for(int i = 1; i < comp.size(); ++i){
                part_st = part_st + "\t" +comp[i];
            }
            r1 = part_st;
        }
    }
    else if(res.size() > 2){
        // r1 = res[2];
        for(int i = 2; i < res.size(); ++i){
            r1 = r1 +"\n"+ res[i];
        }
        r1 = r1 + "\n";
    }
    else{
        r1 = string{};
    }
    closedir(dir);
    pair<string,bool> ret = make_pair(r1,flag);
    return ret;
}
