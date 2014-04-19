#!/usr/bin/perl
use strict;
use warnings;

my %gStackCounter = ();
my %gAddress2Name = ();
my %gName2Addresss = ();

my $cDelemeter = ":";

my $gLogFilename = shift;
my $gAppName = shift;
#TODO: check values!

my $gLogFileHnd;

open $gLogFileHnd, "<", $gLogFilename if defined  $gLogFilename;
die "Cannot open input file $gLogFilename. Exiting" unless defined $gLogFilename;
#
#open $gLogFileHnd, ">>", $logFilePath if defined $logFilePath ; 
#print "Cannot open log file. writing to STDOUT \n " unless defined $gLogFileHnd;
#select ($gLogFileHnd) if defined $gLogFileHnd;

while (my $curLine = <$gLogFileHnd>)
{
   chomp $curLine;
#TODO: verify input
   $curLine =~ /^(.+)=(\d+)\s*$/ ;
   my $stack = $1;
   my $hits = $2;
   #remove (null), (nil) ...
   $stack =~ s/\([niul]+\)/0/g;

   #count hits:
   if (!defined $gStackCounter{$stack} )
   {
      $gStackCounter{$stack} = 0;
   }
   $gStackCounter{$stack} = $gStackCounter{$stack} + $hits;

   #keep all addresses:
   my @frames = split ($cDelemeter, $stack);
   foreach my $frame (@frames)
   {
      $gAddress2Name{$frame} = "" unless $frame eq "0";
   }

}

#resolve the addresses to function names:
while (my ($address, $_ ) = each %gAddress2Name)
{
   my @resolved = `addr2line -f -C -e $gAppName  $address`;
   chomp ($resolved[0]);

   if (!defined $gName2Addresss{$resolved[0]})
   {
      # If this is the first time we see this function, add it to the dictionary
      $gAddress2Name{$address} = $resolved[0];
      $gName2Addresss{$resolved[0]} = $address;
   } else {
      # We already got this function. Replace the new address, with the old one
      #print "replacing $address with $gName2Addresss{$resolved[0]} \n";
      foreach my $oldStack (keys  %gStackCounter)
      {
         my $changed = (my $newStack = $oldStack) =~  s/$address/$gName2Addresss{$resolved[0]}/;
         if ( $changed )
         {
            if (!defined $gStackCounter{$newStack} ) 
            {
               $gStackCounter{$newStack} = 0;
            }
            $gStackCounter{$newStack} += delete $gStackCounter{$oldStack};
         }
      }
      my @newStack  = keys %gStackCounter;

      #Create the updated hash
      @gStackCounter{@newStack} = delete @gStackCounter{keys %gStackCounter}; 

      #Remove the old address from the map
      delete $gAddress2Name{$address};
   }
}

#print the output.
while (my ($addr, $name) = each %gAddress2Name)
{
   print "$addr=$name \n";
}
while (my ($stack, $hits) = each %gStackCounter)
{
   print "$stack=$hits \n";
}
