
#include "scoremanager.h"

void calculate_lowest_highscore(HighScoreManager* high_score_manager) {
    if (high_score_manager->next_score == NULL) {
        high_score_manager->lowest = 0;
        return;
    }

    ScoreEntry* current = high_score_manager->next_score;
    high_score_manager->lowest = current->cur_score;

    while (current != NULL) {
        if (current->cur_score < high_score_manager->lowest) {
            high_score_manager->lowest = current->cur_score;
        }
        current = current->next_score;
    }
}

void add_to_highscores(HighScoreManager* high_score_manager,ScoreEntry* insertion_value){
    if (high_score_manager->next_score == NULL) {
        high_score_manager->next_score = insertion_value;
    } else {

        ScoreEntry* current = high_score_manager->next_score;
        while (current->next_score != NULL&&current->cur_score<MAX_SCORE_VALUE) {
            current = current->next_score;
        }

        current->next_score = insertion_value;
    }
}
char* create_top_line(int point, int diff, const char name[]){
	
	char* ret_string = (char*) malloc(54 * sizeof(char));
    if (ret_string == NULL) {
		printf("Hiba a szövegnek való memóriafoglalás közben.");
        return NULL;
    }
	char string_diff[11];
	switch(diff){
		case 0:
			sprintf(string_diff, "alap");
			break;
		case 1:
			sprintf(string_diff, "nehe'z");
			break;
		case 2:
			sprintf(string_diff, "lehetetlen");
			break;
	}
    snprintf(ret_string, 54, "%d,%s,%s", point, string_diff, name);

    return ret_string;
}
char* create_string_line(int point, int diff, const char name[]) {
    char* file_string = (char*) malloc(54 * sizeof(char));
    if (file_string == NULL) {
		printf("Hiba a szövegnek való memóriafoglalás közben.");
        return NULL;
    }

    snprintf(file_string, 54, "%d,%d,%s", point, diff, name);

    return file_string;
}

void load_top_list_screen(TopListScreen* top_list_screen,HighScoreManager* high_score_manager){
    //top_string
    if (top_list_screen->top_string != NULL){
        free(top_list_screen->top_string);
        top_list_screen->top_string = NULL;
    }

    top_list_screen->top_string = (FontSurface*)malloc(sizeof(FontSurface) + 54*TOP_SCORES_SIZE);
    if (top_list_screen->top_string == NULL) {
        printf("Hiba a top_string-nek való memória foglalás közben!\n");
        return;
    }
    top_list_screen->top_string->pos = init_vec2(80,70);

    //create_string_line(int point, int diff, const char name[])
    strcpy(top_list_screen->top_string->data, " ");
    ScoreEntry* current = high_score_manager->next_score;
    for (int i = 1; i <= TOP_SCORES_SIZE; i++) {
        if (current == NULL) {
            break;
        } else {
            char* line = create_top_line(current->cur_score, current->entry_difficulty, current->entry_name);
            if (line != NULL) {
                char val[54];
                snprintf(val, sizeof(val), "%d: %s\n", i, line);
                
                strcat(top_list_screen->top_string->data, val);
                
            } else {
                printf("Hiba a cur_score-nek való memória foglalás közben!.\n");
                break;
            }
			free(line);
            
            current = current->next_score;
        }
    }
}
void save_highscores(HighScoreManager* high_score_manager) {
    FILE *file_write = fopen(TOP_LIST_FILE, "w");
    if (file_write == NULL) {
        SDL_Log("Nem sikerült a file megnyitása: %s", TOP_LIST_FILE);
        return;
    }

    ScoreEntry* current = high_score_manager->next_score;
    while (current != NULL && current->cur_score < MAX_SCORE_VALUE) {
		char* line=create_string_line(current->cur_score, current->entry_difficulty, current->entry_name);
		if(line==NULL){
			printf("Sor mentése sikertelen!");
		} else {
			fprintf(file_write, "%s\n", line);
			free(line);
		}
        current = current->next_score;
    }
    fclose(file_write);
    printf("Top lista elmentve");
}

//TOP down végig kell menni, ha null az entry akkor stop, ha az jelen elem kisebb mint az entry akkor jelen elem=entry, entry. next=jelenelem, jelenelemet t-be ki kell menteni?
void insert_to_highscores(HighScoreManager* high_score_manager, ScoreEntry* insertion_value) {

    if (high_score_manager->next_score == NULL) {

        high_score_manager->next_score = insertion_value;
        insertion_value->next_score = NULL;
    } else if (high_score_manager->next_score->cur_score < insertion_value->cur_score) {

        ScoreEntry* temp_score = high_score_manager->next_score;
        high_score_manager->next_score = insertion_value;
        insertion_value->next_score = temp_score;
    } else {

        ScoreEntry* current = high_score_manager->next_score;
        ScoreEntry* previous = NULL;

        while (current != NULL && current->cur_score >= insertion_value->cur_score) {
            previous = current;
            current = current->next_score;
        }

        insertion_value->next_score = current;
        if (previous != NULL) {
            previous->next_score = insertion_value;
        }
    }

    calculate_lowest_highscore(high_score_manager);
    save_highscores(high_score_manager);
	
}

void load_highscores(HighScoreManager* high_score_manager) {
    high_score_manager->next_score = NULL;
    high_score_manager->lowest = 0;

    FILE *file_read = fopen(TOP_LIST_FILE, "r");
    if (file_read == NULL) {
        printf("Hiba, a megadott fájlnév nem lett megtalálva.\n");
        return;
    }

    char temp_line[54];
    int iter = 0;

    while (fgets(temp_line, sizeof(temp_line), file_read) && iter < TOP_SCORES_SIZE) {
        int t_score = 0;
        int t_difficulty = -1;
        char t_name[13];

        int consumed = sscanf(temp_line, "%d,%d,%12s", &t_score, &t_difficulty, t_name);
        if (consumed != 3) {
            printf("Nem sikerült a top listát beolvasi!\n");
            fclose(file_read);
            return;
        }

        ScoreEntry* next_r_score = (ScoreEntry*) malloc(sizeof(ScoreEntry));
        if (next_r_score == NULL) {
            printf("Hiba a top listának való memória foglalás közben!\n");
            fclose(file_read);
            return;
        }

        next_r_score->next_score=NULL;
        next_r_score->cur_score = t_score;
        next_r_score->entry_difficulty = t_difficulty;
        strncpy(next_r_score->entry_name, t_name, sizeof(next_r_score->entry_name) - 1);
        next_r_score->entry_name[sizeof(next_r_score->entry_name) - 1] = '\0';

        add_to_highscores(high_score_manager, next_r_score);
        iter++;
    }

    fclose(file_read);
    calculate_lowest_highscore(high_score_manager);
}

void free_markers(ScoreManager* sman){
	if(sman->next_marker==NULL){
		return;
	}
	
    ScoreMarker* current = sman->next_marker;
    while (current != NULL) {
        ScoreMarker* sm_to_free = current;
        if (sm_to_free->fontSurface != NULL) {
			free(sm_to_free->fontSurface);
			sm_to_free->fontSurface=NULL;
        }
        current = current->next_marker;
        free(sm_to_free);
    }
    sman->next_marker = NULL;
}
void cleanup_scores(HighScoreManager* hscoreman,ScoreManager* sman) {
    if (hscoreman != NULL) {
        ScoreEntry* current = hscoreman->next_score;
        while (current != NULL) {
            ScoreEntry* hscores_to_free = current;
            current = current->next_score;
            free(hscores_to_free);
        }
        hscoreman->next_score = NULL;
    }
	if (sman != NULL) {
        if (sman->scorelabel != NULL) {
            cleanup_button(sman->scorelabel);
        }
		free_markers(sman);
    }
	
}


void cleanup_death_screen(DeathScreen* death_screen) {
	if(death_screen->freed==true){
		return;
	}
	death_screen->freed=true;
	if(death_screen->back_btn!=NULL){
		cleanup_button(death_screen->back_btn);
	}
	if(death_screen->name_label!=NULL){
		free(death_screen->name_label);
	}
	if(death_screen->self_label!=NULL){
		free(death_screen->self_label);
	}
}
void append_score_marker(ScoreManager* score_manager, int a_score, Vector2D pos) {
    ScoreMarker* new_marker = (ScoreMarker*) malloc(sizeof(ScoreMarker));
    if (new_marker == NULL) {
        printf("Hiba a score_marker-nek való memória foglalás közben!\n");
        return;
    }

    char score_buffer[12];
    snprintf(score_buffer, sizeof(score_buffer), "%d", a_score);

    size_t score_length = strlen(score_buffer) + 1;

    new_marker->fontSurface = (FontSurface*) malloc(sizeof(FontSurface) + score_length);
    if (new_marker->fontSurface == NULL) {
        printf("Hiba a score_maker fonthelyének való memória foglalás közben!\n");
        free(new_marker);
        return;
    }

    new_marker->fontSurface->pos = pos;
    snprintf(new_marker->fontSurface->data, score_length, "%s", score_buffer);

    new_marker->timeleft = 2.5;
    new_marker->next_marker = NULL;

    if (score_manager->next_marker == NULL) {

        score_manager->next_marker = new_marker;
    } else {

        ScoreMarker* current = score_manager->next_marker;
        while (current->next_marker != NULL && current->timeleft < 1000.0) {
            current = current->next_marker;
        }
        current->next_marker = new_marker;
    }
}
void reset_score_manager(ScoreManager* score_manager){
	score_manager->given_lives = 0;
    score_manager->score = 0;
	free_markers(score_manager);
}
void setup_score_manager(ScoreManager* score_manager) {
	score_manager->next_marker=NULL;
    reset_score_manager(score_manager);
    score_manager->base_string = strdup("pontok: ");

	
    setup_button(&score_manager->scorelabel, init_vec2(4, 4), init_vec2(0, 0), -1, init_vec2(4, 4), "pontok: ");
}
void draw_score_manager(ScoreManager* score_manager, RenderingContext* context, FontManager* font_manager, Player* player) {
    char score_buffer[20];
    snprintf(score_buffer, sizeof(score_buffer), "%d", score_manager->score);
    
    char live_buffer[20];
    snprintf(live_buffer, sizeof(live_buffer), "%d", player->lives);

    size_t base_length = strlen(score_manager->base_string);
    size_t score_length = strlen(score_buffer);
    size_t lives_length = strlen(live_buffer);
    size_t total_length = base_length + score_length + lives_length + 12;



    snprintf(score_manager->scorelabel->fontSurface->data, total_length, "%s%s e'letek: %s", score_manager->base_string, score_buffer, live_buffer);

    draw_button(score_manager->scorelabel, context, font_manager);

    ScoreMarker* current = score_manager->next_marker;
	
    while (current != NULL) {
        if (current->timeleft < 1000.0) {
            draw_letters(context, font_manager, current->fontSurface);
        }
        current = current->next_marker;
    }
}

void append_score(ScoreManager* score_manager,Player* player){
    score_manager->score+=player->bonus_score;
    player->bonus_score=0;
    if(score_manager->score-10000*score_manager->given_lives>10000){
        score_manager->given_lives++;
        player->lives++;
        play_sound(&player->player_sounds,"assets/sfx/powerup.wav");
    }
    if(score_manager->next_marker==NULL){
        return;
    }
}
void update_score_manager(ScoreManager* score_manager,GameState* game_state){
    
    ScoreMarker* current = score_manager->next_marker;
    ScoreMarker* prev = NULL;

    while (current != NULL&&current->timeleft<1000.0) {

        current->timeleft -=game_state->delta_time;

        if (current->timeleft<=0.0) {
            
            if (prev == NULL) {

                score_manager->next_marker = current->next_marker;
                free(current->fontSurface);
                free(current);
                current = score_manager->next_marker;
            } else {

                prev->next_marker = current->next_marker;
                free(current->fontSurface);
                free(current);
                current = prev->next_marker;
            }
        } else {
            prev = current;
            current = current->next_marker;
        }
    }
}
void setup_stats(DeathScreen* death_screen,ScoreManager* score_manager,HighScoreManager* high_score_manager){
	death_screen->freed=false;
    death_screen->ontoplist=false;
    death_screen->timer=20.0;

    death_screen->self_label = (FontSurface*)malloc(sizeof(FontSurface)+sizeof(char)*100);
    if (death_screen->self_label == NULL) {
        printf("Hiba a self_label-nek való memória foglalás közben!\n");
        return;
    }
    death_screen->self_label->pos = init_vec2(40, 40);


    death_screen->name_label = (FontSurface*)malloc(sizeof(FontSurface) +sizeof(char)*14);
    if (death_screen->name_label == NULL) {
        printf("Hiba a name_label-nek való memória foglalás közben!\n");
        return;
    }
    death_screen->name_label->pos = init_vec2(400,110);
    
    char val[60];
    death_screen->done=false;
    if(high_score_manager->lowest<score_manager->score){
        sprintf(val, "meghalta'l! pontszamod: %d\n bent vagy a top 10be!", score_manager->score);
        death_screen->ontoplist=true;

    } else {
        death_screen->done=true;
        sprintf(val, "meghalta'l! pontszamod: %d\n", score_manager->score);
    }

    strcpy(death_screen->self_label->data, val);
}

int register_name(char* username) {
    SDL_Event event;
    static int name_length = 0;

    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            SDL_Keycode key = event.key.keysym.sym;

            if (key == SDLK_RETURN && name_length > 0) {
                username[name_length] = '\0';
                return 1;
            }

            if (key == SDLK_BACKSPACE && name_length > 0) {
                name_length--;
                username[name_length] = '\0';
            }

            if (isalnum(key) && name_length < MAX_NAME_LENGTH) {
				char t_char=tolower((char)key);
				if(t_char >= ' ' && t_char <= '~'){
					username[name_length] = t_char;
					name_length++;
					username[name_length] = '\0';
				}
            }
        }
    }

    return 0;
}
void save_score(HighScoreManager* hman,char* player_name,ScoreManager* score_manager,GameState* gamestate){
    ScoreEntry* insertion_value=(ScoreEntry*) malloc(sizeof(ScoreEntry));

    insertion_value->cur_score=score_manager->score;
    insertion_value->entry_difficulty=gamestate->difficulty;
    strcpy(insertion_value->entry_name,player_name);

    insertion_value->next_score=NULL;

    insert_to_highscores(hman,insertion_value);
}
