#ifndef _PATCH_COLLECTION_H_
#define _PATCH_COLLECTION_H_

#include "patches.h"

#define PATCH_VALID_ID            (1)
#define PATCH_INVALID_ID          (-1)

#define THM_NUM_PATCHES   	      6
#define THM_NUM_CPATCHES          6

// +1 for NULL patch
thm_patch_t* patch_collection[ THM_NUM_PATCHES+1 ];
uint8_t      num_patches;

thm_cpatch_t* cpatch_collection[ THM_NUM_CPATCHES+1 ];
uint8_t      num_cpatches;

// ALL PATCHES
void         registerPatches( void );
void         initializeAllPatches( void );

// VISUAL PATCH
void         addPatch( thm_patch_t* new_patch );
int8_t      isValidPatchID( char patch_id );
void         setCurrentPatch( char patch_id );

// CONTROL PATCH
void         addCPatch( thm_cpatch_t* new_patch );
int8_t      isValidCPatchID( char cpatch_id );
void         setCurrentCPatch( char patch_id );

#endif
