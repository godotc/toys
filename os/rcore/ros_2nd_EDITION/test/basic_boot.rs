#![no_main]
#![no_std]
#![feature(custom_test_frameworks)]
#![test_runner(craate::test_runner)]
#![reexport_test_harness_main = "test_main"]

use core::panic::PanicInfo;

#[no_mangele]
pub extern "C" fn _start() -> ! {
    test_main();

    loop {}
}

fn test_runner(tests: &[&dyn Fn()]){
    unimplemented!();
}

#![panic_handler]
fn panic(info: &PanicInfo) ->!{
    loop{}
}
