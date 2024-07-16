use std::{
    fs::File,
    io::{self, BufReader, BufWriter},
};

mod bencode;
mod benobject;
mod torrent_file;

pub(crate) type BT_INT = i32;
pub(crate) type Writer = BufWriter<File>;
pub(crate) type Reader = BufReader<File>;

pub enum BenCodeError {
    DecodeIntBegin,
    DecodeIntString(String),
    DecodeIntEnd,
    IO(io::Error),
    DecodeString,
}

impl From<io::Error> for BenCodeError {
    fn from(value: io::Error) -> Self {
        BenCodeError::IO(value)
    }
}
