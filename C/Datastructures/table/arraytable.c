
#include <stdlib.h>
#include <stdio.h>

#include "table.h"
#include "array_1d.h"

/*
 * Implementation of a generic table for the "Datastructures and
 * algorithms" courses at the Department of Computing Science, Umea
 * University.
 *
 * Duplicates are handled by inspect and remove.
 *
 * Authors: Gustaf Söderlund (et14gsd@cs.umu.se)
 *
 * Based on earlier code by: Johan Eliasson (johane@cs.umu.se).
 *
 * Version information:
 *   2018-02-06: v1.0, first public version.
 *   2019-02-21: v1.1, second version without dlist/memfreehandler.
 *   2019-03-04: v1.2, bugfix in table_remove.
 */

// ===========INTERNAL DATA TYPES============

struct table {
	array_1d *entries;
	compare_function *key_cmp_func;
	free_function key_free_func;
	free_function value_free_func;
};

struct table_entry {
	void *key;
	void *value;
};
#define HIGH 80000
#define LOW 0
// ===========INTERNAL FUNCTION IMPLEMENTATIONS============

/**
 * table_empty() - Create an empty table.
 * @key_cmp_func: A pointer to a function to be used to compare keys.
 * @key_free_func: A pointer to a function (or NULL) to be called to
 *		   de-allocate memory for keys on remove/kill.
 * @value_free_func: A pointer to a function (or NULL) to be called to
 *		     de-allocate memory for values on remove/kill.
 *
 * Returns: Pointer to a new table.
 */
table *table_empty(compare_function *key_cmp_func,
		   free_function key_free_func,
		   free_function value_free_func)
{
	// Allocate the table header.
	table *t = calloc(1, sizeof(table));
	// Create the array to hold the table_entry-ies.
	t->entries = array_1d_create(LOW, HIGH, NULL);
	// Store the key compare function and key/value free functions.
	t->key_cmp_func = key_cmp_func;
	t->key_free_func = key_free_func;
	t->value_free_func = value_free_func;

	return t;
}

/**
 * table_is_empty() - Check if a table is empty.
 * @table: Table to check.
 *
 * Returns: True if table contains no key/value pairs, false otherwise.
 */
bool table_is_empty(const table *t)
{
	if(array_1d_has_value(t->entries,LOW)){
		return 0;
	}

	else {
		return true;
	}
}

/**
 * table_insert() - Add a key/value pair to a table.
 * @table: Table to manipulate.
 * @key: A pointer to the key value.
 * @value: A pointer to the value value.
 *
 * Insert the key/value pair into the table. test is performed to
 * check if key is a duplicate.
 *
 * Returns: Nothing.
 */
 void table_insert(table *t, void *key, void *value)
 {
	 // Allocate the key/value structure.
	 struct table_entry *entry = malloc(sizeof(struct table_entry));

	 // Set the pointers and insert first in the array. This will
	 // cause table_lookup() to find the latest added value.
	 entry->key = key;
	 entry->value = value;

	 int pos = LOW;

	 do{
		 // inspect the value/key pairs.
		 struct table_entry *entry2 = array_1d_inspect_value(t->entries, pos);

		 //Search for an empty position and set the value on the position.
		 if(!array_1d_has_value(t->entries, pos)){

			 array_1d_set_value(t->entries, entry, pos);

			 pos = HIGH;

		 }
		 //if the key or value is a duplicate free the first value/key.
		 else if(!t->key_cmp_func(entry2->key, key)){

			 if((entry2->key != key) && (t->key_free_func != NULL)){
				 //free the duplicate key
				 t->key_free_func(entry2->key);
			 }
			 if((entry2->value != value) && (t->value_free_func != NULL)){
				 //free the duplicate value
				 t->value_free_func(entry2->value);
			 }
			 free(entry2);

			 array_1d_set_value(t->entries, entry, pos);
			 pos = HIGH;
		 }

		 pos++;
	 }while(pos < HIGH);

 }



/**
 * table_lookup() - Look up a given key in a table.
 * @table: Table to inspect.
 * @key: Key to look up.
 *
 * Returns: The value corresponding to a given key, or NULL if the key
 * is not found in the table. If the table contains duplicate keys,
 * the value that was latest inserted will be returned.
 */
 void *table_lookup(const table *t, const void *key)
 {
	 int pos = LOW;
	 // Iterate over the list. Return first match.
	 while(array_1d_has_value(t->entries, pos)){
		 struct table_entry *entry = array_1d_inspect_value(t->entries, pos);
		 // Check if the entry key matches the search key.
		 if (t->key_cmp_func(entry->key, key) == 0) {
			 // If yes, return the corresponding value pointer.
			 return entry->value;
		 }
		 //Continue with next position
		 pos++;
	 }

	 // No match found. Return NULL.
	 return NULL;
 }

/**
 * table_choose_key() - Return an arbitrary key.
 * @t: Table to inspect.
 *
 * Return an arbitrary key stored in the table. Can be used together
 * with table_remove() to deconstruct the table. Undefined for an
 * empty table.
 *
 * Returns: An arbitrary key stored in the table.
 */
void *table_choose_key(const table *t)
{
	// Return first key value.
	struct table_entry *entry = array_1d_inspect_value(t->entries, LOW);

  return entry->key;
}

/*
 * table_remove() - Remove a key/value pair in the table.
 * @table: Table to manipulate.
 * @key: Key for which to remove pair.
 *
 * Any matching duplicates will be removed. Will call any free
 * functions set for keys/values. Does nothing if key is not found in
 * the table.
 *
 * Returns: Nothing.
 */

 void table_remove(table *t, const void *key)
 {

	 // Start at lowest index.
	 int pos = array_1d_low(t->entries);

	 // Iterate over the values/keys. Remove any entries with matching keys.
	 while (array_1d_has_value(t->entries, pos)) {
		 // Inspect the table entry
		 struct table_entry *entry = array_1d_inspect_value(t->entries, pos);

		 // Compare the supplied key with the key of this entry.
		 if (t->key_cmp_func(entry->key, key) == 0) {
			 // If we have a match, call free on the key
			 // and/or value if given the responsiblity
			 if (t->key_free_func != NULL) {
				 //Free
				 t->key_free_func(entry->key);
			 }
			 if (t->value_free_func != NULL) {
				 //free
				 t->value_free_func(entry->value);
			 }
			 // Remove the list element itself.
			 array_1d_set_value(t->entries, NULL, pos);
			 // Deallocate the table entry structure.
			 free(entry);
		 }

		 else {
			 // No match, move on to next element in the list.
			 pos++;

		 }
	 }
	 // iterate the value with index higher than pos
	 while(array_1d_has_value(t->entries, pos+1)) {

		 //inspect key/values
		 struct table_entry *entry = array_1d_inspect_value(t->entries, pos+1);
		 //Switch the value and key from higher index to the lower.
		 array_1d_set_value(t->entries, entry, pos);

		 pos++;
		 array_1d_set_value(t->entries, NULL, pos);
	 }
 }


/*
 * table_kill() - Destroy a table.
 * @table: Table to destroy.
 *
 * Return all dynamic memory used by the table and its elements. If a
 * free_func was registered for keys and/or values at table creation,
 * it is called each element to free any user-allocated memory
 * occupied by the element values.
 *
 * Returns: Nothing.
 */
 void table_kill(table *t)
 {
	 int pos = LOW;
	 // Iterate over the list. Destroy all elements.
	 while(array_1d_has_value(t->entries, pos)) {
		 // Inspect the key/value pair.
		 struct table_entry *entry = array_1d_inspect_value(t->entries, pos);
		 // Free key and/or value if given the authority to do so.
		 if (t->key_free_func != NULL) {
			 t->key_free_func(entry->key);
		 }
		 if (t->value_free_func != NULL) {
			 t->value_free_func(entry->value);
		 }
		 // Move on to next element
		 pos++;
		 // Deallocate the table entry structure.
		 free(entry);

	 }

	 // Kill what's left of the list...
	 array_1d_kill(t->entries);
	 // ...and the table.
	 free(t);
 }

/**
 * table_print() - Print the given table.
 * @t: Table to print.
 * @print_func: Function called for each key/value pair in the table.
 *
 * Iterates over the key/value pairs in the table and prints them.
 * Will print all stored elements, including duplicates.
 *
 * Returns: Nothing.
 */
void table_print(const table *t, inspect_callback_pair print_func)
{
	// Iterate over all elements. Call print_func on keys/values.
for (int i = LOW; i <= HIGH; i++) {


		struct table_entry *e = array_1d_inspect_value(t->entries, i);
		// Call print_func
		print_func(e->key, e->value);
	}
}
