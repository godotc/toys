#include <engine/node.h>

namespace engine {

Node::Node(Eigen::Vector3d &coords_, NodeType type_)
    : m_coords(coords_),
      m_type(type_) {}

void                  Node::setCoords(Eigen::Vector3d &coords_) { m_coords = coords_; }
const Eigen::Vector3d Node::getCoords() { return m_coords; }
void                  Node::setType(NodeType type_) { m_type = type_; }
NodeType              Node::getType() { return m_type; }

} // namespace engine
