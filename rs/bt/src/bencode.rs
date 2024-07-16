use std::{
    io::{BufRead, BufReader, Read, Seek, Write},
    str::FromStr,
};

use crate::{BenCodeError, Reader, Writer, BT_INT};

fn read_decimal(r: &mut Reader) -> Result<BT_INT, BenCodeError> {
    todo!()
}

fn write_decimal(w: &mut Writer, i: BT_INT) -> usize {
    w.write(i.to_string().as_bytes())
        .expect("failed to write decimal")
}

pub fn EncodeInt(w: &mut Writer, i: BT_INT) -> Result<usize, BenCodeError> {
    let mut write_len = 0;
    write_len += w.write(b"i").expect("Failed to the the prefix of i");
    write_len += write_decimal(w, i);
    write_len += w.write(b"e").expect("Failed to the the prefix of i");

    w.flush()?;
    Ok(write_len)
}

pub fn DecodeInt(r: &mut Reader) -> Result<BT_INT, BenCodeError> {
    let mut byte = [0u8; 1];
    r.read_exact(&mut byte)
        .expect("Failed to read the begin of int");
    if b'i' != byte[0] {
        return Err(BenCodeError::DecodeIntBegin);
    }

    let i = read_decimal(r)?;

    r.read_exact(&mut byte)
        .expect("Failed to read the end of int");
    if b'e' != byte[0] {
        return Err(BenCodeError::DecodeIntEnd);
    }

    Ok(i)
}

pub fn EncodeString(w: &mut Writer, v: &str) -> Result<usize, BenCodeError> {
    assert!(v.len() < BT_INT::MAX as usize);
    let mut write_len: usize = write_decimal(w, v.len() as BT_INT);

    write_len += w
        .write(format!(":{}", v).as_bytes())
        .expect("Failed to write the successor content of string");
    Ok(write_len)
}

pub fn DecodeString(r: &mut Reader) -> Result<String, BenCodeError> {
    let len = read_decimal(r)?;
    if len <= 0 {
        // TODO: add more clearly error
        return Err(BenCodeError::DecodeString);
    }

    let mut serpater_byte = [0u8; 1];
    r.read_exact(&mut serpater_byte)
        .expect("Failed to read the seprator of a string");
    if serpater_byte[0] != b':' {
        // TODO: add more clearly error
        return Err(BenCodeError::DecodeString);
    }

    let mut content = Vec::<u8>::new();
    content.reserve(len as usize);
    r.read_exact(&mut content)
        .expect("Failed to read the seprator of a string");

    Ok(String::from_utf8_lossy(&content).into_owned())
}

#[cfg(test)]
mod tests {
    use std::{
        fs::File,
        io::{BufWriter, Read, Write},
    };

    #[test]
    fn write_something_from_multi_input() {
        let file = File::create("test").unwrap();
        let mut buf_writer = BufWriter::new(file);

        let input_string = "hello world";

        buf_writer
            .write(format!("{}:{}", input_string.len(), input_string).as_bytes())
            .unwrap();
        buf_writer.flush().unwrap();

        let mut file = File::open("test").unwrap();
        let mut buf = vec![];
        file.read_to_end(&mut buf).unwrap();

        println!("{:#?}", buf);
        println!("{:#?}", String::from_utf8(buf));
    }

    #[test]
    fn int_to_a_asci_string() {
        let number = -1234;

        // Using string concatenation
        let number_string_concat = format!("\"{}\"", number);
        println!("Number as string (concatenation): {}", number_string_concat);

        // Using format! macro
        let number_string_format = format!("\"{}\"", number);
        println!("Number as string (format!): {}", number_string_format);

        assert!(12345.to_string().is_ascii());

        (-12345)
            .to_string()
            .chars()
            .for_each(|c| print!("{}", c.to_ascii_lowercase()));
        println!();

        println!("{}", 0.to_string());
    }
}
