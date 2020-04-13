#!/bin/bash
#
#

DB=${1:-"econsulter_dev"}
DBUSER="e-consulter"

PSQL="/usr/bin/psql"
DROPDB="/usr/bin/dropdb"
CREATEDB="/usr/bin/createdb"
FILEPATH="../"


$DROPDB $DB
$CREATEDB $DB

$PSQL -f $FILEPATH/e-consulter_schema.dump $DB &&
$PSQL -U $DBUSER -f $FILEPATH/e-consulter_aufkat.dump $DB &&
$PSQL -U $DBUSER -f $FILEPATH/e-consulter_auskat.dump $DB &&
$PSQL -U $DBUSER -f $FILEPATH/e-consulter_branchen.dump $DB &&
$PSQL -U $DBUSER -f $FILEPATH/e-consulter_pos.dump $DB &&
$PSQL -U $DBUSER -f $FILEPATH/e-consulter_qualifikationen.dump $DB &&
$PSQL -U $DBUSER -f $FILEPATH/e-consulter_translate.sql $DB &&
$PSQL -U $DBUSER -f $FILEPATH/e-consulter_assfrage.dump $DB &&
$PSQL -U $DBUSER -f $FILEPATH/e-consulter_asskategorie.dump $DB &&
$PSQL -U $DBUSER -f $FILEPATH/e-consulter_prjbereichstamm.dump $DB &&
$PSQL -U $DBUSER -f $FILEPATH/e-consulter_prjteameigenschaftstamm.dump $DB &&
$PSQL -U $DBUSER -f $FILEPATH/e-consulter_prjzielstamm.dump $DB &&
$PSQL -U $DBUSER -f $FILEPATH/e-consulter_necessities.sql $DB

