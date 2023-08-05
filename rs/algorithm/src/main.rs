//use algorithm::*;

use crate::buble_sort::buble_sort;
use crate::merge_sort::merge_sort;
use crate::quick_sort::quick_sort;

use rand::{thread_rng, Rng};
use std::{env, time::Instant};

//fn get_function_name<T>(_: T) -> &'static str {
//    let name = std::any::type_name::<T>();
//    let symbol = get_symbol_name(name);
//    symbol
//}
//
//fn get_symbol_name(fun_ptr: &str) -> &'static str {
//    let symbol_name: &[u8] = unsafe { std::mem::transmute(fun_ptr) };
//    let symbol_name = std::str::from_utf8(symbol_name).expect("Invalid symbol name");
//    symbol_name
//}

fn get_random_array(n: i32, l: i32, r: i32) -> Vec<i32> {
    let mut rng = thread_rng();
    let array: Vec<i32> = (0..n).map(|_| rng.gen_range(l..r)).collect();
    return array;
}

fn main() {
    let mut size = 10;
    if let Some(arg1) = env::args().nth(1) {
        size = arg1.parse().unwrap_or(10);
    }

    let origin_array = get_random_array(size, 0, 1000);
    let sort_fns = vec![buble_sort, merge_sort, quick_sort];

    for algorithm in sort_fns {
        let mut copy = origin_array.clone();

        // TODO
        //let name = get_function_name(algorithm);
        //println!("Functio name is: {}", name);

        //let _stdout = io::stdout();
        //let sink = io::sink();
        //let _ = io::stdout().flush();
        //let _ = std::io::stdout().write_all(b"\x1b[8m");

        let start_time = Instant::now();

        // TODO: disable print output in every sort... So that can the the proper message of
        // difference of every algorithm
        algorithm(&mut copy);

        let elapsed_time = start_time.elapsed();

        //let _ = std::io::stdout().flush(); // Flush stdout to avoid mixing output
        //let _ = std::io::stdout().write_all(b"\x1b[28m"); // Enable console output (on UNIX-like systems)
        //drop(sink); // Ensure sink is dropped to discard output

        println!("Time elapsed: {:.3?}", elapsed_time);
        println!("----------------------------------");
    }
}
