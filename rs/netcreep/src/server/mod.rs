use std::{sync::Arc, thread};

use crate::server::connection::Connection;

use self::server::Server;

pub mod connection;
pub mod server;

pub struct Bus {
    server: Option<Server>,
}

impl Bus {
    pub fn new() -> Bus {
        Bus { server: None }
    }

    pub fn create_server(&mut self, bind_hostname: String, bind_port: String) {
        self.server = Some(Server::new(bind_hostname, bind_port))
    }

    pub fn listen(&mut self) {
        let server = Arc::new(self.server.as_mut().unwrap());
        let listener = Arc::new(server.start_listen());

        for stream in listener.incoming() {
            println!("New client coming...");
            let stream = stream.unwrap();

            println!("peer addr: {}", stream.peer_addr().unwrap());
            // let mut ctx = Box::new(&self);
            thread::spawn(|| {
                let mut conn = Connection::new(server, stream);
                conn.run();
            });
        }
    }
}
