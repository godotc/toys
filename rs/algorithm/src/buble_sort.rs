use rand::{thread_rng, Rng};
use std::env;

pub fn buble_sort(array: &mut [i32]) {
    let len = array.len();
    for i in 0..len {
        for j in i + 1..len {
            if array[i] > array[j] {
                array.swap(i, j)
            }
        }
        //println!("Buble once: {:?}", array);
    }
}

#[test]
fn test() {
    let mut size = 10;
    if let Some(arg1) = env::args().nth(1) {
        size = arg1.parse().unwrap_or(10);
    }

    let mut rng = thread_rng();
    let mut array: Vec<i32> = (0..size).map(|_| rng.gen_range(0..100)).collect();

    println!("Origin array: {:?}", array);
    println!("=====================================");
    buble_sort(&mut array);
    println!("=====================================");
    println!("Target array: {:?}", array);

    for i in 1..array.len() - 1 {
        assert!(array[i - 1] < array[i + 1])
    }
}
