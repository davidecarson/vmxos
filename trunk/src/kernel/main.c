#include "main.h"

void main()
{
	CPUID_t cpu;
	CR4_t cr4;
	CR0_t cr0;
	EFLAGS_t eflags;
	unsigned int a20 = check_A20();

	cls();
	puts("Welcome to VMXOS "); puts(vmxos_version); puts(" build "); puts(dec2string(vmxos_build)); puts("\n");
	puts("Inizialising the kernel modules\n\n");

	puts("A20   : "); puts(a20 ? "enabled\n" : "not enabled\n");
	if (!a20) { puts("enabling A20 gate "); enable_A20(); puts("[done]\n"); }

	cpuid(&cpu);
	puts("CPUID : EAX -> "); puts(hex2string(cpu.eax)); puts(" = "); puts(bin2string(cpu.eax)); puts("\n");
	puts("      : EBX -> "); puts(hex2string(cpu.ebx)); puts(" = "); puts(bin2string(cpu.ebx)); puts("\n");
	puts("      : ECX -> "); puts(hex2string(UINT(cpu.ecx))); puts(" = "); puts(bin2string(UINT(cpu.ecx))); puts("\n");
	puts("      : EDX -> "); puts(hex2string(UINT(cpu.edx))); puts(" = "); puts(bin2string(UINT(cpu.edx))); puts("\n");
	puts("VMX   : "); puts(cpu.ecx.b05_VMX ? "supported\n" : "not supported\n");
	puts("MSR   : "); puts(cpu.edx.b05_MSR ? "supported\n" : "not supported\n");

	UINT(eflags) = vmx_read_eflags();
	puts("EFLAGS: "); puts(hex2string(UINT(eflags))); puts(" = "); puts(bin2string(UINT(eflags))); puts("\n");
	puts("VM86 Mode is "); puts(eflags.b17_VM ? "on\n" : "off\n");

	UINT(cr4) = vmx_read_cr4();
	cr4.b13_VMXE = 1;
	puts("Enabling CR4.VME "); vmx_write_cr4(UINT(cr4)); puts("[done]\n");

	UINT(cr0) = vmx_read_cr0();
	cr0.b00_PE = 1; puts("enabling CR0.PE "); vmx_write_cr0(UINT(cr0)); puts("[done]\n");
	cr0.b05_NE = 1; puts("enabling CR0.NE "); vmx_write_cr0(UINT(cr0)); puts("[done]\n");
	puts("Enabling paging "); enable_paging(); puts("[done]\n");
	
	puts("CR0   : "); puts(hex2string(vmx_read_cr0())); puts(" = "); puts(bin2string(vmx_read_cr0())); puts("\n");
	puts("CR3   : "); puts(hex2string(vmx_read_cr3())); puts(" = "); puts(bin2string(vmx_read_cr3())); puts("\n");
	puts("CR4   : "); puts(hex2string(vmx_read_cr4())); puts(" = "); puts(bin2string(vmx_read_cr4())); puts("\n");

	remap_pic(0x20, 0x28);
	mask_irq(IRQ_TIMER);
	//unmask_irq(IRQ_KEYBOARD);
	puts("Inizializing interrupt handlers "); init_exceptions(); puts("[done]\n");
	sti();
	//invoke48();

	puts("\n\nType something: ");

	while(1);
}