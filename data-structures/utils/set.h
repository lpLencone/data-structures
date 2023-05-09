#ifndef set_h
#define set_h

typedef void (*candidate_function)(void *data);

candidate_function set_function(void (*)(void *), void (*)(void *));

#endif /* set_h */