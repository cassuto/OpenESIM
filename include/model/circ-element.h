#ifndef CIRC_ELEMENT_H_
#define CIRC_ELEMENT_H_

#include <dsim/list.h>
#include <model/model.h>
#include <dsim/cdecl.h>

C_DECLS

typedef struct circ_element_s
{
  DS_LIST_NODE();
  int                   id;
  struct circ_pin_s   **pin_vector;
  int                   pin_count;
  void                 *param;
  const struct circ_element_descriptor_s *desc;
  union {
    const void                     *ptr;
    const struct asim_descriptor_s *asim;
    const struct dsim_descriptor_s *dsim;
  }u;
  struct circuit_s     *circuit;
} circ_element_t;

typedef struct circ_pin_s circ_pin_t;

circ_element_t *circ_element_create( struct circuit_s *circuit, const char *symbol, int id );
int circ_element_set_pins( circ_element_t *element, int n );
circ_pin_t *circ_element_get_pin( circ_element_t *element, int index );
void circ_element_set_digital_pin( circ_element_t *element, int start, int end );
void circ_element_free( void *element );

static inline int
circ_element_get_pin_count( circ_element_t *element )
{
  return element->pin_count;
}

#define circ_element_set_param(element, val) (element)->param = (void*)(val)
#define circ_element_get_param(element, type) (type *)((element)->param)

#define asim_element_init(element)          ((element)->u.asim->pfn_init( element ))
#define asim_element_stamp(element)         ((element)->u.asim->pfn_stamp ? (element)->u.asim->pfn_stamp( element ) : 0 )
#define asim_element_vchanged(element)      ((element)->u.asim->pfn_vchanged( element ))
#define asim_element_config(element)        (element)->u.asim->pfn_config
#define asim_element_reset(element)         ((element)->u.asim->pfn_reset ? ((element)->u.asim->pfn_reset(element)) : 0 )
#define asim_element_uninit(element)        ((element)->u.asim->pfn_uninit ? ((element)->u.asim->pfn_uninit( element )) : 0 )

#define dsim_element_init(element)          ((element)->u.dsim->pfn_init( element ))
#define dsim_element_config(element)        (element)->u.dsim->pfn_config
#define dsim_element_reset(element)         ((element)->u.dsim->pfn_reset ? ((element)->u.dsim->pfn_reset(element)) : 0 )
#define dsim_element_uninit(element)        ((element)->u.dsim->pfn_uninit ? ((element)->u.dsim->pfn_uninit( element )) : 0 )
#define dsim_element_clock(element)         ((element)->u.dsim->pfn_clock ? ((element)->u.dsim->pfn_clock( element )) : 0 )
#define dsim_element_event(element)         ((element)->u.dsim->pfn_event( element ))

#define adsim_element_init(element)         ((element)->u.asim->pfn_init( element ))
#define adsim_element_config(element)       (element)->u.asim->pfn_config
#define adsim_element_reset(element)        ((element)->u.asim->pfn_reset ? ((element)->u.asim->pfn_reset(element)) : 0 )
#define adsim_element_uninit(element)       ((element)->u.asim->pfn_uninit ? ((element)->u.asim->pfn_uninit( element )) : 0 )

#define FN_CONFIG_GET_ELEM_PTR(element, op, ...) (element)

/* e.g. int rc = circ_element_config( (element, 0, 0) ); */
#define circ_element_config(params) \
  ({ \
  int _rc_ = 0; \
  switch( (FN_CONFIG_GET_ELEM_PTR params)->desc->mdel_type ) \
  { \
    case MDEL_ANALOG: \
      _rc_ = ((asim_element_config(FN_CONFIG_GET_ELEM_PTR params) params )); \
      break; \
    case MDEL_DIGITAL: \
      _rc_ = ((dsim_element_config(FN_CONFIG_GET_ELEM_PTR params) params )); \
      break; \
    case MDEL_AD: \
      _rc_ = ((adsim_element_config(FN_CONFIG_GET_ELEM_PTR params) params )); \
      break; \
  } \
  (_rc_); \
  })

END_C_DECLS

#endif//!defined(CIRC_ELEMENT_H_)
