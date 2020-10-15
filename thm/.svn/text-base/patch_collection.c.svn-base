#include <inttypes.h>
#include <avr/pgmspace.h>
#include "patch_collection.h"
#include "patches.h"

//#include "attractor_patch.h"
#include "pulse_patch.h"
// #include "sequence_patch.h"
#include "sine_patch.h"
#include "sparkle_patch.h"
#include "solenoid_patch.h"
#include "simple_solenoid_cpatch.h"
#include "heartbeat_cpatch.h"
#include "flame_cpatch.h"
#include "patches.h"
#include "patch_id.h"

/**
 *  Utility function to obtain the index of a patch given
 *  a 'patch_id' char.
 */
static int8_t getPatchIndex( char patch_id ) {
    int i;

    if (patch_id == PATCH_ID_NULL)
        return 0;

    // First patch is NULL patch
    for( i=1; i < THM_NUM_PATCHES+1; i++ ) {
        if( patch_collection[i] == NULL ) {
            return PATCH_INVALID_ID;
        } else { 
            if( patch_collection[i]->id == patch_id ) {
                return i;
            }
        }
    }

    return PATCH_INVALID_ID;
}

/**
 *  Utility function to obtain the index of a cpatch given
 *  a 'patch_id' char.
 */
static int8_t getCPatchIndex( char cpatch_id ) {
    int i;

    if (cpatch_id == CPATCH_ID_NULL)
        return 0;

    // First patch is NULL patch
    for( i=1; i < THM_NUM_CPATCHES+1; i++ ) {
        if( cpatch_collection[i] == NULL ) {
            return PATCH_INVALID_ID;
        } else {
            if( cpatch_collection[i]->id == cpatch_id ) {
                return i;
            }
        }
    }

    return PATCH_INVALID_ID;
}

int8_t isValidPatchID( char patch_id ) {
    return ( getPatchIndex( patch_id ) == PATCH_INVALID_ID ? 
           PATCH_INVALID_ID : PATCH_VALID_ID );
}

int8_t isValidCPatchID( char patch_id ) {
    return ( getCPatchIndex( patch_id ) == PATCH_INVALID_ID ? 
           PATCH_INVALID_ID : PATCH_VALID_ID );
}

/**
 *  Call this function to register all the patches
 *  (must be explicitly added to the function below)
 *  Also, the order must match the ordering in the
 *  patch_collection.h header
 */
void registerPatches() {

	num_patches = 0;
//    printf_P(PSTR("Registering Patch: Attractor\n\r"));
//    patch_collection[ patchCollection_num_patches++ ] = &attractor_patch;
//    printf_P(PSTR("Registering Patch: Pulse\n\r"));
//    patch_collection[ patchCollection_num_patches++ ] = &pulse_patch;
//    printf_P(PSTR("Registering Patch: Wipe\n\r"));
//    patch_collection[ patchCollection_num_patches++ ] = &wipe_patch;
//    printf_P(PSTR("Registering Patch: Sequence\n\r"));
//    patch_collection[ patchCollection_num_patches++ ] = &sequence_patch;
//
    patch_collection[ num_patches++ ] = NULL;

    printf_P(PSTR("Registering Patch: Sine\n\r"));
    if ( sine_patch.id == '\0' )
        printf_P( PSTR("Error registering patch with ID: %c\n\r"));

    patch_collection[ num_patches++ ] = &sine_patch;
    patch_collection[ num_patches++ ] = &pulse_patch;
    patch_collection[ num_patches++ ] = &sparkle_patch;
    
    // CONTROL PATCHES
    //
	num_cpatches = 0;
//    printf_P(PSTR("Registering Patch: Solenoid\n\r"));
//    patch_control_collection[ patchCollection_control_num_patches++ ] = &solenoid_patch;
    printf_P(PSTR("Registering Patch: Simple Solenoid\n\r"));
    cpatch_collection[ num_cpatches++ ] = NULL;
    cpatch_collection[ num_cpatches++ ] = &solenoid_cpatch;
    cpatch_collection[ num_cpatches++ ] = &heartbeat_patch;
//    cpatch_collection[ num_cpatches++ ] = &flame_patch;

}

/**
 *  Add a patch to the patch collection
 */
void addPatch( thm_patch_t* new_patch ) {
    patch_collection[ num_patches++ ] = new_patch;
}

/**
 *  Add a patch to the patch collection
 */
void addCPatch( thm_cpatch_t* new_patch ) {
    cpatch_collection[ num_cpatches++ ] = new_patch;
}

/**
 *  For each available patch, call it's init function.
 */
void initializeAllPatches() {
    uint8_t patch_num, patch_control_num;

    //for ( patch_num = 0; patch_num < patchCollection_num_patches; patch_num++ )
    patch_num = 0;

//    attractor_patch.init();
//    pulse_patch.init();
//    wipe_patch.init();
//    sequence_patch.init();
    sine_patch.init();
    //simple_solenoid_patch.init();
    pulse_patch.init();
    //flame_patch.init();
    //solenoid_patch.init();

    patch_control_num = 0;
}


/**
 * Run the current patch's exit function and switch to the new patch.
 * Sets the current_patch to a new patch  
 */
void setCurrentPatch( char patch_id ) {
    uint8_t new_patch_index = getPatchIndex( patch_id );

    // Don't switch the patch if the target patch doesn't exist
    if ( new_patch_index == PATCH_INVALID_ID )
        return;

    // Run the exit function for the current patch
    if ( current_patch != NULL && current_patch->exit != NULL )
        current_patch->exit();

    // Switch patch and run its enter function.
    current_patch = patch_collection[ new_patch_index ];

    if ( current_patch != NULL )
        current_patch->enter();

    current_patch_index = new_patch_index;
}

/**
 * Run the current control patch's exit function and switch to the new patch.
 * Sets the current_control_patch to a new patch
 */
void setCurrentCPatch( char patch_id ) {
    uint8_t new_patch_index = getCPatchIndex( patch_id );

    // Don't switch the patch if the target patch doesn't exist
    if ( new_patch_index == PATCH_INVALID_ID )
        return;

    // Run the exit function for the current patch
    if ( current_cpatch != NULL && current_cpatch->exit != NULL )
        current_cpatch->exit();

    // Switch patch and run its enter function.
    current_cpatch = cpatch_collection[ new_patch_index ];

    if ( current_cpatch != NULL )
        current_cpatch->enter();

    current_cpatch_index = new_patch_index;
}

