#ifndef _T_EDITOR_
#define _T_EDITOR_

struct Controller;
void init(struct Controller* idxer);
void save(struct Controller* idxer);
int confirm_exit(struct Controller* idxer);
int open_file(struct Controller* idxer);

#endif