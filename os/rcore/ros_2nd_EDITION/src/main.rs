#![no_std]
#![no_main]
// customor test framework
#![feature(custom_test_frameworks)]
#![test_runner(crate::test_runner)]
#![reexport_test_harness_main = "test_main"] // because we has #![no_main] attribute

mod serial;
mod vga_buffer;

use core::panic::PanicInfo;

// disable name mangling: output func with the name _start without attribute
#[no_mangle]
pub extern "C" fn _start() -> ! {
    println!("Hello world {} !", "godot42");

    #[cfg(test)]
    test_main();

    loop {}
}

// qemu isa-debug-exit exit function for io read/wirt on x86_64 arch
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(u32)]
pub enum QemuExitCode {
    Success = 0x10,
    Failed = 0x11,
}
pub fn exit_qemu(exit_code: QemuExitCode) {
    use x86_64::instructions::port::Port;
    unsafe {
        let mut port = Port::new(0xf4);
        port.write(exit_code as u32);
    }
}

#[cfg(test)]
fn test_runner(tests: &[&dyn Fn()]) {
    serial_println!("Running {} tests", tests.len());
    for test in tests {
        test();
    }
    exit_qemu(QemuExitCode::Success);
}

#[test_case]
fn trivial_assertion() {
    serial_print!("trivial assertion... ");
    assert_eq!(1, 1);
    //assert_eq!(0, 1);
    serial_println!("[ok]");
}

// contain file,line... panic msg
// -> ! means defined as a diverging function: function will never return `never type`
#[cfg(not(test))]
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    println!("{}", _info);
    loop {}
}

// for serial test
#[cfg(test)]
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    serial_println!("[failed]\n");
    serial_println!("Error: {}\n", _info);
    exit_qemu(QemuExitCode::Failed);
    loop {}
}
