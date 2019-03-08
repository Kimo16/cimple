#ifndef M_COLOR_H
#define M_COLOR_H


short negative_filter(SDL_Texture* img);
short black_and_white_filter(SDL_Texture* img);
short grey_filter(SDL_Texture* img,int level);
short replace_color(SDL_Texture* img,SDL_Color origin_color,SDL_Color target_color,int margin);
short color_zone(SDL_Texture* img ,SDL_Color color,int x1,int x2,int y1,int y2); 

#endif 