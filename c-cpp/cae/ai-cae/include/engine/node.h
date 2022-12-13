#ifndef GCAE_ENGINE_NODE_H
#define GCAE_ENGINE_NODE_H

#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/src/Core/Matrix.h>

namespace engine {

enum NodeType
{
    INTERNAL_NODE,
    EXTERNAL_NODE
};

class Node
{
  public:
    Node(Eigen::Vector3d &coords_, NodeType type_);
    ~Node() = default;

  public:
    void setCoords(Eigen::Vector3d &coords_);
    void setType(NodeType type_);

    const Eigen::Vector3d getCoords();
    NodeType              getType();

  private:
    Eigen::Vector3d m_coords;
    NodeType        m_type;
};


} // namespace engine
#endif
