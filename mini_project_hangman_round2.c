#include <SDL3/SDL.h>
#include <stdio.h> // Needed strictly for EOF and standard I/O constants used by OS terminal layers

// Include your custom string library
#include "bent1.c"

#ifdef _WIN32
    #include <conio.h>
    const int KEY_ENTER = 13;
    const int KEY_BACKSPACE = 8;
    void clear_screen() { SDL_Log("\033[2J\033[H"); } // ANSI clear screen
    int get_char_hidden() {
        return _getch();
    }
#endif
#ifdef __linux__
    #include <termios.h>
    #include <unistd.h>
    const unsigned char KEY_ENTER = '\n';
    const int KEY_BACKSPACE = 127;

    void clear_screen(void) { 
    for (int i = 0; i < 100; ++i)
    {
            printf("\n"); 
    }
    } // ANSI clear screen

    int get_char_hidden(void) {
        struct termios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO); 
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
#endif

// Returns a dynamically allocated b_str_t encoded as UTF-8
b_str_t get_secret_word(void) {
    char temp_word[256];
    int index = 0;
    int ch;
    
    SDL_Log("Enter the Secret Word (recommended [6-9] letters) >> ");

    for (;;) {
        ch = get_char_hidden(); 

        if (ch == EOF) exit(0);

        if (ch == KEY_ENTER) { 
            if (index > 0) {
                temp_word[index] = '\0';
                break; 
            }
        }
        else if (ch == KEY_BACKSPACE) {
            if (index > 0) {
                index--; 
                SDL_Log("Letter removed. Current length: %d", index);
            }
        }
        else {
            if (index < 255) {
                temp_word[index++] = (char)ch;
                SDL_Log("Letter added. Current length: %d", index);
            }
        }
    }
    
    // Create and return a new DYNAMIC utf-8 encoded string using your library
    return b_str_new_pro(temp_word, index, B_STR_ENC_UTF8);
}

void draw_hangman(int lives) {
    // ASCII art left untouched, formatted as single logs to avoid log prefixes per-line
    switch (lives) {
        case 6:
            SDL_Log(
                "  +---------------+\n"
                "  |               |\n"
                "  |                \n"
                "  |                \n"
                "  |                \n"
                "  |                \n"
                "  |                \n"
                "  |                \n"
                "  |                \n"
                "  |                \n"
                "  |                \n"
                "  |                \n"
                "  |                \n"
                "  |                \n"
                "  |                \n"
                "  |                \n"
                "  ======================="
            );
            break;
        case 5:
            SDL_Log(
                "  +---------------+ \n"
                "  |               | \n"
                "  |            _______ \n"
                "  |           |       | \n"
                "  |           |       | \n"
                "  |           |_______| \n"
                "  | \n"
                "  | \n"
                "  | \n"
                "  | \n"
                "  | \n"
                "  | \n"
                "  | \n"
                "  | \n"
                "  | \n"
                "  | \n"
                "  ======================="
            );
            break;
        case 4:
            SDL_Log(
                "  +---------------+\n"
                "  |               |\n"
                "  |            _______ \n"
                "  |           |       | \n"
                "  |           |       | \n"
                "  |           |_______| \n"
                "  |               | \n"
                "  |               | \n"
                "  |               | \n"
                "  |               | \n"
                "  |               | \n"
                "  | \n"
                "  | \n"
                "  | \n"
                "  | \n"
                "  | \n"
                "  ======================="
            );
            break;
        case 3:
            SDL_Log(
                "  +---------------+\n"
                "  |               |\n"
                "  |            _______ \n"
                "  |           |       | \n"
                "  |           |       | \n"
                "  |           |_______| \n"
                "  |               | \n"
                "  |           ____| \n"
                "  |          /    | \n"
                "  |         /     | \n"
                "  |               | \n"
                "  | \n"
                "  | \n"
                "  | \n"
                "  | \n"
                "  | \n"
                "  ======================="
            );
            break;
        case 2:
            SDL_Log(
                "  +---------------+\n"
                "  |               |\n"
                "  |            _______ \n"
                "  |           |       | \n"
                "  |           |       | \n"
                "  |           |_______| \n"
                "  |               | \n"
                "  |           ____|____ \n"
                "  |          /    |    \\ \n"
                "  |         /     |     \\ \n"
                "  |               | \n"
                "  | \n"
                "  | \n"
                "  | \n"
                "  | \n"
                "  | \n"
                "  ======================="
            );
            break;
        case 1:
            SDL_Log(
                "  +---------------+\n"
                "  |               |\n"
                "  |            _______ \n"
                "  |           |       | \n"
                "  |           |       | \n"
                "  |           |_______| \n"
                "  |               | \n"
                "  |           ____|____ \n"
                "  |          /    |    \\ \n"
                "  |         /     |     \\ \n"
                "  |               | \n"
                "  |              / \n"
                "  |             / \n"
                "  |            / \n"
                "  | \n"
                "  | \n"
                "  ======================="
            );
            break;
        case 0:
            SDL_Log(
                "  +---------------+\n"
                "  |               |\n"
                "  |            _______ \n"
                "  |           |       | \n"
                "  |           |  x x  | \n"
                "  |           |_______| \n"
                "  |               | \n"
                "  |           ____|____ \n"
                "  |          /    |    \\ \n"
                "  |         /     |     \\ \n"
                "  |               | \n"
                "  |              / \\ \n"
                "  |             /   \\ \n"
                "  |            /     \\ \n"
                "  | \n"
                "  | \n"
                "  ======================="
            );
            break;
    }
}

int main(void) {
    clear_screen();
    
    b_str_t secret_word = get_secret_word();
    size_t byte_length = b_str_len(secret_word);
    
    // Allocate using SDL wrappers
    char* temp_guess = (char*)SDL_malloc(byte_length + 1);
    if (!temp_guess) return 1;
    
    SDL_memset(temp_guess, '_', byte_length);
    temp_guess[byte_length] = '\0';
    
    b_str_t guess_word = b_str_new_pro(temp_guess, byte_length, B_STR_ENC_UTF8);
    SDL_free(temp_guess); // Free using SDL wrapper

    b_str_t guessed_letters = b_str_new_pro("", 0, B_STR_ENC_UTF8);

    int lives = 6;
    char message[256];
    SDL_strlcpy(message, "Welcome to Hangman!", sizeof(message));

    while (lives > 0) {
        clear_screen();
        draw_hangman(lives);
        
        SDL_Log("GUESS THE WORD: %.*s", (int)b_str_len(guess_word), (const char*)guess_word);
        SDL_Log("Guessed Letters: %.*s", (int)b_str_len(guessed_letters), (const char*)guessed_letters);
        SDL_Log("Message: %s", message);
        
        // SDL wrapper for memcmp
        if (SDL_memcmp(secret_word, guess_word, byte_length) == 0) {
            break;
        }
        
        SDL_Log("Enter a letter: ");
        int raw_ch = get_char_hidden();
        
        char input = (char)SDL_toupper(raw_ch);

        // SDL wrappers for isalpha and strchr
        if (!SDL_isalpha(input)) {
            SDL_snprintf(message, sizeof(message), "ERROR: '%c' is not a letter!", input);
        }
        else if (SDL_strchr((const char*)guessed_letters, input)) {
            SDL_snprintf(message, sizeof(message), "WARNING: You already guessed '%c'.", input);
        }
        else {
            size_t curr_len = b_str_len(guessed_letters);
            b_str_t new_guessed = b_str_new_pro(NULL, curr_len + 1, B_STR_ENC_UTF8);
            
            // SDL wrapper for memcpy
            SDL_memcpy(new_guessed, guessed_letters, curr_len);
            new_guessed[curr_len] = input;
            
            b_str_free(guessed_letters);
            guessed_letters = new_guessed;

            int found = 0;
            for (size_t i = 0; i < byte_length; i++) {
                if (SDL_toupper(secret_word[i]) == input) {
                    guess_word[i] = secret_word[i]; 
                    found = 1;
                }
            }

            if (found) {
                SDL_snprintf(message, sizeof(message), "Good job! Found '%c'.", input);
            } else {
                lives--;
                SDL_snprintf(message, sizeof(message), "Sorry, '%c' is not in the word.", input);
            }
        }
    }

    clear_screen();
    draw_hangman(lives);

    if (lives > 0) {
        SDL_Log("WORD: %.*s\nYOU WIN! The word was: %.*s", 
               (int)b_str_len(guess_word), (const char*)guess_word, 
               (int)byte_length, (const char*)secret_word);
               
        SDL_Log(
            "W     W  IIIII  N   N  N   N  EEEEE  RRRR  \n"
            "W     W    I    NN  N  NN  N  E      R   R \n"
            "W  W  W    I    N N N  N N N  EEEEE  RRRR  \n"
            " W W W     I    N  NN  N  NN  E      R  R  \n"
            "  W W    IIIII  N   N  N   N  EEEEE  R   R "
        );
    } else {
        SDL_Log("GAME OVER! The word was: %.*s", (int)byte_length, (const char*)secret_word);
    }
    
    b_str_free(secret_word);
    b_str_free(guess_word);
    b_str_free(guessed_letters);
    
    return 0;
}