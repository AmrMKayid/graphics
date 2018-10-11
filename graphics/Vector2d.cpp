//
//  Vector2d.cpp
//  graphics
//
//  Created by Amr M. Kayid on 10/11/18.
//  Copyright © 2018 Amr M. Kayid. All rights reserved.
//

#include "Vector2d.hpp"

//----------------------------------------------------------------------
// Vector2d methods
//----------------------------------------------------------------------
// constructors
Vector2d::Vector2d() : x(0), y(0) { }

Vector2d::Vector2d(Scalar _x, Scalar _y) : x(_x), y(_y) { }

Vector2d::Vector2d(const Vector2d& v) : x(v.x), y(v.y) { }

// assignment ops
Vector2d& Vector2d::operator=(const Vector2d& v)
{ x = v.x; y = v.y; return *this; }

Vector2d& Vector2d::operator+=(const Vector2d& v)
{ x += v.x; y += v.y; return *this; }

Vector2d& Vector2d::operator-=(const Vector2d& v)
{ x -= v.x; y -= v.y; return *this; }

Vector2d& Vector2d::operator*=(Scalar s)
{ x *= s; y *= s; return *this; }

Vector2d& Vector2d::operator/=(Scalar s)
{ x /= s; y /= s; return *this; }
// arithmetic ops
Vector2d Vector2d::operator+(const Vector2d& v) const
{ return Vector2d(x + v.x, y + v.y); }

Vector2d Vector2d::operator-(const Vector2d& v) const
{ return Vector2d(x - v.x, y -v.y); }

Vector2d Vector2d::operator*(Scalar s) const
{ return Vector2d(s*x, s*y); }

Vector2d Vector2d::operator/(Scalar s) const
{ return Vector2d(x/s, y/s); }

Vector2d Vector2d::operator-() const
{ return Vector2d(-x, -y); }

Scalar Vector2d::length() const
{ return sqrt(x*x + y*y); }

void Vector2d::normalize() {                // normalize to unit length
    Scalar w = length();
    if (w == 0) { x = 1; y = 0; }            // too short!
    else { x /= w; y /= w; }                // scale by 1/length
}

Vector2d Vector2d::normalize(const Vector2d& v) {
    Vector2d tmp = v;
    tmp.normalize();
    return tmp;
}

// dot product
Scalar Vector2d::dot(const Vector2d& v, const Vector2d& w)
{ return v.x * w.x + v.y * w.y; }

// distance
Scalar Vector2d::distance(const Vector2d& v, const Vector2d& w)
{ return (v - w).length(); }

// project v parallel to w
Vector2d Vector2d::parProject(const Vector2d& v, const Vector2d& w)
{ return w * (dot(v,w) / dot(w,w)); }

// project v orthogonal to w
Vector2d Vector2d::orthProject(const Vector2d& v, const Vector2d& w)
{ return v - parProject(v, w); }

// output operator
std::ostream& operator<<(std::ostream& out, const Vector2d& p)
{
    out << "(" << p.x << ", " << p.y << ")";
    return out;
}

//----------------------------------------------------------------------
// Rect2d methods
//----------------------------------------------------------------------

// constructors
Rect2d::Rect2d() : lowLeft(0,0), upRight(1,1) { }

Rect2d::Rect2d(const Point2d& l, const Point2d& u) : lowLeft(l), upRight(u) { }

Point2d Rect2d::getLowerLeft() const            // get corners
{ return lowLeft; }

Point2d Rect2d::getUpperRight() const
{ return upRight; }

Rect2d& Rect2d::operator=(const Rect2d& r) {
    lowLeft = r.lowLeft;
    upRight = r.upRight;
    return *this;
}

bool Rect2d::contains(const Point2d& p) const // do we contain p?
{
    return    lowLeft.x <= p.x &&
    p.x <= upRight.x &&
    lowLeft.y <= p.y &&
    p.y <= upRight.y;
}

bool Rect2d::overlaps(const Rect2d& r) const // do we overlap r?
{
    return !(lowLeft.x > r.upRight.x ||
             lowLeft.y > r.upRight.y ||
             r.lowLeft.x > upRight.x ||
             r.lowLeft.y > upRight.y);
}

// translate r by t
Rect2d translate(const Rect2d& r, const Vector2d& t)
{
    Point2d ll = r.getLowerLeft();
    Point2d ur = r.getUpperRight();
    return Rect2d(Point2d(ll.x + t.x, ll.y + t.y),
                  Point2d(ur.x + t.x, ur.y + t.y));
}

void drawRectangle(const Rect2d& rect, const GLfloat *color) {
    glColor3fv(color);
    glRectf(rect.getLowerLeft().x, rect.getLowerLeft().y, rect.getUpperRight().x, rect.getUpperRight().y);
}

// output operator
std::ostream& operator<<(std::ostream& out, const Rect2d& r)
{
    out << "[" << r.getLowerLeft() << " -- " << r.getUpperRight() << "]";
    return out;
}
