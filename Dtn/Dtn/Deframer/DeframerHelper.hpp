#ifndef Dtn_DeframerHelper_HPP
#define Dtn_DeframerHelper_HPP

#include "ltpP.h"

namespace Dtn
{

class DeframerHelper
{
    public:
        static void init();
        static void ltpDeframe(unsigned char *buffer, int segmentSize);
};

} // end namespace Dtn


#endif // Dtn_DeframerHelper_HPP
