#ifndef Dtn_DeframerHelper_HPP
#define Dtn_DeframerHelper_HPP

namespace Dtn
{

class DeframerHelper
{
    private:

    public:
        void init();
        void ltpDeframe(unsigned char *buffer, int segmentSize);
};

} // end namespace Dtn


#endif // Dtn_DeframerHelper_HPP
