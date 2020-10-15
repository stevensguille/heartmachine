#include <inttypes.h>
#include "patch_collection.h"
#include "patches.h"

#include "attractor_patch.h"
#include "pulse_patch.h"
#include "wipe_patch.h"

/**
 *  Call this function to register all the patches
 *  (must be explicitly added to the function below)
 *  Also, the order must match the ordering in the
 *  patch_collection.h header
 */
void registerPatches() {
    printf("Registering Patch: Attactor\n\r");
    addPatch( &attractor_patch );

    printf("Registering Patch: Pulse\n\r");
    addPatch( &pulse_patch );
    printf("Registering Patch: Wipe\n\r");
    addPatch( &wipe_patch );
}


/**
 *  Add a patch to the patch collection
 */
void addPatch( thm_patch_t* new_patch ) {
    printf( "Adding Patch: %d\n\r", patchCollection_num_patches );
    //patch_collection[ patchCollection_num_patches++ ] = new_patch;
}


/**
 *  For each available patch, call it's init function.
 */
void initializeAllPatches() {
    uint8_t patch_num;
    for ( patch_num = 0; patch_num < patchCollection_num_patches; patch_num++ )
        patch_collection[ patch_num ]->init();
}


/**
 *  Checks if patch_number falls in range of the available patche numbers.
 */
uint8_t isValidPatchNum( uint8_t patch_num ) {
    if ( patch_num < patchCollection_num_patches )
       return THM_PATCH_VALID_NUM;
    else
       return THM_PATCH_INVALID_NUM;
}


/**
 * Run the current patch's exit function and switch to the new patch.
 * Sets the current_patch to a new patch  
 */
void setCurrentPatch( uint8_t patch_num ) {
    // Run the exit function for the current patch
//    if ( current_patch != NULL && current_patch->exit )
//        current_patch->exit();

    // Switch patch and run it's enter function.
    current_patch = patch_collection[ patch_num ];

    current_patch->enter();
    current_patch->state = THM_PATCH_STATE_RUNNING;

    current_patch_index = patch_num;
}

