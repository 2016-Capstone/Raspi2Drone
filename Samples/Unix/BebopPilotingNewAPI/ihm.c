/*
    Copyright (C) 2014 Parrot SA

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions
    are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the 
      distribution.
    * Neither the name of Parrot nor the names
      of its contributors may be used to endorse or promote products
      derived from this software without specific prior written
      permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
    COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
    OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
    AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
    OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
    OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
    SUCH DAMAGE.
*/
/**
 * @file ihm.c
 * @brief This file contains sources about ncurses IHM used by arsdk example "JumpingSumoPiloting"
 * @date 15/01/2015
 */

/*****************************************
 *
 *             include file :
 *
 *****************************************/

#include <stdlib.h>
#include <curses.h> //터미널 화면을 제어하는  시스템 라이브러리
#include <string.h>

#include <libARSAL/ARSAL.h>

#include "ihm.h"

/*****************************************
 *
 *             define : 터미널 상에 정보들이 출력될 포지션 설정
 *
 *****************************************/

#define HEADER_X 0
#define HEADER_Y 0

#define INFO_X 0
#define INFO_Y 11

#define BATTERY_X 0
#define BATTERY_Y 9

#define VALUE_X 0
#define VALUE_Y 3

/*****************************************
 *
 *             private header:
 *
 ****************************************/
void *IHM_InputProcessing(void *data);  // 키보드 입력을 처리하는 함수

/*****************************************
 *
 *             implementation :
 *
 *****************************************/

/**
 * @brief : IHM_t 초기화, curses.h를 이용한 터미널 초기설정, 키보드 입력 스레드 실행, 파라미터나 메모리 할당 실패시 IHM_Delete 호출
 * @arg : [from ihm.h] This is a function pointer, args is enum and customData
 * */
IHM_t *IHM_New (IHM_onInputEvent_t onInputEventCallback)
{
    int failed = 0;         // 초기화 실패시 1로 설정되어 모든 루틴 건너뛰게 됨
    IHM_t *newIHM = NULL;   // 설정대상
    
    // check parameters
    if (onInputEventCallback == NULL)   // onInputEventCallback = enum + customData
    {
        failed = 1;     // 모든 Routine을 건너뛰고 IHM_Delete 실행
    }
    
    if (!failed)
    {
        //  Initialize IHM
        newIHM = malloc(sizeof(IHM_t));     // 메모리 할당
        if (newIHM != NULL)                 // 할당 성공
        {
            //  Initialize ncurses
            newIHM->mainWindow = initscr();     // [from curses.h] curses mode로 전환
            newIHM->inputThread = NULL;         // NULL로 초기화한 후  ARSAL_Thread_Create의 파라미터로 입력되어 할당
            newIHM->run = 1;                    // IHM_InputProcessing의 Loop를 제어하는 플래그 1:true
            newIHM->onInputEventCallback = onInputEventCallback;    // enum+customData
            newIHM->customData = NULL;
        }
        else                                // 할당 실패
        {
            failed = 1;
        }
    }
    
    if (!failed)
    {
        raw();                  // Line buffering disabled
        keypad(stdscr, TRUE);   // [from curses.h] Enables the keypad of the user's terminal
        noecho();               // Don't echo() while we do getch
        timeout(100);           // Start COMMAND, and kill it if still runnning after Duration
        
        refresh();              // [from curses.h]
    }
    
    if (!failed)
    {
        // start input thread
        // @arg1 : ARSAL_Thread_t
        // @arg2 : keyboard input processing function with loop
        // @arg3 : IHM_t
        // @return : 0 == sucess
        if(ARSAL_Thread_Create(&(newIHM->inputThread), IHM_InputProcessing, newIHM) != 0)
        {
            failed = 1;
        }
    }
    
    if (failed)
    {
        IHM_Delete (&newIHM);
    }

    return  newIHM;
}

/**
 * @brief : IHM_t 를 제거
 * */
void IHM_Delete (IHM_t **ihm)
{
    //  Clean up

    if (ihm != NULL)
    {
        if ((*ihm) != NULL)
        {
            (*ihm)->run = 0;
            
            if ((*ihm)->inputThread != NULL)    // 키보드 입력 스레드가 동작하고 있다면..
            {
                // 스레드 파괴
                ARSAL_Thread_Join((*ihm)->inputThread, NULL);
                ARSAL_Thread_Destroy(&((*ihm)->inputThread));
                (*ihm)->inputThread = NULL;
            }
            
            delwin((*ihm)->mainWindow); // initscr()의 반대
            (*ihm)->mainWindow = NULL;
            endwin();                   // [from curses.h] curses mode 종료
            refresh();
            
            free (*ihm);                // 메모리 방출
            (*ihm) = NULL;
        }
    }
}

/**
 * @brief : 생성된 IHM_t에 *customData 설정
 * */
void IHM_setCustomData(IHM_t *ihm, void *customData)
{
    if (ihm != NULL)
    {
        ihm->customData = customData;
    }
}

/**
 * @brief : Loop를 돌며 키보드 입력 처리
 * @arg1 : IHM_t
 * */
void *IHM_InputProcessing(void *data)
{
    IHM_t *ihm = (IHM_t *) data;
    int key = 0;
    
    if (ihm != NULL)
    {
        // Loop start
        while (ihm->run)
        {
            key = getch();  // 사용자 입력 대기
            
            if ((key == 27) || (key =='q'))
            {
                if(ihm->onInputEventCallback != NULL)   // ihm에 함수가 붙어 있으면...
                {
                    // @brief : ihm에 설정된 함수에 본인이 가지고 있던 값들을 전달
                    // @arg1 : enum의 값, 키보드 입력
                    // @arg2 : ihm이 가지고 있던 customData
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_EXIT, ihm->customData);
                }
            }
            else if(key == KEY_UP)
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_UP, ihm->customData);
                }
            }
            else if(key == KEY_DOWN)
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_DOWN, ihm->customData);
                }
            }
            else if(key == KEY_LEFT)
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_LEFT, ihm->customData);
                }
            }
            else if(key == KEY_RIGHT)
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_RIGHT, ihm->customData);
                }
            }
            else if(key == 'e')
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_EMERGENCY, ihm->customData);
                }
            }
            else if(key == 't')
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_TAKEOFF, ihm->customData);
                }
            }
            else if(key == ' ')
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_LAND, ihm->customData);
                }
            }
            else if(key == 'r')
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_FORWARD, ihm->customData);
                }
            }
            else if(key == 'f')
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_BACK, ihm->customData);
                }
            }
            else if(key == 'd')
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_ROLL_LEFT, ihm->customData);
                }
            }
            else if(key == 'g')
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_ROLL_RIGHT, ihm->customData);
                }
            }
            else
            {
                if(ihm->onInputEventCallback != NULL)
                {
                    ihm->onInputEventCallback (IHM_INPUT_EVENT_NONE, ihm->customData);
                }
            }
            
            usleep(10); // sleep every 10 micro sec
        }// End of Loop
    }
    
    return NULL;
}

/**
 * @brief : print label(header) on the terminal
 * */
void IHM_PrintHeader(IHM_t *ihm, char *headerStr)
{
    if (ihm != NULL)
    {
        move(HEADER_Y, 0);   // move to begining of line
        clrtoeol();          // clear line
        mvprintw(HEADER_Y, HEADER_X, headerStr);    // just print at that point
    }
}

/**
 * @brief : print payload after the header
 * */
void IHM_PrintInfo(IHM_t *ihm, char *infoStr)
{
    if (ihm != NULL)
    {
        move(INFO_Y, 0);    // move to begining of line
        clrtoeol();         // clear line
        mvprintw(INFO_Y, INFO_X, infoStr);
    }
}


/**
 * @brief : print amount of remaining battery
 * */
void IHM_PrintBattery(IHM_t *ihm, uint8_t percent)
{
    if (ihm != NULL)
    {
        move(BATTERY_Y, 0);     // move to begining of line
        clrtoeol();             // clear line
        mvprintw(BATTERY_Y, BATTERY_X, "Battery: %d", percent); // just print formatted string
    }
}


/**
 * JUMPSNACK's CODE :D
 */
void IHM_PrintValue(IHM_t *ihm, char* TAG, uint8_t value)
{
	if(ihm != NULL)
	{
		move(VALUE_Y, 0);
		clrtoeol();
		mvprintw(VALUE_Y, VALUE_X, "[VALUE] %s :: %d", TAG, value);
	}
}
