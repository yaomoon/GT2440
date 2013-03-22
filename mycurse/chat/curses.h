
#define debug
extern FILE *moon_log;
void init_curses(void);
void refresh_state(void);
void fill_state(char *name, char *value);
void draw_bottom(void);
void tablet_str(char *str);
void fill_scene(char *line);
void moon_test(char *line);
