use std::net::TcpListener;

#[derive(Debug, Clone)]
pub struct Server {
    bind_hostname: String,
    bind_port: String,
    pub is_running: bool,
}

impl Server {
    pub fn new(bind_hostname: String, bind_port: String) -> Server {
        Server {
            bind_hostname,
            bind_port,
            is_running: false,
        }
    }

    pub fn start_listen(&mut self) -> TcpListener {
        println!("Initializing TCP server...");

        TcpListener::bind(self.bind_hostname.clone() + ":" + self.bind_port.as_str()).unwrap()
    }
}
