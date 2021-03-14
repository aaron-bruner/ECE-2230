/* datatypes.h 
 * Prof. Calhoun 
 * jonccal
 * ECE 2230 Spring 2021
 * MP3
 *
 * Propose: The data type that is stored in the list ADT is defined here.  We
 *          define a single mapping that allows the list ADT to be defined in
 *          terms of a generic data_t.
 *
 * Assumptions: 
 *
 *     data_t: The type of data that we want to store in the list
 *
 * Bugs:
 * 
 */
#define NDIM 2

typedef struct atom_info {
    unsigned int atom_id;       // Unique atom id
	unsigned int atomic_num;    // Atomic number for atom
	float mass;			        // Atom's mass
	
    float position[NDIM];       // Atom position
	float momenta[NDIM];        // Atom momenta vector
    float force[NDIM];          // Forces
	float potential_energy;     // Potential energy per atom
} atom_t;

/* the list ADT works on atom data of this type */
typedef atom_t data_t;


/* commands for vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */
