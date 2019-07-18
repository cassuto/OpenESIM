/**
 * To declare a callback, define macros listed below and then include this file。
 * For example:
 * ----------------------------------------------------------------
 * #define RUNNABLE_CALLBACK MyCallback
 * #define RUNNABLE_CALLBACK_BASE MyCallbackBase
 * #define CALLBACK_RETURN init
 * #define CALLBACK_PARAM_DECLS (int a0, const char *a1)
 * #define CALLBACK_PARAMS (op, format)
 * #include "runnablecallback.h"
 *
 * class A {
 *   public: int callback(int a0, const char *a1) { return x; do something... }
 * } a;
 * MyCallbackBase *cb = new MyCallback<A>(&a, &A::callback);
 * int ret = (*cb)(0, 0l);
 * ----------------------------------------------------------------
 * Note: 1. Needless to #undef these macros after #include.
 *       2. CALLBACK_RETURN should not be void. (i.e. callback function
 *          must return a value)
 */
#ifndef RUNNABLE_CALLBACK
# define RUNNABLE_CALLBACK RunnableCallback
# define RUNNABLE_CALLBACK_BASE RunnableCallbackBase
# define CALLBACK_RETURN int
# define CALLBACK_PARAM_DECLS (void)
# define CALLBACK_PARAMS ()
#endif

class RUNNABLE_CALLBACK_BASE
{
public:
  virtual ~RUNNABLE_CALLBACK_BASE() {}
  virtual CALLBACK_RETURN operator() CALLBACK_PARAM_DECLS=0;
};

template <class Class>
  class RUNNABLE_CALLBACK : public RUNNABLE_CALLBACK_BASE
  {
  public:
    typedef CALLBACK_RETURN (Class::*Method) CALLBACK_PARAM_DECLS;

    RUNNABLE_CALLBACK(Class* instance, Method method) :
      m_instance(instance),
      m_method(method)
    {}

    CALLBACK_RETURN operator() CALLBACK_PARAM_DECLS
    {
       return (m_instance->*m_method) CALLBACK_PARAMS;
    };

  private:
    Class*  m_instance;
    Method m_method;
  };

#undef RUNNABLE_CALLBACK
#undef RUNNABLE_CALLBACK_BASE
#undef CALLBACK_RETURN
#undef CALLBACK_PARAM_DECLS
#undef CALLBACK_PARAMS
