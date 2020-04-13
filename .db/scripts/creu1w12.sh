#!/bin/sh

s=`date`

(
  for ((a=1; a<=100; a=a+1))
  do
    echo "
insert into benutzer(ben_uid, ben_password, ben_projekt, ben_personal, ben_anrede,
  ben_vorname, ben_nachname, ben_email)
  values('M${a}', '', 'f', 'f', 'Herr', '', 'M${a}', '');
insert into benutzer(ben_uid, ben_password, ben_projekt, ben_personal, ben_anrede,
  ben_vorname, ben_nachname, ben_email)
  values('W${a}', '', 'f', 'f', 'Frau', '', 'W${a}', '');"
  done
) | psql projekt

e=`date`

echo "Start $s"
echo "Ende  $e"
