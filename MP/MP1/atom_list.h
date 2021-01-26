/* atom_list.h 
 * Prof. Calhoun 
 * ECE 2230 Spring 2021
 * MP1
 *
 * Purpose: The interface definition for MP1.  DO NOT CHANGE THIS FILE
 *
 * Assumptions: structure definitions and public functions as defined for
 * assignmnet.  There are no extensions.
 *
 * Bugs:
 */

#define NDIM 2

struct atom_t {
    unsigned int atom_id;       // Unique atom id
	unsigned int atomic_num;    // Atomic number for atom
	float mass;			        // Atom's mass
	
    float position[NDIM];       // Atom position
	float momenta[NDIM];        // Atom momenta vector
    float force[NDIM];          // Forces
	float potential_energy;     // Potential energy per atom
};


// Header block for the list
struct atom_list_t {
    int atom_count;             // current number of records in list
    int atom_size;              // size of the list
    struct atom_t **atom_ptr;
};

//  The sequential list ADT must have the following interface:

struct atom_list_t *atom_list_construct(int size);
void atom_list_destruct(struct atom_list_t *);
int atom_list_number_entries(struct atom_list_t *);
int atom_list_add(struct atom_list_t *, struct atom_t *);
struct atom_t *atom_list_access(struct atom_list_t *, int index);
struct atom_t *atom_list_remove(struct atom_list_t *, int index);

void atom_list_advance_momenta(struct atom_list_t *, float dt);
void atom_list_advance_position(struct atom_list_t *, float dt);
void atom_list_compute_forces(struct atom_list_t *);

int atom_list_lookup_max_potential_energy(struct atom_list_t *,
        float potential_energy);
int atom_list_determine_inside_box(struct atom_t *, float x_min, float x_max,
        float y_min, float y_max);
struct atom_list_t *atom_list_form_migrate_list(struct atom_list_t *,
        float x_min, float x_max, float y_min, float y_max);



