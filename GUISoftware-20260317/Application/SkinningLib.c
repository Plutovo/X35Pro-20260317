//#include "WM.h"
#include "GUI.H"
#include "Skinning_Lib.H"
/*********************************************************************************************************/
int Arc(int xL, int yT, int xR, int yB, int r1, int r2, int octant)
{
// this is using a variant of the Midpoint (Bresenham's) Algorithm
    typedef enum
    {
        BEGIN,
        QUAD11, BARRIGHT1,
        QUAD12, BARRIGHT2,
        QUAD21, BARLEFT1,
        QUAD22, BARLEFT2,
        QUAD31, BARTOP1,
        QUAD32, BARTOP2,
        QUAD41, BARBOTTOM1,
        QUAD42, BARBOTTOM2,
        CHECK,
    } OCTANTARC_STATES;
    int temp;
    int y1Limit, y2Limit;
    int x1, x2, y1, y2;
    int err1, err2;
    int x1Cur, y1Cur, y1New;
    int x2Cur, y2Cur, y2New;
    OCTANTARC_STATES state = BEGIN;
    while(1)
    {
        switch(state)
        {
        case BEGIN:
            temp = SIN45*r1;
            y1Limit  = temp / 0xFFFF;
            temp = SIN45*r2;
            y2Limit  = temp / 0xFFFF;
            temp = ONEP25 - (r1<<16);
            err1  = temp / 0xFFFF;
            temp = ONEP25 - (r2<<16);
            err2  = temp / 0xFFFF;
            x1 = r1;
            x2 = r2;
            y1 = 0;
            y2 = 0;
            x1Cur = x1;
            y1Cur = y1;
            y1New = y1;
            x2Cur = x2;
            y2Cur = y2;
            y2New = y2;
            state = CHECK;
        case CHECK:
arc_check_state:
            if (y2 > y2Limit)
            {
                state = BARRIGHT1;
                goto arc_draw_width_height_state;
            }
            // y1New & y2New records the last y positions
            y1New = y1;
            y2New = y2;
            if (y1 <= y1Limit)
            {
                if(err1 > 0)
                {
                    x1--;
                    err1 += 5;
                    err1 += (y1-x1)<<1;
                }
                else
                {
                    err1 += 3;
                    err1 += y1<<1;
                }
                y1++;
            }
            else
            {
                y1++;
                if (x1 < y1)
                {
                    x1 = y1;
                }
            }
            if(err2 > 0)
            {
                x2--;
                err2 += 5;
                err2 += (y2-x2)<<1;
            }
            else
            {
                err2 += 3;
                err2 += y2<<1;
            }
            y2++;
            state = QUAD11;
            break;
        case QUAD11:
            if ((x1Cur != x1) || (x2Cur != x2))
            {
                // 1st octant
                if (octant&0x01)
                {
                    GUI_FillRect(xR+y2Cur, yT-x2Cur, xR+y1New, yT-x1Cur);
                }
            }
            else
            {
                state = CHECK;
                goto arc_check_state;
            }
            state = QUAD12;
            break;
        case QUAD12:
            // 2nd octant
            if (octant&0x02)
            {
                GUI_FillRect(xR+x1Cur, yT-y1New, xR+x2Cur, yT-y2Cur);
            }
            state = QUAD21;
            break;
        case QUAD21:
            // 3rd octant
            if (octant&0x04)
            {
                GUI_FillRect(xR+x1Cur, yB+y1Cur, xR+x2Cur, yB+y2New);
            }
            state = QUAD22;
            break;
        case QUAD22:
            // 4th octant
            if (octant&0x08)
            {
                GUI_FillRect(xR+y1Cur, yB+x1Cur, xR+y2New, yB+x2Cur);
            }
            state = QUAD31;
            break;
        case QUAD31:
            // 5th octant
            if (octant&0x10)
            {
                GUI_FillRect(xL-y1New, yB+x1Cur, xL-y2Cur, yB+x2Cur);
            }
            state = QUAD32;
            break;
        case QUAD32:
            // 6th octant
            if (octant&0x20)
            {
                GUI_FillRect(xL-x2Cur, yB+y2Cur, xL-x1Cur, yB+y1New);
            }
            state = QUAD41;
            break;
        case QUAD41:
            // 7th octant
            if (octant&0x40)
            {
                GUI_FillRect(xL-x2Cur, yT-y2New, xL-x1Cur, yT-y1Cur);
            }
            state = QUAD42;
            break;
        case QUAD42:
            // 8th octant
            if (octant&0x80)
            {
                GUI_FillRect(xL-y2New, yT-x2Cur, xL-y1Cur, yT-x1Cur);
            }
            // update current values
            x1Cur = x1;
            y1Cur = y1;
            x2Cur = x2;
            y2Cur = y2;
            state = CHECK;
            break;
        case BARRIGHT1:             // draw upper right
arc_draw_width_height_state:
            if ((xR-xL) || (yB-yT))
            {
                // draw right
                if (octant&0x02)
                {
                    GUI_FillRect(xR+r1,yT,xR+r2,(yB+yT)>>1);
                }
            }
            else
            {
                state = BEGIN;
                return 1;
            }
            state = BARRIGHT2;
            break;
        case BARRIGHT2:             // draw lower right
            if (octant&0x04)
            {
                GUI_FillRect(xR+r1,((yB+yT)>>1), xR+r2, yB);
            }
            state = BARBOTTOM1;
            break;
        case BARBOTTOM1:            // draw left bottom
            // draw bottom
            if (octant&0x10)
            {
                GUI_FillRect(xL, yB+r1, ((xR+xL)>>1),yB+r2);
            }
            state = BARBOTTOM2;
            break;
        case BARBOTTOM2:            // draw right bottom
            if (octant&0x08)
            {
                GUI_FillRect(((xR+xL)>>1),yB+r1,xR,yB+r2);
            }
            state = BARTOP1;
            break;
        case BARTOP1:               // draw left top
            if(xR-xL)
            {
                // draw top
                if (octant&0x80)
                {
                    GUI_FillRect(xL, yT-r2, ((xR+xL)>>1),yT-r1);
                }
                state = BARTOP2;
            }
            else
            {
                state = BARLEFT1;    // no width go directly to height bar
            }
            break;
        case BARTOP2:               // draw right top
            if (octant&0x01)
            {
                GUI_FillRect(((xR+xL)>>1),yT-r2,xR,yT-r1);
            }
            state = BARLEFT1;
            break;
        case BARLEFT1:              // draw upper left
            if (yT-yB)
            {
                // draw left
                if (octant&0x40)
                {
                    GUI_FillRect(xL-r2,yT,xL-r1,((yB+yT)>>1));
                }
                state = BARLEFT2;
            }
            else
            {
                state = BEGIN;      // no height go back to BEGIN
                return 1;
            }
            break;
        case BARLEFT2:              // draw lower left
            if (octant&0x20)
            {
                GUI_FillRect(xL-r2,((yB+yT)>>1),xL-r1,yB);
            }
            state = BEGIN;
            return 1;
        }// end of switch
    }// end of while
}
/*******************************************************************
int Arc1(int xL, int yT, int xR, int yB, int r1, int r2, int octant)
{
}*/
/********************************************************************/
void Bevel(int x1, int y1, int x2, int y2, int rad)
{
    int  style, type, xLimit, xPos, yPos, error;
    int  temp;
    temp = SIN45*rad;
    xLimit   = temp/0xffff;
    temp = ONEP25 -(rad<<16);
    error    = temp/0xffff;
    yPos     = rad;
    style = 0;
    type =1;
    if (rad)
    {
        for (xPos=0; xPos <=xLimit; xPos++)
        {
            if(type)
            {
                GUI_DrawPixel(x2+xPos, y1-yPos);        // 1st quadrant
                GUI_DrawPixel(x2+yPos, y1-xPos);
                GUI_DrawPixel(x2+xPos, y2+yPos);        // 2nd quadrant
                GUI_DrawPixel(x2+yPos, y2+xPos);
                GUI_DrawPixel(x1-xPos, y2+yPos);        // 3rd quadrant
                GUI_DrawPixel(x1-yPos, y2+xPos);
                GUI_DrawPixel(x1-yPos, y1-xPos);        // 4th quadrant
                GUI_DrawPixel(x1-xPos, y1-yPos);
            }
            if(error > 0)
            {
                yPos--;
                error += 5+((xPos-yPos)<<1);
            }
            else
            {
                error += 3+(xPos<<1);
            }
        }
    }
    // Must use lines here since this can also be used to draw focus of round buttons
    if (x2-x1)
    {
        GUI_DrawLine(x1,y1-rad,x2,y1-rad);              // draw top
    }
    if (y2-y1)
    {
        GUI_DrawLine(x1-rad,y1,x1-rad,y2);              // draw left
    }
    if ((x2-x1) || (y2-y1))
    {
        GUI_DrawLine(x2+rad,y1,x2+rad,y2);              // draw right
        GUI_DrawLine(x1,y2+rad,x2,y2+rad);              // draw bottom
    }
}
/********************************************************************/
int FillBevel(int x1, int y1, int x2, int y2, int rad)
{
    typedef enum
    {
        BEGIN,
        CHECK,
        Q8TOQ1,
        Q7TOQ2,
        Q6TOQ3,
        Q5TOQ4,
        WAITFORDONE,
        FACE
    } FILLCIRCLE_STATES;
    int  temp;
    int  err;
    int  yLimit, xPos, yPos;
    int  xCur, yCur, yNew;
    FILLCIRCLE_STATES state = BEGIN;
    while(1)
    {
        switch(state)
        {
        case BEGIN:
            if (!rad)                                   // no radius object is a filled rectangle
            {
                state = FACE;
                break;
            }
            // compute variables
            temp = SIN45*rad;
            yLimit   = temp/0xFFFF;
            temp = ONEP25 -(rad<<16);
            err      = temp/0xFFFF;
            xPos     = rad;
            yPos = 0;
            xCur     = xPos;
            yCur = yPos;
            yNew = yPos;
            state    = CHECK;
        case CHECK:
bevel_fill_check:
            if (yPos>yLimit)
            {
                state = FACE;
                break;
            }
            // y1New records the last y position
            yNew = yPos;
            // calculate the next value of x and y
            if(err > 0)
            {
                xPos--;
                err += 5+((yPos-xPos)<<1);
            }
            else
            {
                err += 3+(yPos<<1);
            }
            yPos++;
            state = Q6TOQ3;
        case Q6TOQ3:
            if (xCur != xPos)
            {
                // 6th octant to 3rd octant
                GUI_FillRect(x1-xCur, y2+yCur, x2+xCur, y2+yNew);
                state = Q5TOQ4;
                break;
            }
            state = CHECK;
            goto bevel_fill_check;
        case Q5TOQ4:
            // 5th octant to 4th octant
            GUI_FillRect(x1-yNew, y2+xPos, x2+yNew, y2+xCur);
            state = Q8TOQ1;
            break;
        case Q8TOQ1:
            // 8th octant to 1st octant
            GUI_FillRect(x1-yNew, y1-xCur, x2+yNew, y1-xPos);
            state = Q7TOQ2;
            break;
        case Q7TOQ2:
            // 7th octant to 2nd octant
            GUI_FillRect(x1-xCur, y1-yNew, x2+xCur, y1-yCur);
            // update current values
            xCur = xPos;
            yCur = yPos;
            state = CHECK;
            break;
        case FACE:
            if ((x2-x1)||(y2-y1))
            {
                GUI_FillRect(x1-rad, y1, x2+rad, y2);
                state = WAITFORDONE;
            }
            else
            {
                state = BEGIN;
                return 1;
            }
        case WAITFORDONE:
            state = BEGIN;
            return 1;
        }// end of switch
    }// end of while
}
