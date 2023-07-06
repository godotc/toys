use rand::{thread_rng, Rng};

const N: i32 = 10;

fn quick_sort(array: &mut [i32]) {
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

fn main() {
    let mut rng = thread_rng();
    let mut array: Vec<i32> = (0..N).map(|_| rng.gen_range(0..100)).collect();

    println!("Origin array: {:?}", array);
    quick_sort(&mut array);
    println!("Target array: {:?}", array);

    for i in 1..array.len() - 1 {
        assert!(array[i - 1] < array[i + 1])
    }
}
