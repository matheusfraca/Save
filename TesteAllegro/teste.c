#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <stdbool.h>
#include <stdio.h>
#include "estruturas.h"
// ----------------------------------------------------------------------------------------------------------------------------------------
        // VARIÁVEIS GLOBAIS
// ########################################################################################################################################

const int LARGURA_TELA = 1280;
const int ALTURA_TELA = 720;
const int NUM_BULLETS = 5;
int i;

// ----------------------------------------------------------------------------------------------------------------------------------------

// ########################################################################################################################################

ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_SAMPLE *musica_capa = NULL;
ALLEGRO_SAMPLE *sample = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_BITMAP *fundo = NULL;
ALLEGRO_BITMAP *ajustes = NULL;
ALLEGRO_BITMAP *escolha = NULL;
ALLEGRO_BITMAP *botao_off = NULL;
ALLEGRO_BITMAP *correto = NULL;
ALLEGRO_BITMAP *navea = NULL;
ALLEGRO_BITMAP *naveb = NULL;
ALLEGRO_BITMAP *navec = NULL;
ALLEGRO_BITMAP *bala = NULL;
ALLEGRO_BITMAP *fundo_nivel = NULL;
ALLEGRO_BITMAP *coracao = NULL;
ALLEGRO_BITMAP *tampa_coracao = NULL;
ALLEGRO_SAMPLE_ID *id_music = NULL;



// ----------------------------------------------------------------------------------------------------------------------------------------

// ########################################################################################################################################


enum teclas{UP, DOWN, LEFT, RIGHT, SPACE};

bool tela_ajustes = false;
bool melodia = false;
bool efeito_sonoro = false;
bool tela_de_escolha = false;
bool nave1 = false;
bool nave2 = false;
bool nave3 = false;
bool btvoltar_datela_ajustes = false;
bool btvoltar_datela_escolha = false;
bool btcomecar_datela_escolha = false;
bool tela_da_capa = true;
bool saire = false;
bool jogando = false;
bool teclas[5] = {false, false, false, false, false};
bool apagar_coracao = false;
bool tampar1 = false;
bool tampar2 = false;
bool tampar3 = false;

void InitBullet(Bullet bullet[5], int size);
void DrawBullet(Bullet bullet[5], int size);
void FireBullet(Bullet bullet[5], int size);
void UpdateBullet(Bullet bullet[5], int size);



// ----------------------------------------------------------------------------------------------------------------------------------------

// ########################################################################################################################################

bool inicializar();

int main(void)
{
//=======================================================================================================
//      POSIÇÃO INICIAL DA NAVE NA TELA

    int pos_x = 1202 / 2;
    int pos_y = 1200 / 2;
    int pos_xbala = navea;
    int pos_ybala = 1200 / 2 - 30;

    bool redraw = true;
    Bullet bullets[5];




    if (!inicializar())
    {
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return -1;
    }

// ----------------------------------------------------------------------------------------------------------------------------------------

// ########################################################################################################################################

    al_set_sample_instance_playing(musica_capa, true);
    al_init_image_addon();
    al_init_primitives_addon();
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
    al_install_keyboard();

// ----------------------------------------------------------------------------------------------------------------------------------------
        // CARREGAMENTO DOS ARQUIVOS "IMAGENS"
// ########################################################################################################################################

    fundo = al_load_bitmap("background.png");
    ajustes = al_load_bitmap("setting.png");
    botao_off = al_load_bitmap("off.png");
    escolha = al_load_bitmap("tela_escolha.png");
    correto = al_load_bitmap("correto.png");
    fundo_nivel = al_load_bitmap("fundo_nivel.png");
    navea = al_load_bitmap("navea.png");
    naveb = al_load_bitmap("naveb.png");
    navec = al_load_bitmap("navec.png");
    coracao = al_load_bitmap("vida.png");
    tampa_coracao = al_load_bitmap("tampa.png");
    bala = al_load_bitmap("tiros.png");


// ----------------------------------------------------------------------------------------------------------------------------------------
        // VERIFICAÇÃO DO CARREGAMENTO DO BACKGROUND (FUNDO)
// ########################################################################################################################################

    if (!fundo)
    {
        fprintf(stderr, "Falha ao carregar fundo.png\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_display(janela);
        al_destroy_sample(sample);
        return false;
    }
// ----------------------------------------------------------------------------------------------------------------------------------------
                //  FUNÇAO PRINCIPAL DO JOGO
// ########################################################################################################################################

    while (!saire)
    {
        while (!al_is_event_queue_empty(fila_eventos))
        {
            ALLEGRO_EVENT evento;
            al_wait_for_event(fila_eventos, &evento);

                if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                    saire = true; //BOTÃO "X" DA TELA
                }

                if (evento.type == ALLEGRO_EVENT_MOUSE_AXES) {
                    printf("[%d, %d]\n", evento.mouse.x, evento.mouse.y);
                }
// ----------------------------------------------------------------------------------------------------------------------------------------
                // RESET DE VARIÁVEIS DE CONTROLE
// ########################################################################################################################################

                if (!jogando) {
                    btcomecar_datela_escolha = false;
                }
// ----------------------------------------------------------------------------------------------------------------------------------------
        //  BOTÃO VOLTAR DA TELA AJUSTES
// ########################################################################################################################################

                if(btvoltar_datela_ajustes == true)
                {
                    tela_da_capa = true;

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                    {
                        if (evento.mouse.x >= 73 &&
                            evento.mouse.x <= 290 &&
                            evento.mouse.y >= 526 &&
                            evento.mouse.y <= 679)
                        {
                            btvoltar_datela_ajustes = true;
                            tela_ajustes = false;
                        }
                    }
                }
// ----------------------------------------------------------------------------------------------------------------------------------------
        //  BOTÃO VOLTAR DA TELA DE ESCOLHA DE NAVES
// ########################################################################################################################################

                if(btvoltar_datela_escolha == true)
                {
                    tela_da_capa = true;

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                    {
                        if (evento.mouse.x >= 130 &&
                            evento.mouse.x <= 290 &&
                            evento.mouse.y >= 535 &&
                            evento.mouse.y <= 645)
                        {
                            btvoltar_datela_escolha = true;
                            tela_de_escolha = false;
                        }
                    }
                }
// ----------------------------------------------------------------------------------------------------------------------------------------
        // TELA DA CAPA DE FUNDO
// ########################################################################################################################################

            if(tela_da_capa == true)
            {
                al_draw_bitmap(fundo , 0, 0, 0);

                if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                    saire = true; //BOTÃO "X" DA TELA
                }
//=======================================================================================================
//      BOTÃO PARA A TELA AJUSTES

                if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                {
                    if (evento.mouse.x >= 190 &&
                        evento.mouse.x <= 419 &&
                        evento.mouse.y >= 505 &&
                        evento.mouse.y <= 644)
                    {
                        tela_ajustes = true;
                    }
                }
//=======================================================================================================
//      BOTÃO PARA A TELA DE ESCOLHA DE NAVES

                if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                {
                    if (evento.mouse.x >= 475 &&
                        evento.mouse.x <= 718 &&
                        evento.mouse.y >= 505 &&
                        evento.mouse.y <= 644)
                    {
                        tela_de_escolha = true;
                    }
                }
//=======================================================================================================
//      BOTÃO PARA SAIR DO JOGO
                if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                {
                    if (evento.mouse.x >= 772 &&
                        evento.mouse.x <= 1002 &&
                        evento.mouse.y >= 505 &&
                        evento.mouse.y <= 644)
                    {
                        saire = true;
                    }
                }
            }
// ----------------------------------------------------------------------------------------------------------------------------------------
        // TELA DE AJUSTES
// ########################################################################################################################################

            if(tela_ajustes == true)
            {
                tela_de_escolha = false;
                saire = false;

                al_draw_bitmap(ajustes, 0, 0, 0);

                if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                    saire = true; //BOTÃO "X" DA TELA
                }
//=======================================================================================================
//      BOTÃO DE VOLTAR PARA A TELA DA CAPA

                if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                {
                    if (evento.mouse.x >= 73 &&
                        evento.mouse.x <= 290 &&
                        evento.mouse.y >= 526 &&
                        evento.mouse.y <= 679)
                    {
                        btvoltar_datela_ajustes = true;
                        tela_ajustes = false;
                    }
                }
//=======================================================================================================
//      BOTÃO DE MELODIA (LIGAR E DESLIGAR)

                if(melodia == true)  // VERIFICAÇÃO DO ESTADO DO BOTÃO
                {
                    al_draw_bitmap(botao_off, 420, 175, 0);

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                    {
                        if (evento.mouse.x >= 440 &&
                            evento.mouse.x <= 686 &&
                            evento.mouse.y >= 196 &&
                            evento.mouse.y <= 355)
                        {
                            melodia = false;
                        }
                    }
                }
                else if(melodia == false)  // VERIFICAÇÃO DO ESTADO DO BOTÃO
                {
                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                    {
                        if (evento.mouse.x >= 440 &&
                            evento.mouse.x <= 686 &&
                            evento.mouse.y >= 196 &&
                            evento.mouse.y <= 355)
                        {
                            melodia = true;
                        }
                    }
                }
//=======================================================================================================
//      BOTÃO DE EFEITOS SONOROS (LIGAR E DESLIGAR)

                if(efeito_sonoro == true) // VERIFICAÇÃO DO ESTADO DO BOTÃO
                {
                     al_draw_bitmap(botao_off, 855, 410, 0);

                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                    {
                        if (evento.mouse.x >= 875 &&
                            evento.mouse.x <= 1118 &&
                            evento.mouse.y >= 424 &&
                            evento.mouse.y <= 595)
                        {
                            efeito_sonoro = false;

                        }
                    }
                }
                else if(efeito_sonoro == false)  // VERIFICAÇÃO DO ESTADO DO BOTÃO
                {
                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                    {
                        if (evento.mouse.x >= 875 &&
                            evento.mouse.x <= 1118 &&
                            evento.mouse.y >= 424 &&
                            evento.mouse.y <= 595)
                        {
                            efeito_sonoro = true;
                        }
                    }
                }
            }
// ----------------------------------------------------------------------------------------------------------------------------------------
        // TELA DE ESCOLHA DE NAVES
// ########################################################################################################################################
          if(tela_de_escolha == true)
            {
                tela_ajustes = false;
                saire = false;

                al_draw_bitmap(escolha, 0, 0, 0);

                if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                    saire = true; // BOTÃO "X" DA TELA
                }
//=======================================================================================================
//      BOTÃO DE VOLTAR PARA A TELA DE CAPA

                if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                {
                    if (evento.mouse.x >= 100 &&
                        evento.mouse.x <= 316 &&
                        evento.mouse.y >= 510 &&
                        evento.mouse.y <= 650)
                    {
                        btvoltar_datela_escolha = true;
                        tela_de_escolha = false;
                    }
                }
//=======================================================================================================
//     BOTÃO DE COMEÇAR PARA JOGAR

                printf("antes do if: %d\n", btcomecar_datela_escolha);
               if(btcomecar_datela_escolha == false) // VERIFICAÇÃO DO ESTADO DO BOTÃO
                {
                    printf("dentro do if da tela de escolha\n");
                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
                    {
                        printf("dentro do if da tela de escolha - AND - button down\n");
                        if (evento.mouse.x >= 554 &&
                            evento.mouse.x <= 747 &&
                            evento.mouse.y >= 534 &&
                            evento.mouse.y <= 676)
                        {
                            printf("dentro do if da tela de escolha - AND - button down - AND - dentro da área\n");
                            btcomecar_datela_escolha = true;
                            tela_de_escolha = false;
                        }
                    }
                }
//=======================================================================================================
//      BOTÃO DE ESCOLHA DA NAVE 1

                if(nave1 == false)
                {
                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                        if (evento.mouse.x >= 163 &&
                            evento.mouse.x <= 425 &&
                            evento.mouse.y >= 192 &&
                            evento.mouse.y <= 466)
                        {
                            nave1 = true;
                            nave2 = false;
                            nave3 = false;
                        }
                    }
                }

                else (nave1 == true);

//=======================================================================================================
//      BOTÃO DE ESCOLHA DA NAVE 2
                if(nave2 == false)
                {
                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                        if (evento.mouse.x >= 512 &&
                            evento.mouse.x <= 765 &&
                            evento.mouse.y >= 204 &&
                            evento.mouse.y <= 459)
                        {
                            nave1 = false;
                            nave2 = true;
                            nave3 = false;
                        }
                    }
                }

                else if(nave2 == true);

//=======================================================================================================
//      BOTÃO DE ESCOLHA DA NAVE 3
                if(nave3 == false)
                {
                    if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
                    {
                        if (evento.mouse.x >= 806 &&
                            evento.mouse.x <= 1115 &&
                            evento.mouse.y >= 204 &&
                            evento.mouse.y <= 459)
                        {
                            nave1 = false;
                            nave2 = false;
                            nave3 = true;
                        }
                    }
                }

                else if(nave3 == true);

//=======================================================================================================
//      CÓDIGO DE DESENHAR O "CORRETO" NA NAVE SELECIONADA

                if(nave1 == true)
                {
                    al_draw_bitmap(correto, 200, 241, 0);
                }
                else if(nave2 == true)
                {
                    al_draw_bitmap(correto, 539, 241, 0);
                }
                else if(nave3)
                {
                    al_draw_bitmap(correto, 888, 241, 0);
                }
            }

//=======================================================================================================
//      BOTÃO DE COMECAR DA TELA DE ESCOLHA

            printf("antes da tela do if da tela do jogo: %d / n1: %d / n2: %d / n3: %d\n", btcomecar_datela_escolha, nave1, nave2, nave3);
            if((btcomecar_datela_escolha == true) && (nave1 == true || nave2 == true || nave3 == true)) // VERIFICAÇÃO DO ESTADO DO BOTÃO
            {
                printf("dentro do if da tela do jogo\n");
                saire = false;

                al_draw_bitmap(fundo_nivel, 0, 0, 0);
                al_draw_bitmap(coracao, 50, 0, 0);
                al_draw_bitmap(coracao, 120, 0, 0);
                al_draw_bitmap(coracao, 190, 0, 0);

//=======================================================================================================
//      FUNÇÃO TAMPAR CORAÇÃO "VIDA"
                if(apagar_coracao == true)
                {
                    if(tampar1 == true)
                    {
                        al_draw_bitmap(tampa_coracao, 210, 10, 0);
                        //tampar1 = false;
                    }

                    if(tampar2 == true)
                    {
                        al_draw_bitmap(tampa_coracao, 140, 10, 0);
                        tampar2 = false;
                    }

                    if(tampar3 == true)
                    {
                        al_draw_bitmap(tampa_coracao, 70, 10, 0);
                        tampar3 = false;
                    }
                }

//=======================================================================================================
//      JOGANDO COM A NAVE 1

                if(nave1 == true) // MOVER A NAVE COM O TECLADO PRESSIONADO
                {

                    if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
                    {
                        switch(evento.keyboard.keycode)
                        {
                            case ALLEGRO_KEY_UP:
                            teclas[UP] = true;
                            break;
                            case ALLEGRO_KEY_DOWN:
                            teclas[DOWN] = true;
                            break;
                            case ALLEGRO_KEY_LEFT:
                            teclas[LEFT] = true;
                            break;
                            case ALLEGRO_KEY_RIGHT:
                            teclas[RIGHT] = true;
                            break;
                            case ALLEGRO_KEY_SPACE:
                            teclas[SPACE] = true;
                            break;
                        }
                    }
                    if (evento.type == ALLEGRO_EVENT_KEY_UP)
                    {
                        switch(evento.keyboard.keycode)
                        {
                            case ALLEGRO_KEY_UP:
                            teclas[UP] = false;
                            break;
                            case ALLEGRO_KEY_DOWN:
                            teclas[DOWN] = false;
                            break;
                            case ALLEGRO_KEY_LEFT:
                            teclas[LEFT] = false;
                            break;
                            case ALLEGRO_KEY_RIGHT:
                            teclas[RIGHT] = false;
                            break;

                        }
                    }
                     if(teclas[SPACE]== true)
                    {

                        al_draw_bitmap(bala, pos_x - 210, pos_y - 200,0);

                    }
                    al_draw_bitmap(navea, pos_x, pos_y, 0);
                    pos_y -= teclas[UP] * 10;
                    pos_y += teclas[DOWN] * 10;
                    pos_x -= teclas[LEFT] * 10;
                    pos_x += teclas[RIGHT] * 10;

                    if(pos_x>=830)
                    {
                        teclas[RIGHT] = false;
                    }
                    else if(pos_x<=371)
                    {
                        teclas[LEFT] = false;
                    }
                    if(pos_y>=630)
                    {
                        teclas[DOWN] = false;
                    }
                    else if(pos_y<=0)
                    {
                        teclas[UP] = false;
                    }

                    if(pos_y == 0 || pos_y == 720 || pos_x == 371 || pos_x == 830)
                    {
                       apagar_coracao = true;
                       tampar1 = true;
                       //tampar2 = true;
                       //tampar3 = true;
                    }
                }

//=======================================================================================================
//      JOGANDO COM A NAVE 2

                if(nave2 == true)
                {
                    if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
                    {
                        switch(evento.keyboard.keycode)
                        {
                            case ALLEGRO_KEY_UP:
                            teclas[UP] = true;
                            break;
                            case ALLEGRO_KEY_DOWN:
                            teclas[DOWN] = true;
                            break;
                            case ALLEGRO_KEY_LEFT:
                            teclas[LEFT] = true;
                            break;
                            case ALLEGRO_KEY_RIGHT:
                            teclas[RIGHT] = true;
                            break;
                        }
                    }

                    if (evento.type == ALLEGRO_EVENT_KEY_UP)
                    {
                        switch(evento.keyboard.keycode)
                        {
                            case ALLEGRO_KEY_UP:
                            teclas[UP] = false;
                            break;
                            case ALLEGRO_KEY_DOWN:
                            teclas[DOWN] = false;
                            break;
                            case ALLEGRO_KEY_LEFT:
                            teclas[LEFT] = false;
                            break;
                            case ALLEGRO_KEY_RIGHT:
                            teclas[RIGHT] = false;
                            break;
                        }
                    }

                    al_draw_bitmap(naveb, pos_x, pos_y, 0);
                    pos_y -= teclas[UP] * 10;
                    pos_y += teclas[DOWN] * 10;
                    pos_x -= teclas[LEFT] * 10;
                    pos_x += teclas[RIGHT] * 10;

                    if(pos_x>=830)
                    {
                        teclas[RIGHT] = false;
                    }
                    else if(pos_x<=371)
                    {
                        teclas[LEFT] = false;
                    }
                    if(pos_y>=644)
                    {
                        teclas[DOWN] = false;
                    }
                    else if(pos_y<=0)
                    {
                        teclas[UP] = false;
                    }

                }

//=======================================================================================================
//      JOGANDO COM A NAVE 3

                if(nave3 == true)
                {
                    if (evento.type == ALLEGRO_EVENT_KEY_DOWN)
                    {
                        switch(evento.keyboard.keycode)
                        {
                            case ALLEGRO_KEY_UP:
                            teclas[UP] = true;
                            break;
                            case ALLEGRO_KEY_DOWN:
                            teclas[DOWN] = true;
                            break;
                            case ALLEGRO_KEY_LEFT:
                            teclas[LEFT] = true;
                            break;
                            case ALLEGRO_KEY_RIGHT:
                            teclas[RIGHT] = true;
                            break;
                        }
                    }

                    if (evento.type == ALLEGRO_EVENT_KEY_UP)
                    {
                        switch(evento.keyboard.keycode)
                        {
                            case ALLEGRO_KEY_UP:
                            teclas[UP] = false;
                            break;
                            case ALLEGRO_KEY_DOWN:
                            teclas[DOWN] = false;
                            break;
                            case ALLEGRO_KEY_LEFT:
                            teclas[LEFT] = false;
                            break;
                            case ALLEGRO_KEY_RIGHT:
                            teclas[RIGHT] = false;
                            break;
                        }
                    }

                    al_draw_bitmap(navec, pos_x, pos_y, 0);
                    pos_y -= teclas[UP] * 10;
                    pos_y += teclas[DOWN] * 10;
                    pos_x -= teclas[LEFT] * 10;
                    pos_x += teclas[RIGHT] * 10;

                    if(pos_x>=830)
                    {
                        teclas[RIGHT] = false;
                    }
                    else if(pos_x<=371)
                    {
                        teclas[LEFT] = false;
                    }
                    if(pos_y>=626)
                    {
                        teclas[DOWN] = false;
                    }
                    else if(pos_y<=0)
                    {
                        teclas[UP] = false;
                    }
                }

                jogando = true;

                if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                {
                    saire = true; // BOTÃO "X" DA TELA
                }

            }

        }
        // ATUALIZA A TELA
        al_flip_display();
    }
//=======================================================================================================
//      DESTRUIÇÃO DOS ARQUIVOS - LIMPA MEMÓRIA

    al_destroy_bitmap(fundo);
    al_destroy_bitmap(ajustes);
    al_destroy_bitmap(botao_off);
    al_destroy_bitmap(escolha);
    al_destroy_bitmap(navea);
    al_destroy_bitmap(naveb);
    al_destroy_bitmap(navec);
    al_destroy_bitmap(correto);
    al_destroy_bitmap(coracao);
    al_destroy_bitmap(bala);
    al_destroy_sample(musica_capa);
    al_destroy_event_queue(fila_eventos);
    al_destroy_display(janela);

    return 0;
}
void InitBullet(Bullet bullet[], int size)
{
	for( i = 0; i < size; i++)
	{
		bullet[i].ID = BULLET;
		bullet[i].speed = 10;
		bullet[i].live = false;
	}
}
void DrawBullet(Bullet bullet[], int size)
{
	for( i = 0; i < size; i++)
	{
		if(bullet[i].live)
			al_draw_filled_circle(bullet[i].x, bullet[i].y, 2, al_map_rgb(0, 0, 255));
	}
}
void FireBullet(Bullet bullet[], int size)
{
	for( i = 0; i < size; i++)
	{
		if(!bullet[i].live)
		{
			bullet[i].x = 1202/2 + 17;
			bullet[i].y = 1200;
			bullet[i].live = true;
			break;
		}
	}
}
void UpdateBullet(Bullet bullet[], int size)
{
	for( i = 0; i < size; i++)
	{
		if(bullet[i].live)
		{
			bullet[i].x += bullet[i].speed;
			if(bullet[i].x > ALTURA_TELA)
				bullet[i].live = false;
		}
	}
}


// ----------------------------------------------------------------------------------------------------------------------------------------
        //INICIALIZAÇÕES E FUNÇÕES
// ########################################################################################################################################

bool inicializar()
{

    if (!al_init())
    {
        fprintf(stderr, "Não inicializou Allegro.\n");
        return false;
    }

    if (!al_install_audio())
    {
        fprintf(stderr, "Falha ao inicializar áudio.\n");
        return false;
    }

    if (!al_init_acodec_addon())
    {
        fprintf(stderr, "Falha ao inicializar codecs de áudio.\n");
        return false;
    }

    if (!al_reserve_samples(1))
    {
        fprintf(stderr, "Falha ao alocar canais de áudio.\n");
        return false;
    }
    if (!al_install_keyboard())
    {
        fprintf(stderr, "Falha ao inicializar teclado.\n");
        return false;
    }

    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela)
    {
        fprintf(stderr, "Falha ao criar a janela.\n");
        return false;
    }
    fila_eventos = al_create_event_queue();
    if (!fila_eventos)
    {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
        al_destroy_sample(sample);
        return false;
    }

    id_music = malloc(sizeof(ALLEGRO_SAMPLE_ID));
    musica_capa = al_load_sample("04.wav");
    al_play_sample(musica_capa, 0.6, 0.3, 1, ALLEGRO_PLAYMODE_LOOP, id_music);
    if (!musica_capa)
    {
        fprintf(stderr, "Falha ao carregar audio.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_display(janela);
        al_destroy_sample(sample);
        return false;
    }
            // CONFIGURAÇÃO DO TÍTULO DA JANELA
    al_set_window_title(janela, "SAVE THE EARTH - The Social Problems");

            // TORNA APTO O USO DO MOUSE
    if (!al_install_mouse())
    {
        fprintf(stderr, "Falha ao inicializar o mouse.\n");
        al_destroy_display(janela);
        return -1;
    }

            // ATRIBUI O CURSOR PADRÃO DO SISTEMA A SER USADO
    if (!al_set_system_mouse_cursor(janela, ALLEGRO_SYSTEM_MOUSE_CURSOR_DEFAULT))
    {
        fprintf(stderr, "Falha ao atribuir ponteiro do mouse.\n");
        al_destroy_display(janela);
        return -1;
    }

    if (!id_music)
    {
        fprintf(stderr, "Falha ao carregar id_music.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_display(janela);
        al_destroy_sample(sample);
        return false;
    }

//=======================================================================================================
//      REGISTROS DE EVENTOS
    al_register_event_source(fila_eventos, al_get_mouse_event_source());
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));

    return true;
}
