#include <bits/stdc++.h>


using namespace std;

class data{
public:
    data(string f, string t, string w){
    this->from = f;
    this->to = t;
    this->weight = atof(w.c_str());
  }
  string from,to;
  double weight;

};
class station{
public:
    string name, gps;
    station(string name, string gps){
      this->name = name;
      this->gps = gps;
    }

};

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

signed main(){
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  string s;
  vector<data> edgeList;

  set<pair<string,string> > stations;

  getline(cin,s);
  while(getline(cin,s)){
    vector<string> in = line(s);
    if(in[20] != "")
    stations.insert(make_pair(in[5],in[20]));
  //  stations.insert(station(in[5],in[20]));
  //  stations.insert(station(in[7],in[21]));
    if(in[9] == "False"){
      swap(in[5],in[7]);
    }

    if(in.size() < 3)break;
    data e = data(in[5],in[7],in[12]);
    edgeList.push_back(e);
  }
  set<string> st;
  for(int i = 0 ;i<edgeList.size(); i++){
    st.insert(edgeList[i].from);
    st.insert(edgeList[i].to);
  }
  map<string, int> mapping;
  int k = 0;
  vector<string> indexing;
  for(auto i:st){
    mapping[i] = k;
    indexing.push_back(i);
    k++;
  }

   //ками
   cout<<stations.size()<<"\n";

  for(auto i:stations){

    string k = "";
    string v = i.second;
    for(int i = 0; i<v.size(); i++){
      if(v[i] != ',')
        k += v[i];
    }
    cout<<i.first<<" "<<k<<"\n";
  }

/*
   //cella
  cout<<edgeList.size()<<"\n";
  for(int i = 0; i<edgeList.size(); i++){
    cout<<edgeList[i].from<<" "<<edgeList[i].to<<" "<<edgeList[i].weight<<"\n";
  }
*/
}
