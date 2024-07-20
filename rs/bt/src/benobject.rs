use std::{
    collections::HashMap,
    io::Write,
};
use std::fs::File;
use std::io::{BufReader, Read};
use std::ops::Index;

use crate::{bencode, BencodeProcess, BencodeType, BtInt};
use crate::BenCodeError;

// global variable?
static mut PREFIX: String = String::new();


#[repr(u8)]
#[derive(PartialEq, Eq, Debug)]
pub enum BenObject {
    String(String),
    Int(BtInt),
    List(Vec<Box<BenObject>>),
    Dictionary(HashMap<String, Box<BenObject>>),
}

#[derive(Ord, PartialOrd, PartialEq, Eq)]
enum LogLevel {
    Debug,
    Info,
    Warn,
    Error,
}

impl BenObject {
    const LOG_LEVEL: LogLevel = LogLevel::Info;

    pub fn parser_torrent_file(filename: String) -> Self {
        let file = File::open(filename).unwrap();
        let mut rd = BufReader::new(file);
        let mut buf = vec![];
        rd.read_to_end(&mut buf).unwrap();
        let obj = Self::decode(&buf, &mut 0).unwrap();
        assert!(obj.is_dictionary());
        obj
    }

    pub fn encode(&mut self, w: &mut impl Write) -> Result<usize, BenCodeError> {
        let mut write_len = 0;

        match self {
            BenObject::String(str) => write_len += bencode::encode_string(w, str)?,
            BenObject::Int(i) => write_len += bencode::encode_int(w, *i)?,
            BenObject::List(list) => {
                w.write(b"l").expect("Failed to write the prefix of list");
                for elem in list {
                    write_len += elem.encode(w)?;
                }
                w.write(b"e").expect("Failed to write the suffix of list");
                write_len += 2;
            }
            BenObject::Dictionary(dict) => {
                w.write(b"d").expect("Failed to write the the prefix of dict");
                for (k, v) in dict {
                    write_len += bencode::encode_string(w, k)?;
                    write_len += v.encode(w)?;
                }
                w.write(b"e").expect("Failed to write the the suffix of dict");
                write_len += 2;
            }
        }
        w.flush()?;
        Ok(write_len)
    }

    pub fn decode(buf: &Vec<u8>, idx: &mut usize) -> Result<Self, BenCodeError> {
        match buf[*idx] {
            x if x.is_ascii_digit() => BenObject::decode_string(buf, idx),
            b'i' => BenObject::decode_int(buf, idx),
            b'l' => BenObject::decode_list(buf, idx),
            b'd' => BenObject::decode_dictionary(buf, idx),
            _ => Err(BenCodeError::Bencode {
                type_t: BencodeType::Int,
                process: BencodeProcess::Decode,
                msg: format!("Invalid byte: {} -> {}", buf[*idx], buf[*idx] as char),
            })
        }
    }


    fn decode_string(buf: &Vec<u8>, idx: &mut usize) -> Result<BenObject, BenCodeError> {
        debug_assert!(buf[*idx].is_ascii_digit());

        let mut str_len: usize = 0;
        let mut v;

        loop {
            v = buf[*idx];
            if !v.is_ascii_digit() { break; }
            str_len = str_len * 10 + (v - b'0') as usize;
            *idx += 1;
        }

        assert_eq!(v, b':');
        *idx += 1;

        let mut target_string: Vec<u8> = Vec::new();
        target_string.reserve(str_len);

        let r_range = *idx + str_len;
        target_string.append(&mut buf[*idx..r_range].to_vec());
        *idx = r_range; // to next prefix?

        let output_string = String::from_utf8_lossy(target_string.as_slice()).to_string();

        if BenObject::LOG_LEVEL <= LogLevel::Debug {
            unsafe {
                let a = &output_string[0..if output_string.len() > 25 { 25 } else { output_string.len() }];
                print!("{}\t{}", PREFIX, a);
            }
        }

        Ok(BenObject::String(output_string))
    }

    fn decode_int(buf: &Vec<u8>, idx: &mut usize) -> Result<BenObject, BenCodeError> {
        debug_assert_eq!(buf[*idx], b'i');
        *idx += 1;

        let mut numeric_string = String::new();
        let mut v;

        loop {
            v = buf[*idx];
            // dbg!(v as char);
            assert!(v.is_ascii_digit() || v == b'-' || v == b'e');
            if v == b'e' {
                break;
            }
            numeric_string.push(v as char);
            *idx += 1;
        }

        assert_eq!(v, b'e');
        *idx += 1;

        let output_number = numeric_string.parse::<BtInt>().unwrap();

        if BenObject::LOG_LEVEL <= LogLevel::Debug {
            unsafe {
                print!("{}\t{}", PREFIX, output_number);
            }
        }
        return Ok(BenObject::Int(output_number));
    }

    fn decode_list(buf: &Vec<u8>, idx: &mut usize) -> Result<BenObject, BenCodeError> {
        if BenObject::LOG_LEVEL <= LogLevel::Debug {
            unsafe {
                println!("\n{}List:", PREFIX);
                PREFIX.push(' ');
            }
        }

        debug_assert_eq!(buf[*idx], b'l');
        *idx += 1;

        let mut list = vec![];
        let mut v;
        loop {
            v = buf[*idx];
            if v == b'e' {
                break;
            }
            list.push(Box::new(BenObject::decode(buf, idx)?));
            // *idx += 1;
        }
        assert_eq!(v, b'e');
        *idx += 1;

        if BenObject::LOG_LEVEL <= LogLevel::Debug {
            unsafe {
                println!("\n{}End:", PREFIX);
                PREFIX.pop();
            }
        }
        return Ok(BenObject::List(list));
    }

    fn decode_dictionary(buf: &Vec<u8>, idx: &mut usize) -> Result<BenObject, BenCodeError> {
        if BenObject::LOG_LEVEL <= LogLevel::Debug {
            unsafe {
                println!("\n{}Dictionary:", PREFIX);
                PREFIX.push(' ');
            }
        }

        debug_assert_eq!(buf[*idx], b'd');
        *idx += 1;

        let mut dict = std::collections::HashMap::new();

        loop {
            if buf[*idx] == b'e' {
                break;
            }
            let key = match BenObject::decode_string(buf, idx)? {
                BenObject::String(x) => x,
                _ => {
                    return Err(BenCodeError::Bencode {
                        type_t: BencodeType::Diction,
                        process: BencodeProcess::Decode,
                        msg: "Failed to decode one dictionary's key".to_string(),
                    })
                }
            };


            // debug_assert_eq!(buf[*idx - 1], b'e');
            let value = Box::new(BenObject::decode(buf, idx)?);
            dict.insert(key, value);
        }

        assert_eq!(buf[*idx], b'e');
        *idx += 1;
        if BenObject::LOG_LEVEL <= LogLevel::Debug {
            unsafe {
                println!("\n{}End:", PREFIX);
                PREFIX.pop();
            }
        }

        return Ok(BenObject::Dictionary(dict));
    }


    pub fn is_dictionary(&self) -> bool {
        match self {
            BenObject::Dictionary(_) => true,
            _ => false
        }
    }
    pub fn is_string(&self) -> bool {
        match self {
            BenObject::String(_) => true,
            _ => false
        }
    }

    pub fn as_string(&self) -> Option<&String> {
        match self {
            BenObject::String(x) => Some(x),
            _ => None
        }
    }

    pub fn as_int(&self) -> Option<BtInt> {
        match self {
            BenObject::Int(x) => Some(*x),
            _ => None
        }
    }

    pub fn as_list(&self) -> Option<&Vec<Box<BenObject>>> {
        match self {
            BenObject::List(x) => Some(x),
            _ => None
        }
    }

    pub fn as_dictionary(&self) -> Option<&HashMap<String, Box<BenObject>>> {
        match self {
            BenObject::Dictionary(x) => Some(x),
            _ => None
        }
    }
}

impl Index<String> for BenObject {
    type Output = BenObject;
    fn index(&self, index: String) -> &Self::Output {
        match self {
            BenObject::Dictionary(x) => x.get(&index.to_string()).unwrap(),
            _ => panic!("Trying to index a non-dictionary benobject")
        }
    }
}
impl Index<&str> for BenObject {
    type Output = BenObject;
    fn index(&self, index: &str) -> &Self::Output {
        match self {
            BenObject::Dictionary(x) => x.get(index).unwrap(),
            _ => panic!("Trying to index a non-dictionary benobject")
        }
    }
}

impl Index<usize> for BenObject {
    type Output = BenObject;
    fn index(&self, index: usize) -> &Self::Output {
        match self {
            BenObject::List(x) => x.get(index).unwrap(),
            _ => panic!("Trying to index a non-list benobject")
        }
    }
}


#[cfg(test)]
mod tests {
    use std::fs::File;
    use std::io::{BufReader, Read};
    use std::ops::Deref;

    use crate::benobject::BenObject;

    #[test]
    fn test_ref() {
        let a = &b'3';
        let b = b'3';
        assert_eq!(*a, b);
        assert_eq!(a, &b);
        println!("{}", a);
    }

    #[test]
    fn test_iter() {
        let mut i = [1, 2, 3, 4, 5].into_iter();
        i.next();
        i.next();
        assert_ne!(i.peekable().peek().unwrap().deref(), &2);
    }


    #[test]
    fn test_parse_torrent_file() {
        let filename = "tmp/grand_theft_auto_v_repack.torrent";
        let file = File::open(filename).unwrap();
        let mut rd = BufReader::new(file);
        let mut buf = vec![];

        rd.read_to_end(&mut buf).unwrap();


        let mut idx = 0;
        let obj = BenObject::decode(&mut buf, &mut idx).unwrap();

        dbg!(obj);
    }

    #[test]
    fn test_access_announce_from_torrent_file() {
        let filename = "tmp/grand_theft_auto_v_repack.torrent";
        let obj = BenObject::parser_torrent_file(filename.to_string());

        // let err = obj.get_int().expect("not a int");

        // let map = obj.get_dictionary().unwrap();
        // let v = map.get("announce").unwrap();
        let v = &obj["announce"];

        assert!(v.is_string());

        println!("Announce: {}", v.as_string().unwrap());
    }
}