#get psotgresql

sudo apt-get install postgresql libpqxx3-dev

sudo -u postgres psql < ../src/db/sql/create_db.sql
sudo -u postgres psql < ../src/db/sql/tuna.sql
sudo -u postgres psql < ../src/db/sql/tables.sql

