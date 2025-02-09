
#include "fontmanager.h"

void load_font_manager(FontManager* font_manager,RenderingContext* context, char path[]) {
    font_manager->cur_font = load_sprite(context, path);
}
void draw_letter(RenderingContext* context,FontSurface* written_surface,SDL_Texture* font_texture,int offset_x,int global_offset,int y_adder){
    SDL_Rect srcRect;
    srcRect.x = +offset_x * FONT_WIDTH;
    srcRect.y = 0;
    srcRect.w = FONT_WIDTH;
    srcRect.h = FONT_HEIGHT;

    SDL_Rect dstRect;
    dstRect.x = global_offset;
    dstRect.y = written_surface->pos.y+y_adder;
    dstRect.w = FONT_WIDTH;
    dstRect.h = FONT_HEIGHT;

    SDL_RenderCopy(context->renderer, font_texture, &srcRect, &dstRect);
}
void draw_letters(RenderingContext* context,FontManager* font_manager,FontSurface* written_surface){
    int letter_index=0;
    int global_offset=written_surface->pos.x;
    int y_adder=0;
    int e_mod=0;
    while (written_surface->data[letter_index]!='\0')
    {
        char cur_letter=written_surface->data[letter_index];
        
        int offset_x=-1;
        if (cur_letter >= 'a' && cur_letter <= 'z') {
            offset_x = cur_letter - 'a';
        } else {
            switch(cur_letter){
                case '0':
                    offset_x=26;
                    break;
                case '1':
                    offset_x=27;
                    break;
                case '2':
                    offset_x=28;
                    break;
                case '3':
                    offset_x=29;
                    break;
                case '4':
                    offset_x=30;
                    break;
                case '5':
                    offset_x=31;
                    break;
                case '6':
                    offset_x=32;
                    break;
                case '7':
                    offset_x=33;
                    break;
                case '8':
                    offset_x=34;
                    break;
                case '9':
                    offset_x=35;
                    break;
                case ':':
                    offset_x = 36;
                    break;
                case '!':
                    offset_x = 37;
                    break;
                case '\'':
                    global_offset-=16;
                    offset_x = 38;
                    e_mod=14;
                    break;
				case '\n':
                    y_adder+=30;
					global_offset=written_surface->pos.x;
					break;
            }
        }
        if(cur_letter!=-1){
            draw_letter(context,written_surface,font_manager->cur_font,offset_x,global_offset,y_adder-e_mod);
            if(e_mod!=0){
                e_mod=0;
            }
        }
        global_offset+=16;
        letter_index++;
    }
}
