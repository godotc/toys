ENTRY(start) /* global start */

SECTIONS{
	/* where as the start of .boot .text  secgment */
	. = 1M;  /* set the load address of first 1Mib*/

	.boot :
	{
		*(.multiboot_header)
	}

	.text :
	{
		*(.text)
	}
}
