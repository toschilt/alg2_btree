#ifndef __PUBLICINTERFACE_H__
#define __PUBLICINTERFACE_H__

#include "student.h"
#include "btree.h"
#include "streamHandler.h"


void search(int key);
void insert(studentRegister *student);
void update(studentRegister *student);


#endif