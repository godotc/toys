#![no_std]
#![no_main]
#![feature(custom_test_frameworks)] // customor test framework
#![test_runner(ros::test_runner)]
#![reexport_test_harness_main = "test_main"] // because we has #![no_main] attribute

use core::panic::PanicInfo;
use ros::println;

// disable name mangling: output func with the name _start without attribute
#[no_mangle]
pub extern "C" fn _start() -> ! {
    println!("Hello world {}!", "godot42");

    #[cfg(test)]
    test_main();

    loop {}
}

// contain file,line... panic msg
// -> ! means defined as a diverging function: function will never return `never type`
#[cfg(not(test))]
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    println!("{}", _info);
    loop {}
}

#[cfg(test)]
#[panic_handler]
fn panic(info: &PanicInfo) -> ! {
    ros::test_panic_handler(info);
}

#[test_case]
fn trivial_assertion() {
    assert_eq!(1, 1);
    //assert_eq!(0, 1);
}
