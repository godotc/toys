use std::io::{self};

mod bencode;
mod benobject;

pub(crate) type BtInt = i64;


struct TorrentFileInfoFiles {
    length: i64,
    path: Vec<String>,
}

struct TorrentFileInfo {
    name: String,
    // length: i64,
    files: Vec<TorrentFileInfoFiles>,

    piece_length: i64,
    piepces: String,
}

struct TorrentFile {
    announce: String,
    created_by: String,
    creation_date: i64,

    announce_list: Vec<Vec<String>>,
    info: TorrentFileInfo,
   
    encoding: Option<String>,
}


#[derive(Debug)]
pub enum BencodeType {
    Source,
    Int,
    String,
    List,
    Diction,
}

#[derive(Debug)]
pub enum BencodeProcess {
    Encode,
    Decode,
}


#[derive(Debug)]
pub enum BenCodeError {
    Bencode {
        type_t: BencodeType,
        process: BencodeProcess,
        msg: String,
    },
    IO(io::Error),
}

impl From<io::Error> for BenCodeError {
    fn from(value: io::Error) -> Self {
        BenCodeError::IO(value)
    }
}

// impl BenCodeError {
//     fn to_string(&self) -> String {
//         match self {
//             BenCodeError::Bencode { type_t, process, msg } => {
//                 format!("Type: {}, Process: {}, Msg: {} ,", type_t, process, msg)
//             }
//             BenCodeError::IO(e) => e.to_string()
//         }
//     }
// }
// 
