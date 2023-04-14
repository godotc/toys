#![no_std]
#![no_main]

use core::panic::PanicInfo;

// contain file,line... panic msg
// -> ! means defined as a diverging function: function will never return `never type`
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}

static HELLO: &[u8] = b"Hello World!";

// disable name mangling: output func with the name _start without attribute
#[no_mangle]
pub extern "C" fn _start() -> ! {
    let vga_buffer = 0xb8000 as *mut u8;

    for (i, &byte) in HELLO.iter().enumerate() {
        unsafe {
            *vga_buffer.offset(i as isize * 2) = byte; // char
            *vga_buffer.offset(i as isize * 2 + 1) = 0xb; // color: cyan
        }
    }

    loop {}
}
