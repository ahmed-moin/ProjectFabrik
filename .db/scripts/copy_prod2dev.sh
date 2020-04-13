#!/bin/bash
#
#

DBPROD="econsulter"
DBDEV="econsulter_dev"
DBUSER="e-consulter"

PSQL="/usr/bin/psql"
DROPDB="/usr/bin/dropdb"
CREATEDB="/usr/bin/createdb"
PGDUMP="/usr/bin/pg_dump"
FILEPATH="../"


$DROPDB $DBDEV &&
$CREATEDB $DBDEV &&

$PGDUMP $DBPROD | $PSQL $DBDEV
