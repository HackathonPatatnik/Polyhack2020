#include <bits/stdc++.h>


using namespace std;


vector<string> line(string s){
  vector<string> ret;
  string cs = "";
  for(int i = 0; i<s.size(); i++){
    if(s[i] == ';'){
      ret.push_back(cs);
      cs = "";
      continue;
    }
    cs += s[i];
  }
  ret.push_back(cs);
  return ret;
}
string cmp(string s){
  string ret = "";
  for(int i = 0; i<s.size(); i++){
    if(s[i] != '-')ret += s[i];
  }
  return ret;
}

signed main(){
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  string s;
  getline(cin,s);
  vector<string> out;
  while(getline(cin,s)){
    vector<string> in = line(s);
    if(in.size() <= 1)continue;
    if(in[8] == "" && in[9] == "")continue;
    if(in[2].size() == 0 || in[3].size() == 0)continue;
    if(in[12] == "")in[12] = "0";
    in[8] = cmp(in[8]);
    in[9] = cmp(in[9]);
    string k = "";
    k += in[2]+" "+in[3]+" "+in[12]+" "+in[8]+" "+in[9];
    out.push_back(k);
  }
  cout<<out.size()<<"\n";
  for(int i = 0; i<out.size(); i++){
    cout<<out[i]<<"\n";
  }

}
