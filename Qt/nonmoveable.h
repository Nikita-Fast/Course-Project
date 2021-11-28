#ifndef NONMOVEABLE_H
#define NONMOVEABLE_H

template <class T>
class NonMoveable
{
  public:
    NonMoveable (const NonMoveable &) = delete;
    NonMoveable (NonMoveable &&) = delete;
    T & operator = (const T &) = delete;

  protected:
    NonMoveable () = default;
    ~NonMoveable () = default;
};

#endif // NONMOVEABLE_H
