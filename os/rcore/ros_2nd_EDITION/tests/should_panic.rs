#![no_std]
#![no_main]

use core::panic::PanicInfo;
use ros::{exit_qemu, serial_println, QemuExitCode};

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    serial_println!("[ok]");
    exit_qemu(QemuExitCode::Success);
    loop {}
}

#[no_mangle]
pub extern "C" fn _start() -> ! {
    should_fail();
    serial_println!("test did not println");
    exit_qemu(QemuExitCode::Failed);
    loop {}
}

fn should_fail() {
    serial_println!("should_panic:should_fail...\t");
    assert_eq!(0, 1);
}
