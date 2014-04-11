#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#define DEFAULT_SPEED   115200
#define DEFAULT_HOST    "localhost"
#define MAX_ID          255

extern int speed;
extern char *host;

/* Load ports list from config file. */
int loadports(const char *filename);

/* Return port number associated with provides id.
 * If no port is associated with this id, return 0.
 */
int getportbyid(int id);

#endif // _SETTINGS_H_
