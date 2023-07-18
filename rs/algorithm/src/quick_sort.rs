use rand::{thread_rng, Rng};
use std::env;

pub fn quick_sort(array: &mut [i32]) {
    let len = array.len();
    if len >= 2 {
        let pivot = partition(array);
        quick_sort(&mut array[0..pivot]);
        quick_sort(&mut array[pivot + 1..len]);
    }
}

fn partition(array: &mut [i32]) -> usize {
    let len = array.len();
    let mid = len / 2;

    //by index, mit to end
    array.swap(mid, len - 1);

    let mut idx = 0;
    for i in 0..len {
        // compare with the last (mid value)
        if array[i] < array[len - 1] {
            array.swap(i, idx);
            idx += 1;
        }
    }

    // bring the mid value back
    array.swap(idx, len - 1);

    idx
}

#[allow(dead_code)]
fn main() {
    let mut size = 10;
    if let Some(arg1) = env::args().nth(1) {
        size = arg1.parse().unwrap_or(10);
    }

    let mut rng = thread_rng();
    let mut array: Vec<i32> = (0..size).map(|_| rng.gen_range(0..100)).collect();

    println!("Origin array: {:?}", array);
    quick_sort(&mut array);
    println!("Target array: {:?}", array);

    for i in 1..array.len() - 1 {
        assert!(array[i - 1] < array[i + 1])
    }
}
