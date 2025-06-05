#include<bits/stdc++.h>
using namespace std;
#define fastIO ios_base::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL)
#define FOR(a,c) for(int (a)=0; (a)<(c); (a)++)
typedef long long int ll;
typedef vector<int> vi;
typedef pair<int,int> pi;
#define F first
#define S second
#define PB push_back
#define POB pop_back
#define MP make_pair
#define all(x) x.begin(), x.end()
#define sortall(x) sort(all(x))

struct Building {
  double x,y,cost;
};

struct Point {
  double x,y;
  Point(double x=0,double y=0):x(x),y(y){}
};

struct FastPolygonSolver{
  vector<Building> b;
  int N,K;
  double bestCost=1e18;
  vector<Point> bestPoly;
  vector<Point> createRect(const vi &idxs){
    double minX=b[idxs[0]].x,maxX=b[idxs[0]].x,minY=b[idxs[0]].y,maxY=b[idxs[0]].y;
    for(auto i: idxs){
      minX=min(minX,b[i].x);
      maxX=max(maxX,b[i].x);
      minY=min(minY,b[i].y);
      maxY=max(maxY,b[i].y);
    }
    return {Point(minX,minY),Point(maxX,minY),Point(maxX,maxY),Point(minX,maxY)};
  }
  double rectPerim(const vi &idxs){
    double minX=b[idxs[0]].x,maxX=b[idxs[0]].x,minY=b[idxs[0]].y,maxY=b[idxs[0]].y;
    for(auto i: idxs){
      minX=min(minX,b[i].x);
      maxX=max(maxX,b[i].x);
      minY=min(minY,b[i].y);
      maxY=max(maxY,b[i].y);
    }
    return 2*((maxX-minX)+(maxY-minY));
  }
  void eval(const vi &sel){
    if((int)sel.size()<K) return;
    double perim=rectPerim(sel), wsum=0;
    for(auto i: sel) wsum+=b[i].cost;
    double total=perim+wsum;
    if(total<bestCost){
      bestCost=total;
      bestPoly=createRect(sel);
    }
  }
  vi selectNeg(){
    vi neg,pos;
    FOR(i,N){
      if(b[i].cost<0) neg.PB(i);
      else pos.PB(i);
    }
    if((int)neg.size()>=K) return neg;
    sortall(pos);
    sort(pos.begin(),pos.end(),[&](int a,int c){return b[a].cost<b[c].cost;});
    for(int i=0;(int)neg.size()<K && i<(int)pos.size();i++) neg.PB(pos[i]);
    return neg;
  }
  vi fastGreedy(){
    vi idx(N),res;
    iota(all(idx),0);
    sort(idx.begin(),idx.end(),[&](int a,int c){return b[a].cost<b[c].cost;});
    FOR(i,K) res.PB(idx[i]);
    for(int i=K;i<N && i<K+10;i++){
      if(b[idx[i]].cost<0){
        vi temp=res;
        temp.PB(idx[i]);
        double cur=rectPerim(res);
        for(auto x: res) cur+=b[x].cost;
        double nw=rectPerim(temp);
        for(auto x: temp) nw+=b[x].cost;
        if(nw<cur) res=temp;
      }
    }
    return res;
  }
  pair<double, vector<Point>> solve(){
    vi s1=selectNeg(); eval(s1);
    vi s2=fastGreedy(); eval(s2);
    vi idx(N); iota(all(idx),0);
    sort(idx.begin(),idx.end(),[&](int a,int c){return b[a].cost<b[c].cost;});
    vi s3(idx.begin(),idx.begin()+K);
    eval(s3);
    return {bestCost,bestPoly};
  }
  FastPolygonSolver(vector<Building> &buildings,int k):b(buildings),N((int)buildings.size()),K(k){}
};

int main(){
  fastIO;
  int N,K; cin>>N>>K;
  vector<Building> b(N);
  FOR(i,N) cin>>b[i].x>>b[i].y>>b[i].cost;
  FastPolygonSolver solver(b,K);
  pair<double, vector<Point>> res = solver.solve();
  double cost = res.first;  
  vector<Point> poly = res.second;
  cout<<fixed<<setprecision(6)<<cost<<"\n";
  int n=(int)poly.size();
  FOR(i,n){
    int j=(i+1)%n;
    cout<<fixed<<setprecision(6)<<poly[i].x<<" "<<poly[i].y<<" "<<poly[j].x<<" "<<poly[j].y<<"\n";
  }
  return 0;
}
