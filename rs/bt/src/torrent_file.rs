use std::str::Bytes;

struct RawInfo {
    name: String,
    length: i64,
    piece_length: i64,
    piepces: String,
}

struct RawFile {
    announce: String,
    info: RawInfo,
}

const SHALEN: usize = 20;

struct TorrentFile {
    announce: String,
    info_sha: [u8; SHALEN],
    filename: String,
    file_len: i64,
    piece_len: i64,
    piece_sha: Vec<[u8; SHALEN]>,
}
