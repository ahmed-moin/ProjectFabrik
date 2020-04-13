=head1 NAME

Contavia::DBI - erweiterte DBI-Klasse

=head1 SYNOPSIS

  use Contavia::DBI;
  my $dbh = Contavia::DBI->connect('dbi:Pg:dbname=dbname');
  $dbh->insert('tabelle',
    { key  => 5,
      col1 => $dbh->quote($col1),
      col2 => $col2,
      col3 => $col3,
      col4 => $dbh->bool(1) } );
  $dbh->update('tabelle',
    { key1 => 5 },
    { col2 => $col2neu } );
  $dbh->replace('tabelle',
    { key1 => 6 },
    { col1 => $wert1,
      col2 => $wert2 } );
    { col3 => $default3 } );
  my %result = $dbh->select_hash("
    select key1, col1, col2, col3 from tabele where col4 = ?", {},
          $dbh->bool(1) );

=head1 DESCRIPTION

Erweitert die DBI-Klasse um einige nützliche Methoden

=head1 Methoden

Zusätzlich zu den Methoden der Basisklasse DBI stehen diese Methoden
zur Verfügung:

=head2 insert

Führt ein SQL-Insert aus. Nimmt als Parameter den Tabellennamen und eine
Hashreferenz mit Spalten-Wert-Zuordnungen. Die Werte müssen dabei geeignet
formatiert (inklusive quotes) sein. Für Textspalten empfiehlt sich die
Methode DBI::quote.

=head2 update

Führt ein SQL-Update aus. Nimmt als Parameter den Tabellenamen, eine
Hashreferenz mit den Schlüsselwerten und eine Hashreferenz mit den zu
aktualisierenden Werten.

=head2 replace

Führt ein SQL-Update oder SQL-Insert aus. Insert wird ausgeführt, wenn das
Update keine Sätze aktualisiert hat.

Als Parameter wird der Tabellennahme, eine Hashreferenz mit Schlüsselwerten,
eine Hashreferenz mit zu aktualisierenden Werten und optional zusätzliche
Spalten-Werte-Paare für das Insert.

=head1 AUTHOR

Copyright 2001, Tommi Mäkitalo, Contavia AG

=cut

package Contavia::DBI;

require Exporter;
@ISA = qw(Exporter);

use DBI;
use Carp;
use vars qw($AUTOLOAD);

my $Debug = 0;

sub debug
{
  my $self = shift;
  my $level = shift;
  unless (ref($self))
  {
    $Debug = $level
  }
}

sub connect
{
  my $proto = shift;
  my $class = ref($proto) || $proto;
  my $self = {};
  $self->{dbh} = DBI->connect(@_);
  bless ($self, $class);
  return $self;
}

sub AUTOLOAD
{
  my $self = shift;
  my $type = ref($self) or croak "$self is not an object";
  my $method = $AUTOLOAD;
  $method =~ s/.*://;
  $self->{dbh}->$method(@_);
}

sub getsql_insert
{
  my ($dbh, $table, $values) = @_;

  my $first = 1;
  my (@cols, @vals, @assigns);

  while ( my ($col, $value) = each(%$values) )
  {
    push @cols, $col;
    push @vals, defined($value) ? $value : 'null';
    push @assigns, defined($value) ? "$col = $value" : "$col = null";
  }

  my $cols = join(', ', @cols);
  my $vals = join(', ', @vals);
  my $assigns = join(', ', @assigns);

  return "insert into $table($cols)\n values ($vals)\n";
}

sub insert
{
  my $dbh = shift;
  $dbh->do( $dbh->getsql_insert(@_) );
}

sub getsql_update
{
  my ($dbh, $table, $key, $values) = @_;

  my $first = 1;
  my (@keycols, @keyvals, @keyeqs, @cols, @vals, @assigns);

  while ( my ($col, $value) = each(%$key) )
  {
    push @keyeqs, defined($value) ? "$col = $value" : "$col is null";
  }

  while ( my ($col, $value) = each(%$values) )
  {
    push @vals, defined($value) ? $value : 'null';
    push @assigns, defined($value) ? "$col = $value" : "$col = null";;
  }

  my $where = join(' and ', @keyeqs);
  my $vals = join(', ', @vals);
  my $assigns = join(', ', @assigns);

  return "update $table\n set $assigns\n where $where\n";
}

sub update
{
  my $dbh = shift;
  return $dbh->do( $dbh->getsql_update(@_) );
}

sub getsql_replace
{
  my ($dbh, $table, $key, $values, $insvalues) = @_;

  my $first = 1;
  my (@keycols, @keyvals, @keyeqs, @cols, @vals, @assigns);

  while ( my ($col, $value) = each(%$key) )
  {
    push @keycols, $col;   # i
    push @keyvals, defined($value) ? $value : 'null';   # i
    push @keyeqs,  defined($value) ? "$col = $value" : "$col is null";  # u
  }

  while ( my ($col, $value) = each(%$values) )
  {
    push @cols, $col;  # i
    push @vals, defined($value) ? $value : 'null'; # i
    push @assigns, defined($value) ? "$col = $value" : "$col = null"; # u
  }

  if ($insvalues)
  {
    while ( my ($col, $value) = each(%$insvalues) )
    {
      push @cols, $col;                              # i
      push @vals, defined($value) ? "$value" : 'null'; # i
    }
  }

  my $where   = join(' and ', @keyeqs);    # u
  my $assigns = join(', ', @assigns);      # u

  my $keycols = join(', ', @keycols);      # i
  my $keyvals = join(', ', @keyvals);      # i
  my $cols    = join(', ', @cols);         # i
  my $vals    = join(', ', @vals);         # i

  #`echo "update $table\n set $assigns\n where $where" >> /debug`;
  #`echo "insert into $table($keycols, $cols)\n values ($keyvals, $vals)" >> /debug`;

  return ("update $table\n set $assigns\n where $where" ,
          "insert into $table($keycols, $cols)\n values ($keyvals, $vals)");
}

sub replace
{
  my $dbh = shift;
  my ($update, $insert) = $dbh->getsql_replace(@_);
  return $dbh->do($update) > 0 || $dbh->do($insert);
}

sub select_hash
{
  my $self = shift;
  my $sql = shift;
  my $attr = shift;

  my $sth = $self->prepare($sql, $attr);
  $sth->execute(@_);
  my %ret;
  while( my $row = $sth->fetch )
  {
    my @row = @$row;
    my $key = shift @row;
    $ret{$key} = $#row >= 1 ? \@row : $row[0];
  }

  return %ret;
}

sub bool
{
  my $self = shift;
  my $value = shift;
  return $value ? '\'t\'' : '\'f\'';
}

1;
