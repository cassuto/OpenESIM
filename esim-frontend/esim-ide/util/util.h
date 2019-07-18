#ifndef UTIL_H_
#define UTIL_H_

#define s_max(op1, op2) ({ int _op1=op1, _op2=op2; ((_op1 > _op2) ? _op1 : _op2); })
#define s_min(op1, op2) ({ int _op1=op1, _op2=op2; ((_op1 < _op2) ? _op1 : _op2); })

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

#endif // UTIL_H_
