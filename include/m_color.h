#ifndef M_COLOR_H
#define M_COLOR_H

void filter_handler(char * option/*,pixels matrice struct*/);
void negative_filter(/*pixels matrice struct*/);
void black_and_white_filter(/*pixels matrice struct*/);
void grey_filter(int level/*,pixels matrice struct*/);
void replace_color(/*pixel struct*//*,pixel matrice struct,*/int margin);
void color_zone(int x1,int x2,int y1,int y2/*,pixel struct , pixel matrice struct*/); 

#endif 