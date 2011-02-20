#get psotgresql

sudo apt-get install postgresql

sudo -u postgres psql -f ../src/db/sql/create_db.sql
sudo -u postgres psql -f ../src/db/sql/tuna.sql

