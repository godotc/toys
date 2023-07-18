use std::env;

use rand::{thread_rng, Rng};

pub fn merge_sort(array: &mut [i32]) {
    let len = array.len();
    let mid = len / 2;
    if len > 2 {
        merge_sort(&mut array[0..mid + 1]);
        merge_sort(&mut array[mid..len]);
        merge(&mut array[..]);
    } else if len == 2 {
        if array[0] > array[1] {
            array.swap(0, 1);
        }
    } else {
        return;
    }
}

fn merge(array: &mut [i32]) {
    let len = array.len();
    let mid = len / 2;

    let mut arr = Vec::new();
    arr.resize(len, 0);

    let mut i = 0;
    let mut j = mid;
    let mut idx = 0;

    while i < mid && j < len {
        if array[i] < array[j] {
            arr[idx] = array[i];
            i += 1;
        } else {
            arr[idx] = array[j];
            j += 1;
        }
        idx += 1;
    }

    while i < mid {
        arr[idx] = array[i];
        idx += 1;
        i += 1;
    }
    while j < len {
        arr[idx] = array[j];
        idx += 1;
        j += 1;
    }

    for i in 0..len {
        array[i] = arr[i]
    }
    //println!("Merged array: {:?}", array);
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
    println!("=====================================");
    merge_sort(&mut array);
    println!("=====================================");
    println!("Target array: {:?}", array);

    for i in 1..array.len() - 1 {
        assert!(array[i - 1] < array[i + 1])
    }
}
