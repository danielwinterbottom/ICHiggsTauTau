#include "../interface/Vertex.hh"

namespace ic {
// Constructors/Destructors
Vertex::Vertex() : chi2_(0.), ndof_(0.), id_(0) {}

Vertex::~Vertex() {}

void Vertex::Print() const { std::cout << point_ << std::endl; }
}
