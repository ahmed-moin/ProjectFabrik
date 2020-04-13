=head1 NAME

Contavia - Hilfsfunktionen für den E-Consulter

=head1 SYNOPSIS

  fehler("xyz konnte nicht ausgeführt werden, weil ...");
  $m->out(formatnumber(1234567.89, 2) . ' EUR');

=head1 FUNCTIONS

=item B<fehler>

löst eine Exception aus. Die übergebene Fehlermeldung wird vom autohandler
extrahiert und dargestellt.

=item B<formatnumber>

Formatiert eine Zahl mit Tausendertrennern und optionalen Nachkommastellen

  $m->out(formatnumber(1234567.89123, 2, 'de') . ' EUR');

ergibt

  1.234.567,89 EUR

Der dritte Parameter ist die Lokalisation, entweder 'de' oder 'en'. Wenn
nichts angegeben ist, wird automatisch 'de' benutzt. Beeinflusst das Komma
und die Tausender Trennzeichen.

=head1 AUTHOR

Copyright 2001, Tommi Mäkitalo, Contavia AG

=cut

package Contavia;

require Exporter;
@ISA = qw(Exporter);

@EXPORT_OK = qw(fehler formatnumber formatpt);

sub fehler
{
  my $msg = shift;
  die "<fehler>$msg</fehler>";
}

sub formatnumber
{
  my ($value, $prec, $lang) = @_;

  $value = sprintf('%.*f', $prec, $value);

  # locale handling
  if ($lang eq 'en') {
    while($value =~ s/^(-?)(\d+)(\d\d\d)/$1$2,$3/) {}
  }
  else {
    $value =~ s/\./,/;

    while($value =~ s/^(-?)(\d+)(\d\d\d)/$1$2.$3/) {}
  }

  return $value;
}

sub formatpt
{
  my ($value) = @_;

  $value = 0 unless $value;

  my $ret = $value < 0 ? '-': '';
  $a = abs($value);
  $ret .= int($a + .01);            # Personentage

  my $h = ($a - int($a)) * 8.0;  # Stunden als float
  if ($h > 0.01)
  {
    my $hh = int($h + .01);             # ganze Stunden
    $ret .= "t$hh";
    my $m = int(($h - $hh) * 60 + .5);  # Minuten gerundet
    $ret .= sprintf(':%02d', $m) if $m > 0;
    $ret .= 'h';
  }

  return $ret;
}
