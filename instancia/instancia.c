#include "entry_table.h"
#include "instancia.h"
#include "storage.h"

void instance_set(key_value_t* key_value) {
	int next_entry = entry_table_next_entry(key_value);

	entry_table_insert(next_entry, key_value);

	storage_set(next_entry, key_value->value, key_value->size);


}

int instance_entries_used() {
	return get_total_entries() - entries_left;
}
