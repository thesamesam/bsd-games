// Copyright (c) 2016 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#pragma once

#define ArraySize(a)	(sizeof(a)/sizeof(a[0]))
#define ArrayBlock(a)	a, ArraySize(a)

void*	xalloc (size_t sz) noexcept __attribute__((malloc));
void*	xrealloc (void* p, size_t sz) noexcept __attribute__((malloc));
#define xfree(p)	do { if (p) { free(p); p = NULL; }} while (false)

struct CharVector {
    char*	d;
    size_t	size;
    size_t	allocated;
    size_t	elsize;
};

// Declares a vector for the given type.
// Use the VECTOR macro to instantiate it. Fields are marked const to
// ensure only the vector_ functions modify it.
#define DECLARE_VECTOR_TYPE(name,type)	\
struct name {				\
    type* const		d;		\
    const size_t	size;		\
    const size_t	allocated;	\
    const size_t	elsize;		\
}

#define VECTOR_INIT(vtype)	{ NULL, 0, 0, sizeof(*(((vtype*)NULL)->d)) }
#define VECTOR(vtype,name)	vtype name = VECTOR_INIT(vtype)
#define VECTOR_MEMBER_INIT(vtype,name)	vector_init(name, sizeof(*(((vtype*)NULL)->d)))

#define vector_begin(v)			v.d
#define vector_end(v)			(v.d+v.size)
#define vector_foreach(vtype,p,v)	for (vtype *p = vector_begin(v), *p##end = vector_end(v); p < p##end; ++p)

void	vector_init (void* v, size_t elsz) noexcept;
void	vector_reserve (void* v, size_t sz) noexcept;
void	vector_deallocate (void* v) noexcept;
void	vector_insert (void* v, size_t ip, const void* e) noexcept __attribute__((nonnull));
void*	vector_emplace (void* v, size_t ip) noexcept;
void	vector_erase_n (void* v, size_t ep, size_t n) noexcept;
void	vector_swap (void* v1, void* v2) noexcept __attribute__((nonnull));

static inline void vector_erase (void* v, size_t ep)
    { vector_erase_n (v, ep, 1); }
static inline void vector_push_back (void* v, const void* e)
    { vector_insert (v, ((struct CharVector*)v)->size, e); }
static inline void* vector_emplace_back (void* v)
    { return vector_emplace (v, ((struct CharVector*)v)->size); }
static inline void vector_pop_back (void* v)
    { vector_erase (v, ((struct CharVector*)v)->size-1); }
static inline void vector_clear (void* vv)
    { struct CharVector* v = (struct CharVector*) vv; v->size = 0; }
static inline void vector_detach (void* vv)
    { struct CharVector* v = (struct CharVector*) vv; v->d = NULL; v->size = v->allocated = 0; }
static inline __attribute__((nonnull)) void vector_attach (void* vv, void* e, size_t n) {
    struct CharVector* v = (struct CharVector*) vv;
    assert (!v->d && "This vector is already attached to something. Detach or deallocate first.");
    assert (e && "Attaching requires a non-null pointer");
    v->d = e; v->size = v->allocated = n;
}
