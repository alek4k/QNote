#ifndef NOTE_H
#define NOTE_H

#include "container.h"
#include "nota.h"

class Note : public Container<Nota>
{
public:
    using Container<Nota>::Container;
};

#endif // NOTE_H
