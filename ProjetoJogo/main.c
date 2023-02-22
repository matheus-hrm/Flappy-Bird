/*  UNIVERSIDADE FEDERAL DE GOIÁS - UFG
    INSTITUTO DE INFORMÁTICA - INF
    DISCIPLINA: INTRODUÇÃO À PROGRAMAÇÃO
    PROFESSOR: THIERSON COUTO ROSA
    ALUNO: MATHEUS HENRIQUE RODRIGUES MAGALHAES
    MATRÍCULA: 202103760
    PROJETO FINAL DA DISCIPLINA : FLAPPY BIRD */

#include "./header.h"

int main (){  
    
    inicia();
    
    ALLEGRO_DISPLAY * display = al_create_display(WIDTH,HEIGHT);  // cria a janela
    ALLEGRO_FONT* font = al_create_builtin_font();                // cria a fonte
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);            // cria o timer
    ALLEGRO_EVENT_QUEUE * event_queue = al_create_event_queue(); // cria a fila de eventos

    check(display,"display");                                     // verifica se a janela foi criada
    check(font,"fonte");                                          // verifica se a fonte foi criada
    check(timer, "timer");                                        // verifica se o timer foi criado
    check(event_queue, "fila de eventos");                       // verifica se a fila de eventos foi criada
    
    ALLEGRO_BITMAP* background = al_load_bitmap("./image/background.png"); // carrega o plano de fundo
    ALLEGRO_BITMAP* passaro = al_load_bitmap("./image/bird1.png");              // carrega o passaro
    ALLEGRO_BITMAP* cano = al_load_bitmap("./image/pipe-green.png");      // carrega o cano
    ALLEGRO_BITMAP* base = al_load_bitmap("./image/base.png");            // carrega a base
    ALLEGRO_SAMPLE* asas = al_load_sample("./audio/wing.wav");            // carrega o som das asas
    ALLEGRO_SAMPLE* ponto = al_load_sample("./audio/point.wav");               // carrega o som do ponto
    ALLEGRO_BITMAP* gameover = al_load_bitmap("./image/gameover.png");         // carrega a imagem de gameover
    ALLEGRO_SAMPLE* hit = al_load_sample("./audio/die.wav");                   // carrega o som do hit
    ALLEGRO_BITMAP* box = al_load_bitmap("./image/box.png");             // carrega a caixa
    ALLEGRO_BITMAP* placar = al_load_bitmap("./image/placar.png");             // carrega a caixa
    
    check(background, "plano de fundo"); 
    check(passaro, "passaro");
    check(cano, "cano");
    check(base, "base");
    check(box, "box");
    check(asas, "asas");
    check(ponto, "ponto");
    check(gameover, "gameover");
    check(hit, "hit");
    check(placar, "placar");

  
    al_register_event_source(event_queue, al_get_timer_event_source(timer)); // registra o timer na fila de eventos
    al_set_window_position(display, 200, 200);                             // posiciona a janela
    al_set_window_title(display, "Flappy Bird!");                          // coloca o titulo na janela
    al_register_event_source(event_queue, al_get_display_event_source(display)); // registra o display na fila de eventos
    al_register_event_source(event_queue, al_get_keyboard_event_source()); // registra o teclado na fila de eventos
    al_start_timer(timer);                                                // inicia o timer

    //variaveis
    //SPRITES sprite;
    restart:; // label para reiniciar o jogo  
    BIRD bird; 
    CAIXA *caixa = (CAIXA*) malloc(NUM_BOXES * sizeof(CAIXA));             // cria a struct da caixa
    if(caixa == NULL){                                                 // verifica se a memoria foi alocada
      printf("Erro ao alocar memoria");
      exit(1);
    }                                                        // cria a struct do passaro
    PIPE *pipe = (PIPE*) malloc(NUM_PIPES * sizeof(PIPE));             // cria a struct do cano
    if(pipe == NULL){                                                 // verifica se a memoria foi alocada
      printf("Erro ao alocar memoria");
      exit(1);
    }
    short int p = 0;                                                 // variavel para o cano
    float currentFrame = 0.3f;                                      // variavel para o frame
    
    bird.posY = CenterY - 100; 
    bird.posX = WIDTH/4;
    startpipes(pipe);                                              // inicia os canos
    startboxes(caixa);                                             // inicia as caixas
    short int GAMEOVER = 0;                                         // variavel para o gameover
    int score = 0;                                                 // pontuacao

    bird.velY = 0.0f;                                             // velocidade do passaro
    float inclinacao = 0.0f;                                    // inclinacao do passaro
    int highscore;                                       // highscore
    FILE *highscoretxt = fopen("highscore.txt", "r");          // abre o arquivo do highscore
    if(highscoretxt == NULL){                                   // verifica se o arquivo foi aberto
      printf("Erro ao abrir o arquivo");
      exit(1);
    }
    fscanf(highscoretxt, "%d", &highscore);                     // le o arquivo do highscore
    fclose(highscoretxt);                                      // fecha o arquivo do highscore 
  while(true){                                                 // loop do jogo
    ALLEGRO_EVENT event;                                       // cria o evento
    al_wait_for_event(event_queue, &event);                    // espera o evento acontecer
    if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE ){            // se o evento for fechar a janela
      break;                                                   // sai do loop
    }
    switch (event.type) {                                      // switch para o evento
      case ALLEGRO_EVENT_KEY_DOWN:                             // se o evento for apertar uma tecla
        if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {    // se a tecla for ESC
          return 0;                                            // sai do jogo
        }
        if(event.keyboard.keycode == ALLEGRO_KEY_SPACE){       // se a tecla for espaço
          bird.velY = -10.0f;                                  // muda a velocidade
          inclinacao = -15.0f;                                 // muda a inclinacao 
          al_play_sample(asas,0.5,0,1,ALLEGRO_PLAYMODE_ONCE, 0); // toca o som das asas
        }
      case ALLEGRO_EVENT_TIMER:                               // se o evento for o timer
        currentFrame += 0.3f;                                 // muda o frame
        bird.velY += GRAVIDADE;                               // muda a velocidade
        bird.posY += bird.velY;                               // muda a posacao
        inclinacao = bird.velY * 3.0f;                        // muda a inclinacao
        if(bird.velY > 15.0f){                                // se a velocidade for maior que 15
          bird.velY = 15.0f;                                  // muda a velocidade
        }
        if(bird.velY < 0){                                    // se a velocidade for menor que 0
          inclinacao = -15.0f;                                // muda a inclinacao
        } else {                                              // se não
          inclinacao = 20.0f;                                 // muda a inclinacao
        }
        if(bird.posY < -48){                                  // nao deixa o passaro sair da tela  
          bird.posY = -48;
        }
        if (highscore < score) {                              // se o score for maior que o highscore
          highscore = score;                               // muda o highscore
          fopen("highscore.txt", "w");                      // abre o arquivo do highscore
          if(highscoretxt == NULL){                         // verifica se o arquivo foi aberto
            printf("Erro ao abrir o arquivo");
            exit(1);
          }
          fprintf(highscoretxt, "%d", highscore);            // escreve o highscore no arquivo
          fclose(highscoretxt);                            // fecha o arquivo do highscore
        }
        if(bird.posY > 500){                                 // termina o jogo se o passaro bater no chão
          al_play_sample(hit,0.5,0,1,ALLEGRO_PLAYMODE_ONCE, 0); // toca o som do hit
          GAMEOVER = 1;
        }
        if((bird.posX == pipe[p].posX + 52) && pipe[p].passou == false){ // se o passaro passar pelo cano
          pipe[p].passou = true; 
        }
        if(currentFrame >= 3){ // se o frame for maior que 3
          currentFrame -= 3; // volta o frame
        }
        for (int p = 0; p < NUM_PIPES; p++) {
          if (bird.posX >= pipe[p].posX && bird.posX <= pipe[p].posX + 52) {
            if (bird.posY <= pipe[p].gapY || bird.posY >= pipe[p].gapY + pipe[p].gap) {// colisão com o cano
              al_play_sample(hit,0.5,0,2,ALLEGRO_PLAYMODE_ONCE, 0);
              GAMEOVER = 1;
            } else if (!pipe[p].passou) { // o pássaro passou pelo cano
              pipe[p].passou = true;
              score+=10; // aumenta o score
              al_play_sample(ponto,1,0,1,ALLEGRO_PLAYMODE_ONCE, 0);
            }
          }
        }
        for(int i = 0; i < NUM_BOXES; i++){
          if(bird.posX >= caixa[i].posX && bird.posX <= caixa[i].posX + 60){
            if(bird.posY <= caixa[i].posY + 60  && bird.posY   >= caixa[i].posY){// colisão com a caixa
              al_play_sample(hit,0.5,0,2,ALLEGRO_PLAYMODE_ONCE, 0);
              GAMEOVER = 1;
            }
          }
        }
        if(GAMEOVER == 1){
          drawscore(font, placar, score, highscore);
          al_draw_bitmap(gameover, CenterX - 110 , CenterY + 100, 0); // desenha a imagem de gameover
          al_flip_display();
          al_rest(2);
          al_draw_text(font, al_map_rgb(0, 0, 0), CenterX, CenterY, ALLEGRO_ALIGN_CENTRE, "Pressione ESC para sair ou SPACE para reiniciar");
          al_flip_display();
          while(true){
            al_wait_for_event(event_queue, &event);
            if (event.type == ALLEGRO_EVENT_KEY_DOWN) { // se o evento for apertar uma tecla
              if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) { // se a tecla for espaço
                goto restart; // reinicia o jogo
              }
              if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) { // se a tecla for ESC
                goto destroy; // sai do jogo
              }
            }
          }
        }
      }
    if (redraw && al_is_event_queue_empty(event_queue)) { // se a tela for redesenhada e a fila de eventos estiver vazia
      redraw = false;
      al_draw_bitmap(background, 0, 0, 0); // desenha o fundo
      drawpipe(cano, pipe);
      al_draw_bitmap(base,   0, 500, 0); 
      al_draw_bitmap(base, 336, 500, 0); 
      al_draw_bitmap(base, 672, 500, 0);
      drawbird(bird, passaro, inclinacao, currentFrame); // desenha o passaro
      al_draw_textf(font, al_map_rgb(255, 255, 255), CenterX, 10, ALLEGRO_ALIGN_CENTER, "%d", score);
      drawboxes(box,caixa);
      al_flip_display();
    }
    al_flip_display();
  }
  destroy:
    
    free(pipe); // libera a memoria
    al_destroy_timer(timer);
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_bitmap(box);
    al_destroy_bitmap(background);
    al_destroy_bitmap(passaro);
    al_destroy_bitmap(cano);
    al_destroy_bitmap(base);
    al_destroy_sample(asas);
    al_destroy_sample(ponto);
    al_destroy_bitmap(gameover);
    al_destroy_sample(hit);
    al_destroy_bitmap(placar);
    

    return 0;
}

