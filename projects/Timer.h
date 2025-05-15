#ifndef _E_TIMER_H
#define _E_TIMER_H

#pragma once

#include <windows.h>
#include <stdio.h>
#include <math.h>

// CLASS DEF

class CPPTimer
{
private:
    __int64       _i64PTimerStart;
    __int64       _i64PTimerElapsed;

    unsigned long MTimerStart;
    unsigned long MTimerElapsed;
    

    __int64       Frequency; //push update
    float         Resolution;  //dimentions
    bool          usingPTimer;

    float f_RUN_Time1;
    float f_RUN_Time2;
    float timeDiff;
    float M_FPS;
    int MFramesElapsed;

public:
    CPPTimer() : M_FPS(0.0f), f_RUN_Time1(0.0f), f_RUN_Time2(0.0f), timeDiff(0.0f), MFramesElapsed(0){   } //value construct
    bool Init(){
        if (QueryPerformanceFrequency((LARGE_INTEGER*)&Frequency)){
            usingPTimer = true;
            QueryPerformanceCounter((LARGE_INTEGER*)&_i64PTimerStart);
            Resolution = (1.0f) / (float)Frequency;
            _i64PTimerElapsed = _i64PTimerStart;
        }

        else{
            usingPTimer = false;

            MTimerStart = GetTime();    //record the time the program started
            MTimerElapsed = MTimerStart;  //initialize the elapsed time variable
            Resolution = 1.0f / 1000.0f;
            Frequency = 1000;
        }

        return usingPTimer;
    }
    void Update(void){
        MFramesElapsed++; //hey, at least one frame passed

        if (MFramesElapsed % 5 == 1) { f_RUN_Time1 = GetTime() / 1000; }
        else if (MFramesElapsed % 5 == 0)
        {
            f_RUN_Time1 = f_RUN_Time2;
            f_RUN_Time2 = GetTime() / 1000;
            timeDiff = (float)fabs(f_RUN_Time2 - f_RUN_Time1);
        }

        M_FPS = 5 / (timeDiff);



        /*m_fTime2   = GetTime( )/1000;
        m_fDiffTime= ( float )fabs( m_fTime2-m_fTime1 );
        if (m_fDiffTime > 1.0f)
        {
            m_fTime1 = m_fTime2;
            m_fFPS= m_iFramesElapsed / ( m_fDiffTime );
            m_iFramesElapsed = 0;
        }
        */
    }

    float GetTime(void) {
        __int64 i64Time;

        //usePerfTimer
        if (usingPTimer) { 
            QueryPerformanceCounter((LARGE_INTEGER*)&i64Time);
            return ((float)(i64Time - _i64PTimerStart) * Resolution) * 1000.0f; 
        }

        else { return ((float)(GetTime() - MTimerStart) * Resolution) * 1000.0f; }
    }
    float GetElapsedSeconds(unsigned long elapsedFrames = 1){
        return timeDiff;
    }
    inline float GetFPS(void){
        return M_FPS;
    }
};

#endif // _E_TIMER_H