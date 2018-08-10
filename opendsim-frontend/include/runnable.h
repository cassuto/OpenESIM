/*
 *  OpenDSIM (A/D mixed circuit simulator)
 *  Copyleft (C) 2016, The first Middle School in Yongsheng Lijiang China
 *
 *  This project is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License(GPL)
 *  as published by the Free Software Foundation; either version 2.1
 *  of the License, or (at your option) any later version.
 *
 *  This project is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 */

#ifndef RUNNABLE_H_
#define RUNNABLE_H_

namespace dsim
{

struct Tuple0
{
    explicit Tuple0() {}
};

template <class A>
  struct Tuple1
  {
    Tuple1() {}
    explicit Tuple1(A a) : a(a) {}
    A a;
  };

template <class A, class B>
  struct Tuple2
  {
  public:
    Tuple2() {}
    Tuple2(A a, B b) : a(a), b(b) {}
    A a;
    B b;
  };

template <class A, class B, class C>
  struct Tuple3
  {
  public:
    Tuple3() {}
    Tuple3(A a, B b, C c) : a(a), b(b), c(c) {}
    A a;
    B b;
    C c;
  };

template <class ObjT, class Method>
  inline int CallMethod(ObjT* obj,
                        Method method,
                        const Tuple0& arg)
  {
    return (obj->*method)();
  }

template <class ObjT, class Method, class A>
  inline int CallMethod(ObjT* obj,
                        Method method,
                        const Tuple1<A>& arg)
  {
    return (obj->*method)( arg.a );
  }

template <class ObjT, class Method, class A, class B>
  inline int CallMethod(ObjT* obj,
                        Method method,
                        const Tuple2<A,B>& arg)
  {
    return (obj->*method)( arg.a, arg.b );
  }

template <class ObjT, class Method, class A, class B, class C>
  inline int CallMethod(ObjT* obj,
                        Method method,
                        const Tuple3<A,B,C>& arg)
  {
    return (obj->*method)( arg.a, arg.b, arg.c );
  }


class RunnableFunction
{
public:
  RunnableFunction() {}
  virtual ~RunnableFunction() {}
public:
  virtual int run() = 0;
private:
  RunnableFunction( const RunnableFunction & );
  RunnableFunction &operator=( const RunnableFunction & );
};

template<class T>
class RunnableBind {
public:
    explicit RunnableBind(T* object)
        : m_ptr(object)
    {}

    /**
     * Create a WSEvent package that runs the method specified.
     * @param method Pointer to the target method.
     * @param arg0 Optional, Argument #1
     * @param arg1 Optional, Argument #2
     */
   template <class Method>
    inline RunnableFunction* bind(Method method)
      {
        return new RunnableMethod<Method, Tuple0>(
                m_ptr, method, Tuple0());
      }

   template <class Method, class A>
    inline RunnableFunction* bind(Method method, const A& a)
      {
        return new RunnableMethod<Method, Tuple1<A> >(
            m_ptr, method, Tuple1<A>(a));
      }

   template <class Method, class A, class B>
    inline RunnableFunction* bind(Method method, const A& a, const B& b)
      {
        return new RunnableMethod<Method, Tuple2<A, B> >(
            m_ptr, method, Tuple2<A,B>(a, b));
      }

   template <class Method, class A, class B, class C>
     inline RunnableFunction* bind(Method method, const A& a, const B& b, const C& c)
       {
         return new RunnableMethod<Method, Tuple3<A, B, C> >(
             m_ptr, method, Tuple3<A,B,C>(a, b, c));
       }

protected:
   template <class Method, class Params>
    class RunnableMethod : public RunnableFunction
    {
    public:
        RunnableMethod(T* obj, Method meth, const Params& params)
            : m_obj(obj),
              m_meth(meth),
              m_params(params)
        {}

        virtual int run()
          {
            return CallMethod( m_obj, m_meth, m_params );
          }

    private:
        T* m_obj;
        Method m_meth;
        Params m_params;
    };

private:
    T* m_ptr;
};

}

#endif
