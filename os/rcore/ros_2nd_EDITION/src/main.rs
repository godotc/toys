#![no_std]
#![no_main]

mod vga_buffer;

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
    println!("Hello world {} !", "godot42");

    loop {}
}
