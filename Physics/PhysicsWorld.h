#include <vector>
#include "Rigidbody.h"

namespace n2p{
    class PhysicsWorld {
    private:
        std::vector<Rigidbody*> bodies;
    public:
        void Step(float dt){
            /* 
            Performs one time step, moving dt seconds.
            Apply forces body -> Integrate body
            Loop through all bodies
            */
        }
    };
}
