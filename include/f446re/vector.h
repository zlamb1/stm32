#ifndef F446RE_VECTOR_H
#define F446RE_VECTOR_H 1

#define VECTOR_TABLE_COUNT 113

#ifndef __ASSEMBLER__

typedef void (*isr)(void);

extern isr vector_table[VECTOR_TABLE_COUNT];

static void vectorInstall(unsigned entry, isr isr) {
  vector_table[entry] = isr;
}

static void vectorRemove(unsigned entry) { vector_table[entry] = 0; }

#endif

#endif