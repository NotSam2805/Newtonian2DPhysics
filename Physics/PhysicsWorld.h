#include <vector>
#include "Rigidbody.h"

namespace n2p{
    class PhysicsWorld {
    private:
        std::vector<Rigidbody*> bodies;
    public:
        PhysicsWorld() {}
        
        void Step(float dt){
            /* 
            Performs one time step, moving dt seconds.
            Apply forces body -> Integrate body
            Loop through all bodies
            */
        }

        void AddBody(Rigidbody* body){
            bodies.push_back(body);
        }
    };
}
