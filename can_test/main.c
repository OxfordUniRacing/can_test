
#include <atmel_start.h>

static void CAN_0_tx_callback(struct can_async_descriptor *const descr) {
	(void)descr;

	gpio_toggle_pin_level(LED0);
}

static void CAN_0_rx_callback(struct can_async_descriptor *const descr) {
	gpio_toggle_pin_level(LED0);

	struct can_message msg;
	uint8_t data[64];
	msg.data = data;
	can_async_read(descr, &msg);
	return;
}

static void setup_can(void) {
	can_async_register_callback(&CAN_0, CAN_ASYNC_RX_CB, CAN_0_rx_callback);
	can_async_register_callback(&CAN_0, CAN_ASYNC_TX_CB, CAN_0_tx_callback);
	can_async_enable(&CAN_0);

	struct can_filter filter;
	filter.id   = 0x555;
	filter.mask = 0;
	can_async_set_filter(&CAN_0, 0, CAN_FMT_STDID, &filter);
}

static void do_can_tx(void) {
	struct can_message msg;
	uint8_t data[8] = "Hello";

	msg.id   = 0x555;
	msg.type = CAN_TYPE_DATA;
	msg.data = data;
	msg.len  = 6;
	msg.fmt  = CAN_FMT_STDID;

	can_async_write(&CAN_0, &msg);
}

int main(void) {
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	setup_can();

	while (1) {
		if (!gpio_get_pin_level(SW0)) {
			while (!gpio_get_pin_level(SW0));

			do_can_tx();
		}
	}
}
