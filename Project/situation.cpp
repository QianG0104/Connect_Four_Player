//
//  situation.cpp
//  Strategy
//
//  Created by QianG on 2019/5/12.
//  Copyright © 2019 Yongfeng Zhang. All rights reserved.
//

#include "situation.h"
#include <iostream>
#include<float.h>
using namespace std;

situation::situation(const int _M, const int _N, const int _nx, const int _ny, int* const* _board)
{
    M=_M;
    N=_N;
    nx=_nx;
    ny=_ny;
    for(int i=0;i<M;i++){
        for(int j=0;j<N;j++){
            map[i][j]=_board[i][j];
        }
    }
    map[_nx][_ny]=edge;
}

int situation::topof(int Y)
{
    int temp=M;
    for(;temp>0;temp--){
        if(map[temp-1][Y]==Blank)break;
    }
    return temp;
}

double situation::deep(int Y,double bestbro)
{
    //走子
    deepth++;
    int x=topof(Y);
    if(deepth%2==1)map[x-1][Y]=machine;
    else map[x-1][Y]=user;

    //返回or继续深挖
    if(endgame(x-1,Y)||deepth==bottom){
        int nowScore=situationScore();
        //回滚
        map[x-1][Y]=Blank;
        deepth--;
        //
        return nowScore;
    }
    else {
        double best;
        //*
        if(deepth%2==1){
            best=DBL_MAX;//best=DBL_MAX;
        }
        else best=-DBL_MAX;//best=-DBL_MAX;
         //*/

        double scores[12]={0};
        int y=0;
        //double myvalue;

        for(int i=0;i<N;i++){
            if(topof(i)==0)continue;

            scores[i]=deep(i,best);
            //myvalue=deep(i, best);


            /*
            if(deepth%2==1){//己方落子时，考虑最差后续情况，选下一层最小值作为自己的值。而父层（对方）选本层最大值。当本层已经发现小于上层已知最大值bestbro时放弃本层后续搜索。
                if(myvalue<bestbro){cuttime++;
                    //回滚
                    map[x-1][Y]=Blank;
                    deepth--;
                    //
                    return myvalue;
                }
                if(myvalue<best)best=myvalue;
            }
            else{//对方落子时，考虑最好后续情况，选下一层最大值作为自己的值。而父层（自己）选本层最小值。当本层已经发现大于上层已知最小值bestbro时放弃本层后续搜索。
                if(myvalue>bestbro){cuttime++;
                    //回滚
                    map[x-1][Y]=Blank;
                    deepth--;
                    //
                    return myvalue;
                }
                if(myvalue>best)best=myvalue;
            }
            //*/

            //*
            if(deepth%2==1){
                if(scores[i]<bestbro){//奇数层从下一层中选最小值，而父层选本层最大值。当本层已经发现小于上层已知最大值bestbro时放弃本层后续搜索。
                    cuttime++;
                    //回滚
                    map[x-1][Y]=Blank;
                    deepth--;
                    //
                    return scores[i];
                }
                if(scores[i]<scores[y])y=i;
            }
            else{
                if(scores[i]>bestbro){//偶数层从下一层中选最大值，则父层选本层最小值。当本层已经发现大于上层已知最小值bestbro时放弃本层后续搜索。
                    cuttime++;
                    //回滚
                    map[x-1][Y]=Blank;
                    deepth--;
                    //
                    return scores[i];
                }
                if(scores[i]>scores[y])y=i;
            }
            best=scores[y];
            // */
        }
        //回滚
        map[x-1][Y]=Blank;
        deepth--;
        //
        return scores[y];
        //return myvalue;
    }
}

int situation::decision()
{
    double scores[12]={0};
    int y=0;
    //cout<<"lambda="<<lambda<<" | deepth="<<bottom<<endl;
    for(int i=0;i<N;i++){
        if(topof(i)==0)continue;
        scores[i]=deep(i,-DBL_MAX);
        if(scores[i]>scores[y])y=i;
        //cout<<"*******"<<endl<<i<<" "<<scores[i]<<endl;
    }
    //cout<<cuttime<<"cuts"<<endl;
    return y;
}

double situation::situationScore()
{
    double sc=0;

    int u2d[12][4];//up to down
    int l2r[12][4];//left to right
    int lu2rd[23][4];//left-up to right-down
    int ru2ld[23][4];//right-up to left-down

    for(int i=0;i<23;i++){
        if(i<12){
            u2d[i][SIDE]=edge;//side: edge or blank or side
            u2d[i][QUAN]=1;//quantity: 1+
            u2d[i][HEAD]=edge;//head: edge or blank or other side
            u2d[i][TAIL]=0;//tail: edge or blank or other side

            l2r[i][SIDE]=edge;
            l2r[i][QUAN]=0;
            l2r[i][HEAD]=0;
            l2r[i][TAIL]=0;
        }
        lu2rd[i][SIDE]=edge;
        lu2rd[i][QUAN]=0;
        lu2rd[i][HEAD]=0;
        lu2rd[i][TAIL]=0;

        ru2ld[i][SIDE]=edge;
        ru2ld[i][QUAN]=0;
        ru2ld[i][HEAD]=0;
        ru2ld[i][TAIL]=0;
    }

    for(int i=0;i<M;i++){
        for(int j=0;j<N;j++){
            int here=map[i][j];

            if(here==u2d[i][SIDE])u2d[i][QUAN]++;
            else {
                u2d[i][TAIL]=here;
                sc+=patternScore(u2d[i][SIDE], u2d[i][QUAN], u2d[i][HEAD], u2d[i][TAIL],i,j,UD);
                u2d[i][HEAD]=u2d[i][SIDE];
                u2d[i][SIDE]=here;
                u2d[i][QUAN]=1;
            }

            if(here==l2r[j][SIDE])l2r[j][QUAN]++;
            else {
                l2r[j][TAIL]=here;
                sc+=patternScore(l2r[j][SIDE], l2r[j][QUAN], l2r[j][HEAD], l2r[j][TAIL],i,j,LR);
                l2r[j][HEAD]=l2r[j][SIDE];
                l2r[j][SIDE]=here;
                l2r[j][QUAN]=1;
            }

            int p=i+j;
            if(here==lu2rd[p][SIDE])lu2rd[p][QUAN]++;
            else {
                lu2rd[p][TAIL]=here;
                sc+=patternScore(lu2rd[p][SIDE], lu2rd[p][QUAN], lu2rd[p][HEAD], lu2rd[p][TAIL],i,j,LURD);
                lu2rd[p][HEAD]=lu2rd[p][SIDE];
                lu2rd[p][SIDE]=here;
                lu2rd[p][QUAN]=1;
            }

            int q=i-j+11;
            if(here==ru2ld[q][SIDE])ru2ld[q][QUAN]++;
            else {
                ru2ld[q][TAIL]=here;
                sc+=patternScore(ru2ld[q][SIDE], ru2ld[q][QUAN], ru2ld[q][HEAD], ru2ld[q][TAIL],i,j,RULD);
                ru2ld[q][HEAD]=ru2ld[q][SIDE];
                ru2ld[q][SIDE]=here;
                ru2ld[q][QUAN]=1;
            }
        }
    }


    for(int i=0;i<23;i++){
        if(i<12){
            u2d[i][TAIL]=edge;
            l2r[i][TAIL]=edge;
            sc+=patternScore(u2d[i][SIDE], u2d[i][QUAN], u2d[i][HEAD], u2d[i][TAIL],UD);
            sc+=patternScore(l2r[i][SIDE], l2r[i][QUAN], l2r[i][HEAD], l2r[i][TAIL],LR);
        }
        lu2rd[i][TAIL]=edge;
        ru2ld[i][TAIL]=edge;
        sc+=patternScore(lu2rd[i][SIDE], lu2rd[i][QUAN], lu2rd[i][HEAD], lu2rd[i][TAIL],LURD);
        sc+=patternScore(ru2ld[i][SIDE], ru2ld[i][QUAN], ru2ld[i][HEAD], ru2ld[i][TAIL],RULD);
    }

    return sc;
}

/*
double situation::patternScore(int s,int q,int h,int t,int x,int y,int dir)
{
    if(s==edge||s==Blank)return 0;
    double flag=1;
    if(s==user)flag=-1;
    if(q>=4)return flag*get4;
    if(h==Blank&&t==Blank){
        if(q==3){
            //cout<<dir<<" free3"<<endl;
            int d1=topof(y)-1-x;
            int d2=0;
            if(dir==LR)d2=topof(y-4)-1-x;
            else if(dir==LURD)d2=topof(y-4)-1-(x-4);
            else if(dir==RULD)d2=topof(y+4)-1-(x-4);
            switch (d1+d2) {
                case 0:flag*=2;
                case 1:flag*=1.5;
                case 2:flag*=1.25;break;
                default:break;
            }
            return flag*free3;
        }
            //return flag*free3;
        if(q==2){
            //cout<<dir<<" free2"<<endl;
            int d1=topof(y)-1-x;
            int d2=0;
            if(dir==LR)d2=topof(y-3)-1-x;
            else if(dir==LURD)d2=topof(y-3)-1-(x-3);
            else if(dir==RULD)d2=topof(y+3)-1-(x-3);
            switch (d1+d2) {
                case 0:flag*=2;
                case 1:flag*=1.5;
                case 2:flag*=1.25;break;
                default:break;
            }
            return flag*free2;
        }
            //return flag*free2;
        if(q==1){
            //cout<<dir<<" free1"<<endl;
            int d1=topof(y)-1-x;
            int d2=0;
            if(dir==LR)d2=topof(y-2)-1-x;
            else if(dir==LURD)d2=topof(y-2)-1-(x-2);
            else if(dir==RULD)d2=topof(y+2)-1-(x-2);
            switch (d1+d2) {
                case 0:flag*=1.25;
                case 1:flag*=1.25;break;
                default:break;
            }
            return flag*free3;
        }
            //return flag*single;
    }
    else if(h==Blank||t==Blank){
        if(q==3)return flag*single3;
        if(q==2)return flag*single2;
        if(q==1)return flag*single;
    }
    return 0;
}
*/
double situation::patternScore(int s,int q,int h,int t,int x,int y,int dir)
{
    if(s==edge||s==Blank)return 0;
    double flag=1.0-lambda*(deepth/bottom);
    if(s==user)flag=-1;
    if(q>=4)return flag*get4;
    if(h==Blank&&t==Blank){
        if(q==3)return flag*free3;
        if(q==2)return flag*free2;
        if(q==1)return flag*single;
    }
    else if(h==Blank||t==Blank){
        if(q==3)return flag*single3;
        if(q==2)return flag*single2;
        if(q==1)return flag*single;
    }
    return 0;
}

double situation::patternScore(int s,int q,int h,int t,int dir)
{
    if(s==edge||s==Blank)return 0;
    double flag=1.0-lambda*(deepth/bottom);
    if(s==user)flag*=-1;
    if(q>=4)return flag*get4;
    if(h==Blank&&t==Blank){
        if(q==3)return flag*free3;
        if(q==2)return flag*free2;
        if(q==1)return flag*single;
    }
    else if(h==Blank||t==Blank){
        if(q==3)return flag*single3;
        if(q==2)return flag*single2;
        if(q==1)return flag*single;
    }
    return 0;
}

bool situation::endgame(int x,int y)
{
    int side=map[x][y];
    //横向检测
    int i, j;
    int count = 0;
    for (i = y; i >= 0; i--)
        if (!(map[x][i] == side))
            break;
    count += (y - i);
    for (i = y; i < N; i++)
        if (!(map[x][i] == side))
            break;
    count += (i - y - 1);
    if (count >= 4) return true;

    //纵向检测
    count = 0;
    for (i = x; i < M; i++)
        if (!(map[i][y] == side))
            break;
    count += (i - x);
    if (count >= 4) return true;

    //左下-右上
    count = 0;
    for (i = x, j = y; i < M && j >= 0; i++, j--)
        if (!(map[i][j] == side))
            break;
    count += (y - j);
    for (i = x, j = y; i >= 0 && j < N; i--, j++)
        if (!(map[i][j] == side))
            break;
    count += (j - y - 1);
    if (count >= 4) return true;

    //左上-右下
    count = 0;
    for (i = x, j = y; i >= 0 && j >= 0; i--, j--)
        if (!(map[i][y] == side))
            break;
    count += (y - j);
    for (i = x, j = y; i < M && j < N; i++, j++)
        if (!(map[i][y] == side))
            break;
    count += (j - y - 1);
    if (count >= 4) return true;

    return false;
}


/*
double situation::patternScore(int s,int q,int h,int t,int x,int y,int dir)
{
    if(s==edge||s==Blank)return 0;
    int flag=1;
    if(s==user)flag=-1;
    if(q>=4)return flag*get4;
    if(h==Blank&&t==Blank){
        if(q==3)return flag*free3;
        if(q==2)return flag*free2;
        if(q==1)return flag*single;
    }
    else if(h==Blank||t==Blank){
        if(q==3)return flag*single3;
        if(q==2)return flag*single2;
        if(q==1)return flag*single;
    }
    return 0;
}

double situation::patternScore(int s,int q,int h,int t,int dir)
{
    if(s==edge||s==Blank)return 0;
    double flag=1.0-0.3*(deepth/bottom);
    if(s==user)flag*=-1;
    if(q>=4)return flag*get4;
    if(h==Blank&&t==Blank){
        if(q==3)return flag*free3;
        if(q==2)return flag*free2;
        if(q==1)return flag*single;
    }
    else if(h==Blank||t==Blank){
        if(q==3)return flag*single3;
        if(q==2)return flag*single2;
        if(q==1)return flag*single;
    }
    return 0;
}
*/














