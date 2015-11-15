#ifndef INFO_ACPI_H
#define INFO_ACPI_H

#define EBDA_FIND_LENGTH  1024
#define MAGIC_POINTER     0x40E
#define MEM_REG_BEGIN     0x000E0000
#define MEM_REG_END       0x000FFFFF

#define IO_APIC_TYPE                  0x1
#define LOCAL_APIC_ADDR_OVERRIDE_TYPE 0x5
#define PROC_LOCAL_APIC_TYPE          0x0
#define PROC_LOCAL_X2_APIC_TYPE       0x9

#define PC_AT_DUAL_SUPPORTED_FLAG     0x1


void about_apic();

#endif /* INFO_ACPI_H */
