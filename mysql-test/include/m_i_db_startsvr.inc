--exec echo "wait" > $MYSQLTEST_VARDIR/tmp/mysqld.1.expect
--shutdown_server 10
--source include/wait_until_disconnected.inc

--perl
use File::Path 'rmtree';
$DDIR=$ENV{"MYSQL_TMP_DIR"}."/installdb_test";
rmtree([ "$DDIR" ]);
EOF

--echo # Execution of mysql_install_db starts............
--exec $MYSQL_INSTALL_DB --no-defaults --datadir=$DDIR --basedir=$BASEDIR --extra-sql-file=$MYSQL_TEST_DIR/t/m_i_db_createschema.sql --defaults-file=$MYSQL_TEST_DIR/t/m_i_db_config.cnf $midparam
--echo # Execution of mysql_install_db ends..............
--exec echo "restart:--datadir=$DDIR " > $MYSQLTEST_VARDIR/tmp/mysqld.1.expect
--enable_reconnect
--source include/wait_until_connected_again.inc

--perl
$passwd="";
open(PWDPATH, "$ENV{MYSQL_TMP_DIR}/readpwdfilepath.txt") or die("Could not open pwdpath file");
my $filename = <PWDPATH>;
chomp $filename;
if (-e $filename) {
  open(PWDFILE, $filename) or die("Could not open password file");
  my @pass = <PWDFILE>;
  chomp @pass;
  $passwd=@pass[1];
}
open(FILE, ">", "$ENV{MYSQL_TMP_DIR}/readpwd.inc") or die $!;
print FILE "delimiter lessprobability;\n";
print FILE "let passwd=$passwd";
print FILE "lessprobability\n";
print FILE "--delimiter ;\n";
close FILE;
EOF
--source $MYSQL_TMP_DIR/readpwd.inc