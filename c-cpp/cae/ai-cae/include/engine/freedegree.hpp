#ifndef GCAE_ENGINE_FREEDEGREE_H
#define GCAE_ENGINE_FREEDEGREE_H

#include <eigen3/Eigen/Core>
#include <unordered_map>
#include <utility>

namespace engine {

//! Directions expressed by Bit domain:
//!> Translate, Rotate, Scale
struct Bits_dir
{
    unsigned char x : 1;
    unsigned char y : 1;
    unsigned char z : 1;
};

//! The [SHEAR] Directions expressed by Bit domain:
struct Bits_dir_SHEAR
{
    unsigned char x_y : 1;
    unsigned char x_z : 1;
    unsigned char y_x : 1;
    unsigned char y_z : 1;
    unsigned char z_x : 1;
    unsigned char z_y : 1;
};


class FreeDrgee
{
  public:
    using CanTranslate = Bits_dir;
    using CanRotate    = Bits_dir;
    using CanScale     = Bits_dir;
    using CanShear     = Bits_dir_SHEAR;

  public:
    FreeDrgee(CanTranslate canTranslate_, CanRotate canRotate_);
    ~FreeDrgee() = default;

  public:
    bool canShearOn(CanShear canShear_) { return false; }

  public:
    void setTranslate(CanTranslate canTranslate_) { m_canTranslate = canTranslate_; }
    void setRotate(CanRotate canRotate_) { m_canRotate = canRotate_; }
    void setScale(CanScale canScale_) { m_canScale = canScale_; }
    void setShear(CanShear canShear_) { m_canShear = canShear_; }

    CanTranslate getTranslate() { return m_canTranslate; }
    CanRotate    getRotate() { return m_canRotate; }
    CanScale     getScale() { return m_canScale; }
    CanShear     getShear() { return m_canShear; }

  private:
    // Get which is free on some direction
    CanTranslate m_canTranslate;
    CanRotate    m_canRotate;
    CanScale     m_canScale;
    CanShear     m_canShear;

    //! Left and Right range
    struct Range
    {
        float lefg;
        float right;
    };

    /**
     * @brief
     *   Get the range of free on the specfic dir.
     *   不能用m_xxx作键值，只能传入(构造一个)代表那个方向的 bits_dir 来获取对应的range
     */
    //!> Translate, Rotate, Scale range
    std::unordered_map<Bits_dir, std::pair<float, float>> m_TRSrange;
    //!> Shear range
    std::unordered_map<Bits_dir_SHEAR, std::pair<float, float>> m_shaerRange;
};
} // namespace engine
#endif