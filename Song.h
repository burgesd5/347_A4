#pragma once
#define FIELD_BUFFER_SIZE 16

typedef struct
{
    char name[FIELD_BUFFER_SIZE];
    char artist[FIELD_BUFFER_SIZE];
    char length[FIELD_BUFFER_SIZE];
}Song;