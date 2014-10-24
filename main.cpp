
#include<SDL.h>
#include<SDL_image.h>
#include<iostream>
#include<stdlib.h>
#include<list>

using namespace std;

SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event Event;
SDL_Texture *background, *game_background, *pelotita_buena, *pelotita_mala, *game_over;
SDL_Rect rect_background, rect_pelotita;

//Game Over instanciado primero para que no cree conflictos con el loop del mismo en el loopJuego
void loopGameOver()
{
    while(true)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                return;
            }
            if(Event.type == SDL_KEYDOWN)
            {


                if(Event.key.keysym.sym == SDLK_ESCAPE)
                    return;
            }
        }

        SDL_RenderCopy(renderer, game_over, NULL, &rect_background);
        SDL_RenderPresent(renderer);
    }
}

void loopJuego()
{
    list<float>pelotitas_x;
    pelotitas_x.push_back(100);
    pelotitas_x.push_back(300);
    pelotitas_x.push_back(0);
    pelotitas_x.push_back(200);
    int contador = 4; //Contadores para el Game Over (cuentan el numero de pelotitas restantes)
    int cont2 = 0;
    bool derecha = true;

    while(true)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                exit(0);
            }
            if(Event.type == SDL_KEYDOWN)
            {


                if(Event.key.keysym.sym == SDLK_ESCAPE)
                    return;
            }
            if(Event.type == SDL_MOUSEBUTTONDOWN)
            {
                int click_x , click_y;
                SDL_GetMouseState(&click_x, &click_y);
                cout<<click_x<<","<<click_y<<endl;


                for(list<float>::iterator i=pelotitas_x.begin(); i!=pelotitas_x.end(); i++)
                {
                    if(click_x<66+(*i) && click_x>0+(*i) && click_y<66+rect_pelotita.y && click_y>0)
                    {
                        pelotitas_x.erase(i);
                        contador-=1; //Resta al contador cada vez que se destuye una pelotita
                        break;
                    }
                }
            }

        }
        if(contador==0){
            loopGameOver(); //Presenta el Game Over una vez que ya no queden pelotitas
            return;
        }
        for(list<float>::iterator i=pelotitas_x.begin(); i!=pelotitas_x.end(); i++)
        {
            (*i)+=0.1;
            if((*i)>=434)
                (*i)=0;

//            if(derecha==true)
//            (*i)+=0.1;
//            if((*i)>=434)
//                derecha=false;
//            if(derecha==false)
//            (*i)-=0.1;
//            if((*i)<=0)
//                derecha=true;
        }

        SDL_RenderCopy(renderer, game_background, NULL, &rect_background);
        //SDL_RenderCopy(renderer, pelotita_buena, NULL, &rect_pelotita);
        for(list<float>::iterator i=pelotitas_x.begin(); i!=pelotitas_x.end(); i++)
        {
            //Este codigo ayuda a poner las pelotitas en diferentes partes de Y, ademas de cambiar su posicion
            rect_pelotita.x=(*i);

            if(cont2==4){
            if(contador==4)
                cont2=0;
            if(contador==3)
                cont2=1;
            if(contador==2)
                cont2=2;
            if(contador==1)
                cont2=3;
            }


            if(cont2==0)
                rect_pelotita.y =0;
            if(cont2==1)
                rect_pelotita.y =50;
            if(cont2==2)
                rect_pelotita.y =100;
            if(cont2==3)
                rect_pelotita.y =150;

            cont2++;

            SDL_RenderCopy(renderer, pelotita_mala, NULL, &rect_pelotita);
        }

        rect_pelotita.x=0;
        SDL_RenderPresent(renderer);
    }

}


void loopMenu()
{

//Main Loop
    while(true)
    {
        while(SDL_PollEvent(&Event))
        {
            if(Event.type == SDL_QUIT)
            {
                return;
            }
            if(Event.type == SDL_KEYDOWN)
            {


                if(Event.key.keysym.sym == SDLK_RETURN)
                    loopJuego();
            }
        }

        SDL_RenderCopy(renderer, background, NULL, &rect_background);
        SDL_RenderPresent(renderer);
    }

}


int main( int argc, char* args[] )
{

    //Init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        return 10;
    }
    //Creates a SDL Window
    if((window = SDL_CreateWindow("Image Loading", 100, 100, 500/*WIDTH*/, 250/*HEIGHT*/, SDL_WINDOW_RESIZABLE | SDL_RENDERER_PRESENTVSYNC)) == NULL)
    {
        return 20;
    }
    //SDL Renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED );
    if (renderer == NULL)
    {
        std::cout << SDL_GetError() << std::endl;
        return 30;
    }

    //Init textures
    int w=0,h=0;
    background = IMG_LoadTexture(renderer,"assets/menu.png");
    game_over = IMG_LoadTexture(renderer,"assets/game_over.png");
    SDL_QueryTexture(background, NULL, NULL, &w, &h);
    rect_background.x = 0;
    rect_background.y = 0;
    rect_background.w = w;
    rect_background.h = h;


    game_background = IMG_LoadTexture(renderer, "assets/background.png");
    pelotita_buena = IMG_LoadTexture(renderer, "assets/pelotita_buena.png");
    pelotita_mala = IMG_LoadTexture(renderer, "assets/pelotita_mala.png");
    SDL_QueryTexture(pelotita_buena, NULL, NULL, &rect_pelotita.w, &rect_pelotita.h);
    rect_pelotita.x = 0;
    rect_pelotita.y = 0;


    loopMenu();

    return 0;
}
