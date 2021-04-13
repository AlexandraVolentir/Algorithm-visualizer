#ifndef ALGO_H_INCLUDED
#define ALGO_H_INCLUDED

//Start, Stop, Input, Output, Decision, Assignment
struct Object {
	char	name; //s,t,i,o,d,c
	int     top_left, bottom_left, top_right, bottom_right;
	int     next_blocks[2] = {}; //If-else has 2 conditions, if it's 0 when saving then ignore it
	char	IOA_name; //Used only to keep variable names for Input, Output and Assignment
	char	A_value[100] = {}; //what we want to assign
	char	decision[3] = {}; //Sign, Left-value, Right-value
	int     eval_val;
    Object() : top_left(0), top_right(0), bottom_left(0), bottom_right(0), IOA_name('\0'), eval_val(0)  {}
};

struct DrawStruct {
    int x1, y1, x2, y2, x3, y3, index;
    bool bDrawn;
    DrawStruct() : x1(0), y1(0), x2(0), y2(0), x3(0), y3(0), index(0) {}
};

struct Link {
    int first, second;
    Link() : first(0), second(0) {}
};

bool LoadSchemeFile();
void Generate(int pos, bool bReturn = false);
void GeneratePrint(Object O, bool bReturn = false);
void FindVariables();
void PrintVariables();
bool SearchInVector(char V[], int len, char c);
bool CheckDuplicatePair(int a, int b);
bool CheckBranchType(int index);

bool ReadFromFile(const char* file_name);
int  GetXCirclePosition(Object O, int param = 0);
int  GetYCirclePosition(Object O, int param = 0);
void AssignSmallCircles();
void DrawLinksBetweenThem(int color);
void DrawLine(int x1, int y1, int x2, int y2, int color);
void MakeVectorOfLinks(int pos);

void ShowOutText(char* text, bool bInsertTab = false);
void AddNewLine();
void AddNewTab();
void DelNewTab();

void Eval();
void EvalReadVariable(Object& O);
void EvalPrintVariable(Object O);
void EvalAssignVariable(Object& O);
int  EvalCheckCondition(Object O, int index);
int  ExtractValueBasedOnVariableName(char var);

#endif // HAZIFLORINMARIAN_H_INCLUDED

