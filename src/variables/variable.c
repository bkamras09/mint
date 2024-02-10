// Defines the HashTable item.

typedef struct Ht_item
{
  char* key;
  char* value;
} Ht_item;

// Defines the HashTable.
typedef struct HashTable
{
  // Contains an array of pointers to items.
  Ht_item** items;
  int size;
  int count;
} HashTable;

Ht_item* create_item(char* key, char* value)
{
  // Creates a pointer to a new HashTable item.
  Ht_item* item = (Ht_item*) malloc(sizeof(Ht_item));
  item->key = (char*) malloc(strlen(key) + 1);
  item->value = (char*) malloc(strlen(value) + 1);
  strcpy(item->key, key);
  strcpy(item->value, value);
  return item;
}

HashTable* create_table(int size)
{
  // Creates a new HashTable.
  HashTable* table = (HashTable*) malloc(sizeof(HashTable));
  table->size = size;
  table->count = 0;
  table->items = (Ht_item**) calloc(table->size, sizeof(Ht_item*));

  for (int i = 0; i < table->size; i++)
    table->items[i] = NULL;

  return table;
}

#define CAPACITY 50000 // Size of the HashTable.

unsigned long hash_function(char* str)
{
  unsigned long i = 0;

  for (int j = 0; str[j]; j++)
    i += str[j];

  return i % CAPACITY;
}

void free_item(Ht_item* item)
{
  // Frees an item.
  free(item->key);
  free(item->value);
  free(item);
}

void free_table(HashTable* table)
{
  // Frees the table.
  for (int i = 0; i < table->size; i++)
    {
      Ht_item* item = table->items[i];

      if (item != NULL)
        free_item(item);
    }

  free(table->items);
  free(table);
}

void print_table(HashTable* table)
{
  printf("\nHash Table\n-------------------\n");

  for (int i = 0; i < table->size; i++)
    {
      if (table->items[i])
        {
          printf("Index:%d, Key:%s, Value:%s\n", i, table->items[i] -> key, table->items[i]->value);
        }
    }

  printf("-------------------\n\n");
}
