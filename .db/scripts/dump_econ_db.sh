#!/bin/bash
#
#

DB=${1:-"econsulter"}
DBUSER="e-consulter"

PSQL="/usr/bin/psql"
DROPDB="/usr/bin/dropdb"
CREATEDB="/usr/bin/createdb"
PGDUMP="/usr/bin/pg_dump"
FILEPATH="../"


$PGDUMP -s -f $FILEPATH/e-consulter_schema.dump $DB
$PGDUMP -aDt aufkat -f $FILEPATH/e-consulter_aufkat.dump $DB
$PGDUMP -aDt auskat -f $FILEPATH/e-consulter_auskat.dump $DB
$PGDUMP -aDt branche -f $FILEPATH/e-consulter_branchen.dump $DB
$PGDUMP -aDt pos -f $FILEPATH/e-consulter_pos.dump $DB
$PGDUMP -aDt qual -f $FILEPATH/e-consulter_qualifikationen.dump $DB
$PGDUMP -aDt translate -f $FILEPATH/e-consulter_translate.sql $DB
$PGDUMP -aDt assfrage -f $FILEPATH/e-consulter_assfrage.dump $DB
$PGDUMP -aDt asskategorie -f $FILEPATH/e-consulter_asskategorie.dump $DB
$PGDUMP -aDt prjbereichstamm -f $FILEPATH/e-consulter_prjbereichstamm.dump $DB
$PGDUMP -aDt prjteameigenschaftstamm -f $FILEPATH/e-consulter_prjteameigenschaftstamm.dump $DB
$PGDUMP -aDt prjzielstamm -f $FILEPATH/e-consulter_prjzielstamm.dump $DB

