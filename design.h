#ifndef DESIGN_H_INCLUDED
#define DESIGN_H_INCLUDED

void InitializeWindow();
void BlockWithPattern();
void TitluMeniu();
void DecizieDesenareBloc();


void Read_start_stop(int i);
void Window();
void Read_input_output(struct Object TEMP, int color);
void Read_decizie(struct Object TEMP, int color);
void Read_atribuire(struct Object TEMP, int color);
void Read_Blocks();
void InitWindow();
int MID_X(Object TEMP);
int MID_Y(Object TEMP);
void Move_The_Blocks();


#endif 
