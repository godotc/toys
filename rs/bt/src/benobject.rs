use std::{
    collections::HashMap,
    io::Write,
};
use std::fs::File;
use std::io::{BufReader, Read};
use std::ops::Index;

use crate::{BencodeProcess, BencodeType, BtInt};
use crate::BenCodeError;
use crate::LogLevel;

// global variable?
static mut PREFIX: String = String::new();

struct TokenView<T> {
    buf: Vec<T>,
    pos: usize,
}

impl<T> TokenView<T> {
    fn new(buf: Vec<T>) -> Self {
        Self { buf, pos: 0 }
    }
    fn peek(&self) -> Option<T> {
        self.buf.get(self.pos).cloned()
    }
    // for performance
    fn eat(&mut self) {
        self.pos += 1;
    }

    // i++
    pub fn advance(&mut self) -> Option<T> {
        self.pos += 1;
        self.buf.get(self.pos - 1).cloned()
    }

    fn eat_n(&mut self, n: usize) {
        self.pos += n;
    }

    fn subspan(&self, start: usize, end: usize) -> &mut [T] {
        &mut self.buf[start..end]
    }

    fn pos(&self) -> usize {
        self.pos
    }
}


#[derive(PartialEq, Eq, Debug)]
pub enum BenObject {
    String(String),
    Int(BtInt),
    List(Vec<Box<BenObject>>),
    Dictionary(HashMap<String, Box<BenObject>>),
}


impl BenObject {
    const LOG_LEVEL: LogLevel = LogLevel::Info;

    pub fn parser_torrent_file(filename: String) -> Self {
        let file = File::open(filename).unwrap();
        let mut rd = BufReader::new(file);
        rd.read_to_end(&mut vec![]).unwrap();

        let mut buf = TokenView::new(rd.buffer().to_vec());
        let obj = Self::decode_token_view(&mut buf).unwrap();

        assert!(obj.is_dictionary());
        obj
    }

    pub fn encode(&mut self, w: &mut impl Write) -> Result<usize, BenCodeError> {
        let mut write_len = 0;

        match self {
            BenObject::String(str) => write_len += Self::encode_string(w, str)?,
            BenObject::Int(i) => write_len += Self::encode_int(w, *i)?,
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
                    write_len += Self::encode_string(w, k)?;
                    write_len += v.encode(w)?;
                }
                w.write(b"e").expect("Failed to write the the suffix of dict");
                write_len += 2;
            }
        }
        w.flush()?;
        Ok(write_len)
    }

    fn encode_int(w: &mut impl Write, i: BtInt) -> Result<usize, BenCodeError> {
        match w.write(format!("i{}e", i).as_bytes()) {
            Ok(len) => Ok(len),
            Err(e) => {
                Err(BenCodeError::Bencode {
                    type_t: BencodeType::Int,
                    process: BencodeProcess::Encode,
                    msg: format!("Failed to encode_int: {}", e),
                })
            }
        }
    }

    fn encode_string(w: &mut impl Write, v: &str) -> Result<usize, BenCodeError> {
        assert!(v.len() < BtInt::MAX as usize);
        match w.write(format!("{}:{}", v.len(), v).as_bytes()) {
            Ok(len) => Ok(len),
            Err(e) => {
                Err(BenCodeError::Bencode {
                    type_t: BencodeType::String,
                    process: BencodeProcess::Encode,
                    msg: format!("Failed to encode_string: {}", e),
                })
            }
        }
    }

    pub fn loadfile(filename: String) -> Self {
        let file = File::open(filename).unwrap();
        let mut rd = BufReader::new(file);
        rd.read_to_end(&mut vec![]).unwrap();
        let mut buf = TokenView::new(rd.buffer().to_vec());
        Self::decode_token_view(&mut buf).unwrap()
    }

    pub fn decode_vec(buf: Vec<u8>) -> Result<Self, BenCodeError> {
        let mut buf = TokenView::new(buf);
        Self::decode_token_view(&mut buf)
    }
    pub fn decode_token_view(buf: &mut TokenView<u8>) -> Result<Self, BenCodeError> {
        match buf.peek().unwrap() {
            x if x.is_ascii_digit() => BenObject::decode_string(buf),
            b'i' => BenObject::decode_int(buf),
            b'l' => BenObject::decode_list(buf),
            b'd' => BenObject::decode_dictionary(buf),
            _ => Err(BenCodeError::Bencode {
                type_t: BencodeType::Int,
                process: BencodeProcess::Decode,
                msg: format!("Invalid byte: {} -> {}", buf.peek().unwrap(), buf.peek().unwrap() as char),
            })
        }
    }


    fn decode_string(buf: &mut TokenView<u8>) -> Result<BenObject, BenCodeError> {
        debug_assert!(buf.peek().unwrap().is_ascii_digit());

        let mut str_len: usize = 0;
        let mut v;

        loop {
            v = buf.peek().unwrap();
            if !v.is_ascii_digit() { break; }
            str_len = str_len * 10 + (v - b'0') as usize;
            buf.eat();
        }

        assert_eq!(v, b':');
        buf.eat();

        let mut target_string: Vec<u8> = Vec::new();
        target_string.reserve(str_len);

        let span = buf.subspan(buf.pos(), buf.pos() + str_len);
        target_string.append(span.to_vec().as_mut());
        buf.eat_n(str_len); // to next prefix?

        let output_string = String::from_utf8_lossy(target_string.as_slice()).to_string();

        #[cfg(BenObject::LOG_LEVEL <= LogLevel::Debug)]
        {
            unsafe {
                let a = &output_string[0..if output_string.len() > 25 { 25 } else { output_string.len() }];
                print!("{}\t{}", PREFIX, a);
            }
        }

        Ok(BenObject::String(output_string))
    }

    fn decode_int(buf: &mut TokenView<u8>) -> Result<BenObject, BenCodeError> {
        debug_assert_eq!(buf.peek().unwrap(), b'i');
        buf.eat();

        let mut numeric_string = String::new();
        let mut v;

        loop {
            v = buf.peek().unwrap();
            // dbg!(v as char);
            assert!(v.is_ascii_digit() || v == b'-' || v == b'e');
            if v == b'e' {
                break;
            }
            numeric_string.push(v as char);
            buf.eat();
        }

        assert_eq!(v, b'e');
        buf.eat();

        let output_number = numeric_string.parse::<BtInt>().unwrap();

        #[cfg(BenObject::LOG_LEVEL <= LogLevel::Debug)]
        {
            unsafe {
                print!("{}\t{}", PREFIX, output_number);
            }
        }
        return Ok(BenObject::Int(output_number));
    }

    fn decode_list(buf: &mut TokenView<u8>) -> Result<BenObject, BenCodeError> {
        #[cfg(BenObject::LOG_LEVEL <= LogLevel::Debug)]
        {
            unsafe {
                println!("\n{}List:", PREFIX);
                PREFIX.push(' ');
            }
        }

        debug_assert_eq!(buf.advance().unwrap(), b'l');

        let mut list = vec![];
        let mut v;
        loop {
            v = buf.peek().unwrap();
            if v == b'e' {
                break;
            }
            list.push(Box::new(BenObject::decode_token_view(buf)?));
            // *idx += 1;
        }
        assert_eq!(v, b'e');
        buf.eat();

        #[cfg(BenObject::LOG_LEVEL <= LogLevel::Debug)]
        {
            unsafe {
                println!("\n{}End:", PREFIX);
                PREFIX.pop();
            }
        }
        return Ok(BenObject::List(list));
    }

    fn decode_dictionary(buf: &mut TokenView<u8>) -> Result<BenObject, BenCodeError> {
        #[cfg(BenObject::LOG_LEVEL <= LogLevel::Debug)] {
            unsafe {
                println!("\n{}Dictionary:", PREFIX);
                PREFIX.push(' ');
            }
        }

        debug_assert_eq!(buf.advance().unwrap(), b'd');

        let mut dict = std::collections::HashMap::new();

        loop {
            if buf.peek().unwrap() == b'e' {
                break;
            }
            let key = match BenObject::decode_string(buf)? {
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
            let value = Box::new(BenObject::decode_token_view(buf)?);
            dict.insert(key, value);
        }

        assert_eq!(buf.advance().unwrap(), b'e');

        #[cfg(BenObject::LOG_LEVEL <= LogLevel::Debug)]
        {
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
    pub fn is_list(&self) -> bool {
        match self {
            BenObject::List(_) => true,
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
mod basic_tests {
    use std::io::{BufWriter, Write};

    use crate::benobject::BenObject;

    #[test]
    fn test_leading_zero() {
        assert_eq!(format!("{}", 01234), "1234");
        assert_eq!(format!("{}", -01234), "-1234")
    }

    #[test]
    fn int_to_a_asci_string() {
        let number = -1234;

        let number_string_format = format!("\"{}\"", number);
        println!("Number as string (format!): {}", number_string_format);

        assert_eq!(format!("{}", number), number.to_string());

        assert!(12345.to_string().is_ascii());

        (-12345)
            .to_string()
            .chars()
            .for_each(|c| print!("{}", c.to_ascii_lowercase()));
        println!();

        println!("{}", 0.to_string());
    }


    #[test]
    fn test_encoding() {
        let mut wt = BufWriter::new(vec![]);

        assert!(BenObject::encode_int(&mut wt, 123).is_ok());
        wt.write(b"\n").unwrap();

        assert!(BenObject::encode_string(&mut wt, "hello world").is_ok());

        // wt.flush().expect("WTF"); // will erase  all in buffer

        let buf = wt.buffer();
        println!("{}", String::from_utf8(buf.to_vec()).unwrap());
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
        let obj = BenObject::decode_vec(buf).unwrap();

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

