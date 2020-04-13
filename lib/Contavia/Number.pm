package Contavia::Number;

use strict;
use Data::Dumper;

sub new
{
  shift;

  my ($value, $unit, $name);
  if ($#_ == 0)
  {
    $value = shift;
  }
  elsif ($#_ == 2)
  {
    ($value, $unit, $name) = @_;
  }
  elsif ($#_ % 2 == 1)
  {
    my %h = @_;
    $value = $h{value};
    $unit  = $h{unit};
    $name  = $h{name};
  }

  my $self = {
               value => $value,
               unit  => $unit,
               name  => $name,
             };

  bless($self);
  return $self;
}

sub value
{
  my $self = shift;
  $self->{value} = shift if (@_);
  return $self->{value};
}

sub unit
{
  my $self = shift;
  $self->{unit} = shift if (@_);
  return $self->{unit};
}

sub name
{
  my $self = shift;
  $self->{name} = shift if (@_);
  return $self->{name};
}

sub format
{
  my $self = shift;
  return
      is_null($self) ? undef
    : $self->{unit}  ? sprintf("%f %s", $self->{value}, $self->{unit})
    : $self->{value};
}

sub pgformat
{
  my $self = shift;
  return is_null($self) ? undef :
    $self->{value};
}

sub qpgformat
{
  my $self = shift;
  return is_null($self) ? 'NULL'
                        : $self->{value};
}

sub is_null
{
  my $self = shift;
  return ! defined $self->{value};
}

1;
