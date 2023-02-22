#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/keyboard.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/transformations.h>

#define HEIGHT 600
#define WIDTH 1000
#define CenterX WIDTH/2
#define CenterY HEIGHT/2
#define FPS 30
#define KEY_SEEN     1
#define KEY_RELEASED 2
#define GRAVIDADE 1.2
#define NUM_PIPES 6
#define NUM_BOXES 3

typedef struct PIPE {
    float posX;
    float posY;
    float speed;
    float gap;
    float gapY;
    bool passou;
} PIPE;

typedef struct BIRD {
    float posY;
    float posX; 
    float speed;
    float velY;
} BIRD;

typedef struct Score {
    int *score;
} SCORE;

typedef struct SPRITES{
  ALLEGRO_BITMAP* background;
  ALLEGRO_BITMAP* cano;
  ALLEGRO_BITMAP* base;
  ALLEGRO_BITMAP* passaro;
  ALLEGRO_BITMAP* gameover;
  ALLEGRO_BITMAP* placar;
  ALLEGRO_BITMAP* box;
} SPRITES;

typedef struct box{
  float posX;
  float posY;
  float speed;
} CAIXA;

typedef struct display{
  ALLEGRO_DISPLAY * display;
  ALLEGRO_FONT* font;
  ALLEGRO_TIMER* timer;
  ALLEGRO_EVENT_QUEUE * event_queue;
}display;

void drawpipe(ALLEGRO_BITMAP *cano, PIPE pipe[]);
void inicia();
void check(bool test,char *str);
void drawbird(BIRD bird, ALLEGRO_BITMAP *passaro, float inclinacao, float currentFrame);
void startboxes(CAIXA caixa[]);
void drawboxes(ALLEGRO_BITMAP* box, CAIXA caixa[]);
void drawscore(ALLEGRO_FONT* font, ALLEGRO_BITMAP *placar, int score, int highscore);
bool redraw = true;

void startpipes(PIPE pipe[]){
  for(int i = 0; i < NUM_PIPES; i++){
      pipe[i].posX = 950 + i * 330;
      pipe[i].posY = rand() % (HEIGHT - 450) + 200; // define uma posicao aleatória para o pos
      pipe[i].gap = 120;
      pipe[i].gapY = (pipe[i].posY - pipe[i].gap);
      pipe[i].speed = 3.0f;
      pipe[i].passou = false;
  }
}

void drawpipe(ALLEGRO_BITMAP *cano, PIPE pipe[]) {
  for(int i = 0; i < NUM_PIPES; i++) {
    al_draw_bitmap_region(cano, 0, 0, 52, pipe[i].posY, pipe[i].posX , -1 * pipe[i].gap , ALLEGRO_FLIP_VERTICAL);
    al_draw_bitmap_region(cano, 0, 0, 52, pipe[i].posY +112, pipe[i].posX ,  pipe[i].gap + pipe[i].gapY, 0);
    pipe[i].posX -= (int) pipe[i].speed;
    
    if(pipe[i].posX < -104){
      pipe[i].posX = 200 * i + 1052;
      pipe[i].posY = rand() % (HEIGHT - 450) + 200; // define uma posicao aleatória para o pos
      pipe[i].gap = 110;
      pipe[i].gapY = (pipe[i].posY - pipe[i].gap);
      pipe[i].speed = 3.0f;
      pipe[i].passou = false;
      redraw = true;
    }
  } 
}
void drawbird(BIRD bird, ALLEGRO_BITMAP *passaro, float inclinacao, float currentFrame){
  ALLEGRO_TRANSFORM transform;
  al_identity_transform(&transform);
  al_rotate_transform(&transform, inclinacao * ALLEGRO_PI / 180.0);
  al_translate_transform(&transform, bird.posX, bird.posY);
  al_use_transform(&transform);
  al_draw_bitmap_region(passaro, 43 * (int) currentFrame, currentFrame , 40, 80, -20, -40, 0);
  al_identity_transform(&transform);
  al_use_transform(&transform);
  redraw = true;
}
void inicia(){
  if(!al_init()) {
        printf("falha ao inicializar allegro!\n");
        exit(1);
    }
    if(!al_init_image_addon()) {
        printf("falha ao inicializar image addon!\n");
        exit(1);
    }
    if(!al_install_keyboard()) {
        printf("falha ao inicializar keyboard!\n");
        exit(1);
    }
    if(!al_init_font_addon()) {
        printf("falha ao inicializar font addon!\n");
        exit(1);
    }
    if(!al_init_acodec_addon()) {
        printf("falha ao inicializar audio codec addon!\n");
        exit(1);
    }
    if(!al_install_audio()) {
        printf("falha ao inicializar audio!\n");
        exit(1);
    }
    if(!al_reserve_samples(1)) {
        printf("falha ao inicializar samples!\n");
        exit(1);
    }
}
void check(bool test, char *str){
    if(!test){
      printf("falha ao carregar %s!\n", str);
      exit(1);
    }
}
void startboxes(CAIXA caixa[]){
  for(int i = 0; i < NUM_BOXES; i++){
    caixa[i].posX = rand () % ((WIDTH) + i * 500) + 900;
    caixa[i].posY = -100; // define uma posicao aleatória para o pos
    caixa[i].speed = 3.0f;
  }
}
void drawboxes(ALLEGRO_BITMAP* box, CAIXA caixa[]){
  for(int i = 0; i < NUM_BOXES; i++){
    al_draw_bitmap_region(box, 0, 0, 60, 60, caixa[i].posX, caixa[i].posY, 0);
    caixa[i].posY += (int) caixa[i].speed * 2;
    caixa[i].posX -= (int) caixa[i].speed;	

    if (caixa[i].posY >= 440){
      
      caixa[i].posY = rand () % (HEIGHT) - 600;
      caixa[i].posX = rand () % (WIDTH) + i * 500;
      redraw = true;
    }
  }

}
void drawscore(ALLEGRO_FONT* font, ALLEGRO_BITMAP *placar, int score, int highscore){
  int placarX = CenterX - 70;
  int placarY = CenterY - 100;
  int pontosX = CenterX + 19;
  int pontosY = CenterY - 82;
  ALLEGRO_TRANSFORM transform;
  al_identity_transform(&transform);
  al_use_transform(&transform);
  al_scale_transform(&transform, 2, 2);
  if(score < 10){
    al_draw_bitmap_region(placar, 0, 0,113, 58, placarX, placarY, 0); // desenha a imagem do placar
    al_draw_textf(font, al_map_rgb(255, 255, 255), pontosX,pontosY, 0, "%d", score);
    al_draw_textf(font, al_map_rgb(255, 255, 255), pontosX,pontosY + 19, 0, "%d", highscore); 
  }
  else if(score < 20 && score >= 10){
    al_draw_bitmap_region(placar, 114,0, 113, 58, placarX, placarY, 0); // desenha a imagem do placar
    al_draw_textf(font, al_map_rgb(255, 255, 255), pontosX,pontosY, 0, "%d", score);
    al_draw_textf(font, al_map_rgb(255, 255, 255), pontosX,pontosY + 19, 0, "%d", highscore);
  }
  else if(score < 30 && score >= 20){
    al_draw_bitmap_region(placar, 228,0, 113, 58, placarX, placarY, 0); // desenha a imagem do placar
    al_draw_textf(font, al_map_rgb(255, 255, 255),pontosX,pontosY, 0, "%d", score);
    al_draw_textf(font, al_map_rgb(255, 255, 255), pontosX,pontosY + 19, 0, "%d", highscore);
  }
  else if(score >= 30){
    al_draw_bitmap_region(placar, 341,0, 114, 58, placarX, placarY, 0); // desenha a imagem do placar
    al_draw_textf(font, al_map_rgb(255, 255, 255), pontosX,pontosY, 0, "%d", score);
    al_draw_textf(font, al_map_rgb(255, 255, 255), pontosX,pontosY + 19, 0, "%d", highscore);
  }
  al_identity_transform(&transform);
  al_use_transform(&transform);
  
  redraw = true;
}
