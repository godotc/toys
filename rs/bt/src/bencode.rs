use std::io::Write;

use crate::{BenCodeError, BencodeProcess, BencodeType, BtInt};

// fn read_decimal(r: &mut Reader) -> Result<BtInt, BenCodeError> {
//     todo!()
// }
// 


pub fn encode_int(w: &mut impl Write, i: BtInt) -> Result<usize, BenCodeError> {
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

// pub fn decode_int(r: &mut Reader) -> Result<BtInt, BenCodeError> {
//     let mut byte = [0u8; 1];
//     r.read_exact(&mut byte)
//         .expect("Failed to read the begin of int");
//     if b'i' != byte[0] {
//         return Err(BenCodeError::DecodeIntBegin);
//     }
// 
//     let i = read_decimal(r)?;
// 
//     r.read_exact(&mut byte)
//         .expect("Failed to read the end of int");
//     if b'e' != byte[0] {
//         return Err(BenCodeError::DecodeIntEnd);
//     }
// 
//     Ok(i)
// }

pub fn encode_string(w: &mut impl Write, v: &str) -> Result<usize, BenCodeError> {
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

// pub fn DecodeString(r: &mut Reader) -> Result<String, BenCodeError> {
//     let len = read_decimal(r)?;
//     if len <= 0 {
//         // TODO: add more clearly error
//         return Err(BenCodeError::DecodeString);
//     }
// 
//     let mut serpater_byte = [0u8; 1];
//     r.read_exact(&mut serpater_byte)
//         .expect("Failed to read the seprator of a string");
//     if serpater_byte[0] != b':' {
//         // TODO: add more clearly error
//         return Err(BenCodeError::DecodeString);
//     }
// 
//     let mut content = Vec::<u8>::new();
//     content.reserve(len as usize);
//     r.read_exact(&mut content)
//         .expect("Failed to read the seprator of a string");
// 
//     Ok(String::from_utf8_lossy(&content).into_owned())
// }

#[cfg(test)]
mod tests {
    use std::io::{BufWriter, Write};

    use crate::bencode::{encode_int, encode_string};

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

        assert!(encode_int(&mut wt, 123).is_ok());
        wt.write(b"\n").unwrap();

        assert!(encode_string(&mut wt, "hello world").is_ok());

        // wt.flush().expect("WTF"); // will erase  all in buffer

        let buf = wt.buffer();
        println!("{}", String::from_utf8(buf.to_vec()).unwrap());
    }
}