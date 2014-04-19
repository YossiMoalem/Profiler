#!/usr/bin/perl
use strict;
use warnings;
use constant AddressDelemeter => ":";
use constant StackHitDelemeter => "=";

my %gStackCounter = ();
my %gAddress2Name = ();
my %gName2Addresss = ();

my $gAppName;
my $gLogFileHnd;

init (@ARGV);
readFile ();
resolveFuncNames ();
printOutput ();

sub readFile 
{
   while (my $curLine = <$gLogFileHnd>)
   {
      chomp $curLine;
      #remove (null), (nil) ...
      $curLine=~ s/\([niul]+\)/0/g;
#                     --Zero Addr : --          | Real Addr    :                 --          =                  hits   
      $curLine =~ /^((0[${\AddressDelemeter()}]?|0x[0-9a-f]+[${\AddressDelemeter()}]?)*)${\StackHitDelemeter()}(\d+)\s*$/ or die "Error: line $curLine does not seems to be a valid profiler log entry";
      my $stack = $1;
      my $hits = $3;

      #count hits:
      if (!defined $gStackCounter{$stack} )
      {
         $gStackCounter{$stack} = 0;
      }
      $gStackCounter{$stack} = $gStackCounter{$stack} + $hits;

      #keep all addresses:
      my @frames = split (AddressDelemeter, $stack);
      foreach my $frame (@frames)
      {
         $gAddress2Name{$frame} = "" unless $frame eq "0";
      }
   }
}

#Resolve the addresses to function names
#if we encounter two addresses to the same function
#replace one with the other, so each function will have
#one, distinct address.
sub resolveFuncNames
{
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
#print "DEBUG: replacing $address with $gName2Addresss{$resolved[0]} \n";
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
}

#print the output.
sub printOutput
{
   while (my ($addr, $name) = each %gAddress2Name)
   {
      print "$addr=$name \n";
   }
   while (my ($stack, $hits) = each %gStackCounter)
   {
      print "$stack=$hits \n";
   }
}

sub init
{
   my $logFilename      = shift;
   $gAppName            = shift;
   my $outputFileName   = shift;

   if (! defined $gAppName)
   {
      print "Usage: $0 <Log file> <executable> [Output file] \n";
      exit -1;
   }
   open $gLogFileHnd, "<", $logFilename if defined  $logFilename;
   die "Cannot open input file $logFilename. Exiting" unless defined $logFilename;

   open my $outputFileHnd, ">>", $outputFileName or die "Cannot open log file $outputFileName" if defined $outputFileName ;
   select ($outputFileHnd) if defined $outputFileHnd;

}
