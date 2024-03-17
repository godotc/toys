use core::time;
use std::{
    io::{BufReader, BufWriter, Read},
    net::TcpStream,
    sync::Arc,
    thread::sleep,
};

use super::server::Server;

pub(crate) struct Connection {
    stream: TcpStream,
    reader: BufReader<TcpStream>,
    writer: BufWriter<TcpStream>,
    context: Arc<Server>,
}

impl Connection {
    pub fn new(ctx: Arc<&mut Server>, stream: TcpStream) -> Connection {
        let r = stream.try_clone().unwrap();
        let w = stream.try_clone().unwrap();

        Connection {
            stream,
            reader: BufReader::new(r),
            writer: BufWriter::new(w),
            context: ctx,
        }
    }

    pub fn run(&mut self) {
        sleep(time::Duration::from_secs(2));
        self.context.is_running = false;

        loop {
            println!("peer addr: {}", self.stream.peer_addr().unwrap());
            // println!("local addr: {}", self.stream.local_addr().unwrap());

            let mut buf = String::new();
            match self.reader.read_to_string(&mut buf) {
                Ok(_) => todo!(),
                Err(_) => todo!(),
            }
        }
    }
}
