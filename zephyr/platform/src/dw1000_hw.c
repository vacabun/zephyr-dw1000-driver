#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <zephyr/kernel.h>
#include "dw1000_spi.h"
#include "deca_device_api.h"
LOG_MODULE_REGISTER(dw1000, CONFIG_DW1000_LOG_LEVEL);

#define DW_INST DT_INST(0, qorvo_dw1000)

static struct k_work dw1000_isr_work;
static struct gpio_callback gpio_cb;

struct dw1000_config {
	struct gpio_dt_spec gpio_irq;
	struct gpio_dt_spec gpio_reset;
	struct gpio_dt_spec gpio_wakeup;
	struct gpio_dt_spec gpio_spi_pol;
	struct gpio_dt_spec gpio_spi_pha;
};
static const struct dw1000_config conf = {
	.gpio_irq = GPIO_DT_SPEC_GET_OR(DW_INST, irq_gpios, {0}),
	.gpio_reset = GPIO_DT_SPEC_GET_OR(DW_INST, reset_gpios, {0}),
	.gpio_wakeup = GPIO_DT_SPEC_GET_OR(DW_INST, wakeup_gpios, {0}),
	.gpio_spi_pol = GPIO_DT_SPEC_GET_OR(DW_INST, spi_pol_gpios, {0}),
	.gpio_spi_pha = GPIO_DT_SPEC_GET_OR(DW_INST, spi_pha_gpios, {0}),
};

int dw1000_hw_init()
{
	/* Reset */
	if (conf.gpio_reset.port) {
		gpio_pin_configure_dt(&conf.gpio_reset, GPIO_OUTPUT_INACTIVE);
		LOG_INF("RESET on %s pin %d", conf.gpio_reset.port->name,
				conf.gpio_reset.pin);
	}

	/* Wakeup (optional) */
	if (conf.gpio_wakeup.port) {
		gpio_pin_configure_dt(&conf.gpio_wakeup, GPIO_OUTPUT_ACTIVE);
		LOG_INF("WAKEUP on %s pin %d", conf.gpio_wakeup.port->name,
				conf.gpio_wakeup.pin);
	}

	/* SPI Polarity (optional) */
	if (conf.gpio_spi_pol.port) {
		gpio_pin_configure_dt(&conf.gpio_spi_pol, GPIO_OUTPUT_INACTIVE);
		LOG_INF("SPI_POL on %s pin %d", conf.gpio_spi_pol.port->name,
				conf.gpio_spi_pol.pin);
	}

	/* SPI Phase (optional) */
	if (conf.gpio_spi_pha.port) {
		gpio_pin_configure_dt(&conf.gpio_spi_pha, GPIO_OUTPUT_INACTIVE);
		LOG_INF("SPI_PHA on %s pin %d", conf.gpio_spi_pha.port->name,
				conf.gpio_spi_pha.pin);
	}

	return dw1000_spi_init();
}

static void dw1000_hw_isr_work_handler(struct k_work* item)
{
	dwt_isr();
}

static void dw1000_hw_isr(const struct device* dev, struct gpio_callback* cb,
						  uint32_t pins)
{
	k_work_submit(&dw1000_isr_work);
}

int dw1000_hw_init_interrupt(void)
{
	if (conf.gpio_irq.port) {
		k_work_init(&dw1000_isr_work, dw1000_hw_isr_work_handler);

		gpio_pin_configure_dt(&conf.gpio_irq, GPIO_INPUT);
		gpio_init_callback(&gpio_cb, dw1000_hw_isr, BIT(conf.gpio_irq.pin));
		gpio_add_callback(conf.gpio_irq.port, &gpio_cb);
		gpio_pin_interrupt_configure_dt(&conf.gpio_irq, GPIO_INT_EDGE_RISING);

		LOG_INF("IRQ on %s pin %d", conf.gpio_irq.port->name,
				conf.gpio_irq.pin);
		return 0;
	} else {
		LOG_ERR("IRQ pin not configured");
		return -ENOENT;
	}
}

void dw1000_hw_interrupt_enable(void)
{
	if (conf.gpio_irq.port) {
		gpio_pin_interrupt_configure_dt(&conf.gpio_irq, GPIO_INT_EDGE_RISING);
	}
}

void dw1000_hw_interrupt_disable(void)
{
	if (conf.gpio_irq.port) {
		gpio_pin_interrupt_configure_dt(&conf.gpio_irq, GPIO_INT_DISABLE);
	}
}

void dw1000_hw_fini(void)
{
	// TODO
	dw1000_spi_fini();
}

void dw1000_hw_reset()
{
	if (!conf.gpio_reset.port) {
		LOG_ERR("No HW reset configured");
		return;
	}

	gpio_pin_configure_dt(&conf.gpio_reset, GPIO_OUTPUT_ACTIVE);
	k_msleep(2); // 10 us?
	gpio_pin_configure_dt(&conf.gpio_reset, GPIO_INPUT);
	k_msleep(2);
}

/** wakeup either using the WAKEUP pin or SPI CS */
void dw1000_hw_wakeup(void)
{
	if (conf.gpio_wakeup.port) {
		/* Use WAKEUP pin if available */
		LOG_INF("WAKEUP PIN");
		gpio_pin_set_dt(&conf.gpio_wakeup, 1);
		k_msleep(1);
		gpio_pin_set_dt(&conf.gpio_wakeup, 0);
	} else {
		/* Use SPI CS pin */
		LOG_INF("WAKEUP CS");
		dw1000_spi_wakeup();
	}
	// k_sleep(K_MSEC(1));
}

/** set WAKEUP pin low if available */
void dw1000_hw_wakeup_pin_low(void)
{
	if (conf.gpio_wakeup.port) {
		gpio_pin_set_dt(&conf.gpio_wakeup, 0);
	}
}