use std::{
    collections::HashMap,
    fs::File,
    io::{Read, Write},
    iter::Map,
    sync::Arc,
};

use crate::BenCodeError;
use crate::{Reader, Writer, BT_INT};

#[repr(u8)]
enum BenObject {
    String(String) = 0x01,
    Int(BT_INT) = 0x02,
    List(Vec<Box<BenObject>>) = 0x03,
    Dictionary(HashMap<String, Box<BenObject>>) = 0x04,
}

impl BenObject {
    fn BenCode(&mut self, w: &mut Writer) -> Result<usize, BenCodeError> {
        let mut write_len = 0;

        match self {
            BenObject::String(str) => write_len += EncodeString(w, str)?,

            BenObject::Int(i) => write_len += EncodeInt(w, *i)?,

            BenObject::List(list) => {
                write_len += w.write(b"l").expect("Failed to the the prefix of list");
                for elem in list {
                    write_len += elem.BenCode(w)?;
                }
                write_len += w.write(b"e").expect("Failed to the the suffix of list");
            }

            BenObject::Dictionary(dict) => {
                write_len += w.write(b"d").expect("Failed to the the prefix of dict");
                for (k, v) in dict {
                    write_len += EncodeString(w, k)?;
                    write_len += v.BenCode(w)?;
                }
                write_len += w.write(b"e").expect("Failed to the the suffix of dict");
            }
        }

        w.flush()?;

        Ok(write_len)
    }

    fn Parse(&mut self, r: &mut Reader) {
        let mut byte = [0u8; 1];
        r.read_exact(&mut byte);

        match byte {}
    }
}
