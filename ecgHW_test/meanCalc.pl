#!/usr/local/bin/perl



my $filename = "ReversedOrder_4Unroll_idle.csv";

open (fh, '<', $filename);

@test = <fh>;
close fh;

my $count = 0;
my $sum = 0;
my $numSamples = 0;

foreach (@test){
	if ($count > 0){
		if ($_ != "\n"){
			my @chars = split(",", $_);
			my $val = $chars[30];	
#			print "${val}  ";
			$sum = $sum + $val;
			$numSamples = $numSamples + 1;
		}
	}
	$count = $count + 1;
}
my $mean = $sum/$numSamples;
print "$mean\n";
