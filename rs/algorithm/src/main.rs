mod buble_sort;
mod merge_sort;
mod quick_sort;

use crate::buble_sort::buble_sort;
use crate::merge_sort::merge_sort;
use crate::quick_sort::quick_sort;

use rand::{thread_rng, Rng};
use std::{env, io::stdout};

fn main() {
    let mut size = 10;
    if let Some(arg1) = env::args().nth(1) {
        size = arg1.parse().unwrap_or(10);
    }
    let mut rng = thread_rng();
    let mut array: Vec<i32> = (0..size).map(|_| rng.gen_range(0..100)).collect();

    let sort_fns = vec![buble_sort, merge_sort, quick_sort];

    for i in 0..sort_fns.len() {
        // TODO: disable output when every sort...
        sort_fns[i](&mut array);
    }
}
