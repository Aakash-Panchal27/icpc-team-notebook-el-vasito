#include <bits/stdc++.h>
#define pb push_back
#define mp make_pair
#define fst first
#define snd second
#define fore(i,a,b) for(int i=a,ThxDem=b;i<ThxDem;++i)
using namespace std;
typedef long long ll;

typedef int tp; // type of polynomial
template<class T=tp>
struct poly {  // poly<> : 1 variable, poly<poly<>>: 2 variables, etc.
	vector<T> c;
	T& operator[](int k){return c[k];}
	poly(vector<T>& c):c(c){}
	poly(initializer_list<T> c):c(c){}
	poly(int k):c(k){}
	poly(){}
	poly operator+(poly<T> o){
		int m=c.size(),n=o.c.size();
		poly res(max(m,n));
		fore(i,0,m)res[i]=res[i]+c[i];
		fore(i,0,n)res[i]=res[i]+o.c[i];
		return res;
	}
	poly operator*(tp k){
		poly res(c.size());
		fore(i,0,c.size())res[i]=c[i]*k;
		return res;
	}
	poly operator*(poly o){
		int m=c.size(),n=o.c.size();
		poly res(m+n-1);
		fore(i,0,m)fore(j,0,n)res[i+j]=res[i+j]+c[i]*o.c[j];
		return res;
	}
	poly operator-(poly<T> o){return *this+(o*-1);}
	T operator()(tp v){
		T sum(0);
		for(int i=c.size()-1;i>=0;--i)sum=sum*v+c[i];
		return sum;
	}
};
// example: p(x,y)=2*x^2+3*x*y-y+4
// poly<poly<>> p={{4,-1},{0,3},{2}}
// printf("%d\n",p(2)(3)) // 27 (p(2,3))
set<tp> roots(poly<> p){ // only for integer polynomials
	set<tp> r;
	while(!p.c.empty()&&!p.c.back())p.c.pop_back();
	if(!p(0))r.insert(0);
	if(p.c.empty())return r;
	tp a0=0,an=abs(p[p.c.size()-1]);
	for(int k=0;!a0;a0=abs(p[k++]));
	vector<tp> ps,qs;
	fore(i,1,sqrt(a0)+1)if(a0%i==0)ps.pb(i),ps.pb(a0/i);
	fore(i,1,sqrt(an)+1)if(an%i==0)qs.pb(i),qs.pb(an/i);
	for(auto pt:ps)for(auto qt:qs)if(pt%qt==0){
		tp x=pt/qt;
		if(!p(x))r.insert(x);
		if(!p(-x))r.insert(-x);
	}
	return r;
}
pair<poly<>,tp> ruffini(poly<> p, tp r){ // returns pair (result,rem)
	int n=p.c.size()-1;
	vector<tp> b(n);
	b[n-1]=p[n];
	for(int k=n-2;k>=0;--k)b[k]=p[k+1]+r*b[k+1];
	return {poly<>(b),p[0]+r*b[0]};
}
// only for double polynomials
poly<> interpolate(vector<tp> x, vector<tp> y){
	poly<> q={1},S={0};
	for(tp a:x)q=poly<>({-a,1})*q;
	fore(i,0,x.size()){
		poly<> Li=ruffini(q,x[i]).fst;
		Li=Li*(1.0/Li(x[i])); // change for int polynomials
		S=S+Li*y[i];
	}
	return S;
}

int main(){
	poly<poly<>> p={{4,-1},{0,3},{2}}; // p(x,y)=2*x^2+3*x*y-y+4
	assert(p(2)(3)==27);
	poly<poly<>> c={{3,7,2},{5,4}};
	poly<poly<>> d={{1,2,3},{4,5,6,7}};
	poly<poly<>> cd=c*d;
	poly<poly<>> s=c+d;
	poly<poly<>> z=c-d;
	assert(c(10)(7)*d(10)(7)==cd(10)(7));
	assert(c(10)(7)+d(10)(7)==s(10)(7));
	assert(c(10)(7)-d(10)(7)==z(10)(7));
	poly<> x={0,1};
	poly<> u={1};
	poly<> a=(x-u)*(x-u*5)*(x-u*9)*(x+u*15)*x*(x+u*7);
	set<tp> r=roots(a);
	assert(roots(a)==set<tp>({-15,-7,0,1,5,9}));
	auto w=ruffini(a,-7);
	assert(w.snd==0);
	poly<> a2=(x-u)*(x-u*5)*(x-u*9)*(x+u*15)*x;
	assert(w.fst.c==a2.c);
	return 0;
}
