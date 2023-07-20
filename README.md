# QtTestProject

Using Qt5 and PostgreSQL@14



### Installation and running
<b>To set up a database on Linux/macOS:</b> \
`sudo apt install postgresql postgresql-contrib` or `brew install postgresql libpq` \
`initdb --locale=C -E UTF-8 -U postgres <database_dir>` \
`pg_ctl -D '<database_dir>' -l logfile start` \
Also add exports during the installation. 

<b>To check the connection:</b> \
`psql postgres` \
`\conninfo`
