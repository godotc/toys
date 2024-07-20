use std::{
    fs::File,
    io::{BufReader, Read},
};
use std::io::Write;

fn main() {
    println!("Hello, world!");
    let mut f = File::create("test").unwrap();

    let mut vector = vec![];
    f.read_vectored(&mut vector);

    let rd = BufReader::new(f);

    let a: Vec<u8> = vec![];

}
