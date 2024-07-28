int min(int a, int b);
int max(int a, int b);

/* sets back the terminal settings to the same as at the beginning */
void reset_input_mode();
/* puts the terminal in noncanonical mode, also saves the previous settings for reset_input_mode and calls atextit(reset_input_mode) */
void set_noncanonical();
/* reads user input
 * return -1 to quit
 *        1-4 for directional input
 *        5 to select a tile
*/
int get_input();
