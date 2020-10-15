/**
 *  Base struct and functions for creating and manipulating node patches.
 *
 *  alexander kon-i ho <koniho@gmail.com>
 */
#ifndef _PATCHES_H_
#define _PATCHES_H_

#include <inttypes.h>
#include <stdio.h>
#include "mstimer.h"

// number of persistent data bytes for each patch
#define THM_PATCH_NUM_DATA          8

#define THM_PATCH_STATE_UNKNOWN     255
#define THM_PATCH_STATE_ENTER       0
#define THM_PATCH_STATE_RUNNING     1
#define THM_PATCH_STATE_EXIT        2

#define THM_PATCH_VALID_ID         1
#define THM_PATCH_INVALID_ID       0

// It is recommended that each patch uses a progress counter
// to report to the Game Logic. 
// #define THM_PATCH_PROGRESS_INDEX    0
// #define THM_PATCH_PARAM_TEMPO       1

typedef struct thm_patch_st {
    void (*init)(void);
    void (*enter)(void);
    void (*update)( int16_t* qt_deltas );
    void (*exit)(void);
    char    id; // Every patch must implement it's ID
} thm_patch_t;

typedef struct thm_cpatch_st {
    void (*init)(void);
    void (*enter)(void);
    void (*update)( int16_t* qt_deltas );
    void (*deactivate)(void);   // only used by control patches (what is it?)
    void (*activate)(int16_t* params);   // only used by control patches (what is it?)
    void (*exit)(void);
    char id;
    int16_t data[THM_PATCH_NUM_DATA];
} thm_cpatch_t;

int16_t patch_data[THM_PATCH_NUM_DATA];
int16_t cpatch_data[THM_PATCH_NUM_DATA];

// A collection of pointers to the available patches.
thm_patch_t*  current_patch;
uint8_t       current_patch_index;

thm_cpatch_t* current_cpatch;
uint8_t       current_cpatch_index;

// Any patch may use this timer for setting the update interval
mstimer_t    patch_timer;
mstimer_t    cpatch_timer;

// Control Patches may use these timers to schedule stuff that looks cool.
mstimer_t    sol1_timer;
mstimer_t    sol2_timer;

void     activateCurrentControlPatch( uint16_t val ); // ??
void     deactivateCurrentControlPatch( void );

#endif
