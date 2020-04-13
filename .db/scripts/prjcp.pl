#!/usr/bin/perl -w

use strict;
use DBI;
use Data::Dumper;
use Getopt::Std;

my %opts;
getopts('vqd', \%opts);

our ($prj_alt, $prj_neu, $dbh, $count, $debug, $verbose, $quiet);

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
  # neue Projekt-ID ermitteln
  $prj_neu = $dbh->selectrow_array( "
    select max(prj_id) + 1 from projekt" ) || 1;

  # Tabellen kopieren
  copytab('projekt',                   'prj_id', 0);
  copytab('projektplan',               'ppl_pid', 0);
  copytab('budget',                    'bud_pid', 0);
  copytab('budgetplan',                'bpl_pid', 0);
  copytab('teilprojekt',               'tpr_pid', 0);
  copytab('teilprojektplan',           'tpl_pid', 0);
  copytab('aufgabe',                   'auf_pid', 0);
  copytab('aufgabenplan',              'apl_pid', 0);
  copytab('bombe',                     'bmb_pid', 0);
  copytab('projektziel',               'prz_pid', 0);
  copytab('teilprojektziel',           'tpz_pid', 0);
  copytab('teilprojektagg',            'tpa_pid', 0);
  copytab('teilprojektrealisierung',   'tpe_pid', 0);
  copytab('rolle',                     'rol_pid', 3);

  copytab('prjziel',                   'prj_id',  0);
  copytab('prjbereich',                'prj_id',  0);
  copytab('prjteameigenschaft',        'prj_id',  0);

  # Aufträge lesen
  my $atr = $dbh->selectall_arrayref( "
    select * from auftrag where atr_pid = $prj_alt order by atr_nummer" );

  foreach (@$atr)
  {
    my @values = @$_;

    my $atr_alt = $values[0];

    print "kopiere Auftrag $atr_alt\n" unless $quiet;

    # sicherheitshalber verifizieren wir die Projektid
    die "Fehler beim lesen der Auftagsdaten für Auftrag $atr_alt"
      if ($values[3] != $prj_alt);

    # ermittele nächste verfügbare Auftragsnummer
    my $atr_neu = $dbh->selectrow_array( "
      select max(atr_nummer) + 1 from auftrag" ) || 1;

    # ersetze Auftragnummer und Projektnummer
    $values[0] = $atr_neu;
    $values[3] = $prj_neu;

    inshelper('auftrag', \@values);

    copytab('taetigkeit',     'tae_auftrag', 0, $atr_alt, $atr_neu);
    copytab('kosten',         'kos_auftrag', 0, $atr_alt, $atr_neu);
    copytab('auftragbericht', 'abr_nummer',  0, $atr_alt, $atr_neu);
  }

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
}

$dbh->disconnect if $dbh;

print "$count Sätze zu Projekt $prj_neu kopiert\n" unless $quiet;

sub copytab
{
  my ($tab, $pidnam, $pidcol, $keyalt, $keyneu) = @_;

  print "kopiere $tab\n" unless $quiet;

  $keyalt = $prj_alt unless $keyalt;
  $keyneu = $prj_neu unless $keyneu;

  my $values = $dbh->selectall_arrayref ( "
    select * from $tab where $pidnam = $keyalt" );

  foreach (@$values)
  {
    die "fehler beim kopieren der Tabelle $tab"
      if ($_->[$pidcol] != $keyalt);

    $_->[$pidcol] = $keyneu;
    inshelper($tab, $_);
  }
}

sub inshelper
{
  my ($tab, $values) = @_;

  if ($verbose)
  {
    foreach (@$values)
    {
      print $_ if $_;
      print '|';
    }
    print "\n";
  }

  my $sql = "insert into $tab values(" . '?, ' x (@$values - 1) . '?)';

  print "$sql\n", Dumper($values) if $verbose;

  $dbh->do($sql, {}, @$values);

  ++$count;
}

