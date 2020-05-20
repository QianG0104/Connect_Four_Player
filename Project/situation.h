//
//  situation.hpp
//  Strategy
//
//  Created by QianG on 2019/5/12.
//  Copyright © 2019 Yongfeng Zhang. All rights reserved.
//

#ifndef situation_h
#define situation_h

#define user 1
#define machine 2
#define edge -1
#define Blank 0

#define bottom 6

#define SIDE 0
#define QUAN 1
#define HEAD 2
#define TAIL 3

#define UD  'A'
#define LR  'B'
#define LURD  'C'
#define RULD  'D'

#define get4    100000000
#define free3   100000//500000
#define single3 10000
#define free2   100
#define single2 10
#define single  1

#define lambda 0.05

class situation
{
private:
    int cuttime=0;
    int deepth=0;
    int topof(int Y);
    double deep(int Y,double bestbro);
    double patternScore(int s,int q,int h,int t,int x,int y,int dir);
    double patternScore(int s,int q,int h,int t,int dir);
    //int BlankPower(int x,int y,int p,int q);
    double situationScore();
    bool endgame(int x,int y);
public:
    int M,N,nx,ny;
    int map[12][12]={edge};

    situation(const int _M, const int _N, const int _nx, const int _ny, int* const* _board);
    int decision();
};

#endif /* situation_h */
