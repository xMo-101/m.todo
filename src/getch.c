#ifdef __unix__

#include <termios.h>
#include <unistd.h>
#include <stdio.h>

/**
 * @brief Reads a single character from the standard input without echoing it.
 * 
 * This function disables canonical mode and echo mode of the terminal settings,
 * allowing the program to read a single character without waiting for a newline
 * and without displaying the character on the screen.
 * 
 * @return The character read from the standard input.
 */
int getch(void) {
    struct termios old_settings, new_settings;
    tcgetattr(STDIN_FILENO, &old_settings); // Get the current terminal settings
    new_settings = old_settings; // Copy the old settings to the new settings
    new_settings.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo mode
    tcsetattr(STDIN_FILENO, TCSANOW, &new_settings); // Set the new settings
    int ch = getchar(); // Read a single character
    tcsetattr(STDIN_FILENO, TCSANOW, &old_settings); // Restore the old settings
    return ch;
}

#endif 