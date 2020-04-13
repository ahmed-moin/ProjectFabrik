#!/usr/bin/perl -w

use strict;
use DBI;
use Data::Dumper;
use Getopt::Std;

my %opts;
getopts('vqd', \%opts);

our ($prj_alt, $dbh, $debug, $verbose, $quiet, $count);

$verbose = $opts{v};
$debug = $opts{d};
$quiet = $opts{q};

$prj_alt = $ARGV[0] or die "Projektid muß angegeben werden";

my @tabs =
  qw (
    aufgabe
    aufgabenplan
    auftragbericht
    bombe
    budget
    budgetplan
    kosten
    projekt
    projektplan
    projektziel
    rolle
    taetigkeit
    teilprojekt
    teilprojektagg
    teilprojektplan
    teilprojektrealisierung
    teilprojektziel
  );

# Datenbankverbindung aufbauen
$dbh = DBI->connect( 'dbi:Pg:dbname=projekt', '', '',
  { RaiseError => 1, AutoCommit => 0 } );

eval
{
  print "lösche taetigkeit\n" unless $quiet;
  $count += $dbh->do("
    delete from taetigkeit
     where tae_auftrag in (
       select atr_nummer
         from auftrag
        where atr_pid = $prj_alt)");

  print "lösche kosten\n" unless $quiet;
  $count += $dbh->do("
    delete from kosten
     where kos_auftrag in (
       select atr_nummer
         from auftrag
        where atr_pid = $prj_alt)");

  print "lösche auftragbericht\n" unless $quiet;
  $count += $dbh->do("
    delete from auftragbericht
     where abr_nummer in (
       select atr_nummer
         from auftrag
        where atr_pid = $prj_alt)");

  # Daten löschen
  deltab('auftrag',                   'atr_pid',  0);
  deltab('bombe',                     'bmb_pid', 0);
  deltab('budget',                    'bud_pid', 0);
  deltab('budgetplan',                'bpl_pid', 0);
  deltab('teilprojektplan',           'tpl_pid', 0);
  deltab('aufgabenplan',              'apl_pid', 0);
  deltab('aufgabe',                   'auf_pid', 0);
  deltab('projektziel',               'prz_pid', 0);
  deltab('teilprojektziel',           'tpz_pid', 0);
  deltab('teilprojektagg',            'tpa_pid', 0);
  deltab('teilprojektrealisierung',   'tpe_pid', 0);
  deltab('teilprojekt',               'tpr_pid', 0);
  deltab('rolle',                     'rol_pid', 3);
  deltab('projektplan',               'ppl_pid', 0);

  deltab('prjziel',                   'prj_id',  0);
  deltab('prjbereich',                'prj_id',  0);
  deltab('prjteameigenschaft',        'prj_id',  0);

  deltab('projekt',                   'prj_id',  0);

}; # eval

# Abschluß

if ($@)
{
  $dbh->rollback if $dbh;
  print STDERR $@ if ($@);
}
elsif ($dbh)
{
  if ($debug)
  {
    $dbh->rollback;
  }
  else
  {
    $dbh->commit;
  }

  print "$count Sätze aus Projekt $prj_alt gelöscht\n" unless $quiet;
}

$dbh->disconnect if $dbh;

sub deltab
{
  my ($tab, $pidnam, $pidcol) = @_;

  print "lösche $tab\n" unless $quiet;

  print ("delete from $tab where $pidnam = $prj_alt\n" ) if $verbose;

  $count += $dbh->do ("delete from $tab where $pidnam = $prj_alt" );
}

