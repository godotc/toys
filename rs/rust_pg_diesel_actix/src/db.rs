use crate::error_handler::CustomError;
use diesel::pg::PgConnection;
use diesel::r2d2::ConnectionManager;
use diesel_migrations::embed_migrations;
use lazy_static::lazy_static;
use r2d2;
use std::env;

type Pool = r2d2::Pool<ConnectionManager<PgConnection>>;
pub type DbConnection = r2d2::PooledConnection<ConnectionManager<PgConnection>>;

embed_migrations!();

lazy_static! {
    static ref POOL: Pool = {
        let db_url = env::var("DATABASE_URL").expect("not set database url");
        let manager = ConnectionManager::<PgConnection>::new(db_url);

        Pool::new(manager).expect("database connection pool failed!")
    };
}

pub fn connection() -> Result<DbConnection, CustomError> {
    POOL.get()
        .map_err(|e| CustomError::new(500, format!("Get database connection failed: {}", e)))
}

pub fn init() {
    lazy_static::initialize(&POOL);
    let conn = connection().expect("Get db connection failed!");
    embed_migrations::run(&conn).unwrap();
}
