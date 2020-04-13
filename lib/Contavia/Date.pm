=head1 NAME

Contavia::Date - Datumsklasse

=head1 SYNOPSIS

  my $date = new Contavia::Date('10.9.2001');
  my $v = $date->voriger_tag;
  my $n = $date->naechster_tag;
  my $vv = $date->voriger_monat;
  my $nn = $date->naechster_monat;
  my $t = $date->tage_im_monat;
  print $date->format;
  printf "<input type='hidden' name='datum' value='%s'>", $date->cgiformat
  $dbh->do("update tabelle set datumsfeld = ? where key = 5",
           {}, $date->qpgdate);
  my $c = $date->cmp(Contavia::Date::current);
  print "Datum liegt in der Zukunft" if $c > 0;
  print "Datum liegt in der Vergangenheit" if $c < 0;
  print "Datum ist Heute" if $c == 0;

=head1 DESCRIPTION

Contavia::Date stellt eine Datumsklasse dar, welches allerlei nützliche
Methoden zur Verfügung stellt, die sich im E-Consulter gebrauchen lassen

=head1 Methoden

=head2 new

Erzeugt eine Datumsklasse.
Das Jahr wird immer 4-Stellig gespeichert. Dabei wird angenommen, daß
eine Jahreszahl weniger als 10 Jahre in der Zukunft liegt.

Bei fehlerhafter Angabe wird eine Ausnahme mit einer entsprechenden
Fehlermeldung ausgelöst. Die Fehlermeldung ist in <fehler>-Tags eingebettet.

Folgende Parameter werden akzeptiert:

=item tt, mm, jj oder jjjj, mm, tt

Tag, Monat und Jahr werden auf die übergebenen Werte gesetzt.
Ist der 1. Parameter >= 1900, wird es als Jahr interpretiert.

=item [tag, monat, jahr] oder [jahr, monat, tag]

Übergabe als Arrayreferenz; Behandlung wie oben.

=item (tag => tt, monat => mm => jahr => jj)

Übergabe als Hashreferenz.

=item jj-mm-tt, mm/tt/jj, tt.mm.jj, jjjjmmttt

Übergabe als String.

=head2 tag monat jahr

Setzt oder liest den Tag, Monat oder das Jahr.

=head2 voriger_tag, naechster_tag, voriger_monat, naechster_monat

berechnet den vorigen/nächsten Tag/Monat und liefert ihn zurück

=head2 tage_im_monat

liefert die Anzahl der Tage des Monats

=head2 datumstage

liefert die Anzahl der Tage sein 1.1.1970

=head2 zinstage

Liefert Zinstage seit 1.1.1970. Dabei hat jeder Monat 30 Tage.

=head2 format

Liefert das Datum im Format t.m.jjjj

=head2 cgiformat

Liefert das Datum als String, welches durch den Konstruktor wieder eingelesen
werden kann. Eignet sich für Datumsangaben in hidden Feldern von Webformularen.

=head2 fixedformat

Liefert das Datum im Format tt.mm.jjjj

=head2 preformat

Liefert das Datum inklusive <pre>-Tags für die HTML-Ausgabe.

=head2 pgdate

Liefert das Datum im Format jjjj-mm-tt. Dieses Format wird von Postgresql
verstanden.

=head2 qpgdate

wie pgdate gequotet oder 'NULL', wenn das Datum nicht gesetzt ist

=head2 convert

Wandelt jeden übergebenen Parameter einzeln in ein Contava::Date-Objekt um
und liefert alle als Array zurück. Beispiel:

  my ($d1, $d2, $d3) = Contavia::Date::convert(
                       '1.1.2001',
                       Contavia::Date::current,
                       [2004, 5, 31] );

=head2 is_null

Prüft, ob das Datum gesetzt ist.

=head2 current

Liefert das aktuelle Datum zurück.

  my $current = Contavia::Date::current;

=head2 cmp

Vergleicht Datumsangaben miteinander. Die übergebenen Parameter werden direkt
an den Konstruktor weitergeleitet, der ein Datumsobjekt erzeugt, so daß
das Vergleichsdatum bei Bedarf erzeugt wird.

Liefert -1 zurück, wenn das aktuelle Objekt kleiner (älter) als das übergebene
Datum ist, 1, wenn es grösser (neuer) als das übergebene oder 0, wenn die
Angaben gleich sind.

=head1 AUTHOR

Copyright 2001, Tommi Mäkitalo, Contavia AG

=cut

package Contavia::Date;

use strict;
use Time::Local;

sub new
{
  shift;

  my ($tag, $monat, $jahr);
  if (scalar @_ == 3)
  {
    ($tag, $monat, $jahr) =
      ($_[0] >= 1900) ? ($_[2], $_[1], $_[0]) : @_;
  }
  elsif (scalar @_ == 1)
  {
    my $datum = shift;
    if (ref($datum) eq 'ARRAY')
    {
      ($tag, $monat, $jahr) =
        ($datum->[0] >= 1900) ? ($datum->[2], $datum->[1], $datum->[0])
                              : @$datum;
    }
    elsif (ref($datum))
    {
      $tag   = $datum->tag;
      $monat = $datum->monat;
      $jahr  = $datum->jahr;
    }
    else
    {
      # Eingabeformate:
      # jahr-monat-tag, monat/tag/jahr, tag.monat.jahr, jjjjmmtt
      ($tag, $monat, $jahr) =
                $datum =~ m{(\d+)-(\d+)-(\d+)} ? ($3 + 0, $2 + 0, $1 + 0)
              : $datum =~ m{(\d+)/(\d+)/(\d+)} ? ($2 + 0, $1 + 0, $3 + 0)
              : $datum =~ m{(\d+)\.(\d+)\.(\d+)} ? ($1 + 0, $2 + 0, $3 + 0)
              : $datum =~ m{(\d\d\d\d)(\d\d)(\d\d)} ? ($3 + 0, $2 + 0, $1 + 0)
              : die "<fehler>ungültiges Datumsformat: $datum</fehler>"
                if defined($datum);
    }
  }

  if ($tag && $monat && $jahr < 100)
  {
    # Jahr < 100 angegeben.
    # Dieser Algorithmus geht davon aus, daß eine Jahreszahl weniger als
    # 10 Jahre in der Zukunft liegt. Bei 2-stelligen Jahreszahlen grösser
    # oder gleich als aktuelles Jahr + 10 wird das vorige Jahrhundert
    # angenommen. (Es wird das aktuelle Jahr 2000-2099 angenommen)
    # z. B. aktuelles Jahr 2001: jahr < 11 => 20xx, jahr >= 11 => 19xx
    #       aktuelles Jahr 2004: jahr < 14 => 20xx, jahr >= 14 => 19xx
    my @l = localtime;
    my $j = $l[5] % 100;
    $jahr += $jahr < $j + 10 ? 2000 : 1900;
  }

  my $self = {
               tag   => $tag,
               monat => $monat,
               jahr  => $jahr
             };

  bless($self);
  return $self;
}

sub tag
{
  my $self = shift;
  if (@_)
  {
    $self->{tag} = shift;
    $self->{time} = undef;
  }
  return $self->{tag};
}

sub monat
{
  my $self = shift;
  if (@_)
  {
    $self->{monat} = shift;
    $self->{time} = undef;
  }
  return $self->{monat};
}

sub jahr
{
  my $self = shift;
  if (@_)
  {
    $self->{jahr} = shift;
    $self->{time} = undef;
  }
  return $self->{jahr};
}

sub voriger_tag
{
  my $self = shift;
  my ($tag, $monat, $jahr) = ($self->{tag}, $self->{monat}, $self->{jahr});
  $self->{time} = timelocal(0, 0, 0, $tag, $monat - 1, $jahr)
    unless $self->{time};
  my @l = localtime($self->{time} - 24 * 60 * 60);
  return Contavia::Date->new($l[3], $l[4] + 1, $l[5] + 1900);
}

sub naechster_tag
{
  my $self = shift;
  my ($tag, $monat, $jahr) = ($self->{tag}, $self->{monat}, $self->{jahr});
  $self->{time} = timelocal(0, 0, 0, $tag, $monat - 1, $jahr)
    unless $self->{time};
  my @l = localtime($self->{time} + 24 * 60 * 60);
  return Contavia::Date->new($l[3], $l[4] + 1, $l[5] + 1900);
}

sub voriger_monat
{
  my $self = shift;
  my ($tag, $monat, $jahr) = ($self->{tag}, $self->{monat}, $self->{jahr});
  if ($monat > 1)
  {
    --$monat
  }
  else
  {
    $monat = 12;
    --$jahr;
  }
  if ($tag > 28)
  {
    # Bei Tagen > 28 machen wir noch eine zusätzliche Prüfung, ob der Tag
    # noch gültig ist. 
    my $t = new Contavia::Date(1, $monat, $jahr)->naechster_monat
                   ->voriger_tag->tag;
    $tag = $t if $tag > $t;
  }
  return Contavia::Date->new($tag, $monat, $jahr);
}

sub naechster_monat
{
  my $self = shift;
  my ($tag, $monat, $jahr) = ($self->{tag}, $self->{monat}, $self->{jahr});
  if ($monat < 12)
  {
    ++$monat
  }
  else
  {
    $monat = 1;
    ++$jahr;
  }
  if ($tag > 28)
  {
    # Bei Tagen > 28 machen wir noch eine zusätzliche Prüfung, ob der Tag
    # noch gültig ist. 
    my $t = new Contavia::Date(1, $monat, $jahr)->naechster_monat
                   ->voriger_tag->tag;
    $tag = $t if $tag > $t;
  }
  return Contavia::Date->new($tag, $monat, $jahr);
}

sub tage_im_monat
{
  my $self = shift;
  my $d = new Contavia::Date(1, $self->{monat}, $self->{jahr})
    ->naechster_monat->voriger_tag;
  return $d->tag;
}

sub datumstage
{
  my $self = shift;

  return undef if is_null($self);

  $self->{time} = timelocal(0, 0, 0, $self->{tag}, $self->{monat} - 1,
                            $self->{jahr})
    unless $self->{time};

  return $self->{time} / (24 * 60 * 60);
}

sub zinstage
{
  my $self = shift;
  is_null($self) ? undef
                 : ($self->{tag} + $self->{monat} * 30 + $self->{jahr} * 360)
}

sub format
{
  my $self = shift;
  return is_null($self) ? undef :
    sprintf('%d.%d.%d', $self->{tag}, $self->{monat}, $self->{jahr})
}

sub cgiformat
{
  my $self = shift;
  return is_null($self) ? undef :
    sprintf('%04d%02d%02d', $self->{jahr}, $self->{monat}, $self->{tag})
}

sub fixedformat
{
  my $self = shift;
  return is_null($self) ? undef :
    sprintf('%02d.%02d.%04d', $self->{tag}, $self->{monat}, $self->{jahr})
}

sub preformat
{
  my $self = shift;
  return is_null($self) ? undef :
    sprintf('<pre>%02d.%02d.%04d</pre>', $self->{tag}, $self->{monat}, $self->{jahr})
}

sub pgdate
{
  my $self = shift;
  return is_null($self) ? undef :
    sprintf('%04d-%02d-%02d', $self->{jahr}, $self->{monat}, $self->{tag});
}

sub qpgdate
{
  my $self = shift;
  if (is_null($self))
  {
    return 'NULL';
  }
  else
  {
    return sprintf('\'%04d-%02d-%02d\'', $self->{jahr}, $self->{monat},
      $self->{tag});
  }
}

sub convert
{
  return map new Contavia::Date($_), @_;
}

sub is_null
{
  my $self = shift;
  return !($self->{tag} && $self->{monat} && $self->{jahr});
}

sub current
{
  my @l = localtime;
  return Contavia::Date->new($l[3], $l[4] + 1, $l[5] + 1900);
}

sub cmp
{
  my $self = shift;
  my $other = new Contavia::Date(@_);
  return $self->is_null || $other->is_null ? undef
       : $other->{jahr}  != $self->{jahr}  ? $self->{jahr}  <=> $other->{jahr}
       : $other->{monat} != $self->{monat} ? $self->{monat} <=> $other->{monat}
       :                                     $self->{tag}   <=> $other->{tag}
}

1;
