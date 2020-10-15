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
#define THM_PATCH_NUM_PATCHES       2

#define THM_PATCH_STATE_UNKNOWN     255
#define THM_PATCH_STATE_ENTER       0
#define THM_PATCH_STATE_RUNNING     1
#define THM_PATCH_STATE_EXIT        2

#define THM_PATCH_VALID_NUM         1
#define THM_PATCH_INVALID_NUM       0

// It is recommended that each patch uses a progress counter
// to report to the Game Logic. 
// #define THM_PATCH_PROGRESS_INDEX    0
// #define THM_PATCH_PARAM_TEMPO       1

typedef struct thm_patch_st {
    void (*init)(void);
    void (*enter)(void);
    void (*update)( int16_t* qt_deltas );
    void (*exit)(void);
    uint8_t  state; // probably unneccessary
    uint16_t progress;
    uint16_t tempo;  // Used to define the timer interval?
    int16_t data[THM_PATCH_NUM_DATA];
} thm_patch_t;

// A collection of pointers to the available patches.
thm_patch_t* current_patch;
uint8_t      current_patch_index;

// Any patch may use this timer
mstimer_t    patch_timer;

uint8_t  isValidPatchNum( uint8_t patch_num );
void     setCurrentPatch( uint8_t patch_num );

uint16_t getCurrentPatchData( uint8_t data_index );
void     setCurrentPatchData( uint8_t data_index, uint16_t val );

uint16_t getPatchData( thm_patch_t* patch, uint8_t data_index );
void     setPatchData( thm_patch_t* patch, uint8_t data_index, uint16_t val );

#endif
