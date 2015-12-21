#include <linux/module.h>
#include <linux/init.h>
#include <acpi/acpi_bus.h>
#include <asm/io.h>
#include <acpi/actypes.h>
static struct fadt_descriptor* pacpi_fadt = &acpi_fadt;

static int __init acpi_reboot_init(void)
{
	struct acpi_generic_address *reset_reg = &pacpi_fadt->reset_register;
	ACPI_PLog("id=%d,width=%d,addr=0x%llx,value=%d",reset_reg->address_space_id,reset_reg->register_bit_width,reset_reg->address,pacpi_fadt->reset_value);
	u32 width = reset_reg->register_bit_width;
	if(ACPI_ADR_SPACE_SYSTEM_MEMORY == reset_reg->address_space_id)
	{
		void __iomem* virt_addr = ioremap(reset_reg->address, width);
		switch(width)
		{
			case 8:
				writeb(pacpi_fadt->reset_value, virt_addr);
				break;
			case 16:
				writew(pacpi_fadt->reset_value, virt_addr);
				break;
			case 32:
				writel(pacpi_fadt->reset_value, virt_addr);
				break;
		}
	}
	else if(ACPI_ADR_SPACE_SYSTEM_IO == reset_reg->address_space_id)
	{
		switch(width)
		{
			case 8:
				outb(pacpi_fadt->reset_value,(u32)reset_reg->address);
				break;
			case 16:
				outw(pacpi_fadt->reset_value,(u32)reset_reg->address);
				break;
			case 32:
				outl(pacpi_fadt->reset_value,(u32)reset_reg->address);
				break;
		}
	}
	return 0;
}

static void __exit acpi_reboot_exit(void)
{
}

module_init(acpi_reboot_init);
module_exit(acpi_reboot_exit);
MODULE_LICENSE("GPL");
