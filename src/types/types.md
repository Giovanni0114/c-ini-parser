# c-ini-parser types

## Main structure types
Config -> Section -> Line

### Config
    A container of `Sections`, have display name and uuid.

This struct implements interface `c-ini-parser-linked-list` as described below.

### Section
    A container of `Lines`, have display name and uuid.

This struct implements interfaces `c-ini-parser-linked-list` and `c-ini-parser-linked-list-object` as described below.

### Line 
    A key-value object that is the smallest data type. 

This struct implements interface `c-ini-parser-linked-list-object` as described below.

## Parser configuration

### AppConfig
    Stores config of parser, is loaded on start from file given as command-line parametr.
    Also contains names of `Sections` that have to exist in any given `Config`

## Interfaces

## c-ini-parser-linked-list-object 
```c
struct LinkedListObject {
    struct LinkedListObject *nextObject;
};
```

## c-ini-parser-linked-list
```c
struct LinkedList {
    struct LinkedListObject *firstObject;
};

LinkedListObject *getLinkedListObjectAt(LinkedList *list, int idx);
LinkedListObject *getLastLinkedListObject(LinkedList *list);

void appendLinkedListObject(LinkedList *list, LinkedListObject *object);
bool insertLinkedListObject(LinkedList *list, LinkedListObject *object, int idx);

bool deleteLinkedListObjectAt(LinkedList *list, int idx);

size_t getLinkedListSize(LinkedList *list);
bool isLinkedListEmpty(LinkedList *list);
```
