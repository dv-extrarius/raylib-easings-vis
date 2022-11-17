/*******************************************************************************************
* Reasings Visualization.
* Copyright 2022 James R. Byerly. No Rights Reserved.
* 
* You may use this under the terms of the CC0 1.0 Universal (CC0 1.0) Public Domain Dedication,
* which is available at https://creativecommons.org/publicdomain/zero/1.0/
**********************************************************************************************/

#include "raylib.h"
#include "reasings.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

const int ScreenWidth = 800;
const int ScreenHeight = 600;

const int GraphOffsetX = 50;
const int GraphOffsetY = 50;
const int GraphWidth = 500;
const int GraphHeight = 500;

int EasingFunction1 = 0;
int EasingFunction2 = 20;

const float SitStillTime = 0.25f;
const float TimeSpan = 3.0f;
float CurrentTime = 0.0;

const char *const EasingFunctionNames[] = {
    "EaseLinearNone",
    "EaseLinearIn",
    "EaseLinearOut",
    "EaseLinearInOut",
    "EaseSineIn",
    "EaseSineOut",
    "EaseSineInOut",
    "EaseCircIn",
    "EaseCircOut",
    "EaseCircInOut",
    "EaseQuadIn",
    "EaseQuadOut",
    "EaseQuadInOut",
    "EaseExpoIn",
    "EaseExpoOut",
    "EaseExpoInOut",
    "EaseBackIn",
    "EaseBackOut",
    "EaseBackInOut",
    "EaseBounceIn",
    "EaseBounceOut",
    "EaseBounceInOut",
    "EaseElasticIn",
    "EaseElasticOut",
    "EaseElasticInOut",
};

//Call an easing function based on an index of which function
float CalculateEasing(unsigned int function, float time, float startValue, float deltaValue, float duration)
{
    switch(function % 25) //Wrap function 25 back to 0
    {
    /////////////////////////////////////////////////////////////
    //EaseLinear*
    case 0:
        return EaseLinearNone(time, startValue, deltaValue, duration);
    case 1:
        return EaseLinearIn(time, startValue, deltaValue, duration);
    case 2:
        return EaseLinearOut(time, startValue, deltaValue, duration);
    case 3:
        return EaseLinearInOut(time, startValue, deltaValue, duration);

    /////////////////////////////////////////////////////////////
    //EaseSine*
    case 4:
        return EaseSineIn(time, startValue, deltaValue, duration);
    case 5:
        return EaseSineOut(time, startValue, deltaValue, duration);
    case 6:
        return EaseSineInOut(time, startValue, deltaValue, duration);

    /////////////////////////////////////////////////////////////
    //EaseCirc*
    case 7:
        return EaseCircIn(time, startValue, deltaValue, duration);
    case 8:
        return EaseCircOut(time, startValue, deltaValue, duration);
    case 9:
        return EaseCircInOut(time, startValue, deltaValue, duration);

    /////////////////////////////////////////////////////////////
    //EaseQuad*
    case 10:
        return EaseQuadIn(time, startValue, deltaValue, duration);
    case 11:
        return EaseQuadOut(time, startValue, deltaValue, duration);
    case 12:
        return EaseQuadInOut(time, startValue, deltaValue, duration);

    /////////////////////////////////////////////////////////////
    //EaseExpo*
    case 13:
        return EaseExpoIn(time, startValue, deltaValue, duration);
    case 14:
        return EaseExpoOut(time, startValue, deltaValue, duration);
    case 15:
        return EaseExpoInOut(time, startValue, deltaValue, duration);

    /////////////////////////////////////////////////////////////
    //EaseBack*
    case 16:
        return EaseBackIn(time, startValue, deltaValue, duration);
    case 17:
        return EaseBackOut(time, startValue, deltaValue, duration);
    case 18:
        return EaseBackInOut(time, startValue, deltaValue, duration);

    /////////////////////////////////////////////////////////////
    //EaseBounce*
    case 19:
        return EaseBounceIn(time, startValue, deltaValue, duration);
    case 20:
        return EaseBounceOut(time, startValue, deltaValue, duration);
    case 21:
        return EaseBounceInOut(time, startValue, deltaValue, duration);

    /////////////////////////////////////////////////////////////
    //EaseElastic*
    case 22:
        return EaseElasticIn(time, startValue, deltaValue, duration);
    case 23:
        return EaseElasticOut(time, startValue, deltaValue, duration);
    case 24:
        return EaseElasticInOut(time, startValue, deltaValue, duration);

    /////////////////////////////////////////////////////////////
    //Can't actually reach here, but it silences a warning
    default:
        return 0.0f;
    }
}

void DoExampleLoop(void)
{
    /////////////////////////////////////////////////////////////////
    //Update Time and wrap to the range 0..(TimeSpan + 2 * SitTime)
    CurrentTime += GetFrameTime();
    CurrentTime = fmodf(CurrentTime, TimeSpan + 2.0f * SitStillTime);
    /////////////////////////////////////////////////////////////////


    /////////////////////////////////////////////////////////////////
    //Calculate a fake time that sits still for a bit at the start and end
    float FakeCurrentTime = CurrentTime;
    if(FakeCurrentTime < SitStillTime)
    {
        FakeCurrentTime = 0.0f;
    }
    else if(FakeCurrentTime < SitStillTime + TimeSpan)
    {
        FakeCurrentTime -= SitStillTime;
    }
    else
    {
        FakeCurrentTime = TimeSpan;
    }
    /////////////////////////////////////////////////////////////////


    /////////////////////////////////////////////////////////////////
    BeginDrawing();
    ClearBackground(BLACK);
    /////////////////////////////////////////////////////////////////


    /////////////////////////////////////////////////////////////////
    //Fill in the graph
    //Easing from GraphOffsetY to GraphOffsetY+GraphHeight, current time is 0 out of GraphWidth
    float PrevPosition1 = CalculateEasing(EasingFunction1, 0.0f, GraphOffsetY, GraphHeight, GraphWidth);
    float PrevPosition2 = CalculateEasing(EasingFunction2, 0.0f, GraphOffsetY, GraphHeight, GraphWidth);
    for(int ii = 0; ii < GraphWidth; ii += 5)
    {
        //Easing from GraphOffsetY to GraphOffsetY+GraphHeight, current time is ii out of GraphWidth
        float NextPosition1 = CalculateEasing(EasingFunction1, ii + 5, GraphOffsetY, GraphHeight, GraphWidth);
        float NextPosition2 = CalculateEasing(EasingFunction2, ii + 5, GraphOffsetY, GraphHeight, GraphWidth);

        DrawLine(ii + GraphOffsetX, PrevPosition1, ii + 5 + GraphOffsetX, NextPosition1, RED);
        DrawLine(ii + GraphOffsetX, PrevPosition2, ii + 5 + GraphOffsetX, NextPosition2, BLUE);

        PrevPosition1 = NextPosition1;
        PrevPosition2 = NextPosition2;
    }
    /////////////////////////////////////////////////////////////////


    /////////////////////////////////////////////////////////////////
    //Draw instructions
    DrawText("Press 1 to change red easing, 2 to change blue easing", 0, 0, 20, WHITE);
    /////////////////////////////////////////////////////////////////


    /////////////////////////////////////////////////////////////////
    //Draw a graph box with labels
    const int TextTimeWidth = MeasureText("Time", 20);
    DrawText("Time", GraphOffsetX + (GraphWidth - TextTimeWidth) / 2, GraphOffsetY - 20, 20, WHITE);
    const int TextPositionWidth = MeasureText("Position", 20);
    DrawTextPro(GetFontDefault(), "Position",
        (Vector2){GraphOffsetX - 20.0f, GraphOffsetY + (GraphHeight + TextPositionWidth) / 2.0f},
        (Vector2){0.0f, 00.0f},
        -90.0f, 20.0f, 2.0f, WHITE);
    DrawRectangleLines(GraphOffsetX, GraphOffsetY, GraphWidth, GraphHeight, WHITE);
    /////////////////////////////////////////////////////////////////


    /////////////////////////////////////////////////////////////////
    //Draw Legend
    DrawText(EasingFunctionNames[EasingFunction1], GraphOffsetX, GraphOffsetY + GraphHeight + 0, 20, RED);
    DrawText(EasingFunctionNames[EasingFunction2], GraphOffsetX, GraphOffsetY + GraphHeight + 20, 20, BLUE);
    /////////////////////////////////////////////////////////////////


    /////////////////////////////////////////////////////////////////
    //Draw time line on graph
    int TimeX = GraphOffsetX + GraphWidth * (FakeCurrentTime / TimeSpan);
    DrawLine(TimeX, GraphOffsetY, TimeX, GraphOffsetY + GraphHeight, GREEN);
    /////////////////////////////////////////////////////////////////


    /////////////////////////////////////////////////////////////////
    //Draw Boxes, easing from GraphOffsetY to GraphOffsetY+GraphHeight over TimeSpan seconds
    float Box1Y = CalculateEasing(EasingFunction1, FakeCurrentTime, GraphOffsetY, GraphHeight, TimeSpan);
    float Box2Y = CalculateEasing(EasingFunction2, FakeCurrentTime, GraphOffsetY, GraphHeight, TimeSpan);

    DrawRectangle(GraphOffsetX + GraphWidth + 50, Box1Y - 25, 50, 50, RED);
    DrawRectangle(GraphOffsetX + GraphWidth + 150, Box2Y - 25, 50, 50, BLUE);

    //Draw outlines at start and end position
    DrawRectangleLines(GraphOffsetX + GraphWidth + 50, GraphOffsetY - 25, 50, 50, RED);
    DrawRectangleLines(GraphOffsetX + GraphWidth + 50, GraphOffsetY + GraphHeight - 25, 50, 50, RED);
    DrawRectangleLines(GraphOffsetX + GraphWidth + 150, GraphOffsetY - 25, 50, 50, BLUE);
    DrawRectangleLines(GraphOffsetX + GraphWidth + 150, GraphOffsetY + GraphHeight - 25, 50, 50, BLUE);
    /////////////////////////////////////////////////////////////////


    /////////////////////////////////////////////////////////////////
    //Update Functions based on input
    if(IsKeyPressed(KEY_ONE))
    {
        EasingFunction1 = (EasingFunction1 + 1) % 25;
    }
    if(IsKeyPressed(KEY_TWO))
    {
        EasingFunction2 = (EasingFunction2 + 1) % 25;
    }
    /////////////////////////////////////////////////////////////////
    

    EndDrawing();
}

int main()
{
    InitWindow(ScreenWidth, ScreenHeight, "Easings Visualization");

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(DoExampleLoop, 60, 1);
#else
    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        DoExampleLoop();
    }
    CloseWindow();
#endif
}