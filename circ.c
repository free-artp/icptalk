/*
 * circ.c
 *
 * Created: 13.11.2017 18:40:08
 *  Author: Artp
 */ 

#include "circ.h"

void circ_init(struct circ_buffer *buff)
{
	buff->pointer = buff->start;
	buff->datalen = 0;
}

__monitor int circ_push(struct circ_buffer *buff, uint8_t *data, int len)
{
  uint8_t *bend = buff->start + buff->length - 1;
  uint8_t *dend = (buff->pointer - buff->start + buff->datalen) % buff->length + buff->start; /* This points to new byte */
#ifdef __CIRC_DELEGATES__
  uint8_t old_state = circ_is_empty(buff);
#endif

  for (; len > 0; len--) {
    if (dend > bend) dend = buff->start;
    if (buff->datalen != 0 && dend == buff->pointer) break;
    *dend = *data;
    dend++;
    data++;
    buff->datalen++;
  }

#ifdef __CIRC_DELEGATES__        
  if (old_state && !circ_is_empty(buff) && (buff->t_nonfree) )
    (buff->t_nonfree)();
#endif
  
  return len; /* Return amount of bytes left */
}

__monitor int circ_push_byte(struct circ_buffer *buff, uint8_t data)
{
  uint8_t *bend = buff->start + buff->length - 1;
  uint8_t *dend = (buff->pointer - buff->start + buff->datalen) % buff->length + buff->start; /* This points to new byte */
  int len;
#ifdef __CIRC_DELEGATES__
  uint8_t old_state = circ_is_empty(buff);
#endif

  if (dend > bend) dend = buff->start;
  if (buff->datalen != 0 && dend == buff->pointer) {
    len = 1;
  } else {
    *dend = data;
    buff->datalen++;
    len = 0;
  }
#ifdef __CIRC_DELEGATES__        
  if (old_state && !circ_is_empty(buff) && (buff->t_nonfree) )
    (buff->t_nonfree)();
#endif
	return len; /* Return amount of bytes left */
}

__monitor int circ_pop(struct circ_buffer *buff, uint8_t *data, int len)
{
  uint8_t *bend = buff->start + buff->length - 1;
  int i;
#ifdef __CIRC_DELEGATES__
  uint8_t old_state = circ_is_full(buff);
#endif

  for (i = 0; i < len && buff->datalen > 0; i++) {
          data[i] = *buff->pointer;
          buff->pointer++;
          buff->datalen--;
          if (buff->pointer > bend) buff->pointer = buff->start;
  }
  
#ifdef __CIRC_DELEGATES__
  if (old_state && !circ_is_full(buff) && (buff->t_nonfull) )
    (buff->t_nonfull)();
#endif
	return i; /* Return the amount of bytes actually popped */
}

__monitor int circ_peek(struct circ_buffer *buff, uint8_t *data, int len)
{
	volatile uint8_t *ptr = buff->pointer;
	volatile uint8_t *bend = buff->start + buff->length - 1;
	int i;

	for (i = 0; i < len && i < buff->datalen; i++) {
		data[i] = ptr[i];
		if (ptr > bend) ptr = buff->start;
	}

	return i; /* Return the amount of bytes actually peeked */
}

int circ_find(struct circ_buffer *buff, uint8_t byte)
{
	volatile uint8_t *ptr = buff->pointer;
	volatile uint8_t *bend = buff->start + buff->length - 1;
	int i;

	for (i = 0; i < buff->datalen; i++) {
		if (byte == ptr[i]) return 1;
		if (ptr > bend) ptr = buff->start;
	}
  return 0;
}