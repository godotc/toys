use std::io::{self};

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
    pub announce: String,
    pub created_by: String,
    pub creation_date: i64,

    pub announce_list: Vec<Vec<String>>,
    pub info: TorrentFileInfo,

    pub encoding: Option<String>,
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

#[derive(Ord, PartialOrd, PartialEq, Eq)]
enum LogLevel {
    Debug,
    Info,
    Warn,
    Error,
}


impl TorrentFile {
    pub fn from_benobject(benobject: &mut benobject::BenObject) -> Self {
        let mut obj = TorrentFile {
            announce: benobject["announce"].as_string().unwrap().to_string(),
            created_by: benobject["created by"].as_string().unwrap().to_string(),
            creation_date: benobject["creation date"].as_int().unwrap(),
            announce_list: vec![],
            info: TorrentFileInfo::from_benobject(&mut benobject["info"]),
            encoding: None,
        };

        let announce_list = benobject["announce-list"].as_list().unwrap();
        for x in announce_list {
            if x.is_list() {
                let mut vec = vec![];
                x.as_list().unwrap().iter().for_each(|x| {
                    vec.push(x.as_string().unwrap().clone());
                });
                obj.announce_list.push(vec);
            }
        }

        obj
    }
}

impl TorrentFileInfo {
    pub fn from_benobject(benobject: &mut benobject::BenObject) -> Self {
        TorrentFileInfo {
            name: benobject["name"].as_string().unwrap().to_string(),
            files: benobject["files"].as_list().unwrap().iter().as_ref().map(|&mut x| TorrentFileInfoFiles::from_benobject(&mut x)).collect(),
            piece_length: benobject["piece length"].as_int().unwrap(),
            piepces: benobject["pieces"].as_string().unwrap().to_string(),
        }
    }
}

impl TorrentFileInfoFiles {
    pub fn from_benobject(benobject: &mut benobject::BenObject) -> Self {
        TorrentFileInfoFiles {
            length: benobject["length"].as_int().unwrap(),
            path: benobject["path"].as_list().unwrap().iter().map(|x| x.as_string().unwrap().clone()).collect(),
        }
    }
}