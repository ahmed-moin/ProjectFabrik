=head1 NAME

Contavia::Db - Hilfsfunktionen für DBI

=head1 SYNOPSIS

  use Contavia::Db qw(insert update);

=head1 DESCRIPTION

Diese Funktionen sind in der Klasse L<Contavia::DBI> aufgegangen. Sie
stehen aus Kompatibilität zu bestehenden Code weiterhin zur Verfügung

=head1 FUNKTIONEN

=head2 insert

=head2 update

=head2 replace

=head2 quote

=head2 select_hash

=head2 pgboolean

Siehe Contavia::DBI-Klasse. Die Funktionen nehmen als 1. Parameter den
Datenbankhandle auf.

=cut

package Contavia::Db;

require Exporter;
@ISA = qw(Exporter);

@EXPORT_OK = qw(insert update replace quote select_hash select_hashf pgboolean);

sub insert
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

  if ($dbh)
  {
    $dbh->do( "insert into $table($cols) values ($vals)" ) 
  }
  else
  {
    return "insert into $table($cols)\n values ($vals)\n";
  }
}

sub update
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

  if ($dbh)
  {
    return $dbh->do( "update $table set $assigns where $where" );
  }
  else
  {
    return "update $table\n set $assigns\n where $where\n";
  }
}

sub replace
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
      push @vals, defined($value) ? $value : 'null'; # i
    }
  }

  my $where   = join(' and ', @keyeqs);    # u
  my $assigns = join(', ', @assigns);      # u

  my $keycols = join(', ', @keycols);      # i
  my $keyvals = join(', ', @keyvals);      # i
  my $cols    = join(', ', @cols);         # i
  my $vals    = join(', ', @vals);         # i

  #open(FILE, ">/debug") or die("shitfuck");
  print(FILE "update $table set $assigns where $where\n\ninsert into $table($keycols, $cols) values ($keyvals, $vals)");
  close(FILE);

  if ($dbh)
  {
    return $dbh->do( "update $table set $assigns where $where" ) > 0
        || $dbh->do( "insert into $table($keycols, $cols) values ($keyvals, $vals)" );
  }
  else
  {
    return "update $table\n set $assigns\n where $where;\n" .
           "insert into $table($keycols, $cols)\n values ($keyvals, $vals)\n";
  }
}

sub quote
{
  my $str = shift;
  $str =~ s/'/''/g;
  return "'$str'";
}

sub select_hash
{
  my ($dbh, $sql) = @_;

  my $sth = $dbh->prepare($sql);
  $sth->execute;
  my %ret;
  while( my $row = $sth->fetch )
  {
    my @row = @$row;
    my $key = shift @row;
    $ret{$key} = $#row >= 1 ? \@row : $row[0];
  }

  return %ret;
}

sub select_hashf
{
  my $dbh = shift;
  my $sql = shift;
  select_hash($dbh, sprintf($sql, @_));
}

sub pgboolean
{
  my $value = shift;
  return $value ? '\'t\'' : '\'f\'';
}

1;
