#include "board.h"
#include "display.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define CONTINUE_PLAY 0
#define NEXT_LEVEL 1
#define QUIT_GAME 2
#define LOAD_BACKUP 3
#define CREATE_BACKUP 4

int thread_shutdown = 0;

void screen_refresh(board_t * game_board, int mode) {
    debug("REFRESH\n");
    draw_board(game_board, mode);
    refresh_screen();
    if(game_board->tempo != 0)
        sleep_ms(game_board->tempo);       
}

void* ncurses_thread(void* arg) {
    board_t* game_board = (board_t*)arg;
    // Initial delay before starting the game loop
    sleep_ms(game_board->tempo / 2); 

    while(true) {
        sleep_ms(game_board->tempo); // Refresh the screen at half the temp

        if(thread_shutdown) {
            return NULL;
        }
    
        screen_refresh(game_board, DRAW_MENU); 
    }
    return NULL;
}

void* pacman_thread(void* arg) {
    board_t* game_board = (board_t*)arg;

    pacman_t* pacman = &game_board->pacmans[0];
    
    int *retval = malloc(sizeof(int));

    while(true){
        if(!pacman->alive) {
            *retval = QUIT_GAME;
            return (void *) retval;
        }

        // Wait for the duration of each play
        sleep_ms(game_board->tempo * (1 + pacman->passo)); 

        command_t* play;
        command_t c;
        if (pacman->n_moves == 0) { // if is user input
            c.command = get_input();

            if(c.command == '\0') {
                continue; // No input received, continue to the next iteration
            }

            c.turns = 1;
            play = &c;
        }
        else{ // else if the moves are pre-defined in the file
            // avoid buffer overflow wrapping around with modulo of n_moves
            // this ensures that we always access a valid move for the pacman
            play = &pacman->moves[pacman->current_move % pacman->n_moves];
        }

        debug("KEY %c\n", play->command);

        // QUIT
        if (play->command == 'Q') {
            *retval = QUIT_GAME;
            return (void *) retval;
        }

        int result = move_pacman(game_board, 0, play);
        if (result == REACHED_PORTAL) {
            // Next level
            *retval = NEXT_LEVEL;
            return (void *) retval;
        }

        if(result == DEAD_PACMAN) {
            *retval = QUIT_GAME;
            return (void *) retval;
        }
    }
    return (void *) retval;
}

int main(void) {
    // Random seed for any random movements
    srand((unsigned int)time(NULL));

    open_debug_file("debug.log");

    terminal_init();
    
    int accumulated_points = 0;
    bool end_game = false;
    board_t game_board;

    while (!end_game) {
        load_level(&game_board, accumulated_points);
        draw_board(&game_board, DRAW_MENU);
        refresh_screen();

        while(true) {
            pthread_t ncurses_tid, pacman_tid;

            thread_shutdown = 0; // Signal the ncurses thread to shut down

            debug("Creating threads\n");

            pthread_create(&pacman_tid, NULL, pacman_thread, (void*)&game_board);

            // TODO: Create a thread for each ghost.

            pthread_create(&ncurses_tid, NULL, ncurses_thread, (void*)&game_board);

            int *retval;
            pthread_join(pacman_tid, (void**)&retval);

            thread_shutdown = 1; // Signal the ncurses thread to shut down
            pthread_join(ncurses_tid, NULL);

            int result = *retval;
            free(retval);

            if(result == NEXT_LEVEL) {
                screen_refresh(&game_board, DRAW_WIN);
                sleep_ms(game_board.tempo);
                break;
            }

            if(result == QUIT_GAME) {
                screen_refresh(&game_board, DRAW_GAME_OVER); 
                sleep_ms(game_board.tempo);
                end_game = true;
                break;
            }
    
            screen_refresh(&game_board, DRAW_MENU); 

            accumulated_points = game_board.pacmans[0].points;      
        }
        print_board(&game_board);
        unload_level(&game_board);
    }    

    terminal_cleanup();

    close_debug_file();

    return 0;
}
