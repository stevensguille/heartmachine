#ifndef _PATCH_COLLECTION_H_
#define _PATCH_COLLECTION_H_

#include "patches.h"

#define ATTRACTOR_PATCH_NUM     0
#define PULSE_PATCH_NUM         1
#define WIPE_PATCH_NUM          2
#define UNKNOWN_PATCH_NUM       3

thm_patch_t* patch_collection[ THM_PATCH_NUM_PATCHES ];
uint8_t      patchCollection_num_patches;

thm_patch_t* current_patch;
uint8_t      current_patch_index;

void         registerPatches( void );
void         addPatch( thm_patch_t* new_patch );
void         initializeAllPatches( void );
uint8_t      isValidPatchNum( uint8_t patch_num );
void         setCurrentPatch( uint8_t patch_num );

#endif
