#include "dimage.h"
#include "cimage.h"
#include "image.h"
#include "rec.h"


#define min(a,b) ((a)<(b)?(a):(b))


#define max(a,b) ((a)>(b)?(a):(b))

/*
    Copyright (C) 2002
    For non-commercial use only.

    File	: dimage.cpp
    Date	: 01/07/2002
    Author	: O. Senhadji El Rhazi
    Contact	: senhadjielrhazi@gmail.com
    Projet	: Vision
    Ecole	: Ecole Nationale des Ponts et Chausses
*/

dimage::dimage()
{
	g=0;
}

void dimage::alloc(int _w,int _h)
{
	w=_w;h=_h;
	g=new double[w*h];
}

dimage::dimage(int _w,int _h)
{
	alloc(_w,_h);
}

dimage::dimage(const image& ima)
{
	alloc(ima.width(),ima.height());
	for (int i=0;i<w;i++)
		for (int j=0;j<h;j++)
			g[i+w*j]=ima(i,j);
}

dimage::dimage(const dimage& ima)
{
	alloc(ima.w,ima.h);
	for (int i=0;i<w;i++)
		for (int j=0;j<h;j++)
			g[i+w*j]=ima(i,j);
}

dimage::dimage(const cimage& ima,bool phase)
{
	alloc(ima.width(),ima.height());
	for (int i=0;i<w;i++)
		for (int j=0;j<h;j++)
			g[i+w*j]=phase?ima(i,j).phase():ima(i,j).module();
}

void dimage::kill()
{
	if (g) {
		delete[] g;
		g=0;
	}
}

dimage::~dimage(void)
{
	kill();
}

const dimage& dimage::operator=(const dimage& ima)
{
	kill();
	alloc(ima.w,ima.h);
	for (int i=0;i<w;i++)
		for (int j=0;j<h;j++)
			g[i+w*j]=ima(i,j);
	return *this;
}

void dimage::range(double &m,double &M) const
{
	m=M=g[0];
	for (int i=1;i<w*h;i++) {
		if (m>g[i])
			m=g[i];
		if (M<g[i])
			M=g[i];
	}
}

void dimage::display(int x,int y,bool rescale) const
{
	image ima(*this,rescale);
	ima.display(x,y);
}


dimage dimage::lowFilter(double r0,double alpha) const
{
	cimage C1(*this);
	cimage F=C1.fft();
	for (int i=0;i<w;i++)
		for (int j=0;j<h;j++) {
			int x=(i+w/2)%w;
			int y=(j+h/2)%h;
			double r=sqrt((x-w/2)*(x-w/2)+(y-h/2)*(y-h/2));
			F(i,j)=F(i,j)*(1+exp(-alpha*r0))/(1+exp(-alpha*(r0-r)));
		}
	cimage C2=F.fft(true);
	return dimage(C2);
}

dimage dimage::lowRectFilter(int wf,int hf) const
{
	cimage C1(*this);
	cimage F=C1.fft();
	for (int i=0;i<w;i++)
		for (int j=0;j<h;j++) {
			int x=(i+w/2)%w;
			int y=(j+h/2)%h;
			if (abs(x-w/2)>wf || abs(y-h/2)>hf)
				F(i,j)=0;
		}
	cimage C2=F.fft(true);
	return dimage(C2);
}

dimage dimage::highFilter(double r0,double alpha) const
{
	cimage C1(*this);
	cimage F=C1.fft();
	for (int i=0;i<w;i++)
		for (int j=0;j<h;j++) {
			int x=(i+w/2)%w;
			int y=(j+h/2)%h;
			double r=sqrt((x-w/2)*(x-w/2)+(y-h/2)*(y-h/2));
			F(i,j)=F(i,j)*(1-(1+exp(-alpha*r0))/(1+exp(-alpha*(r0-r))));
		}
	cimage C2=F.fft(true);
	return dimage(C2);
}

dimage dimage::reduce() const
{
	dimage D(w/2,h/2);
	for (int i=0;i<w/2;i++)
		for (int j=0;j<h/2;j++)
			D(i,j)=(*this)(2*i,2*j);
	return D;
}

dimage dimage::reduceFilter() const
{
	dimage F=this->lowRectFilter(w/4,h/4);
	dimage D(w/2,h/2);
	for (int i=0;i<w/2;i++)
		for (int j=0;j<h/2;j++)
			D(i,j)=F(2*i,2*j);
	return D;
}

dimage dimage::reduceMean() const
{
	dimage D(w/2,h/2);
	for (int i=0;i<w/2;i++)
		for (int j=0;j<h/2;j++) {
			int i1=min(2*i+1,w-1);
			int j1=min(2*j+1,h-1);
			D(i,j)=((*this)(2*i,2*j)+(*this)(i1,2*j)+(*this)(2*i,j1)+(*this)(i1,j1))/4;
		}
	return D;
}

dimage dimage::enlarge() const
{
	dimage D(2*w,2*h);
	for (int i=0;i<2*w;i++)
		for (int j=0;j<2*h;j++)
			D(i,j)=(*this)(i/2,j/2);
	return D;
}



image dimage::zeros(dimage* R, double seuil) const{

	image D(w,h);
	for(int i=0;i<w-1;i++)
		for(int j=0;j<h-1;j++)
	{
			bool res = false;
			 if( (*this)(i,j)*(*this)(i+1,j) < 0) res = true;
		else if( (*this)(i,j)*(*this)(i,j+1) < 0) res = true;
		else if( (*this)(i,j)*(*this)(i+1,j+1) < 0) res = true;
		if(R)
		{if( (*R)(i,j)<seuil ) res=false;}
		if(res) D(i,j)=255;
		else D(i,j)=1;
	}
	return D;
}

image dimage::contour(double seuil,double lissage){
	
dimage P=deriche(1,0,lissage);
dimage Q=deriche(0,1,lissage);
dimage PP=deriche(2,0,lissage);
dimage QQ=deriche(0,2,lissage);
dimage PQ=deriche(1,1,lissage);

dimage R(w,h);
for(int i=0;i<w;i++)
	for(int j=0;j<h;j++)
		R(i,j)=P(i,j)*P(i,j)+Q(i,j)*Q(i,j);


dimage RR(w,h);
for(i=0;i<w;i++)
	for(int j=0;j<h;j++)
		RR(i,j)=P(i,j)*P(i,j)*PP(i,j)+Q(i,j)*Q(i,j)*QQ(i,j)+2*P(i,j)*Q(i,j)*PQ(i,j);

image M=RR.zeros(&R,seuil);
return M;
}
			
void dimage::cercle(int x0,int y0,double r){

	for (int i=0;i<w;i++)
			for (int j=0;j<h;j++)
				(*this)(i,j)=sqrt((i-x0)*(i-x0)+(j-y0)*(j-y0))-r;
}

double dimage::diff(int x,int y,int var){

int i=x;
int j=y;
 double Dpv, Dmv;

if(var==1){
	
	Dpv=(*this)((i+1+w)%w,j)-(*this)(i,j);
	Dmv=(*this)(i,j)-(*this)((i-1+w)%w,j);
	}
if (var==2){
	Dpv=(*this)(i,(j+1+h)%h)-(*this)(i,j);
	Dmv=(*this)(i,j)-(*this)(i,(j-1+h)%h);}

return max(max(0,Dpv),Dmv);
}	


dimage dimage::beta1(double dt) {
	dimage Res(w,h);
	for (int i=0;i<w;i++)
		for (int j=0;j<h;j++){
//cout<<i<<" "<<j<<endl;

				Res(i,j)=(*this)(i,j)+dt*sqrt( (*this).diff(i,j,1)* (*this).diff(i,j,1)+ (*this).diff(i,j,2)*(*this).diff(i,j,2));}
		return Res;
}


dimage dimage::enlargeInterpol() const
{
	dimage D(2*w,2*h);
	for (int i=0;i<w;i++)
		for (int j=0;j<h;j++) {
			int i1=min(i+1,w-1);
			int j1=min(j+1,h-1);
			D(2*i,2*j)=(*this)(i,j);
			D(2*i+1,2*j)=((*this)(i,j)+(*this)(i1,j))/2;
			D(2*i,2*j+1)=((*this)(i,j)+(*this)(i,j1))/2;
			D(2*i+1,2*j+1)=((*this)(i,j)+(*this)(i,j1)+(*this)(i1,j)+(*this)(i1,j1))/4;
		}
	return D;
}

dimage dimage::convolve(const dimage& K,int u0,int v0) const
{
	dimage D(w,h);
	for (int i=0;i<w;i++)
		for (int j=0;j<h;j++) {
			D(i,j)=0;
			for (int u=0;u<K.w;u++)
				for (int v=0;v<K.h;v++)
					D(i,j)+=K(u,v)*(*this)((i+u-u0+w)%w,(j+v-v0+h)%h);
		}
	return D;
}

dimage dimage::sharpen(double alpha) const
{
	dimage K(3,3);
	K(0,0)=K(2,0)=K(0,2)=K(2,2)=0;
	K(1,0)=K(0,1)=K(1,2)=K(2,1)=-alpha;
	K(1,1)=1+4*alpha;
	return this->convolve(K,1,1);
}

dimage dimage::deriche(int dx,int dy,double alpha) const
{
	dimage D(w,h);
	RecDericheFilter(g,w,h,dx,dy,alpha,D.g);
	return D;
}

