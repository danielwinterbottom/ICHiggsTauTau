#include "../interface/SecondaryVertex.hh"

namespace ic {
// Constructors/Destructors
SecondaryVertex::SecondaryVertex() : distance_3d_(0.), distance_err_3d_(0.) {}

SecondaryVertex::~SecondaryVertex() {}

void SecondaryVertex::Print() const { Vertex::Print(); }
}
