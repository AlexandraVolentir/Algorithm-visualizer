#include <stdio.h>
#include <string.h>
#include "algo.h"
#include "design.h"
#include <graphics.h>

#define NEXT (Block[i].next_blocks[0])

extern Object Block[30];
extern int l;

DrawStruct V[30];
int V_length;

Link Links[30];
int iTotalLinks;

char Variables[30];
int  iVariableNum;

int x_out = (900*2/3)+35, y_out = 0;

void Generate(int pos, bool bReturn)
{
    char cTemp[50];

    while(1)
    {
        if(Block[pos].name == 't')
        {
            if(bReturn == true)
                GeneratePrint(Block[pos], true);
            else
                GeneratePrint(Block[pos]);
            return;
        }
        if(pos > Block[pos].next_blocks[0])
        {
            GeneratePrint(Block[pos]);
            return;
        }

        if(Block[pos].name == 'd')
        {
            int iLeft = Block[pos].next_blocks[0];
            int iRight = Block[pos].next_blocks[1];

            if(CheckBranchType(iLeft) == false || CheckBranchType(iRight) == false)
            {
                sprintf(cTemp, "label%d:", pos);
                ShowOutText(cTemp, true);
            }

            GeneratePrint(Block[pos]);

//            if(CheckBranchType(iLeft) == false)
//                printf("Trebuie GoTo.\n");
//            else
//                printf("Nu trebuie GoTo.\n");
            Generate(iLeft, true);

            ShowOutText("}", true);
            ShowOutText("else", true);
            ShowOutText("{", true);

            if(CheckBranchType(iRight) == true)
            {
                Generate(iRight, true);
                ShowOutText("}", true);
            }
            else
            {
                sprintf(cTemp, "goto label%d;", iRight);
                ShowOutText(cTemp, true);
                ShowOutText("}", true);

                sprintf(cTemp, "label%d:", iRight);
                ShowOutText(cTemp, true);
                ShowOutText("{", true);

                Generate(iRight, true);
                sprintf(cTemp, "goto label%d;", pos);
                ShowOutText(cTemp, true);
                ShowOutText("}", true);
            }

            ShowOutText("}");
            return;
        }
        else
        {
            GeneratePrint(Block[pos]);
            pos = Block[pos].next_blocks[0];
        }
    }
}

void GeneratePrint(Object O, bool bReturn)
{
    char cTemp[50];

        switch(O.name)
        {
        case 's':
            ShowOutText("#include <iostream>");
            ShowOutText("#include <stdlib.h>");
            ShowOutText("int main()");
            ShowOutText("{");
            PrintVariables();
            break;
        case 't':
            if(bReturn == true)
            {
                sprintf(cTemp, "return 0;");
                ShowOutText(cTemp, true);
            }
            else
            {
                ShowOutText("");
                sprintf(cTemp, "return 0;");
                ShowOutText("}");
                ShowOutText(cTemp, true);
            }
            break;
        case 'i':
            sprintf(cTemp, "scanf(\"%%d\", &%c);", O.IOA_name, O.IOA_name);
            ShowOutText(cTemp, true);
            break;
        case 'o':
            sprintf(cTemp, "printf(\"%c = %%d\", %c);", O.IOA_name, O.IOA_name);
            ShowOutText(cTemp, true);
            break;
        case 'c':
            sprintf(cTemp, "%c = %s;", O.IOA_name, O.A_value);
            ShowOutText(cTemp, true);
            break;
        case 'd':
            sprintf(cTemp, "if (%c %c %c)", O.decision[0], O.decision[1], O.decision[2]);
            ShowOutText(cTemp, true);
            sprintf(cTemp, "{");
            ShowOutText(cTemp, true);
            break;
        }
}

bool CheckBranchType(int index)
{
    while(1)
    {
        if(Block[index].name == 't')
            return true;
        else if(index > Block[index].next_blocks[0])
            return false;
        else
            index = Block[index].next_blocks[0];
    }
}

void FindVariables()
{
    bool bAlreadyExists = false;
    for(int i = 0; i < l; i++)
    {
        if(SearchInVector(Variables, iVariableNum, Block[i].IOA_name) == true)
            bAlreadyExists = true;

        if(!bAlreadyExists && Block[i].IOA_name != '\0')
        {
            Variables[iVariableNum++] = Block[i].IOA_name;
            bAlreadyExists = false;
        }
    }
}

void PrintVariables()
{
    FindVariables();
    char cTemp[50];
    for(int j = 0; j < iVariableNum; j++)
    {
        sprintf(cTemp, "int %c;", Variables[j]);
        ShowOutText(cTemp, true);
    }
}

bool SearchInVector(char V[], int len, char c)
{
    for(int i = 0; i < len; i++)
        if (V[i] == c)
            return true;
    return false;
}

bool ReadFromFile(const char * file_name)
{
    FILE * fp = fopen(file_name, "r");
    if (fp == NULL)
        return false;
    int iBlocks, i = 0;
    char cBlockType;
    fscanf(fp, "%d", & iBlocks);

    while (fscanf(fp, " %c", & cBlockType) != EOF) //That space it's very important to bypass new line reading as a char
    {
        Object TEMP;
        TEMP.name = cBlockType;

        if((i == 0 && TEMP.name != 's') || (i != 0 && TEMP.name == 's') || (i != iBlocks-1 && TEMP.name == 't') || (i == iBlocks-1 && TEMP.name != 't'))
        {
            printf("ERROR! Something wrong with variabile %c (position %d)\n", TEMP.name, i);
            return false;
        }
        if(TEMP.next_blocks[0] > iBlocks || TEMP.next_blocks[1] > iBlocks)
        {
            printf("ERROR! Something wrong with links between blocks (look at block %d)\n");
            return false;
        }

        switch (TEMP.name)
        {
        case 's':
            fscanf(fp, "%d %d %d %d", & TEMP.top_left, & TEMP.top_right, & TEMP.bottom_left, & TEMP.bottom_right);
            fscanf(fp, "%d", & TEMP.next_blocks[0]);
            Block[l++] = TEMP;
            break;
        case 't':
            fscanf(fp, "%d %d %d %d", & TEMP.top_left, & TEMP.top_right, & TEMP.bottom_left, & TEMP.bottom_right);
            Block[l++] = TEMP;
            break;
        case 'i':
            fscanf(fp, "%d %d %d %d", & TEMP.top_left, & TEMP.top_right, & TEMP.bottom_left, & TEMP.bottom_right);
            fscanf(fp, " %c", & TEMP.IOA_name);
            fscanf(fp, "%d", & TEMP.next_blocks[0]);
            Block[l++] = TEMP;
            break;
        case 'o':
            fscanf(fp, "%d %d %d %d", & TEMP.top_left, & TEMP.top_right, & TEMP.bottom_left, & TEMP.bottom_right);
            fscanf(fp, " %s", & TEMP.IOA_name);
            fscanf(fp, "%d", & TEMP.next_blocks[0]);
            Block[l++] = TEMP;
            break;
        case 'd':
            fscanf(fp, "%d %d %d %d", & TEMP.top_left, & TEMP.top_right, & TEMP.bottom_left, & TEMP.bottom_right);
            fscanf(fp, " %c", & TEMP.decision[1]); //!!! am schimbat aici
            fscanf(fp, " %c", & TEMP.decision[0]);
            fscanf(fp, " %c", & TEMP.decision[2]);
            fscanf(fp, "%d %d", & TEMP.next_blocks[0], & TEMP.next_blocks[1]);
            Block[l++] = TEMP;
            break;
        case 'c':
            fscanf(fp, "%d %d %d %d", & TEMP.top_left, & TEMP.top_right, & TEMP.bottom_left, & TEMP.bottom_right);
            fscanf(fp, " %c", & TEMP.IOA_name);
            fscanf(fp, "%s", & TEMP.A_value);
            fscanf(fp, "%d", & TEMP.next_blocks[0]);
            Block[l++] = TEMP;
            break;
        default:
            printf("Unknown value %c !\n", cBlockType);
            return false;
        }
//        printf("i = %d\nTEMP.name: %c\n TEMP.top_left: %d\n TEMP.bottom_left: %d\n TEMP.top_right: %d\n TEMP.bottom_right: %d\n TEMP.next_blocks[0]: %d\n TEMP.next_blocks[1]: %d\n TEMP.IOA_name: %c\n TEMP.A_value: %s\n TEMP.decision[0]: %c\n TEMP.decision[1]: %c\n TEMP.decision[2]: %c\n\n", i, TEMP.name, TEMP.top_left, TEMP.bottom_left, TEMP.top_right, TEMP.bottom_right, TEMP.next_blocks[0], TEMP.next_blocks[1], TEMP.IOA_name, TEMP.A_value, TEMP.decision[0], TEMP.decision[1], TEMP.decision[2]);
        i++;
    }

    if (iBlocks != i) {
        printf("ERROR! Expected %d blocks and received %d.\n", iBlocks, i);
        return false;
    }

    fclose(fp);

    return true;
}

int  GetXCirclePosition(Object O, int param)
{
    if(param == 0) //Center
        return (O.bottom_left + O.top_left) / 2;
    else if (param == 1) //Left
        return O.top_left;
    else if (param == 2) //Right
        return O.bottom_left;
    else
        printf("ERROR! Parameter %d has a wrong value!\n", param);
}

int  GetYCirclePosition(Object O, int param)
{
    if (param == 0) //s, i, o, c
        return O.bottom_right;
    else if (param == 1) //t, i, o, c
        return O.top_right;
    else if (param == 2)
        return O.top_right;
    else if (param == 3)
        return O.bottom_right;
    else
        printf("ERROR! Parameter %d has a wrong value!\n", param);
}

void AssignSmallCircles()
{
    setcolor(LIGHTMAGENTA);

    for (int i = 0; i < l; i++)
    {
        switch(Block[i].name)
        {
            case 's':
                V[i].x1 = GetXCirclePosition(Block[i]);
                V[i].y1 = GetYCirclePosition(Block[i]);
                circle(V[i].x1, V[i].y1, 5);
                break;
            case 't':
                V[i].x1 = GetXCirclePosition(Block[i]);
                V[i].y1 = GetYCirclePosition(Block[i], 1);
                circle(V[i].x1, V[i].y1, 5);
                break;
            case 'i':
            case 'o':
            case 'c':
                V[i].x1 = GetXCirclePosition(Block[i]);
                V[i].y1 = GetYCirclePosition(Block[i], 1);
                V[i].x2 = GetXCirclePosition(Block[i]);
                V[i].y2 = GetYCirclePosition(Block[i]);
                circle(V[i].x1, V[i].y1, 5);
                circle(V[i].x2, V[i].y2, 5);
                break;
            case 'd':
                V[i].x1 = GetXCirclePosition(Block[i]);
                V[i].x2 = GetXCirclePosition(Block[i], 1);
                V[i].x3 = GetXCirclePosition(Block[i], 2);
                V[i].y1 = GetYCirclePosition(Block[i], 2);
                V[i].y2 = GetYCirclePosition(Block[i], 3);
                V[i].y3 = GetYCirclePosition(Block[i], 3);
                circle(V[i].x1, V[i].y1, 5);
                circle(V[i].x2, V[i].y2, 5);
                circle(V[i].x3, V[i].y3, 5);
                break;
        }
    }
}

void DrawLine(int x1, int y1, int x2, int y2, int color)
{
    int y_center = (y1+y2)/2;
    setcolor(color);
    line(x1,y1,x1,y_center);
    line(x1,y_center,x2,y_center);
    line(x2,y_center,x2,y2);
}

void DrawLinksBetweenThem(int color)
{
    bool bLeftRight = true;

    for(int k = 0; k < iTotalLinks; k++)
    {
            int i = Links[k].first;
            int j = Links[k].second;
            if(Block[i].name == 's')
            {
                DrawLine(V[i].x1, V[i].y1, V[j].x1, V[j].y1, color);
            }
            else if(Block[i].name == 'i' || Block[i].name == 'o' || Block[i].name == 'c')
            {
                DrawLine(V[i].x2, V[i].y2, V[j].x1, V[j].y1, color);
            }
            else if(Block[i].name == 'd')
            {
                if(bLeftRight == true)
                    DrawLine(V[i].x2, V[i].y2, V[j].x1, V[j].y1, color);
                else
                    DrawLine(V[i].x3, V[i].y3, V[j].x1, V[j].y1, color);
                bLeftRight = !bLeftRight;
            }
    }
}

void MakeVectorOfLinks(int pos)
{
    while(1)
    {
        if(Block[pos].name == 't')
            return;
        if(pos > Block[pos].next_blocks[0])
        {
            Links[iTotalLinks].first = pos;
            Links[iTotalLinks].second = Block[pos].next_blocks[0];
            iTotalLinks++;
            return;
        }
        if(Block[pos].name == 'd')
        {
            Links[iTotalLinks].first = pos;
            Links[iTotalLinks].second = Block[pos].next_blocks[0];
            iTotalLinks++;
            Links[iTotalLinks].first = pos;
            Links[iTotalLinks].second = Block[pos].next_blocks[1];
            iTotalLinks++;

            MakeVectorOfLinks(Block[pos].next_blocks[0]);
            MakeVectorOfLinks(Block[pos].next_blocks[1]);
            return;
        }
        if (CheckDuplicatePair(pos, Block[pos].next_blocks[0]) == false)
        {
            Links[iTotalLinks].first = pos;
            Links[iTotalLinks].second = Block[pos].next_blocks[0];
            pos = Block[pos].next_blocks[0];
            iTotalLinks++;
        }
        else
            pos = Block[pos].next_blocks[0];
    }
}

bool CheckDuplicatePair(int a, int b)
{
    for(int i = 0; i < iTotalLinks; i++)
        if(Links[i].first == a && Links[i].second == b)
            return true;
    return false;
}

void ShowOutText(char* text, bool bInsertTab)
{
    if (bInsertTab == true)
        AddNewTab();

    if(strcmp(text, "") != 0)
        outtextxy(x_out+20, y_out, text);
    AddNewLine();

    if (bInsertTab == true)
        DelNewTab();
}

void AddNewLine()
{
    y_out += 15;
}

void AddNewTab()
{
    x_out += 20;
}

void DelNewTab()
{
    x_out -= 20;
}

void Eval()
{
    int i = Links[0].first;
    while(Block[i].name != 't')
    {
        if (Block[i].name == 'i')
        {
            EvalReadVariable(Block[i]);
            i = Links[i].second;
        }
        else if (Block[i].name == 'o')
        {
            EvalPrintVariable(Block[i]);
            i = Links[i].second;
        }
        else if (Block[i].name == 'c')
        {
            EvalAssignVariable(Block[i]);
            i = Links[i].second;
        }
        else if (Block[i].name == 'd')
            i = EvalCheckCondition(Block[i], i);
        else
            i = Links[i].second; //start doing nothing, just go further
    }
    for(int i = 0; i<l;i++)
        printf("%d -> eval_val = %d\n", i, Block[i].eval_val);
}

void EvalReadVariable(Object& O)
{
    printf("Enter value for variable %c: ", O.IOA_name);
    scanf("%d", &O.eval_val);
}

void EvalPrintVariable(Object O)
{
    printf("%c = %d\n", O.IOA_name, O.eval_val);
}

void EvalAssignVariable(Object& O)
{
    O.eval_val = ExtractValueBasedOnVariableName(O.A_value[0]);
}

int EvalCheckCondition(Object O, int index)
{
    int iLeftValue = O.decision[0], iRightValue = O.decision[2];

    int iLeftSide = Block[index].next_blocks[0];
    int iRightSide = Block[index].next_blocks[1];
    switch(O.decision[1])
    {
    case '>':
        if (iLeftValue > iRightValue)
            return iLeftSide;
        else
            return iRightSide;
    case '<':
        if (iLeftValue < iRightValue)
            return iRightSide;
        else
            return iLeftSide;
    case '=':
        if (iLeftValue == iRightValue)
            return iLeftSide;
        else
            return iRightSide;
    default:
        printf("ERROR! I don't know how to deal with %c\n", O.decision[1]);
        return l-1; //stop
    }
}

int ExtractValueBasedOnVariableName(char var)
{
    for(int i = 0; i < l; i++)
        if(var == Block[i].IOA_name && Block[i].name == 'i' || Block[i].name == 'c')
            return Block[i].eval_val;

    return 0;
}
