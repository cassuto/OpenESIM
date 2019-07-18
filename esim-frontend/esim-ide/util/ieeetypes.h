#ifndef IEEE_TYPES_H_
#define IEEE_TYPES_H_

namespace util
{

enum ElectricalType
{
  ELEC_INPUT = 0,
  ELEC_OUTPUT,
  ELEC_BIDIRECTIONAL,
  ELEC_TRI_STATE,
  ELEC_PASSIVE,
  ELEC_POWER_INPUT,
  ELEC_POWER_OUTPUT,
  ELEC_OPEN_COLLECTOR,
  ELEC_OPEN_EMITTER,
  ELEC_NOT_CONNECTED
};
    
const char *getElectricalTypeName(ElectricalType type);

enum PinType
{
  PIN_NONE = 0,
  PIN_REVERSED,
  PIN_CLOCK,
  PIN_REVERSED_CLOCK,
  PIN_LOW_INPUT,
  PIN_LOW_CLOCK,
  PIN_LOW_OUTPUT,
  PIN_CLOCK_FALL_EDGE,
  PIN_LOGIC_NOT
};
    
const char *getPinTypeName(PinType type);

} // namespace util

#endif // IEEE_TYPES_H_
