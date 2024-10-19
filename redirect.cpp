#include "main.h"


extern int child_pid;
int ret_in(vector<string> x,string l){
  int tmp;
  for(int i = 0; i < x.size();++i){
    if(x[i] == l){
      tmp = i;
      return i;
    }
  }
  return tmp;
}
void handle_redirect(string pars){
  if(pars.find(">") != string::npos && pars.find("<") != string::npos && pars.find(">>") == string::npos){
    vector<string> final = tokenizer(pars);
    int in1 = ret_in(final,">");
    int in2 = ret_in(final,"<");
    int pid = fork();
    if(pid == 0){
      int wd = open(final[in1+1].c_str(),O_CREAT|O_WRONLY);
      int rd = open(final[in2+1].c_str(),O_RDONLY);
      dup2(wd,1);
      dup2(rd,0);
      core(final[in2-1]);
      close(wd);
      close(rd);
      exit(0);
    }
    else{
      child_pid = pid;
      wait(NULL);
    }
  }
  else if(pars.find(">") != string::npos && pars.find(">>")==string::npos){
    int pid = fork();
    if(pid == 0){
      vector<string> final = tokenizer(pars);
      int in1 = ret_in(final,">");
      int wd = open(final[in1+1].c_str(),O_WRONLY | O_CREAT,0644);
      string tmp_com;
      for(int i = 0; i < in1; ++i){tmp_com = tmp_com + " "+ final[i];}
      dup2(wd,1);
      core(tmp_com);
      close(wd);
      exit(0);
    }
    else{
      child_pid = pid;
      wait(NULL);
    }
  }
  else if(pars.find("<") != string::npos && pars.find(">>") == string::npos){
    int pid = fork();
    if(pid == 0){
      vector<string> final = tokenizer(pars);
      int in1 = ret_in(final,"<");
      int wd = open(final[in1+1].c_str(),O_RDONLY);
      if(wd == -1){ cout << "file doesn't exist" << "\n";exit(0);}
      string tmp_com;
      for(int i = 0; i < in1; ++i){tmp_com = tmp_com + " "+ final[i];}
      dup2(wd,0);
      core(tmp_com);
      close(wd);
      exit(0);
    }
    else{
      child_pid = pid;
      wait(NULL);
    }
  }
  else if(pars.find(">>") != string::npos){
    int pid = fork();
    if(pid == 0){
      vector<string> final = tokenizer(pars);
      int in1 = ret_in(final,">>");
      int wd = open(final[in1+1].c_str(),O_WRONLY | O_CREAT | O_APPEND,0644);
      string tmp_com;
      for(int i = 0; i < in1; ++i){tmp_com = tmp_com + " "+ final[i];}
      cout << tmp_com << "\n";
      dup2(wd,1);
      core(tmp_com);
      close(wd);
      exit(0);
    }
    else{
      child_pid = pid;
      wait(NULL);
    }
  }
}
