/*
 * Copyright (c) 1993 Brooke Paul
 *
 * $Id: editor.h,v 1.1 2001/08/14 23:13:08 develop Exp develop $
 *
 * $Log: editor.h,v $
 * Revision 1.1  2001/08/14 23:13:08  develop
 * Initial revision
 *
/* prototypes for editor.c */
extern void edit_object();
extern void edit_monster();
extern void edit_room();
extern void edit_exits(room *rom_ptr);
extern void getnum(int *i, int min, int max);
extern void getnums(long *i, long orig, long min, long max);
extern void getstr(char *str, char *origstr, int maxlen);
extern void convert_to_csv();
extern void convert_from_csv();
extern int save_creature(creature *crt_ptr, int index);
extern int save_object(object *obj_ptr, int index);
extern int save_room(room *rom_ptr, int index);

/* prototypes for editor2.c */
extern void edit_player();
extern void edit_items(creature *ply_ptr);

