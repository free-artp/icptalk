/*
 * circ.h
 *
 * Created: 13.11.2017 18:43:12
 *  Author: Artp
 */ 


#ifndef __CIRC_H_
#define __CIRC_H_



// Если не нужна отработока "делегатов" по изменению состояния буфферов, то убрать
//#define __CIRC_DELEGATES__   

// при инициализации:
//uint8_t buff_inp[DRV_BUFF];
//
//struct circ_buffer uart_queue_inp = {
//	.start = buff_inp,
//	.length = sizeof(buff_inp),
//      .t_full =  uart_rcv_disable,
//      .t_nonfull = uart_rcv_enable
//};
// затем вызывается circ_init(&uart_queue_inp);
//

//
// push - увеличивает datalen. Данные складываются за pointer'ом очередной байт не догонит pointer
// pop - отдает данные, начиная с pointer. Pointer увеличивается, а datalen уменьшается.
// peek - отдает данные, но не изменяя pointer и datalen
//

#ifndef IAR
#define __monitor 
#endif

typedef void (*TPTR)(void);
typedef unsigned char uint8_t;

struct circ_buffer {
	uint8_t *start;					// указатель на начало буфера. Константа
	int length;						// 

	/* Private */
	volatile uint8_t        *pointer;		// указывает на первый непрочтенный байт в буфере. При чтении (pop) увеличивается, а datalen уменьшается. При push'е не изменяется.
	volatile int            datalen;		// говорит сколько содержательных байт лежит после pointer'a. Увеличивается, когда засовываем (push) в буфер

#ifdef __CIRC_DELEGATES__
        /* delegates for corresponding condition in buffer */
        TPTR    t_full; 
        TPTR    t_nonfull;
        TPTR    t_free;
        TPTR    t_nonfree;
#endif        

};

#define circ_datalen(buff) \
((buff)->datalen)

#define circ_is_full(buff) \
((buff)->length == (buff)->datalen)

#define circ_is_empty(buff) \
(circ_datalen(buff) == 0)

void circ_init(struct circ_buffer *buff);

/* Return the amount of bytes left */

__monitor int circ_push(struct circ_buffer *buff, uint8_t *data, int len);
__monitor int circ_push_byte(struct circ_buffer *buff, uint8_t data);

/* Returns amount of bytes popped/peeked */
__monitor int circ_pop(struct circ_buffer *buff, uint8_t *data, int len);
__monitor int circ_peek(struct circ_buffer *buff, uint8_t *data, int len);



#endif /* __CIRC_H_ */